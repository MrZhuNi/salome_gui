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
// File:      QtxMainWindow.cxx
// Author:    Sergey TELKOV

#include "QtxMainWindow.h"

#include "QtxToolBar.h"
#include "QtxResourceMgr.h"

#include <QtGui/qevent.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qapplication.h>
#include <QtGui/qdesktopwidget.h>

/*!
    Class: QtxMainWindow::Filter [Internal]
    Descr: Internal object with event filter for QtxMainWindow.
*/

class QtxMainWindow::Filter : public QObject
{
public:
  Filter( QWidget*, QtxMainWindow*, QObject* = 0 );
  virtual ~Filter();

  virtual bool eventFilter( QObject*, QEvent* );

private:
  QMainWindow* myMain;
  QWidget*     myWidget;
};

/*!
  Constructor
*/
QtxMainWindow::Filter::Filter( QWidget* wid, QtxMainWindow* mw, QObject* parent )
: QObject( parent ),
myMain( mw ),
myWidget( wid )
{
  QApplication::instance()->installEventFilter( this );
};

/*!
  Destructor
*/
QtxMainWindow::Filter::~Filter()
{
}

/*!
  Custom event filter
*/
bool QtxMainWindow::Filter::eventFilter( QObject* o, QEvent* e )
{
  if ( myMain == o && e->type() == QEvent::ChildRemoved &&
       myWidget == ((QChildEvent*)e)->child() )
    return true;

  return QObject::eventFilter( o, e );
}

/*!
    Class: QtxMainWindow [Public]
    Descr: Main window with support of dockable menubar/status bar
           and geometry store/retrieve.
*/
QtxMainWindow::QtxMainWindow( QWidget* parent, Qt::WindowFlags f )
: QMainWindow( parent ),
myMode( -1 ),
myMenuBar( 0 ),
myStatusBar( 0 )
{
}

/*!
  Destructor
*/
QtxMainWindow::~QtxMainWindow()
{
  setDockableMenuBar( false );
  setDockableStatusBar( false );
}

/*!
  \return true if menu bar exists
*/
bool QtxMainWindow::isDockableMenuBar() const
{
  return myMenuBar;
}

/*!
  Creates or deletes menu bar
  \param on - if it is true, then to create, otherwise - to delete
*/
void QtxMainWindow::setDockableMenuBar( const bool on )
{
  if ( isDockableMenuBar() == on )
    return;

  QMenuBar* mb = menuBar();
  if ( !mb )
    return;

  if ( on && !myMenuBar )
  {
    myMenuBar = new QtxToolBar( true, this );
    new Filter( mb, this, myMenuBar );
    myMenuBar->setObjectName( "menu_bar_container" );
    myMenuBar->setWindowTitle( tr( "Menu bar" ) );
    myMenuBar->addWidget( mb );
    myMenuBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );

    addToolBarBreak( Qt::TopToolBarArea );
    addToolBar( Qt::TopToolBarArea, myMenuBar );
    addToolBarBreak( Qt::TopToolBarArea );

    connect( myMenuBar, SIGNAL( destroyed( QObject* ) ), this, SLOT( onDestroyed( QObject* ) ) );
  }
  else if ( !on && myMenuBar )
  {
    setMenuBar( mb );
    disconnect( myMenuBar, SIGNAL( destroyed( QObject* ) ), this, SLOT( onDestroyed( QObject* ) ) );
    delete myMenuBar;
    myMenuBar = 0;
  }
}

/*!
  \return true if status bar exists
*/
bool QtxMainWindow::isDockableStatusBar() const
{
  return myStatusBar;
}

/*!
  Creates or deletes status bar
  \param on - if it is true, then to create, otherwise - to delete
*/
void QtxMainWindow::setDockableStatusBar( const bool on )
{
  if ( isDockableStatusBar() == on )
    return;

  QStatusBar* sb = statusBar();
  if ( !sb )
    return;

  if ( on && !myStatusBar )
  {
    sb->setMinimumWidth( 250 );
    sb->setSizeGripEnabled( false );
    myStatusBar = new QtxToolBar( true, this );
    new Filter( sb, this, myStatusBar );
    myStatusBar->setObjectName( "status_bar_container" );
    myStatusBar->setWindowTitle( tr( "Status bar" ) );
    myStatusBar->addWidget( sb );
    myStatusBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );

    addToolBar( Qt::BottomToolBarArea, myStatusBar );

    connect( myStatusBar, SIGNAL( destroyed( QObject* ) ), this, SLOT( onDestroyed( QObject* ) ) );
  }
  else if ( !on && myStatusBar )
  {
    setStatusBar( sb );
    disconnect( myStatusBar, SIGNAL( destroyed( QObject* ) ), this, SLOT( onDestroyed( QObject* ) ) );
    delete myStatusBar;
    myStatusBar = 0;

    sb->setSizeGripEnabled( true );
  }
}

/*!
  Retrieve the geometry information from the specified resource manager section.
  \param resMgr - instance of ersource manager
  \param section - section name
*/
void QtxMainWindow::loadGeometry( QtxResourceMgr* resMgr, const QString& section )
{
  QString sec = section.trimmed();
  if ( !resMgr || sec.isEmpty() )
    return;

  int winState = -1;
  if ( !resMgr->value( sec, "state", winState ) )
  {
    QString stateStr;
    if ( resMgr->value( sec, "state", stateStr ) )
      winState = windowState( stateStr );
  }

  int win_w = resMgr->integerValue( sec, "width", width() );
  int win_h = resMgr->integerValue( sec, "height", height() );

  int winPosX = windowPosition( resMgr->stringValue( sec, QString( "pos_x" ), QString::null ) );
  int winPosY = windowPosition( resMgr->stringValue( sec, QString( "pos_y" ), QString::null ) );

  QWidget* desk = QApplication::desktop();

  int win_x = 0;
  if ( winPosX == WP_Absolute )
    win_x = resMgr->integerValue( sec, "pos_x", x() );
  else if ( desk )
    win_x = relativeCoordinate( winPosX, desk->width(), win_w );

  int win_y = 0;
  if ( winPosX == WP_Absolute )
    win_y = resMgr->integerValue( sec, "pos_y", y() );
  else if ( desk )
    win_y = relativeCoordinate( winPosY, desk->height(), win_h );

  bool vis = isVisibleTo( parentWidget() );

  resize( win_w, win_h );
  move( win_x, win_y );

  myMode = -1;
/*
  if ( vis )
    QApplication::postEvent( this, new QEvent( QEvent::User, (void*)winState ) );
  else
    myMode = winState;
*/
}

/*!
  Shows main window
*/
void QtxMainWindow::show()
{
/*
  if ( myMode != -1 )
    QApplication::postEvent( this, new QCustomEvent( QEvent::User, (void*)myMode ) );
*/
  myMode = -1;

  QMainWindow::show();
}

/*!
  Handler of custom events
*/
void QtxMainWindow::customEvent( QEvent* e )
{
  QMainWindow::customEvent( e );

  int mode = WS_Normal;
//  int mode = (int)e->data();
  switch ( mode )
  {
  case WS_Normal:
    showNormal();
    break;
  case WS_Minimized:
    showMinimized();
    break;
  case WS_Maximized:
    showMaximized();
    break;
  }
}

/*!
  \return relative co-ordinate by two points
  \param type - type of result: WP_Center (center), WP_Left (left), WP_Right (right)
  \param wh - left point
  \param WH - right point
*/
int QtxMainWindow::relativeCoordinate( const int type, const int WH, const int wh ) const
{
  int res = 0;
  switch ( type )
  {
  case WP_Center:
    res = ( WH - wh ) / 2;
    break;
  case WP_Left:
    res = 0;
    break;
  case WP_Right:
    res = WH - wh;
    break;
  }
  return res;
}

/*!
  Store the geometry information into the specified resource manager section.
  \param resMgr - instance of ersource manager
  \param section - section name
*/
void QtxMainWindow::saveGeometry( QtxResourceMgr* resMgr, const QString& section ) const
{
  QString sec = section.trimmed();
  if ( !resMgr || sec.isEmpty() )
    return;

  resMgr->setValue( sec, "pos_x", pos().x() );
  resMgr->setValue( sec, "pos_y", pos().y() );
  resMgr->setValue( sec, "width", width() );
  resMgr->setValue( sec, "height", height() );

  int winState = WS_Normal;
  if ( isMinimized() )
    winState = WS_Minimized;
  else if ( isMaximized() )
    winState = WS_Maximized;

  resMgr->setValue( sec, "state", winState );
}

/*!
  Custom event filter
*/
bool QtxMainWindow::eventFilter( QObject* o, QEvent* e )
{
  return QMainWindow::eventFilter( o, e );
}

/*!
  SLOT: called on object destroyed, clears internal fields in case of deletion of menu bar or status bar
*/
void QtxMainWindow::onDestroyed( QObject* obj )
{
  QObject* o = 0;
  if ( obj == myMenuBar )
  {
    myMenuBar = 0;
    o = menuBar();
  }
  else if ( obj == myStatusBar )
  {
    myStatusBar = 0;
    o = statusBar();
  }

  if ( o )
  {
    QChildEvent ce( QEvent::ChildRemoved, o );
    QApplication::sendEvent( this, &ce );
  }
}

/*!
  \return flag of window state by it's name
  \param str - name of flag
*/
int QtxMainWindow::windowState( const QString& str ) const
{
  static QMap<QString, int> winStateMap;
  if ( winStateMap.isEmpty() )
  {
    winStateMap["normal"]    = WS_Normal;
    winStateMap["min"]       = WS_Minimized;
    winStateMap["mini"]      = WS_Minimized;
    winStateMap["minimized"] = WS_Minimized;
    winStateMap["max"]       = WS_Maximized;
    winStateMap["maxi"]      = WS_Maximized;
    winStateMap["maximized"] = WS_Maximized;
    winStateMap["hidden"]    = WS_Hidden;
    winStateMap["hided"]     = WS_Hidden;
    winStateMap["hide"]      = WS_Hidden;
    winStateMap["invisible"] = WS_Hidden;
  }

  int res = -1;
  QString stateStr = str.trimmed().toLower();
  if ( winStateMap.contains( stateStr ) )
    res = winStateMap[stateStr];
  return res;
}

/*!
  \return flag of position by it's name
  \param str - name of position
*/
int QtxMainWindow::windowPosition( const QString& str ) const
{
  static QMap<QString, int> winPosMap;
  if ( winPosMap.isEmpty() )
  {
    winPosMap["center"] = WP_Center;
    winPosMap["left"]   = WP_Left;
    winPosMap["right"]  = WP_Right;
    winPosMap["top"]    = WP_Top;
    winPosMap["bottom"] = WP_Bottom;
  }

  int res = WP_Absolute;
  QString posStr = str.trimmed().toLower();
  if ( winPosMap.contains( posStr ) )
    res = winPosMap[posStr];
  return res;
}
