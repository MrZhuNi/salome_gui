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
// File:      LightApp_Application.cxx
// Created:   6/20/2005 18:39:45 PM
// Author:    Natalia Donis
// Copyright (C) CEA 2005

#ifndef DISABLE_PYCONSOLE
  #include <PyConsole_Interp.h> // WARNING! This include must be the first!
  #include <PyConsole_Console.h>
#endif

#include "LightApp_Application.h"
#include "LightApp_Module.h"
#include "LightApp_DataModel.h"
#include "LightApp_Study.h"
#include "LightApp_Preferences.h"
#include "LightApp_PreferencesDlg.h"
#include "LightApp_ModuleDlg.h"
#include "LightApp_AboutDlg.h"
#include "LightApp_ModuleAction.h"
// temporary commented
//#include "LightApp_OBFilter.h"
#include "LightApp_EventFilter.h"
#include "LightApp_OBSelector.h"
#include "LightApp_SelectionMgr.h"
#include "LightApp_DataObject.h"

#include <SALOME_Event.h>

#include <Style_Model.h>
#include <Style_Salome.h>

#include <CAM_Module.h>
#include <CAM_DataModel.h>
#include <CAM_Study.h>
#include <STD_TabDesktop.h>

#include <SUIT_DataBrowser.h>
#include <SUIT_Session.h>
#include <SUIT_Study.h>
#include <SUIT_FileDlg.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Tools.h>
#include <SUIT_Accel.h>
#include <SUIT_MessageBox.h>
#include <SUIT_ViewWindow.h>

#include <Qtx.h>
#include <QtxToolBar.h>
#include <QtxTreeView.h>
#include <QtxMRUAction.h>
#include <QtxDockAction.h>
#include <QtxDockWidget.h>
#include <QtxActionToolMgr.h>
#include <QtxSearchTool.h>

#include <LogWindow.h>

#ifndef DISABLE_GLVIEWER
  #include <GLViewer_Viewer.h>
  #include <GLViewer_ViewManager.h>
  #include "LightApp_GLSelector.h"
#endif

#ifndef DISABLE_PLOT2DVIEWER
  #include <Plot2d_ViewManager.h>
  #include <Plot2d_ViewModel.h>
#ifndef DISABLE_SALOMEOBJECT
  #include <SPlot2d_ViewModel.h>
#else
  #include <Plot2d_ViewModel.h>
#endif
#endif

#ifndef DISABLE_OCCVIEWER
  #include <OCCViewer_ViewManager.h>
#ifndef DISABLE_SALOMEOBJECT
  #include <SOCC_ViewModel.h>
#else
  #include <OCCViewer_ViewModel.h>
#endif
  #include "LightApp_OCCSelector.h"
#endif

#ifndef DISABLE_VTKVIEWER
#ifndef DISABLE_SALOMEOBJECT
  #include <SVTK_ViewModel.h>
  #include <SVTK_ViewManager.h>
  #include "LightApp_VTKSelector.h"
#else
  #include <VTKViewer_ViewModel.h>
  #include <VTKViewer_ViewManager.h>
#endif
  #include <VTKViewer_ViewModel.h>
#endif

//#ifndef DISABLE_SUPERVGRAPHVIEWER
//  #include <SUPERVGraph_ViewModel.h>
//  #include <SUPERVGraph_ViewFrame.h>
//  #include <SUPERVGraph_ViewManager.h>
//#endif

#ifndef DISABLE_QXGRAPHVIEWER
  #include <QxGraph_ViewModel.h>
  #include <QxGraph_ViewWindow.h>
  #include <QxGraph_ViewManager.h>
#endif

#include <QDir>
#include <QImage>
#include <QString>
#include <QWidget>
#include <QStringList>
#include <QFile>
#include <QApplication>
#include <QMap>
#include <QStatusBar>
#include <QThread>
#include <QObjectList>
#include <QComboBox>
#include <QInputDialog>
#include <QFontDatabase>
#include <QIcon>
#include <QByteArray>
#include <QMenu>
#include <QProcess>

#define FIRST_HELP_ID 1000000

#ifndef DISABLE_SALOMEOBJECT
  #include <SALOME_InteractiveObject.hxx>
  #include <SALOME_ListIO.hxx>
#endif

#define ToolBarMarker    0
#define DockWidgetMarker 1

static const char* imageEmptyIcon[] = {
"20 20 1 1",
". 	c None",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"...................."};

int LightApp_Application::lastStudyId = 0;

/*!
  \return last global id of study
*/
int LightApp_Application::studyId()
{
  return LightApp_Application::lastStudyId;
}

/*!Create new instance of LightApp_Application.*/
extern "C" LIGHTAPP_EXPORT SUIT_Application* createApplication()
{
  return new LightApp_Application();
}

/*! \var global preferences of LightApp */
LightApp_Preferences* LightApp_Application::_prefs_ = 0;

/*!
  \class LightApp_Application
  Application containing LightApp module
*/

/*!Constructor.*/
LightApp_Application::LightApp_Application()
: CAM_Application( false ),
  myPrefs( 0 )
{
  STD_TabDesktop* desk = new STD_TabDesktop();

  setDesktop( desk );

  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  QPixmap aLogo = aResMgr->loadPixmap( "LightApp", tr( "APP_DEFAULT_ICO" ), false );

  desktop()->setWindowIcon( aLogo );
  desktop()->setDockableMenuBar( false );
  desktop()->setDockableStatusBar( false );

  // base logo (salome itself)
  desktop()->logoInsert( "_app_base",  aResMgr->loadPixmap( "LightApp", tr( "APP_BASE_LOGO" ), false ) );
  // extra logo (salome-based application)
  desktop()->logoInsert( "_app_extra", aResMgr->loadPixmap( "LightApp", tr( "APP_EXTRA_LOGO" ), false ) );

  clearViewManagers();

  mySelMgr = new LightApp_SelectionMgr( this );

  myAccel = SUIT_Accel::getAccel();

#ifndef DISABLE_OCCVIEWER
  myAccel->setActionKey( SUIT_Accel::PanLeft,     Qt::CTRL+Qt::Key_Left,     OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanRight,    Qt::CTRL+Qt::Key_Right,    OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanUp,       Qt::CTRL+Qt::Key_Up,       OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanDown,     Qt::CTRL+Qt::Key_Down,     OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomIn,      Qt::CTRL+Qt::Key_Plus,     OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomOut,     Qt::CTRL+Qt::Key_Minus,    OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomFit,     Qt::CTRL+Qt::Key_Asterisk, OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::RotateLeft,  Qt::ALT+Qt::Key_Left,      OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::RotateRight, Qt::ALT+Qt::Key_Right,     OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::RotateUp,    Qt::ALT+Qt::Key_Up,        OCCViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::RotateDown,  Qt::ALT+Qt::Key_Down,      OCCViewer_Viewer::Type() );
#endif
#ifndef DISABLE_VTKVIEWER
  myAccel->setActionKey( SUIT_Accel::PanLeft,     Qt::CTRL+Qt::Key_Left,     VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanRight,    Qt::CTRL+Qt::Key_Right,    VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanUp,       Qt::CTRL+Qt::Key_Up,       VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanDown,     Qt::CTRL+Qt::Key_Down,     VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomIn,      Qt::CTRL+Qt::Key_Plus,     VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomOut,     Qt::CTRL+Qt::Key_Minus,    VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomFit,     Qt::CTRL+Qt::Key_Asterisk, VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::RotateLeft,  Qt::ALT+Qt::Key_Left,      VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::RotateRight, Qt::ALT+Qt::Key_Right,     VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::RotateUp,    Qt::ALT+Qt::Key_Up,        VTKViewer_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::RotateDown,  Qt::ALT+Qt::Key_Down,      VTKViewer_Viewer::Type() );
#endif
#ifndef DISABLE_PLOT2DVIEWER
  myAccel->setActionKey( SUIT_Accel::PanLeft,     Qt::CTRL+Qt::Key_Left,     Plot2d_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanRight,    Qt::CTRL+Qt::Key_Right,    Plot2d_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanUp,       Qt::CTRL+Qt::Key_Up,       Plot2d_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::PanDown,     Qt::CTRL+Qt::Key_Down,     Plot2d_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomIn,      Qt::CTRL+Qt::Key_Plus,     Plot2d_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomOut,     Qt::CTRL+Qt::Key_Minus,    Plot2d_Viewer::Type() );
  myAccel->setActionKey( SUIT_Accel::ZoomFit,     Qt::CTRL+Qt::Key_Asterisk, Plot2d_Viewer::Type() );
#endif

  connect( mySelMgr, SIGNAL( selectionChanged() ), this, SLOT( onSelection() ) );

  // Set existing font for the python console in resources
  if( !aResMgr->hasValue( "PyConsole", "font" ) )
    return;

  QFont f = aResMgr->fontValue( "PyConsole", "font" );
  QFontDatabase fdb;
  QStringList famdb = fdb.families();

  if ( famdb.contains(f.family()) || !aResMgr->hasValue( "PyConsole", "additional_families" ) )
    return;

  QStringList anAddFamilies = aResMgr->stringValue( "PyConsole", "additional_families" ).split( ";", QString::SkipEmptyParts );
  QString aFamily;
  for ( QStringList::Iterator it = anAddFamilies.begin(); it != anAddFamilies.end(); ++it )
    {
      aFamily = *it;
      if ( famdb.contains(aFamily) )
	{
	  f.setFamily( aFamily );
	  aResMgr->setValue( "PyConsole", "font", f );
	  break;
	}
    }
}

/*!Destructor.
 *\li Save window geometry.
 *\li Save desktop geometry.
 *\li Save resource maneger.
 *\li Delete selection manager.
 */
LightApp_Application::~LightApp_Application()
{
  delete mySelMgr;
}

/*!Start application.*/
void LightApp_Application::start()
{
  CAM_Application::start();

  updateWindows();
  updateViewManagers();

  putInfo( "" );
  desktop()->statusBar()->showMessage( "" );

  LightApp_EventFilter::Init();
}

/*!Gets application name.*/
QString LightApp_Application::applicationName() const
{
  return tr( "APP_NAME" );
}

/*!Gets application version.*/
QString LightApp_Application::applicationVersion() const
{
  static QString _app_version;

  if ( _app_version.isEmpty() )
  {
    QString resVersion = tr( "APP_VERSION" );
    if ( resVersion != "APP_VERSION" )
    {
      _app_version = resVersion;
    }
    else
    {
      QString path( ::getenv( "GUI_ROOT_DIR" ) );
      if ( !path.isEmpty() )
        path += QDir::separator();
      path += QString( "bin/salome/VERSION" );

      QFile vf( path );
      if ( vf.open( QIODevice::ReadOnly ) )
      {
        QString line( vf.readLine( 1024 ) );
	vf.close();

	if ( !line.isEmpty() )
        {
	  while ( !line.isEmpty() && line.at( line.length() - 1 ) == QChar( '\n' ) )
	    line.remove( line.length() - 1, 1 );

	  int idx = line.lastIndexOf( ":" );
	  if ( idx != -1 )
	    _app_version = line.mid( idx + 1 ).trimmed();
        }
      }
    }
  }
  return _app_version;
}

/*!Load module by \a name.*/
CAM_Module* LightApp_Application::loadModule( const QString& name )
{
  CAM_Module* mod = CAM_Application::loadModule( name );
  if ( mod )
  {
    connect( this, SIGNAL( studyOpened() ), mod, SLOT( onModelOpened() ) );
    connect( this, SIGNAL( studySaved() ),  mod, SLOT( onModelSaved() ) );
    connect( this, SIGNAL( studyClosed() ), mod, SLOT( onModelClosed() ) );
  }
  return mod;
}

/*!Activate module by \a modName*/
bool LightApp_Application::activateModule( const QString& modName )
{
  QString actName;
  CAM_Module* prevMod = activeModule();

  if ( prevMod )
    actName = prevMod->moduleName();

  if ( actName == modName )
    return true;

  putInfo( tr( "ACTIVATING_MODULE" ).arg( modName ) );

  saveDockWindowsState();

  bool status = CAM_Application::activateModule( modName );

  updateModuleActions();

  putInfo( "" );

  if ( !status )
    return false;

  updateWindows();
  updateViewManagers();

  return true;
}

/*!Gets selection manager.*/
LightApp_SelectionMgr* LightApp_Application::selectionMgr() const
{
  return mySelMgr;
}

/*!Creat action "New window" for certain type of viewer:*/
void LightApp_Application::createActionForViewer( const int id,
                                                  const int parentId,
                                                  const QString& suffix,
                                                  const int accel )
{
  QAction* a = createAction( id, tr( QString( "NEW_WINDOW_%1" ).arg( suffix ).toLatin1().constData() ), QIcon(),
			       tr( QString( "NEW_WINDOW_%1" ).arg( suffix ).toLatin1().constData() ),
			       tr( QString( "NEW_WINDOW_%1" ).arg( suffix ).toLatin1().constData() ),
			       accel, desktop(), false, this, SLOT( onNewWindow() ) );
  createMenu( a, parentId, -1 );
}

/*!Create actions:*/
void LightApp_Application::createActions()
{
  STD_Application::createActions();

  SUIT_Desktop* desk = desktop();
  SUIT_ResourceMgr* resMgr = resourceMgr();

  //! Preferences
  createAction( PreferencesId, tr( "TOT_DESK_PREFERENCES" ), QIcon(),
		tr( "MEN_DESK_PREFERENCES" ), tr( "PRP_DESK_PREFERENCES" ),
		Qt::CTRL+Qt::Key_R, desk, false, this, SLOT( onPreferences() ) );

  //! Help for modules
  int helpMenu = createMenu( tr( "MEN_DESK_HELP" ), -1, -1, 1000 );
  int helpModuleMenu = createMenu( tr( "MEN_DESK_MODULE_HELP" ), helpMenu, -1, 0 );
  createMenu( separator(), helpMenu, -1, 1 );

  QStringList aModuleList;
  modules( aModuleList, false );

  int id = LightApp_Application::UserID + FIRST_HELP_ID;
  // help for KERNEL and GUI
  QString dir;//QByteArray dir;
  QString aFileName = "index.html";
  QString root;
  QAction* a;
  dir = getenv("GUI_ROOT_DIR");
  if ( !dir.isEmpty() ) {
    root = Qtx::addSlash( Qtx::addSlash(dir) + Qtx::addSlash("share") + Qtx::addSlash("doc") +
                          Qtx::addSlash("salome") + Qtx::addSlash("gui") +  Qtx::addSlash("GUI") );
    if ( QFileInfo( root + aFileName ).exists() ) {
      a = createAction( id, tr( QString("GUI Help").toLatin1().constData() ),
			resMgr->loadPixmap( "STD", tr( "ICON_HELP" ), false ),
			tr( QString("GUI Help").toLatin1().constData() ),
			tr( QString("GUI Help").toLatin1().constData() ),
			0, desk, false, this, SLOT( onHelpContentsModule() ) );
      a->setObjectName( QString("GUI") );
      createMenu( a, helpModuleMenu, -1 );
      id++;
    }
  }
  dir = getenv("KERNEL_ROOT_DIR");
  if ( !dir.isEmpty() ) {
    root = Qtx::addSlash( Qtx::addSlash(dir) + Qtx::addSlash("share") + Qtx::addSlash("doc") +
			  Qtx::addSlash("salome") );
    if ( QFileInfo( root + aFileName ).exists() ) {
      a = createAction( id, tr( QString("KERNEL Help").toLatin1().constData() ),
			resMgr->loadPixmap( "STD", tr( "ICON_HELP" ), false ),
			tr( QString("KERNEL Help").toLatin1().constData() ),
			tr( QString("KERNEL Help").toLatin1().constData() ),
			0, desk, false, this, SLOT( onHelpContentsModule() ) );
      a->setObjectName( QString("KERNEL") );
      createMenu( a, helpModuleMenu, -1 );
      id++;
    }
  }
  // help for other existing modules
  QStringList::Iterator it;
  for ( it = aModuleList.begin(); it != aModuleList.end(); ++it )
  {
    if ( (*it).isEmpty() )
      continue;

    QString modName = moduleName( *it );

    dir = getenv( (modName + "_ROOT_DIR").toLatin1().constData() );
    if ( !dir.isEmpty() ) {
      root = Qtx::addSlash( Qtx::addSlash(dir) +  Qtx::addSlash("share") + Qtx::addSlash("doc") +
                            Qtx::addSlash("salome") + Qtx::addSlash("gui") +  Qtx::addSlash(modName) );
      if ( QFileInfo( root + aFileName ).exists() ) {

	QAction* a = createAction( id, tr( (moduleTitle(modName) + QString(" Help")).toLatin1().constData() ),
				   resMgr->loadPixmap( "STD", tr( "ICON_HELP" ), false ),
				   tr( (moduleTitle(modName) + QString(" Help")).toLatin1().constData() ),
				   tr( (moduleTitle(modName) + QString(" Help")).toLatin1().constData() ),
				   0, desk, false, this, SLOT( onHelpContentsModule() ) );
	a->setObjectName( modName );
	createMenu( a, helpModuleMenu, -1 );
	id++;
      }
    }
  }

  //! MRU
  static QtxMRUAction* mru = new QtxMRUAction( tr( "TOT_DESK_MRU" ), tr( "MEN_DESK_MRU" ), 0 );
  connect( mru, SIGNAL( activated( const QString& ) ), this, SLOT( onMRUActivated( const QString& ) ) );
  registerAction( MRUId, mru );

  // default icon for neutral point ('SALOME' module)
  QPixmap defIcon = resMgr->loadPixmap( "LightApp", tr( "APP_DEFAULT_ICO" ), false );
  if ( defIcon.isNull() )
    defIcon = QPixmap( imageEmptyIcon );

  //! default icon for any module
  QPixmap modIcon = resMgr->loadPixmap( "LightApp", tr( "APP_MODULE_ICO" ), false );
  if ( modIcon.isNull() )
    modIcon = QPixmap( imageEmptyIcon );

  QStringList modList;
  modules( modList, false );

  if ( modList.count() > 1 )
  {
    LightApp_ModuleAction* moduleAction =
      new LightApp_ModuleAction( tr( "APP_NAME" ), defIcon, desk );

    QMap<QString, QString> iconMap;
    moduleIconNames( iconMap );

    const int iconSize = 20;

    for ( it = modList.begin(); it != modList.end(); ++it )
    {
      if ( !isLibExists( *it ) )
        continue;

      QString iconName;
      if ( iconMap.contains( *it ) )
        iconName = iconMap[*it];

      QString modName = moduleName( *it );

      QPixmap icon = resMgr->loadPixmap( modName, iconName, false );
      if ( icon.isNull() )
      {
	icon = modIcon;
	printf( "****************************************************************\n" );
	printf( "*    Icon for %s not found. Using the default one.\n", (*it).toLatin1().constData() );
	printf( "****************************************************************\n" );
      }

      icon = Qtx::scaleIcon( icon, iconSize );

      moduleAction->insertModule( *it, icon );
    }

    connect( moduleAction, SIGNAL( moduleActivated( const QString& ) ), this, SLOT( onModuleActivation( const QString& ) ) );
    registerAction( ModulesListId, moduleAction );
  }

  // New window
  int windowMenu = createMenu( tr( "MEN_DESK_WINDOW" ), -1, MenuWindowId, 100 );
  int newWinMenu = createMenu( tr( "MEN_DESK_NEWWINDOW" ), windowMenu, -1, 0 );
  createMenu( separator(), windowMenu, -1, 1 );


#ifndef DISABLE_GLVIEWER
  createActionForViewer( NewGLViewId, newWinMenu, QString::number( 0 ), Qt::ALT+Qt::Key_G );
#endif
#ifndef DISABLE_PLOT2DVIEWER
  createActionForViewer( NewPlot2dId, newWinMenu, QString::number( 1 ), Qt::ALT+Qt::Key_P );
#endif
#ifndef DISABLE_OCCVIEWER
  createActionForViewer( NewOCCViewId, newWinMenu, QString::number( 2 ), Qt::ALT+Qt::Key_O );
#endif
#ifndef DISABLE_VTKVIEWER
  createActionForViewer( NewVTKViewId, newWinMenu, QString::number( 3 ), Qt::ALT+Qt::Key_K );
#endif
#ifndef DISABLE_QXGRAPHVIEWER
  createActionForViewer( NewQxGraphViewId, newWinMenu, QString::number( 4 ), Qt::ALT+Qt::Key_C );
#endif

  createAction( RenameId, tr( "TOT_RENAME" ), QIcon(), tr( "MEN_DESK_RENAME" ), tr( "PRP_RENAME" ),
		Qt::SHIFT+Qt::Key_R, desk, false, this, SLOT( onRenameWindow() ) );
  createMenu( RenameId, windowMenu, -1 );

  int fileMenu = createMenu( tr( "MEN_DESK_FILE" ), -1 );
  createMenu( PreferencesId, fileMenu, 50, -1 );
  createMenu( separator(), fileMenu, -1, 50, -1 );

  createMenu( separator(), fileMenu, -1, 100, -1 );
  createMenu( MRUId, fileMenu, 100, -1 );
  createMenu( separator(), fileMenu, -1, 100, -1 );

  int modTBar = createTool( tr( "INF_TOOLBAR_MODULES" ) );
  createTool( ModulesListId, modTBar );
}

/*!On module activation action.*/
void LightApp_Application::onModuleActivation( const QString& modName )
{
  // Force user to create/open a study before module activation
  QMap<QString, QString> iconMap;
  moduleIconNames( iconMap );
  QPixmap icon = resourceMgr()->loadPixmap( moduleName( modName ), iconMap[ modName ], false );
  if ( icon.isNull() )
    icon = resourceMgr()->loadPixmap( "LightApp", tr( "APP_MODULE_BIG_ICO" ), false ); // default icon for any module

  bool cancelled = false;

  while ( !modName.isEmpty() && !activeStudy() && !cancelled ){
    LightApp_ModuleDlg aDlg( desktop(), modName, icon );
    QMap<int, QString> opmap = activateModuleActions();
    for ( QMap<int, QString>::ConstIterator it = opmap.begin(); it != opmap.end(); ++it )
      aDlg.addButton( it.value(), it.key() );

    int res = aDlg.exec();
    if ( res ) {
      // some operation is selected
      moduleActionSelected( res );
    }
    else {
      // cancelled
      putInfo( tr("INF_CANCELLED") );

      LightApp_ModuleAction* moduleAction =
	qobject_cast<LightApp_ModuleAction*>( action( ModulesListId ) );
      if ( moduleAction )
	moduleAction->setActiveModule( QString() );
      cancelled = true;
    }
  }

  if ( !cancelled )
    activateModule( modName );
}

/*!Default module activation.*/
QString LightApp_Application::defaultModule() const
{
  QStringList aModuleNames;
  modules( aModuleNames, false ); // obtain a complete list of module names for the current configuration
  //! If there's the one and only module --> activate it automatically
  //! TODO: Possible improvement - default module can be taken from preferences
  return aModuleNames.count() > 1 ? "" : ( aModuleNames.count() ? aModuleNames.first() : "" );
}

/*!On new window slot.*/
void LightApp_Application::onNewWindow()
{
  const QObject* obj = sender();
  if ( !obj || !obj->inherits( "QAction" ) )
    return;

  QString type;
  int id = actionId( (QAction*)obj );
  switch ( id )
  {
#ifndef DISABLE_GLVIEWER
  case NewGLViewId:
    type = GLViewer_Viewer::Type();
    break;
#endif
#ifndef DISABLE_PLOT2DVIEWER
  case NewPlot2dId:
    type = Plot2d_Viewer::Type();
    break;
#endif
#ifndef DISABLE_OCCVIEWER
  case NewOCCViewId:
    type = OCCViewer_Viewer::Type();
    break;
#endif
#ifndef DISABLE_VTKVIEWER
  case NewVTKViewId:
    type = VTKViewer_Viewer::Type();
    break;
#endif
#ifndef DISABLE_QXGRAPHVIEWER
  case NewQxGraphViewId:
    type = QxGraph_Viewer::Type();
    break;
#endif
  }

  if ( !type.isEmpty() )
    createViewManager( type );
}

/*!
  SLOT: Creates new document
*/
void LightApp_Application::onNewDoc()
{
  saveDockWindowsState();

  CAM_Application::onNewDoc();
}

/*!
  SLOT: Opens new document
*/
void LightApp_Application::onOpenDoc()
{
  SUIT_Study* study = activeStudy();
  saveDockWindowsState();

  CAM_Application::onOpenDoc();

  if ( !study ) // new study will be create in THIS application
  {
    updateWindows();
    updateViewManagers();
  }
}

/*!
  SLOT: Opens new document.
  \param aName - name of file
*/
bool LightApp_Application::onOpenDoc( const QString& aName )
{
#ifdef SWITCH_OFF
  bool isAlreadyOpen = false;

  // Look among opened studies
  if (activeStudy()) { // at least one study is opened
    SUIT_Session* aSession = SUIT_Session::session();
    QList<SUIT_Application*> aAppList = aSession->applications();
    QListIterator<SUIT_Application*> it (aAppList);
    SUIT_Application* aApp = 0;
    // iterate on all applications
    while ( it.hasNext() && !isAlreadyOpen ) {
      if ( !(aApp = it.next()) ) break;
      if (aApp->activeStudy()->studyName() == aName) {
        isAlreadyOpen = true; // Already opened, ask user what to do

        // The document ... is already open.
        // Do you want to reload it?
        int aAnswer = SUIT_MessageBox::question(desktop(), tr("WRN_WARNING"),
						tr("QUE_DOC_ALREADYOPEN").arg(aName),
						SUIT_MessageBox::Yes | SUIT_MessageBox::No,
						SUIT_MessageBox::No );
        if (aAnswer == SUIT_MessageBox::Yes) { // reload
          if (activeStudy()->studyName() == aName && aAppList.count() > 1) {
            // Opened in THIS (active) application.
            STD_Application* app1 = (STD_Application*)aAppList.at(0);
            STD_Application* app2 = (STD_Application*)aAppList.at(1);
            if (!app1 || !app2) {
              // Error
              return false;
            }
            if (app1->activeStudy()->studyName() == aName) {
              // app1 is this application, we need another one
              app1 = app2;
            }
            // Close document of this application. This application will be destroyed.
            onCloseDoc(/*ask = */false);
            // Open the file with another application, as this one will be destroyed.
            return app1->onOpenDoc(aName);
          } else {
            // Opened in another application.
            STD_Application* app = (STD_Application*)aApp;
            if (app)
              app->onCloseDoc(/*ask = */false);
          }
        } else { // do not reload
          // OK, the study will not be reloaded, but we call
          // CAM_Application::onOpenDoc( aName ) all the same.
          // It will activate a desktop of the study <aName>.
        }
      }
    }
  }
#endif

  bool res = CAM_Application::onOpenDoc( aName );

  QtxMRUAction* mru = ::qobject_cast<QtxMRUAction*>( action( MRUId ) );
  if ( mru )
  {
    if ( res )
      mru->insert( aName );
    else
      mru->remove( aName );
  }
  return res;
}

/*!
  SLOT: Displays "About" message box
*/
void LightApp_Application::onHelpAbout()
{
  LightApp_AboutDlg dlg( applicationName(), applicationVersion(), desktop() );
  dlg.exec();
}

/*!
  Private SLOT: Called on selection is changed
  Dispatchs active module that selection is changed
*/
void LightApp_Application::onSelection()
{
  onSelectionChanged();

  if ( activeModule() && activeModule()->inherits( "LightApp_Module" ) )
    ((LightApp_Module*)activeModule())->selectionChanged();
}

/*!
  Sets active study.
 \param study - SUIT_Study.
*/
void LightApp_Application::setActiveStudy( SUIT_Study* study )
{
  CAM_Application::setActiveStudy( study );
}

/*!
  Enables/Disables menu items and toolbar buttons. Rebuild menu
*/
void LightApp_Application::updateCommandsStatus()
{
  CAM_Application::updateCommandsStatus();
  QAction* a = 0;

#ifndef DISABLE_GLVIEWER
  a = action( NewGLViewId );
  if( a )
    a->setEnabled( activeStudy() );
#endif

#ifndef DISABLE_PLOT2DVIEWER
  a = action( NewPlot2dId );
  if( a )
    a->setEnabled( activeStudy() );
#endif

#ifndef DISABLE_OCCVIEWER
  a = action( NewOCCViewId );
  if( a )
    a->setEnabled( activeStudy() );
#endif

#ifndef DISABLE_VTKVIEWER
  a = action( NewVTKViewId );
  if( a )
    a->setEnabled( activeStudy() );
#endif

#ifndef DISABLE_QXGRAPHVIEWER
  a = action( NewQxGraphViewId );
  if( a )
    a->setEnabled( activeStudy() );
#endif
}

/*!
  \class RunBrowser
  Runs system command in separate thread
*/
class RunBrowser: public QThread
{
public:
  RunBrowser( LightApp_Application* app,
	      const QString&        theApp,
	      const QString&        theParams,
	      const QString&        theHelpFile,
	      const QString&        theContext = QString() )
    : myApp( theApp ),
      myParams( theParams ),
#ifdef WIN32
      myHelpFile( "file://" + theHelpFile ),
#else
      myHelpFile( "file:" + theHelpFile ),
#endif
      myContext( theContext ),
      myStatus(0),
      myLApp( app )
  {
  }

  virtual void run()
  {
    if ( !myApp.isEmpty()) {
      QString aCommand = QString( "%1 %2 %3" ).arg( myApp, myParams, myHelpFile );
      if ( !myContext.isEmpty() )
	aCommand += "#" + myContext;

      QProcess* proc = new QProcess();

      proc->start( aCommand );
      if ( !proc->waitForStarted() ) {
	SALOME_CustomEvent* ce2000 = new SALOME_CustomEvent( 2000 );
	QString* msg = new QString( QObject::tr( "EXTERNAL_BROWSER_CANNOT_SHOW_PAGE" ).arg( myApp, myHelpFile ) );
	ce2000->setData( msg );
	QApplication::postEvent( myLApp, ce2000 );
      }
    }
  }

private:
  QString               myApp;
  QString               myParams;
  QString               myHelpFile;
  QString               myContext;
  int                   myStatus;
  LightApp_Application* myLApp;
};

/*!
  SLOT: Displays help contents for choosen module
*/
void LightApp_Application::onHelpContentsModule()
{
  const QAction* obj = (QAction*) sender();

  QString aComponentName = obj->objectName();
  QString aFileName = "index.html";

  QString dir = getenv( (aComponentName + "_ROOT_DIR").toLatin1().constData() );
  QString homeDir = !aComponentName.compare(QString("KERNEL")) ?
    Qtx::addSlash( Qtx::addSlash(dir) + Qtx::addSlash("share") + Qtx::addSlash("doc") + Qtx::addSlash("salome") ) :
    Qtx::addSlash( Qtx::addSlash(dir) + Qtx::addSlash("share") + Qtx::addSlash("doc") + Qtx::addSlash("salome") + Qtx::addSlash("gui") +  Qtx::addSlash(aComponentName) );

  QString helpFile = QFileInfo( homeDir + aFileName ).absoluteFilePath();
  SUIT_ResourceMgr* resMgr = resourceMgr();
	QString platform;
#ifdef WIN32
	platform = "winapplication";
#else
	platform = "application";
#endif
	QString anApp = resMgr->stringValue("ExternalBrowser", platform);
#ifdef WIN32
	QString quote("\"");
	anApp.prepend( quote );
	anApp.append( quote );
#endif
  QString aParams = resMgr->stringValue("ExternalBrowser", "parameters");

  if (!anApp.isEmpty()) {
    RunBrowser* rs = new RunBrowser( this, anApp, aParams, helpFile );
    rs->start();
  }
  else {
    if( SUIT_MessageBox::question(desktop(), tr("WRN_WARNING"),
				  tr("DEFINE_EXTERNAL_BROWSER"),
				  SUIT_MessageBox::Yes | SUIT_MessageBox::No,
				  SUIT_MessageBox::Yes ) == SUIT_MessageBox::Yes )
      onPreferences();
  }
}

/*!
  SLOT: Displays help contents for choosen dialog
*/
void LightApp_Application::onHelpContextModule( const QString& theComponentName,
						const QString& theFileName,
						const QString& theContext )
{
  QString homeDir = "";
  if ( !theComponentName.isEmpty() ) {
    QString dir = getenv( ( theComponentName + "_ROOT_DIR" ).toLatin1().constData() );
    if ( !dir.isEmpty() )
      homeDir = Qtx::addSlash( Qtx::addSlash( dir )      +
			       Qtx::addSlash( "share" )  +
			       Qtx::addSlash( "doc" )    +
			       Qtx::addSlash( "salome" ) +
			       Qtx::addSlash( "gui" )    +
			       Qtx::addSlash( theComponentName ) );
  }

  QString helpFile = QFileInfo( homeDir + theFileName ).absoluteFilePath();
  SUIT_ResourceMgr* resMgr = resourceMgr();
	QString platform;
#ifdef WIN32
	platform = "winapplication";
#else
	platform = "application";
#endif
	QString anApp = resMgr->stringValue("ExternalBrowser", platform);
#ifdef WIN32
	QString quote("\"");
	anApp.prepend( quote );
	anApp.append( quote );
#endif
  QString aParams = resMgr->stringValue("ExternalBrowser", "parameters");

  if (!anApp.isEmpty()) {
    RunBrowser* rs = new RunBrowser( this, anApp, aParams, helpFile, theContext );
    rs->start();
  }
  else {
    if( SUIT_MessageBox::question(desktop(), tr("WRN_WARNING"),
				  tr("DEFINE_EXTERNAL_BROWSER"),
				  SUIT_MessageBox::Yes | SUIT_MessageBox::No,
				  SUIT_MessageBox::Yes ) == SUIT_MessageBox::Yes )
      onPreferences();
  }
}

/*!
  Sets enable or disable some actions on selection changed.
*/
void LightApp_Application::onSelectionChanged()
{
}

/*!
  Adds window to application.
  \param wid - QWidget
  \param flag - key for window
  \param studyId - study id
  Flag used how identificator of window in windows list.
*/
/*
void LightApp_Application::addWindow( QWidget* wid, const int flag, const int studyId )
{
  if ( !wid )
    return;

  int sId = studyId;
  if ( sId < 0 )
  {
    if ( !activeStudy() )
      return;
    else
      sId = activeStudy()->id();
  }

  if ( !myWindows.contains( flag ) )
  {
    QMap<int, int> winMap;
    currentWindows( winMap );

    LightApp_WidgetContainer* newWC = new LightApp_WidgetContainer( flag, desktop() );
    connect( newWC, SIGNAL(  destroyed ( QObject* ) ), this, SLOT( onWCDestroyed( QObject* ) ) );
    // asv: connecting a slot for storing visibility flag of a window
    connect( newWC, SIGNAL( visibilityChanged ( bool ) ), SLOT( onVisibilityChanged( bool ) ) );
    myWindows.insert( flag, newWC );
    if ( winMap.contains( flag ) ) {
      //desktop()->removeDockWidget( myWindows[flag] );
      desktop()->addDockWidget( (Qt::DockWidgetArea)winMap[flag], myWindows[flag] );
    }

    //myWindows[flag]->setResizeEnabled( true );
    myWindows[flag]->setFeatures( QDockWidget::AllDockWidgetFeatures );
    myWindows[flag]->setObjectName( QString( "dock_window_%1" ).arg( flag ) );
    //myWindows[flag]->setFixedExtentWidth( wid->width() );
    //myWindows[flag]->setFixedExtentHeight( wid->height() );
    myWindows[flag]->resize( wid->width(), wid->height() );
  }

  QFont f;
#ifndef DISABLE_PYCONSOLE
  if( wid->inherits( "PyConsole_Console" ) )
  {
    if( resourceMgr()->hasValue( "PyConsole", "font" ) )
      f = resourceMgr()->fontValue( "PyConsole", "font" );
    else
      {
	f = ( ( PyConsole_Console* )wid )->font();
	resourceMgr()->setValue( "PyConsole", "font", f );
      }
  }
  else
#endif
    f = wid->font();

  myWindows[flag]->insert( sId, wid );
  wid->setFont( f );

  setWindowShown( flag, !myWindows[flag]->isEmpty() );
}
*/

QWidget* LightApp_Application::dockWindow( const int id ) const
{
  QWidget* wid = 0;
  if ( myWin.contains( id ) )
    wid = myWin[id];
  return wid;
}

QDockWidget* LightApp_Application::windowDock( QWidget* wid ) const
{
  if ( !wid )
    return 0;

  QDockWidget* dock = 0;
  QWidget* w = wid->parentWidget();
  while ( w && !dock )
  {
    dock = ::qobject_cast<QDockWidget*>( w );
    w = w->parentWidget();
  }
  return dock;
}

void LightApp_Application::insertDockWindow( const int id, QWidget* wid )
{
  if ( !wid )
    return;

  if ( wid != dockWindow( id ) )
    removeDockWindow( id );

  myWin.insert( id, wid );

  QtxDockWidget* dock = new QtxDockWidget( true, desktop() );
  connect( dock, SIGNAL(  destroyed( QObject* ) ), this, SLOT( onWCDestroyed( QObject* ) ) );

  dock->setFeatures( QDockWidget::AllDockWidgetFeatures );
  dock->setObjectName( QString( "window_%1" ).arg( id ) );
  dock->setWidget( wid );
  dock->show();
}

void LightApp_Application::removeDockWindow( const int id )
{
  QWidget* wid = dockWindow( id );
  if ( !wid )
    return;

  myWin.remove( id );

  QDockWidget* dock = windowDock( wid );
  if ( !dock )
    return;

  dock->setWidget( 0 );
  wid->setParent( 0 );
  wid->setVisible( false );
  delete dock;
}

void LightApp_Application::placeDockWindow( const int id, Qt::DockWidgetArea place )
{
  QDockWidget* dock = windowDock( dockWindow( id ) );
  if ( dock && desktop() )
    desktop()->addDockWidget( place, dock );
}

/*!
  Gets window.
  \param flag - key for window
  \param studyId - study id
  Flag used how identificator of window in windows list.
*/
QWidget* LightApp_Application::getWindow( const int flag, const int )
{
  QWidget* wid = dockWindow( flag );
  if ( !wid )
    insertDockWindow( flag, wid = createWindow( flag ) );

  QMap<int, int> winMap;
  currentWindows( winMap );
  if ( winMap.contains( flag ) )
    placeDockWindow( flag, (Qt::DockWidgetArea)winMap[flag] );

  return wid;
}

/*!
  \return Object Browser
*/
SUIT_DataBrowser* LightApp_Application::objectBrowser()
{
  return qobject_cast<SUIT_DataBrowser*>( dockWindow( WT_ObjectBrowser ) );
}

/*!
  \return Log Window
*/
LogWindow* LightApp_Application::logWindow()
{
  return qobject_cast<LogWindow*>( dockWindow( WT_LogWindow ) );
}

#ifndef DISABLE_PYCONSOLE
/*!
  \return Python Console
*/
PyConsole_Console* LightApp_Application::pythonConsole()
{
  return qobject_cast<PyConsole_Console*>( dockWindow( WT_PyConsole ) );
}
#endif

/*!
  Updates object browser and maybe data models
  \param updateModels - if it is true, then data models are updated
*/
void LightApp_Application::updateObjectBrowser( const bool updateModels )
{
  // update existing data models
  if ( updateModels )
  {
    const bool isAutoUpdate = objectBrowser() ? objectBrowser()->autoUpdate() : true;
    if ( objectBrowser() )
      objectBrowser()->setAutoUpdate( false );

    LightApp_Study* study = dynamic_cast<LightApp_Study*>(activeStudy());
    if ( study ) {
      CAM_Study::ModelList dm_list;
      study->dataModels( dm_list );
      QListIterator<CAM_DataModel*> it( dm_list );
      while ( it.hasNext() ) {
	CAM_DataModel* camDM = it.next();
        if ( camDM && camDM->inherits( "LightApp_DataModel" ) )
          ((LightApp_DataModel*)camDM)->update();
      }
    }

    if( objectBrowser() )
      objectBrowser()->setAutoUpdate( isAutoUpdate );
  }

  if ( objectBrowser() ) {
    objectBrowser()->updateGeometry();
    objectBrowser()->updateTree( 0, false );
  }
}

/*!
  \return preferences
*/
LightApp_Preferences* LightApp_Application::preferences() const
{
  return preferences( false );
}

/*!
  \return first view manager of some type
  \param vmType - type of view manager
  \param create - is it necessary to create view manager in case, when there is no manager of such type
*/
SUIT_ViewManager* LightApp_Application::getViewManager( const QString& vmType, const bool create )
{
  SUIT_ViewManager* aVM = viewManager( vmType );
  SUIT_ViewManager* anActiveVM = CAM_Application::activeViewManager();

  if ( anActiveVM && anActiveVM->getType() == vmType )
    aVM = anActiveVM;

  if ( aVM && create )
  {
    if ( !aVM->getActiveView() )
      aVM->createView();
    else
      aVM->getActiveView()->setFocus();
  }
  else if ( create )
    aVM = createViewManager( vmType );

  return aVM;
}

/*!
  Creates view manager of some type
  \param vmType - type of view manager
*/
SUIT_ViewManager* LightApp_Application::createViewManager( const QString& vmType )
{
  SUIT_ResourceMgr* resMgr = resourceMgr();

  SUIT_ViewManager* viewMgr = 0;
#ifndef DISABLE_GLVIEWER
  if( vmType == GLViewer_Viewer::Type() )
  {
    viewMgr = new GLViewer_ViewManager( activeStudy(), desktop() );
    new LightApp_GLSelector( (GLViewer_Viewer2d*)viewMgr->getViewModel(), mySelMgr );
  }
#endif
#ifndef DISABLE_PLOT2DVIEWER
  if( vmType == Plot2d_Viewer::Type() )
  {
    viewMgr = new Plot2d_ViewManager( activeStudy(), desktop() );
    Plot2d_Viewer* vm;
#ifndef DISABLE_SALOMEOBJECT
    vm = new SPlot2d_Viewer();
#else
    vm = new Plot2d_Viewer();
#endif
    viewMgr->setViewModel( vm  );// custom view model, which extends SALOME_View interface
    Plot2d_ViewWindow* wnd = dynamic_cast<Plot2d_ViewWindow*>( viewMgr->getActiveView() );
    if( wnd )
    {
      Plot2d_ViewFrame* frame = wnd->getViewFrame();
      frame->setBackgroundColor( resMgr->colorValue( "Plot2d", "Background", frame->backgroundColor() ) );
    }
  }
#endif
  //#ifndef DISABLE_SUPERVGRAPHVIEWER
  //  if( vmType == SUPERVGraph_Viewer::Type() )
  //  {
  //    viewMgr = new SUPERVGraph_ViewManager( activeStudy(), desktop(), new SUPERVGraph_Viewer() );
  //  }
  //#endif
#ifndef DISABLE_QXGRAPHVIEWER
  if( vmType == QxGraph_Viewer::Type() )
    {
      viewMgr = new QxGraph_ViewManager( activeStudy(), desktop(), new QxGraph_Viewer() );
    }
#endif
#ifndef DISABLE_OCCVIEWER
  if( vmType == OCCViewer_Viewer::Type() )
  {
    viewMgr = new OCCViewer_ViewManager( activeStudy(), desktop() );
    OCCViewer_Viewer* vm;
#ifndef DISABLE_SALOMEOBJECT
    vm = new SOCC_Viewer();
#else
    vm = new OCCViewer_Viewer();
#endif
    vm->setBackgroundColor( resMgr->colorValue( "OCCViewer", "background", vm->backgroundColor() ) );
    vm->setTrihedronSize( resMgr->doubleValue( "OCCViewer", "trihedron_size", vm->trihedronSize() ) );
    int u( 1 ), v( 1 );
    vm->isos( u, v );
    u = resMgr->integerValue( "OCCViewer", "iso_number_u", u );
    v = resMgr->integerValue( "OCCViewer", "iso_number_v", v );
    vm->setIsos( u, v );
    viewMgr->setViewModel( vm );// custom view model, which extends SALOME_View interface
    new LightApp_OCCSelector( (OCCViewer_Viewer*)viewMgr->getViewModel(), mySelMgr );
  }
#endif
#ifndef DISABLE_VTKVIEWER
#ifndef DISABLE_SALOMEOBJECT
  if ( vmType == SVTK_Viewer::Type() )
#else
  if ( vmType == VTKViewer_Viewer::Type() )
#endif
  {
#ifndef DISABLE_SALOMEOBJECT
    viewMgr = new SVTK_ViewManager( activeStudy(), desktop() );
    SVTK_Viewer* vm = dynamic_cast<SVTK_Viewer*>( viewMgr->getViewModel() );
    if( vm )
    {
      vm->setBackgroundColor( resMgr->colorValue( "VTKViewer", "background", vm->backgroundColor() ) );
      vm->setTrihedronSize( resMgr->doubleValue( "VTKViewer", "trihedron_size", vm->trihedronSize() ),
			    resMgr->booleanValue( "VTKViewer", "relative_size", vm->trihedronRelative() ) );
      new LightApp_VTKSelector( vm, mySelMgr );
    }
#else
    viewMgr = new VTKViewer_ViewManager( activeStudy(), desktop() );
    VTKViewer_Viewer* vm = dynamic_cast<VTKViewer_Viewer*>( viewMgr->getViewModel() );
    if ( vm )
      vm->setBackgroundColor( resMgr->colorValue( "VTKViewer", "background", vm->backgroundColor() ) );
#endif
  }
#endif

  if ( !viewMgr )
    return 0;

  addViewManager( viewMgr );
  SUIT_ViewWindow* viewWin = viewMgr->createViewWindow();

  if ( viewWin && desktop() )
    viewWin->resize( (int)( desktop()->width() * 0.6 ), (int)( desktop()->height() * 0.6 ) );

  return viewMgr;
}

/*!
  SLOT: Removes view manager from application
*/
void LightApp_Application::onCloseView( SUIT_ViewManager* theVM )
{
  removeViewManager( theVM );
}

/*!
  Protected SLOT: On study created.
  \param theStudy - just created study
*/
void LightApp_Application::onStudyCreated( SUIT_Study* theStudy )
{
  SUIT_DataObject* aRoot = 0;
  if ( theStudy && theStudy->root() )
  {
    aRoot = theStudy->root();
    //aRoot->setName( tr( "DATA_MODELS" ) );
  }

  getWindow( WT_ObjectBrowser );

  loadDockWindowsState();

  if ( objectBrowser() )
    objectBrowser()->setRoot( aRoot );

  activateModule( defaultModule() );

  if ( objectBrowser() )
    objectBrowser()->openLevels();
}

/*!
  Protected SLOT: On study opened.
  \param theStudy - just opened  study
*/
void LightApp_Application::onStudyOpened( SUIT_Study* theStudy )
{
  SUIT_DataObject* aRoot = 0;
  if ( theStudy && theStudy->root() )
  {
    aRoot = theStudy->root();
    //aRoot->dump();
  }

  getWindow( WT_ObjectBrowser );

  loadDockWindowsState();

  if ( objectBrowser() )
    objectBrowser()->setRoot( aRoot );

  activateModule( defaultModule() );

  if ( objectBrowser() )
    objectBrowser()->openLevels();

  emit studyOpened();
}

/*!Protected SLOT. On study saved.*/
void LightApp_Application::onStudySaved( SUIT_Study* s )
{
  QtxMRUAction* mru = ::qobject_cast<QtxMRUAction*>( action( MRUId ) );
  if ( mru && s )
      mru->insert( s->studyName() );

  emit studySaved();
}

/*!Protected SLOT. On study closed.*/
void LightApp_Application::onStudyClosed( SUIT_Study* s )
{
  // Bug 10396: clear selection
  mySelMgr->clearSelected();

  // Bug 12944: emit signal only after clear selection
  emit studyClosed();

  activateModule( "" );
}

/*!Protected SLOT.On desktop activated.*/
void LightApp_Application::onDesktopActivated()
{
  CAM_Application::onDesktopActivated();
  LightApp_Module* aModule = dynamic_cast<LightApp_Module*>(activeModule());
  if(aModule)
    aModule->studyActivated();
}

void LightApp_Application::studyOpened( SUIT_Study* s )
{
  CAM_Application::studyOpened( s );

  updateWindows();
  updateViewManagers();
}

void LightApp_Application::studyCreated( SUIT_Study* s )
{
  CAM_Application::studyCreated( s );

  updateWindows();
  updateViewManagers();
}

/*!Gets file filter.
 *\retval QString "(*.bin)"
 */
QString LightApp_Application::getFileFilter() const
{
  //return "(*.bin)";
  // HDF persistence
  return "(*.hdf)";
}

/*!
  Shows file dialog and return user selected file name
*/
QString LightApp_Application::getFileName( bool open, const QString& initial, const QString& filters,
                                           const QString& caption, QWidget* parent )
{
  if ( !parent )
    parent = desktop();
  QStringList fls = filters.split( ";;", QString::SkipEmptyParts );
  return SUIT_FileDlg::getFileName( parent, initial, fls, caption, open, true );
}

/*! Gets directory*/
QString LightApp_Application::getDirectory( const QString& initial, const QString& caption, QWidget* parent )
{
  if ( !parent )
    parent = desktop();
  return SUIT_FileDlg::getExistingDirectory( parent, initial, caption, true );
}

/*! Get open file names*/
QStringList LightApp_Application::getOpenFileNames( const QString& initial, const QString& filters,
                                                    const QString& caption, QWidget* parent )
{
  if ( !parent )
    parent = desktop();
  QStringList fls = filters.split( ";;", QString::SkipEmptyParts );
  return SUIT_FileDlg::getOpenFileNames( parent, initial, fls, caption, true );
}

/*!Private SLOT. Update object browser.*/
void LightApp_Application::onRefresh()
{
  updateObjectBrowser( true );
}

/*!Private SLOT. On preferences.*/
void LightApp_Application::onPreferences()
{
  QApplication::setOverrideCursor( Qt::WaitCursor );

  LightApp_PreferencesDlg* prefDlg = new LightApp_PreferencesDlg( preferences( true ), desktop());

  QApplication::restoreOverrideCursor();

  if ( !prefDlg )
    return;

  if ( ( prefDlg->exec() == QDialog::Accepted || prefDlg->isSaved() ) &&  resourceMgr() ) {
    if ( desktop() )
      resourceMgr()->setValue( "desktop", "geometry", desktop()->storeGeometry() );
    resourceMgr()->save();
  }

  delete prefDlg;
}

/*!Protected SLOT. On preferences changed.*/
void LightApp_Application::onPreferenceChanged( QString& modName, QString& section, QString& param )
{
  LightApp_Module* sMod = 0;
  CAM_Module* mod = module( modName );
  if ( mod && mod->inherits( "LightApp_Module" ) )
    sMod = (LightApp_Module*)mod;

  if ( sMod )
    sMod->preferencesChanged( section, param );
  else
    preferencesChanged( section, param );
  // emit signal to allow additional preferences changing processing
  emit preferenceChanged( modName, section, param );
}

/*!Remove all windows from study.*/
void LightApp_Application::beforeCloseDoc( SUIT_Study* s )
{
  CAM_Application::beforeCloseDoc( s );
}

/*!Update actions.*/
void LightApp_Application::updateActions()
{
  updateCommandsStatus();
}

/*!
  Creates new study
*/
SUIT_Study* LightApp_Application::createNewStudy()
{
  LightApp_Application::lastStudyId++;

  LightApp_Study* aStudy = new LightApp_Study( this );

  // Set up processing of major study-related events
  connect( aStudy, SIGNAL( created( SUIT_Study* ) ), this, SLOT( onStudyCreated( SUIT_Study* ) ) );
  connect( aStudy, SIGNAL( opened ( SUIT_Study* ) ), this, SLOT( onStudyOpened ( SUIT_Study* ) ) );
  connect( aStudy, SIGNAL( saved  ( SUIT_Study* ) ), this, SLOT( onStudySaved  ( SUIT_Study* ) ) );
  connect( aStudy, SIGNAL( closed ( SUIT_Study* ) ), this, SLOT( onStudyClosed ( SUIT_Study* ) ) );

  return aStudy;
}

/*!
  Creates window by flag.
  \param flag - identificator of window type
*/
QWidget* LightApp_Application::createWindow( const int flag )
{
  QWidget* wid = 0;

  SUIT_ResourceMgr* resMgr = resourceMgr();

  if ( flag == WT_ObjectBrowser )
  {
    SUIT_DataBrowser* ob = new SUIT_DataBrowser( new LightApp_DataObject(), desktop() );
    ob->setSortMenuEnabled( true );
    ob->setAutoUpdate( true );
    if ( resMgr->hasValue( "ObjectBrowser", "auto_hide_search_tool" ) )
      ob->searchTool()->enableAutoHide( resMgr->booleanValue( "ObjectBrowser", "auto_hide_search_tool" ) );

    //ob->setAutoOpenLevel( 1 ); // commented by ASV as a fix to bug IPAL10107
    ob->setWindowTitle( tr( "OBJECT_BROWSER" ) );
    connect( ob, SIGNAL( requestUpdate() ), this, SLOT( onRefresh() ) );

    // temporary commented
    /*
    OB_ListView* ob_list = dynamic_cast<OB_ListView*>( const_cast<QListView*>( ob->listView() ) );
    if( ob_list )
      ob_list->setColumnMaxWidth( 0, desktop()->width()/4 );

    ob->setFilter( new LightApp_OBFilter( selectionMgr() ) );
    */

    // Create OBSelector
    new LightApp_OBSelector( ob, mySelMgr );

    wid = ob;

    ob->connectPopupRequest( this, SLOT( onConnectPopupRequest( SUIT_PopupClient*, QContextMenuEvent* ) ) );
  }
#ifndef DISABLE_PYCONSOLE
  else  if ( flag == WT_PyConsole )
  {
    PyConsole_Console* pyCons = new PyConsole_Console( desktop() );
    pyCons->setWindowTitle( tr( "PYTHON_CONSOLE" ) );
    wid = pyCons;
    pyCons->connectPopupRequest( this, SLOT( onConnectPopupRequest( SUIT_PopupClient*, QContextMenuEvent* ) ) );
  }
#endif
  else if ( flag == WT_LogWindow )
  {
    LogWindow* logWin = new LogWindow( desktop() );
    logWin->setWindowTitle( tr( "LOG_WINDOW" ) );
    wid = logWin;
    logWin->connectPopupRequest( this, SLOT( onConnectPopupRequest( SUIT_PopupClient*, QContextMenuEvent* ) ) );
  }
  return wid;
}

/*!
  \return default windows( Object Browser, Python Console )
  Adds to map \a aMap.
 */
void LightApp_Application::defaultWindows( QMap<int, int>& aMap ) const
{
  aMap.insert( WT_ObjectBrowser, Qt::LeftDockWidgetArea );
#ifndef DISABLE_PYCONSOLE
  aMap.insert( WT_PyConsole, Qt::BottomDockWidgetArea );
#endif
  //  aMap.insert( WT_LogWindow, Qt::DockBottom );
}

/*!Default view managers*/
void LightApp_Application::defaultViewManagers( QStringList& ) const
{
  /*!Do nothing.*/
}

/*!
  \return preferences.
  Create preferences, if \a crt = true.
*/
LightApp_Preferences* LightApp_Application::preferences( const bool crt ) const
{
  if ( myPrefs )
    return myPrefs;

  LightApp_Application* that = (LightApp_Application*)this;

  bool toCreate = !_prefs_ && crt;
  if( toCreate )
  {
    _prefs_ = new LightApp_Preferences( resourceMgr() );
    that->createPreferences( _prefs_ );
  }

  that->myPrefs = _prefs_;

  QList<SUIT_Application*> appList = SUIT_Session::session()->applications();
  QListIterator<SUIT_Application*> appIt ( appList );
  while ( appIt.hasNext() )
  {
    SUIT_Application* anItem = appIt.next();
    if ( !anItem->inherits( "LightApp_Application" ) )
      continue;

    LightApp_Application* app = (LightApp_Application*)anItem;

    QStringList modNameList;
    app->modules( modNameList, false );
    for ( QStringList::const_iterator it = modNameList.begin();
	  it != modNameList.end(); ++it )
      _prefs_->addPreference( *it );

    ModuleList modList;
    app->modules( modList );
    QListIterator<CAM_Module*> itr( modList );
    while ( itr.hasNext() )
    {
      LightApp_Module* mod = 0;

      CAM_Module* anItem = itr.next();
      if ( anItem->inherits( "LightApp_Module" ) )
	mod = (LightApp_Module*)anItem;

      if ( mod && !_prefs_->hasModule( mod->moduleName() ) )
      {
	_prefs_->addPreference( mod->moduleName() );
	if( toCreate )
	  mod->createPreferences();
	QtxPreferenceItem* item = _prefs_->findItem( mod->moduleName(), true );
	if ( item && item->isEmpty() )
	  delete item;
      }
    }
  }
  _prefs_->setItemProperty( "info", tr( "PREFERENCES_NOT_LOADED" ) );

  connect( myPrefs, SIGNAL( preferenceChanged( QString&, QString&, QString& ) ),
           this, SLOT( onPreferenceChanged( QString&, QString&, QString& ) ) );

  return myPrefs;
}

/*!
  Adds new module to application
*/
void LightApp_Application::moduleAdded( CAM_Module* mod )
{
  CAM_Application::moduleAdded( mod );

  LightApp_Module* lightMod = 0;
  if ( mod && mod->inherits( "LightApp_Module" ) )
    lightMod = (LightApp_Module*)mod;

  if ( myPrefs && lightMod && !myPrefs->hasModule( lightMod->moduleName() ))
  {
    myPrefs->addPreference( mod->moduleName() );
    lightMod->createPreferences();
    QtxPreferenceItem* item = myPrefs->findItem( mod->moduleName(), true );
    if ( item && item->isEmpty() )
      delete item;
  }
}

/*!
  Create preferences
*/
void LightApp_Application::createPreferences( LightApp_Preferences* pref )
{
  if ( !pref )
    return;

  int salomeCat = pref->addPreference( tr( "PREF_CATEGORY_SALOME" ) );

  int genTab = pref->addPreference( tr( "PREF_TAB_GENERAL" ), salomeCat );
  int studyGroup = pref->addPreference( tr( "PREF_GROUP_STUDY" ), genTab );

  //pref->setItemProperty( "columns", 1, studyGroup );

  pref->addPreference( tr( "PREF_MULTI_FILE" ), studyGroup, LightApp_Preferences::Bool, "Study", "multi_file" );
  pref->addPreference( tr( "PREF_ASCII_FILE" ), studyGroup, LightApp_Preferences::Bool, "Study", "ascii_file" );
  pref->addPreference( tr( "PREF_STORE_POS" ),  studyGroup, LightApp_Preferences::Bool, "Study", "store_positions" );

  int extgroup = pref->addPreference( tr( "PREF_GROUP_EXT_BROWSER" ), genTab );
  //pref->setItemProperty( "columns", 1, extgroup );
	QString platform;
#ifdef WIN32
	platform = "winapplication";
#else
	platform = "application";
#endif
  int apppref = pref->addPreference( tr( "PREF_APP" ), extgroup, LightApp_Preferences::File, "ExternalBrowser", platform );
  pref->setItemProperty( "mode", Qtx::PT_OpenFile, apppref );

  pref->addPreference( tr( "PREF_PARAM" ), extgroup, LightApp_Preferences::String, "ExternalBrowser", "parameters" );

  int pythonConsoleGroup = pref->addPreference( tr( "PREF_GROUP_PY_CONSOLE" ), genTab );
  //pref->setItemProperty( "columns", 1, pythonConsoleGroup );
  pref->addPreference( tr( "PREF_FONT" ), pythonConsoleGroup, LightApp_Preferences::Font, "PyConsole", "font" );

  int viewTab = pref->addPreference( tr( "PREF_TAB_VIEWERS" ), salomeCat );

  int occGroup = pref->addPreference( tr( "PREF_GROUP_OCCVIEWER" ), viewTab );

  int vtkGroup = pref->addPreference( tr( "PREF_GROUP_VTKVIEWER" ), viewTab );

  int plot2dGroup = pref->addPreference( tr( "PREF_GROUP_PLOT2DVIEWER" ), viewTab );

  int supervGroup = pref->addPreference( tr( "PREF_GROUP_SUPERV" ), viewTab );

  pref->setItemProperty( "columns", 2, occGroup );
  pref->setItemProperty( "columns", 2, vtkGroup );
  pref->setItemProperty( "columns", 2, plot2dGroup );

  int occTS = pref->addPreference( tr( "PREF_TRIHEDRON_SIZE" ), occGroup,
				   LightApp_Preferences::DblSpin, "OCCViewer", "trihedron_size" );
  pref->addPreference( tr( "PREF_VIEWER_BACKGROUND" ), occGroup,
		       LightApp_Preferences::Color, "OCCViewer", "background" );

  pref->setItemProperty( "min", 1.0E-06, occTS );
  pref->setItemProperty( "max", 1000, occTS );

  int isoU = pref->addPreference( tr( "PREF_ISOS_U" ), occGroup,
				  LightApp_Preferences::IntSpin, "OCCViewer", "iso_number_u" );
  int isoV = pref->addPreference( tr( "PREF_ISOS_V" ), occGroup,
				  LightApp_Preferences::IntSpin, "OCCViewer", "iso_number_v" );

  pref->setItemProperty( "min", 0, isoU );
  pref->setItemProperty( "max", 100000, isoU );

  pref->setItemProperty( "min", 0, isoV );
  pref->setItemProperty( "max", 100000, isoV );

  int vtkTS = pref->addPreference( tr( "PREF_TRIHEDRON_SIZE" ), vtkGroup,
				   LightApp_Preferences::DblSpin, "VTKViewer", "trihedron_size" );
  pref->addPreference( tr( "PREF_VIEWER_BACKGROUND" ), vtkGroup,
		       LightApp_Preferences::Color, "VTKViewer", "background" );
  pref->addPreference( tr( "PREF_RELATIVE_SIZE" ), vtkGroup, LightApp_Preferences::Bool, "VTKViewer", "relative_size" );

  pref->setItemProperty( "min", 1.0E-06, vtkTS );
  pref->setItemProperty( "max", 150, vtkTS );

  pref->addPreference( tr( "PREF_SHOW_LEGEND" ), plot2dGroup,
		       LightApp_Preferences::Bool, "Plot2d", "ShowLegend" );

  int legendPosition = pref->addPreference( tr( "PREF_LEGEND_POSITION" ), plot2dGroup,
					    LightApp_Preferences::Selector, "Plot2d", "LegendPos" );
  QStringList aLegendPosList;
  aLegendPosList.append( tr("PREF_LEFT") );
  aLegendPosList.append( tr("PREF_RIGHT") );
  aLegendPosList.append( tr("PREF_TOP") );
  aLegendPosList.append( tr("PREF_BOTTOM") );

  QList<QVariant> anIndexesList;
  anIndexesList.append(0);
  anIndexesList.append(1);
  anIndexesList.append(2);
  anIndexesList.append(3);

  pref->setItemProperty( "strings", aLegendPosList, legendPosition );
  pref->setItemProperty( "indexes", anIndexesList, legendPosition );

  int curveType = pref->addPreference( tr( "PREF_CURVE_TYPE" ), plot2dGroup,
				       LightApp_Preferences::Selector, "Plot2d", "CurveType" );
  QStringList aCurveTypesList;
  aCurveTypesList.append( tr("PREF_POINTS") );
  aCurveTypesList.append( tr("PREF_LINES") );
  aCurveTypesList.append( tr("PREF_SPLINE") );

  anIndexesList.clear();
  anIndexesList.append(0);
  anIndexesList.append(1);
  anIndexesList.append(2);

  pref->setItemProperty( "strings", aCurveTypesList, curveType );
  pref->setItemProperty( "indexes", anIndexesList, curveType );

  int markerSize = pref->addPreference( tr( "PREF_MARKER_SIZE" ), plot2dGroup,
					LightApp_Preferences::IntSpin, "Plot2d", "MarkerSize" );

  pref->setItemProperty( "min", 0, markerSize );
  pref->setItemProperty( "max", 100, markerSize );

  QStringList aScaleModesList;
  aScaleModesList.append( tr("PREF_LINEAR") );
  aScaleModesList.append( tr("PREF_LOGARITHMIC") );

  anIndexesList.clear();
  anIndexesList.append(0);
  anIndexesList.append(1);

  int horScale = pref->addPreference( tr( "PREF_HOR_AXIS_SCALE" ), plot2dGroup,
				      LightApp_Preferences::Selector, "Plot2d", "HorScaleMode" );

  pref->setItemProperty( "strings", aScaleModesList, horScale );
  pref->setItemProperty( "indexes", anIndexesList, horScale );

  int verScale = pref->addPreference( tr( "PREF_VERT_AXIS_SCALE" ), plot2dGroup,
				      LightApp_Preferences::Selector, "Plot2d", "VerScaleMode" );

  pref->setItemProperty( "strings", aScaleModesList, verScale );
  pref->setItemProperty( "indexes", anIndexesList, verScale );

  pref->addPreference( tr( "PREF_VIEWER_BACKGROUND" ), plot2dGroup,
		       LightApp_Preferences::Color, "Plot2d", "Background" );

  int dirTab = pref->addPreference( tr( "PREF_TAB_DIRECTORIES" ), salomeCat );
  int dirGroup = pref->addPreference( tr( "PREF_GROUP_DIRECTORIES" ), dirTab );
  //pref->setItemProperty( dirGroup, "columns", 1 );
  pref->addPreference( tr( "" ), dirGroup,
		       LightApp_Preferences::DirList, "FileDlg", "QuickDirList" );

  pref->setItemProperty( "columns", 2, supervGroup );
  pref->addPreference( tr( "PREF_VIEWER_BACKGROUND" ), supervGroup,
		       LightApp_Preferences::Color, "SUPERVGraph", "Background" );
  pref->addPreference( tr( "PREF_SUPERV_TITLE_COLOR" ), supervGroup,
		       LightApp_Preferences::Color, "SUPERVGraph", "Title" );
//  pref->addPreference( tr( "PREF_SUPERV_CTRL_COLOR" ), supervGroup,
//		       LightApp_Preferences::Color, "SUPERVGraph", "Ctrl" );

  int obTab = pref->addPreference( tr( "PREF_TAB_OBJBROWSER" ), salomeCat );
  int stGroup = pref->addPreference( tr( "PREF_OBJ_BROWSER_SEARCH_TOOL" ), obTab );
  pref->addPreference( tr( "PREF_AUTO_HIDE_SEARCH_TOOL" ), stGroup, LightApp_Preferences::Bool,
		       "ObjectBrowser", "auto_hide_search_tool" );

  int objSetGroup = pref->addPreference( tr( "PREF_OBJ_BROWSER_SETTINGS" ), obTab );
  pref->setItemProperty( "columns", 2, objSetGroup );
  pref->addPreference( tr( "PREF_AUTO_SIZE_FIRST" ), objSetGroup, LightApp_Preferences::Bool,
		       "ObjectBrowser", "auto_size_first" );
  pref->addPreference( tr( "PREF_AUTO_SIZE" ), objSetGroup, LightApp_Preferences::Bool,
		       "ObjectBrowser", "auto_size" );

  // theme values
  Style_Model* aSModel = 0;
  QStyle* style = qApp->style();
  if ( style ) {
    Style_Salome* aSStyle = qobject_cast<Style_Salome*>( style );
    if ( aSStyle )
      aSModel = aSStyle->getModel();
  }
  if ( !aSModel )
    return;

  QString aSection = aSModel->sectionName();
  int themaTab = pref->addPreference( aSection, salomeCat );
  QtxResourceMgr* aResMgr = pref->resourceMgr();
  aSModel->initFromResource( aResMgr );

  QList<int> aTabLst = aSModel->getTabs();
  QList<int>::iterator aTabIt = aTabLst.begin(), aTabEnd = aTabLst.end();
  QList<int> aGrpLst, aPropLst;
  QList<int>::iterator anIt, anEnd, aPropIt, aPropEnd;
  int aGrpId, aPropId, aPrefId;
  int themaSubTab = pref->addPreference( "ThemeTabs", themaTab,
                                         SUIT_PreferenceMgr::Tab );
  for ( ; aTabIt != aTabEnd; ++aTabIt ) {
    QList<int> aGrpLst = aSModel->getGroups( *aTabIt );
    int themaSubSubTab = pref->addPreference( aSModel->getTabTitle( *aTabIt ), themaSubTab,
					   SUIT_PreferenceMgr::Frame );
    for( anIt = aGrpLst.begin(), anEnd = aGrpLst.end(); anIt != anEnd; ++anIt ) {
      aGrpId = *anIt;
      int themaGroup = pref->addPreference( aSModel->getGroupTitle( aGrpId ), themaSubSubTab, SUIT_PreferenceMgr::GroupBox );
      pref->setItemProperty( "columns", aSModel->getGroupNbColumns( aGrpId ), themaGroup );
      aPropLst = aSModel->getGroupProps( aGrpId );
      for( aPropIt = aPropLst.begin(), aPropEnd = aPropLst.end(); aPropIt != aPropEnd; ++aPropIt ) {
        aPropId = *aPropIt;
        Style_Model::PropType aType = aSModel->getPropType( aPropId );
        LightApp_Preferences::PrefItemType aPType = LightApp_Preferences::Auto;
        switch( aType ) {
          case Style_Model::Bool:     aPType = LightApp_Preferences::Bool; break;
          case Style_Model::Color:    aPType = LightApp_Preferences::Color; break;
          case Style_Model::String:   aPType = LightApp_Preferences::String; break;
          case Style_Model::IntSpin:  aPType = LightApp_Preferences::IntSpin; break;
          case Style_Model::DblSpin:  aPType = LightApp_Preferences::DblSpin; break;
          case Style_Model::Selector: aPType = LightApp_Preferences::Selector; break;
          case Style_Model::Font:     aPType = LightApp_Preferences::Font; break;
          default: break;
        }
        aPrefId = pref->addPreference( aSModel->getPropTitle( aPropId ), themaGroup,
                                       aPType, aSection, aSModel->getPropName( aPropId ) );
        aSModel->getValueTo( aResMgr, aPropId, true );//set default values into resource
        if ( aPType == LightApp_Preferences::Selector ) {
          QStringList lst;
          QList<QVariant> ids;
          aSModel->getSelector( aPropId, lst, ids );
          pref->setItemProperty( "strings", lst, aPrefId );
          pref->setItemProperty( "indexes", ids, aPrefId );
        }
      }
    }
  }
  pref->retrieve();
}

/*!
  Changes appearance of application according to changed preferences
  \param sec - section
  \param param - name of changed parameter
*/
void LightApp_Application::preferencesChanged( const QString& sec, const QString& param )
{
  SUIT_ResourceMgr* resMgr = resourceMgr();
  if ( !resMgr )
    return;

#ifndef DISABLE_OCCVIEWER
  if ( sec == QString( "OCCViewer" ) && param == QString( "trihedron_size" ) )
  {
    double sz = resMgr->doubleValue( sec, param, -1 );
    QList<SUIT_ViewManager*> lst;
    viewManagers( OCCViewer_Viewer::Type(), lst );
    QListIterator<SUIT_ViewManager*> it( lst );
    while ( it.hasNext() && sz >= 0 )
    {
      SUIT_ViewModel* vm = it.next()->getViewModel();
      if ( !vm || !vm->inherits( "OCCViewer_Viewer" ) )
	continue;

      OCCViewer_Viewer* occVM = (OCCViewer_Viewer*)vm;
      occVM->setTrihedronSize( sz );
      occVM->getAISContext()->UpdateCurrentViewer();
    }
  }
#endif

#ifndef DISABLE_VTKVIEWER
  if ( sec == QString( "VTKViewer" ) && (param == QString( "trihedron_size" ) || param == QString( "relative_size" )) )
  {
    double sz = resMgr->doubleValue( "VTKViewer", "trihedron_size", -1 );
    bool isRelative = resMgr->booleanValue( "VTKViewer", "relative_size", true );
    QList<SUIT_ViewManager*> lst;
#ifndef DISABLE_SALOMEOBJECT
    viewManagers( SVTK_Viewer::Type(), lst );
    QListIterator<SUIT_ViewManager*> it( lst );
    while ( it.hasNext() && sz >= 0 )
    {
      SUIT_ViewModel* vm = it.next()->getViewModel();
      if ( !vm || !vm->inherits( "SVTK_Viewer" ) )
	continue;

      SVTK_Viewer* vtkVM = dynamic_cast<SVTK_Viewer*>( vm );
      if( vtkVM )
      {
	vtkVM->setTrihedronSize( sz, isRelative );
	vtkVM->Repaint();
      }
    }
#endif
  }
#endif

#ifndef DISABLE_OCCVIEWER
  if ( sec == QString( "OCCViewer" ) && ( param == QString( "iso_number_u" ) || param == QString( "iso_number_v" ) ) )
  {
    QList<SUIT_ViewManager*> lst;
    viewManagers( OCCViewer_Viewer::Type(), lst );
    int u = resMgr->integerValue( sec, "iso_number_u" );
    int v = resMgr->integerValue( sec, "iso_number_v" );
    QListIterator<SUIT_ViewManager*> it( lst );
    while ( it.hasNext() )
    {
      OCCViewer_ViewManager* mgr = dynamic_cast<OCCViewer_ViewManager*>( it.next() );
      if( mgr && mgr->getOCCViewer() )
	mgr->getOCCViewer()->setIsos( u, v );
    }
  }
#endif

  if( sec=="ObjectBrowser" )
  {
    if( param=="auto_size" || param=="auto_size_first" )
    {
      // temporary commented
      /*SUIT_DataBrowser* ob = objectBrowser();
      if( !ob )
	return;

      bool autoSize = resMgr->booleanValue( "ObjectBrowser", "auto_size", false ),
           autoSizeFirst = resMgr->booleanValue( "ObjectBrowser", "auto_size_first", true );
      ob->setWidthMode( autoSize ? QListView::Maximum : QListView::Manual );
      ob->listView()->setColumnWidthMode( 0, autoSizeFirst ? QListView::Maximum : QListView::Manual );
      if( autoSize )
	for( int i=1; i<ob->listView()->columns(); i++ )
	  if( ob->listView()->columnWidth( i )>0 )
	    ob->listView()->adjustColumn( i );
	    updateObjectBrowser( false );*/
    }
    else if ( param == "auto_hide_search_tool" )
    {
      objectBrowser()->searchTool()->enableAutoHide( resMgr->booleanValue( "ObjectBrowser", "auto_hide_search_tool" ) );
    }
  }

  if( sec=="Study" )
  {
    if( param=="store_positions" )
      updateWindows();
  }

#ifndef DISABLE_PYCONSOLE
  if( sec=="PyConsole" )
  {
    if( param=="font" )
      if( pythonConsole() )
	pythonConsole()->setFont( resMgr->fontValue( "PyConsole", "font" ) );
  }
#endif
  QStyle* style = qApp->style();
  if ( style ) {
    Style_Salome* aSStyle = qobject_cast<Style_Salome*>( style );
    if ( aSStyle ) {
      Style_Model* aSModel = aSStyle->getModel();
      if ( sec==aSModel->sectionName() ) {
        bool retrieve = aSModel->updateFromResource( resMgr, param );
        if ( retrieve && _prefs_ )
          _prefs_->retrieve();
        aSStyle->polish(qApp);
      }
    }
  }
}

/*!
  Loads preferences
*/
void LightApp_Application::loadPreferences()
{
  CAM_Application::loadPreferences();

  SUIT_ResourceMgr* aResMgr = resourceMgr();

  if ( !aResMgr )
    return;

  static bool mru_load = true;
  if ( mru_load )
  {
    QtxMRUAction* mru = ::qobject_cast<QtxMRUAction*>( action( MRUId ) );
    if ( mru )
      mru->loadLinks( aResMgr, "MRU" );
    mru_load = false;
  }

  myWinGeom.clear();
  QStringList mods = aResMgr->parameters( "windows_geometry" );
  for ( QStringList::const_iterator it = mods.begin(); it != mods.end(); ++it )
  {
    QByteArray arr;
    if ( aResMgr->value( "windows_geometry", *it, arr ) )
      myWinGeom.insert( *it, arr );
  }

  myWinVis.clear();
  mods = aResMgr->parameters( "windows_visibility" );
  for ( QStringList::const_iterator itr = mods.begin(); itr != mods.end(); ++itr )
  {
    QByteArray arr;
    if ( aResMgr->value( "windows_visibility", *itr, arr ) )
      myWinVis.insert( *itr, arr );
  }

  if ( desktop() )
    desktop()->retrieveGeometry( aResMgr->stringValue( "desktop", "geometry" ) );
}

/*!
  Saves preferences
*/
void LightApp_Application::savePreferences()
{
  CAM_Application::savePreferences();

  saveDockWindowsState();

  SUIT_ResourceMgr* aResMgr = resourceMgr();

  if ( !aResMgr )
    return;

  QtxMRUAction* mru = ::qobject_cast<QtxMRUAction*>( action( MRUId ) );
  if ( mru )
    mru->saveLinks( aResMgr, "MRU" );

  for ( WinGeom::const_iterator it = myWinGeom.begin(); it != myWinGeom.end(); ++it )
    aResMgr->setValue( "windows_geometry", it.key(), it.value() );

  for ( WinVis::const_iterator itr = myWinVis.begin(); itr != myWinVis.end(); ++itr )
    aResMgr->setValue( "windows_visibility", itr.key(), itr.value() );

  if ( desktop() )
    aResMgr->setValue( "desktop", "geometry", desktop()->storeGeometry() );

  aResMgr->save();
}

/*!
  Updates desktop title
*/
void LightApp_Application::updateDesktopTitle()
{
  QString aTitle = applicationName();
  QString aVer = applicationVersion();
  if ( !aVer.isEmpty() )
    aTitle += QString( " " ) + aVer;

  if ( activeStudy() ) {
    QString sName = SUIT_Tools::file( activeStudy()->studyName().trimmed(), false );
    aTitle += QString( " - [%1]" ).arg( sName );
  }

  desktop()->setWindowTitle( aTitle );
}

/*!
  \brief Get map of the operations which can be performed
  on the module activation.

  The method should return the map of the kind \c {<id>:<name>}
  where \c <id> is an integer identifier of the operation and
  \c <name> is a title for the button to be added to the
  dialog box. After user selects the required operation by the
  clicking the corresponding button in the dialog box, its identifier
  is passed to the moduleActionSelected() method to process
  the made choice.

  \return map of the operations
  \sa moduleActionSelected()
*/
QMap<int, QString> LightApp_Application::activateModuleActions() const
{
  QMap<int, QString> opmap;
  opmap.insert( NewStudyId,  tr( "ACTIVATE_MODULE_OP_NEW" ) );
  opmap.insert( OpenStudyId, tr( "ACTIVATE_MODULE_OP_OPEN" ) );
  return opmap;
}

/*!
  \brief Called when the used selectes required operation chosen
  from "Activate module" dialog box.

  Performs the required operation according to the user choice.

  \param id operation identifier
  \sa activateModuleActions()
*/
void LightApp_Application::moduleActionSelected( const int id )
{
  switch ( id ) {
  case NewStudyId:
    onNewDoc();
    break;
  case OpenStudyId:
    onOpenDoc();
    break;
  default:
    break;
  }
}

/*!
  Updates windows after close document
*/
void LightApp_Application::afterCloseDoc()
{
  updateWindows();

  CAM_Application::afterCloseDoc();
}

/*!
  Updates actions of active module
*/
void LightApp_Application::updateModuleActions()
{
  QString modName;
  if ( activeModule() )
    modName = activeModule()->moduleName();

  LightApp_ModuleAction* moduleAction =
    qobject_cast<LightApp_ModuleAction*>( action( ModulesListId ) );
  if ( moduleAction )
    moduleAction->setActiveModule( modName );
}

/*!
  Gets current windows.
  \param winMap - output current windows map.
*/
void LightApp_Application::currentWindows( QMap<int, int>& winMap ) const
{
  winMap.clear();
  if ( !activeStudy() )
    return;

  if ( activeModule() && activeModule()->inherits( "LightApp_Module" ) )
    ((LightApp_Module*)activeModule())->windows( winMap );
  else
    defaultWindows( winMap );
}

/*!
  Gets current view managers.
  \param lst - output current view managers list.
*/
void LightApp_Application::currentViewManagers( QStringList& lst ) const
{
  lst.clear();
  if ( !activeStudy() )
    return;

  if ( activeModule() && activeModule()->inherits( "LightApp_Module" ) )
    ((LightApp_Module*)activeModule())->viewManagers( lst );
  else
    defaultViewManagers( lst );
}

/*!
  Updates windows
*/
void LightApp_Application::updateWindows()
{
  QMap<int, int> winMap;
  currentWindows( winMap );

  if ( activeStudy() )
  {
    for ( QMap<int, int>::ConstIterator it = winMap.begin(); it != winMap.end(); ++it )
    {
      if ( !dockWindow( it.key() ) )
	getWindow( it.key() );
    }
  }

  for ( WinMap::ConstIterator it = myWin.begin(); it != myWin.end(); ++it )
  {
    QWidget* wid = it.value();
    if ( activeStudy() )
      wid->setVisible( winMap.contains( it.key() ) );
    else
      delete wid;
  }

  if ( activeStudy() )
    loadDockWindowsState();
  else
    myWin.clear();
}

/*!
  Updates view managers
*/
void LightApp_Application::updateViewManagers()
{
  QStringList lst;
  currentViewManagers( lst );

  for ( QStringList::const_iterator it = lst.begin(); it != lst.end(); ++it )
    getViewManager( *it, true );
}

/*!
  Loads windows geometry
*/
void LightApp_Application::loadDockWindowsState()
{
  if ( !desktop() )
    return;

  bool store = resourceMgr()->booleanValue( "Study", "store_positions", true );
  if( !store )
    return;

  QString modName;
  if ( activeModule() )
    modName = activeModule()->name();

  if ( myWinGeom.contains( modName ) )
    desktop()->restoreState( myWinGeom[modName] );

  if ( !myWinVis.contains( modName ) )
    return;

  QMap<QString, bool> tbMap, dwMap;
  dockWindowsState( myWinVis[modName], tbMap, dwMap );

  QList<QToolBar*> tbList = qFindChildren<QToolBar*>( desktop() );
  for ( QList<QToolBar*>::iterator tit = tbList.begin(); tit != tbList.end(); ++tit )
  {
    QToolBar* tb = *tit;

    QObject* po = Qtx::findParent( tb, "QMainWindow" );
    if ( po != desktop() )
      continue;

    if ( tbMap.contains( tb->objectName() ) )
      tb->setVisible( tbMap[tb->objectName()] );
  }

  QList<QDockWidget*> dwList = qFindChildren<QDockWidget*>( desktop() );
  for ( QList<QDockWidget*>::iterator dit = dwList.begin(); dit != dwList.end(); ++dit )
  {
    QDockWidget* dw = *dit;

    QObject* po = Qtx::findParent( dw, "QMainWindow" );
    if ( po != desktop() )
      continue;

    if ( dwMap.contains( dw->objectName() ) )
      dw->setVisible( dwMap[dw->objectName()] );
  }
}

/*!
  Saves windows geometry
*/
void LightApp_Application::saveDockWindowsState()
{
  if ( !desktop() )
    return;

  bool store = resourceMgr()->booleanValue( "Study", "store_positions", true );
  if( !store )
    return;

  QString modName;
  if ( activeModule() )
    modName = activeModule()->name();

  myWinGeom.insert( modName, desktop()->saveState() );

  QByteArray visArr;
  if ( myWinVis.contains( modName ) )
    visArr = myWinVis[modName];

  QMap<QString, bool> tbMap, dwMap;
  dockWindowsState( visArr, tbMap, dwMap );

  QList<QToolBar*> tbList = qFindChildren<QToolBar*>( desktop() );
  for ( QList<QToolBar*>::iterator it = tbList.begin(); it != tbList.end(); ++it )
  {
    QToolBar* tb = *it;
    tbMap.insert( tb->objectName(), tb->toggleViewAction()->isChecked() );
  }

  QList<QDockWidget*> dwList = qFindChildren<QDockWidget*>( desktop() );
  for ( QList<QDockWidget*>::iterator it = dwList.begin(); it != dwList.end(); ++it )
  {
    QDockWidget* wid = *it;
    dwMap.insert( wid->objectName(), wid->toggleViewAction()->isChecked() );
  }

  visArr = dockWindowsState( tbMap, dwMap );

  myWinVis.insert( modName, visArr );
}

QByteArray LightApp_Application::dockWindowsState( const QMap<QString, bool>& tb, const QMap<QString, bool>& dw ) const
{
  QByteArray visArr;
  QDataStream stream( &visArr, QIODevice::WriteOnly );

  stream << (uchar)ToolBarMarker;
  stream << tb.size();
  for ( QMap<QString, bool>::const_iterator tit = tb.begin(); tit != tb.end(); ++tit )
  {
    stream << tit.key();
    stream << (uchar)( tit.value() ? 1 : 0 );
  }

  stream << (uchar)DockWidgetMarker;
  stream << dw.size();
  for ( QMap<QString, bool>::const_iterator wit = dw.begin(); wit != dw.end(); ++wit )
  {
    stream << wit.key();
    stream << (uchar)( wit.value() ? 1 : 0 );
  }

  return visArr;
}

void LightApp_Application::dockWindowsState( const QByteArray& arr, QMap<QString, bool>& tb, QMap<QString, bool>& dw ) const
{
  tb.clear();
  dw.clear();

  QByteArray visArr = arr;
  QDataStream stream( &visArr, QIODevice::ReadOnly );

  uchar marker;
  stream >> marker;
  if ( marker != ToolBarMarker )
    return;

  int lines;
  stream >> lines;
  for ( int i = 0; i < lines; ++i )
  {
    QString objectName;
    stream >> objectName;
    uchar shown;
    stream >> shown;
    tb.insert( objectName, shown );
  }

  stream >> marker;
  if ( marker != DockWidgetMarker )
    return;

  stream >> lines;
  for ( int j = 0; j < lines; ++j )
  {
    QString objectName;
    stream >> objectName;
    uchar shown;
    stream >> shown;
    dw.insert( objectName, shown );
  }
}

/*!
  Adds icon names for modules
*/
void LightApp_Application::moduleIconNames( QMap<QString, QString>& iconMap ) const
{
  iconMap.clear();

  SUIT_ResourceMgr* resMgr = resourceMgr();
  if ( !resMgr )
    return;

  QStringList modList;
  modules( modList, false );

  for ( QStringList::const_iterator it = modList.begin(); it != modList.end(); ++it )
  {
    QString modName = *it;
    QString modIntr = moduleName( modName );
    QString modIcon = resMgr->stringValue( modIntr, "icon", QString() );

    if ( modIcon.isEmpty() )
      continue;

    if ( SUIT_Tools::extension( modIcon ).isEmpty() )
      modIcon += QString( ".png" );

    iconMap.insert( modName, modIcon );
  }
}

/*!
  Inserts items in popup, which necessary for current application
*/
void LightApp_Application::contextMenuPopup( const QString& type, QMenu* thePopup, QString& title )
{
  CAM_Application::contextMenuPopup( type, thePopup, title );

  SUIT_DataBrowser* ob = objectBrowser();
  if ( ob && type == ob->popupClientType() ) {
    thePopup->addSeparator();
    QAction* a = thePopup->addAction( tr( "MEN_REFRESH" ), this, SLOT( onRefresh() ) );
    if ( ob->updateKey() )
      a->setShortcut( ob->updateKey() );
  }
}

/*!
  Create empty study
*/
void LightApp_Application::createEmptyStudy()
{
  CAM_Application::createEmptyStudy();

  if ( objectBrowser() )
    objectBrowser()->updateTree();
}

/*!
  Activates module
  \param mod - module to be activated
*/
bool LightApp_Application::activateModule( CAM_Module* mod )
{
  bool res = CAM_Application::activateModule( mod );

  if ( objectBrowser() )
    objectBrowser()->updateTree();

  return res;
}

/*!
  \return keyborad accelerators manager object
*/
SUIT_Accel* LightApp_Application::accel() const
{
  return myAccel;
}

/*!
  Removes dead widget container from map
*/
void LightApp_Application::onWCDestroyed( QObject* ob )
{
  // remove destroyed widget container from windows map
  for ( WinMap::ConstIterator itr = myWin.begin(); itr != myWin.end(); ++itr )
  {
    if ( itr.value() != ob )
      continue;

    int key = itr.key();
    myWin.remove( key );
    break;
  }
}

void LightApp_Application::onMRUActivated( const QString& name )
{
  SUIT_Session* s = SUIT_Session::session();
  if ( s && s->activeApplication() == this )
    onOpenDoc( name );
}

/*!
  Connects just added view manager
*/
void LightApp_Application::addViewManager( SUIT_ViewManager* vm )
{
  connect( vm, SIGNAL( lastViewClosed( SUIT_ViewManager* ) ),
           this, SLOT( onCloseView( SUIT_ViewManager* ) ) );
  STD_Application::addViewManager( vm );
}

/*!
  Remove view manager from memory
*/
void LightApp_Application::removeViewManager( SUIT_ViewManager* vm )
{
  disconnect( vm, SIGNAL( lastViewClosed( SUIT_ViewManager* ) ),
           this, SLOT( onCloseView( SUIT_ViewManager* ) ) );
  STD_Application::removeViewManager( vm );
  delete vm;
}

/*!
  Renames active window of desktop
*/
void LightApp_Application::onRenameWindow()
{
  if( !desktop() )
    return;

  QWidget* w = desktop()->activeWindow();
  if( !w )
    return;

  bool ok;
  QString name = QInputDialog::getText( w, tr( "TOT_RENAME" ), tr( "PRP_RENAME" ), QLineEdit::Normal, w->windowTitle(), &ok );
  if( ok && !name.isEmpty() )
    w->setWindowTitle( name );
}

/*!
  \return if the library of module exists
  \param moduleTitle - title of module
*/
bool LightApp_Application::isLibExists( const QString& moduleTitle ) const
{
  if( moduleTitle.isEmpty() )
    return false;

  QString lib = moduleLibrary( moduleTitle );

  //abd: changed libSalomePyQtGUI to SalomePyQtGUI for WIN32
  bool isPythonModule = lib.contains("SalomePyQtGUI");

  QStringList paths;
#ifdef WIN32
  paths = QString(::getenv( "PATH" )).split( ";", QString::SkipEmptyParts );
#else
  paths = QString(::getenv( "LD_LIBRARY_PATH" )).split( ":", QString::SkipEmptyParts );
#endif

  bool isLibFound = false;
  QStringList::const_iterator anIt = paths.begin(), aLast = paths.end();
  for( ; anIt!=aLast; anIt++ )
  {
    QFileInfo inf( Qtx::addSlash( *anIt ) + lib );

    if( inf.exists() )
      {
	isLibFound = true;
	break;
      }
  }

  if ( !isLibFound )
    {
      printf( "****************************************************************\n" );
      printf( "*    Warning: library %s cannot be found\n", lib.toLatin1().constData() );
      printf( "*    Module will not be available\n" );
      printf( "*    Module %s will not be available in GUI mode\n", moduleTitle.toLatin1().constData() );
      printf( "****************************************************************\n" );
    }
  else if ( !isPythonModule )
    return true;

  if ( isPythonModule )
    {
      QString pylib = moduleName( moduleTitle ) + QString(".py");
      QString pylibgui = moduleName( moduleTitle ) + QString("GUI.py");

      // Check the python library
#ifdef WIN32
      paths = QString(::getenv( "PATH" )).split( ";", QString::SkipEmptyParts );
#else
      paths = QString(::getenv( "PYTHONPATH" )).split( ":", QString::SkipEmptyParts );
#endif
      bool isPyLib = false, isPyGuiLib = false;
      QStringList::const_iterator anIt = paths.begin(), aLast = paths.end();
      for( ; anIt!=aLast; anIt++ )
	{
	  QFileInfo inf( Qtx::addSlash( *anIt ) + pylib );
	  QFileInfo infgui( Qtx::addSlash( *anIt ) + pylibgui );

	  if( !isPyLib && inf.exists() )
	    isPyLib = true;

	  if( !isPyGuiLib && infgui.exists() )
	    isPyGuiLib = true;

	  if ( isPyLib && isPyGuiLib && isLibFound)
	    return true;
	}

      printf( "****************************************************************\n" );
      printf( "*    Warning: python library for %s cannot be found:\n", moduleTitle.toLatin1().constData() );
      if (!isPyLib)
	printf( "*    No module named %s\n", moduleName( moduleTitle ).toLatin1().constData() );
      if (!isPyGuiLib)
	printf( "*    No module named %s\n", (moduleName( moduleTitle ) + QString("GUI")).toLatin1().constData() );
      printf( "****************************************************************\n" );
      return true;
  }
  return false;
}

/*!
  \return default name for an active study
*/
void LightApp_Application::setDefaultStudyName( const QString& theName )
{
  QStringList anInfoList;
  modules( anInfoList, false );

  LightApp_Study* aStudy = (LightApp_Study*)activeStudy();
  if( anInfoList.count() == 1 && // to avoid a conflict between different modules
      !aStudy->isSaved() )
  {
    aStudy->setStudyName( theName );
    updateDesktopTitle();
  }
}

/*!
  Custom event handler
*/
bool LightApp_Application::event( QEvent* e )
{
  if( e && e->type()==2000 )
  {
    SALOME_CustomEvent* ce = ( SALOME_CustomEvent* )e;
    QString* d = ( QString* )ce->data();
    if( SUIT_MessageBox::question(0, tr("WRN_WARNING"),
				  d ? *d : "",
				  SUIT_MessageBox::Yes | SUIT_MessageBox::No,
				  SUIT_MessageBox::Yes ) == SUIT_MessageBox::Yes )
       onPreferences();
    if( d )
      delete d;
    return true;
  }
  return CAM_Application::event( e );
}

/*! Check data object */
bool LightApp_Application::checkDataObject(LightApp_DataObject* theObj)
{
  if (theObj)
    {
      bool isSuitable =	!theObj->entry().isEmpty() &&
	                !theObj->componentDataType().isEmpty() &&
	                !theObj->name().isEmpty();
      return isSuitable;
    }

  return false;
}

int LightApp_Application::openChoice( const QString& aName )
{
  int choice = CAM_Application::openChoice( aName );

  if ( choice == OpenExist ) // The document is already open.
  {
    // Do you want to reload it?
    if ( SUIT_MessageBox::question( desktop(), tr( "WRN_WARNING" ), tr( "QUE_DOC_ALREADYOPEN" ).arg( aName ),
				    SUIT_MessageBox::Yes | SUIT_MessageBox::No, SUIT_MessageBox::No ) == SUIT_MessageBox::Yes )
      choice = OpenReload;
  }

  return choice;
}

bool LightApp_Application::openAction( const int choice, const QString& aName )
{
  bool res = false;
  switch ( choice )
  {
  case OpenReload:
    {
      STD_Application* app = 0;
      SUIT_Session* session = SUIT_Session::session();
      QList<SUIT_Application*> appList = session->applications();
      for ( QList<SUIT_Application*>::iterator it = appList.begin(); it != appList.end() && !app; ++it )
      {
	if ( (*it)->activeStudy() && (*it)->activeStudy()->studyName() == aName )
	  app = ::qobject_cast<STD_Application*>( *it );
      }

      if ( app )
      {
	app->onCloseDoc( false );
	appList = session->applications();
	STD_Application* other = 0;
	for ( QList<SUIT_Application*>::iterator it = appList.begin(); it != appList.end() && !other; ++it )
	  other = ::qobject_cast<STD_Application*>( *it );

	if ( other )
	  res = other->onOpenDoc( aName );
      }
    }
    break;
  default:
    res = CAM_Application::openAction( choice, aName );
    break;
  }

  return res;
}
