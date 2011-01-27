//  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : Plot2d_HistogramItem.h
//  Author : Natalia ERMOLAEVA, Open CASCADE S.A.S. (natalia.donis@opencascade.com)

#ifndef PLOT2D_PLOTITEMS_H
#define PLOT2D_PLOTITEMS_H

#include "Plot2d.h"

#include <qwt_legend_item.h>
#include <qwt_plot_item.h>
#include <qwt_plot_curve.h>
#include <qwt_interval_data.h>
#include <qwt_plot.h>

class PLOT2D_EXPORT Plot2d_QwtLegendItem : public QwtLegendItem
{
public:
  enum YAxisIdentifierMode { IM_None = 0, IM_Left, IM_Right };

public:
  Plot2d_QwtLegendItem( QWidget* = 0 );
  virtual ~Plot2d_QwtLegendItem();

public:
  void             setYAxisIdentifierMode( const int );

protected:
  virtual void     drawIdentifier( QPainter*, const QRect& ) const;

private:
  int              myYAxisIdentifierMode;
  QPixmap          myYAxisLeftIcon;
  QPixmap          myYAxisRightIcon;
  int              mySpacingCollapsed;
  int              mySpacingExpanded;
};

class PLOT2D_EXPORT Plot2d_QwtPlotCurve : public QwtPlotCurve
{
public:
  Plot2d_QwtPlotCurve( const QString&, QwtPlot::Axis = QwtPlot::yLeft );
  virtual ~Plot2d_QwtPlotCurve();

public:
  virtual void     setYAxisIdentifierEnabled( const bool );

protected:
  virtual void     updateLegend( QwtLegend* ) const;
  virtual QWidget* legendItem() const;

private:
  QwtPlot::Axis    myYAxis;
  bool             myYAxisIdentifierEnabled;
};

class PLOT2D_EXPORT Plot2d_HistogramQwtItem: public QwtPlotItem
{
public:
  enum HistogramAttribute 
  {
    Auto = 0,
    Xfy  = 1
  };

  explicit Plot2d_HistogramQwtItem( const QString& = QString() );
  explicit Plot2d_HistogramQwtItem( const QwtText& );
  virtual ~Plot2d_HistogramQwtItem();

  void                   setData( const QwtIntervalData& );
  const QwtIntervalData& data() const;

  void                   setColor( const QColor& );
  QColor                 color() const;

  virtual QwtDoubleRect  boundingRect() const;
  virtual int            rtti() const;
  virtual void           draw( QPainter*, const QwtScaleMap&, 
			       const QwtScaleMap&, const QRect& ) const;

  void                   setBaseline( double );
  double                 baseline() const;

  void                   setHistogramAttribute( HistogramAttribute, bool = true );
  bool                   testHistogramAttribute( HistogramAttribute ) const;

protected:
  virtual void           drawBar( QPainter*, Qt::Orientation, const QRect& ) const;

private:
  void                   init();

private:
  int                    myAttributes;
  QwtIntervalData        myData;
  QColor                 myColor;
  double                 myReference;
};

class PLOT2D_EXPORT Plot2d_HistogramItem : public Plot2d_HistogramQwtItem
{
public:
  explicit Plot2d_HistogramItem( const QString& = QString() );
  explicit Plot2d_HistogramItem( const QwtText& );
  virtual ~Plot2d_HistogramItem();

  QList<QRect>           getBars() const;

  virtual void           updateLegend( QwtLegend* ) const;
  virtual void           draw( QPainter*, const QwtScaleMap&, 
			       const QwtScaleMap&, const QRect& ) const;

  void                   setCrossItems( bool theCross );
  bool                   isCrossItems() const;

protected:
  void                   drawRectAndLowers( QPainter*, Qt::Orientation,
					    const QRect& ) const;
  int                    getCrossedTop( const QRect& ) const;

protected:
  QList<QRect>           myBarItems;
  bool                   myCrossed;
};

#endif // PLOT2D_PLOTITEMS_H