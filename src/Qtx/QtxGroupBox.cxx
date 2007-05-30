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
// File:      QtxGroupBox.cxx
// Author:    Sergey TELKOV

#include "QtxGroupBox.h"

#include <QLayout>
#include <QObjectList>
#include <QToolButton>
#include <QApplication>

/*!
  Constructor
*/
QtxGroupBox::QtxGroupBox( QWidget* parent )
: QGroupBox( parent ),
myContainer( 0 )
{
  initialize();
}

/*!
  Constructor
*/
QtxGroupBox::QtxGroupBox( const QString& title, QWidget* parent )
: QGroupBox( title, parent ),
myContainer( 0 )
{
  initialize();
}

/*!
  Destructor
*/
QtxGroupBox::~QtxGroupBox()
{
}

/*!
  Creates horizontal box as container
*/
void QtxGroupBox::initialize()
{
  myContainer = new QWidget( this );
  QHBoxLayout* base = new QHBoxLayout( myContainer );
  base->setMargin( 0 );
  base->setSpacing( 0 );

  updateTitle();
}

/*!
  Inserts title widget
  \param wid - new title widget
*/
void QtxGroupBox::insertTitleWidget( QWidget* wid )
{
  if ( !myContainer )
    return;

  myContainer->layout()->addWidget( wid );
  wid->installEventFilter( this );

  updateTitle();
}

/*!
  Removes title widget
  \param wid - title widget
*/
void QtxGroupBox::removeTitleWidget( QWidget* wid )
{
  if ( !myContainer || wid->parentWidget() != myContainer )
    return;

  myContainer->layout()->removeWidget( wid );
  wid->setParent( 0 );
  wid->removeEventFilter( this );
  wid->hide();

  updateTitle();
}

/*!
  Shows group box
*/
void QtxGroupBox::setVisible( bool on )
{
  if ( on )
    updateTitle();

  QGroupBox::setVisible( on );
}

/*!
  \return the recommended size for the widget
*/
QSize QtxGroupBox::sizeHint() const
{
  return expandTo( QGroupBox::sizeHint() );
}

/*!
  \return the recommended minimum size for the widget
*/
QSize QtxGroupBox::minimumSizeHint() const
{
  return expandTo( QGroupBox::minimumSizeHint() );
}

/*!
  Custom event filter
*/
bool QtxGroupBox::eventFilter( QObject* obj, QEvent* e )
{
  QEvent::Type type = e->type();
  if ( myContainer && obj->parent() == myContainer &&
       ( type == QEvent::Show || type == QEvent::ShowToParent ||
         type == QEvent::Hide || type == QEvent::HideToParent ) )
    QApplication::postEvent( this, new QEvent( QEvent::User ) );

  return QGroupBox::eventFilter( obj, e );
}

QWidget* QtxGroupBox::widget() const
{
  if ( !layout() )
    return 0;

  QWidget* w = 0;
  for ( int i = 0; i < (int)layout()->count() && !w; i++ )
    w = layout()->itemAt( i )->widget();
  return w;
}

void QtxGroupBox::setWidget( QWidget* wid )
{
  QWidget* w = widget();
  if ( w == wid )
    return;

  if ( layout() )
    layout()->removeWidget( w );

  if ( !wid )
    delete layout();
  else if ( !layout() )
  {
    QLayout* bl = new QVBoxLayout( this );
    bl->setMargin( 0 );
    bl->setSpacing( 0 );
  }

  if ( layout() )
    layout()->addWidget( wid );

  if ( wid )
    wid->updateGeometry();
}

/*!
  Custom resize event filter
*/
void QtxGroupBox::resizeEvent( QResizeEvent* e )
{
  QGroupBox::resizeEvent( e );

  updateTitle();
}

/*!
  Custom child event filter
*/
void QtxGroupBox::childEvent( QChildEvent* e )
{
/*
  if ( e->type() == QEvent::ChildAdded && e->child() == myContainer )
    return;
*/
  QGroupBox::childEvent( e );
}

/*!
  Event filter of custom items
*/
void QtxGroupBox::customEvent( QEvent* )
{
  updateTitle();
}

/*!
  \return size of title
*/
QSize QtxGroupBox::titleSize() const
{
  return QSize( fontMetrics().width( title() ), fontMetrics().height() );
}

/*!
  Updates title
*/
void QtxGroupBox::updateTitle()
{
  if ( !myContainer )
    return;

  int align = alignment();

  if ( title().isEmpty() )
    align = Qt::AlignRight;

  QSize ts = titleSize();

  int m = 5;

  int w = width() - ts.width();
  if ( align == Qt::AlignCenter )
    w = w / 2;

  w -= m;

  myContainer->resize( myContainer->minimumSizeHint() );

  bool vis = false;
  const QObjectList list = myContainer->children();
  for ( QObjectList::const_iterator it = list.begin(); it != list.end() && !vis; ++it )
    vis = (*it)->isWidgetType() && ((QWidget*)(*it))->isVisibleTo( myContainer );

  if ( !vis )
    myContainer->hide();
  else
  {
    int x = 0;
    if ( align == Qt::AlignRight )
      x = rect().left() + m;
    else
      x = rect().right() - myContainer->width() - m;

    int y = rect().top() - ( myContainer->height() - ts.height() ) / 2;

    QPoint pos( x, qMax( 0, y ) );
    myContainer->move( pos );
    myContainer->show();
  }

  if ( layout() )
  {
    if ( myContainer && myContainer->isVisibleTo( this ) )
      setInsideMargin( qMax( 0, myContainer->height() - ts.height() ) );
    else
      setInsideMargin( 0 );
  }

  updateGeometry();
}

QSize QtxGroupBox::expandTo( const QSize& sz ) const
{
  int sh = 0;
  int sw = titleSize().width();
  if ( myContainer && myContainer->isVisibleTo( (QWidget*)this ) )
  {
    if ( alignment() == Qt::AlignCenter )
      sw += 2 * ( myContainer->width() + 5 );
    else
      sw += 1 * ( myContainer->width() + 5 );
    sw += 20;
    sh = myContainer->height() + 5;
  }
  return QSize( qMax( sz.width(), sw ), qMax( sz.height(), sh ) );
}

void QtxGroupBox::setInsideMargin( const int m )
{
  QVBoxLayout* bl = ::qobject_cast<QVBoxLayout*>( layout() );

  if ( !bl )
    return;

  QSpacerItem* spacer = 0;
  if ( bl->count() )
    spacer = bl->itemAt( 0 )->spacerItem();

  if ( !spacer )
    bl->insertSpacing( 0, m );
  else
    spacer->changeSize( 0, m );
}
