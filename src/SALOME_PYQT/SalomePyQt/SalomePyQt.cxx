//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#include "SALOME_PYQT_Module.h" // this include must be first!!!
#include "SalomePyQt.h"

#include <qapplication.h>
#include <qmenubar.h>
#include <qwidget.h>
#include <qpopupmenu.h>
#include <qimage.h>
#include <qstringlist.h>
#include <qaction.h>

#include "SALOME_Event.hxx"

#include "SUIT_Session.h"
#include "SUIT_Desktop.h"
#include "SUIT_ResourceMgr.h"
#include "SUIT_Tools.h"
#include "STD_MDIDesktop.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"
#include "LightApp_SelectionMgr.h"
#include "OB_Browser.h"
//#include "QtxAction.h"
#include "LogWindow.h"
#include <STD_TabDesktop.h>
#include <OCCViewer_ViewWindow.h>
#include <QtxWorkstack.h>
#include <Plot2d_ViewManager.h>
#include <Plot2d_ViewWindow.h>

using namespace std;


/*!
 * Macro declaring Execute() method calling a method of SALOME_PYQT_Module
 */
#define EXEC_PYQT_MODULE_CALL(meth_call)                                  \
virtual void Execute() {                                                  \
  if ( SalomeApp_Application* anApp = getApplication() ) {                \
    SALOME_PYQT_Module* module = SALOME_PYQT_Module::getInitModule();     \
    if ( !module )                                                        \
      module = dynamic_cast<SALOME_PYQT_Module*>( anApp->activeModule() );\
    if ( module )                                                         \
      myResult = (TResult)module->meth_call;                              \
  }}

/*!
  \return active application object [ static ]
*/
static SalomeApp_Application* getApplication() {
  if ( SUIT_Session::session() )
    return dynamic_cast<SalomeApp_Application*>( SUIT_Session::session()->activeApplication() );
  return NULL;
}

/*!
  \return active study or 0 if there is no study opened [ static ]
*/
static SalomeApp_Study* getActiveStudy()
{
  if ( getApplication() )
    return dynamic_cast<SalomeApp_Study*>( getApplication()->activeStudy() );
  return 0;
}

static QMap<SalomeApp_Application*, SALOME_Selection*> SelMap;

/*!
  Creates or finds the selection object (one per study).
*/
SALOME_Selection* SALOME_Selection::GetSelection( SalomeApp_Application* app )
{
  SALOME_Selection* sel = 0;
  if ( app && SelMap.find( app ) != SelMap.end() )
    sel = SelMap[ app ];
  else 
    sel = SelMap[ app ] = new SALOME_Selection( app );
  return sel;
}

/*!
  Selection constructor.
*/
SALOME_Selection::SALOME_Selection( QObject* p ) : QObject( 0 ), mySelMgr( 0 )
{
  SalomeApp_Application* app = dynamic_cast<SalomeApp_Application*>( p );
  if ( app ) {
    mySelMgr = app->selectionMgr();
    connect( mySelMgr, SIGNAL( selectionChanged() ), this, SIGNAL( currentSelectionChanged() ) );
    connect( mySelMgr, SIGNAL( destroyed() ),        this, SLOT  ( onSelMgrDestroyed() ) );
  }
}
/*!
  Selection destructor. Removes selection object from the map.
*/
SALOME_Selection::~SALOME_Selection()
{
  SalomeApp_Application* app = 0;
  QMap<SalomeApp_Application*, SALOME_Selection*>::Iterator it;
  for ( it = SelMap.begin(); it != SelMap.end() && !app; ++it ) {
    if ( it.data() == this ) app = it.key();
  }
  if ( app ) SelMap.remove( app );
}

/*!
  Watches for the selection manager destroying when study is closed.
*/
void SALOME_Selection::onSelMgrDestroyed()
{
  mySelMgr = 0;
}

/*!
  Clears the selection.
*/
void SALOME_Selection::Clear()
{
  class TEvent: public SALOME_Event {
    LightApp_SelectionMgr* mySelMgr;
  public:
    TEvent( LightApp_SelectionMgr* selMgr ) 
      : mySelMgr( selMgr ) {}
    virtual void Execute() {
      if ( mySelMgr )
        mySelMgr->clearSelected();
    }
  };
  ProcessVoidEvent( new TEvent( mySelMgr ) );
}

/*!
  Clears the selection.
*/
void SALOME_Selection::ClearIObjects()
{
  Clear();
}

/*!
  Removes all selection filters.
*/
void SALOME_Selection::ClearFilters()
{
  class TEvent: public SALOME_Event {
    LightApp_SelectionMgr* mySelMgr;
  public:
    TEvent( LightApp_SelectionMgr* selMgr ) 
      : mySelMgr( selMgr ) {}
    virtual void Execute() {
      if ( mySelMgr )
        mySelMgr->clearFilters();
    }
  };
  ProcessVoidEvent( new TEvent( mySelMgr ) );
}

/*!
  \return desktop (0 if error)
*/
class TGetDesktopEvent: public SALOME_Event {
public:
  typedef QWidget* TResult;
  TResult myResult;
  TGetDesktopEvent() : myResult( 0 ) {}
  virtual void Execute() {
    if ( getApplication() )
      myResult = (QWidget*)( getApplication()->desktop() );
  }
};

/*!
  \return desktop
*/
QWidget* SalomePyQt::getDesktop()
{
  return ProcessEvent( new TGetDesktopEvent() );
}

/*!
  \return workspace widget (0 if error)
*/
class TGetMainFrameEvent: public SALOME_Event {
public:
  typedef QWidget* TResult;
  TResult myResult;
  TGetMainFrameEvent() : myResult( 0 ) {}
  virtual void Execute() {
    if ( getApplication() ) {
      SUIT_Desktop* aDesktop = getApplication()->desktop();
      myResult = (QWidget*)( aDesktop->centralWidget() );
    }
  }
};

/*!
  \return workspace widget (0 if error)
*/
QWidget* SalomePyQt::getMainFrame()
{
  return ProcessEvent( new TGetMainFrameEvent() );
}

/*!
  SalomePyQt::getMainMenuBar
  Gets main menu. Returns 0 in error.
*/
class TGetMainMenuBarEvent: public SALOME_Event {
public:
  typedef QMenuBar* TResult;
  TResult myResult;
  TGetMainMenuBarEvent() : myResult( 0 ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      myResult = anApp->desktop()->menuBar();
    }
  }
};

/*!
  \return main menu
  \retval 0 in error.
*/
QMenuBar* SalomePyQt::getMainMenuBar() 
{
  return ProcessEvent( new TGetMainMenuBarEvent() );
}

/*!
  SalomePyQt::getPopupMenu
  Gets an main menu's child popup menu by its id
*/
class TGetPopupMenuEvent: public SALOME_Event {
public:
  typedef QPopupMenu* TResult;
  TResult  myResult;
  MenuName myMenuName;
  TGetPopupMenuEvent( const MenuName menu ) : myResult( 0 ), myMenuName( menu ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      QMenuBar* menuBar = anApp->desktop()->menuBar();
      if ( menuBar ) {
        QString menu;
        switch( myMenuName) {
        case File:
          menu = QObject::tr( "MEN_DESK_FILE" );        break;
        case View:
          menu = QObject::tr( "MEN_DESK_VIEW" );        break;
        case Edit:
          menu = QObject::tr( "MEN_DESK_EDIT" );        break;
        case Preferences:
          menu = QObject::tr( "MEN_DESK_PREFERENCES" ); break;
        case Tools:
          menu = QObject::tr( "MEN_DESK_TOOLS" );       break;
        case Window:
          menu = QObject::tr( "MEN_DESK_WINDOW" );      break;
        case Help:
          menu = QObject::tr( "MEN_DESK_HELP" );        break;
        }
        for ( int i = 0; i < menuBar->count() && !myResult; i++ ) {
          QMenuItem* item = menuBar->findItem( menuBar->idAt( i ) );
	  if ( item && item->text() == menu && item->popup() )
            myResult = item->popup();
        }
      }
    }
  }
};

/*!
  \return popup menu
  \param menu - menu name
*/
QPopupMenu* SalomePyQt::getPopupMenu( const MenuName menu )
{
  return ProcessEvent( new TGetPopupMenuEvent( menu ) );
}

/*!
  SalomePyQt::getStudyId
  Returns active study's ID or 0 if there is no active study.
*/
class TGetStudyIdEvent: public SALOME_Event {
public:
  typedef int TResult;
  TResult myResult;
  TGetStudyIdEvent() : myResult( 0 ) {}
  virtual void Execute() {
    if ( SalomeApp_Study* aStudy = getActiveStudy() ) {
      myResult = aStudy->studyDS()->StudyId();
    }
  }
};

/*!
  SalomePyQt::getStudyId
  Returns active study's ID or 0 if there is no active study.
*/
int SalomePyQt::getStudyId()
{
  return ProcessEvent( new TGetStudyIdEvent() );
}

/*!
  SalomePyQt::getSelection
  Creates a Selection object (to provide a compatibility with previous SALOME GUI).
*/
class TGetSelectionEvent: public SALOME_Event {
public:
  typedef SALOME_Selection* TResult;
  TResult myResult;
  TGetSelectionEvent() : myResult( 0 ) {}
  virtual void Execute() {
    myResult = SALOME_Selection::GetSelection( getApplication() );
  }
};

/*!
  Creates a Selection object (to provide a compatibility with previous SALOME GUI).
  \return just created selection object
*/
SALOME_Selection* SalomePyQt::getSelection()
{
  return ProcessEvent( new TGetSelectionEvent() );
}

/*!
  SalomePyQt::putInfo
  Puts an information message to the desktop's status bar
  (with optional delay parameter given in seconds)
*/
class TPutInfoEvent: public SALOME_Event {
  QString myMsg;
  int     mySecs;
public:
  TPutInfoEvent( const QString& msg, const int sec = 0 ) : myMsg( msg ), mySecs( sec ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      anApp->putInfo( myMsg, mySecs * 1000 );
    }
  }
};

/*!
  Puts an information message to the desktop's status bar
  (with optional delay parameter given in seconds)
  \param msg - message text 
  \param sec - delay in seconds
*/
void SalomePyQt::putInfo( const QString& msg, const int sec )
{
  ProcessVoidEvent( new TPutInfoEvent( msg, sec ) );
}

/*!
  SalomePyQt::getActiveComponent
  Returns an active component name or empty string if there is no active component
*/
class TGetActiveComponentEvent: public SALOME_Event {
public:
  typedef QString TResult;
  TResult myResult;
  TGetActiveComponentEvent() {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      if ( CAM_Module* mod = anApp->activeModule() ) {
        myResult = mod->name("");
      }
    }
  }
};

/*!
  \return an active component name or empty string if there is no active component
*/
const QString SalomePyQt::getActiveComponent()
{
  return ProcessEvent( new TGetActiveComponentEvent() );
}

/*!
  SalomePyQt::updateObjBrowser
  Updates an Object Browser of a given study.
  If <studyId> <= 0 the active study's object browser is updated.
  <updateSelection> parameter is obsolete parameter and currently not used. To be removed lately.
*/
void SalomePyQt::updateObjBrowser( const int studyId, bool updateSelection )
{  
  class TEvent: public SALOME_Event {
    int  myStudyId;
    bool myUpdateSelection;
  public:
    TEvent( const int studyId, bool updateSelection ) 
      : myStudyId( studyId ), myUpdateSelection( updateSelection ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        if ( getActiveStudy() && myStudyId <= 0 )
          myStudyId = getActiveStudy()->id();
	if ( myStudyId > 0 ) {
          QPtrList<SUIT_Application> apps = SUIT_Session::session()->applications();
          QPtrListIterator<SUIT_Application> it( apps );
	  for( ; it.current(); ++it ) {
            SalomeApp_Application* anApp = dynamic_cast<SalomeApp_Application*>( it.current() );
            if ( anApp && anApp->activeStudy() && anApp->activeStudy()->id() == myStudyId )
	      anApp->updateObjectBrowser();
          }
        }
      }
    }
  };
  ProcessVoidEvent( new TEvent( studyId, updateSelection ) );
}

const char* DEFAULT_SECTION = "SalomePyQt";

/*!
  SalomePyQt::addStringSetting
  Adds a string setting to the application preferences
  <autoValue> parameter is obsolete parameter and currently not used. To be removed lately.
  This function is obsolete. Use addSetting() instead.
*/
void SalomePyQt::addStringSetting( const QString& name, const QString& value, bool autoValue )
{
  class TEvent: public SALOME_Event {
    QString myName;
    QString myValue;
    bool    myAutoValue;
  public:
    TEvent( const QString& name, const QString& value, bool autoValue ) 
      : myName( name ), myValue( value ), myAutoValue( autoValue ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	QStringList sl = QStringList::split( ":", myName );
	QString _sec = sl.count() > 1 ? sl[ 0 ].stripWhiteSpace() : QString( DEFAULT_SECTION );
	QString _nam = sl.count() > 1 ? sl[ 1 ].stripWhiteSpace() : sl.count() > 0 ? sl[ 0 ].stripWhiteSpace() : QString( "" );
	if ( !_sec.isEmpty() && !_nam.isEmpty() )
          resMgr->setValue( _sec, _nam, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( name, value, autoValue ) );
}

/*!
  SalomePyQt::addIntSetting
  Adds an integer setting to the application preferences
  <autoValue> parameter is obsolete parameter and currently not used. To be removed lately.
  This function is obsolete. Use addSetting() instead.
*/
void SalomePyQt::addIntSetting( const QString& name, const int value, bool autoValue)
{
  class TEvent: public SALOME_Event {
    QString myName;
    int     myValue;
    bool    myAutoValue;
  public:
    TEvent( const QString& name, const int value, bool autoValue ) 
      : myName( name ), myValue( value ), myAutoValue( autoValue ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	QStringList sl = QStringList::split( ":", myName );
	QString _sec = sl.count() > 1 ? sl[ 0 ].stripWhiteSpace() : QString( DEFAULT_SECTION );
	QString _nam = sl.count() > 1 ? sl[ 1 ].stripWhiteSpace() : sl.count() > 0 ? sl[ 0 ].stripWhiteSpace() : QString( "" );
	if ( !_sec.isEmpty() && !_nam.isEmpty() )
          resMgr->setValue( _sec, _nam, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( name, value, autoValue ) );
}

/*!
  SalomePyQt::addDoubleSetting
  Adds an double setting to the application preferences
  <autoValue> parameter is obsolete parameter and currently not used. To be removed lately.
  This function is obsolete. Use addSetting() instead.
*/
void SalomePyQt::addDoubleSetting( const QString& name, const double value, bool autoValue )
{
  class TEvent: public SALOME_Event {
    QString myName;
    double  myValue;
    bool    myAutoValue;
  public:
    TEvent( const QString& name, const double value, bool autoValue ) 
      : myName( name ), myValue( value ), myAutoValue( autoValue ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	QStringList sl = QStringList::split( ":", myName );
	QString _sec = sl.count() > 1 ? sl[ 0 ].stripWhiteSpace() : QString( DEFAULT_SECTION );
	QString _nam = sl.count() > 1 ? sl[ 1 ].stripWhiteSpace() : sl.count() > 0 ? sl[ 0 ].stripWhiteSpace() : QString( "" );
	if ( !_sec.isEmpty() && !_nam.isEmpty() )
          resMgr->setValue( _sec, _nam, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( name, value, autoValue ) );
}

/*!
  SalomePyQt::addBoolSetting
  Adds an boolean setting to the application preferences
  <autoValue> parameter is obsolete parameter and currently not used. To be removed lately.
  This function is obsolete. Use addSetting() instead.
*/
void SalomePyQt::addBoolSetting( const QString& name, const bool value, bool autoValue )
{
  class TEvent: public SALOME_Event {
    QString myName;
    bool    myValue;
    bool    myAutoValue;
  public:
    TEvent( const QString& name, const bool value, bool autoValue ) 
      : myName( name ), myValue( value ), myAutoValue( autoValue ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	QStringList sl = QStringList::split( ":", myName );
	QString _sec = sl.count() > 1 ? sl[ 0 ].stripWhiteSpace() : QString( DEFAULT_SECTION );
	QString _nam = sl.count() > 1 ? sl[ 1 ].stripWhiteSpace() : sl.count() > 0 ? sl[ 0 ].stripWhiteSpace() : QString( "" );
	if ( !_sec.isEmpty() && !_nam.isEmpty() )
          resMgr->setValue( _sec, _nam, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( name, value, autoValue ) );
}

/*!
  SalomePyQt::removeSettings
  Removes a setting from the application preferences
  This function is obsolete. Use removeSetting() instead.
*/
void SalomePyQt::removeSettings( const QString& name )
{
  class TEvent: public SALOME_Event {
    QString myName;
  public:
    TEvent( const QString& name ) : myName( name ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	QStringList sl = QStringList::split( ":", myName );
	QString _sec = sl.count() > 1 ? sl[ 0 ].stripWhiteSpace() : QString( DEFAULT_SECTION );
	QString _nam = sl.count() > 1 ? sl[ 1 ].stripWhiteSpace() : sl.count() > 0 ? sl[ 0 ].stripWhiteSpace() : QString( "" );
	if ( !_sec.isEmpty() && !_nam.isEmpty() )
          resMgr->remove( _sec, _nam );
      }
    }
  };
  ProcessVoidEvent( new TEvent( name ) );
}

/*!
  SalomePyQt::getSetting
  Gets a setting value (as string)
  This function is obsolete. Use stringSetting(), integerSetting(), 
  boolSetting(), stringSetting() or colorSetting() instead.
*/
class TGetSettingEvent: public SALOME_Event {
public:
  typedef QString TResult;
  TResult myResult;
  QString myName;
  TGetSettingEvent( const QString& name ) : myName( name ) {}
  virtual void Execute() {
    if ( SUIT_Session::session() ) {
      SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
      QStringList sl = QStringList::split( ":", myName );
      QString _sec = sl.count() > 1 ? sl[ 0 ].stripWhiteSpace() : QString( DEFAULT_SECTION );
      QString _nam = sl.count() > 1 ? sl[ 1 ].stripWhiteSpace() : sl.count() > 0 ? sl[ 0 ].stripWhiteSpace() : QString( "" );
      myResult = ( !_sec.isEmpty() && !_nam.isEmpty() ) ? resMgr->stringValue( _sec, _nam, "" ) : QString( "" );
    }
  }
};

/*!
  \return a setting value (as string)
  This function is obsolete. Use stringSetting(), integerSetting(), 
  boolSetting(), stringSetting() or colorSetting() instead.
*/
QString SalomePyQt::getSetting( const QString& name )
{
  return ProcessEvent( new TGetSettingEvent( name ) );
}

/*!
  SalomePyQt::addSetting
  Adds a double setting to the application preferences
*/
void SalomePyQt::addSetting( const QString& section, const QString& name, const double value )
{
  class TEvent: public SALOME_Event {
    QString mySection;
    QString myName;
    double  myValue;
  public:
    TEvent( const QString& section, const QString& name, double value ) 
      : mySection( section ), myName( name ), myValue( value ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	if ( !mySection.isEmpty() && !myName.isEmpty() )
          resMgr->setValue( mySection, myName, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( section, name, value ) );
}

/*!
  SalomePyQt::addSetting
  Adds an integer setting to the application preferences
*/
void SalomePyQt::addSetting( const QString& section, const QString& name, const int value )
{
  class TEvent: public SALOME_Event {
    QString mySection;
    QString myName;
    int     myValue;
  public:
    TEvent( const QString& section, const QString& name, int value ) 
      : mySection( section ), myName( name ), myValue( value ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	if ( !mySection.isEmpty() && !myName.isEmpty() )
          resMgr->setValue( mySection, myName, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( section, name, value ) );
}

/*!
  SalomePyQt::addSetting
  Adds a boolean setting to the application preferences
  (note: the last "dumb" parameter is used in order to avoid
  sip compilation error because of conflicting int and bool types)
*/
void SalomePyQt::addSetting( const QString& section, const QString& name, const bool value, const int )
{
  class TEvent: public SALOME_Event {
    QString mySection;
    QString myName;
    bool    myValue;
  public:
    TEvent( const QString& section, const QString& name, bool value ) 
      : mySection( section ), myName( name ), myValue( value ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	if ( !mySection.isEmpty() && !myName.isEmpty() )
          resMgr->setValue( mySection, myName, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( section, name, value ) );
}

/*!
  SalomePyQt::addSetting
  Adds a string setting to the application preferences
*/
void SalomePyQt::addSetting( const QString& section, const QString& name, const QString& value )
{
  class TEvent: public SALOME_Event {
    QString mySection;
    QString myName;
    QString myValue;
  public:
    TEvent( const QString& section, const QString& name, const QString& value ) 
      : mySection( section ), myName( name ), myValue( value ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	if ( !mySection.isEmpty() && !myName.isEmpty() )
          resMgr->setValue( mySection, myName, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( section, name, value ) );
}

/*!
  SalomePyQt::addSetting
  Adds a color setting to the application preferences
*/
void SalomePyQt::addSetting( const QString& section, const QString& name, const QColor& value )
{
  class TEvent: public SALOME_Event {
    QString mySection;
    QString myName;
    QColor  myValue;
  public:
    TEvent( const QString& section, const QString& name, const QColor& value ) 
      : mySection( section ), myName( name ), myValue( value ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	if ( !mySection.isEmpty() && !myName.isEmpty() )
          resMgr->setValue( mySection, myName, myValue );
      }
    }
  };
  ProcessVoidEvent( new TEvent( section, name, value ) );
}

/*!
  SalomePyQt::integerSetting
  Gets an integer setting from the application preferences
*/
class TGetIntSettingEvent: public SALOME_Event {
public:
  typedef int TResult;
  TResult myResult;
  QString mySection;
  QString myName;
  TResult myDefault;
  TGetIntSettingEvent( const QString& section, const QString& name, const int def ) 
    : mySection( section ), myName( name ), myDefault( def ) {}
  virtual void Execute() {
    if ( SUIT_Session::session() ) {
      SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
      myResult = ( !mySection.isEmpty() && !myName.isEmpty() ) ? resMgr->integerValue( mySection, myName, myDefault ) : myDefault;
    }
  }
};

/*!
  \return an integer setting from the application preferences
*/
int SalomePyQt::integerSetting( const QString& section, const QString& name, const int def )
{
  return ProcessEvent( new TGetIntSettingEvent( section, name, def ) );
}

/*!
  SalomePyQt::doubleSetting
  Gets a double setting from the application preferences
*/
class TGetDblSettingEvent: public SALOME_Event {
public:
  typedef double TResult;
  TResult myResult;
  QString mySection;
  QString myName;
  TResult myDefault;
  TGetDblSettingEvent( const QString& section, const QString& name, const double def ) 
    : mySection( section ), myName( name ), myDefault( def ) {}
  virtual void Execute() {
    if ( SUIT_Session::session() ) {
      SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
      myResult = ( !mySection.isEmpty() && !myName.isEmpty() ) ? resMgr->doubleValue( mySection, myName, myDefault ) : myDefault;
    }
  }
};

/*!
  \return an double setting from the application preferences
*/
double SalomePyQt::doubleSetting( const QString& section, const QString& name, const double def )
{
  return ProcessEvent( new TGetDblSettingEvent( section, name, def ) );
}

/*!
  SalomePyQt::boolSetting
  Gets a boolean setting from the application preferences
*/
class TGetBoolSettingEvent: public SALOME_Event {
public:
  typedef bool TResult;
  TResult myResult;
  QString mySection;
  QString myName;
  TResult myDefault;
  TGetBoolSettingEvent( const QString& section, const QString& name, const bool def ) 
    : mySection( section ), myName( name ), myDefault( def ) {}
  virtual void Execute() {
    if ( SUIT_Session::session() ) {
      SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
      myResult = ( !mySection.isEmpty() && !myName.isEmpty() ) ? resMgr->booleanValue( mySection, myName, myDefault ) : myDefault;
    }
  }
};

/*!
  \return an boolean setting from the application preferences
*/
bool SalomePyQt::boolSetting( const QString& section, const QString& name, const bool def )
{
  return ProcessEvent( new TGetBoolSettingEvent( section, name, def ) );
}

/*!
  SalomePyQt::stringSetting
  Gets a string setting from the application preferences
*/
class TGetStrSettingEvent: public SALOME_Event {
public:
  typedef QString TResult;
  TResult myResult;
  QString mySection;
  QString myName;
  TResult myDefault;
  TGetStrSettingEvent( const QString& section, const QString& name, const QString& def ) 
    : mySection( section ), myName( name ), myDefault( def ) {}
  virtual void Execute() {
    if ( SUIT_Session::session() ) {
      SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
      myResult = ( !mySection.isEmpty() && !myName.isEmpty() ) ? resMgr->stringValue( mySection, myName, myDefault ) : myDefault;
    }
  }
};

/*!
  \return an string setting from the application preferences
*/
QString SalomePyQt::stringSetting( const QString& section, const QString& name, const QString& def )
{
  return ProcessEvent( new TGetStrSettingEvent( section, name, def ) );
}

/*!
  SalomePyQt::colorSetting
  Gets a color setting from the application preferences
*/
class TGetColorSettingEvent: public SALOME_Event {
public:
  typedef QColor TResult;
  TResult myResult;
  QString mySection;
  QString myName;
  TResult myDefault;
  TGetColorSettingEvent( const QString& section, const QString& name, const QColor& def ) 
    : mySection( section ), myName( name ), myDefault( def ) {}
  virtual void Execute() {
    if ( SUIT_Session::session() ) {
      SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
      myResult = ( !mySection.isEmpty() && !myName.isEmpty() ) ? resMgr->colorValue( mySection, myName, myDefault ) : myDefault;
    }
  }
};

/*!
  \return a color setting from the application preferences
*/
QColor SalomePyQt::colorSetting ( const QString& section, const QString& name, const QColor& def )
{
  return ProcessEvent( new TGetColorSettingEvent( section, name, def ) );
}

/*!
  SalomePyQt::removeSetting
  Removes a setting from the application preferences
*/
void SalomePyQt::removeSetting( const QString& section, const QString& name )
{
  class TEvent: public SALOME_Event {
    QString mySection;
    QString myName;
  public:
    TEvent( const QString& section, const QString& name ) : mySection( section ), myName( name ) {}
    virtual void Execute() {
      if ( SUIT_Session::session() ) {
        SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
	if ( !mySection.isEmpty() && !myName.isEmpty() )
          resMgr->remove( mySection, myName );
      }
    }
  };
  ProcessVoidEvent( new TEvent( section, name ) );
}

/*!
  SalomePyQt::hasSetting
  Returns True if the settings exists
*/
class THasColorSettingEvent: public SALOME_Event {
public:
  typedef bool TResult;
  TResult myResult;
  QString mySection;
  QString myName;
  THasColorSettingEvent( const QString& section, const QString& name ) 
    : mySection( section ), myName( name ) {}
  virtual void Execute() {
    if ( SUIT_Session::session() ) {
      SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
      myResult = resMgr->hasValue( mySection, myName );
    }
  }
};
bool SalomePyQt::hasSetting( const QString& section, const QString& name )
{
  return ProcessEvent( new THasColorSettingEvent( section, name ) );
}

/*!
  SalomePyQt::getFileName
  Displays 'Open/Save file' dialog box and returns a user's choice (file name)
*/
class TGetFileNameEvent: public SALOME_Event {
public:
  typedef QString TResult;
  TResult     myResult;
  QWidget*    myParent;
  QString     myInitial;
  QStringList myFilters;
  QString     myCaption;
  bool        myOpen;
  TGetFileNameEvent( QWidget*           parent, 
		     const QString&     initial, 
		     const QStringList& filters, 
 		     const QString&     caption,
	 	     bool               open ) 
    : myParent ( parent ), 
      myInitial( initial ), 
      myFilters( filters ), 
      myCaption( caption ), 
      myOpen ( open ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      myResult = anApp->getFileName( myOpen, myInitial, myFilters.join(";;"), myCaption, myParent );
    }
  }
};

/*!
  Displays 'Open/Save file' dialog box and returns a user's choice (file name)
*/
QString SalomePyQt::getFileName( QWidget*           parent, 
				 const QString&     initial, 
				 const QStringList& filters, 
				 const QString&     caption,
				 bool               open )
{
  return ProcessEvent( new TGetFileNameEvent( parent, initial, filters, caption, open ) );
}

/*!
  SalomePyQt::getOpenFileNames
  Displays 'Open files' dialog box and returns a user's choice (a list of file names)
*/
class TGetOpenFileNamesEvent: public SALOME_Event {
public:
  typedef QStringList TResult;
  TResult     myResult;
  QWidget*    myParent;
  QString     myInitial;
  QStringList myFilters;
  QString     myCaption;
  TGetOpenFileNamesEvent( QWidget*           parent, 
			  const QString&     initial, 
			  const QStringList& filters, 
			  const QString&     caption ) 
    : myParent ( parent ), 
      myInitial( initial ), 
      myFilters( filters ), 
      myCaption( caption ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      myResult = anApp->getOpenFileNames( myInitial, myFilters.join(";;"), myCaption, myParent );
    }
  }
};

/*!
  Displays 'Open files' dialog box and returns a user's choice (a list of file names)
*/
QStringList SalomePyQt::getOpenFileNames( QWidget*           parent, 
					  const QString&     initial, 
					  const QStringList& filters, 
					  const QString&     caption )
{
  return ProcessEvent( new TGetOpenFileNamesEvent( parent, initial, filters, caption ) );
}

/*!
  SalomePyQt::getExistingDirectory
  Displays 'Get Directory' dialog box and returns a user's choice (a directory name)
*/
class TGetExistingDirectoryEvent: public SALOME_Event {
public:
  typedef QString TResult;
  TResult     myResult;
  QWidget*    myParent;
  QString     myInitial;
  QString     myCaption;
  TGetExistingDirectoryEvent( QWidget*           parent, 
			      const QString&     initial, 
			      const QString&     caption ) 
    : myParent ( parent ), 
      myInitial( initial ), 
      myCaption( caption ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      myResult = anApp->getDirectory( myInitial, myCaption, myParent );
    }
  }
};

/*!
  Displays 'Get Directory' dialog box and returns a user's choice (a directory name)
*/
QString SalomePyQt::getExistingDirectory( QWidget*       parent,
					  const QString& initial,
					  const QString& caption )
{
  return ProcessEvent( new TGetExistingDirectoryEvent( parent, initial, caption ) );
}

/*!
  \fn QString SalomePyQt::loadIcon( const QString& filename );
  \brief Load an icon from the module resources by the specified file name.
  \param filename icon file name
  \return icon object
*/
class TLoadIconEvent: public SALOME_Event 
{
public:
  typedef QIconSet TResult;
  TResult     myResult;
  QString     myModule;
  QString     myFileName;
  TLoadIconEvent( const QString& module, const QString& filename ) 
    : myModule( module ), 
      myFileName ( filename ) {}
  virtual void Execute() 
  {
    if ( LightApp_Application* anApp = getApplication() ) {
      if ( !myFileName.isEmpty() ) {
	QPixmap pixmap = anApp->resourceMgr()->loadPixmap( myModule, 
                         qApp->translate( myModule.latin1(), 
					  myFileName.latin1() ) );
	if ( !pixmap.isNull() )
	  myResult = QIconSet( pixmap );
      }
    }
  }
};
QIconSet SalomePyQt::loadIcon( const QString& module, const QString& filename )
{
  return ProcessEvent( new TLoadIconEvent( module, filename ) );
}

/*!
  SalomePyQt::helpContext
  Opens external browser to display 'context help' information
  current implementation does nothing.
*/
void SalomePyQt::helpContext( const QString& source, const QString& context ) {
  class TEvent: public SALOME_Event {
    QString mySource;
    QString myContext;
  public:
    TEvent( const QString& source, const QString& context ) 
      : mySource( source ), myContext( context ) {}
    virtual void Execute() {
      if ( /*SalomeApp_Application* anApp =*/ getApplication() ) {
	// VSR: TODO
        // anApp->helpContext( mySource, myContext );
      }
    }
  };
  ProcessVoidEvent( new TEvent( source, context ) );
}

/*!
  SalomePyQt::dumpView
  Dumps the contents of the currently active view to the image file 
  in the given format (JPEG, PNG, BMP are supported)
*/
class TDumpViewEvent: public SALOME_Event {
public:
  typedef bool TResult;
  TResult myResult;
  QString myFileName;
  TDumpViewEvent( const QString& filename ) 
    : myResult ( false ), myFileName( filename ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      SUIT_ViewManager* vm = anApp->activeViewManager();
      if ( vm ) { 
        SUIT_ViewWindow* vw = vm->getActiveView();
	if ( vw ) {
          QImage im = vw->dumpView();
	  if ( !im.isNull() && !myFileName.isEmpty() ) {
            QString fmt = SUIT_Tools::extension( myFileName ).upper();
	    if ( fmt.isEmpty() ) fmt = QString( "BMP" ); // default format
	    if ( fmt == "JPG" )  fmt = "JPEG";
	    myResult = im.save( myFileName, fmt.latin1() );
          }
	}
      }
    }
  }
};

/*!
  Dumps the contents of the currently active view to the image file 
  in the given format (JPEG, PNG, BMP are supported)
*/
bool SalomePyQt::dumpView( const QString& filename )
{
  return ProcessEvent( new TDumpViewEvent( filename ) );
}

/*!
  SalomePyQt::defaultMenuGroup
  Returns default menu group
*/
class TDefMenuGroupEvent: public SALOME_Event {
public:
  typedef int TResult;
  TResult myResult;
  TDefMenuGroupEvent() : myResult( -1 ) {}
  virtual void Execute() {
    myResult = SALOME_PYQT_Module::defaultMenuGroup();
  }
};

/*!
  \return default menu group
*/
int SalomePyQt::defaultMenuGroup()
{
  return ProcessEvent( new TDefMenuGroupEvent() );
}

/*!
  SalomePyQt::createTool
  These methods allow operating with the toolbars:
  - create a new toolbar or get the existing one (the toolbar name is passed as parameter);
    this method returns an id of the toolbar;
  - add action with given id (must be created previously) and optional index to the existing toolbar
    (toobar is identified either by its id or by its name)
    these methods return an id of the action.
  If error occurs, the -1 value is returned.
*/
class CrTool
{
public:
  CrTool( const QString& tBar ) 
    : myCase( 0 ), myTbName( tBar ) {}
  CrTool( const int id, const int tBar, const int idx ) 
    : myCase( 1 ), myId( id ), myTbId( tBar ), myIndex( idx ) {}
  CrTool( const int id, const QString& tBar, const int idx )
    : myCase( 2 ), myId( id ), myTbName( tBar ), myIndex( idx ) {}
  CrTool( QAction* action, const int tbId, const int id, const int idx )
    : myCase( 3 ), myAction( action ), myTbId( tbId ), myId( id ), myIndex( idx ) {}
  CrTool( QAction* action, const QString& tBar, const int id, const int idx )
    : myCase( 4 ), myAction( action ), myTbName( tBar ), myId( id ), myIndex( idx ) {}

  int execute( SALOME_PYQT_Module* module ) const
  {
    if ( module ) {
      switch ( myCase ) {
      case 0:
        return module->createTool( myTbName );
      case 1:
        return module->createTool( myId, myTbId, myIndex );
      case 2:
        return module->createTool( myId, myTbName, myIndex );
      case 3:
        return module->createTool( myAction, myTbId, myId, myIndex );
      case 4:
        return module->createTool( myAction, myTbName, myId, myIndex );
      }
    }
    return -1;
  }
private:
   int        myCase;
   QString    myTbName;
   int        myTbId;
   QAction* myAction;
   int        myId;
   int        myIndex;
};
class TCreateToolEvent: public SALOME_Event {
public:
  typedef int TResult;
  TResult myResult;
  const CrTool& myCrTool;
  TCreateToolEvent( const CrTool& crTool ) 
    : myResult( -1 ), myCrTool( crTool ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      SALOME_PYQT_Module* module = SALOME_PYQT_Module::getInitModule();
      if ( !module )
        module = dynamic_cast<SALOME_PYQT_Module*>( anApp->activeModule() );
      myResult = myCrTool.execute( module );
    }
  }
};

/*!
  create new toolbar or get existing by name 
*/
int SalomePyQt::createTool( const QString& tBar )
{
  return ProcessEvent( new TCreateToolEvent( CrTool( tBar ) ) );
}
/*! add action with id and index to the existing tollbar
*/
int SalomePyQt::createTool( const int id, const int tBar, const int idx )
{
  return ProcessEvent( new TCreateToolEvent( CrTool( id, tBar, idx ) ) );
}
/*! add action with id and index to the existing tollbar
*/
int SalomePyQt::createTool( const int id, const QString& tBar, const int idx )
{
  return ProcessEvent( new TCreateToolEvent( CrTool( id, tBar, idx ) ) );
}
/*! add action with id and index to the existing tollbar
*/
int SalomePyQt::createTool( QAction* a, const int tBar, const int id, const int idx )
{
  return ProcessEvent( new TCreateToolEvent( CrTool( a, tBar, id, idx ) ) );
}
/*! add action with id and index to the existing tollbar
*/
int SalomePyQt::createTool( QAction* a, const QString& tBar, const int id, const int idx )
{
  return ProcessEvent( new TCreateToolEvent( CrTool( a, tBar, id, idx ) ) );
}

/*!
  SalomePyQt::createMenu
  These methods allow operating with the main menu:
  - create a new menu or submenu or get the existing one (the parent menu name or id is passed as parameter, 
    if it is empty or -1, it means that main menu is created, otherwise submenu is created);
    this method returns an id of the menu/submenu;
  - add action with given id (must be created previously) and optional index and group number to the existing menu
    or submenu (menu name or id us passed as parameter)
    these methods return an id of the action.
  If error occurs, the -1 value is returned.
*/
class CrMenu
{
public:
  CrMenu( const QString& subMenu, const int menu, const int id, const int group, const int idx ) 
    : myCase( 0 ), mySubMenuName( subMenu ), myMenuId( menu ), myId( id ), myGroup( group ), myIndex( idx ) {}
  CrMenu( const QString& subMenu, const QString& menu, const int id, const int group, const int idx ) 
    : myCase( 1 ), mySubMenuName( subMenu ), myMenuName( menu ), myId( id ), myGroup( group ), myIndex( idx ) {}
  CrMenu( const int id, const int menu, const int group, const int idx ) 
    : myCase( 2 ), myId( id ), myMenuId( menu ), myGroup( group ), myIndex( idx ) {}
  CrMenu( const int id, const QString& menu, const int group, const int idx ) 
    : myCase( 3 ), myId( id ), myMenuName( menu ), myGroup( group ), myIndex( idx ) {}
  CrMenu( QAction* action, const int menu, const int id, const int group, const int idx ) 
    : myCase( 4 ), myAction( action ), myMenuId( menu ), myId( id ), myGroup( group ), myIndex( idx ) {}
  CrMenu( QAction* action, const QString& menu, const int id, const int group, const int idx ) 
    : myCase( 5 ), myAction( action ), myMenuName( menu ), myId( id ), myGroup( group ), myIndex( idx ) {}

  int execute( SALOME_PYQT_Module* module ) const
  {
    if ( module ) {
      switch ( myCase ) {
      case 0:
        return module->createMenu( mySubMenuName, myMenuId, myId, myGroup, myIndex );
      case 1:
        return module->createMenu( mySubMenuName, myMenuName, myId, myGroup, myIndex );
      case 2:
        return module->createMenu( myId, myMenuId, myGroup, myIndex );
      case 3:
        return module->createMenu( myId, myMenuName, myGroup, myIndex );
      case 4:
        return module->createMenu( myAction, myMenuId, myId, myGroup, myIndex );
      case 5:
        return module->createMenu( myAction, myMenuName, myId, myGroup, myIndex );
      }
    }
    return -1;
  }
private:
   int        myCase;
   QString    myMenuName;
   int        myMenuId;
   QString    mySubMenuName;
   int        myGroup;
   QAction* myAction;
   int        myId;
   int        myIndex;
};
class TCreateMenuEvent: public SALOME_Event {
public:
  typedef int TResult;
  TResult myResult;
  const CrMenu& myCrMenu;
  TCreateMenuEvent( const CrMenu& crMenu ) 
    : myResult( -1 ), myCrMenu( crMenu ) {}
  virtual void Execute() {
    if ( SalomeApp_Application* anApp = getApplication() ) {
      SALOME_PYQT_Module* module = SALOME_PYQT_Module::getInitModule();
      if ( !module )
        module = dynamic_cast<SALOME_PYQT_Module*>( anApp->activeModule() );
      myResult = myCrMenu.execute( module );
    }
  }
};
int SalomePyQt::createMenu( const QString& subMenu, const int menu, const int id, const int group, const int idx )
{
  return ProcessEvent( new TCreateMenuEvent( CrMenu( subMenu, menu, id, group, idx ) ) );
}

int SalomePyQt::createMenu( const QString& subMenu, const QString& menu, const int id, const int group, const int idx )
{
  return ProcessEvent( new TCreateMenuEvent( CrMenu( subMenu, menu, id, group, idx ) ) );
}

int SalomePyQt::createMenu( const int id, const int menu, const int group, const int idx )
{
  return ProcessEvent( new TCreateMenuEvent( CrMenu( id, menu, group, idx ) ) );
}

int SalomePyQt::createMenu( const int id, const QString& menu, const int group, const int idx )
{
  return ProcessEvent( new TCreateMenuEvent( CrMenu( id, menu, group, idx ) ) );
}

int SalomePyQt::createMenu( QAction* a, const int menu, const int id, const int group, const int idx )
{
  return ProcessEvent( new TCreateMenuEvent( CrMenu( a, menu, id, group, idx ) ) );
}

int SalomePyQt::createMenu( QAction* a, const QString& menu, const int id, const int group, const int idx )
{
  return ProcessEvent( new TCreateMenuEvent( CrMenu( a, menu, id, group, idx ) ) );
}

/*!
  SalomePyQt::createSeparator
  Create a separator action which can be then used in the menu or toolbar.
*/
class TCreateSepEvent: public SALOME_Event {
public:
  typedef QAction* TResult;
  TResult myResult;
  TCreateSepEvent() 
    : myResult( 0 ) {}
  EXEC_PYQT_MODULE_CALL(createSeparator());
};
QAction* SalomePyQt::createSeparator()
{
  return ProcessEvent( new TCreateSepEvent() );
}

/*!
  SalomePyQt::createAction
  Create an action which can be then used in the menu or toolbar:
  - id         : the unique id action to be registered to;
  - menuText   : action text which should appear in menu;
  - tipText    : text which should appear in the tooltip;
  - statusText : text which should appear in the status bar when action is activated;
  - icon       : the name of the icon file (the actual icon file name can be coded in the translation files);
  - key        : the key accelrator for the action
  - toggle     : if true the action is checkable
*/
class TCreateActionEvent: public SALOME_Event {
public:
  typedef QAction* TResult;
  TResult myResult;
  int     myId;
  QString myMenuText;
  QString myTipText;
  QString myStatusText;
  QString myIcon;
  int     myKey;
  bool    myToggle;
  TCreateActionEvent( const int id, const QString& menuText, const QString& tipText, 
		      const QString& statusText, const QString& icon, const int key, const bool toggle ) 
    : myResult( 0 ), myId( id ), myMenuText( menuText ), myTipText( tipText ),
      myStatusText( statusText ), myIcon( icon ), myKey( key ), myToggle( toggle ) {}
  EXEC_PYQT_MODULE_CALL(createAction( myId, myTipText, myIcon, myMenuText, myStatusText, myKey, myToggle ));
};
QAction* SalomePyQt::createAction( const int id,           const QString& menuText, 
				     const QString& tipText, const QString& statusText, 
				     const QString& icon,    const int key, const bool toggle )
{
  return ProcessEvent( new TCreateActionEvent( id, menuText, tipText, statusText, icon, key, toggle ) );
}

/*!
  SalomePyQt::createActionGroup
  Create an action group which can be then used in the menu or toolbar:
  - id         : the unique id action to be registered to;
  - toggle     : if true the action group does exclusive toggling
*/
struct TcreateActionGroupEvent: public SALOME_Event {
  typedef QActionGroup* TResult;
  TResult myResult;
  int     myId;
  bool    myExclusive;
  TcreateActionGroupEvent(const int id, const bool exclusive):myId(id),myExclusive(exclusive) {}
  EXEC_PYQT_MODULE_CALL( createActionGroup( myId, myExclusive ));
};

QActionGroup* SalomePyQt::createActionGroup(const int id, const bool exclusive)
{
  return ProcessEvent( new TcreateActionGroupEvent(id, exclusive));
}

/*!
  SalomePyQt::action
  Get an action by its id. Returns 0 if the action with such id was not registered.
*/
class TActionEvent: public SALOME_Event {
public:
  typedef QAction* TResult;
  TResult myResult;
  int     myId;
  TActionEvent( const int id )
    : myResult( 0 ), myId( id ) {}
  EXEC_PYQT_MODULE_CALL(action( myId ));
};
QAction* SalomePyQt::action( const int id )
{
  return ProcessEvent( new TActionEvent( id ) );
}

/*!
  SalomePyQt::actionId
  Get an action id. Returns -1 if the action was not registered.
*/
class TActionIdEvent: public SALOME_Event {
public:
  typedef  int TResult;
  TResult  myResult;
  const QAction* myAction;
  TActionIdEvent( const QAction* action )
    : myResult( -1 ), myAction( action ) {}
  EXEC_PYQT_MODULE_CALL(actionId( myAction ));
};
int SalomePyQt::actionId( const QAction* a )
{
  return ProcessEvent( new TActionIdEvent( a ) );
}

/*!
  SalomePyQt::clearMenu
  Clears given menu (recursively if necessary)
*/
class TClearMenuEvent: public SALOME_Event {
public:
  typedef  bool TResult;
  TResult  myResult;
  int      myId;
  int      myMenu;
  bool     myRemoveActions;
  TClearMenuEvent( const int id, const int menu, const bool removeActions )
    : myResult( false ), myId( id ), myMenu( menu ), myRemoveActions( removeActions ) {}
  EXEC_PYQT_MODULE_CALL(clearMenu( myId, myMenu, myRemoveActions ));
};
bool SalomePyQt::clearMenu( const int id, const int menu, const bool removeActions )
{
  return ProcessEvent( new TClearMenuEvent( id, menu, removeActions ) );
}

/*!
  SalomePyQt::addGlobalPreference
  Adds global (not module) preferences group 
 */
class TAddGlobalPrefEvent: public SALOME_Event {
public:
  typedef int TResult;
  TResult myResult;
  QString myLabel;
  TAddGlobalPrefEvent( const QString& label )
    : myResult( -1 ), myLabel( label ) {}
  EXEC_PYQT_MODULE_CALL(addGlobalPreference( myLabel ));
};
int SalomePyQt::addGlobalPreference( const QString& label )
{
  return ProcessEvent( new TAddGlobalPrefEvent( label ) );
}

/*!
  SalomePyQt::addPreference
  Adds preference 
 */
class TAddPrefEvent: public SALOME_Event {
public:
  typedef int TResult;
  TResult myResult;
  QString myLabel;
  TAddPrefEvent( const QString& label )
    : myResult( -1 ), myLabel( label ) {}
  EXEC_PYQT_MODULE_CALL(addPreference( myLabel ));
};
int SalomePyQt::addPreference( const QString& label )
{
  return ProcessEvent( new TAddPrefEvent( label ) );
}

/*!
  SalomePyQt::addPreference
  Adds preference 
 */
class TAddPrefParamEvent: public SALOME_Event {
public:
  typedef int TResult;
  TResult myResult;
  QString myLabel;
  int     myPId;
  int     myType;
  QString mySection;
  QString myParam;
  TAddPrefParamEvent( const QString& label, 
		      const int pId, const int type,
		      const QString& section, 
		      const QString& param )
    : myResult( -1 ),
      myLabel( label ), myPId( pId ), myType( type ), 
      mySection( section ), myParam ( param ) {}
  EXEC_PYQT_MODULE_CALL(addPreference( myLabel, myPId, myType, mySection, myParam ));
};
int SalomePyQt::addPreference( const QString& label, const int pId, const int type,
			       const QString& section, const QString& param )
{
  return ProcessEvent( new TAddPrefParamEvent( label, pId, type, section, param ) );
}

/*!
  SalomePyQt::preferenceProperty
  Gets the property value for the given (by id) preference
 */
class TPrefPropEvent: public SALOME_Event {
public:
  typedef QVariant TResult;
  TResult myResult;
  int     myId;
  QString myProp;
  TPrefPropEvent( const int id, const QString& prop )
    : myId( id ), myProp( prop )
  { 
    myResult = QVariant();
  }
  EXEC_PYQT_MODULE_CALL(preferenceProperty( myId, myProp ));
};
QVariant SalomePyQt::preferenceProperty( const int id, const QString& prop )
{
  return ProcessEvent( new TPrefPropEvent( id, prop ) );
}

/*!
  SalomePyQt::setPreferenceProperty
  Sets the property value for the given (by id) preference
 */
void SalomePyQt::setPreferenceProperty( const int id, 
					const QString& prop,
					const QVariant& var )
{
  class TEvent: public SALOME_Event {
    int      myId;
    QString  myProp;
    QVariant myVar;
  public:
    TEvent( const int id, const QString& prop, const QVariant& var ) 
      : myId( id ), myProp( prop ), myVar( var ) {}
    virtual void Execute() {
      if ( SalomeApp_Application* anApp = getApplication() ) {
	SALOME_PYQT_Module* module = SALOME_PYQT_Module::getInitModule();
	if ( !module )
	  module = dynamic_cast<SALOME_PYQT_Module*>( anApp->activeModule() );
	if ( module )
	  module->setPreferenceProperty( myId, myProp, myVar );
      }
    }
  };
  ProcessVoidEvent( new TEvent( id, prop, var) );
}

/*!
  SalomePyQt::addPreferenceProperty
  Adds the property value to the list of values 
  for the given (by id) preference

  This method allows creating properties which are QValueList<QVariant>
  - there is no way to pass such values directly to QVariant parameter
  from Python
 */
void SalomePyQt::addPreferenceProperty( const int id, 
					const QString& prop,
					const int idx, 
					const QVariant& var )
{
  class TEvent: public SALOME_Event {
    int      myId;
    QString  myProp;
    int      myIdx;
    QVariant myVar;
  public:
    TEvent( const int id, const QString& prop, const int idx, const QVariant& var ) 
      : myId( id ), myProp( prop ), myIdx( idx), myVar( var ) {}
    virtual void Execute() {
      if ( SalomeApp_Application* anApp = getApplication() ) {
	SALOME_PYQT_Module* module = SALOME_PYQT_Module::getInitModule();
	if ( !module )
	  module = dynamic_cast<SALOME_PYQT_Module*>( anApp->activeModule() );
	if ( module ) {
	  QVariant var =  module->preferenceProperty( myId, myProp );
	  if ( var.isValid() ) {
	    if ( var.type() == QVariant::StringList ) {
	      QStringList sl = var.asStringList();
	      if ( myIdx >= 0 && myIdx < sl.count() ) 
		sl[myIdx] = myVar.asString();
	      else
		sl.append( myVar.asString() );
	      module->setPreferenceProperty( myId, myProp, sl );
	    }
	    else if ( var.type() == QVariant::List ) {
	      QValueList<QVariant> vl = var.asList();
	      if ( myIdx >= 0 && myIdx < vl.count() ) 
		vl[myIdx] = myVar;
	      else
		vl.append( myVar );
	      module->setPreferenceProperty( myId, myProp, vl );
	    }
	  }
	  else {
	    QValueList<QVariant> vl;
	    vl.append( myVar );
	    module->setPreferenceProperty( myId, myProp, vl );
	  }
	}
      }
    }
  };
  ProcessVoidEvent( new TEvent( id, prop, idx, var) );
}

/*!
  SalomePyQt::message
  Puts the message to the Log output window
 */
void SalomePyQt::message( const QString& msg, bool addSeparator )
{
  class TEvent: public SALOME_Event {
    QString  myMsg;
    bool     myAddSep;
  public:
    TEvent( const QString& msg, bool addSeparator ) 
      : myMsg( msg ), myAddSep( addSeparator ) {}
    virtual void Execute() {
      if ( SalomeApp_Application* anApp = getApplication() ) {
	LogWindow* lw = anApp->logWindow();
	if ( lw )
	  lw->putMessage( myMsg, myAddSep );
      }
    }
  };
  ProcessVoidEvent( new TEvent( msg, addSeparator ) );
}

/*!
  SalomePyQt::clearMessages
  Removes all the messages from the Log output window
 */
void SalomePyQt::clearMessages()
{
  class TEvent: public SALOME_Event {
  public:
    TEvent() {}
    virtual void Execute() {
      if ( SalomeApp_Application* anApp = getApplication() ) {
	LogWindow* lw = anApp->logWindow();
	if ( lw )
	  lw->clear();
      }
    }
  };
  ProcessVoidEvent( new TEvent() );
}

/*!
  \brief Gets window with specified identifier 
  \param id window identifier 
  \return pointer on the window
*/
static SUIT_ViewWindow* getWnd( const int id )
{
  SUIT_ViewWindow* resWnd = 0;

  SalomeApp_Application* app  = getApplication();
  if ( app )
  {
    STD_TabDesktop* tabDesk = dynamic_cast<STD_TabDesktop*>( app->desktop() );
    if ( tabDesk )
    {
      QPtrList<SUIT_ViewWindow> list = tabDesk->windows();
      SUIT_ViewWindow* wnd;
      for ( wnd = list.first(); wnd; wnd = list.next() )
      {
        if ( id == wnd->getId() )
        {
          resWnd = wnd;
          break;
        }
      }
    }
  }

  return resWnd;
}

//=============================================================================

/*!
  \class TGetViews
  \brief Gets list of integer identifiers of all the currently opened views
*/
class TGetViews: public SALOME_Event
{
public:
  typedef QValueList<int> TResult;
  TResult myResult;

public:
  /*!
    \brief Constructor.
  */
  TGetViews() 
  {
  }
  /*!
    \brief Gets list of integer identifiers of all the currently opened views
  */
  virtual void Execute() 
  {
    myResult.clear();
    SalomeApp_Application* app  = getApplication();
    if ( app )
    {
      STD_TabDesktop* tabDesk = dynamic_cast<STD_TabDesktop*>( app->desktop() );
      if ( tabDesk )
      {
        QPtrList<SUIT_ViewWindow> list = tabDesk->windows();
        SUIT_ViewWindow* wnd;
        for ( wnd = list.first(); wnd; wnd = list.next() )
          myResult.append( wnd->getId() );
      }
    }
  }
};


/*!
  \brief Gets list of integer identifiers of all the currently opened views
  \return list of integer identifiers of all the currently opened views
*/
QValueList<int> SalomePyQt::getViews()
{
  return ProcessEvent( new TGetViews() );
}

//=============================================================================

/*!
  \class TGetActiveView
  \brief Gets integer identifier of the currently active view
*/
class TGetActiveView: public SALOME_Event
{
public:
  typedef int TResult;
  TResult myResult;

public:
  /*!
    \brief Constructor.
  */
  TGetActiveView()
  : myResult( -1 )
  {
  }
  /*!
    \brief Gets list of integer identifiers of all the currently opened views
  */
  virtual void Execute() 
  {
    SalomeApp_Application* app = getApplication();
    if ( app )
    {
      SUIT_ViewManager* viewMgr = app->activeViewManager();
      if ( viewMgr )
      {
        SUIT_ViewWindow* wnd = viewMgr->getActiveView();
        if ( wnd )
          myResult = wnd->getId();
      }
    }
  }
};


/*!
  \brief Gets integer identifier of the currently active view
  \return integer identifier of the currently active view
*/
int SalomePyQt::getActiveView()
{
  return ProcessEvent( new TGetActiveView() );
}

//=============================================================================

/*!
  \class TGetViewType
  \brief Gets string, which is the view type, e.g. "OCCViewer"
*/
class TGetViewType: public SALOME_Event
{
public:
  typedef QString TResult;
  TResult myResult;
  int myWndId;

public:
  /*!
    \brief Constructor.
    \param id window identifier
  */
  TGetViewType( const int id )
  : myWndId( id )
  {
  }
  /*!
    \brief Gets string, which is the view type, e.g. "OCCViewer"
  */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
    {
      SUIT_ViewManager* viewMgr = wnd->getViewManager();
      if ( viewMgr )
        myResult = viewMgr->getType();
    }
  }
};

/*!                      
  \brief Gets string, which is the view type, e.g. "OCCViewer"
  \param id window identifier
  \return string, which is the view type, e.g. "OCCViewer"
*/ 
QString SalomePyQt::getViewType( const int id )
{
  return ProcessEvent( new TGetViewType( id ) );
}

//=============================================================================

/*!
  \class TSetViewTitle
  \brief Changes view caption  
*/
class TSetViewTitle: public SALOME_Event
{
public:
  typedef bool TResult;
  TResult myResult;
  int myWndId;
  QString myTitle;
  
public:
  /*!
    \brief Constructor.
    \param id window identifier
    \param title new window title
  */
  TSetViewTitle( const int id, const QString& title )
  : myResult( false ),
    myWndId( id ),
    myTitle( title )
  {
  }
  /*!
    \brief Changes view caption  
  */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
    {
      wnd->setCaption( myTitle );
      myResult = true;
    }
  }
};

/*!
  \brief Changes view caption  
  \param id window identifier
  \param title new window title
  \return TRUE if operation is completed successfully, FALSE otherwise 
*/
bool SalomePyQt::setViewTitle( const int id, const QString& title )
{
  return ProcessEvent( new TSetViewTitle( id, title ) );
}

//=============================================================================

/*!
  \class TGetViewTitle
  \brief Gets view caption  
*/
class TGetViewTitle: public SALOME_Event
{
public:
  typedef QString TResult;
  TResult myResult;
  int myWndId;

public:
  /*!
    \brief Constructor.
    \param id window identifier
  */
  TGetViewTitle( const int id )
  : myWndId( id )
  {
  }
  /*!
    \brief Gets view caption  
  */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
      myResult = wnd->caption();
  }
};

/*!
  \brief Gets view caption  
  \param id window identifier
  \return view caption  
*/
QString SalomePyQt::getViewTitle( const int id )
{
  return ProcessEvent( new TGetViewTitle( id ) );
}

//=============================================================================

/*!
  \class TFindViews
  \brief Gets list of integer identifiers of all the 
         currently opened views of the specified type
*/
class TFindViews: public SALOME_Event
{
public:
  typedef QValueList<int> TResult;
  TResult myResult;
  QString myType;

public:
  /*!
    \brief Constructor.
    \param type viewer type
  */
  TFindViews( const QString& type )
  : myType( type )
  {
  }
  /*!
    \brief Gets list of integer identifiers of all the 
           currently opened views of the specified type
  */
  virtual void Execute() 
  {
    myResult.clear();
    SalomeApp_Application* app  = getApplication();
    if ( app )
    {
      ViewManagerList vmList;
      app->viewManagers( myType, vmList );
      SUIT_ViewManager* viewMgr;
      for ( viewMgr = vmList.first(); viewMgr; viewMgr = vmList.next() )
      {
        QPtrVector<SUIT_ViewWindow> vec = viewMgr->getViews();
        for ( int i = 0, n = vec.size(); i < n; i++ )
        {
          SUIT_ViewWindow* wnd = vec[ i ];
          if ( wnd )
            myResult.append( wnd->getId() );
        }
      }
    }
  }
};

/*!
  \brief Gets list of integer identifiers of all the 
         currently opened views of the specified type
  \param type viewer type
  \return list of integer identifiers 
*/
QValueList<int> SalomePyQt::findViews( const QString& type )
{
  return ProcessEvent( new TFindViews( type ) );
}

//=============================================================================

/*!
  \class TActivateView
  \brief Activates view
*/
class TActivateView: public SALOME_Event
{
public:
  typedef bool TResult;
  TResult myResult;
  int myWndId;
  
public:
  /*!
    \brief Constructor
    \param id window identifier
  */
  TActivateView( const int id )
  : myResult( false ),
    myWndId( id )
  {
  }
  /*!
    \brief Activates view
  */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
    {
      wnd->setFocus();
      myResult = true;
    }
  }
};

/*!
  \brief Activates view
  \param id window identifier
  \return TRUE if operation is completed successfully, FALSE otherwise 
*/
bool SalomePyQt::activateView( const int id )
{
  return ProcessEvent( new TActivateView( id ) );
}

//=============================================================================

/*!
  \class TCreateView
  \brief Creates new view and activate it
*/
class TCreateView: public SALOME_Event
{
public:
  typedef int TResult;
  TResult myResult;
  QString myType;
  
public:
  /*!
    \brief Constructor.
    \param type viewer type
  */
  TCreateView( const QString& theType )
  : myResult( -1 ),
    myType( theType )
  {
  }
  /*!
    \brief Creates new view and activate it
  */
  virtual void Execute() 
  {
    SalomeApp_Application* app  = getApplication();
    if ( app )
    {
      SUIT_ViewManager* viewMgr = app->createViewManager( myType );
      if ( viewMgr )
      {
        SUIT_ViewWindow* wnd = viewMgr->getActiveView();
        if ( wnd )
          myResult = wnd->getId();
      }
    }
  }
};

/*!
  \brief Creates new view and activate it
  \param type viewer type
  \return integer identifier of created vieew (or -1 if view could not be created)
*/
int SalomePyQt::createView( const QString& type )
{
  return ProcessEvent( new TCreateView( type ) );
}

//=============================================================================

/*!
  \class TCloseView
  \brief Closes view
*/
class TCloseView: public SALOME_Event
{
public:
  typedef bool TResult;
  TResult myResult;
  int myWndId;
  
public:
  /*!
    \brief Constructor.
    \param id window identifier
  */
  TCloseView( const int id )
  : myResult( false ),
    myWndId( id )
  {
  }
  /*!
    \brief Closes view
  */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
    {
      SUIT_ViewManager* viewMgr = wnd->getViewManager();
      if ( viewMgr )
      {
        wnd->close();
        myResult = true;
      }
    }
  }
};

/*!
  \brief Closes view
  \param id window identifier
  \return TRUE if operation is completed successfully, FALSE otherwise 
*/
bool SalomePyQt::closeView( const int id )
{
  return ProcessEvent( new TCloseView( id ) );
}

//=============================================================================

/*!
  \class TCloneView
  \brief Clones view (if this operation is supported for specified view type)
*/
class TCloneView: public SALOME_Event
{
public:
  typedef int TResult;
  TResult myResult;
  int myWndId;
  
public:
  /*!
    \brief Constructor.
    \param id window identifier
  */
  TCloneView( const int id )
  : myResult( -1 ),
    myWndId( id )
  {
  }
  /*!
    \brief Clones view (if this operation is supported for specified view type)
  */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
    {
      SUIT_ViewManager* viewMgr = wnd->getViewManager();
      if ( viewMgr )
      {
        if ( wnd->inherits( "OCCViewer_ViewWindow" ) )
        {
          OCCViewer_ViewWindow* occView = (OCCViewer_ViewWindow*)( wnd );
          occView->onCloneView();

          wnd = viewMgr->getActiveView();
          if ( wnd )
            myResult = wnd->getId();
        }
        else if ( wnd->inherits( "Plot2d_ViewWindow" ) ) 
        {
          Plot2d_ViewManager* viewMgr2d = dynamic_cast<Plot2d_ViewManager*>( viewMgr );
          Plot2d_ViewWindow* srcWnd2d = dynamic_cast<Plot2d_ViewWindow*>( wnd );
          if ( viewMgr2d && srcWnd2d )
          {
            Plot2d_ViewWindow* resWnd = viewMgr2d->cloneView( srcWnd2d );
            myResult = resWnd->getId();
          }
        }
      }
    }
  }
};

/*!
  \brief Clones view (if this operation is supported for specified view type)
  \param id window identifier
  \return integer identifier of the cloned view or -1 or operation could not be performed
*/
int SalomePyQt::cloneView( const int id )
{
  return ProcessEvent( new TCloneView( id ) );
}

//=============================================================================

/*!
  \class TIsViewVisible
  \brief Clones view (if this operation is supported for specified view type)
*/
class TIsViewVisible: public SALOME_Event
{
public:
  typedef bool TResult;
  TResult myResult;
  int myWndId;
  
public:
  /*!
    \brief Constructor.
    \param id window identifier
  */
  TIsViewVisible( const int id )
  : myResult( false ),
    myWndId( id )
  {
  }
  /*!
    \brief Clones view (if this operation is supported for specified view type)
  */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
    {
      QWidget* p = wnd->parentWidget();
      myResult = ( p && p->isVisibleTo( p->parentWidget() ) );
    }
  }
};

/*!
  \brief Verifies whether view is visible ( i.e. it is on the top of the views stack)
  \param id window identifier
  \return TRUE if view is visible, FALSE otherwise 
*/
bool SalomePyQt::isViewVisible( const int id )
{
  return ProcessEvent( new TIsViewVisible( id ) );
}

//=============================================================================

/*!
  \class TGroupAllViews
  \brief Groups all views to the single tab area
*/
class TGroupAllViews: public SALOME_Event
{
public:
  typedef bool TResult;
  TResult myResult;
public:
  /*!
  \brief Constructor.
  */
  TGroupAllViews()
    : myResult( false )
  {
  }
  /*!
  \brief Groups all views to the single tab area
  */
  virtual void Execute() 
  {
    SalomeApp_Application* app  = getApplication();
    if ( app )
    {
      STD_TabDesktop* tabDesk = dynamic_cast<STD_TabDesktop*>( app->desktop() );
      if ( tabDesk )
      {
        QtxWorkstack* wStack = tabDesk->workstack();
        if ( wStack )
        {
          wStack->stack();
          myResult = true;
        }
      }
    }
  }
};
  
/*!
  \brief Groups all views to the single tab area
  \return TRUE if operation is completed successfully, FALSE otherwise 
*/
bool SalomePyQt::groupAllViews()
{
  return ProcessEvent( new TGroupAllViews() );
}

//=============================================================================

/*!
  \class TSplitView
  \brief Splits tab area to which view with identifier belongs to
*/
class TSplitView: public SALOME_Event
{
public:
  typedef bool TResult;
  TResult myResult;
  int myWndId;
  Orientation myOri;
  Action myAction;
  
public:
  /*!
    \brief Constructor.
    \param id window identifier
    \param ori orientation of split operation
    \param action action to be performed
  */
  TSplitView( const int id, 
              const Orientation ori, 
              const Action action )
  : myResult( false ),
    myWndId( id ),
    myOri( ori ),
    myAction( action )
  {
  }
  /*!
  \brief Splits tab area to which view with identifier belongs to
  */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
    {
      // activate view
      // wnd->setFocus(); ???

      // split workstack
      if ( getApplication() )
      {
        STD_TabDesktop* desk = 
          dynamic_cast<STD_TabDesktop*>( getApplication()->desktop() );
        if ( desk )
        {
          QtxWorkstack* wStack = desk->workstack();
          if ( wStack )
          {
            Qt::Orientation qtOri = 
              ( myOri == Horizontal ) ? Qt::Horizontal : Qt::Vertical;

            QtxWorkstack::SplitType sType;
            if ( myAction == MoveWidget )
              sType = QtxWorkstack::SPLIT_MOVE;
            else if ( myAction == LeaveWidget )
              sType = QtxWorkstack::SPLIT_STAY;
            else 
              sType = QtxWorkstack::SPLIT_AT;

            wStack->Split( wnd, qtOri, sType );
            myResult = true;
          }
        }
      }
    }
  }
};

/*!
  \brief Splits tab area to which view with identifier belongs to
  \param id window identifier
  \param ori orientation of split operation
  \param action action to be performed
  \return TRUE if operation is completed successfully, FALSE otherwise 
*/
bool SalomePyQt::splitView( const int id, const Orientation ori, const Action action )
{
  return ProcessEvent( new TSplitView( id, ori, action ) );
}

//=============================================================================

/*!
  \class TMoveView
  \brief Moves view with the first identifier to the same area which 
         another view with the second identifier belongs to
*/
class TMoveView: public SALOME_Event
{
public:
  typedef bool TResult;
  TResult myResult;
  int myWndId;
  int myWndToId;
  bool myIsBefore;
  
public:
  /*!
    \brief Constructor.
    \param id source window identifier
    \param id_to destination window identifier  
  */
  TMoveView( const int id, const int id_to, const bool before )
  : myResult( false ),
    myWndId( id ),
    myWndToId( id_to ),
    myIsBefore( before )
  {
  }
  /*!
    \brief Closes view
    */
  virtual void Execute() 
  {
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    SUIT_ViewWindow* wnd_to = getWnd( myWndToId );
    if ( wnd && wnd_to )
    {
      QtxWorkstack* wStack = dynamic_cast<STD_TabDesktop*>( 
        getApplication()->desktop() )->workstack();
      if ( wStack )
        myResult = wStack->move( wnd, wnd_to, myIsBefore );
    }
  }
};

/*!
  \brief Moves view with the first identifier to the same area which 
         another view with the second identifier belongs to
  \param id source window identifier
  \param id_to destination window identifier  
  param before specifies whether the first viewt has to be moved before or after 
        the second view
  \return TRUE if operation is completed successfully, FALSE otherwise 
*/
bool SalomePyQt::moveView( const int id, const int id_to, const bool before )
{
  return ProcessEvent( new TMoveView( id, id_to, before ) );
}

//=============================================================================

/*!
  \class TNeighbourViews
  \brief Gets list of views identifiers that belongs to the same area as 
         specified view (excluding it)
*/
class TNeighbourViews: public SALOME_Event
{
public:
  typedef QValueList<int> TResult;
  TResult myResult;
  int myWndId;

public:
  /*!
    \brief Constructor.
    \param id window identifier
  */
  TNeighbourViews( const int id )
  : myWndId( id )
  {
  }
  /*!
    \brief Gets list of views identifiers that belongs to the same area as 
           specified view (excluding it)
  */
  virtual void Execute() 
  {
    myResult.clear();
    SUIT_ViewWindow* wnd = getWnd( myWndId );
    if ( wnd )
    {
      QtxWorkstack* wStack = dynamic_cast<STD_TabDesktop*>( 
        getApplication()->desktop() )->workstack();
      if ( wStack )
      {
        QWidgetList wgList = wStack->windowList( wnd );
        QWidget* it;
        for ( it = wgList.first(); it; it = wgList.next() )
        {
          SUIT_ViewWindow* tmpWnd = dynamic_cast<SUIT_ViewWindow*>( it );
          if ( tmpWnd && tmpWnd != wnd )
            myResult.append( tmpWnd->getId() );
        }
      }
    }
  }
};

/*!
  \brief Gets list of views identifiers that belongs to the same area as 
         specified view (excluding it)
  \param id window identifier
  \return list of views identifiers
*/
QValueList<int> SalomePyQt::neighbourViews( const int id )
{
  return ProcessEvent( new TNeighbourViews( id ) );
}

