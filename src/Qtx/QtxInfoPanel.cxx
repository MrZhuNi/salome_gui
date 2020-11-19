// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "QtxInfoPanel.h"

#include <QAction>
#include <QFont>
#include <QGroupBox>
#include <QLabel>
#include <QMap>
#include <QPalette>
#include <QSizePolicy>
#include <QToolButton>
#include <QVBoxLayout>

/*!
  \class QtxInfoPanel::Container
  \short Container to store widgets within info panel
  \internal
*/
class QtxInfoPanel::Container: public QWidget
{
public:
  Container( QWidget* = 0 );
  Container( const QString&, QWidget* = 0 );

  void addAction( QAction*, const int );
  void addLabel( const QString&, Qt::Alignment, const int );
  void addGroup( const QString&, const int );

  QWidget* find( const int ) const;

  void remove( const int );
  void clear();

  void put( QWidget* );

private:
  QMap<int, QWidget*> ids;
  QGroupBox* group;
};

QtxInfoPanel::Container::Container( QWidget* parent )
  : QWidget( parent ), group( 0 )
{
  QVBoxLayout* l = new QVBoxLayout( this );
  l->setContentsMargins( 0, 0, 0, 0 );
}

QtxInfoPanel::Container::Container( const QString& title, QWidget* parent )
  : Container( parent )
{
  QVBoxLayout* l = dynamic_cast<QVBoxLayout*>( layout() );
  group = new QGroupBox( title );
  group->setLayout( new QVBoxLayout() );
  l->addWidget( group );
}

void QtxInfoPanel::Container::put( QWidget* widget )
{
  QVBoxLayout* l = group ? dynamic_cast<QVBoxLayout*>( group->layout() ) : dynamic_cast<QVBoxLayout*>( layout() );
  l->addWidget( widget );
}

void QtxInfoPanel::Container::addLabel( const QString& text, Qt::Alignment alignment, const int id )
{
  QLabel* label = new QLabel( text );
  QFont f = label->font();
  f.setItalic( true );
  label->setFont( f );
  label->setAlignment( alignment );
  label->setWordWrap( true );
  put( label );
  ids[ id ] = label;
}

void QtxInfoPanel::Container::addAction( QAction* action, const int id )
{
  QToolButton* button = new QToolButton( this );
  button->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
  button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  button->setAutoRaise( true );
  button->setDefaultAction( action );
  put( button );
  ids[ id ] = button;
}

void QtxInfoPanel::Container::addGroup( const QString& text, const int id )
{
  Container* group = new Container( text, this );
  put( group );
  ids[ id ] = group;
}

QWidget* QtxInfoPanel::Container::find( const int id ) const
{
  if ( ids.contains( id ) )
    return ids[id];

  QMap<int, QWidget*>::ConstIterator it;
  QWidget* widget = 0;
  for( it = ids.begin(); it != ids.end() && !widget; ++it )
  {
    Container* group = dynamic_cast<Container*>( it.value() );
    if ( group )
      widget = group->find( id );
  }

  return widget;
}

void QtxInfoPanel::Container::remove( const int id )
{
  if ( ids.contains( id ) )
  {
    QVBoxLayout* l = group ? dynamic_cast<QVBoxLayout*>( group->layout() ) : dynamic_cast<QVBoxLayout*>( layout() );
    l->removeWidget( ids[id] );
    ids[id]->deleteLater();
    l->invalidate();
    ids.remove( id );
  }
}

void QtxInfoPanel::Container::clear()
{
  QVBoxLayout* l = group ? dynamic_cast<QVBoxLayout*>( group->layout() ) : dynamic_cast<QVBoxLayout*>( layout() );

  QList<QWidget*> widgets = ids.values();
  foreach( QWidget* widget, widgets )
  {
    l->removeWidget( widget );
    widget->deleteLater();
  }

  l->invalidate();
  ids.clear();
}


/*!
  \class QtxInfoPanel::Title
  \short Info panel's title widget
  \internal
*/
class QtxInfoPanel::Title: public QLabel
{
public:
  Title( QWidget* parent = 0 );
};

QtxInfoPanel::Title::Title( QWidget* parent )
  : QLabel( parent )
{
  setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
  QString bg = palette().color( QPalette::Highlight ).name();
  QString fg = palette().color( QPalette::HighlightedText ).name();
  setStyleSheet( QString( "QLabel { background:%1; color:%2; }" ).arg( bg ).arg( fg ) );
  setTextFormat( Qt::PlainText );
  QFont f = font();
  f.setBold( true );
  setFont( f );
  setContentsMargins( 2, 5, 2, 5 );
}


/*!
  \class QtxInfoPanel
  \short Info panel.
*/
QtxInfoPanel::QtxInfoPanel( QWidget* parent )
  : QWidget( parent )
{
  title = new Title( this );
  container = new Container( this );
  QVBoxLayout* layout = new QVBoxLayout( this );
  layout->setMargin( 0 );
  layout->addWidget( title );
  layout->addWidget( container );
  layout->addStretch();
  setTitle( "" );
}

QtxInfoPanel::~QtxInfoPanel()
{
}

int QtxInfoPanel::addLabel( const QString& text, const int groupId )
{
  return addLabel( text, Qt::AlignLeft, groupId );
}

int QtxInfoPanel::addLabel( const QString& text, Qt::Alignment alignment, const int groupId )
{
  int id = 0;
  Container* c = dynamic_cast<Container*>( find( groupId ) );
  if ( c )
  {
    id = generateId();
    c->addLabel( text, alignment, id );
  }
  return id;
}

int QtxInfoPanel::addAction( QAction* action, const int groupId )
{
  int id = 0;
  Container* c = dynamic_cast<Container*>( find( groupId ) );
  if ( c )
  {
    id = generateId();
    c->addAction( action, id );
  }
  return id;
}

int QtxInfoPanel::addGroup( const QString& text, const int groupId )
{
  int id = 0;
  Container* c = dynamic_cast<Container*>( find( groupId ) );
  if ( c )
  {
    id = generateId();
    c->addGroup( text, id );
  }
  return id;
}

void QtxInfoPanel::setTitle( const QString& text )
{
  title->setText( text );
  title->setVisible( !title->text().isEmpty() );
}

void QtxInfoPanel::remove( const int id )
{
  QWidget* widget = find( id );
  if ( widget )
  {
    Container* group = dynamic_cast<Container*>( widget->parentWidget() );
    if ( !group )
      group = dynamic_cast<Container*>( widget->parentWidget()->parentWidget() );
    if ( group )
      group->remove( id );
  }
}

void QtxInfoPanel::clear( const int groupId )
{
  Container* c = dynamic_cast<Container*>( find( groupId ) );
  if ( c )
    c->clear();
}

QWidget* QtxInfoPanel::find( const int id ) const
{
  if ( id == -1 )
    return container;
  return container->find( id );
}

void QtxInfoPanel::setVisible( const int id, bool visible )
{
  QWidget* widget = find( id );
  if ( widget )
    widget->setVisible( visible );
}

void QtxInfoPanel::setEnabled( const int id, bool enabled )
{
  QWidget* widget = find( id );
  if ( widget )
    widget->setEnabled( enabled );
}

int QtxInfoPanel::generateId() const
{
  static int id = -100;
  return --id;
}
