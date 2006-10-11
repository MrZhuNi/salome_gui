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
// File:      QtxStyleWrap.cxx
// Author:    Sergey TELKOV

#include "QtxStyleWrap.h"

#include <qwidget.h>

/*!
  Class QtxStyleWrap
*/

QtxStyleWrap::QtxStyleWrap( QStyle* s )
: QStyle(),
myStyle( s )
{
  myItems.setAutoDelete( true );
}

QtxStyleWrap::QtxStyleWrap( QWidget* w )
: QStyle(),
myStyle( 0 )
{
  myItems.setAutoDelete( true );

  if ( w )
  {
    myStyle = &w->style();
    w->setStyle( this );
  }
}

QtxStyleWrap::~QtxStyleWrap()
{
}

QStyle* QtxStyleWrap::style() const
{
  return myStyle;
}

void QtxStyleWrap::addItem( QtxStyleWrapItem* item )
{
  if ( myItems.contains( item ) )
    return;

  myItems.append( item );
  item->myWrap = this;
}

void QtxStyleWrap::removeItem( QtxStyleWrapItem* item )
{
  myItems.remove( item );
  if ( item->myWrap == this )
    item->myWrap = 0;
}

void QtxStyleWrap::polish( QWidget* w )
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->polish( w );
  if ( !over && style() )
    style()->polish( w );
}

void QtxStyleWrap::unPolish( QWidget* w )
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->unPolish( w );
  if ( !over && style() )
    style()->unPolish( w );
}

void QtxStyleWrap::polish( QApplication* a )
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->polish( a );
  if ( !over && style() )
    style()->polish( a );
}

void QtxStyleWrap::unPolish( QApplication* a )
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->unPolish( a );
  if ( !over && style() )
    style()->unPolish( a );
}

void QtxStyleWrap::polish( QPalette& p )
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->polish( p );
  if ( !over && style() )
    style()->polish( p );
}

void QtxStyleWrap::polishPopupMenu( QPopupMenu* pm )
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->polishPopupMenu( pm );
  if ( !over && style() )
    style()->polishPopupMenu( pm );
}

QRect QtxStyleWrap::itemRect( QPainter* p, const QRect& r, int flags, bool enabled,
                              const QPixmap* pixmap, const QString& text, int len ) const
{
  QRect res;
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->itemRect( p, r, flags, enabled, pixmap, text, len, res );
  if ( !over && style() )
    res = style()->itemRect( p, r, flags, enabled, pixmap, text, len );
  return res;
}

void QtxStyleWrap::drawItem( QPainter* p, const QRect& r, int flags, const QColorGroup& g,
                             bool enabled, const QPixmap* pixmap, const QString& text,
                             int len, const QColor* penColor ) const
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->drawItem( p, r, flags, g, enabled, pixmap, text, len, penColor );
  if ( !over && style() )
    style()->drawItem( p, r, flags, g, enabled, pixmap, text, len, penColor );
}

void QtxStyleWrap::drawPrimitive( PrimitiveElement pe, QPainter* p, const QRect& r,
                                  const QColorGroup& cg, SFlags flags, const QStyleOption& opt ) const
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->drawPrimitive( pe, p, r, cg, flags, opt );
  if ( !over && style() )
    style()->drawPrimitive( pe, p, r, cg, flags, opt );
}

void QtxStyleWrap::drawControl( ControlElement element, QPainter* p, const QWidget* widget,
                                const QRect& r, const QColorGroup& cg, SFlags flags, const QStyleOption& opt ) const
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->drawControl( element, p, widget, r, cg, flags, opt );
  if ( !over && style() )
    style()->drawControl( element, p, widget, r, cg, flags, opt );
}

void QtxStyleWrap::drawControlMask( ControlElement element, QPainter* p, const QWidget* widget,
                                    const QRect& r, const QStyleOption& opt ) const
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->drawControlMask( element, p, widget, r, opt );
  if ( !over && style() )
    style()->drawControlMask( element, p, widget, r, opt );
}

void QtxStyleWrap::drawComplexControl( ComplexControl control, QPainter* p, const QWidget* widget,
                                       const QRect& r, const QColorGroup& cg, SFlags how,
                                       SCFlags sub, SCFlags subActive, const QStyleOption& opt ) const
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->drawComplexControl( control, p, widget, r, cg, how, sub, subActive, opt );
  if ( !over && style() )
    style()->drawComplexControl( control, p, widget, r, cg, how, sub, subActive, opt );
}

void QtxStyleWrap::drawComplexControlMask( ComplexControl control, QPainter* p, const QWidget* widget,
                                           const QRect& r, const QStyleOption& opt ) const
{
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->drawComplexControlMask( control, p, widget, r, opt );
  if ( !over && style() )
    style()->drawComplexControlMask( control, p, widget, r, opt );
}

QRect QtxStyleWrap::subRect( SubRect subrect, const QWidget* widget ) const
{
  QRect res;
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->subRect( subrect, widget, res );
  if ( !over && style() )
    res = style()->subRect( subrect, widget );
  return res;
}

QRect QtxStyleWrap::querySubControlMetrics( ComplexControl control, const QWidget* widget,
                                            SubControl subcontrol, const QStyleOption& opt ) const
{
  QRect res;
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->querySubControlMetrics( control, widget, subcontrol, opt, res );
  if ( !over && style() )
    res = style()->querySubControlMetrics( control, widget, subcontrol, opt );
  return res;
}

QStyle::SubControl QtxStyleWrap::querySubControl( ComplexControl control, const QWidget* widget,
                                                  const QPoint& pos, const QStyleOption& opt ) const
{
  QStyle::SubControl res;
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->querySubControl( control, widget, pos, opt, res );
  if ( !over && style() )
    res = style()->querySubControl( control, widget, pos, opt );
  return res;
}

int QtxStyleWrap::pixelMetric( PixelMetric metric, const QWidget* widget ) const
{
  int res;
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->pixelMetric( metric, widget, res );
  if ( !over && style() )
    res = style()->pixelMetric( metric, widget );
  return res;
}

int QtxStyleWrap::styleHint( StyleHint stylehint, const QWidget* widget,
                             const QStyleOption & opt, QStyleHintReturn* returnData ) const
{
  int res;
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->styleHint( stylehint, widget, opt, returnData, res );
  if ( !over && style() )
    res = style()->styleHint( stylehint, widget, opt, returnData );
  return res;
}

QPixmap QtxStyleWrap::stylePixmap( StylePixmap stylepixmap,
                                   const QWidget* widget, const QStyleOption& opt ) const
{
  QPixmap res;
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->stylePixmap( stylepixmap, widget, opt, res );
  if ( !over && style() )
    res = style()->stylePixmap( stylepixmap, widget, opt );
  return res;
}

QSize QtxStyleWrap::sizeFromContents( ContentsType c, const QWidget* w,
                                      const QSize& sz, const QStyleOption& opt ) const
{
  QSize res;
  bool over = false;
  for ( ItemListIterator it( myItems ); it.current(); ++it )
    over = over || it.current()->sizeFromContents( c, w, sz, opt, res );
  if ( !over && style() )
    res = style()->sizeFromContents( c, w, sz, opt );
  return res;
}

/*!
  Class QtxStyleWrapItem
*/

QtxStyleWrapItem::QtxStyleWrapItem( QtxStyleWrap* wrap )
: myWrap( wrap )
{
  if ( wrap )
    wrap->addItem( this );
}

QtxStyleWrapItem::~QtxStyleWrapItem()
{
  if ( myWrap )
    myWrap->removeItem( this );
}

QStyle* QtxStyleWrapItem::style() const
{
  if ( styleWrap() )
    return styleWrap()->style();
  else
    return 0;
}

QtxStyleWrap* QtxStyleWrapItem::styleWrap() const
{
  return myWrap;
}

bool QtxStyleWrapItem::polish( QWidget* )
{
  return false;
}

bool QtxStyleWrapItem::unPolish( QWidget* )
{
  return false;
}

bool QtxStyleWrapItem::polish( QApplication* )
{
  return false;
}

bool QtxStyleWrapItem::unPolish( QApplication* )
{
  return false;
}

bool QtxStyleWrapItem::polish( QPalette& )
{
  return false;
}

bool QtxStyleWrapItem::polishPopupMenu( QPopupMenu* )
{
  return false;
}

bool QtxStyleWrapItem::itemRect( QPainter*, const QRect&, int, bool,
                                 const QPixmap*, const QString&, int, QRect& ) const
{
  return false;
}

bool QtxStyleWrapItem::drawItem( QPainter*, const QRect&, int, const QColorGroup&,
                                 bool, const QPixmap*, const QString&, int, const QColor* ) const
{
  return false;
}

bool QtxStyleWrapItem::drawPrimitive( QStyle::PrimitiveElement, QPainter*, const QRect&,
                                      const QColorGroup&, QStyle::SFlags, const QStyleOption& ) const
{
  return false;
}

bool QtxStyleWrapItem::drawControl( QStyle::ControlElement, QPainter*, const QWidget*, const QRect&,
                                    const QColorGroup&, QStyle::SFlags, const QStyleOption& ) const
{
  return false;
}

bool QtxStyleWrapItem::drawControlMask( QStyle::ControlElement, QPainter*, const QWidget*,
                                        const QRect&, const QStyleOption& ) const
{
  return false;
}

bool QtxStyleWrapItem::drawComplexControl( QStyle::ComplexControl, QPainter*, const QWidget*,
                                           const QRect&, const QColorGroup&, QStyle::SFlags,
                                           QStyle::SCFlags, QStyle::SCFlags, const QStyleOption& ) const
{
  return false;
}

bool QtxStyleWrapItem::drawComplexControlMask( QStyle::ComplexControl, QPainter*, const QWidget*,
                                               const QRect&, const QStyleOption& ) const
{
  return false;
}

bool QtxStyleWrapItem::subRect( QStyle::SubRect, const QWidget*, QRect& ) const
{
  return false;
}

bool QtxStyleWrapItem::querySubControlMetrics( QStyle::ComplexControl, const QWidget*,
                                               QStyle::SubControl, const QStyleOption&, QRect& ) const
{
  return false;
}

bool QtxStyleWrapItem::querySubControl( QStyle::ComplexControl, const QWidget*, const QPoint&,
                                        const QStyleOption&, QStyle::SubControl& ) const
{
  return false;
}

bool QtxStyleWrapItem::pixelMetric( QStyle::PixelMetric, const QWidget*, int& ) const
{
  return false;
}

bool QtxStyleWrapItem::styleHint( QStyle::StyleHint, const QWidget*,
                                  const QStyleOption&, QStyleHintReturn*, int& ) const
{
  return false;
}

bool QtxStyleWrapItem::stylePixmap( QStyle::StylePixmap, const QWidget*,
                                    const QStyleOption&, QPixmap& ) const
{
  return false;
}

bool QtxStyleWrapItem::sizeFromContents( QStyle::ContentsType, const QWidget*,
                                         const QSize&, const QStyleOption&, QSize& ) const
{
  return false;
}
