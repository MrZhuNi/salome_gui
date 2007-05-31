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
// File:      QtxGridBox.cxx
// Author:    Sergey TELKOV

#include "QtxGridBox.h"

#include <QLayout>
#include <QChildEvent>

/*!
  \ class QtxGridBox::Space
  Space in the grid box
*/
class QtxGridBox::Space : public QWidget
{
public:
  Space( const int, QtxGridBox* );
  virtual ~Space();

  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;

private:
  int           mySize;
  QtxGridBox*   myGrid;
};

QtxGridBox::Space::Space( const int sz, QtxGridBox* gb )
: QWidget( gb ),
mySize( sz ),
myGrid( gb )
{
  setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}

QtxGridBox::Space::~Space()
{
}

QSize QtxGridBox::Space::sizeHint() const
{
  return minimumSizeHint();
}

QSize QtxGridBox::Space::minimumSizeHint() const
{
  QSize sz( 0, 0 );
  if ( myGrid && myGrid->orientation() == Qt::Horizontal )
    sz.setWidth( mySize );
  else
    sz.setHeight( mySize );
  return sz;
}

/*!
  \ class QtxGridBox
*/
QtxGridBox::QtxGridBox( QWidget* parent, const int m, const int s )
: QWidget( parent ),
myCols( 1 ),
myOrient( Qt::Vertical ),
mySkip( false ),
myCol( 0 ),
myRow( 0 )
{
  myLayout = new QGridLayout( this );
  myLayout->setMargin( m );
  myLayout->setSpacing( s );
}

QtxGridBox::QtxGridBox( const int cols, Qt::Orientation o, QWidget* parent, const int m, const int s )
: QWidget( parent ),
myCols( cols ),
myOrient( o ),
myLayout( 0 ),
mySkip( false ),
myCol( 0 ),
myRow( 0 )
{
  myLayout = new QGridLayout( this );
  myLayout->setMargin( m );
  myLayout->setSpacing( s );
}

QtxGridBox::~QtxGridBox()
{
}

int QtxGridBox::columns() const
{
  return myCols;
}

Qt::Orientation QtxGridBox::orientation() const
{
  return myOrient;
}

void QtxGridBox::setColumns( const int cols )
{
  setLayout( cols, orientation() );
}

void QtxGridBox::setOrientation( Qt::Orientation o )
{
  setLayout( columns(), o );
}

void QtxGridBox::setLayout( const int cols, Qt::Orientation o )
{
  if ( myCols == cols && myOrient == o )
    return;

  myCols = cols;
  myOrient = o;

  arrangeWidgets();
}

bool QtxGridBox::skipInvisible() const
{
  return mySkip;
}

void QtxGridBox::setSkipInvisible( const bool on )
{
  if ( mySkip == on )
    return;

  mySkip = on;
  arrangeWidgets();
}

void QtxGridBox::addSpace( const int sp )
{
  new Space( sp, this );
}

int QtxGridBox::insideMargin() const
{
  return myLayout->margin();
}

int QtxGridBox::insideSpacing() const
{
  return myLayout->spacing();
}

void QtxGridBox::setInsideMargin( const int m )
{
  myLayout->setMargin( m );
}

void QtxGridBox::setInsideSpacing( const int s )
{
  myLayout->setSpacing( s );
}

bool QtxGridBox::eventFilter( QObject* o, QEvent* e )
{
  if ( skipInvisible() && ( e->type() == QEvent::Show || e->type() == QEvent::ShowToParent ||
                            e->type() == QEvent::Hide || e->type() == QEvent::HideToParent ) )
    arrangeWidgets();

  return QWidget::eventFilter( o, e );
}

void QtxGridBox::childEvent( QChildEvent* e )
{
  if ( e->child()->isWidgetType() )
  {
    QWidget* wid = (QWidget*)e->child();
    if ( e->type() == QEvent::ChildAdded )
    {
      placeWidget( wid );
      wid->installEventFilter( this );
    }
    else if ( e->type() == QEvent::ChildRemoved )
      wid->removeEventFilter( this );
  }
  QWidget::childEvent( e );
}

void QtxGridBox::skip()
{
  if ( orientation() == Qt::Horizontal )
  {
    myCol++;
    if ( myCol >= columns() )
    {
      myRow++;
      myCol = 0;
    }
  }
  else
  {
    myRow++;
    if ( myRow >= columns() )
    {
      myCol++;
      myRow = 0;
    }
  }
}

void QtxGridBox::arrangeWidgets()
{
  myRow = myCol = 0;
  int m = myLayout ? myLayout->margin() : 0;
  int s = myLayout ? myLayout->spacing() : 0;
  delete myLayout;
  myLayout = new QGridLayout( this );
  myLayout->setMargin( m );
  myLayout->setSpacing( s );

  QObjectList list = children();
  for ( QObjectList::iterator it = list.begin(); it != list.end(); ++it )
  {
    if ( !(*it)->isWidgetType() )
      continue;

    QWidget* wid = (QWidget*)(*it);
    if ( !skipInvisible() || wid->isVisibleTo( this ) )
      placeWidget( wid );
  }
  updateGeometry();
}

void QtxGridBox::placeWidget( QWidget* wid )
{
  myLayout->addWidget( wid, myRow, myCol );

  skip();
}
