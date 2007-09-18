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
// File:      QtxTable.cxx
// Author:    Sergey TELKOV

#include "QtxTable.h"

#ifndef QT_NO_TABLE

#include "QtxStyleWrap.h"

#include <qstyle.h>
#include <qpainter.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qmemarray.h>
#include <qtoolbutton.h>
#include <qstylesheet.h>
#include <qapplication.h>
#include <qsimplerichtext.h>

/*!
  Class QtxTable::Header
  Internal header for QtxTable
*/

class QtxTable::Header : public QHeader
{
public:
  Header( int, QtxTable*, const char* = 0 );
  virtual ~Header();

  QtxTable*     table() const;

  void          clear();

  int           horizontalSpan( const int ) const;
  void          setHorizontalSpan( const int, const int );

  int           verticalSpan( const int ) const;
  void          setVerticalSpan( const int, const int );

  QColor        foregroundColor( const int ) const;
  QColor        backgroundColor( const int ) const;
  void          setForegroundColor( const int, const QColor& );
  void          setBackgroundColor( const int, const QColor& );

  void          removeSection( const int );
  virtual void  setLabel( int, const QString&, int = -1 );

  QRect         indexRect( const int, int* = 0 );
  void          swapSections( const int, const int );
  int           spanedSection( const int index );

  virtual QSize sizeHint() const;

protected:
  virtual void  mouseMoveEvent( QMouseEvent* );
  virtual void  mousePressEvent( QMouseEvent* );
  virtual void  mouseReleaseEvent( QMouseEvent* );
  virtual void  paintSection( QPainter*, int, const QRect& );
  virtual void  paintEvent( QPaintEvent * );

private:
  typedef QMap<int, int>        SpanMap;
  typedef QMap<int, QColor>     ColorMap;
  typedef QPair<int, int>       SpanRange;
  typedef QMap<int, SpanRange>  SpanRangeMap;

private:
  QHeader*      mainHeader() const;
  bool          filterEvent( QMouseEvent* );

  SpanRange     findSpanRange( const int );

  QPoint        sectionCenter( const int, const QPoint& = QPoint() ) const;

private:
  SpanMap       myHSpan;
  SpanMap       myVSpan;
  QtxTable*     myTable;
  ColorMap      myFgColor;
  ColorMap      myBgColor;
  int           myPressed;
  int           mySection;
  SpanRangeMap  mySpanRanges;
  friend class QtxTable::StyleItem;
};

QtxTable::Header::Header( int n, QtxTable* table, const char* name )
: QHeader( n, table, name ),
myTable( table ),
myPressed( -1 ),
mySection( -1 )
{
  setWFlags( WRepaintNoErase );
}

QtxTable::Header::~Header()
{
}

QtxTable* QtxTable::Header::table() const
{
  return myTable;
}

void QtxTable::Header::clear()
{
  for ( int i = 0; i < (int)count(); i++ )
    setLabel( i, orientation() == Horizontal ? QString( "" ) : QString::null );
  myHSpan.clear();
  myVSpan.clear();
  myFgColor.clear();
  myBgColor.clear();
}

QSize QtxTable::Header::sizeHint() const
{
  QSize sz = QHeader::sizeHint();

  int size = orientation() == Horizontal ? sz.height() : sz.width();

  for ( int i = 0; i < (int)count(); i++ )
  {
    QString lab = label( mapToSection( i ) );
    if ( !QStyleSheet::mightBeRichText( lab ) )
      continue;

    int s = 0;
    QIconSet* icon = iconSet( mapToSection( i ) );
    if ( icon )
    {
	    QPixmap pixmap = icon->pixmap( QIconSet::Small, QIconSet::Normal );
      s = orientation() == Horizontal ? pixmap.height() : pixmap.width();
    }

    QStyleSheet sheet;
    QStyleSheetItem* item = sheet.item( "p" );
    if ( item )
    {
      item->setMargin( QStyleSheetItem::MarginAll, 0 );
      item->setWhiteSpaceMode( QStyleSheetItem::WhiteSpaceNoWrap );
    }

    QSimpleRichText rt( QString( "<p>%1</p>" ).arg( lab ), font(), QString::null, &sheet );
    if ( orientation() == Horizontal )
      rt.setWidth( sectionSize( mapToSection( i ) ) );
    s += orientation() == Horizontal ? rt.height() : rt.width();
 
    size = QMAX( size, s );
  }

  if ( orientation() == Horizontal )
    sz.setHeight( size );
  else
    sz.setWidth( size );

  return sz;
}

void QtxTable::Header::removeSection( const int section )
{
  int old = count();

  removeLabel( section );

  if ( old == count() )
    return;

  SpanMap newHMap, newVMap;
  for ( SpanMap::ConstIterator hIt = myHSpan.begin(); hIt != myHSpan.end(); ++hIt )
  {
    if ( hIt.key() == section )
      continue;
    newHMap.insert( hIt.key() < section ? hIt.key() : hIt.key() - 1, hIt.data() );
  }
  myHSpan = newHMap;

  for ( SpanMap::ConstIterator vIt = myVSpan.begin(); vIt != myVSpan.end(); ++vIt )
  {
    if ( vIt.key() == section )
      continue;
    newVMap.insert( vIt.key() < section ? vIt.key() : vIt.key() - 1, vIt.data() );
  }
  myVSpan = newVMap;

  mySpanRanges.clear();

  ColorMap newFMap, newBMap;
  for ( ColorMap::ConstIterator fIt = myFgColor.begin(); fIt != myFgColor.end(); ++fIt )
  {
    if ( fIt.key() == section )
      continue;
    newFMap.insert( fIt.key() < section ? fIt.key() : fIt.key() - 1, fIt.data() );
  }
  myFgColor = newFMap;

  for ( ColorMap::ConstIterator bIt = myBgColor.begin(); bIt != myBgColor.end(); ++bIt )
  {
    if ( bIt.key() == section )
      continue;
    newBMap.insert( bIt.key() < section ? bIt.key() : bIt.key() - 1, bIt.data() );
  }
  myBgColor = newBMap;
}

void QtxTable::Header::setLabel( int section, const QString& s, int size )
{
  QHeader::setLabel( section, s, size );
  if ( table() && isUpdatesEnabled() )
    table()->updateHeaderGeometries( orientation() );
}

int QtxTable::Header::horizontalSpan( const int section ) const
{
  return myHSpan.contains( section ) ? myHSpan[section] : 1;
}

void QtxTable::Header::setHorizontalSpan( const int section, const int sp )
{
  if ( horizontalSpan( section ) == sp )
    return;

  mySpanRanges.clear();
  myHSpan.insert( section, sp );
  if ( isUpdatesEnabled() )
    repaint( indexRect( mapToIndex( section ) ) );
}

int QtxTable::Header::verticalSpan( const int section ) const
{
  return myVSpan.contains( section ) ? myVSpan[section] : 1;
}

void QtxTable::Header::setVerticalSpan( const int section, const int sp )
{
  if ( verticalSpan( section ) == sp )
    return;

  mySpanRanges.clear();
  myVSpan.insert( section, sp );
  if ( isUpdatesEnabled() )
    repaint( indexRect( mapToIndex( section ) ) );
}

QColor QtxTable::Header::foregroundColor( const int section ) const
{
  return myFgColor.contains( section ) ? myFgColor[section] : QColor();
}

QColor QtxTable::Header::backgroundColor( const int section ) const
{
  return myBgColor.contains( section ) ? myBgColor[section] : QColor();
}

void QtxTable::Header::setForegroundColor( const int section, const QColor& c )
{
  if ( foregroundColor( section ) == c )
    return;

  myFgColor.insert( section, c );
  if ( isUpdatesEnabled() )
    repaint( indexRect( mapToIndex( section ) ) );
}

void QtxTable::Header::setBackgroundColor( const int section, const QColor& c )
{
  if ( backgroundColor( section ) == c )
    return;

  myBgColor.insert( section, c );
  if ( isUpdatesEnabled() )
    repaint( indexRect( mapToIndex( section ) ) );
}

void QtxTable::Header::swapSections( const int oldIdx, const int newIdx )
{
  QIconSet oldIconSet, newIconSet;
  if ( iconSet( oldIdx ) )
	  oldIconSet = *iconSet( oldIdx );
  if ( iconSet( newIdx ) )
	  newIconSet = *iconSet( newIdx );
  QString oldLabel = label( oldIdx );
  QString newLabel = label( newIdx );
  QHeader::setLabel( oldIdx, newIconSet, newLabel );
  QHeader::setLabel( newIdx, oldIconSet, oldLabel );

  int oldHSpan = horizontalSpan( oldIdx );
  int newHSpan = horizontalSpan( newIdx );
  setHorizontalSpan( oldIdx, newHSpan );
  setHorizontalSpan( newIdx, oldHSpan );

  int oldVSpan = verticalSpan( oldIdx );
  int newVSpan = verticalSpan( newIdx );
  setVerticalSpan( oldIdx, newVSpan );
  setVerticalSpan( newIdx, oldVSpan );

  QColor oldFg = foregroundColor( oldIdx );
  QColor newFg = foregroundColor( newIdx );
  setForegroundColor( oldIdx, newFg );
  setForegroundColor( newIdx, oldFg );

  QColor oldBg = backgroundColor( oldIdx );
  QColor newBg = backgroundColor( newIdx );
  setBackgroundColor( oldIdx, newBg );
  setBackgroundColor( newIdx, oldBg );
}

void QtxTable::Header::mouseMoveEvent( QMouseEvent* e )
{
  if ( e->button() == NoButton && filterEvent( e ) )
    return;

  QHeader::mouseMoveEvent( e );
  if ( mainHeader() )
    QApplication::sendEvent( mainHeader(), e );
}

void QtxTable::Header::mousePressEvent( QMouseEvent* e )
{
  if ( filterEvent( e ) )
    return;

  int index = mapToIndex( sectionAt( ( orientation() == Horizontal ? e->pos().x() : e->pos().y() ) + offset() ) );
  SpanRange range = findSpanRange( index );

  bool shift = e->state() & ShiftButton;
  if ( mainHeader() )
    QApplication::sendEvent( mainHeader(), e );

  for ( int i = range.second; i >= range.first && !shift; i-- )
  {
    QPoint pos = sectionCenter( i, e->pos() );
    QMouseEvent me( e->type(), pos, e->button(), e->state() | ControlButton );
    if ( i == range.first )
      QHeader::mousePressEvent( &me );
    if ( mainHeader() )
      QApplication::sendEvent( mainHeader(), &me );
  }

  if ( !shift )
  {
    QMouseEvent me( e->type(), sectionCenter( index, e->pos() ), e->button(), e->state() | ControlButton );
    if ( mainHeader() )
      QApplication::sendEvent( mainHeader(), &me );
  }

  myPressed = range.first;
  repaint( indexRect( range.first ) );
}

void QtxTable::Header::mouseReleaseEvent( QMouseEvent* e )
{
  QHeader::mouseReleaseEvent( e );
  if ( mainHeader() )
    QApplication::sendEvent( mainHeader(), e );

  repaint( indexRect( myPressed ) );
  myPressed = -1;
}

void QtxTable::Header::paintSection( QPainter* p, int index, const QRect& fr )
{
  int idx = index;
  QRect r = fr;
  if ( index < count() )
    r = indexRect( index, &idx );

  // Currently painted section number. This number will be used in the drawing style.
  // Because QHeader::paintSection() implementation doesn't send section id through style options :(
  mySection = mapToSection( index );

  QHeader::paintSection( p, idx, r );

  mySection = -1;
}

void QtxTable::Header::paintEvent( QPaintEvent *pe )
{
  QRect r = rect();
  QPixmap pix( r.width(), r.height() );
  QPainter::redirect( this, &pix );
  QHeader::paintEvent( pe );
  QPainter::redirect( this, 0 );
  QPainter painter( this );
  painter.drawPixmap( r, pix );
  //QHeader::paintEvent( pe );
}

bool QtxTable::Header::filterEvent( QMouseEvent* e )
{
  int c = orientation() == Horizontal ? e->pos().x() : e->pos().y();
  c += offset();
/*
  if ( reverse() )
	  c = d->lastPos - c;
*/
  int section = sectionAt( c );
  if ( section < 0 )
    return false;

  int handleIdx = -1;
  int margin = style().pixelMetric( QStyle::PM_HeaderGripMargin );
  int index = mapToIndex( section );

  QRect r = sectionRect( section );
  int size = sectionSize( section );
  int pos = ( orientation() == Horizontal ? r.x() : r.y() ) + offset();
  if ( c < pos + margin )
	  handleIdx = index - 1;
  else if ( c > pos + size - margin )
	  handleIdx = index;

  if ( handleIdx == -1 )
    return false;

  return findSpanRange( handleIdx ).second != handleIdx;
}

QPoint QtxTable::Header::sectionCenter( const int index, const QPoint& p ) const
{
  QPoint pos = p;
  if ( orientation() == Horizontal )
    pos.setX( sectionPos( mapToSection( index ) ) + sectionSize( mapToSection( index ) ) / 2 - offset() );
  else
    pos.setY( sectionPos( mapToSection( index ) ) + sectionSize( mapToSection( index ) ) / 2 - offset() );

  if ( p.isNull() )
  {
    if ( orientation() == Horizontal )
      pos.setY( height() / 2 );
    else
      pos.setX( width() / 2 );
  }
  return pos;
}

QtxTable::Header::SpanRange QtxTable::Header::findSpanRange( const int index )
{
  SpanRange res( index, index );
  if ( mySpanRanges.isEmpty() )
  {
    // no span sections and no first simple sections stored in map
    // so we have to fill it.
    for ( int i = 0; i < (int)count(); i++ )
    {
      int sp = horizontalSpan( mapToSection( i ) );
      sp = QMAX( sp, 1 );
      SpanRange range( i, QMIN( i + sp - 1, count() - 1 ) );
      if ( range.first == range.second && !mySpanRanges.isEmpty() )
        continue; // do not store simple section without span (store only first)
      mySpanRanges.insert( i, range );
      for ( int j = i + 1; j < i + sp; j++ )
        mySpanRanges.insert( j, range );
      i += sp - 1;
    }
  }
  
  if ( mySpanRanges.contains( index ) )
    res = mySpanRanges[ index ];


  /* try to optimise by checking range during step from one to next
  SpanRangeList ranges;
  spanRanges( ranges );

  for ( SpanRangeList::const_iterator it = ranges.begin(); it != ranges.end() && res.first < 0; ++it )
  {
    if ( (*it).first <= index && index <= (*it).second )
      res = *it;
  }
  */
  for ( int i = 0; i < (int)count() && res.first < 0; i++ )
  {
    int sp = horizontalSpan( mapToSection( i ) );
    sp = QMAX( sp, 1 );
    SpanRange range( i, QMIN( i + sp - 1, count() - 1 ) );
    if ( range.first <= index && index <= range.second )
      res = range;
    i += sp - 1;
  }
  return res;
}

QRect QtxTable::Header::indexRect( const int index, int* start )
{
  SpanRange range = findSpanRange( index );

  if ( start )
    *start = range.first;

  QRect r;
  for ( int i = range.first; i <= range.second; i++ )
    r = r.unite( sRect( i ) );
  return r;
}

int QtxTable::Header::spanedSection( const int index )
{
  SpanRange range = findSpanRange( index );
  return range.first;
}

QHeader* QtxTable::Header::mainHeader() const
{
  if ( !table() )
    return 0;

  return orientation() == Horizontal ? table()->horizontalHeader() :
                                       table()->verticalHeader();
}

/*!
  Class QtxTable::StyleItem
*/

class QtxTable::StyleItem : public QtxStyleWrapItem
{
public:
  StyleItem( QtxStyleWrap* );
  ~StyleItem();

  virtual bool drawControl( QStyle::ControlElement, QPainter*, const QWidget*, const QRect&,
                            const QColorGroup&, QStyle::SFlags, const QStyleOption& ) const;
  virtual bool drawPrimitive( QStyle::PrimitiveElement, QPainter*, const QRect&,
                              const QColorGroup&, QStyle::SFlags, const QStyleOption& ) const;
};

QtxTable::StyleItem::StyleItem( QtxStyleWrap* wrap )
: QtxStyleWrapItem( wrap )
{
}

QtxTable::StyleItem::~StyleItem()
{
}

bool QtxTable::StyleItem::drawControl( QStyle::ControlElement element, QPainter* p, const QWidget* widget,
                                       const QRect& r, const QColorGroup& cg, QStyle::SFlags flags,
                                       const QStyleOption& opt ) const
{
  if ( element != QStyle::CE_HeaderLabel )
    return false;

	const QHeader* header = (const QHeader*)widget;
	int section = opt.headerSection();

  QColor fc;
  QTable* table = ::qt_cast<QTable*>( header->parent() );
  if ( table && table->verticalHeader() != header && table->horizontalHeader() != header )
    fc = ((Header*)header)->foregroundColor( section );

  QColorGroup grp( cg );
  if ( fc.isValid() )
    grp.setColor( QColorGroup::ButtonText, fc );
  grp.setColor( QColorGroup::Text, grp.buttonText() );

  QString lab = header->label( section );
  if ( !QStyleSheet::mightBeRichText( lab ) )
  {
    if ( style() )
		  style()->drawControl( element, p, widget, r, grp, flags, opt );
  }
  else
  {
    QRect rect = r;
    QIconSet* icon = header->iconSet( section );
    if ( icon )
    {
	    QPixmap pixmap = icon->pixmap( QIconSet::Small, flags & QStyle::Style_Enabled ? QIconSet::Normal :
                                                                                      QIconSet::Disabled );
      int pixw = pixmap.width();
		  int pixh = pixmap.height();

		  QRect pixRect = rect;
		  pixRect.setY( rect.center().y() - (pixh - 1) / 2 );
      if ( style() )
		    style()->drawItem( p, pixRect, AlignVCenter, cg, flags & QStyle::Style_Enabled, &pixmap, QString::null );
      rect.setLeft( rect.left() + pixw + 2 );
    }

    QStyleSheet sheet;
    QStyleSheetItem* i = sheet.item( "p" );
    if ( i )
    {
      i->setMargin( QStyleSheetItem::MarginAll, 0 );
      i->setWhiteSpaceMode( QStyleSheetItem::WhiteSpaceNoWrap );
    }

    QSimpleRichText rt( QString( "<p>%1</p>" ).arg( lab ), header->font(), QString::null, &sheet );
    rt.setWidth( rect.width() );
    rt.draw( p, rect.x(), rect.y() + ( rect.height() - rt.height() ) / 2, rect, grp );
  }

  return true;
}

bool QtxTable::StyleItem::drawPrimitive( QStyle::PrimitiveElement pe, QPainter* p, const QRect& r,
                                         const QColorGroup& cg, QStyle::SFlags flags, const QStyleOption& opt ) const
{
  if ( pe != QStyle::PE_HeaderSection )
    return false;

  if ( opt.isDefault() )
    return false;

  QHeader* hdr = ::qt_cast<QHeader*>( opt.widget() );
  if ( !hdr )
    return false;

  QTable* table = ::qt_cast<QTable*>( hdr->parent() );
  if ( table && ( table->verticalHeader() == hdr || table->horizontalHeader() == hdr ) )
    return false;

  Header* h = (Header*)hdr;
  int section = h->mySection;

  if ( section < 0 )
    return false;

  QColorGroup grp( cg );
  QColor c = h->backgroundColor( section );
  if ( c.isValid() )
  {
    grp.setColor( QColorGroup::Button, c );
    grp.setColor( QColorGroup::Light, c );
  }
  if ( style() )
    style()->drawPrimitive( pe, p, r, grp, flags, opt );

  return true;
}

/*!
  Class QtxTable
*/

/*!
  Constructor
*/
QtxTable::QtxTable( QWidget* parent, const char* name )
: QTable( parent, name ),
myStyleWrapper( 0 ),
myHeaderEditor( 0 ),
myEditedHeader( 0 ),
myEditedSection( -1 ),
mySelectAll( 0 )
{
  myVerHeaders.setAutoDelete( true );
  myHorHeaders.setAutoDelete( true );

  verticalHeader()->setStyle( styleWrapper() );
  horizontalHeader()->setStyle( styleWrapper() );

  connect( verticalHeader(), SIGNAL( sizeChange( int, int, int ) ),
           this, SLOT( onHeaderSizeChange( int, int, int ) ) );
  connect( horizontalHeader(), SIGNAL( sizeChange( int, int, int ) ),
           this, SLOT( onHeaderSizeChange( int, int, int ) ) );
  connect( verticalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( onScrollBarMoved( int ) ) );
  connect( horizontalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( onScrollBarMoved( int ) ) );

  updateHeaders( Horizontal );
  updateHeaders( Vertical );
}

/*!
  Constructor
*/
QtxTable::QtxTable( int numRows, int numCols, QWidget* parent, const char* name )
: QTable( numRows, numCols, parent, name ),
myStyleWrapper( 0 ),
myHeaderEditor( 0 ),
myEditedHeader( 0 ),
myEditedSection( -1 ),
mySelectAll( 0 )
{
  myVerHeaders.setAutoDelete( true );
  myHorHeaders.setAutoDelete( true );

  verticalHeader()->setStyle( styleWrapper() );
  horizontalHeader()->setStyle( styleWrapper() );

  connect( verticalHeader(), SIGNAL( sizeChange( int, int, int ) ),
           this, SLOT( onHeaderSizeChange( int, int, int ) ) );
  connect( horizontalHeader(), SIGNAL( sizeChange( int, int, int ) ),
           this, SLOT( onHeaderSizeChange( int, int, int ) ) );
  connect( verticalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( onScrollBarMoved( int ) ) );
  connect( horizontalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( onScrollBarMoved( int ) ) );
}

/*!
  Destructor
*/
QtxTable::~QtxTable()
{
  delete myStyleWrapper;
}

bool QtxTable::isSelectAllEnabled() const
{
  return mySelectAll;
}

void QtxTable::setSelectAllEnabled( const bool on )
{
  if ( isSelectAllEnabled() == on )
    return;

  if ( on )
    connect( mySelectAll = new QToolButton( this ), SIGNAL( clicked() ), this, SLOT( selectAll() ) );
  else
  {
    delete mySelectAll;
    mySelectAll = 0;
  }
  updateSelectAllButton();
}

/*!
  Select the all cells of the table.
*/
void QtxTable::selectAll()
{
  if ( selectionMode() != Multi && selectionMode() != MultiRow )
    return;

  selectCells( 0, 0, numRows() - 1, numCols() - 1 );
}

/*!
  Reimplemented for 'SelectAll' button geometry updating.
*/
void QtxTable::setTopMargin( int m )
{
  QTable::setTopMargin( m );
  updateSelectAllButton();
}

/*!
  Reimplemented for 'SelectAll' button geometry updating.
*/
void QtxTable::setLeftMargin( int m )
{
  QTable::setLeftMargin( m );
  updateSelectAllButton();
}

/*!
  Reimplemented for 'SelectAll' button updating.
*/
void QtxTable::setSelectionMode( SelectionMode mode )
{
  QTable::setSelectionMode( mode );
  updateSelectAllButton();
}

/*!
  \return true if header is editable
  \param o - header orientation
*/
bool QtxTable::headerEditable( Orientation o, const int idx ) const
{
  QHeader* hdr = idx < 0 ? header( o ) : header( o, idx );
  return myHeaderEditable.contains( hdr ) ? myHeaderEditable[hdr] : false;
}

/*!
  Changes editable state of header
  \param o - header orientation
  \param on - new state
*/
void QtxTable::setHeaderEditable( Orientation o, const bool on, const int idx )
{
  if ( headerEditable( o, idx ) == on )
    return;

  QHeader* hdr = idx < 0 ? header( o ) : header( o, idx );
  myHeaderEditable.insert( hdr, on );

  if ( !on && myEditedHeader == hdr )
    endHeaderEdit( false );

  if ( on )
    hdr->installEventFilter( this );
  else
    hdr->removeEventFilter( this );
}

void QtxTable::setHeadersEditable( Orientation o, bool on )
{
  setHeaderEditable( o, on );
  for ( int i = 0; i < numHeaders( o ); i++ )
    setHeaderEditable( o, on, i );
}

/*!
  Starts edition of header
  \param o - header orientation
  \param sec - column/row
*/
bool QtxTable::editHeader( Orientation o, const int sec )
{
  return beginHeaderEdit( o, sec );
}

/*!
  Finishes edition of header
  \param accept - whether new value must be accepted
*/
void QtxTable::endEditHeader( const bool accept )
{
  endHeaderEdit( accept );
}

/*!
  Custom event filter
  Starts edition of header by double click
  Finishes edition by escape/return/enter pressing
*/
bool QtxTable::eventFilter( QObject* o, QEvent* e )
{
  if ( e->type() == QEvent::MouseButtonDblClick )
  {
    QMouseEvent* me = (QMouseEvent*)e;
    QHeader* hdr = ::qt_cast<QHeader*>( o );
    if ( hdr )
      return beginHeaderEdit( hdr, me->pos() );
  }

  if ( o == myHeaderEditor && e->type() == QEvent::KeyPress && isHeaderEditing() )
  {
	  QKeyEvent* ke = (QKeyEvent*)e;
    if ( ke->key() == Key_Escape )
    {
      endHeaderEdit( false );
      return true;
    }

    if ( ke->state() == NoButton && ( ke->key() == Key_Return || ke->key() == Key_Enter ) )
    {
      endHeaderEdit( true );
      return true;
    }

    return false;
  }

  if ( o == myHeaderEditor && e->type() == QEvent::FocusOut &&
       ((QFocusEvent*)e)->reason() != QFocusEvent::Popup )
  {
		  endHeaderEdit();
		  return true;
  }

  if ( e->type() == QEvent::Wheel && isHeaderEditing() )
    return true;

  return QTable::eventFilter( o, e );
}

void QtxTable::adjustRow( int row )
{
  QTable::adjustRow( row );

  int size = 0;
  for ( uint i = 0; i < myVerHeaders.count(); i++ )
  {
    QHeader* hdr = myVerHeaders.at( i );
    QString txt = hdr->label( row );
    int h = hdr->iconSet( row ) ? hdr->iconSet( row )->pixmap().height() : 0;
    if ( !QStyleSheet::mightBeRichText( txt ) )
      h = QMAX( h, hdr->fontMetrics().height() );
    else
    {
      QStyleSheet sheet;
      QStyleSheetItem* item = sheet.item( "p" );
      if ( item )
        item->setMargin( QStyleSheetItem::MarginAll, 0 );

      QSimpleRichText rt( txt, hdr->font(), QString::null, &sheet );
      rt.setWidth( hdr->width() );
      h = QMAX( h, rt.height() );
    }
    size = QMAX( size, h );
  }

  size = QMAX( size, rowHeight( row ) );
  setRowHeight( row, size );

}

void QtxTable::adjustColumn( int col )
{
  QTable::adjustColumn( col );

  int size = 0;
  for ( uint i = 0; i < myHorHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myHorHeaders.at( i );
    QString txt = hdr->label( col );
    int w = hdr->iconSet( col ) ?
            hdr->iconSet( col )->pixmap( QIconSet::Automatic, QIconSet::Normal ).width() + 5 : 0;
    int txtW = 0;
    if ( !QStyleSheet::mightBeRichText( txt ) )
      txtW += hdr->fontMetrics().width( txt );
    else
    {
      QStyleSheet sheet;
      QStyleSheetItem* item = sheet.item( "p" );
      if ( item )
      {
        item->setMargin( QStyleSheetItem::MarginAll, 0 );
        item->setWhiteSpaceMode( QStyleSheetItem::WhiteSpaceNoWrap );
      }

      QSimpleRichText rt( QString( "<p>%1</p>" ).arg( txt ), hdr->font(), QString::null, &sheet );
      txtW += rt.width();
    }

    if ( hdr->horizontalSpan( col ) > 1 )
      txtW = txtW / hdr->horizontalSpan( col );

    w = w + txtW + 10;

    size = QMAX( size, w );
  }

  for ( int i = 0; i < numRows(); i++ )
  {
    QFont fnt = cellFont( i, col );
    int sz = QFontMetrics( fnt ).width( text( i, col ) ) + 10;
    size = QMAX( size, sz );
  }

  size = QMAX( size, columnWidth( col ) );
  setColumnWidth( col, size );
}

void QtxTable::rowHeightChanged( int row )
{
  QTable::rowHeightChanged( row );

  int h = rowHeight( row );
  for ( uint i = 0; i < myVerHeaders.count(); i++ )
  {
    QHeader* hdr = myVerHeaders.at( i );
    hdr->resizeSection( row, h );
  }
}

void QtxTable::columnWidthChanged( int col )
{
  QTable::columnWidthChanged( col );

  int w = columnWidth( col );
  for ( uint i = 0; i < myHorHeaders.count(); i++ )
  {
    QHeader* hdr = myHorHeaders.at( i );
    hdr->resizeSection( col, w );
  }
}

/*!
  Reimplemented for internal reasons.
  Firstly end the edition of header.
*/
void QtxTable::setNumRows( int rows )
{
  endHeaderEdit();
  int old = numRows();

  QTable::setNumRows( rows );

  for ( uint i = 0; i < myVerHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myVerHeaders.at( i );
    for ( int d = hdr->count() - 1; d >= (int)numRows(); d-- )
      hdr->removeSection( d );
    for ( int c = hdr->count(); c < (int)numRows(); c++ )
      hdr->addLabel( QString::null );
  }

  for ( int j = rows + 1; j <= old; j++ )
      myCellProps.remove( j );
}

/*!
  Reimplemented for internal reasons.
  Firstly end the edition of header.
*/
void QtxTable::setNumCols( int cols )
{
  endHeaderEdit();
  int old = numCols();

  QTable::setNumCols( cols );

  for ( uint i = 0; i < myHorHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myHorHeaders.at( i );
    for ( int d = hdr->count() - 1; d >= (int)numCols(); d-- )
      hdr->removeSection( d );
    for ( int c = hdr->count(); c < (int)numCols(); c++ )
      hdr->addLabel( QString::null );
  }

  for ( CellMap::Iterator it = myCellProps.begin(); it != myCellProps.end(); ++it )
  {
    PropsMap& map = it.data();
    for ( int i = cols + 1; i <= old; i++ )
      map.remove( i );
  }
}

/*!
  Reimplemented for internal reasons.
  Firstly end the edition of header.
*/
void QtxTable::insertRows( int row, int count )
{
  endHeaderEdit();

  QTable::insertRows( row, count );

  for ( uint i = 0; i < myVerHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myVerHeaders.at( i );
    for ( int j = numRows() - count - 1; j >= row; --j )
	    hdr->swapSections( j, j + count );
  }

  CellMap tmp = myCellProps;
  myCellProps.clear();
  for ( CellMap::Iterator it = tmp.begin(); it != tmp.end(); ++it )
  {
    int r = it.key() < row ? it.key() : it.key() + count;
    myCellProps.insert( r, it.data() );
  }
}

/*!
  Reimplemented for internal reasons.
  Firstly end the edition of header.
*/
void QtxTable::insertColumns( int col, int count )
{
  endHeaderEdit();

  QTable::insertColumns( col, count );

  for ( uint i = 0; i < myHorHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myHorHeaders.at( i );
    for ( int j = numCols() - count - 1; j >= col; --j )
	    hdr->swapSections( j, j + count );
  }

  for ( CellMap::Iterator itr = myCellProps.begin(); itr != myCellProps.end(); ++itr )
  {
    PropsMap tmp = itr.data();
    itr.data().clear();
    for ( PropsMap::Iterator it = tmp.begin(); it != tmp.end(); ++it )
    {
      int c = it.key() < col ? it.key() : it.key() + count;
      itr.data().insert( c, it.data() );
    }
  }
}

/*!
  Reimplemented for internal reasons.
  Firstly end the edition of header.
*/
void QtxTable::removeRow( int row )
{
  endHeaderEdit();

  for ( uint i = 0; i < myVerHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myVerHeaders.at( i );
    hdr->removeSection( row );
  }

  CellMap tmp = myCellProps;
  myCellProps.clear();
  for ( CellMap::Iterator it = tmp.begin(); it != tmp.end(); ++it )
  {
    if ( it.key() == row )
      continue;

    int r = it.key() < row ? it.key() : it.key() - 1;
    myCellProps.insert( r, it.data() );
  }

  QTable::removeRow( row );
}

/*!
  Reimplemented for internal reasons.
  Firstly end the edition of header.
*/
void QtxTable::removeRows( const QMemArray<int>& rows )
{
  endHeaderEdit();

  int decr = 0;
  QMap<int, int> indexes;
  for ( int r = 0; r < numRows(); r++ )
  {
    if ( rows.contains( r ) )
      decr++;
    else
      indexes.insert( r, r - decr );
  }

  for ( uint i = 0; i < myVerHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myVerHeaders.at( i );
    for ( int d = rows.count() - 1; d >= 0; d-- )
      hdr->removeSection( rows[d] );
  }

  CellMap tmp = myCellProps;
  myCellProps.clear();
  for ( CellMap::Iterator it = tmp.begin(); it != tmp.end(); ++it )
  {
    if ( indexes.contains( it.key() ) )
      myCellProps.insert( indexes[it.key()], it.data() );
  }

  QTable::removeRows( rows );
}

/*!
  Reimplemented for internal reasons.
  Firstly end the edition of header.
*/
void QtxTable::removeColumn( int col )
{
  endHeaderEdit();

  for ( uint i = 0; i < myHorHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myHorHeaders.at( i );
    hdr->removeSection( col );
  }

  for ( CellMap::Iterator itr = myCellProps.begin(); itr != myCellProps.end(); ++itr )
  {
    PropsMap tmp = itr.data();
    itr.data().clear();
    for ( PropsMap::Iterator it = tmp.begin(); it != tmp.end(); ++it )
    {
      if ( it.key() == col )
        continue;

      int c = it.key() < col ? it.key() : it.key() - 1;
      itr.data().insert( c, it.data() );
    }
  }

  QTable::removeColumn( col );
}

/*!
  Reimplemented for internal reasons.
  Firstly end the edition of header.
*/
void QtxTable::removeColumns( const QMemArray<int>& cols )
{
  endHeaderEdit();

  int decr = 0;
  QMap<int, int> indexes;
  for ( int c = 0; c < numCols(); c++ )
  {
    if ( cols.contains( c ) )
      decr++;
    else
      indexes.insert( c, c - decr );
  }

  for ( uint i = 0; i < myHorHeaders.count(); i++ )
  {
    Header* hdr = (Header*)myHorHeaders.at( i );
    for ( int d = cols.count() - 1; d >= 0; d-- )
      hdr->removeSection( cols[d] );
  }

  for ( CellMap::Iterator itr = myCellProps.begin(); itr != myCellProps.end(); ++itr )
  {
    PropsMap tmp = itr.data();
    itr.data().clear();
    for ( PropsMap::Iterator it = tmp.begin(); it != tmp.end(); ++it )
    {
      if ( indexes.contains( it.key() ) )
        itr.data().insert( indexes[it.key()], it.data() );
    }
  }

  QTable::removeColumns( cols );
}

void QtxTable::setUpdatesEnabled( bool enable )
{
  QTable::setUpdatesEnabled( enable );

  if ( enable )
  {
    updateHeaderGeometries( Horizontal );
    updateHeaderGeometries( Vertical );
  }
}

QHeader* QtxTable::header( const Orientation o, const int idx ) const
{
  HeaderVector* vec = headerVector( o );

  if ( idx < 0 || idx >= (int)vec->count() )
    return 0;

  return vec->at( idx );
}

int QtxTable::numHeaders( const Orientation o ) const
{
  return headerVector( o )->count();
}

void QtxTable::setNumHeaders( const Orientation o, const int n )
{
  headerVector( o )->resize( QMAX( n, 0 ) );

  updateHeaders( o );
  updateHeaderSizes( o );
  updateHeaderSpace( o );

  updateGeometries();
}

int QtxTable::verticalSpan( const Orientation o, const int idx, const int section ) const
{
  Header* hdr = (Header*)header( o, idx );
  if ( !hdr )
    return -1;

  return hdr->verticalSpan( section );
}

int QtxTable::horizontalSpan( const Orientation o, const int idx, const int section ) const
{
  Header* hdr = (Header*)header( o, idx );
  if ( !hdr )
    return -1;

  return hdr->horizontalSpan( section );
}

void QtxTable::setVerticalSpan( const Orientation o, const int idx, const int section, const int span )
{
  Header* hdr = (Header*)header( o, idx );
  if ( hdr )
    hdr->setVerticalSpan( section, span );
}

void QtxTable::setHorizontalSpan( const Orientation o, const int idx, const int section, const int span )
{
  Header* hdr = (Header*)header( o, idx );
  if ( hdr )
    hdr->setHorizontalSpan( section, span );
}

QColor QtxTable::headerForegroundColor( const Orientation o, const int idx, const int section ) const
{
  Header* hdr = (Header*)header( o, idx );
  if ( !hdr )
    return QColor();

  return hdr->foregroundColor( section );
}

QColor QtxTable::headerBackgroundColor( const Orientation o, const int idx, const int section ) const
{
  Header* hdr = (Header*)header( o, idx );
  if ( !hdr )
    return QColor();

  return hdr->backgroundColor( section );
}

void QtxTable::setHeaderForegroundColor( const Orientation o, const int idx, const int section, const QColor& c )
{
  Header* hdr = (Header*)header( o, idx );
  if ( hdr )
    hdr->setForegroundColor( section, c );
}

void QtxTable::setHeaderBackgroundColor( const Orientation o, const int idx, const int section, const QColor& c )
{
  Header* hdr = (Header*)header( o, idx );
  if ( hdr )
    hdr->setBackgroundColor( section, c );
}

QFont QtxTable::cellFont( const int row, const int col ) const
{
  QFont res = font();
  QVariant val = cellProperty( row, col, Font );
  if ( val.canCast( QVariant::Font ) )
    res = val.toFont();
  return res;
}

QColor QtxTable::cellForegroundColor( const int row, const int col ) const
{
  QColor res;
  QVariant val = cellProperty( row, col, FgColor );
  if ( val.canCast( QVariant::Color ) )
    res = val.toColor();
  return res;
}

QColor QtxTable::cellBackgroundColor( const int row, const int col ) const
{
  QColor res;
  QVariant val = cellProperty( row, col, BgColor );
  if ( val.canCast( QVariant::Color ) )
    res = val.toColor();
  return res;
}

void QtxTable::setCellFont( const int row, const int col, QFont& f )
{
  setCellProperty( row, col, Font, f );
}

void QtxTable::setCellForegroundColor( const int row, const int col, const QColor& c )
{
  setCellProperty( row, col, FgColor, c );
}

void QtxTable::setCellBackgroundColor( const int row, const int col, const QColor& c )
{
  setCellProperty( row, col, BgColor, c );
}

void QtxTable::paintCell( QPainter* p, int row, int col, const QRect& cr,
                          bool selected, const QColorGroup& cg )
{
  QColor fg = cellForegroundColor( row, col );
  QColor bg = cellBackgroundColor( row, col );

  QColorGroup cGroup = cg;
  if ( fg.isValid() )
    cGroup.setColor( QColorGroup::Text, fg );
  if ( bg.isValid() )
    cGroup.setColor( QColorGroup::Base, bg );

  p->save();
  p->setFont( cellFont( row, col ) );

  QTable::paintCell( p, row, col, cr, selected, cGroup );

  p->restore();
}

void QtxTable::clear( const bool withHeaders )
{
  for ( int r = 0; r < numRows(); r++ )
  {
    for ( int c = 0; c < numCols(); c++ )
    {
      clearCell( r, c );
      clearCellProperties( r, c );
    }
  }
  if ( !withHeaders )
    return;

  QHeader* hh = horizontalHeader();
  for ( int hi = 0; hi < hh->count(); hi++ )
    hh->setLabel( hi, QString( "" ) );
  QHeader* vh = verticalHeader();
  for ( int vi = 0; vi < vh->count(); vi++ )
    vh->setLabel( vi, QString( "" ) );

  for ( int h = 0; h < (int)myHorHeaders.size(); h++ )
    ((Header*)myHorHeaders.at( h ))->clear();
  for ( int v = 0; v < (int)myVerHeaders.size(); v++ )
    ((Header*)myVerHeaders.at( v ))->clear();
}

/*!
  SLOT: called on scroll
*/
void QtxTable::onScrollBarMoved( int )
{
  updateHeaderEditor();
}

/*!
  SLOT: called on header size changing
*/
void QtxTable::onHeaderSizeChange( int section, int oldSize, int newSize )
{
  const QHeader* header = ::qt_cast<const QHeader*>( sender() );
  if ( !header )
    return;

  HeaderVector* vec = headerVector( header->orientation() );
  for ( int i = 0; i < (int)vec->size(); i++ )
  {
    QHeader* hdr = vec->at( i );
    hdr->resizeSection( section, newSize );
  }

  if ( header == myEditedHeader )
    updateHeaderEditor();
}

/*!
  Custom hide event handler
*/
void QtxTable::hideEvent( QHideEvent* e )
{
  endHeaderEdit();

  QTable::hideEvent( e );
}

/*!
  Custom resize event handler
*/
void QtxTable::resizeEvent( QResizeEvent* e )
{
  QTable::resizeEvent( e );

  updateHeaderEditor();

  updateGeometries();
}

/*!
  Returns the specified property of the cell.
  \param row - row number of cell
  \param col - column number of cell
  \param name - property name
*/
QVariant QtxTable::cellProperty( const int row, const int col, const int name ) const
{
  QVariant res;

  if ( myCellProps.contains( row ) )
  {
    const PropsMap& map = myCellProps[row];
    if ( map.contains( col ) )
    {
      const Properties& props = map[col];
      if ( props.contains( name ) )
        res = props[name];
    }
  }
  return res;
}

/*!
  Sets the specified property value of the cell.
  \param row - row number of cell
  \param col - column number of cell
  \param name - property name
  \param val - setted property value
*/
void QtxTable::setCellProperty( const int row, const int col, const int name, const QVariant& val )
{
  if ( row < 0 || row >= numRows() || col < 0 || col >= numCols() )
    return;

  if ( !myCellProps.contains( row ) )
    myCellProps.insert( row, PropsMap() );

  PropsMap& map = myCellProps[row];
  if ( !map.contains( col ) )
    map.insert( col, Properties() );

  Properties& props = map[col];
  props.insert( name, val );
}

/*!
  Removes the specified property value of the cell.
  \param row - row number of cell
  \param col - column number of cell
  \param name - property name
*/
void QtxTable::unsetCellProperty( const int row, const int col, const int name )
{
  if ( !myCellProps.contains( row ) )
    return;

  PropsMap& map = myCellProps[row];
  if ( !map.contains( col ) )
    return;

  map[col].remove( name );

  if ( map[col].isEmpty() )
    map.remove( col );

  if ( myCellProps[row].isEmpty() )
    myCellProps.remove( row );
}

/*!
  Removes the all properties of the cell.
  \param row - row number of cell
  \param col - column number of cell
*/
void QtxTable::clearCellProperties( const int row, const int col )
{
  if ( !myCellProps.contains( row ) )
    return;

  myCellProps[row].remove( col );
  if ( myCellProps[row].isEmpty() )
    myCellProps.remove( row );
}

/*!
  Starts edition of header
  \param o - header orientation
  \param sec - column/row
*/
bool QtxTable::beginHeaderEdit( Orientation o, const int section, const int idx )
{
  QHeader* hdr = 0;
  if ( idx < 0 )
    hdr = header( o );
  if ( !hdr->isVisibleTo( this ) )
    hdr = header( o, idx );

  return beginHeaderEdit( hdr, section );
}

bool QtxTable::beginHeaderEdit( QHeader* hdr, const int s )
{
  if ( !hdr || !myHeaderEditable.contains( hdr ) || !myHeaderEditable[hdr] || !hdr->isVisibleTo( this ) )
    return false;

  endHeaderEdit();

  int section = s;
  QRect r = headerSectionRect( hdr, s, &section );
  if ( !r.isValid() )
    return false;

  if ( hdr->orientation() == Horizontal )
    r.setLeft( QMAX( r.left(), leftMargin() ) );
  else
    r.setTop( QMAX( r.top(), topMargin() ) );

  myHeaderEditor = createHeaderEditor( hdr, section );
  if ( !myHeaderEditor )
    return false;

  myEditedHeader = hdr;
  myEditedSection = section;

  myHeaderEditor->reparent( this, QPoint( 0, 0 ), false );

  updateHeaderEditor();

  myHeaderEditor->show();

  myHeaderEditor->setActiveWindow();
  myHeaderEditor->setFocus();

  myHeaderEditor->installEventFilter( this );

  return true;
}

/*!
  Finishes edition of header
  \param accept - whether new value must be accepted
*/
void QtxTable::endHeaderEdit( const bool accept )
{
  if ( !isHeaderEditing() )
    return;

  QString oldTxt = myEditedHeader ? myEditedHeader->label( myEditedSection ) : QString::null;

  if ( accept && myEditedHeader )
    setHeaderContentFromEditor( myEditedHeader, myEditedSection, myHeaderEditor );

  QString newTxt = myEditedHeader ? myEditedHeader->label( myEditedSection ) : QString::null;

  int sec = myEditedSection;
  QHeader* hdr = myEditedHeader;

  myEditedHeader = 0;
  myEditedSection = -1;

  myHeaderEditor->hide();
  myHeaderEditor->deleteLater();
  myHeaderEditor->removeEventFilter( this );
  myHeaderEditor = 0;

  if ( oldTxt != newTxt )
  {
    emit headerEdited( hdr, sec );
    emit headerEdited( hdr == horizontalHeader() ? Horizontal : Vertical, sec );
  }
}

/*!
  \return true if header is being edited
*/
bool QtxTable::isHeaderEditing() const
{
  return myHeaderEditor && myEditedHeader && myEditedSection != -1;
}

/*!
  Creates and \return header editor
  \param hdr - header
  \param sec - column/row
  \param init - init editor with value
*/
QWidget* QtxTable::createHeaderEditor( QHeader* hdr, const int sec, const bool init )
{
  QString lab = hdr ? hdr->label( sec ) : QString::null;

  QWidget* wid = 0;
  if ( !QStyleSheet::mightBeRichText( lab ) && !lab.contains( "\n" ) )
  {
    QLineEdit* le = new QLineEdit( 0 );
    if ( init )
      le->setText( lab );
    wid = le;
  }
  else
  {
    QTextEdit* te = new QTextEdit( 0 );
    te->setVScrollBarMode( QTextEdit::AlwaysOff );
    te->setHScrollBarMode( QTextEdit::AlwaysOff );
    QStyleSheet* sheet = new QStyleSheet( te );
    QStyleSheetItem* i = sheet->item( "p" );
    if ( i )
      i->setMargin( QStyleSheetItem::MarginAll, 0 );
    te->setStyleSheet( sheet );

    if ( init )
      te->setText( lab );
    wid = te;
  }

  return wid;
}

/*!
  Initialize editor with value
  \param hdr - header
  \param sec - column/row
  \param editor - editor
*/
void QtxTable::setHeaderContentFromEditor( QHeader* hdr, const int sec, QWidget* editor )
{
  if ( !hdr || !editor )
    return;

  QLineEdit* le = ::qt_cast<QLineEdit*>( editor );
  if ( le )
    hdr->setLabel( sec, le->text() );
  else
  {
    QTextEdit* te = ::qt_cast<QTextEdit*>( editor );
    if ( te )
      hdr->setLabel( sec, te->text().remove( "\n" ) );
  }
}

/*!
  \return header
  \param o - orientation
*/
QHeader* QtxTable::header( Orientation o ) const
{
  return o == Horizontal ? horizontalHeader() : verticalHeader();
}

/*!
  Starts edition of header
  \param o - header orientation
  \param p - point
*/
bool QtxTable::beginHeaderEdit( QHeader* hdr, const QPoint& p )
{
  if ( !hdr )
    return false;

  int pos = hdr->orientation() == Horizontal ? p.x() : p.y();
  int sec = hdr->sectionAt( hdr->offset() + pos );
  if ( sec < 0 )
    return false;

  bool ok = false;
  QPoint pnt = mapFromGlobal( hdr->mapToGlobal( p ) );
  int cur = hdr->orientation() == Horizontal ? pnt.x() : pnt.y();

  int bid = 0;
  QRect sRect = headerSectionRect( hdr, sec, &bid );
  if ( sRect.isValid() )
  {
    int m = 4;
    int from = hdr->orientation() == Horizontal ? sRect.left() : sRect.top();
    int to = hdr->orientation() == Horizontal ? sRect.right() - m : sRect.bottom() - m;
    if ( sec > 0 )
      from += m;

    ok = from <= cur && cur <= to;
  }

  if ( ok )
    beginHeaderEdit( hdr, sec );

  return ok;
}

/*!
  \return rectangle of header section
  \param hdr - header
  \param sec - column/row
*/
QRect QtxTable::headerSectionRect( QHeader* hdr, const int sec, int* main ) const
{
  QRect r( -1, -1, -1, -1 );

  if ( !hdr )
    return r;

  r = hdr->sectionRect( sec );
  if ( hdr != verticalHeader() && hdr != horizontalHeader() )
    r = ((Header*)hdr)->indexRect( hdr->mapToIndex( sec ), main );

  if ( r.isValid() )
    r = QRect( mapFromGlobal( hdr->mapToGlobal( r.topLeft() ) ), r.size() );

  return r;
}

/*!
  Updates header editor
*/
void QtxTable::updateHeaderEditor()
{
  if ( !myHeaderEditor || !myEditedHeader || myEditedSection < 0 )
    return;

  QRect r = headerSectionRect( myEditedHeader, myEditedSection );
  if ( !r.isValid() )
    return;

  if ( myEditedHeader->orientation() == Horizontal )
  {
    r.setLeft( QMAX( r.left(), leftMargin() ) );
    r.setRight( QMIN( r.right(), width() - rightMargin() - 2 ) );
  }
  else
  {
    r.setTop( QMAX( r.top(), topMargin() ) );
    r.setBottom( QMIN( r.bottom(), height() - bottomMargin() - 2 ) );
  }

  myHeaderEditor->resize( r.size() );
  myHeaderEditor->move( r.topLeft() );
}

/*!
  Remove selected rows or columns if any of cell selected in it
*/
void QtxTable::removeSelected( const bool row )
{
  QValueList<int> idsList;
  QMap<int, int> idMap;
  int i = 0;
  for ( int nb = numSelections(); i < nb; i++ )
  {
    QTableSelection sel = selection( i );
    int minId = row ? sel.topRow() : sel.leftCol();
    int maxId = row ? sel.bottomRow() : sel.rightCol();
    for ( ; minId <= maxId; minId++ )
      if ( !idMap.contains( minId ) )
      {
        idMap[ minId ] = 1;
        idsList.append( minId );
      }
  }
  if ( idMap.isEmpty() )
    return;

  qHeapSort( idsList );
  QMemArray<int> idsArr( idsList.count() );
  QValueList<int>::const_iterator it = idsList.begin();
  QValueList<int>::const_iterator itEnd = idsList.end();
  for ( i = 0; it != itEnd; ++it, ++i )
    idsArr[ i ] = *it;

  if ( row )
    removeRows( idsArr );
  else
    removeColumns( idsArr );
}

QtxTable::HeaderVector* QtxTable::headerVector( const Orientation o ) const
{
  return o == Horizontal ? (HeaderVector*)&myHorHeaders : (HeaderVector*)&myVerHeaders;
}

QtxStyleWrap* QtxTable::styleWrapper()
{
  if ( !myStyleWrapper )
  {
    myStyleWrapper = new QtxStyleWrap( &style() );
    new StyleItem( myStyleWrapper );
  }
  return myStyleWrapper;
}

void QtxTable::updateHeaders( const Orientation o )
{
  HeaderVector& vec = o == Vertical ? myVerHeaders : myHorHeaders;
  QScrollBar* sb = o == Vertical ? verticalScrollBar() : horizontalScrollBar();
  QHeader* main = o == Vertical ? verticalHeader() : horizontalHeader();

  main->setShown( !vec.size() );

  for ( uint i = 0; i < vec.size(); i++ )
  {
    if ( vec.at( i ) )
      continue;

    QHeader* hdr = new Header( o == Horizontal ? numCols() : numRows(), this );
    vec.insert( i, hdr );
    hdr->setStyle( styleWrapper() );
    hdr->setMovingEnabled( false );
    hdr->setOrientation( o );
    hdr->installEventFilter( this );
    hdr->show();

    connect( sb, SIGNAL( valueChanged( int ) ), hdr, SLOT( setOffset( int ) ) );
    if ( o == Horizontal )
      connect( hdr, SIGNAL( sectionHandleDoubleClicked( int ) ), this, SLOT( adjustColumn( int ) ) );
    else
      connect( hdr, SIGNAL( sectionHandleDoubleClicked( int ) ), this, SLOT( adjustRow( int ) ) );
  }

  if ( o == Horizontal )
  {
    for ( uint j = 0; j < vec.size(); j++ )
    {
      QHeader* hdr = vec.at( j );
      bool upd = hdr->isUpdatesEnabled();
      hdr->setUpdatesEnabled( false );
      for ( int s = 0; s < (int)hdr->count(); s++ )
        hdr->setLabel( s, QString( "" ) );
      hdr->setUpdatesEnabled( upd );
    }
  }

  // slot onScrollBarMoved should be last connection.
  // QHeader::setOffset() should be invoked before it for every header.
  // Otherwise header editor geometry will not properly updated.
  disconnect( verticalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( onScrollBarMoved( int ) ) );
  disconnect( horizontalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( onScrollBarMoved( int ) ) );
  connect( verticalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( onScrollBarMoved( int ) ) );
  connect( horizontalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( onScrollBarMoved( int ) ) );
}

void QtxTable::updateHeaderSpace( const Orientation o )
{
  HeaderVector& vec = o == Vertical ? myVerHeaders : myHorHeaders;

  int size = 0;
  for ( uint i = 0; i < vec.size(); i++ )
  {
    QHeader* hdr = vec.at( i );
    int sz = o == Vertical ? hdr->width() : hdr->height();
    if ( o == Vertical )
      hdr->move( size, 0 );
    else
      hdr->move( 0, size );
    size += sz;
  }

  if ( vec.size() == 0 )
    size = o == Vertical ? verticalHeader()->width() : horizontalHeader()->height();

  if ( o == Vertical )
    setLeftMargin( size );
  else
    setTopMargin( size );
}

void QtxTable::updateHeaderSizes( const Orientation o )
{
  HeaderVector& vec = o == Vertical ? myVerHeaders : myHorHeaders;

  for ( uint j = 0; j < vec.count(); j++ )
  {
    QHeader* hdr = vec.at( j );
    for ( int s = 0; s < (int)hdr->count(); s++ )
      hdr->resizeSection( s, o == Vertical ? rowHeight( s ) : columnWidth( s ) );
    if ( o == Vertical )
      hdr->resize( hdr->sizeHint().width(), hdr->height() );
    else
      hdr->resize( hdr->width(), hdr->sizeHint().height() );
  }
}

void QtxTable::updateGeometries()
{
  QSize sz = horizontalHeader()->size();
  int size = frameWidth();
  for ( int h = 0; h < (int)myHorHeaders.size(); h++ )
  {
    QHeader* hdr = myHorHeaders.at( h );
    hdr->setGeometry( frameWidth() + leftMargin(), size, sz.width(), hdr->height() );
    size += hdr->height();
  }

  sz = verticalHeader()->size();
  size = frameWidth();
  for ( int v = 0; v < (int)myVerHeaders.size(); v++ )
  {
    QHeader* hdr = myVerHeaders.at( v );
    hdr->setGeometry( size, frameWidth() + topMargin(), hdr->width(), sz.height() );
    size += hdr->width();
  }
}

void QtxTable::updateHeaderGeometries( const Orientation o )
{
  if ( !isUpdatesEnabled() )
    return;

  updateHeaderSizes( o );
  updateHeaderSpace( o );
  updateGeometries();
}

void QtxTable::updateSelectAllButton()
{
  if ( !mySelectAll )
    return;

  mySelectAll->setShown( selectionMode() == Multi || selectionMode() == MultiRow );
  if ( mySelectAll->isVisibleTo( this  ) )
    mySelectAll->setGeometry( frameWidth(), frameWidth(), leftMargin(), topMargin() );
}

int QtxTable::headerSpanedSection( const Orientation o, const int headIdx, const int index )
{
  Header* hdr = (Header*)(headIdx < 0 ? header( o ) : header( o, headIdx ));
  if ( hdr )
    return hdr->spanedSection( index );
  else
    return index;
}

#endif
