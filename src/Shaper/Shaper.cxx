// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, OPEN CASCADE, CSGROUP
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef DISABLE_PYCONSOLE
#include <Container_init_python.hxx> // this include must be the first one as it includes Python.h
#endif
#include <Utils_SALOME_Exception.hxx>
#include <utilities.h>

#include "GUI_version.h"
#include "Qtx.h"
#include "QtxMsgHandler.h"
#include "QtxSplash.h"
//#include "SALOME_Event.h"
#ifdef USE_SALOME_STYLE
#include "Style_Salome.h"
#endif // USE_SALOME_STYLE
#include "SUIT_Application.h"
#include "SUIT_Desktop.h"
#include "SUIT_ExceptionHandler.h"
#include "SUIT_ResourceMgr.h"
#include "SUIT_Session.h"
#include "SUIT_Tools.h"

#ifdef WIN32
#define sleep _sleep
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#endif
#include <time.h>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QRegExp>
#include <QTextStream>
#include <QWaitCondition>

//! CORBA server for SALOME GUI session
/*!
 * SALOME_Session_Server launches a SALOME session servant.
 * The servant registers to the Naming Service.
 * See SALOME_Session.idl for interface specification.
 * Main services offered by the servant are:
 * - launch GUI
 * - stop Session ( must be idle )
 * - get session state
 *
 * Also, session server:
 * - reads arguments,
 * - defines list of embedded services to launch with their arguments,
 * - defines list of standalone services to connect/wait,
 * - waits for naming service to finish its initalization,
 * - creates and runs a separate thread to launch/initialize all services.
 */

#include "SalomeApp_Application.h"


namespace myAppNameSpace{
  const QString myAppName("ShaperApp");
  const QString myLightAppName("LightShaperApp");
 }

namespace
{
  //! Custom handler to manage Qt messages
  class MsgHandler: public QtxMsgHandlerCallback
  {
  public:
    MsgHandler() {}
    void qtMessage( QtMsgType type, const QMessageLogContext& /*context*/, const QString& message )
    {
      (void)message; // unused in debug mode
      switch ( type )
      {
        case QtDebugMsg:
#ifdef QT_DEBUG_MESSAGE
          MESSAGE( "Debug: " << qPrintable( message ) );
#endif
          break;
        case QtWarningMsg:
          MESSAGE( "Warning: " << qPrintable( message ) );
          break;
        case QtCriticalMsg:
          MESSAGE( "Critical: " << qPrintable( message ) );
          break;
        case QtFatalMsg:
          MESSAGE( "Fatal: " << qPrintable( message ) );
          break;
        case QtInfoMsg:
        default:
          MESSAGE( "Information: " << qPrintable( message ) );
          break;
      }
    }
  };

  //! Get version of SALOME GUI module
  QString salomeVersion()
  {
    return GUI_VERSION_STR;
  }

  //! Custom resources manager, that allows customization of application name/version
  //  via configuration/translation files.
  class ResourceMgr : public SUIT_ResourceMgr
  {
  public:
    ResourceMgr( const QString& appName = myAppNameSpace::myAppName ) : SUIT_ResourceMgr( appName, "%1Config" )
    {
      customize(); // activate customization
      setCurrentFormat( "xml" );
      setOption( "translators", QString( "%P_msg_%L.qm|%P_icons.qm|%P_images.qm" ) );
    }

    QString customName() const { return myCustomAppName; }
    QString version() const { return myCustomAppVersion; }

  private:
    static void customize()
    {
      // Try to retrieve actual application name and version from translation files.
      // We create temporary resource manager and load LightApp and ShaperApp translations.
      // This procedure is supposed to be done only once, at first call.
      if ( myCustomAppName.isNull() ) {
        SUIT_ResourceMgr mgr( myAppNameSpace::myAppName, "%1Config" );
        mgr.setCurrentFormat( "xml" );
        mgr.setWorkingMode( IgnoreUserValues ); // prevent reading data from user's file
        mgr.loadLanguage( myAppNameSpace::myLightAppName,  "en" );
        mgr.loadLanguage( myAppNameSpace::myAppName, "en" );

        // actual application name can be customized via APP_NAME resource key
        myCustomAppName = QObject::tr( "APP_NAME" ).trimmed();
        if ( myCustomAppName == "APP_NAME" || myCustomAppName.toLower() == "shaper" )
          myCustomAppName = myAppNameSpace::myAppName; // fallback name

        // actual application name can be customized via APP_VERSION resource key
        myCustomAppVersion = QObject::tr( "APP_VERSION" ).trimmed();
        if ( myCustomAppVersion == "APP_VERSION" )
          myCustomAppVersion = myCustomAppName == myAppNameSpace::myAppName ? salomeVersion() : ""; // fallback version
      }
    }

  protected:
    QString userFileName( const QString& /*appName*/, const bool forLoad ) const
    {
      if ( version().isEmpty() ) return "";
      return SUIT_ResourceMgr::userFileName( myCustomAppName, forLoad );
    }

    virtual long userFileId( const QString& _fname ) const
    {
      //////////////////////////////////////////////////////////////////////////////////////////////
      // In SALOME and SALOME-based applications the user preferences file is named as
      // - <AppName>.xml.<AppVersion> on Windows
      // - <AppName>rc.<AppVersion> on Linux
      // where
      //   * AppName is application name, defaults to SalomeApp. Can be customized in SALOME-based
      //     applications, see ResourceMgr above for more details.
      //   * AppVersion is application version, defaults to current version of SALOME GUI module
      //     if AppName is not customize, otherwise empty. Can be customized in SALOME-based
      //     applications, see ResourceMgr above for more details.
      //
      // Since version 6.5.0 of SALOME, user file is stored in the ~/.config/salome
      // directory. For backward compatibility, when user preferences from nearest
      // version of application is searched, user home directory is also looked through,
      // with lower priority.
      //
      // Since version 6.6.0 of SALOME, user file name on Linux is no more prefixed by dot
      // symbol since it is stored in the hidden ~/.config/salome directory. However, dot-prefixed
      // files are also taken into account (with lower priority) for backward compatibility.
      //
      // Notes:
      // - Currently the following format of version number is supported:
      //   <major>[.<minor>[.<release>[<type><dev>]]]
      //   Parts in square brackets are considered optional. Here:
      //   * major   - major version id
      //   * minor   - minor version id
      //   * release - maintenance version id
      //   * type    - dev or patch marker; it can be either one alphabetical symbol (from 'a' to 'z')
      //               or 'rc' to point release candidate (case-insensitive)
      //   * dev     - dev version or patch number
      //   All numerical values must be of range [1-99].
      //   Examples: 1.0, 6.5.0, 1.2.0a1, 3.3.3rc3 (release candidate 3), 11.0.0p1 (patch 1)
      //
      // - Versioning approach can be customized by implementing and using own resource manager class,
      //   see QtxResurceMgr, SUIT_ResourceMgr classes, and ResourceMgr class above in this file.
      //////////////////////////////////////////////////////////////////////////////////////////////

      long id = -1;
      if ( !myCustomAppName.isEmpty() ) {
#ifdef WIN32
        // On Windows, user file name is something like SalomeApp.xml.6.5.0 where
        // - SalomeApp is an application name (can be customized)
        // - xml is a file format (xml or ini)
        // - 6.5.0 is an application version, can include alfa/beta/rc marks, e.g. 6.5.0a3, 6.5.0rc1
        QRegExp exp( QString( "%1\\.%2\\.([a-zA-Z0-9.]+)" ).arg( myCustomAppName ).arg( currentFormat() ) );
#else
        // On Linux, user file name is something like SalomeApprc.6.5.0 where
        // - SalomeApp is an application name (can be customized)
        // - 6.5.0 is an application version, can include alfa/beta/rc marks, e.g. 6.5.0a3, 6.5.0rc1

        // VSR 24/09/2012: issue 0021781: since version 6.6.0 user filename is not prepended with "."
        // when it is stored in the ~/.config/<appname> directory;
        // for backward compatibility we also check files prepended with "." with lower priority
        QRegExp exp( QString( "\\.?%1rc\\.([a-zA-Z0-9.]+)" ).arg( myCustomAppName ) );
#endif
        QString fname = QFileInfo( _fname ).fileName();
        if ( exp.exactMatch( fname ) ) {
          long fid = Qtx::versionToId( exp.cap( 1 ) );
          if ( fid > 0 ) id = fid;
        }
      }
      return id;
    }

  private:
    static QString myCustomAppName;
    static QString myCustomAppVersion;
  };
  QString ResourceMgr::myCustomAppName;
  QString ResourceMgr::myCustomAppVersion;

  //! Custom session, to use custom resource manager class.
  class Session : public SUIT_Session
  {
  public:
    virtual SUIT_ResourceMgr* createResourceMgr( const QString& appName ) const
    {
      return new ResourceMgr( appName );
    }
  };

  //! Custom QApplication class, redefines notify() method, to transfer all events
  //  via centralized exception handling mechanism.
  class Application : public QApplication
  {
  public:
    Application( int& argc, char** argv )
      : QApplication( argc, argv ),
        myHandler ( 0 )
    {
      myDebug = !Qtx::getenv( "SALOME_DEBUG_EXCEPTIONS" ).isEmpty();
    }

    virtual bool notify( QObject* receiver, QEvent* e )
    {
      if ( myDebug || !myHandler ) {
        return QApplication::notify( receiver, e );
      }
      else {
        try {
          return myHandler->handle( receiver, e );
        }
        catch ( std::exception& e ) {
          std::cerr << "notify(): Caught exception : " << e.what() << std::endl;
        }
        catch (...) {
          std::cerr << "notify(): Caught unknown exception : there's probably a bug in SALOME platform" << std::endl;
        }
        return false;  // return false when exception is caught
      }
    }

    SUIT_ExceptionHandler* handler() const { return myHandler; }
    void setHandler( SUIT_ExceptionHandler* h ) { myHandler = h; }

  private:
    SUIT_ExceptionHandler* myHandler;
    bool myDebug;
  };

  //! Checks command line for presense of given option(s).
  //  Option that results to \c true is specified via \a trueOption parameter.
  //  Option that results to \c false is specified via \a falseOption parameter (empty for default).
  //  Default value for the result (returned if both \a trueOption \a falseOption are not given) is specified via \c defValue parameter.
  bool boolCmdOption( const QString trueOption, const QString falseOption = QString(), bool defValue = false )
  {
    bool value = defValue;

    QStringList args = QApplication::arguments();
    foreach ( QString arg, args )
    {
      if ( arg == trueOption )
        value = true;
      else if ( arg == falseOption )
        value = false;
    }
    return value;
  }

} // end of anonymous namespace

// ---------------------------- MAIN -----------------------
int main( int argc, char **argv )
{
  if( myAppNameSpace::myAppName == "ShaperApp")
    qputenv(QString(myAppNameSpace::myAppName + "Config").toUtf8().data(),
            qgetenv("SalomeAppConfig"));

  // Set-up application settings configuration (as for QSettings)
  // Note: these are default settings which can be customized (see below)
  QApplication::setOrganizationName( "salome" );
  QApplication::setApplicationName( "shaper" );
  QApplication::setApplicationVersion( salomeVersion() );

  // Install Qt debug messages handler
  MsgHandler msgHandler;
  qInstallMessageHandler( QtxMsgHandler );

  // Add <qtdir>/plugins dir to the pluins search path for image plugins
  QString qtdir = Qtx::qtDir( "plugins" );
  if ( !qtdir.isEmpty() )
    QApplication::addLibraryPath( qtdir );

  // Add application library path (to search style plugin etc...)

  QString path = SUIT_Tools::addSlash( Qtx::getenv( "GUI_ROOT_DIR" ) ) + "bin/salome";
  QApplication::addLibraryPath( QDir::toNativeSeparators( path ) );

  // QSurfaceFormat should be set before creation of QApplication,
  // so to avoid conflicts beetween SALOME and ParaView QSurfaceFormats we should merge theirs formats
  // (see void Qtx::initDefaultSurfaceFormat()) and set the resultant format here.
  Qtx::initDefaultSurfaceFormat();

  // Create Qt application instance: this should be done as early as possible!
  // Note: QApplication forces setting locale LC_ALL to system one: setlocale(LC_ALL, "").
  Application app( argc, argv );

  // Initialize Python (only once)
  // Note: Python forces setting locale LC_CTYPE to system one: setlocale(LC_CTYPE, "").
#ifndef  DISABLE_PYCONSOLE
  char* py_argv[] = {(char*)""};
  KERNEL_PYTHON::init_python( 1, py_argv );
#endif
  // Create auxiliary resource manager to access application settings
  ResourceMgr resMgr;
  resMgr.setWorkingMode( ResourceMgr::IgnoreUserValues );
  resMgr.loadLanguage( myAppNameSpace::myLightAppName, "en" );
  resMgr.loadLanguage( myAppNameSpace::myAppName, "en" );
  resMgr.loadLanguage( "Session" );

  // Set-up application settings configuration possible customized via resources
  if ( resMgr.customName() != myAppNameSpace::myAppName ) {
    QApplication::setApplicationName( resMgr.customName() );
    QApplication::setApplicationVersion( resMgr.version() );
  }

  // Force default "C" locale if requested via user's preferences
  // Note: this does not change whole application locale (changed via setlocale() function),
  // but only affects GUI behavior
  resMgr.setWorkingMode( ResourceMgr::AllowUserValues ); // we must take into account user preferences
  if ( resMgr.booleanValue( "language", "locale", true ) )
    QLocale::setDefault( QLocale::c() );
  resMgr.setWorkingMode( ResourceMgr::IgnoreUserValues );

  bool isGUI    = boolCmdOption( "--show-desktop", "--hide-desktop", true ); // true by default
  bool isSplash = boolCmdOption( "--show-splash", "--hide-splash", true ); // true by default

  // Show splash screen (only if both the "GUI" and "SPLASH" options are true)
  QtxSplash* splash = 0;
  if ( isGUI && isSplash ) {
    splash = QtxSplash::splash( QPixmap() );
    splash->readSettings( &resMgr );
    if ( splash->pixmap().isNull() )
      splash->setPixmap( resMgr.loadPixmap(myAppNameSpace::myLightAppName, QObject::tr( "ABOUT_SPLASH" ) ) );
    if ( splash->pixmap().isNull() ) {
      delete splash;
      splash = 0;
    }
    else {
      splash->setOption( "%A", QObject::tr( "APP_NAME" ) );
      splash->setOption( "%V", QObject::tr( "ABOUT_VERSION" ).arg( resMgr.version() ) );
      splash->setOption( "%L", QObject::tr( "ABOUT_LICENSE" ) );
      splash->setOption( "%C", QObject::tr( "ABOUT_COPYRIGHT" ) );
      splash->show();
      QApplication::instance()->processEvents();
    }
  }

  // Initialization
  int result = -1;

  SUIT_Session* aGUISession = 0;

#if defined(WIN32) && defined(UNICODE)
  char** new_argv = NULL;
#endif

  bool shutdownAll = false;
  bool debugExceptions = boolCmdOption( "--no-exception-handler" ) ||
      resMgr.booleanValue( "launch", "noexcepthandler", false );

  while ( true ) {
    // SUIT_Session creation
    aGUISession = new Session();

    MESSAGE( "creation SUIT_Application" );

    // Fill resources configuration file in session
    aGUISession->setResourcesConfigPrefix(myAppNameSpace::myLightAppName);

    aGUISession->resourceMgr() = new ResourceMgr( myAppNameSpace::myAppName );
    aGUISession->resourceMgr()->loadLanguage();

    SalomeApp_Application* aGUIApp = new SalomeApp_Application();
    aGUIApp->setObjectName(myAppNameSpace::myAppName);
    aGUISession->insertApplication(aGUIApp);
    aGUIApp->start();

    // Fill the executable name in session
    aGUISession->setExecutableAppName(aGUISession->shaperAppName());
    if ( aGUIApp )

    {
#ifdef USE_SALOME_STYLE
      Style_Salome::initialize( aGUIApp->resourceMgr() );
      if ( aGUIApp->resourceMgr()->booleanValue( "Style", "use_salome_style", true ) )
        Style_Salome::apply();
#endif // USE_SALOME_STYLE

      if ( !debugExceptions )
        app.setHandler( aGUISession->handler() ); // after loading SalomeApp application
      // aGUISession contains SalomeApp_ExceptionHandler

      // Run GUI loop
      MESSAGE( "run(): starting the main event loop" );

      if ( splash )
        splash->finish( aGUIApp->desktop() );

      result = app.exec();

      splash = 0;

      if ( result == SUIT_Session::NORMAL ) {
        // desktop is explicitly closed by user from GUI
        // exit flags says if it's necessary to shutdown all servers
        // all session server only
        shutdownAll = aGUISession->exitFlags();
      }

    }
    break;
    delete aGUISession;
    aGUISession = 0;

  }

  delete aGUISession;

#if defined(WIN32) && defined(UNICODE)
  delete[] new_argv;
#endif
#ifndef DISABLE_PYCONSOLE
  // Finalize Python
  if ( Py_IsInitialized() )
  {
    PyGILState_Ensure();
    Py_Finalize();
  }
#endif
  MESSAGE( "shaper: end" );

  return result;
}
