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

#include <QMap>
#include <QVector>
#include <QTableWidget>
#include <QHeaderView>
#include <QIcon>
//#include <qvariant.h>
//#include <qptrvector.h>

#ifndef QT_NO_TABLE

class QHeaderView;
class QToolButton;
class QVariant;
class QtxStyleWrap;

#ifdef WIN32
#pragma warning( disable : 4251 )
#endif

class QTX_EXPORT QtxTable : public QTableWidget
{
  Q_OBJECT

  //Q_PROPERTY( bool selectAllEnabled READ isSelectAllEnabled WRITE setSelectAllEnabled )

  class Header;
  class StyleItem;

public:
  QtxTable( QWidget* = 0, const char* = 0 );
  QtxTable( int, int, QWidget* = 0, const char* = 0 );
  virtual ~QtxTable();

  bool             headerEditable( Qt::Orientation, const int = -1 ) const;

  bool             editHeader( Qt::Orientation, const int );
  void             endEditHeader( const bool = true );

  virtual void     setSelectionMode( SelectionMode );

  void             removeSelected( const bool row = true );

  virtual bool     eventFilter( QObject*, QEvent* );

  //virtual void     setNumRows( int );
  //virtual void     setNumCols( int );

  bool             isSelectAllEnabled() const;
  virtual void     setSelectAllEnabled( const bool );

  QHeaderView*     header( const Qt::Orientation, const int ) const;
  int              numHeaders( const Qt::Orientation ) const;
  void             setNumHeaders( const Qt::Orientation, const int );

  QVariant         headerData( const Qt::Orientation, const int, const int = Qt::DisplayRole ) const;
  QFont            headerFont( const Qt::Orientation, const int ) const;
  QColor           headerForeground( const Qt::Orientation, const int ) const;
  QColor           headerBackground( const Qt::Orientation, const int ) const;
  QIcon            headerIcon( const Qt::Orientation, const int ) const;

  void             setHeaderData( const Qt::Orientation, const int, const QVariant&,
                                  const int = Qt::DisplayRole );
  void             setHeaderFont( const Qt::Orientation, const int, const QFont& );
  void             setHeaderForeground( const Qt::Orientation, const int, const QColor& );
  void             setHeaderBackground( const Qt::Orientation, const int, const QColor& );
  void             setHeaderIcon( const Qt::Orientation, const int, const QIcon& );

  QVariant         cellData( const int, const int ) const;
  QFont            cellFont( const int, const int ) const;
  QColor           cellForeground( const int, const int ) const;
  QColor           cellBackground( const int, const int ) const;
  QIcon            cellIcon( const int, const int ) const;

  void             setCellData( const int, const int, const QVariant& );
  void             setCellFont( const int, const int, const QFont& );
  void             setCellForeground( const int, const int, const QColor& );
  void             setCellBackground( const int, const int, const QColor& );
  void             setCellIcont( const int, const int, QIcon& );

  //virtual void     paintCell( QPainter*, int, int, const QRect&, bool, const QColorGroup& );

  virtual void     clear( const bool = true );

signals:
  void             headerEdited( QHeaderView*, int );
  void             headerEdited( Qt::Orientation, int );

public slots:
  virtual void     selectAll();
  virtual void     setTopMargin( int );
  virtual void     setLeftMargin( int );
  void             setHeadersEditable( Qt::Orientation, bool );
  virtual void     setHeaderEditable( Qt::Orientation, bool, const int = -1 );

  //virtual void     insertRows( int, int = 1 );
  //virtual void     insertColumns( int, int = 1 );
  //virtual void     removeRow( int );
  //virtual void     removeRows( const QVector<int>& );
  //virtual void     removeColumn( int );
  //virtual void     removeColumns( const QVector<int>& );

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

  virtual bool     beginHeaderEdit( QHeaderView*, const int );
  bool             beginHeaderEdit( Qt::Orientation, const int, const int = -1 );
  virtual void     endHeaderEdit( const bool = true );
  bool             isHeaderEditing() const;
  virtual QWidget* createHeaderEditor( QHeaderView*, const int, const bool = true );
  virtual void     setHeaderContentFromEditor( QHeaderView*, const int, QWidget* );

  QHeaderView*     header( Qt::Orientation o ) const;
  virtual QRect    headerSectionRect( QHeaderView*, const int, int* = 0 ) const;

  QTableWidgetItem* getItem( const int, const int, const bool = true );

private:
  typedef QMap<QHeaderView*, bool>  HeaderState;
  typedef QVector<QHeaderView*> HeaderVector;

private:
  void             updateHeaderEditor();
  bool             beginHeaderEdit( QHeaderView*, const QPoint& );

  QtxStyleWrap*    styleWrapper();
  HeaderVector*    headerVector( const Qt::Orientation ) const;

  //void             updateGeometries();
  void             updateSelectAllButton();

  void             updateHeaders( const Qt::Orientation );
  void             updateHeaderSizes( const Qt::Orientation );
  void             updateHeaderSpace( const Qt::Orientation );
  void             updateHeaderGeometries( const Qt::Orientation );

private:
  /*
  HeaderVector     myVerHeaders;
  HeaderVector     myHorHeaders;
  QtxStyleWrap*    myStyleWrapper;

  QWidget*         myHeaderEditor;
  QHeaderView*     myEditedHeader;
  int              myEditedSection;
  HeaderState      myHeaderEditable;

  QToolButton*     mySelectAll;
  */

  friend class QtxTable::Header;
};

#ifdef WIN32
#pragma warning( default: 4251 )
#endif

#endif

#endif
