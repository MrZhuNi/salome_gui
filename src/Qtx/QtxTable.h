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
// File:      QtxTable.h
// Author:    Sergey TELKOV

#ifndef QTX_TABLE_H
#define QTX_TABLE_H

#include "Qtx.h"

#include <qmap.h>
#include <qtable.h>
#include <qvariant.h>
#include <qptrvector.h>

#ifndef QT_NO_TABLE

class QHeader;
class QToolButton;
class QtxStyleWrap;

#ifdef WIN32
#pragma warning( disable : 4251 )
#endif

class QTX_EXPORT QtxTable : public QTable
{
  Q_OBJECT

  Q_PROPERTY( bool selectAllEnabled READ isSelectAllEnabled WRITE setSelectAllEnabled )

  class Header;
  class StyleItem;

protected:
  enum { FgColor, BgColor, Font, User };

public:
  QtxTable( QWidget* = 0, const char* = 0 );
  QtxTable( int, int, QWidget* = 0, const char* = 0 );
  virtual ~QtxTable();

  bool             headerEditable( Orientation, const int = -1 ) const;

  bool             editHeader( Orientation, const int );
  void             endEditHeader( const bool = true );

  virtual void     setSelectionMode( SelectionMode );

  void             removeSelected( const bool row = true );

  virtual bool     eventFilter( QObject*, QEvent* );

  virtual void     setNumRows( int );
  virtual void     setNumCols( int );

  bool             isSelectAllEnabled() const;
  virtual void     setSelectAllEnabled( const bool );

  QHeader*         header( const Orientation, const int ) const;
  int              numHeaders( const Orientation ) const;
  void             setNumHeaders( const Orientation, const int );

  int              verticalSpan( const Orientation, const int, const int ) const;
  int              horizontalSpan( const Orientation, const int, const int ) const;
  void             setVerticalSpan( const Orientation, const int, const int, const int );
  void             setHorizontalSpan( const Orientation, const int, const int, const int );

  int              headerSpanedSection( const Orientation, const int headIdx, const int index );

  QColor           headerForegroundColor( const Orientation, const int, const int ) const;
  QColor           headerBackgroundColor( const Orientation, const int, const int ) const;
  void             setHeaderForegroundColor( const Orientation, const int, const int, const QColor& );
  void             setHeaderBackgroundColor( const Orientation, const int, const int, const QColor& );

  QFont            cellFont( const int, const int ) const;
  QColor           cellForegroundColor( const int, const int ) const;
  QColor           cellBackgroundColor( const int, const int ) const;

  void             setCellFont( const int, const int, QFont& );
  void             setCellForegroundColor( const int, const int, const QColor& );
  void             setCellBackgroundColor( const int, const int, const QColor& );

  virtual void     paintCell( QPainter*, int, int, const QRect&, bool, const QColorGroup& );

  virtual void     clear( const bool = true );

signals:
  void             headerEdited( QHeader*, int );
  void             headerEdited( Orientation, int );

public slots:
  virtual void     selectAll();
  virtual void     setTopMargin( int );
  virtual void     setLeftMargin( int );
  void             setHeadersEditable( Orientation, bool );
  virtual void     setHeaderEditable( Orientation, bool, const int = -1 );

  virtual void     insertRows( int, int = 1 );
  virtual void     insertColumns( int, int = 1 );
  virtual void     removeRow( int );
  virtual void     removeRows( const QMemArray<int>& );
  virtual void     removeColumn( int );
  virtual void     removeColumns( const QMemArray<int>& );

  virtual void     setUpdatesEnabled( bool enable );

  virtual void     adjustRow( int );
  virtual void     adjustColumn( int );

protected slots:
  virtual void     rowHeightChanged( int );
  virtual void     columnWidthChanged( int );

private slots:
  void             onScrollBarMoved( int );
  void             onHeaderSizeChange( int, int, int );

protected:
  virtual void     hideEvent( QHideEvent* );
  virtual void     resizeEvent( QResizeEvent* );

  virtual bool     beginHeaderEdit( QHeader*, const int );
  bool             beginHeaderEdit( Orientation, const int, const int = -1 );
  virtual void     endHeaderEdit( const bool = true );
  bool             isHeaderEditing() const;
  virtual QWidget* createHeaderEditor( QHeader*, const int, const bool = true );
  virtual void     setHeaderContentFromEditor( QHeader*, const int, QWidget* );

  QHeader*         header( Orientation o ) const;
  virtual QRect    headerSectionRect( QHeader*, const int, int* = 0 ) const;

  QVariant         cellProperty( const int, const int, const int ) const;
  void             setCellProperty( const int, const int, const int, const QVariant& );
  void             unsetCellProperty( const int, const int, const int );
  void             clearCellProperties( const int, const int );

private:
  typedef QMap<int, QVariant>   Properties;
  typedef QMap<int, Properties> PropsMap;
  typedef QMap<int, PropsMap>   CellMap;
  typedef QMap<QHeader*, bool>  HeaderState;
  typedef QPtrVector<QHeader>   HeaderVector;

private:
  void             updateHeaderEditor();
  bool             beginHeaderEdit( QHeader*, const QPoint& );

  QtxStyleWrap*    styleWrapper();
  HeaderVector*    headerVector( const Orientation ) const;

  void             updateGeometries();
  void             updateSelectAllButton();

  void             updateHeaders( const Orientation );
  void             updateHeaderSizes( const Orientation );
  void             updateHeaderSpace( const Orientation );
  void             updateHeaderGeometries( const Orientation );

private:
  CellMap          myCellProps;
  HeaderVector     myVerHeaders;
  HeaderVector     myHorHeaders;
  QtxStyleWrap*    myStyleWrapper;

  QWidget*         myHeaderEditor;
  QHeader*         myEditedHeader;
  int              myEditedSection;
  HeaderState      myHeaderEditable;

  QToolButton*     mySelectAll;

  friend class QtxTable::Header;
};

#ifdef WIN32
#pragma warning( default: 4251 )
#endif

#endif

#endif
