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
// File:      QtxActionSet.cxx
// Author:    Sergey TELKOV

#include "QtxActionSet.h"

#include <QMenu>
#include <QActionGroup>

/*!
  Constructor
*/
QtxActionSet::QtxActionSet( QObject* parent )
: QtxAction( parent )
{
  connect( this, SIGNAL( changed() ), this, SLOT( onChanged() ) );

  setVisible( false );

  update();
}

/*!
  Destructor
*/
QtxActionSet::~QtxActionSet()
{
}

QList<QAction*> QtxActionSet::actions() const
{
  return mySet;
}

void QtxActionSet::setActions( const QList<QAction*>& lst )
{
  for ( ActionList::iterator it = mySet.begin(); it != mySet.end(); ++it )
  {
    if ( !lst.contains( *it ) )
      delete *it;
  }

  mySet.clear();

  insertActions( lst );
}

void QtxActionSet::insertActions( const QList<QAction*>& lst, const int index )
{
  int idx = qMin( index < 0 ? mySet.count() : index, mySet.count() );

  for ( QList<QAction*>::const_iterator it = lst.begin(); it != lst.end(); ++it )
  {
    QAction* a = *it;
    int ident = generateId();

    a->setParent( this );
    mySet.insert( idx++, a );
    a->setData( ident );

    connect( a, SIGNAL( triggered( bool ) ), this, SLOT( onActionTriggered( bool ) ) );
  }

  update();
}

int QtxActionSet::insertAction( QAction* a, const int id, const int index )
{
  if ( !a )
    return -1;

  int ident = id < 0 ? generateId() : id;
  int idx = qMin( index < 0 ? mySet.count() : index, mySet.count() );

  a->setParent( this );
  mySet.insert( idx, a );
  a->setData( ident );

  connect( a, SIGNAL( triggered( bool ) ), this, SLOT( onActionTriggered( bool ) ) );

  update();

  return ident;
}

int QtxActionSet::insertAction( const QString& txt, const int id, const int index )
{
  return insertAction( new QtxAction( txt, txt, 0, this ), id, index );
}

int QtxActionSet::insertAction( const QString& txt, const QIcon& ico, const int id, const int index )
{
  return insertAction( new QtxAction( txt, ico, txt, 0, this ), id, index );
}

void QtxActionSet::removeAction( QAction* a )
{
  if ( !mySet.contains( a ) )
    return;

  mySet.removeAll( a );
  delete a;
}

void QtxActionSet::removeAction( const int id )
{
  removeAction( action( id ) );
}

void QtxActionSet::clear()
{
  qDeleteAll( mySet );
  mySet.clear();

  update();
}

void QtxActionSet::onChanged()
{
  if ( !isVisible() )
    return;

  bool block = signalsBlocked();
  blockSignals( true );
  setVisible( false );
  blockSignals( block );
}

void QtxActionSet::onActionTriggered( bool )
{
  QAction* a = ::qobject_cast<QAction*>( sender() );
  if ( !a )
    return;

  int id = actionId( a );
  if ( id != -1 )
    emit triggered( id );
  emit triggered( a );
}

void QtxActionSet::addedTo( QWidget* w )
{
  QtxAction::addedTo( w );

  update( w );
}

void QtxActionSet::removedFrom( QWidget* w )
{
  QtxAction::removedFrom( w );

  update( w );
}

QAction* QtxActionSet::action( int id ) const
{
  QAction* a = 0;
  for ( ActionList::const_iterator it = mySet.begin(); it != mySet.end() && !a; ++it )
  {
    if ( actionId( *it ) == id )
      a = *it;
  }
  return a;
}

int QtxActionSet::actionId( QAction* a ) const
{
  int id = -1;
  if ( a && a->data().canConvert( QVariant::Int ) )
    id = a->data().toInt();
  return id;
}

void QtxActionSet::setActionId( QAction* a, const int id )
{
  if ( !a || id == -1 )
    return;

  a->setData( id );
}

int QtxActionSet::generateId() const
{
  QMap<int, int> map;
  for ( ActionList::const_iterator it = mySet.begin(); it != mySet.end(); ++it )
    map.insert( (*it)->data().toInt(), 0 );

  int id = -2;
  while ( map.contains( id ) )
    id--;

  return id;
}

void QtxActionSet::update()
{
  QList<QWidget*> lst = associatedWidgets();
  for ( QList<QWidget*>::iterator it = lst.begin(); it != lst.end(); ++it )
    update( *it );
}

void QtxActionSet::update( QWidget* w )
{
  if ( !w )
    return;

  for ( ActionList::iterator it = mySet.begin(); it != mySet.end(); ++it )
    w->removeAction( *it );

  if ( !associatedWidgets().contains( w ) )
    return;

  for ( int i = 0; i < mySet.count(); i++ )
  {
    QAction* a = mySet.at( i );
    w->insertAction( this, a );
  }
}
