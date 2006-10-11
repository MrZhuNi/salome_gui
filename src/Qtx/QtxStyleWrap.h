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
// File:      QtxStyleWrap.h
// Author:    Sergey TELKOV

#ifndef QTXSTYLEWRAP_H
#define QTXSTYLEWRAP_H

#include "Qtx.h"

#include <qstyle.h>
#include <qptrlist.h>
#include <qguardedptr.h>

class QtxStyleWrapItem;

#ifdef WIN32
#pragma warning( disable : 4251 )
#endif

class QTX_EXPORT QtxStyleWrap : public QStyle
{
  Q_OBJECT

public:
  QtxStyleWrap( QStyle* );
  QtxStyleWrap( QWidget* );
  virtual ~QtxStyleWrap();

  QStyle*            style() const;

  void               addItem( QtxStyleWrapItem* );
  void               removeItem( QtxStyleWrapItem* );

  virtual void       polish( QWidget* );
  virtual void       unPolish( QWidget* );
  virtual void       polish( QApplication* );
  virtual void       unPolish( QApplication* );
  virtual void       polish( QPalette& );
  virtual void       polishPopupMenu( QPopupMenu* );

  virtual QRect      itemRect( QPainter*, const QRect&, int, bool,
                               const QPixmap*, const QString&, int = -1 ) const;
  virtual void       drawItem( QPainter*, const QRect&, int, const QColorGroup&,
                               bool, const QPixmap*, const QString&, int = -1, const QColor* = 0 ) const;
  virtual void       drawPrimitive( PrimitiveElement, QPainter*, const QRect&, const QColorGroup&,
                                    SFlags = Style_Default, const QStyleOption& = QStyleOption::Default ) const;
  virtual void       drawControl( ControlElement, QPainter*, const QWidget*, const QRect&, const QColorGroup&,
                                  SFlags = Style_Default, const QStyleOption& = QStyleOption::Default ) const;
  virtual void       drawControlMask( ControlElement, QPainter*, const QWidget*, const QRect&,
                                      const QStyleOption& = QStyleOption::Default ) const;
  virtual void       drawComplexControl( ComplexControl, QPainter*, const QWidget*, const QRect&, const QColorGroup&,
                                         SFlags = Style_Default, SCFlags = SC_All, SCFlags = SC_None,
                                         const QStyleOption& = QStyleOption::Default ) const;
  virtual void       drawComplexControlMask( ComplexControl, QPainter*, const QWidget*, const QRect&,
                                             const QStyleOption& = QStyleOption::Default ) const;
  virtual QRect      subRect( SubRect, const QWidget* ) const;
  virtual QRect      querySubControlMetrics( ComplexControl, const QWidget*, SubControl,
                                        const QStyleOption& = QStyleOption::Default ) const;
  virtual SubControl querySubControl( ComplexControl, const QWidget*, const QPoint&,
                                      const QStyleOption& = QStyleOption::Default ) const;
  virtual int        pixelMetric( PixelMetric, const QWidget* = 0 ) const;
  virtual int        styleHint( StyleHint, const QWidget* = 0,
                                const QStyleOption& = QStyleOption::Default, QStyleHintReturn* = 0 ) const;
  virtual QPixmap    stylePixmap( StylePixmap, const QWidget* = 0,
                                  const QStyleOption& = QStyleOption::Default ) const;
  virtual QSize      sizeFromContents( ContentsType, const QWidget*, const QSize&,
                                       const QStyleOption& = QStyleOption::Default ) const;

private:
  typedef QGuardedPtr<QStyle>                StylePtr;
  typedef QPtrList<QtxStyleWrapItem>         ItemList;
  typedef QPtrListIterator<QtxStyleWrapItem> ItemListIterator;

private:
  StylePtr           myStyle;
  ItemList           myItems;
};

class QTX_EXPORT QtxStyleWrapItem : public Qtx
{
public:
  QtxStyleWrapItem( QtxStyleWrap* );
  ~QtxStyleWrapItem();

  QStyle*            style() const;
  QtxStyleWrap*      styleWrap() const;

  virtual bool       polish( QWidget* );
  virtual bool       unPolish( QWidget* );
  virtual bool       polish( QApplication* );
  virtual bool       unPolish( QApplication* );
  virtual bool       polish( QPalette& );
  virtual bool       polishPopupMenu( QPopupMenu* );

  virtual bool       itemRect( QPainter*, const QRect&, int, bool,
                               const QPixmap*, const QString&, int, QRect& ) const;
  virtual bool       drawItem( QPainter*, const QRect&, int, const QColorGroup&,
                               bool, const QPixmap*, const QString&, int, const QColor* ) const;
  virtual bool       drawPrimitive( QStyle::PrimitiveElement, QPainter*, const QRect&,
                                    const QColorGroup&, QStyle::SFlags, const QStyleOption& ) const;
  virtual bool       drawControl( QStyle::ControlElement, QPainter*, const QWidget*, const QRect&,
                                  const QColorGroup&, QStyle::SFlags, const QStyleOption& ) const;
  virtual bool       drawControlMask( QStyle::ControlElement, QPainter*, const QWidget*,
                                      const QRect&, const QStyleOption& ) const;
  virtual bool       drawComplexControl( QStyle::ComplexControl, QPainter*, const QWidget*,
                                         const QRect&, const QColorGroup&, QStyle::SFlags,
                                         QStyle::SCFlags, QStyle::SCFlags, const QStyleOption& ) const;
  virtual bool       drawComplexControlMask( QStyle::ComplexControl, QPainter*, const QWidget*,
                                             const QRect&, const QStyleOption& ) const;
  virtual bool       subRect( QStyle::SubRect, const QWidget*, QRect& ) const;
  virtual bool       querySubControlMetrics( QStyle::ComplexControl, const QWidget*,
                                             QStyle::SubControl, const QStyleOption&, QRect& ) const;
  virtual bool       querySubControl( QStyle::ComplexControl, const QWidget*, const QPoint&,
                                      const QStyleOption&, QStyle::SubControl& ) const;
  virtual bool       pixelMetric( QStyle::PixelMetric, const QWidget*, int& ) const;
  virtual bool       styleHint( QStyle::StyleHint, const QWidget*,
                                const QStyleOption&, QStyleHintReturn*, int& ) const;
  virtual bool       stylePixmap( QStyle::StylePixmap, const QWidget*,
                                  const QStyleOption&, QPixmap& ) const;
  virtual bool       sizeFromContents( QStyle::ContentsType, const QWidget*,
                                       const QSize&, const QStyleOption&, QSize& ) const;

private:
  QtxStyleWrap*      myWrap;

  friend class QtxStyleWrap;
};

#ifdef WIN32
#pragma warning( default : 4251 )
#endif

#endif
