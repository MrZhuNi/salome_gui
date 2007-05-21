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
// File:      QtxWorkspaceAction.cxx
// Author:    Sergey TELKOV

#include "QtxWorkspaceAction.h"

#include "QtxWorkspace.h"

#include <QMenu.h>
#include <QWidgetList>
#include <QApplication>

/*!
  Constructor
*/
QtxWorkspaceAction::QtxWorkspaceAction( QtxWorkspace* ws, QObject* parent )
: QtxActionSet( parent )
  myWorkspace( ws )
{
  insertAction( new QtxAction( tr( "Arranges the windows as overlapping tiles" ),
                               tr( "Cascade" ), 0, this ), Cascade );
  insertAction( new QtxAction( tr( "Arranges the windows as nonoverlapping tiles" ),
                               tr( "Tile" ), 0, this ), Tile );
  insertAction( new QtxAction( tr( "Arranges the windows as nonoverlapping horizontal tiles" ),
                               tr( "Tile horizontally" ), 0, this ), HTile );
  insertAction( new QtxAction( tr( "Arranges the windows as nonoverlapping vertical tiles" ),
                               tr( "Tile vertically" ), 0, this ), VTile );

  connect( this, SIGNAL( triggered( int ) ), this, SLOT( onTriggered( int ) ) );

  setItems( Standard );
}

/*!
  Destructor
*/
QtxWorkspaceAction::~QtxWorkspaceAction()
{
}

/*!
  \return corresponding workspace
*/
QtxWorkspace* QtxWorkspaceAction::workspace() const
{
  return myWorkspace;
}

/*!
  \brief Set actions to be visible in the menu.
  
  Actions, which IDs are set in \a flags parameter, will be shown in the 
  menu bar. Other actions will not be shown.

  \param flags ORed together actions flags
*/
void QtxWorkspaceAction::setMenuActions( const int flags )
{
  action( Cascade )->setVisible( flags & Cascade );
  action( Tile )->setVisible( flags & Tile );
  action( VTile )->setVisible( flags & VTile );
  action( HTile )->setVisible( flags & HTile );
  action( Windows )->setVisible( flags & Windows );
}

/*!
  \brief Get menu actions which are currently visible in the menu bar.
  \return ORed together actions flags
  \sa setMenuActions()
*/
int QtxWorkspaceAction::menuActions() const
{
  int ret = 0;
  ret = ret | ( action( Cascade )->isVisible() ? Cascade : 0 );
  ret = ret | ( action( Tile )->isVisible() ? Tile : 0 );
  ret = ret | ( action( VTile )->isVisible() ? VTile : 0 );
  ret = ret | ( action( HTile )->isVisible() ? HTile : 0 );
  ret = ret | ( action( Windows )->isVisible() ? Windows : 0 );
  return ret;
}

/*!
  \return accelerator of item
  \param id - item id
*/
int QtxWorkspaceAction::accel( const int id ) const
{
  int a = 0;
  if ( action( id ) )
    a = action( id )->shortcut();
  return a;
}

/*!
  \return icons of item
  \param id - item id
*/
QIcon QtxWorkspaceAction::icon( const int id ) const
{
  QIcon ico;
  if ( action( id ) )
    ico = action( id )->icon();
  return ico;
}

/*!
  \return menu text of item
  \param id - item id
*/
QString QtxWorkspaceAction::text( const int id ) const
{
  QString txt;
  if ( action( id ) )
    txt = action( id )->text();
  return txt;
}

/*!
  \return status tip of item
  \param id - item id
*/
QString QtxWorkspaceAction::statusTip( const int id ) const
{
  QString txt;
  if ( action( id ) )
    txt = action( id )->statusTip();
  return txt;
}

/*!
  Changes accelerator of item
  \param id - item id
  \param a - new accelerator
*/
void QtxWorkspaceAction::setAccel( const int id, const int a )
{
  if ( action( id ) )
    action( id )->setShortcut( a );
}

/*!
  Changes icons of item
  \param id - item id
  \param ico - new icons
*/
void QtxWorkspaceAction::setIcon( const int id, const QIcon& ico )
{
  if ( action( id ) )
    action( id )->setIcon( ico );
}

/*!
  Changes menu text of item
  \param id - item id
  \param txt - new menu text
*/
void QtxWorkspaceAction::setText( const int id, const QString& txt )
{
  if ( action( id ) )
    action( id )->setText( txt );
}

/*!
  Changes status tip of item
  \param id - item id
  \param txt - new status tip
*/
void QtxWorkspaceAction::setStatusTip( const int id, const QString& txt )
{
  if ( action( id ) )
    action( id )->setStatusTip( txt );
}

/*!
  Performs action
  \param type - action type
*/
void QtxWorkspaceAction::perform( const int type )
{
  switch ( type )
  {
  case Cascade:
    cascade();
    break;
  case Tile:
    tile();
    break;
  case VTile:
    tileVertical();
    break;
  case HTile:
    tileHorizontal();
    break;
  }
}

/*!
  Performs tile action
*/
void QtxWorkspaceAction::tile()
{
  QtxWorkspace* ws = workspace();
  if ( ws )
    ws->tile();
}

/*!
  Performs cascade action
*/
void QtxWorkspaceAction::cascade()
{
  QtxWorkspace* ws = workspace();
  if ( !ws )
    return;

  ws->cascade();

	int w = ws->width();
	int h = ws->height();

	QWidgetList winList = ws->windowList();
  for ( QWidgetList::iterator it = winList.begin(); it != winList.end(); ++it )
		(*it)->resize( int( w * 0.8 ), int( h * 0.8 ) );
}

/*!
  Performs tile vertical action
*/
void QtxWorkspaceAction::tileVertical()
{
  QtxWorkspace* ws = workspace();
  if ( ws )
    ws->tileVertical();
}

/*!
  Performs tile horizontal action
*/
void QtxWorkspaceAction::tileHorizontal()
{
  QtxWorkspace* ws = workspace();
  if ( ws )
    ws->tileHorizontal();
}

void QtxWorkspaceAction::addedTo( QWidget* w )
{
  QtxActionSet::addedTo( w );

  QMenu* pm = ::qobject_cast<QMenu*>( w );
  if ( pm )
    connect( pm, SIGNAL( aboutToShow() ), this, SLOT( onAboutToShow() ) );
}

void QtxWorkspaceAction::removedFrom( QWidget* w )
{
  QtxActionSet::removedFrom( w );

  QMenu* pm = ::qobject_cast<QMenu*>( w );
  if ( pm )
    disconnect( pm, SIGNAL( aboutToShow() ), this, SLOT( onAboutToShow() ) );
}

/*!
  Refills actions and updates thier state.
*/
void QtxWorkspaceAction::updateContent()
{
  bool count = workspace() ? workspace()->windowList().count() : 0;
  action( Cascade )->setEnabled( count );
  action( Tile )->setEnabled( count );
  action( HTile )->setEnabled( count );
  action( VTile )->setEnabled( count );

  updateWindows();
}

void QtxWorkspaceAction::updateWindows()
{
  QtxWorkspace* ws = workspace();
  if ( !ws )
    return;

  QList<QAction*> lst = actions();
  for ( QList<QAction*>::iterator it = lst.begin(); it != lst.end(); ++it )
  {
    int id = actionId( *it );
    if ( id >= Windows )
      removeAction( *it );
  }

  bool base = action( Cascade )->isVisible() || action( Tile )->isVisible() ||
              action( HTile )->isVisible() || action( VTile )->isVisible();

  QList<QAction*> items;
  QMap<QAction*, int> map;
  if ( hasItems( Windows ) )
  {
    int index = 1;
    QWidgetList wList = ws->windowList();
    for ( QWidgetList::iterator it = wList.begin(); it != wList.end(); ++it, index++ )
    {
      QWidget* wid = *it;
      QAction* a = new QtxAction( wid->windowTitle(), wid->windowTitle(), 0, this, true );
      a->setChecked( wid == ws->activeWindow() );
      items.append( a );
      map.insert( a, Windows + index );
    }

    if ( base && !items.isEmpty() )
    {
      QAction* sep = new QtxAction( this );
      sep->setSeparator( true );
      items.prepend( sep );
      map.insert( sep, Windows );
    }
  }

  if ( !items.isEmpty() )
    insertActions( items );

  for ( QMap<QAction*, int>::const_iterator itr = map.begin(); itr != map.end(); ++itr )
    setActionId( itr.key(), itr.value() );
}

void QtxWorkspaceAction::onAboutToShow()
{
  QMenu* pm = ::qobject_cast<QMenu*>( sender() );
  if ( pm )
    updateContent();
}

/*!
  SLOT: called when popup item corresponding to window is activated, activates window
*/
void QtxWorkspaceAction::onItemActivated( int idx )
{
  QtxWorkspace* ws = workspace();
  if ( !ws )
    return;

  QWidgetList wList = ws->windowList();
  if ( idx < 0 || idx >= (int)wList.count() )
    return;

  wList.at( idx )->setFocus();
}

void QtxWorkspaceAction::onTriggered( int id )
{
  if ( id < Windows )
    perform( id );
  else
  {
    int idx = id - Windows - 1;

    QtxWorkspace* ws = workspace();
    if ( ws )
    {
      QWidgetList wList = ws->windowList();
      if ( idx >= 0 && idx < (int)wList.count() )
        wList.at( idx )->setFocus();
    }
  }
}
