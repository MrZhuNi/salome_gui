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
#ifndef PLOT2D_VIEWFRAME_H
#define PLOT2D_VIEWFRAME_H

#include "Plot2d_Curve.h"
#include <QtxMap.h>

#include <QWidget>
#include <QList>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>

class Plot2d_Plot2d;
class Plot2d_Prs;
class QCustomEvent;
class QwtPlotGrid;
class QwtPlotZoomer;

typedef QwtPlotCurve* QwtPlotCurvePtr;
typedef Plot2d_Curve* Plot2d_CurvePtr;
typedef QtxMap<QwtPlotCurvePtr, Plot2d_CurvePtr> CurveDict;

class PLOT2D_EXPORT Plot2d_ViewFrame : public QWidget
{ 
  Q_OBJECT

  enum { NoOpId, FitAreaId, ZoomId, PanId, GlPanId, DumpId, PrintId,
   ModeXLinearId, ModeXLogarithmicId, ModeYLinearId, ModeYLogarithmicId,
   LegendId, CurvePointsId, CurveLinesId, CurveSplinesId };
public:
  /* Construction/destruction */
  Plot2d_ViewFrame( QWidget* parent, const QString& title = "" );
  virtual ~Plot2d_ViewFrame();

  enum ObjectType { MainTitle, XTitle, YTitle, Y2Title, XAxis, YAxis, Y2Axis };

public:
  QWidget* getViewWidget();

  /* display */
  void    DisplayAll();
  void    EraseAll();
  void    Repaint();

  void    Display( const Plot2d_Prs* );
  void    Erase( const Plot2d_Prs*, const bool = false );
  Plot2d_Prs* CreatePrs( const char* entry = 0 );

  virtual bool eventFilter(QObject* watched, QEvent* e);

  /* operations */
  void    updateTitles( bool update = true );
  void    setTitle( const QString& title );
  QString getTitle() const { return myTitle; }
  void    displayCurve( Plot2d_Curve* curve, bool update = false );
  void    displayCurves( const curveList& curves, bool update = false );
  void    eraseCurve( Plot2d_Curve* curve, bool update = false );
  void    eraseCurves( const curveList& curves, bool update = false );
  int     getCurves( curveList& clist );
  const   CurveDict& getCurves();
  bool    isVisible( Plot2d_Curve* curve );
  void    updateCurve( Plot2d_Curve* curve, bool update = false );
  void    updateLegend( const Plot2d_Prs* prs );
  void    fitAll();
  void    fitArea( const QRect& area );
  void    fitData(const int mode,
		  const double xMin, const double xMax,
		  const double yMin, const double yMax,
		  const double y2Min = 0, const double y2Max = 0);

  void    getFitRanges(double& xMin, double& xMax,
		       double& yMin, double& yMax,
		       double& y2Min, double& y2Max);

  void    setTimeColorization( bool, const double theTimeValue = -1,
                               const QColor& theColor = Qt::gray );
  void    setTimeValue( const double theTimeValue );
  bool    isTimeColorization();

  /* view parameters */
  void    copyPreferences( Plot2d_ViewFrame* );
  void    setCurveType( int curveType, bool update = true );
  int     getCurveType() const { return myCurveType; }
  void    setCurveTitle( Plot2d_Curve* curve, const QString& title );
  void    showLegend( bool show, bool update = true );
  bool    isLegendShow() { return myShowLegend; };
  void    setLegendPos( int pos );
  int     getLegendPos() const { return myLegendPos; }
  void    setMarkerSize( const int size, bool update = true  );
  int     getMarkerSize() const { return myMarkerSize; }
  void    setBackgroundColor( const QColor& color );
  QColor  backgroundColor() const;
  void    setXGrid( bool xMajorEnabled,  const int xMajorMax,
                    bool xMinorEnabled,  const int xMinorMax, bool update = true );
  void    getXGrid( bool& xMajorEnabled, int& xMajorMax,
                    bool& xMinorEnabled, int& xMinorMax );
  void    setYGrid( bool yMajorEnabled,   const int yMajorMax,
                    bool yMinorEnabled,   const int yMinorMax,
                    bool y2MajorEnabled,  const int y2MajorMax,
                    bool y2MinorEnabled,  const int y2MinorMax, bool update = true );
  void    getYGrid( bool& yMajorEnabled,  int& yMajorMax,
                    bool& yMinorEnabled,  int& yMinorMax,
                    bool& y2MajorEnabled, int& y2MajorMax,
                    bool& y2MinorEnabled, int& y2MinorMax );
  void    setTitle( bool enabled, const QString& title, ObjectType type, bool update = true );
  QString getTitle( ObjectType type ) const;

  void    setSecondY( const bool& theSecondY );
  bool    getSecondY();

  bool    getKeepCurrentRange();
  void    setKeepCurrentRange( const bool& theKeepCurrentRange );

  bool    isTitleChangedByUser( const ObjectType type );
  void    forgetLocalUserChanges( const ObjectType type );

  void    setFont( const QFont& font, ObjectType type, bool update = true );
  void    setHorScaleMode( const int mode, bool update = true );
  int     getHorScaleMode() const { return myXMode; }
  void    setVerScaleMode( const int mode, bool update = true );
  int     getVerScaleMode() const { return myYMode; }

  bool    isModeHorLinear();
  bool    isModeVerLinear();

  // Protection against QwtCurve::drawLines() bug in Qwt 0.4.x: 
  // it crashes if switched to X/Y logarithmic mode, when one or more points have
  // non-positive X/Y coordinate
  bool    isXLogEnabled() const;
  bool    isYLogEnabled() const;

  virtual bool print( const QString& file = QString::null,
                      const QString& format = QString::null ) const;

  QString getVisualParameters();
  void    setVisualParameters( const QString& parameters );

  void    incrementalPan ( const int incrX, const int incrY );
  void    incrementalZoom( const int incrX, const int incrY );

  void    setAutoUpdateTitle( const ObjectType type, const bool upd );
  bool    getAutoUpdateTitle( const ObjectType type ) const;

  void    updateSymbols();

protected:
  int     testOperation( const QMouseEvent& );
  void    readPreferences();
  void    writePreferences();
  QString getInfo( const QPoint& pnt );
  virtual void wheelEvent( QWheelEvent* );
  QwtPlotCurve* getPlotCurve( Plot2d_Curve* curve );
  bool    hasPlotCurve( Plot2d_Curve* curve );
  void    setCurveType( QwtPlotCurve* curve, int curveType );
  void    cutCurveNonPositiveValues( const bool theIsXAxis,
                                     const bool theIsReplaceWithSmallValues );

public slots:
  void    onViewPan(); 
  void    onViewZoom();
  void    onViewFitAll();
  void    onViewFitArea();
  void    onViewGlobalPan(); 
  void    onSettings();
  void    onCurvesSettings();
  void    onFitData();
  void    onChangeBackground();
  void    onPanLeft();
  void    onPanRight();
  void    onPanUp();
  void    onPanDown();
  void    onZoomIn();
  void    onZoomOut();
  void    onTimeColorizationUpdated( double );

protected:
  virtual void customEvent( QEvent* );
  void    plotMousePressed( const QMouseEvent& );
  bool    plotMouseMoved( const QMouseEvent& );
  void    plotMouseReleased( const QMouseEvent& );

signals:
  void    vpModeHorChanged();
  void    vpModeVerChanged();
  void    vpCurveChanged();
  void    contextMenuRequested( QContextMenuEvent *e );
  void    legendClicked( QwtPlotItem* );
  void    curveDisplayed( Plot2d_Curve* );
  void    curveErased( Plot2d_Curve* );
  void    curvesErased( const curveList& );
  void    settingsUpdated();
  void    curvesUpdated();
  void	  xRangeUpdated( const double&, const double&);
  void	  yRangeUpdated( const double&, const double&, const double&, const double&);
  void	  bothRangesUpdated( const double&, const double&, const double&, 
                             const double&, const double&, const double& );
  void    manualTransformationApplied();
  void    fitAllApplied();
  void    backgroundColorChanged( const QColor& );
  void    legendStateChanged( const bool );

protected:
  Plot2d_Plot2d* myPlot;
  int            myOperation;
  QPoint         myPnt;

  int            myCurveType;
  bool           myShowLegend;
  int            myLegendPos;
  int            myMarkerSize;
  QColor         myBackground;
  QString        myTitle, myXTitle, myYTitle, myY2Title;
  bool           myTitleEnabled, myXTitleEnabled, myYTitleEnabled, myY2TitleEnabled;
  bool           myXGridMajorEnabled, myYGridMajorEnabled, myY2GridMajorEnabled;
  bool           myXGridMinorEnabled, myYGridMinorEnabled, myY2GridMinorEnabled;
  int            myXGridMaxMajor, myYGridMaxMajor, myY2GridMaxMajor;
  int            myXGridMaxMinor, myYGridMaxMinor, myY2GridMaxMinor;
  int            myXMode, myYMode;
  double         myXDistance, myYDistance, myYDistance2;
  bool           mySecondY;
  bool           myKeepCurrentRange;
  
  bool           myTitleAutoUpdate, myXTitleAutoUpdate, myYTitleAutoUpdate;
  bool           myTitleChangedByUser, myXTitleChangedByUser, myYTitleChangedByUser,
                 myY2TitleChangedByUser;

  static         QString myPrefTitle;
  static         QString myPrefXTitle;
  static         QString myPrefYTitle;
  static         QString myPrefY2Title;

  static bool    myPrefTitleChangedByUser;
  static bool    myXPrefTitleChangedByUser;
  static bool    myYPrefTitleChangedByUser;
  static bool    myY2PrefTitleChangedByUser;

  bool           myIsTimeColorization;
  double         myTimePosition;
  QColor         myInactiveColor;
};

class Plot2d_Plot2d : public QwtPlot 
{
  Q_OBJECT
public:
  Plot2d_Plot2d( QWidget* parent );

  void       setLogScale( int axisId, bool log10 );

  void       replot();
  void       getNextMarker( QwtSymbol::Style& typeMarker, QColor& color, Qt::PenStyle& typeLine );
  QwtLegend* getLegend() {
#if QWT_VERSION < 0x040200
     return d_legend;
#else  
     return legend(); /* mpv: porting to the Qwt 4.2.0 */
#endif
  }
  virtual QSize       sizeHint() const;
  virtual QSizePolicy sizePolicy() const;
  virtual QSize       minimumSizeHint() const;
  void                defaultPicker();
  void                setPickerMousePattern( int button, int state = Qt::NoButton );

  bool                polished() const { return myIsPolished; }
  QwtPlotGrid*        grid() { return myGrid; };
  CurveDict& getCurves() { return myCurves; }
  Plot2d_Curve*       getClosestCurve( QPoint p, double& distance, int& index );
  QList<Plot2d_Curve*> getClosestPoints( QPoint thePoint, double theRadius, QList< QList< int > >& thePntIndex );

  long                insertCurve( const QString &title,
                                   int xAxis = xBottom, 
                                   int yAxis = yLeft );

  bool                setCurveNbMarkers( Plot2d_Curve* curve, const double nb );
  double              curveNbMarkers( Plot2d_Curve* curve ) const;

  void                setZoomBase( const bool doReplot = true );

  static bool         closeColors( const QColor& color1, const QColor& color2 );

public slots:
  virtual void polish();

protected:
  bool       existMarker( const QwtSymbol::Style typeMarker, const QColor& color, const Qt::PenStyle typeLine );

signals:
  void                plotZoomed();

protected:
  CurveDict          myCurves;
  QwtPlotGrid*       myGrid;
  QList<QColor>      myColors;
  bool               myIsPolished;
  QwtPlotZoomer*     myPlotZoomer;
};

//! The class is derived from QwtPlotCurve.
/*!
  The class is derived from QwtPlotCurve. Its main purpose is redefining 
  drawSymbols virtual method in order to provide possibility to change 
  number of markers between steps.
*/

class Plot2d_PlotCurve : public QwtPlotCurve
{
public: 
  Plot2d_PlotCurve( const QString &title );
  virtual ~Plot2d_PlotCurve();

  void                setNbMarkers( const double );
  double              nbMarkers() const;
  void                setSymbolsColorData( const int *cData, int size );
  void                setSymbolsColorMap( const colorMap& theMap );

  void                setTimeColorization( bool isTimeColorization,
                                           const double theTimeValue = -1,
                                           const QColor& theColor = Qt::gray );
  bool                isTimeColorization() const;
  double              getTimePosition() const;
  QColor              getInactiveColor() const;

protected:

  virtual void drawCurve( QPainter *p, int style,
                          const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                          int from, int to) const;

  virtual void drawSymbols(QPainter *p, const QwtSymbol &,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        int from, int to) const;

private:
  double myNbMarkers;
  QwtArray<int> mySymbolsColorIds;
  colorMap mySymbolsColorMap;

  bool    myIsTimeColorization;
  double  myTimePosition;
  QColor  myInactiveColor;
};

#endif
