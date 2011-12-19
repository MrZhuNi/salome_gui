// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
#include "SUIT_Session.h"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#endif

#include "SUIT_Study.h"
#include "SUIT_Tools.h"
#include "SUIT_MessageBox.h"
#include "SUIT_ExceptionHandler.h"
#include "SUIT_ResourceMgr.h"

#include <QApplication>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QSet>

SUIT_Session* SUIT_Session::mySession = 0;

/*! Constructor.*/

SUIT_Session::SUIT_Session()
: QObject(),
  myResMgr( 0 ),
  myActiveApp( 0 ),
  myHandler( 0 ),
  myExitStatus( NORMAL ),
  myExitFlags ( 0 ),
  myBTimer( 0 ),
#ifndef WIN32
  myBFileFcntl( 0 ),
#endif
  myBFile( 0 )
{
  SUIT_ASSERT( !mySession )

  mySession = this;
}

/*!destructor. Clear applications list and set mySession to zero.*/
SUIT_Session::~SUIT_Session()
{
  for ( AppList::iterator it = myAppList.begin(); it != myAppList.end(); ++it )
    delete *it;

  myAppList.clear();

  if ( myResMgr )
  {
    delete myResMgr;
    myResMgr = 0;
  }
  mySession = 0;

  // remove backup
  if ( myBFile )
    fclose( myBFile );
  if ( !myBFolder.isEmpty() )
    Qtx::rmDir( myBFolder );

  // remove all unused temporary folders
  removeTmpFiles();
}

/*! \retval return mySession */
SUIT_Session* SUIT_Session::session()
{
  return mySession;
}

/*!
  Starts new application using "createApplication" function of loaded DLL.
*/

SUIT_Application* SUIT_Session::startApplication( const QString& name, int /*args*/, char** /*argv*/ )
{
  AppLib libHandle = 0;

  QString appName = applicationName( name );
  if ( myAppLibs.contains( appName ) )
    libHandle = myAppLibs[appName];

  QString lib;
  if ( !libHandle )
    libHandle = loadLibrary( name, lib );

  if ( !libHandle )
  {
    SUIT_MessageBox::warning( 0, tr( "Error" ),
                              tr( "Can not load application library \"%1\": %2").arg( lib ).arg( lastError() ) );
    return 0;
  }

  if ( !myAppLibs.contains( appName ) || !myAppLibs[appName] ) // jfa 22.06.2005
    myAppLibs.insert( appName, libHandle );

  APP_CREATE_FUNC crtInst = 0;

#ifdef WIN32
  crtInst = (APP_CREATE_FUNC)::GetProcAddress( (HINSTANCE)libHandle, APP_CREATE_NAME );
#else
  crtInst = (APP_CREATE_FUNC)dlsym( libHandle, APP_CREATE_NAME );
#endif

  if ( !crtInst )
  {
    SUIT_MessageBox::warning( 0, tr( "Error" ),
                              tr( "Can not find function \"%1\": %2" ).arg( APP_CREATE_NAME ).arg( lastError() ) );
    return 0;
  }

  // Prepare Resource Manager for the new application if it doesn't exist yet
  if ( !myResMgr )
  {
    myResMgr = createResourceMgr( appName );
    myResMgr->loadLanguage();
  }

  //jfa 22.06.2005:SUIT_Application* app = crtInst( args, argv );
  SUIT_Application* app = crtInst();
  if ( !app )
  {
    SUIT_MessageBox::warning( 0, tr( "Error" ), tr( "Can not create application \"%1\": %2").arg( appName ).arg( lastError() ) );
    return 0;
  }

  app->setObjectName( appName );

  insertApplication( app );

  if ( !myHandler )
  {
    APP_GET_HANDLER_FUNC crtHndlr = 0;
#ifdef WIN32
    crtHndlr = (APP_GET_HANDLER_FUNC)::GetProcAddress( (HINSTANCE)libHandle, APP_GET_HANDLER_NAME );
#else
    crtHndlr = (APP_GET_HANDLER_FUNC)dlsym( libHandle, APP_GET_HANDLER_NAME );
#endif
    if ( crtHndlr )
      myHandler = crtHndlr();
  }

  app->start();

  // Application can be closed during starting (not started).
  if ( !myAppList.contains( app ) )
    app = 0;

  if ( !myBTimer )
  {
    QString anHDFName;
    QStringList anArgs = QCoreApplication::arguments();
    QStringList::const_iterator anArgsIt = anArgs.constBegin();
    for ( ; anArgsIt != anArgs.constEnd(); anArgsIt++ )
    {
      QString anArg = *anArgsIt;
      QFileInfo aFileInfo( anArg );
      if ( aFileInfo.exists() && aFileInfo.suffix().toLower() == "hdf" )
      {
        anHDFName = anArg;
        break;
      }
    }

    if ( !anHDFName.isEmpty() && app && app->useFile( anHDFName ) )
    {
      //Remove backup folders if it exists
      QDir tmpDir( QDir::tempPath() );

      QString pref = getBackupPrefix();
      QFileInfoList aFolders = tmpDir.entryInfoList( QStringList() << QString( pref + "*" ),
                                                     QDir::Dirs );

      QFileInfoList::const_iterator aFoldersIt = aFolders.constBegin();
      for ( ; aFoldersIt != aFolders.constEnd(); ++aFoldersIt )
        Qtx::rmDir( (*aFoldersIt).absoluteFilePath() );
    }
    else
    {
      //Try to restore backup
      myBTimer = (QTimer*)1; // block reaction on creation of new application
      restoreBackup();
      myBTimer = 0;
    }

    createBackupTimer();
  }

  return app;
}

/*!
  Gets the list of all applications
*/
QList<SUIT_Application*> SUIT_Session::applications() const
{
  return myAppList;
}

void SUIT_Session::insertApplication( SUIT_Application* app )
{
  if ( !app || myAppList.contains( app ) )
    return;

  myAppList.append( app );

  connect( app, SIGNAL( applicationClosed( SUIT_Application* ) ),
           this, SLOT( onApplicationClosed( SUIT_Application* ) ) );
  connect( app, SIGNAL( activated( SUIT_Application* ) ), 
	         this, SLOT( onApplicationActivated( SUIT_Application* ) ) );
}

/*!
  Returns the active application
*/
SUIT_Application* SUIT_Session::activeApplication() const
{
  /*
  if ( myAppList.count() == 1 )
    return myAppList.getFirst();

  SUIT_Desktop* desktop = 0;
  for ( AppListIterator it( myAppList ); it.current() && !desktop; ++it )
  {
    SUIT_Desktop* desk = it.current()->desktop();
    if ( desk && desk->isActiveWindow() )
      desktop = desk;
  }

  if ( !desktop )
    return 0;

  SUIT_ViewWindow* win = desktop->activeWindow();
  if ( !win || !win->getViewManager() )
    return 0;

  SUIT_Study* study = win->getViewManager()->study();
  if ( !study )
    return 0;

  return study->application();
  */
  return myActiveApp;
}

/*!
  Returns the resource manager for the specified application name.
*/
SUIT_ResourceMgr* SUIT_Session::resourceMgr() const
{
  return myResMgr;
}

/*!
  Removes the application from the list of launched applications.
  If it is a last application the session will be closed.
*/
void SUIT_Session::onApplicationClosed( SUIT_Application* theApp )
{
  emit applicationClosed( theApp );

  myAppList.removeAll( theApp );
  delete theApp;

  if ( theApp == myActiveApp )
    myActiveApp = 0;

  if ( myAppList.isEmpty() )
  {
    //printf( "Calling QApplication::exit() with exit code = %d\n", myExitStatus );
    QApplication::instance()->exit( myExitStatus );
  }
}

/*!
  Destroys session by closing all applications.
*/
void SUIT_Session::closeSession( int mode, int flags )
{
  AppList apps = myAppList;
  for ( AppList::const_iterator it = apps.begin(); it != apps.end(); ++it )
  {
    SUIT_Application* app = *it;
    bool closePermanently;
    if ( mode == ASK && !app->isPossibleToClose( closePermanently ) )
      return;
    else if ( mode == SAVE )
    {
      SUIT_Study* study = app->activeStudy();
      if ( study->isModified() && study->isSaved() )
	      study->saveDocument();
    }
    else if ( mode == DONT_SAVE )
    {
      myExitStatus = FORCED;
    }

    app->closeApplication();
  }

  myExitFlags = flags;
}

/*!
  Get session exit flags.

  By default, exit flags are set to 0. You can use pass any flags to the
  closeSession() method if you need to process them later on application
  quiting.

  \return exit flags
*/
int SUIT_Session::exitFlags() const
{
  return myExitFlags;
}

/*! \retval return myHandler*/
SUIT_ExceptionHandler* SUIT_Session::handler() const
{
  return myHandler;
}

/*! \retval return last error string.*/
QString SUIT_Session::lastError() const
{
  QString str;
#ifdef WIN32
  LPVOID lpMsgBuf;
  ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                   FORMAT_MESSAGE_IGNORE_INSERTS, 0, ::GetLastError(), 0, (LPTSTR)&lpMsgBuf, 0, 0 );
  LPTSTR msg = (LPTSTR)lpMsgBuf;
  str = QString( SUIT_Tools::toQString( msg ) );
  LocalFree( lpMsgBuf );
#else
  str = QString( dlerror() );
#endif
  return str;
}

/*! Load library to session.
 * \retval Loaded library.
 */
SUIT_Session::AppLib SUIT_Session::loadLibrary( const QString& name, QString& libName )
{
  QString libFile = SUIT_Tools::library( name );

  libName = libFile;
  if ( libFile.isEmpty() )
    return 0;

  AppLib lib = 0;
  QByteArray bid = libFile.toLatin1();
#ifdef WIN32
#ifdef UNICODE
  LPTSTR str = (LPTSTR)libFile.utf16();
#else
  LPTSTR str = (LPTSTR)(const char*)bid;
#endif
  lib = ::LoadLibrary( str );
#else
  lib = dlopen( (const char*)libFile.toLatin1(), RTLD_LAZY | RTLD_GLOBAL  );
#endif
  return lib;
}

/*! \retval Return file name by application name.*/
QString SUIT_Session::applicationName( const QString& str ) const
{
#ifdef WIN32
  return SUIT_Tools::file( str, false );
#else
  QString fileName = SUIT_Tools::file( str, false );
  if ( fileName.startsWith( "lib" ) )
    fileName = fileName.right( fileName.length() - 3 );
  return fileName;
#endif
}

/*!
  Virtual method, creates an instance of ResourceManager
*/
SUIT_ResourceMgr* SUIT_Session::createResourceMgr( const QString& appName ) const
{
  return new SUIT_ResourceMgr( applicationName( appName ) );
}

/*!
  Slot, called on activation of some application's desktop
*/
void SUIT_Session::onApplicationActivated( SUIT_Application* app ) 
{
  myActiveApp = app;
}

/*!
  Gets prefix to be used for  creating backup copies
*/
QString SUIT_Session::getBackupPrefix() const
{
#ifdef WNT
  QString usr( getenv( "USERNAME" ) );
#else 
  QString usr( getenv( "USER" ) );
#endif

  // Create folder
  QString anAppName;
  SUIT_Application* app = activeApplication();
  if ( app )
    anAppName = app->applicationName();
  else 
    anAppName = "SALOME";

  QString pref = anAppName + "_backup_" + usr + "_";

  return pref;
}

/*!
  Gets prefix for study saving; this method is used by storage 
  driver and the session to clear useless files after crash.
*/
QString SUIT_Session::getSavePrefix() const
{
#ifdef WNT
  QString usr( getenv( "USERNAME" ) );
#else 
  QString usr( getenv( "USER" ) );
#endif

  // Create folder
  QString anAppName;
  SUIT_Application* app = activeApplication();
  if ( app )
    anAppName = app->applicationName();
  else 
    anAppName = "SALOME";

  QString pref = anAppName + "_study_" + usr + "_";

  return pref;
}

/*!
  Gets backup interval
*/
double SUIT_Session::backupTime() const
{
  double res;
  if ( myBTimer )
    res = myBTimer->interval() / 60. / 10e-3;
  else 
    res = 0;
  return res;
}

/*!
  Sets backup interval; this method is used by application 
  when the interval is changed in preferences
*/
void SUIT_Session::setBackupTime( const double val ) const
{
  int newInt = val * 60 * 1e3;
  if ( !myBTimer || newInt == myBTimer->interval() )
    return;

  myBTimer->setInterval( newInt );
  if ( val <= 0 )
    myBTimer->stop();
  else 
    myBTimer->start();
}

/*!
  Creates timer to be used for backup
*/
void SUIT_Session::createBackupTimer()
{
  if ( myBTimer )
    return;

  SUIT_Application* app = activeApplication();
  if ( !app )
    return;

  myBTimer = new QTimer( this );
  connect( myBTimer, SIGNAL( timeout() ), this, SLOT( onBTimer() ) );

  double mSec = app->getBackupTime() * 60 * 1e3;
  if ( mSec > 0  )
    myBTimer->start( mSec );

  QString pref = QDir::convertSeparators( QDir::tempPath() + "/" + getBackupPrefix() );
  myBFolder = pref;
  int i = 0;
  do
  {
    myBFolder = pref + QString( "%1" ).arg( ++i );
  }
  while( QFileInfo( myBFolder ).exists() );

  if ( !QDir().mkdir( myBFolder ) )
    myBFolder = "";

  if ( !myBFolder.isEmpty() )
  {
    QString used = Qtx::addSlash( myBFolder ) + "used_by_salome";
    myBFile = fopen( used.toLatin1().constData(), "w" );

#ifndef WIN32
    used += ".fcntl";
    myBFileFcntl = fopen( used.toLatin1().constData(), "w" );
#endif
    lockFcntl( QString() );
  }
}

/*
 * Lock theLF or myBFileFcntl if empty
 * returns 0 on success
 */
int SUIT_Session::lockFcntl( QString theLF )
{
#ifdef WIN32
  return 0;
#else
  if ( theLF.isEmpty() && !myBFileFcntl )
    return -2;

  FILE* aFD;
  if ( theLF.isEmpty() )
    aFD = myBFileFcntl;
  else
    aFD = fopen( theLF.toLatin1().constData(), "w" );
  
  struct flock fLock;
  fLock.l_type = F_WRLCK;
  fLock.l_whence = SEEK_SET;
  fLock.l_len = 0;
  fLock.l_start = 0;
  return fcntl( fileno( aFD ), F_SETLK, &fLock );
#endif

}

/*!
  Slot, called when backup interval is out, iterates through all opened 
  applications, creates 'folderName' directories for them and calls app->backup( folderName );
*/
void SUIT_Session::onBTimer()
{
  QApplication::setOverrideCursor( Qt::WaitCursor );

  // clear folder
  Qtx::rmDir( myBFolder );
  QDir().mkdir( myBFolder );
  
  // create backup
  QString aName;
  QList<SUIT_Application*> aList = applications();
  QList<SUIT_Application*>::iterator it;
  for ( it = aList.begin(); it != aList.end(); ++it )
  {
    SUIT_Application* app = *it;
    if ( app && app->activeStudy() )
    {
      aName = app->activeStudy()->studyName();
      aName.replace( ":", "%" );
      aName.replace( "/", "+" );
      aName.replace( "\\", "+" );
      QString aFName = QDir::convertSeparators( myBFolder + "/" + aName );
      QDir().mkdir( aFName );
      QFileInfo fi( aFName );
      if ( fi.exists() &&fi.isDir() )
        app->backup( aFName );
    }
  }

  QApplication::restoreOverrideCursor();
}

/*!
  Restores crashed studies from backup
*/
void SUIT_Session::restoreBackup()
{
  QString pref = getBackupPrefix();

  // checks whether temp folder contains old backups
  QDir tmpDir( QDir::tempPath() );

  QStringList filt;
  filt.append( pref + "*" );
  tmpDir.setNameFilters( filt );

  QStringList sess = tmpDir.entryList ( QDir::Dirs );
  if ( sess.count() == 0  )
    return;

  QSet<QString> stdSet;
  QList<QString> toRestore;
  QList<QString> toRemove;

  // iterate through temp folder
  QStringList::iterator sessIter;
  for ( sessIter = sess.begin(); sessIter != sess.end(); ++sessIter )
  {
    // iterate through session folder
    const QString& stdRoot = Qtx::addSlash( QDir::tempPath() ) + *sessIter;

    // checks whether folder is not currently used
    QString testFile = Qtx::addSlash( stdRoot ) + "used_by_salome";
    QFileInfo fi( testFile );
    if ( fi.exists() )
    {
#ifdef WIN32
      if ( !QFile( testFile ).remove() )
        continue;
#else
      testFile += ".fcntl";
      if ( lockFcntl( testFile ) )
        continue;
#endif
    }
    
    toRemove.append( stdRoot );

    QDir sessDir( stdRoot );
    QStringList stdList = sessDir.entryList ( QDir::Dirs );

    QStringList::iterator stdIt;
    for ( stdIt = stdList.begin(); stdIt != stdList.end(); ++stdIt )
    {
      const QString& locName = *stdIt;
      if ( *stdIt == "." || *stdIt == ".." )
        continue;

      const QString& study = Qtx::addSlash( stdRoot ) + *stdIt;
      QDir stdDir( study );
      QStringList fList = sessDir.entryList ( QDir::AllEntries );
      if ( fList.count() > 2 && !stdSet.contains( locName ) )
      {
        stdSet.insert( locName );
        toRestore.append( study );
      }
    }
  }

  // restore study if necessary
  if ( !toRestore.isEmpty() )
  {
    QWidget* p = activeApplication() ? (QWidget*)activeApplication()->desktop() : 0;
    
    int aBtn = SUIT_MessageBox::warning( p, tr( "WRN_WARNING" ), tr( "WANT_TO_RESTORE" ),
      SUIT_MessageBox::Yes | SUIT_MessageBox::No, SUIT_MessageBox::Yes );
    if ( aBtn == SUIT_MessageBox::Yes )
    {
      QStringList::iterator it;
      bool isFirst = true;
      for ( it = toRestore.begin(); it != toRestore.end(); ++it )
      {
        SUIT_Application* app = activeApplication();
        if ( !app )
          return;

        if ( !isFirst )
          app = startApplication( app->objectName(), 0, 0 );

        isFirst = false;
        
        if ( !app )
          continue;

        app->setRestoreFolder( *it );
        
        QString fName = *it;
        int ind = fName.lastIndexOf( "\\" );
        if ( ind == -1 )
          ind = fName.lastIndexOf( "/" );
        if ( ind >= 0 )
          fName = fName.right( fName.length() - ind -1 );
        fName.replace( "%", ":" );
        fName.replace( "+", "/" );
        fName.replace( "+", QDir::separator() );
	      if ( !app->useFile( fName ) )
	        app->closeApplication();
      }      
    }
  }

  // remove all backup folders
  QStringList::iterator it;
  for ( it = toRemove.begin(); it != toRemove.end(); ++it )
  {
    Qtx::rmDir( *it );
  }

  // remove all unused temporary folders
  removeTmpFiles();
}

/*!
  Remove useless unused files 
*/
void SUIT_Session::removeTmpFiles()
{
  QString savePref = getSavePrefix();
  QDir tmpDir( QDir::tempPath() );

  QStringList filt;
  filt.append( savePref + "*" );
  tmpDir.setNameFilters( filt );  

  QStringList tmpFolders = tmpDir.entryList( QDir::Dirs );
  QStringList::iterator it;
  for ( it = tmpFolders.begin(); it != tmpFolders.end(); ++it )
  {
    // iterate through tmp folders
    const QString& currF = Qtx::addSlash( QDir::tempPath() ) + *it;
    QString blocName = Qtx::addSlash( currF ) + "used_by_salome";


    bool locked;
#ifdef WIN32
    locked = !QFile( blocName ).remove();
#else
      QString testFile = blocName + ".fcntl";
      locked = lockFcntl( testFile );
#endif

    if ( QFileInfo( blocName ).exists() && !locked  )
    {
      // unused non-removed folder
      Qtx::rmDir( currF );
    }
  }
}
