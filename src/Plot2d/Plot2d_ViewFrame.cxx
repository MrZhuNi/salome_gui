// Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#include "Plot2d_ViewFrame.h"

#include "Plot2d_Prs.h"
#include "Plot2d_Curve.h"
#include "Plot2d_PlotItems.h"
#include "Plot2d_FitDataDlg.h"
#include "Plot2d_ViewWindow.h"
#include "Plot2d_SetupViewDlg.h"
#include "Plot2d_ToolTip.h"

#include "SUIT_Tools.h"
#include "SUIT_Session.h"
#include "SUIT_MessageBox.h"
#include "SUIT_ResourceMgr.h"
#include "SUIT_Application.h"

#include <QApplication>
#include <QToolBar>
#include <QToolButton>
#include <QCursor>
#include <QColorDialog>
#include <QLayout>
#include <QMap>
#include <QPainter>
#include <QPaintDevice>
#include <QEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QPrinter>
#include <QPalette>
#include <QLocale>

#include <qwt_math.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_div.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_curve_fitter.h>

#include <iostream>
#include <stdlib.h>
#include <qprinter.h>

#include <qwt_legend.h>
#include <qwt_scale_widget.h>

#define DEFAULT_LINE_WIDTH     0     // (default) line width
#define DEFAULT_MARKER_SIZE    9     // default marker size
#define MIN_RECT_SIZE          11    // min sensibility area size

#define FITALL_EVENT           ( QEvent::User + 9999 )

const char* imageZoomCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
".#######........................",
"..aaaaaaa.......................",
"................................",
".............#####..............",
"...........##.aaaa##............",
"..........#.aa.....a#...........",
".........#.a.........#..........",
".........#a..........#a.........",
"........#.a...........#.........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
".........#...........#.a........",
".........#a..........#a.........",
".........##.........#.a.........",
"........#####.....##.a..........",
".......###aaa#####.aa...........",
"......###aa...aaaaa.......#.....",
".....###aa................#a....",
"....###aa.................#a....",
"...###aa...............#######..",
"....#aa.................aa#aaaa.",
".....a....................#a....",
"..........................#a....",
"...........................a....",
"................................",
"................................",
"................................",
"................................"};

const char* imageCrossCursor[] = { 
  "32 32 3 1",
  ". c None",
  "a c #000000",
  "# c #ffffff",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "...............#................",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  ".......#################........",
  "........aaaaaaa#aaaaaaaaa.......",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "................a...............",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................"};
  
/*!
  Constructor
*/
Plot2d_ViewFrame::Plot2d_ViewFrame( QWidget* parent, const QString& title )
     : QWidget (parent, 0),
       myOperation( NoOpId ), 
       myCurveType( 1 ), 
       myShowLegend( true ), myLegendPos( 1 ), myLegendFont("Helvetic",12),
       myLegendColor(Qt::black),
       myMarkerSize( DEFAULT_MARKER_SIZE ),
       myBackground( Qt::white ),
       myTitle( "" ), myXTitle( "" ), myYTitle( "" ), myY2Title( "" ),
       myTitleEnabled( true ), myXTitleEnabled( true ),
       myYTitleEnabled( true ), myY2TitleEnabled (true),
       myXGridMajorEnabled( true ), myYGridMajorEnabled( true ), myY2GridMajorEnabled( true ), 
       myXGridMinorEnabled( false ), myYGridMinorEnabled( false ), myY2GridMinorEnabled( false ),
       myXGridMaxMajor( 8 ), myYGridMaxMajor( 8 ), myY2GridMaxMajor( 8 ),
       myXGridMaxMinor( 5 ), myYGridMaxMinor( 5 ), myY2GridMaxMinor( 5 ),
       myXMode( 0 ), myYMode( 0 ), mySecondY( false ), myIsDefTitle( true )
{
  setObjectName( title );
  /* Plot 2d View */
  QVBoxLayout* aLayout = new QVBoxLayout( this ); 
  myPlot = new Plot2d_Plot2d( this );
  new Plot2d_ToolTip( this );

  aLayout->addWidget( myPlot );

  //  createActions();
  connect( myPlot, SIGNAL( legendClicked( QwtPlotItem* ) ), 
           this, SIGNAL( legendClicked( QwtPlotItem* ) ) );

  // IPAL 21465
  /*  connect( myPlot->axisWidget( QwtPlot::xBottom ), SIGNAL( scaleDivChanged() ),
           myPlot, SLOT( onScaleDivChanged() ) );
  connect( myPlot->axisWidget( QwtPlot::yLeft ), SIGNAL( scaleDivChanged() ),
           myPlot, SLOT( onScaleDivChanged() ) );
  if (mySecondY)
    connect( myPlot->axisWidget( QwtPlot::yRight ), SIGNAL( scaleDivChanged() ),
    myPlot, SLOT( onScaleDivChanged() ) );*/

  /* Initial Setup - get from the preferences */
  readPreferences();

  myPlot->setMargin( 5 );
  setCurveType( myCurveType, false );
  setXGrid( myXGridMajorEnabled, myXGridMaxMajor, myXGridMinorEnabled, myXGridMaxMinor, false );
  setYGrid( myYGridMajorEnabled, myYGridMaxMajor, myYGridMinorEnabled, myYGridMaxMinor,
            myY2GridMajorEnabled, myY2GridMaxMajor, myY2GridMinorEnabled, myY2GridMaxMinor, false );

  setTitle( myTitleEnabled,  myTitle,  MainTitle, false );
  setTitle( myXTitleEnabled, myXTitle, XTitle, false );
  setTitle( myYTitleEnabled, myYTitle, YTitle, false );

  if (mySecondY)
    setTitle( myY2TitleEnabled, myY2Title, Y2Title, false );
  setHorScaleMode( myXMode, false );
  setVerScaleMode( myYMode, false );
  setBackgroundColor( myBackground );
  setLegendPos( myLegendPos );
  setLegendFont( myLegendFont );
  setLegendFontColor( myLegendColor );
  showLegend( myShowLegend, false );
  myPlot->replot();

  if ( parent ) {
    resize( (int)(0.8 * parent->width()), (int)(0.8 * parent->height()) );
  }
  QwtScaleMap xMap = myPlot->canvasMap( QwtPlot::xBottom );
  QwtScaleMap yMap = myPlot->canvasMap( QwtPlot::yLeft );
  myXDistance = xMap.s2() - xMap.s1();
  myYDistance = yMap.s2() - yMap.s1();
  myYDistance2 = 0;
  if (mySecondY) {
    QwtScaleMap yMap2 = myPlot->canvasMap( QwtPlot::yRight );
    myYDistance2 = yMap2.s2() - yMap2.s1();
  }
  myPlot->canvas()->installEventFilter( this );
}
/*!
  Destructor
*/
Plot2d_ViewFrame::~Plot2d_ViewFrame()
{
}
/*!
  Gets window's central widget
*/
QWidget* Plot2d_ViewFrame::getViewWidget()
{
  return (QWidget*)myPlot;
}
/*!
  Actually this method just re-displays all curves which are presented in the viewer
*/
void Plot2d_ViewFrame::DisplayAll()
{
  objectList olist;
  getObjects( olist );
  foreach ( Plot2d_Object* o, olist )
    updateObject( o, false );
  myPlot->replot();
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}
/*!
   Removes all curves from the view
*/
void Plot2d_ViewFrame::EraseAll() 
{
  objectList anObjects;
  getObjects( anObjects );
  eraseObjects( anObjects, false );
  myObjects.clear();
  myPlot->replot();
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}
/*!
  Redraws viewframe contents
*/
void Plot2d_ViewFrame::Repaint()
{
  myPlot->replot();
}
/*!
  Display presentation
*/
void Plot2d_ViewFrame::Display( const Plot2d_Prs* prs )
{
  if ( !prs || prs->IsNull() )
    return;

  setEnableAxis( QwtPlot::yRight, prs->isSecondY() ); // VSR: is it correct? maybe we should only enable second Y axis if required

  // display all objects from presentation
  objectList anObjects = prs->getObjects();
  displayObjects( anObjects );
  setXGrid( myXGridMajorEnabled, myXGridMaxMajor, myXGridMinorEnabled, myXGridMaxMinor, true );
  setYGrid( myYGridMajorEnabled, myYGridMaxMajor, myYGridMinorEnabled, myYGridMaxMinor,
            myY2GridMajorEnabled, myY2GridMaxMajor, myY2GridMinorEnabled, myY2GridMaxMinor, true );
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}

/*!
  Erase presentation
*/
void Plot2d_ViewFrame::Erase( const Plot2d_Prs* prs, const bool )
{
  if ( !prs || prs->IsNull() )
    return;

  // erase all objects from presentation
  objectList anObjects = prs->getObjects();
  eraseObjects( anObjects );
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}

bool Plot2d_ViewFrame::eventFilter( QObject* watched, QEvent* e )
{
  if ( watched == myPlot->canvas() ) {
    int aType = e->type();
    switch( aType ) {
      case QEvent::MouseMove: {
        QMouseEvent* me = (QMouseEvent*)e;
        if ( me && ( me->buttons() != 0 || me->button() != 0 ) ) {
          QMouseEvent m( QEvent::MouseMove, me->pos(), me->button(),
                         me->buttons(), me->modifiers() );
          if ( plotMouseMoved( m ) )
            return true;
        }
        break;
      }
      case QEvent::MouseButtonPress: {
        QMouseEvent* me = (QMouseEvent*)e;
        if ( me && ( me->buttons() != 0 || me->button() != 0 ) ) {
          QMouseEvent m( QEvent::MouseButtonPress, me->pos(), me->button(),
                          me->buttons(), me->modifiers() );
          plotMousePressed( m );
        }
        break;
      }
      case QEvent::MouseButtonRelease: {
        QMouseEvent* me = (QMouseEvent*)e;
        if ( me && ( me->buttons() != 0 || me->button() != 0 ) ) {
          QMouseEvent m( QEvent::MouseButtonRelease, me->pos(), me->button(),
                         me->buttons(), me->modifiers() );
          plotMouseReleased( m );
        }
        break;
      }
    case QEvent::ContextMenu:
      // Fix from SLN
      // Do nothing because context menu is called from MouseRelease
      return true;
    }
  }
  return QWidget::eventFilter( watched, e );
}

/*!
  Sets title
*/
void Plot2d_ViewFrame::setTitle( const QString& title )
{
  setTitle( myTitleEnabled, title, MainTitle, true );
  myIsDefTitle = false;
}

/*!
  Gets title
*/
QString Plot2d_ViewFrame::getTitle() const
{
  return myTitle;
}

/*!
  Reads Plot2d view settings from the preferences
*/
void Plot2d_ViewFrame::readPreferences()
{
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();

  myCurveType = resMgr->integerValue( "Plot2d", "CurveType", myCurveType );
  setCurveType( resMgr->integerValue( "Plot2d", "CurveType", myCurveType ) );

  myShowLegend = resMgr->booleanValue( "Plot2d", "ShowLegend", myShowLegend );
  myLegendPos = resMgr->integerValue( "Plot2d", "LegendPos", myLegendPos );
  myLegendFont = resMgr->fontValue( "Plot2d", "LegendFont", myLegendFont );
  myLegendColor = resMgr->colorValue( "Plot2d", "LegendFontColor", myLegendColor );
  myMarkerSize = resMgr->integerValue( "Plot2d", "MarkerSize", myMarkerSize );
  myBackground = resMgr->colorValue( "Plot2d", "Background", myBackground );

  myTitleEnabled = resMgr->booleanValue( "Plot2d", "ShowTitle", myTitleEnabled );
  myXTitleEnabled = resMgr->booleanValue( "Plot2d", "ShowHorTitle", myXTitleEnabled );
  myYTitleEnabled = resMgr->booleanValue( "Plot2d", "ShowVerLeftTitle", myYTitleEnabled );
  myY2TitleEnabled = resMgr->booleanValue( "Plot2d", "ShowVerRightTitle", myY2TitleEnabled );

  myXGridMajorEnabled = resMgr->booleanValue( "Plot2d", "EnableHorMajorGrid", myXGridMajorEnabled );
  myYGridMajorEnabled = resMgr->booleanValue( "Plot2d", "EnableVerMajorGrid", myYGridMajorEnabled );
  myY2GridMajorEnabled = resMgr->booleanValue( "Plot2d", "EnableRightVerMajorGrid", myY2GridMajorEnabled );

  myXGridMinorEnabled = resMgr->booleanValue( "Plot2d", "EnableHorMinorGrid", myXGridMinorEnabled );
  myYGridMinorEnabled = resMgr->booleanValue( "Plot2d", "EnableVerMinorGrid", myYGridMinorEnabled );
  myY2GridMinorEnabled = resMgr->booleanValue( "Plot2d", "EnableRightVerMinorGrid", myY2GridMinorEnabled );

  myXGridMaxMajor = resMgr->integerValue( "Plot2d", "HorMajorGridMax", myXGridMaxMajor );
  myYGridMaxMajor = resMgr->integerValue( "Plot2d", "VerMajorGridMax", myYGridMaxMajor );
  if ( mySecondY )
    myY2GridMaxMajor = resMgr->integerValue( "Plot2d", "VerMajorRightGridMax", myY2GridMaxMajor );

  myXGridMaxMinor = resMgr->integerValue( "Plot2d", "HorMinorGridMax", myXGridMaxMinor );
  myYGridMaxMinor = resMgr->integerValue( "Plot2d", "VerMinorGridMax", myYGridMaxMinor );
  if ( mySecondY )
    myY2GridMaxMinor = resMgr->integerValue( "Plot2d", "VerMinorGridMax", myY2GridMaxMinor );

  setHorScaleMode( qMax( 0, qMin( 1, resMgr->integerValue( "Plot2d", "HorScaleMode", myXMode ) ) ), false );
  setVerScaleMode( qMax( 0, qMin( 1, resMgr->integerValue( "Plot2d", "VerScaleMode", myYMode ) ) ), false );
}

/*!
  Writes Plot2d view settings to the preferences
*/
void Plot2d_ViewFrame::writePreferences()
{
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();

  resMgr->setValue( "Plot2d", "CurveType", myCurveType );
  resMgr->setValue( "Plot2d", "ShowLegend", myShowLegend );
  resMgr->setValue( "Plot2d", "LegendPos", myLegendPos );
  resMgr->setValue( "Plot2d", "LegendFont", myLegendFont );
  resMgr->setValue( "Plot2d", "LegendFontColor", myLegendColor );
  resMgr->setValue( "Plot2d", "MarkerSize", myMarkerSize );
  resMgr->setValue( "Plot2d", "Background", myBackground );
  resMgr->setValue( "Plot2d", "ShowTitle", myTitleEnabled );
  resMgr->setValue( "Plot2d", "ShowHorTitle", myXTitleEnabled );
  resMgr->setValue( "Plot2d", "ShowVerLeftTitle", myYTitleEnabled );
  if ( mySecondY )
    resMgr->setValue( "Plot2d", "ShowVerRightTitle", myY2TitleEnabled );

  resMgr->setValue( "Plot2d", "EnableHorMajorGrid", myXGridMajorEnabled );
  resMgr->setValue( "Plot2d", "EnableVerMajorGrid", myYGridMajorEnabled );
  resMgr->setValue( "Plot2d", "EnableHorMinorGrid", myXGridMinorEnabled );
  resMgr->setValue( "Plot2d", "EnableVerMinorGrid", myYGridMinorEnabled );

  resMgr->setValue( "Plot2d", "HorMajorGridMax", myXGridMaxMajor );
  resMgr->setValue( "Plot2d", "VerMajorGridMax", myYGridMaxMajor );

  resMgr->setValue( "Plot2d", "HorMinorGridMax", myXGridMaxMinor );
  resMgr->setValue( "Plot2d", "VerMinorGridMax", myYGridMaxMinor );

  resMgr->setValue( "Plot2d", "HorScaleMode", myXMode );

  if ( mySecondY )
  {
    resMgr->setValue( "Plot2d", "EnableRightVerMajorGrid", myY2GridMajorEnabled );
    resMgr->setValue( "Plot2d", "EnableRightVerMinorGrid", myY2GridMinorEnabled );
    resMgr->setValue( "Plot2d", "VerRightMajorGridMax", myY2GridMaxMajor );
    resMgr->setValue( "Plot2d", "VerRightMinorGridMax", myY2GridMaxMinor );
  }

  resMgr->setValue( "Plot2d", "VerScaleMode", myYMode );
}

/*!
  Prints mouse cursor coordinates into string
*/
QString Plot2d_ViewFrame::getInfo( const QPoint& pnt ) 
{
  int i;
  QwtValueList aTicks;
  bool xFound = false, yFound = false;
  double xCoord, yCoord;
  const QwtScaleDiv* aXscale = myPlot->axisScaleDiv( QwtPlot::xBottom );
  aTicks = aXscale->ticks( QwtScaleDiv::MajorTick );
  for ( i = 0; i < aTicks.count(); i++ ) {
    double majXmark = aTicks[i];
    int xmark = myPlot->transform( QwtPlot::xBottom, majXmark );
    if ( xmark-2 == pnt.x() ) {
      xCoord = majXmark; 
      xFound = true;
      break;
    }
  }
  if ( !xFound ) {
    aTicks = aXscale->ticks( QwtScaleDiv::MinorTick );
    for ( i = 0; i < aTicks.count(); i++ ) {
      double minXmark = aTicks[i];
      int xmark = myPlot->transform( QwtPlot::xBottom, minXmark );
      if ( xmark-2 == pnt.x() ) {
        xCoord = minXmark; 
        xFound = true;
        break;
      }
    }
  }  
  const QwtScaleDiv* aYscale = myPlot->axisScaleDiv( QwtPlot::yLeft );
  aTicks = aYscale->ticks( QwtScaleDiv::MajorTick );
  for ( i = 0; i < aTicks.count(); i++ ) {
    double majYmark = aTicks[i];
    int ymark = myPlot->transform( QwtPlot::yLeft, majYmark );
    if ( ymark-2 == pnt.y() ) {
      yCoord = majYmark; 
      yFound = true;
      break;
    }
  }
  if ( !yFound ) {
    aTicks = aYscale->ticks( QwtScaleDiv::MinorTick );
    for ( i = 0; i < aTicks.count(); i++ ) {
      double minYmark = aTicks[i];
      int ymark = myPlot->transform( QwtPlot::yLeft, minYmark );
      if ( ymark-2 == pnt.y() ) {
        yCoord = minYmark; 
        yFound = true;
        break;
      }
    }
  }  

  QString strX = QString::number( xFound ? xCoord : myPlot->invTransform( QwtPlot::xBottom, pnt.x() ) ).trimmed();
  if ( strX == "-0" )
    strX = "0";
  QString strY = QString::number( yFound ? yCoord : myPlot->invTransform( QwtPlot::yLeft, pnt.y() ) ).trimmed();
  if ( strY == "-0" )
    strY = "0";
  QString info = "";

  if (mySecondY) {
    bool yFound2 = false;
    double yCoord2;

    const QwtScaleDiv* aYscale2 = myPlot->axisScaleDiv( QwtPlot::yRight );
    aTicks = aYscale2->ticks( QwtScaleDiv::MajorTick );
    for ( i = 0; i < aTicks.count(); i++ ) {
      double majYmark = aTicks[i];
      int ymark = myPlot->transform( QwtPlot::yRight, majYmark );
      if ( ymark-2 == pnt.y() ) {
        yCoord2 = majYmark; 
        yFound2 = true;
        break;
      }
    }
    if ( !yFound2 ) {
      aTicks = aYscale2->ticks( QwtScaleDiv::MinorTick );
      for ( i = 0; i < aTicks.count(); i++ ) {
        double minYmark = aTicks[i];
        int ymark = myPlot->transform( QwtPlot::yRight, minYmark );
        if ( ymark-2 == pnt.y() ) {
          yCoord2 = minYmark; 
          yFound2 = true;
          break;
        }
      }
    }
    QString strY2 = QString::number( yFound2 ? yCoord2 : 
                      myPlot->invTransform( QwtPlot::yRight, pnt.y() ) ).trimmed();
    if ( strY2 == "-0" )
    strY2 = "0";
    info = tr("INF_COORDINATES_SOME_Y").arg( strX ).arg( strY ).arg( strY2 );
  }
  else
    info = tr("INF_COORDINATES").arg( strX ).arg( strY );

  return info;
}

/*!
  Adds curve into view
*/
void Plot2d_ViewFrame::displayCurve( Plot2d_Curve* curve, bool update )
{
  displayObject( curve, update );
}

/*!
  Adds curves into view
*/
void Plot2d_ViewFrame::displayCurves( const curveList& curves, bool update )
{
  objectList objects;
  foreach ( Plot2d_Curve* curve, curves )
    objects << curve;
  displayObjects( objects, update );
}

/*!
  Erases curve
*/
void Plot2d_ViewFrame::eraseCurve( Plot2d_Curve* curve, bool update )
{
  eraseObject( curve, update );
}

/*!
  Erases curves
*/
void Plot2d_ViewFrame::eraseCurves( const curveList& curves, bool update )
{
  objectList objects;
  foreach ( Plot2d_Curve* curve, curves )
    objects << curve;
  eraseObjects( objects, update );
}

/*!
  Updates curves attributes
*/
void Plot2d_ViewFrame::updateCurve( Plot2d_Curve* curve, bool update )
{
  updateObject( curve, update );
}

/*!
  Gets lsit of displayed curves
*/
int Plot2d_ViewFrame::getCurves( curveList& curves ) const
{
  curves.clear();

  CurveDict aCurves = getCurves();
  CurveDict::iterator it;
  for ( it = aCurves.begin(); it != aCurves.end(); it++ )
    curves << it.value();
  return curves.count();
}

CurveDict Plot2d_ViewFrame::getCurves() const
{
  CurveDict curves;
  ObjectDict::const_iterator it = myObjects.begin(), aLast = myObjects.end();
  for ( ; it != aLast; it++ ) {
    QwtPlotItem* anItem = it.key();
    if ( anItem && anItem->rtti() == QwtPlotItem::Rtti_PlotCurve ) {
      QwtPlotCurve* aPCurve = dynamic_cast<QwtPlotCurve*>( anItem );
      Plot2d_Curve* aCurve = dynamic_cast<Plot2d_Curve*>( it.value() );
      if ( aPCurve && aCurve )
        curves.insert( aPCurve, aCurve );
    }
  }
  return curves;
}

/*!
  Adds object into view
*/
void Plot2d_ViewFrame::displayObject( Plot2d_Object* object, bool update )
{
  if ( !object )
    return;
  
  if ( object->getYAxis() == QwtPlot::yRight )
    mySecondY = true;

  // san -- Protection against QwtCurve bug in Qwt 0.4.x: 
  // it crashes if switched to X/Y logarithmic mode, when one or more points have
  // non-positive X/Y coordinate
  if ( myXMode && object->getMinX() <= 0. )
    setHorScaleMode( 0, false );
  if ( myYMode && object->getMinY() <= 0. )
    setVerScaleMode( 0, false );

  if ( object->isAutoAssign() )
    object->autoFill( myPlot );
  
  if ( hasPlotObject( object ) ) {
    updateObject( object, update );
  }
  else {
    QwtPlotItem* anItem = object->createPlotItem();
    anItem->attach( myPlot );
    myObjects.insert( anItem, object );
    //myPlot->setCurveYAxis(curveKey, curve->getYAxis());

    if ( object->rtti() == QwtPlotItem::Rtti_PlotCurve ) {
      Plot2d_Curve* aCurve = dynamic_cast<Plot2d_Curve*>( object );
      if ( aCurve ) {
        aCurve->setMarkerSize( myMarkerSize );
        aCurve->updatePlotItem( anItem );
        setCurveType( getPlotCurve( aCurve ), myCurveType );
      }
    }
  }
  updateTitles();
  myPlot->updateYAxisIdentifiers();
  if ( update )
    myPlot->replot();
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}

/*!
  Adds objects into view
*/
void Plot2d_ViewFrame::displayObjects( const objectList& objects, bool update )
{
  //myPlot->setUpdatesEnabled( false ); // call this function deprecate update of legend
  foreach ( Plot2d_Object* object, objects )
    displayObject( object, false );
  fitAll();
  //myPlot->setUpdatesEnabled( true );
  // update legend
  if ( update )
    myPlot->replot();
}

/*!
  Erases object
*/
void Plot2d_ViewFrame::eraseObject( Plot2d_Object* object, bool update )
{
  if ( !object )
    return;

  if ( hasPlotObject( object ) ) {
    QwtPlotItem* anObject = getPlotObject( object );
    anObject->hide();
    anObject->detach();
    myObjects.remove( anObject );
    updateTitles();
    myPlot->updateYAxisIdentifiers();
    if ( update )
      myPlot->replot();
  }
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}

/*!
  Erases objects
*/
void Plot2d_ViewFrame::eraseObjects( const objectList& objects, bool update )
{
  foreach ( Plot2d_Object* object, objects )
    eraseObject( object, false );

  //  fitAll();
  if ( update )
    myPlot->replot();
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}

/*!
  Updates objects attributes
*/
void Plot2d_ViewFrame::updateObject( Plot2d_Object* object, bool update )
{
  if ( !object )
    return;
  if ( hasPlotObject( object ) ) {
    QwtPlotItem* anItem = getPlotObject( object );
    if ( !anItem )
      return;
    object->updatePlotItem( anItem );
    anItem->setVisible( true );
    if ( update )
      myPlot->replot();
    if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
  }
}

/*!
  Gets lsit of displayed curves
*/
int Plot2d_ViewFrame::getObjects( objectList& objects ) const
{
  objects.clear();

  ObjectDict::const_iterator it;
  for ( it = myObjects.begin(); it != myObjects.end(); it++ )
    objects << it.value();
  return objects.count();
}

/*!
  Returns true if the curve is visible
*/
bool Plot2d_ViewFrame::isVisible( Plot2d_Object* object ) const
{
  return object && hasPlotObject( object ) && getPlotObject( object )->isVisible();
} 

/*!
  update legend
*/
void Plot2d_ViewFrame::updateLegend( const Plot2d_Prs* prs )
{
  if ( !prs || prs->IsNull() )
    return;

  ObjectDict::iterator it = myObjects.begin();
  Plot2d_Object* anObj;
  for (; it != myObjects.end(); ++it ) {
    anObj = *it;
    if ( hasPlotObject( anObj ) )
      getPlotObject( anObj )->setTitle( !anObj->getName().isEmpty() ?
                            anObj->getName() : anObj->getVerTitle() );
  }
}

/*!
  update legend
*/
void Plot2d_ViewFrame::updateLegend() {
  if ( myPlot->getLegend() ) {
    ObjectDict::iterator it = myObjects.begin();
    for( ; it != myObjects.end(); ++it ) 
      it.key()->updateLegend(myPlot->getLegend());
  }
}


/*!
  Fits the view to see all data
*/
void Plot2d_ViewFrame::fitAll()
{
  // Postpone fitAll operation until QwtPlot geometry
  // has been fully defined
  if ( !myPlot->polished() ){
    QApplication::postEvent( this, new QEvent( (QEvent::Type)FITALL_EVENT ) );
    return;
  }

  myPlot->setAxisAutoScale( QwtPlot::yLeft );
  myPlot->setAxisAutoScale( QwtPlot::xBottom );
  myPlot->replot();

  double xmin, xmax, y1min, y1max, y2min, y2max;
  getFitRangeByCurves(xmin, xmax, y1min, y1max, y2min, y2max);

  myPlot->setAxisScale( QwtPlot::xBottom, xmin, xmax );
  myPlot->setAxisScale( QwtPlot::yLeft, y1min, y1max );

  if (mySecondY) {
    myPlot->setAxisAutoScale( QwtPlot::yRight );
    myPlot->replot();
    myPlot->setAxisScale( QwtPlot::yRight, y2min, y2max );
  }
  myPlot->replot();
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}

/*!
  Fits the view to rectangle area (pixels)
*/
void Plot2d_ViewFrame::fitArea( const QRect& area )
{
  QRect rect = area.normalized();
  if ( rect.width() < MIN_RECT_SIZE ) {
    rect.setWidth( MIN_RECT_SIZE );
    rect.setLeft( rect.left() - MIN_RECT_SIZE/2 );
  }
  if ( rect.height() < MIN_RECT_SIZE ) {
    rect.setHeight( MIN_RECT_SIZE );
    rect.setTop( rect.top() - MIN_RECT_SIZE/2 );
  }
  myPlot->setAxisScale( QwtPlot::yLeft, 
            myPlot->invTransform( QwtPlot::yLeft, rect.top() ), 
            myPlot->invTransform( QwtPlot::yLeft, rect.bottom() ) );
  if (mySecondY)
    myPlot->setAxisScale( QwtPlot::yRight, 
            myPlot->invTransform( QwtPlot::yRight, rect.top() ), 
            myPlot->invTransform( QwtPlot::yRight, rect.bottom() ) );
  myPlot->setAxisScale( QwtPlot::xBottom, 
            myPlot->invTransform( QwtPlot::xBottom, rect.left() ), 
            myPlot->invTransform( QwtPlot::xBottom, rect.right() ) );
  myPlot->replot();
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}

/*!
  "Fit Data" command for TUI interface
*/
void Plot2d_ViewFrame::fitData(const int mode,
                               const double xMin, const double xMax,
                               const double yMin, const double yMax,
                               double y2Min, double y2Max)
{
  if ( mode == 0 || mode == 2 ) {
    myPlot->setAxisScale( QwtPlot::yLeft, yMin, yMax );
    if (mySecondY)
      myPlot->setAxisScale( QwtPlot::yRight, y2Min, y2Max );
  }
  if ( mode == 0 || mode == 1 ) 
    myPlot->setAxisScale( QwtPlot::xBottom, xMin, xMax ); 
  myPlot->replot();
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
}

/*!
  Gets current fit ranges for view frame
*/
void Plot2d_ViewFrame::getFitRanges(double& xMin,double& xMax,
                                    double& yMin, double& yMax,
                                    double& y2Min, double& y2Max)
{
  int ixMin = myPlot->canvasMap( QwtPlot::xBottom ).transform( myPlot->canvasMap( QwtPlot::xBottom ).s1() );
  int ixMax = myPlot->canvasMap( QwtPlot::xBottom ).transform( myPlot->canvasMap( QwtPlot::xBottom ).s2() );
  int iyMin = myPlot->canvasMap( QwtPlot::yLeft ).transform( myPlot->canvasMap( QwtPlot::yLeft ).s1() );
  int iyMax = myPlot->canvasMap( QwtPlot::yLeft ).transform( myPlot->canvasMap( QwtPlot::yLeft ).s2() );
  xMin = myPlot->invTransform(QwtPlot::xBottom, ixMin);
  xMax = myPlot->invTransform(QwtPlot::xBottom, ixMax);
  yMin = myPlot->invTransform(QwtPlot::yLeft, iyMin);
  yMax = myPlot->invTransform(QwtPlot::yLeft, iyMax);
  y2Min = 0;
  y2Max = 0;
  if (mySecondY) {
    int iyMin = myPlot->canvasMap( QwtPlot::yRight ).transform( myPlot->canvasMap( QwtPlot::yRight ).s1() );
    int iyMax = myPlot->canvasMap( QwtPlot::yRight ).transform( myPlot->canvasMap( QwtPlot::yRight ).s2() );
    y2Min = myPlot->invTransform(QwtPlot::yRight, iyMin);
    y2Max = myPlot->invTransform(QwtPlot::yRight, iyMax);
  }
}

/*!
  Gets current fit ranges by Curves
*/
void Plot2d_ViewFrame::getFitRangeByCurves(double& xMin,  double& xMax,
                                           double& yMin,  double& yMax,
                                           double& y2Min, double& y2Max)
{
  bool emptyV1 = true, emptyV2 = true;
  if ( !myObjects.isEmpty() ) {
    ObjectDict::const_iterator it = myObjects.begin();
    for ( ; it != myObjects.end(); it++ ) {
      bool isV2 = it.value()->getYAxis() == QwtPlot::yRight;
      if ( !it.value()->isEmpty() ) {
	if ( emptyV1 && emptyV2 ) {
	  xMin = 1e150;
	  xMax = -1e150;
	}
	if ( emptyV1 ) {
	  yMin = 1e150;
	  yMax = -1e150;
	}
	if ( emptyV2 ) {
	  y2Min = 1e150;
	  y2Max = -1e150;
	}
	isV2 ? emptyV2 = false : emptyV1 = false;
	xMin = qMin( xMin, it.value()->getMinX() );
	xMax = qMax( xMax, it.value()->getMaxX() );
        if ( isV2 ) {
          y2Min = qMin( y2Min, it.value()->getMinY() );
          y2Max = qMax( y2Max, it.value()->getMaxY() );
        }
        else {
          yMin = qMin( yMin, it.value()->getMinY() );
          yMax = qMax( yMax, it.value()->getMaxY() );
        }
      }
    }
    if ( xMin == xMax ) {
      xMin = xMin == 0. ? -1. : xMin - xMin/10.;
      xMax = xMax == 0. ?  1. : xMax + xMax/10.;
    }
    if ( yMin == yMax ) {
      yMin = yMin == 0. ? -1. : yMin - yMin/10.;
      yMax = yMax == 0. ?  1  : yMax + yMax/10.;
    }
    if ( y2Min == y2Max ) {
      y2Min = y2Min == 0. ? -1. : y2Min - y2Min/10.;
      y2Max = y2Max == 0. ?  1  : y2Max + y2Max/10.;
    }
  }
  // default values
  if ( emptyV1 && emptyV2 ) {
    xMin = isModeHorLinear() ? 0.    : 1.;
    xMax = isModeHorLinear() ? 1000. : 1e5;
  }
  if ( emptyV1  ) {
    yMin = isModeVerLinear() ? 0.    : 1.;
    yMax = isModeVerLinear() ? 1000. : 1e5;
  }
  if ( emptyV2  ) {
    y2Min = isModeVerLinear() ? 0.    : 1.;
    y2Max = isModeVerLinear() ? 1000. : 1e5;
  }
}

/*!
  Tests if it is necessary to start operation on mouse action
*/
int Plot2d_ViewFrame::testOperation( const QMouseEvent& me )
{
  int btn = me.button() | me.modifiers();
  const int zoomBtn = Qt::ControlModifier | Qt::LeftButton;
  const int panBtn  = Qt::ControlModifier | Qt::MidButton;
  const int fitBtn  = Qt::ControlModifier | Qt::RightButton;

  int op = NoOpId;
  if ( btn == zoomBtn ) {
    QPixmap zoomPixmap (imageZoomCursor);
    QCursor zoomCursor (zoomPixmap);
    myPlot->canvas()->setCursor( zoomCursor );
    op = ZoomId;
  }
  else if ( btn == panBtn ) {
    myPlot->canvas()->setCursor( QCursor( Qt::SizeAllCursor ) );
    op = PanId;
  }
  else if ( btn == fitBtn ) {
    myPlot->canvas()->setCursor( QCursor( Qt::PointingHandCursor ) );
    op = FitAreaId;
  }
  return op;
}

/*!
  "Settings" toolbar action slot
*/
void Plot2d_ViewFrame::onSettings()
{
#ifdef TEST_AUTOASSIGN
  typedef QMap<int,int> IList;
  typedef QMap<QString,int> SList;
  IList mars, lins;
  SList cols;
  cols[ "red-min" ]   = 1000;
  cols[ "red-max" ]   = -1;
  cols[ "green-min" ] = 1000;
  cols[ "green-max" ] = -1;
  cols[ "blue-min" ]  = 1000;
  cols[ "blue-max" ]  = -1;
  for ( unsigned i = 0; i < 10000; i++ ) {
    QwtSymbol::Style typeMarker;
    QColor           color;
    Qt::PenStyle     typeLine;
    myPlot->getNextMarker( typeMarker, color, typeLine );
    if ( mars.contains(typeMarker) )
      mars[ typeMarker ] = mars[ typeMarker ]+1;
    else
      mars[ typeMarker ] = 0;
    if ( lins.contains(typeLine) )
      lins[ typeLine ] = lins[ typeLine ]+1;
    else
      lins[ typeLine ] = 0;
    if ( cols[ "red-max" ] < color.red() )
      cols[ "red-max" ] = color.red();
    if ( cols[ "red-min" ] > color.red() )
      cols[ "red-min" ] = color.red();
    if ( cols[ "green-max" ] < color.green() )
      cols[ "green-max" ] = color.green();
    if ( cols[ "green-min" ] > color.green() )
      cols[ "green-min" ] = color.green();
    if ( cols[ "blue-max" ] < color.blue() )
      cols[ "blue-max" ] = color.blue();
    if ( cols[ "blue-min" ] > color.blue() )
      cols[ "blue-min" ] = color.blue();
  }
#endif
  
  Plot2d_SetupViewDlg* dlg = new Plot2d_SetupViewDlg( this, true, mySecondY );
  dlg->setMainTitle( myTitleEnabled, myTitle );
  dlg->setXTitle( myXTitleEnabled, myXTitle );
  dlg->setYTitle( myYTitleEnabled, myYTitle );
  if (mySecondY)
    dlg->setY2Title( myY2TitleEnabled, myY2Title );
  dlg->setCurveType( myCurveType );
  dlg->setLegend( myShowLegend, myLegendPos, myLegendFont, myLegendColor );
  dlg->setMarkerSize( myMarkerSize );
  dlg->setBackgroundColor( myBackground );
  dlg->setScaleMode(myXMode, myYMode);
  //
  dlg->setMajorGrid( myXGridMajorEnabled, myPlot->axisMaxMajor( QwtPlot::xBottom ),
         myYGridMajorEnabled, myPlot->axisMaxMajor( QwtPlot::yLeft ),
         myY2GridMajorEnabled, myPlot->axisMaxMajor( QwtPlot::yRight ) );
  dlg->setMinorGrid( myXGridMinorEnabled, myPlot->axisMaxMinor( QwtPlot::xBottom ),
         myYGridMinorEnabled, myPlot->axisMaxMinor( QwtPlot::yLeft ),
         myY2GridMinorEnabled, myPlot->axisMaxMinor( QwtPlot::yRight ) );
  if ( dlg->exec() == QDialog::Accepted ) {
    // horizontal axis title
    setTitle( dlg->isXTitleEnabled(), dlg->getXTitle(), XTitle, false );
    // vertical left axis title
    setTitle( dlg->isYTitleEnabled(), dlg->getYTitle(), YTitle, false );
    if (mySecondY) // vertical right axis title
      setTitle( dlg->isY2TitleEnabled(), dlg->getY2Title(), Y2Title, false );

    // main title
    if( dlg->isMainTitleEnabled() && myTitle != dlg->getMainTitle() ) 
      myIsDefTitle = false;
    setTitle( dlg->isMainTitleEnabled(), dlg->getMainTitle(), MainTitle, true );
    // curve type
    if ( myCurveType != dlg->getCurveType() ) {
      setCurveType( dlg->getCurveType(), false );
    }
    // legend
    if ( myShowLegend != dlg->isLegendEnabled() ) {
      showLegend( dlg->isLegendEnabled(), false );
    }
    if ( myLegendPos != dlg->getLegendPos() ) {
      setLegendPos( dlg->getLegendPos() );
    }
	if ( myLegendFont != dlg->getLegendFont() ) {
      setLegendFont( dlg->getLegendFont() );
    }
	if ( myLegendColor != dlg->getLegendColor() ) {
      setLegendFontColor( dlg->getLegendColor() );
    }

    // marker size
    if ( myMarkerSize != dlg->getMarkerSize() ) {
      setMarkerSize( dlg->getMarkerSize(), false );
    }
    // background color
    if ( myBackground != dlg->getBackgroundColor() ) {
      setBackgroundColor( dlg->getBackgroundColor() );
    }
    // grid
    bool aXGridMajorEnabled, aXGridMinorEnabled, aYGridMajorEnabled, aYGridMinorEnabled,
         aY2GridMajorEnabled, aY2GridMinorEnabled;
    int  aXGridMaxMajor, aXGridMaxMinor, aYGridMaxMajor, aYGridMaxMinor,
         aY2GridMaxMajor, aY2GridMaxMinor;
    dlg->getMajorGrid( aXGridMajorEnabled, aXGridMaxMajor, aYGridMajorEnabled, aYGridMaxMajor,
                       aY2GridMajorEnabled, aY2GridMaxMajor);
    dlg->getMinorGrid( aXGridMinorEnabled, aXGridMaxMinor, aYGridMinorEnabled, aYGridMaxMinor,
                       aY2GridMinorEnabled, aY2GridMaxMinor);
    setXGrid( aXGridMajorEnabled, aXGridMaxMajor, aXGridMinorEnabled, aXGridMaxMinor, false );
    setYGrid( aYGridMajorEnabled, aYGridMaxMajor, aYGridMinorEnabled, aYGridMaxMinor,
              aY2GridMajorEnabled, aY2GridMaxMajor, aY2GridMinorEnabled, aY2GridMaxMinor, false );
    if ( myXMode != dlg->getXScaleMode() ) {
      setHorScaleMode( dlg->getXScaleMode() );
    }
    if ( myYMode != dlg->getYScaleMode() ) {
      setVerScaleMode( dlg->getYScaleMode() );
    }
    // update view
    myPlot->replot();
    // update preferences
    if ( dlg->isSetAsDefault() ) 
      writePreferences();
  }
  delete dlg;
}

/*!
  "Fit Data" command slot
*/
void Plot2d_ViewFrame::onFitData()
{
  Plot2d_FitDataDlg* dlg = new Plot2d_FitDataDlg( this, mySecondY );
  double xMin,xMax,yMin,yMax,y2Min,y2Max;
  getFitRanges(xMin,xMax,yMin,yMax,y2Min,y2Max);
  
  dlg->setRange( xMin, xMax, yMin, yMax, y2Min, y2Max );
  if ( dlg->exec() == QDialog::Accepted ) {
    int mode = dlg->getRange( xMin, xMax, yMin, yMax, y2Min, y2Max );
    fitData(mode,xMin,xMax,yMin,yMax,y2Min,y2Max);
  }
  delete dlg;
}

/*!
  Change background color
*/
void Plot2d_ViewFrame::onChangeBackground()
{
  QColor selColor = QColorDialog::getColor ( backgroundColor(), this ); 
  if ( selColor.isValid() ) {
    setBackgroundColor( selColor );
  }
}

/*!
  Sets curve type
*/
void Plot2d_ViewFrame::setCurveType( int curveType, bool update )
{
  myCurveType = curveType;
  CurveDict aCurves = getCurves();
  CurveDict::iterator it = aCurves.begin();
  for ( ; it != aCurves.end(); it++ ) {
    QwtPlotCurve* crv = it.key();
    if ( crv )
      setCurveType( crv, myCurveType );
  }
  if ( update )
    myPlot->replot();
  emit vpCurveChanged();
}

/*!
  Gets curve type
*/
int Plot2d_ViewFrame::getCurveType() const
{
  return myCurveType;
}

/*!
  Sets curve title
  \param curveKey - curve id
  \param title - new title
*/
void Plot2d_ViewFrame::setCurveTitle( Plot2d_Curve* curve, const QString& title ) 
{
  setObjectTitle( curve, title );
}

/*!
  Sets object title
  \param object - object id
  \param title - new title
*/
void Plot2d_ViewFrame::setObjectTitle( Plot2d_Object* object, const QString& title ) 
{ 
  if ( object && hasPlotObject( object ) )
    getPlotObject( object )->setTitle( title );
}   

/*!
  Shows/hides legend
*/
void Plot2d_ViewFrame::showLegend( bool show, bool update )
{
  myShowLegend = show;
  if ( myShowLegend ) {
    QwtLegend* legend = myPlot->legend();
    if ( !legend ) {
      legend = new QwtLegend( myPlot );
      legend->setFrameStyle( QFrame::Box | QFrame::Sunken );      
    }
    legend->setItemMode( QwtLegend::ClickableItem );
    myPlot->insertLegend( legend );
    setLegendPos( myLegendPos );
    setLegendFont( myLegendFont );
    setLegendFontColor( myLegendColor );  
  }
  else
    myPlot->insertLegend( 0 );
  if ( update )
    myPlot->replot();
}

/*!
  Sets legend position : 0 - left, 1 - right, 2 - top, 3 - bottom
*/
void Plot2d_ViewFrame::setLegendPos( int pos )
{
  myLegendPos = pos;
  QwtLegend* legend = myPlot->legend();
  if ( legend ) {
    switch( pos ) {
    case 0:
      myPlot->insertLegend( legend, QwtPlot::LeftLegend );
      break;
    case 1:
      myPlot->insertLegend( legend, QwtPlot::RightLegend );
      break;
    case 2:
      myPlot->insertLegend( legend, QwtPlot::TopLegend );
      break;
    case 3:
      myPlot->insertLegend( legend, QwtPlot::BottomLegend );
      break;
    }
  }
}

/*!
  Gets legend position : 0 - left, 1 - right, 2 - top, 3 - bottom
*/
int Plot2d_ViewFrame::getLegendPos() const
{
  return myLegendPos;
}

/*!
  Sets legend font
*/
void Plot2d_ViewFrame::setLegendFont( const QFont& fnt )
{
  myLegendFont = fnt;
  QwtLegend* legend = myPlot->legend();
  if ( legend ) {
    legend->setFont(fnt);
  }
}

/*!
  Gets legend font
*/
QFont Plot2d_ViewFrame::getLegendFont() const
{
  return myLegendFont;
}

/*!
  Gets legend font color
*/
QColor Plot2d_ViewFrame::getLegendFontColor() const
{
  return myLegendColor;
}

/*!
  Sets legend font color
*/
void Plot2d_ViewFrame::setLegendFontColor( const QColor& col )
{
  myLegendColor = col;
  QwtLegend* legend = myPlot->legend();
  if ( legend ) {
    QPalette pal = legend->palette();
    pal.setColor( QPalette::Text, col );
    legend->setPalette( pal );
  }
}

/*!
  Sets new marker size
*/
void Plot2d_ViewFrame::setMarkerSize( const int size, bool update )
{
  if ( myMarkerSize != size )
  {
    myMarkerSize = size;
    CurveDict aCurves = getCurves();
    CurveDict::iterator it = aCurves.begin();
    for ( ; it != aCurves.end(); it++ ) {
      QwtPlotCurve* crv = it.key();
      if ( crv )
      {
        QwtSymbol aSymbol = crv->symbol();
        aSymbol.setSize( myMarkerSize, myMarkerSize );
        crv->setSymbol( aSymbol );
	if(it.value())
	  it.value()->setMarkerSize( myMarkerSize );
      }
    }
    if ( update )
      myPlot->replot();
  }
}

/*!
  Gets new marker size
*/
int Plot2d_ViewFrame::getMarkerSize() const
{
  return myMarkerSize;
}

/*!
  Sets background color
*/
void Plot2d_ViewFrame::setBackgroundColor( const QColor& color )
{
  myBackground = color;
  myPlot->canvas()->setPalette( myBackground );
  myPlot->setPalette( myBackground );
  if ( myPlot->getLegend() ) {
    QPalette aPal = myPlot->getLegend()->palette();
    for ( int i = 0; i < QPalette::NColorGroups; i++ ) {
      aPal.setColor( QPalette::Base, myBackground );
      aPal.setColor( QPalette::Background, myBackground );
    }
    myPlot->getLegend()->setPalette( aPal );
    updateLegend();
  }
  Repaint();
}
/*!
  Gets background color
*/
QColor Plot2d_ViewFrame::backgroundColor() const
{
  return myBackground;
}
/*!
  Sets hor.axis grid parameters
*/
void Plot2d_ViewFrame::setXGrid( bool xMajorEnabled, const int xMajorMax, 
         bool xMinorEnabled, const int xMinorMax, 
         bool update )
{
  myXGridMajorEnabled = xMajorEnabled;
  myXGridMinorEnabled = xMinorEnabled;
  myXGridMaxMajor = xMajorMax;
  myXGridMaxMinor = xMinorMax;

  myPlot->setAxisMaxMajor( QwtPlot::xBottom, myXGridMaxMajor );
  myPlot->setAxisMaxMinor( QwtPlot::xBottom, myXGridMaxMinor );

  QwtPlotGrid* grid = myPlot->grid();
  if ( myPlot->axisScaleDiv( QwtPlot::xBottom ) )
    grid->setXDiv( *myPlot->axisScaleDiv( QwtPlot::xBottom ) );
  grid->enableX( myXGridMajorEnabled );
  grid->enableXMin( myXGridMinorEnabled );

  if ( update )
    myPlot->replot();
}
/*!
  Sets ver.axis grid parameters
*/
void Plot2d_ViewFrame::setYGrid( bool yMajorEnabled, const int yMajorMax, 
                                 bool yMinorEnabled, const int yMinorMax,
                                 bool y2MajorEnabled, const int y2MajorMax, 
                                 bool y2MinorEnabled, const int y2MinorMax, 
                                 bool update )
{
  myYGridMajorEnabled = yMajorEnabled;
  myYGridMinorEnabled = yMinorEnabled;
  myYGridMaxMajor = yMajorMax;
  myYGridMaxMinor = yMinorMax;

  if (mySecondY) {
    myY2GridMajorEnabled = y2MajorEnabled;
    myY2GridMinorEnabled = y2MinorEnabled;
    myY2GridMaxMajor = y2MajorMax;
    myY2GridMaxMinor = y2MinorMax;
  }
  myPlot->setAxisMaxMajor( QwtPlot::yLeft, myYGridMaxMajor );
  myPlot->setAxisMaxMinor( QwtPlot::yLeft, myYGridMaxMinor );

  if (mySecondY) {
    myPlot->setAxisMaxMajor( QwtPlot::yRight, myY2GridMaxMajor );
    myPlot->setAxisMaxMinor( QwtPlot::yRight, myY2GridMaxMinor );
  }

  QwtPlotGrid* grid = myPlot->grid();
  if ( myPlot->axisScaleDiv( QwtPlot::yLeft ) )
    grid->setYDiv( *myPlot->axisScaleDiv( QwtPlot::yLeft ) );

  if (mySecondY) {
    if (myYGridMajorEnabled) {
      grid->enableY( myYGridMajorEnabled );
      grid->enableYMin( myYGridMinorEnabled );
    }
    else if (myY2GridMajorEnabled) {
      if ( myPlot->axisScaleDiv( QwtPlot::yRight ) )
        grid->setYDiv( *myPlot->axisScaleDiv( QwtPlot::yRight ) );
      grid->enableY( myY2GridMajorEnabled );
      grid->enableYMin( myY2GridMinorEnabled );
    }
    else {
      grid->enableY( false );
      grid->enableYMin( false );
    }
  }
  else {
    grid->enableY( myYGridMajorEnabled );
    grid->enableYMin( myYGridMinorEnabled );
  }
  if ( update )
    myPlot->replot();
}

/*!
  Sets title for some axis
*/
void Plot2d_ViewFrame::setTitle( bool enabled, const QString& title,
                                 ObjectType type, bool update )
{
  switch (type) {
    case MainTitle:
      myTitleEnabled = enabled;
      myTitle = title;
      myPlot->setTitle( myTitleEnabled ? myTitle : QString() );
      break;
    case XTitle:
      myXTitleEnabled = enabled;
      myXTitle = title;
      myPlot->setAxisTitle( QwtPlot::xBottom, myXTitleEnabled ? myXTitle : QString() );
      break;
    case YTitle:
      myYTitleEnabled = enabled;
      myYTitle = title;
      myPlot->setAxisTitle( QwtPlot::yLeft, myYTitleEnabled ? myYTitle : QString() );
      break;
    case Y2Title:
      myY2TitleEnabled = enabled;
      myY2Title = title;
      myPlot->setAxisTitle( QwtPlot::yRight, myY2TitleEnabled ? myY2Title : QString() );
      break;
    default:
      break;
  }
  if ( update )
    myPlot->replot();
}
/*!
  Sets title for some axis
*/
QString Plot2d_ViewFrame::getTitle( ObjectType type ) const
{
  QString title = "";
  switch (type) {
    case MainTitle:
      title = myTitle;   break;
    case XTitle:
      title = myXTitle;  break;
    case YTitle:
      title = myYTitle;  break;
    case Y2Title:
      title = myY2Title; break;
    default:
      break;
  }
  return title;
}
/*!
  Sets font for Plot2d object : title or axis
*/
void Plot2d_ViewFrame::setFont( const QFont& font, ObjectType type, bool update)
{
  switch (type) {
    case MainTitle:
      myPlot->title().setFont(font);
      break;
    case XTitle:
       myPlot->axisTitle(QwtPlot::xBottom).setFont(font); break;
    case YTitle:
      myPlot->axisTitle(QwtPlot::yLeft).setFont(font);    break;
    case Y2Title:
      myPlot->axisTitle(QwtPlot::yRight).setFont(font);   break;
    case XAxis:
      myPlot->setAxisFont(QwtPlot::xBottom, font);        break;
    case YAxis:
      myPlot->setAxisFont(QwtPlot::yLeft, font);          break;
    case Y2Axis:
      myPlot->setAxisFont(QwtPlot::yRight, font);         break;
  }
  if ( update )
    myPlot->replot();
}

/*!
  Sets scale mode for horizontal axis: 0 - linear, 1 - logarithmic
*/
void Plot2d_ViewFrame::setHorScaleMode( const int mode, bool update )
{
  if ( myXMode == mode )
    return;

  // san -- Protection against QwtCurve bug in Qwt 0.4.x: 
  // it crashes if switched to X/Y logarithmic mode, when one or more points have
  // non-positive X/Y coordinate
  if ( mode && !isXLogEnabled() ){
    SUIT_MessageBox::warning(this, tr("WARNING"), tr("WRN_XLOG_NOT_ALLOWED"));
    return;
  }

  myXMode = mode;

  myPlot->setLogScale(QwtPlot::xBottom, myXMode != 0);

  if ( update )
    fitAll();
  emit vpModeHorChanged();
}

/*!
  Gets scale mode for horizontal axis: 0 - linear, 1 - logarithmic
*/
int Plot2d_ViewFrame::getHorScaleMode() const
{
  return myXMode;
}

/*!
  Sets scale mode for vertical axis: 0 - linear, 1 - logarithmic
*/
void Plot2d_ViewFrame::setVerScaleMode( const int mode, bool update )
{
  if ( myYMode == mode )
    return;

  // san -- Protection against QwtCurve bug in Qwt 0.4.x: 
  // it crashes if switched to X/Y logarithmic mode, when one or more points have
  // non-positive X/Y coordinate
  if ( mode && !isYLogEnabled() ){
    SUIT_MessageBox::warning(this, tr("WARNING"), tr("WRN_YLOG_NOT_ALLOWED"));
    return;
  }

  myYMode = mode;
  myPlot->setLogScale(QwtPlot::yLeft, myYMode != 0);
  if (mySecondY)
    myPlot->setLogScale( QwtPlot::yRight, myYMode != 0 );

  if ( update )
    fitAll();
  emit vpModeVerChanged();
}

/*!
  Gets scale mode for vertical axis: 0 - linear, 1 - logarithmic
*/
int Plot2d_ViewFrame::getVerScaleMode() const
{
  return myYMode;
}

/*!
  Return, scale mode for horizontal axis
*/
bool Plot2d_ViewFrame::isModeHorLinear()
{
  return (myXMode == 0 ? true : false);
}

/*!
  Return, scale mode for vertical axis
*/
bool Plot2d_ViewFrame::isModeVerLinear()
{
  return (myYMode == 0 ? true : false);
}

/*!
  Return \c True if legend is shown
*/
bool Plot2d_ViewFrame::isLegendShow() const
{
  return myShowLegend;
}

/*!
  Slot, called when user presses mouse button
*/
void Plot2d_ViewFrame::plotMousePressed( const QMouseEvent& me )
{
  Plot2d_ViewWindow* aParent = dynamic_cast<Plot2d_ViewWindow*>(parent());
  if (aParent)
     aParent->putInfo(getInfo(me.pos()));
  if ( myOperation == NoOpId )
    myOperation = testOperation( me );
  if ( myOperation != NoOpId ) {
    myPnt = me.pos();
    if ( myOperation == GlPanId ) {
      myPlot->setAxisScale( QwtPlot::yLeft,
          myPlot->invTransform( QwtPlot::yLeft, myPnt.y() ) - myYDistance/2, 
          myPlot->invTransform( QwtPlot::yLeft, myPnt.y() ) + myYDistance/2 );
      myPlot->setAxisScale( QwtPlot::xBottom, 
          myPlot->invTransform( QwtPlot::xBottom, myPnt.x() ) - myXDistance/2, 
          myPlot->invTransform( QwtPlot::xBottom, myPnt.x() ) + myXDistance/2 );
      if (mySecondY)
        myPlot->setAxisScale( QwtPlot::yRight,
          myPlot->invTransform( QwtPlot::yRight, myPnt.y() ) - myYDistance2/2, 
          myPlot->invTransform( QwtPlot::yRight, myPnt.y() ) + myYDistance2/2 );
      myPlot->replot();
    }
  }
  else {
    int btn = me.button() | me.modifiers();
    if (btn == Qt::RightButton) {
      QMouseEvent* aEvent = new QMouseEvent(QEvent::MouseButtonPress,
                                            me.pos(), me.button(), me.buttons(), me.modifiers() );
      // QMouseEvent 'me' has the 'MouseButtonDblClick' type. In this case we create new event 'aEvent'.
      parent()->eventFilter(this, aEvent);
    }
  }
  setFocus(); 
}
/*!
  Slot, called when user moves mouse
*/
bool Plot2d_ViewFrame::plotMouseMoved( const QMouseEvent& me )
{
  int    dx = me.pos().x() - myPnt.x();
  int    dy = me.pos().y() - myPnt.y();

  bool aRes = false;
  if ( myOperation != NoOpId) {
    if ( myOperation == ZoomId ) {
      this->incrementalZoom( dx, dy ); 
      myPnt = me.pos();
      aRes = true;
    }
    else if ( myOperation == PanId ) {
      this->incrementalPan( dx, dy );
      myPnt = me.pos();
      aRes = true;
    }
  }
  else {
     Plot2d_ViewWindow* aParent = dynamic_cast<Plot2d_ViewWindow*>(parent());
     if (aParent)
       aParent->putInfo(getInfo(me.pos()));
  }
  return aRes;
}
/*!
  Slot, called when user releases mouse
*/
void Plot2d_ViewFrame::plotMouseReleased( const QMouseEvent& me )
{
  if ( myOperation == NoOpId && me.button() == Qt::RightButton && me.modifiers() != Qt::ControlModifier )
  {
    QContextMenuEvent aEvent( QContextMenuEvent::Mouse,
                              me.pos(), me.globalPos() );
    emit contextMenuRequested( &aEvent );
  }
  myPlot->canvas()->setCursor( QCursor( Qt::CrossCursor ) );
  myPlot->defaultPicker();

  Plot2d_ViewWindow* aParent = dynamic_cast<Plot2d_ViewWindow*>(parent());
   if (aParent)
     aParent->putInfo(tr("INF_READY"));
  myOperation = NoOpId;
}
/*!
  Slot, called when user wheeling mouse
*/
void Plot2d_ViewFrame::wheelEvent(QWheelEvent* event)
{ 
  double aDelta = event->delta();
  double aScale = (aDelta < 0) ? 100./(-aDelta) : aDelta/100.; 

  QwtScaleMap xMap = myPlot->canvasMap( QwtPlot::xBottom );
  QwtScaleMap yMap = myPlot->canvasMap( QwtPlot::yLeft );

  if ( ((yMap.s2() - yMap.s1()) < 10e-13 || (xMap.s2() - xMap.s1()) < 10e-13 ) && aScale < 1 )
    return;

  myPlot->setAxisScale( QwtPlot::yLeft, yMap.s1(), yMap.s1() + aScale*(yMap.s2() - yMap.s1()) );
  myPlot->setAxisScale( QwtPlot::xBottom, xMap.s1(), xMap.s1() + aScale*(xMap.s2() - xMap.s1()) );
  if (mySecondY) {
    QwtScaleMap y2Map = myPlot->canvasMap( QwtPlot::yRight );
    if ( ((y2Map.s2() - y2Map.s1()) < 10e-13  ) && aScale < 1 ) return;
    myPlot->setAxisScale( QwtPlot::yRight, y2Map.s1(), y2Map.s1() + aScale*(y2Map.s2() - y2Map.s1()) );
  }
  myPlot->replot();
  if ( myPlot->zoomer() ) myPlot->zoomer()->setZoomBase();
  myPnt = event->pos();
}

/*!
  Returns qwt plot curve if it is existed in map of curves and 0 otherwise
*/
QwtPlotCurve* Plot2d_ViewFrame::getPlotCurve( Plot2d_Curve* curve ) const
{
  return dynamic_cast<QwtPlotCurve*>( getPlotObject( curve ) );
}
/*!
  Returns true if qwt plot curve is existed in map of curves and false otherwise
*/
bool Plot2d_ViewFrame::hasPlotCurve( Plot2d_Curve* curve ) const
{
  return hasPlotObject( curve );
}

/*!
  Returns qwt plot curve if it is existed in map of curves and 0 otherwise
*/
QwtPlotItem* Plot2d_ViewFrame::getPlotObject( Plot2d_Object* object ) const
{
  ObjectDict::const_iterator it = myObjects.begin();
  for ( ; it != myObjects.end(); it++ ) {
    if ( it.value() == object )
      return it.key();
  }
  return 0;
}
/*!
  Returns true if qwt plot curve is existed in map of curves and false otherwise
*/
bool Plot2d_ViewFrame::hasPlotObject( Plot2d_Object* object ) const
{
  ObjectDict::const_iterator it = myObjects.begin();
  for ( ; it != myObjects.end(); it++ ) {
    if ( it.value() == object )
      return true;
  }
  return false;
}

/*!
  Sets curve type
*/
void Plot2d_ViewFrame::setCurveType( QwtPlotCurve* curve, int curveType )
{
  if ( !curve )
    return;
  if ( myCurveType == 0 )
    curve->setStyle( QwtPlotCurve::Dots );//QwtCurve::NoCurve
  else if ( myCurveType == 1 ) {
    curve->setStyle( QwtPlotCurve::Lines );
    curve->setCurveAttribute( QwtPlotCurve::Fitted, false );
  }
  else if ( myCurveType == 2 ) {
    curve->setStyle( QwtPlotCurve::Lines );
    QwtSplineCurveFitter* fitter = new QwtSplineCurveFitter();
    fitter->setSplineSize( 250 );
    curve->setCurveAttribute( QwtPlotCurve::Fitted, true );
    curve->setCurveFitter( fitter );
  }
}

/*!
  View operations : Pan view
*/
void Plot2d_ViewFrame::onViewPan()
{ 
  QCursor panCursor (Qt::SizeAllCursor);
  myPlot->canvas()->setCursor( panCursor );
  myOperation = PanId;
}
/*!
  View operations : Zoom view
*/
void Plot2d_ViewFrame::onViewZoom() 
{
  QPixmap zoomPixmap (imageZoomCursor);
  QCursor zoomCursor (zoomPixmap);
  myPlot->canvas()->setCursor( zoomCursor );
  myOperation = ZoomId;
}
/*!
  View operations : Fot All
*/
void Plot2d_ViewFrame::onViewFitAll() 
{ 
  fitAll();
}
/*!
  View operations : Fit Area
*/
void Plot2d_ViewFrame::onViewFitArea() 
{
  myPlot->canvas()->setCursor( QCursor( Qt::PointingHandCursor ) );
  myOperation = FitAreaId;
  myPlot->setPickerMousePattern( Qt::LeftButton );
}
/*!
  View operations : Global panning
*/
void Plot2d_ViewFrame::onViewGlobalPan() 
{
  QPixmap globalPanPixmap (imageCrossCursor);
  QCursor glPanCursor (globalPanPixmap);
  myPlot->canvas()->setCursor( glPanCursor );
  myPlot->setLogScale(QwtPlot::xBottom, false);
  myPlot->setLogScale(QwtPlot::yLeft, false);
  if (mySecondY)
    myPlot->setLogScale(QwtPlot::yRight, false);
  myPlot->replot();
  QwtScaleMap xMap = myPlot->canvasMap( QwtPlot::xBottom );
  QwtScaleMap yMap = myPlot->canvasMap( QwtPlot::yLeft );

  myXDistance = xMap.s2() - xMap.s1();
  myYDistance = yMap.s2() - yMap.s1();

  if (mySecondY) {
    QwtScaleMap yMap2 = myPlot->canvasMap( QwtPlot::yRight );
    myYDistance2 = yMap2.s2() - yMap2.s1();
  }
  fitAll();
  myOperation = GlPanId;
}

/*!
  Precaution for logarithmic X scale
*/
bool Plot2d_ViewFrame::isXLogEnabled() const
{
  bool allPositive = true;
  ObjectDict::const_iterator it = myObjects.begin();
  for ( ; allPositive && it != myObjects.end(); it++ )
    allPositive = ( it.value()->getMinX() > 0. );
  return allPositive;
}

/*!
  Precaution for logarithmic Y scale
*/
bool Plot2d_ViewFrame::isYLogEnabled() const
{
  bool allPositive = true;
  ObjectDict::const_iterator it = myObjects.begin();
  for ( ; allPositive && it != myObjects.end(); it++ )
    allPositive = ( it.value()->getMinY() > 0. );
  return allPositive;
}

/**
 *
 */
void Plot2d_ViewFrame::setEnableAxis( QwtPlot::Axis theAxis, bool isEnable )
{
  if ( myPlot->axisEnabled( theAxis ) == isEnable )
    return;
  myPlot->enableAxis( theAxis, isEnable );
  if ( theAxis == QwtPlot::yRight )
    mySecondY = isEnable;
}

class Plot2d_QwtPlotZoomer : public QwtPlotZoomer
{
public:
  Plot2d_QwtPlotZoomer( int xAxis, int yAxis, QwtPlotCanvas* canvas )
  : QwtPlotZoomer( xAxis, yAxis, canvas )
  {
    qApp->installEventFilter( this );
    // now picker working after only a button pick.
    // after click on button FitArea in toolbar of the ViewFrame.
  };
  ~Plot2d_QwtPlotZoomer() {};
};

/*!
  Constructor
*/
Plot2d_Plot2d::Plot2d_Plot2d( QWidget* parent )
  : QwtPlot( parent ),
    myIsPolished( false )
{
  // Create alternative scales
  setAxisScaleDraw( QwtPlot::yLeft,   new Plot2d_ScaleDraw() );
  setAxisScaleDraw( QwtPlot::xBottom, new Plot2d_ScaleDraw() );
  setAxisScaleDraw( QwtPlot::yRight,  new Plot2d_ScaleDraw() );

  myPlotZoomer = new Plot2d_QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft, canvas() );
  myPlotZoomer->setSelectionFlags( QwtPicker::DragSelection | QwtPicker::CornerToCorner );
  myPlotZoomer->setTrackerMode( QwtPicker::AlwaysOff );
  myPlotZoomer->setRubberBand( QwtPicker::RectRubberBand );
  myPlotZoomer->setRubberBandPen( QColor( Qt::green ) );

  defaultPicker();

  // auto scaling by default
  setAxisAutoScale( QwtPlot::yLeft );
  setAxisAutoScale( QwtPlot::yRight );
  setAxisAutoScale( QwtPlot::xBottom );

// grid
  myGrid = new QwtPlotGrid();
  QPen aMajPen = myGrid->majPen();
  aMajPen.setStyle( Qt::DashLine );
  myGrid->setPen( aMajPen );

  myGrid->enableX( false );
  myGrid->enableXMin( false );
  myGrid->enableY( false );
  myGrid->enableYMin( false );

  myGrid->attach( this );

  setMouseTracking( false );
  canvas()->setMouseTracking( true );

  myPlotZoomer->setEnabled( true );
  myPlotZoomer->setZoomBase();

  setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred ) );
}

Plot2d_Plot2d::~Plot2d_Plot2d()
{
}

/*!
  \set axis scale engine - linear or log10
*/
void Plot2d_Plot2d::setLogScale( int axisId, bool log10 )
{
  if ( log10 )
    setAxisScaleEngine( axisId, new QwtLog10ScaleEngine() );
  else
    setAxisScaleEngine( axisId, new QwtLinearScaleEngine() );
}

/*!
  Recalculates and redraws Plot 2d view 
*/
void Plot2d_Plot2d::replot()
{
  // the following code is intended to enable only axes
  // that are really used by displayed objects
  bool enableXBottom = false, enableXTop   = false;
  bool enableYLeft   = false, enableYRight = false;
  const QwtPlotItemList& items = itemList();
  QwtPlotItemIterator it;
  for ( it = items.begin(); it != items.end(); it++ ) {
    QwtPlotItem* item = *it;
    if ( item ) {
      enableXBottom |= item->xAxis() == QwtPlot::xBottom;
      enableXTop    |= item->xAxis() == QwtPlot::xTop;
      enableYLeft   |= item->yAxis() == QwtPlot::yLeft;
      enableYRight  |= item->yAxis() == QwtPlot::yRight;
    }
  }
  enableAxis( QwtPlot::xBottom, enableXBottom );
  enableAxis( QwtPlot::xTop,    enableXTop );
  enableAxis( QwtPlot::yLeft,   enableYLeft );
  enableAxis( QwtPlot::yRight,  enableYRight );

  updateLayout();  // to fix bug(?) of Qwt - view is not updated when title is changed
  QwtPlot::replot(); 
}

/*!
  Get legend
*/
QwtLegend* Plot2d_Plot2d::getLegend()
{
#if QWT_VERSION < 0x040200
  return d_legend;
#else  
  return legend(); /* mpv: porting to the Qwt 4.2.0 */
#endif
}

/*!
  \return the recommended size for the widget
*/
QSize Plot2d_Plot2d::sizeHint() const
{
  return QwtPlot::minimumSizeHint();
}

/*!
  return minimum size for qwt plot
*/
QSize Plot2d_Plot2d::minimumSizeHint() const
{
  return QSize( 0, 0 );
//  QSize aSize = QwtPlot::minimumSizeHint();
//  return QSize(aSize.width()*3/4, aSize.height());
}

void Plot2d_Plot2d::defaultPicker()
{
  myPlotZoomer->setMousePattern( QwtEventPattern::MouseSelect1,
                                 Qt::RightButton, Qt::ControlModifier ); // zooming button
  for ( int i = QwtEventPattern::MouseSelect2; i < QwtEventPattern::MouseSelect6; i++ )
    myPlotZoomer->setMousePattern( i, Qt::NoButton, Qt::NoButton );
}

void Plot2d_Plot2d::setPickerMousePattern( int button, int state )
{
  myPlotZoomer->setMousePattern( QwtEventPattern::MouseSelect1, button, state );
}

bool Plot2d_Plot2d::polished() const
{
  return myIsPolished;
}

QwtPlotGrid* Plot2d_Plot2d::grid() const
{
  return myGrid;
};

QwtPlotZoomer* Plot2d_Plot2d::zoomer() const
{
  return myPlotZoomer;
}

/*!
  Slot: checks the current labels format and change it if needed
*/
void Plot2d_Plot2d::onScaleDivChanged()
{
  QwtScaleWidget* aSW = 0;
  if ( ( aSW = dynamic_cast<QwtScaleWidget*>(sender()) ) ) {
    int axisId = -1;
    switch ( aSW->alignment() ) {
    case QwtScaleDraw::BottomScale:
      axisId = QwtPlot::xBottom;
      break;
    case QwtScaleDraw::LeftScale:
      axisId = QwtPlot::yLeft;
      break;
    case QwtScaleDraw::RightScale:
      axisId = QwtPlot::yRight;
      break;
    default:
      break;
    }

    if ( axisId >= 0 ) {
      QwtScaleMap map = canvasMap(axisId);
      double aDist = fabs(map.s2()-map.s1()) / (axisMaxMajor(axisId)*axisMaxMinor(axisId));

      QString aDistStr;
      aDistStr.sprintf("%e",aDist);
      int aPrecision = aDistStr.right(aDistStr.length()-aDistStr.indexOf('e')-2).toInt();

      QwtScaleDraw* aQwtSD = axisScaleDraw(axisId);
      Plot2d_ScaleDraw* aPlot2dSD = dynamic_cast<Plot2d_ScaleDraw*>(aQwtSD);
      if ( ( !aPlot2dSD && aPrecision > 6 ) || ( aPlot2dSD && aPlot2dSD->precision() != aPrecision ) )
        setAxisScaleDraw( axisId, new Plot2d_ScaleDraw(*aQwtSD, 'f', aPrecision) );
    }
  }
}

/*!
  Updates identifiers of Y axis type in the legend.
*/
void Plot2d_Plot2d::updateYAxisIdentifiers()
{
  bool enableYLeft = false, enableYRight = false;
  const QwtPlotItemList& items = itemList();
  QwtPlotItemIterator it;
  for ( it = items.begin(); it != items.end(); it++ ) {
    QwtPlotItem* item = *it;
    if ( item ) {
      enableYLeft  |= item->yAxis() == QwtPlot::yLeft;
      enableYRight |= item->yAxis() == QwtPlot::yRight;
    }
  }

  // if several curves are attached to different axes
  // display corresponding identifiers in the legend,
  // otherwise hide them
  for ( it = items.begin(); it != items.end(); it++ ) {
    QwtPlotItem* item = *it;
    if ( Plot2d_QwtPlotCurve* aPCurve = dynamic_cast<Plot2d_QwtPlotCurve*>( item ) )
      aPCurve->setYAxisIdentifierEnabled( enableYLeft && enableYRight );
    if ( item && item->isVisible() && legend() )
      item->updateLegend( legend() );
  }
}

/*!
  Sets the flag saying that QwtPlot geometry has been fully defined.
*/
void Plot2d_Plot2d::polish()
{
  QwtPlot::polish();
  myIsPolished = true;
}


/*!
  Creates presentation of object
  Default implementation is empty
*/
Plot2d_Prs* Plot2d_ViewFrame::CreatePrs( const char* /*entry*/ )
{
  return 0;
}

/*!
  Copies preferences from other viewframe
  \param vf - other view frame
*/
void Plot2d_ViewFrame::copyPreferences( Plot2d_ViewFrame* vf )
{
  if( !vf )
    return;

  myCurveType = vf->myCurveType;
  myShowLegend = vf->myShowLegend;
  myLegendPos = vf->myLegendPos;
  myMarkerSize = vf->myMarkerSize;
  myBackground = vf->myBackground;
  myTitle = vf->myTitle; 
  myXTitle = vf->myXTitle;
  myYTitle = vf->myYTitle;
  myY2Title = vf->myY2Title;
  myTitleEnabled = vf->myTitleEnabled;
  myXTitleEnabled = vf->myXTitleEnabled;
  myYTitleEnabled = vf->myYTitleEnabled;
  myY2TitleEnabled = vf->myY2TitleEnabled;
  myXGridMajorEnabled = vf->myXGridMajorEnabled;
  myYGridMajorEnabled = vf->myYGridMajorEnabled;
  myY2GridMajorEnabled = vf->myY2GridMajorEnabled;
  myXGridMinorEnabled = vf->myXGridMinorEnabled;
  myYGridMinorEnabled = vf->myYGridMinorEnabled;
  myY2GridMinorEnabled = vf->myY2GridMinorEnabled;
  myXGridMaxMajor = vf->myXGridMaxMajor;
  myYGridMaxMajor = vf->myYGridMaxMajor;
  myY2GridMaxMajor = vf->myY2GridMaxMajor;
  myXGridMaxMinor = vf->myXGridMaxMinor;
  myYGridMaxMinor = vf->myYGridMaxMinor;
  myY2GridMaxMinor = vf->myY2GridMaxMinor;
  myXMode = vf->myXMode;
  myYMode = vf->myYMode;
  mySecondY = vf->mySecondY;
}

/*!
  Updates titles according to curves
*/
#define BRACKETIZE(x) QString( "[ " ) + x + QString( " ]" )
void Plot2d_ViewFrame::updateTitles() 
{
  ObjectDict::iterator it = myObjects.begin();
  QStringList aXTitles;
  QStringList aYTitles;
  QStringList aY2Titles;
  QStringList aXUnits;
  QStringList aYUnits;
  QStringList aY2Units;
  QStringList aTables;
  int i = 0;

  Plot2d_Object* anObject;
  for ( ; it != myObjects.end(); it++ ) {
    // collect titles and units from all curves...
    anObject = it.value();
    QString xTitle = anObject->getHorTitle().trimmed();
    QString yTitle = anObject->getVerTitle().trimmed();
    QString xUnits = anObject->getHorUnits().trimmed();
    QString yUnits = anObject->getVerUnits().trimmed();
    
    if ( anObject->getYAxis() == QwtPlot::yLeft ) {
      if ( !aYTitles.contains( yTitle ) )
        aYTitles.append( yTitle );
      if ( !aYUnits.contains( yUnits ) )
        aYUnits.append( yUnits );
    }
    else {
      if ( !aY2Titles.contains( yTitle ) )
        aY2Titles.append( yTitle );
      if ( !aY2Units.contains( yUnits ) )
        aY2Units.append( yUnits );
    }
    if ( !aXTitles.contains( xTitle ) )
      aXTitles.append( xTitle );
    if ( !aXUnits.contains( xUnits ) )
      aXUnits.append( xUnits );

    QString aName = anObject->getTableTitle();
    if( !aName.isEmpty() && !aTables.contains( aName ) )
      aTables.append( aName );
    ++i;
  }
  // ... and update plot 2d view
  QString xUnits, yUnits, y2Units;
  if ( aXUnits.count() == 1 && !aXUnits[0].isEmpty() )
    xUnits = BRACKETIZE( aXUnits[0] );
  if ( aYUnits.count() == 1 && !aYUnits[0].isEmpty())
    yUnits = BRACKETIZE( aYUnits[0] );
  if ( aY2Units.count() == 1 && !aY2Units[0].isEmpty())
    y2Units = BRACKETIZE( aY2Units[0] );
  QString xTitle, yTitle, y2Title;
  if ( aXTitles.count() == 1 && aXUnits.count() == 1 )
    xTitle = aXTitles[0];
  if ( aYTitles.count() == 1 )
    yTitle = aYTitles[0];
  if ( mySecondY && aY2Titles.count() == 1 )
    y2Title = aY2Titles[0];

  if ( !xTitle.isEmpty() && !xUnits.isEmpty() )
    xTitle += " ";
  if ( !yTitle.isEmpty() && !yUnits.isEmpty() )
    yTitle += " ";
  if ( !y2Title.isEmpty() && !y2Units.isEmpty() )
    y2Title += " ";

  setTitle( myXTitleEnabled, xTitle + xUnits, XTitle, true );
  setTitle( myYTitleEnabled, yTitle + yUnits, YTitle, true );
  if ( mySecondY )
    setTitle( myY2TitleEnabled, y2Title + y2Units, Y2Title, true );
  if( myIsDefTitle ) 
    setTitle( true, aTables.join("; "), MainTitle, true );
}

/*!
  Outputs content of viewframe to file
  \param file - file name
  \param format - file format
*/
bool Plot2d_ViewFrame::print( const QString& file, const QString& format ) const
{
#ifdef WIN32
  return false;

#else
  bool res = false;
  if( myPlot )
  {
    QPaintDevice* pd = 0;
    if( format=="PS" || format=="EPS" )
    {
      QPrinter* pr = new QPrinter( QPrinter::HighResolution );
      pr->setPageSize( QPrinter::A4 );
      pr->setOutputFileName( file );
      pr->setPrintProgram( "" );
      pd = pr;
    }

    if( pd )
    {
      myPlot->print( *pd );
      res = true;
      delete pd;
    }
  }
  return res;
#endif
}

/**
 * Print Plot2d window
 */
void Plot2d_ViewFrame::printPlot( QPainter* p, const QRect& rect,
                                  const QwtPlotPrintFilter& filter ) const
{
  myPlot->print( p, rect, filter );
}

/*!
  \return string with all visual parameters
*/
QString Plot2d_ViewFrame::getVisualParameters()
{
  double xmin, xmax, ymin, ymax, y2min, y2max;
  getFitRanges( xmin, xmax, ymin, ymax, y2min, y2max );
  QString retStr;
  retStr.sprintf( "%d*%d*%d*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e", myXMode,
                  myYMode, mySecondY, xmin, xmax, ymin, ymax, y2min, y2max );
  return retStr; 
}

/*!
  Restores all visual parameters from string
*/
void Plot2d_ViewFrame::setVisualParameters( const QString& parameters )
{
  QStringList paramsLst = parameters.split( '*' );
  if ( paramsLst.size() == 9 ) {
    double xmin, xmax, ymin, ymax, y2min, y2max;
    myXMode = paramsLst[0].toInt();
    myYMode = paramsLst[1].toInt();
    mySecondY = (bool)paramsLst[2].toInt();
    xmin =  paramsLst[3].toDouble();
    xmax =  paramsLst[4].toDouble();
    ymin =  paramsLst[5].toDouble();
    ymax =  paramsLst[6].toDouble();
    y2min = paramsLst[7].toDouble();
    y2max = paramsLst[8].toDouble();

    if (mySecondY)
      setTitle( myY2TitleEnabled, myY2Title, Y2Title, false );
    setHorScaleMode( myXMode, /*update=*/false );
    setVerScaleMode( myYMode, /*update=*/false );
    
    if (mySecondY) {
      QwtScaleMap yMap2 = myPlot->canvasMap( QwtPlot::yRight );
      myYDistance2 = yMap2.s2() - yMap2.s1();
    }

    fitData( 0, xmin, xmax, ymin, ymax, y2min, y2max );
    fitData( 0, xmin, xmax, ymin, ymax, y2min, y2max );
  }  
}

/*!
  Incremental zooming operation
*/
void Plot2d_ViewFrame::incrementalPan( const int incrX, const int incrY ) {
  QwtScaleMap xMap = myPlot->canvasMap( QwtPlot::xBottom );
  QwtScaleMap yMap = myPlot->canvasMap( QwtPlot::yLeft );
  
  myPlot->setAxisScale( QwtPlot::yLeft, 
                        myPlot->invTransform( QwtPlot::yLeft, yMap.transform( yMap.s1() )-incrY ), 
                        myPlot->invTransform( QwtPlot::yLeft, yMap.transform( yMap.s2() )-incrY ) );
  myPlot->setAxisScale( QwtPlot::xBottom, 
                        myPlot->invTransform( QwtPlot::xBottom, xMap.transform( xMap.s1() )-incrX ),
                        myPlot->invTransform( QwtPlot::xBottom, xMap.transform( xMap.s2() )-incrX ) ); 
  if (mySecondY) {
    QwtScaleMap y2Map = myPlot->canvasMap( QwtPlot::yRight );
    myPlot->setAxisScale( QwtPlot::yRight,
                          myPlot->invTransform( QwtPlot::yRight, y2Map.transform( y2Map.s1() )-incrY ), 
                          myPlot->invTransform( QwtPlot::yRight, y2Map.transform( y2Map.s2() )-incrY ) );
  }
  myPlot->replot();
}

/*!
  Incremental panning operation
*/
void Plot2d_ViewFrame::incrementalZoom( const int incrX, const int incrY ) {
  QwtScaleMap xMap = myPlot->canvasMap( QwtPlot::xBottom );
  QwtScaleMap yMap = myPlot->canvasMap( QwtPlot::yLeft );
  
  myPlot->setAxisScale( QwtPlot::yLeft, yMap.s1(), 
                        myPlot->invTransform( QwtPlot::yLeft, yMap.transform( yMap.s2() ) + incrY ) );
  myPlot->setAxisScale( QwtPlot::xBottom, xMap.s1(), 
                        myPlot->invTransform( QwtPlot::xBottom, xMap.transform( xMap.s2() ) - incrX ) );
  if (mySecondY) {
    QwtScaleMap y2Map = myPlot->canvasMap( QwtPlot::yRight );
    myPlot->setAxisScale( QwtPlot::yRight, y2Map.s1(),
                          myPlot->invTransform( QwtPlot::yRight, y2Map.transform( y2Map.s2() ) + incrY ) );
  }
  myPlot->replot();
}

/**
 *
 */
QwtPlotCanvas* Plot2d_ViewFrame::getPlotCanvas() const
{
  return myPlot ? myPlot->canvas() : 0;
}

/*!
  return closest curve if it exist, else 0
*/
Plot2d_Curve* Plot2d_ViewFrame::getClosestCurve( QPoint p, double& distance, int& index ) const
{
  CurveDict aCurves = getCurves();
  CurveDict::iterator it = aCurves.begin();
  QwtPlotCurve* aCurve;
  for ( ; it != aCurves.end(); it++ ) {
    aCurve = it.key();
    if ( !aCurve )
      continue;
    index = aCurve->closestPoint( p, &distance );
    if ( index > -1 )
      return it.value();
  }
  return 0;
}

#define INCREMENT_FOR_OP 10

/*!
  Performs incremental panning to the left
*/
void Plot2d_ViewFrame::onPanLeft()
{
  this->incrementalPan( -INCREMENT_FOR_OP, 0 );
}

/*!
  Performs incremental panning to the right
*/
void Plot2d_ViewFrame::onPanRight()
{
  this->incrementalPan( INCREMENT_FOR_OP, 0 );
}

/*!
  Performs incremental panning to the top
*/
void Plot2d_ViewFrame::onPanUp()
{
  this->incrementalPan( 0, -INCREMENT_FOR_OP );
}

/*!
  Performs incremental panning to the bottom
*/
void Plot2d_ViewFrame::onPanDown()
{
  this->incrementalPan( 0, INCREMENT_FOR_OP );
}

/*!
  Performs incremental zooming in
*/
void Plot2d_ViewFrame::onZoomIn()
{
  this->incrementalZoom( INCREMENT_FOR_OP, INCREMENT_FOR_OP );
}

/*!
  Performs incremental zooming out
*/
void Plot2d_ViewFrame::onZoomOut()
{
  this->incrementalZoom( -INCREMENT_FOR_OP, -INCREMENT_FOR_OP );
}

/*!
  Schedules a FitAll operation by putting it to the application's
  event queue. This ensures that other important events (show, resize, etc.)
  are processed first.
*/
void Plot2d_ViewFrame::customEvent( QEvent* ce )
{
  if ( ce->type() == FITALL_EVENT )
    fitAll();
}


/*!
 * Return Plot2d_Object by the QwtPlotItem
 *
*/
Plot2d_Object* Plot2d_ViewFrame::getPlotObject( QwtPlotItem* plotItem ) const {
  
  ObjectDict::const_iterator it = myObjects.begin();
  for( ; it != myObjects.end(); ++it ) {
    if ( it.key() == plotItem ) {
      return it.value();
    }
  }
  return 0;
}

Plot2d_ScaleDraw::Plot2d_ScaleDraw( char f, int prec )
  : QwtScaleDraw(),
    myFormat(f),
    myPrecision(prec)
{
  invalidateCache();
}

Plot2d_ScaleDraw::Plot2d_ScaleDraw( const QwtScaleDraw& scaleDraw, char f, int prec )
  : QwtScaleDraw(scaleDraw),
    myFormat(f),
    myPrecision(prec)
{
  invalidateCache();
}

QwtText Plot2d_ScaleDraw::label( double value ) const
{
  QwtScaleMap m = map();
  QString str1 = QwtScaleDraw::label( m.s1() ).text();
  QString str2 = QwtScaleDraw::label( m.s2() ).text();
  if ( str1 == str2 ) {
    double aDist = fabs(map().s2()-map().s1())/5;
    int precision = 0;
    while (aDist < 1 ) {
      precision++; 
      aDist *= 10.; 
    }
    if ( precision > 0 && value > 0 )
      return QLocale::system().toString( value,'f', precision );
  }

  return QwtScaleDraw::label( value );
}


