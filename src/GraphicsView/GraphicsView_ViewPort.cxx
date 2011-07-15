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
// File:   GraphicsView_ViewPort.cxx
// Author: Oleg UVAROV, Open CASCADE S.A.S. (oleg.uvarov@opencascade.com)
//

#include "GraphicsView_ViewPort.h"

#include "GraphicsView_Object.h"
#include "GraphicsView_Scene.h"
#include "GraphicsView_ViewTransformer.h"

#include "SUIT_ResourceMgr.h"
#include "SUIT_Session.h"

#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGridLayout>
#include <QLabel>
#include <QMoveEvent>
#include <QRectF>
#include <QRubberBand>
#include <QScrollBar>

#include <math.h>

int GraphicsView_ViewPort::nCounter = 0;
QCursor* GraphicsView_ViewPort::defCursor = 0;
QCursor* GraphicsView_ViewPort::handCursor = 0;
QCursor* GraphicsView_ViewPort::panCursor = 0;
QCursor* GraphicsView_ViewPort::panglCursor = 0;
QCursor* GraphicsView_ViewPort::zoomCursor = 0;

//=======================================================================
// Name    : GraphicsView_ViewPort::NameLabel
// Purpose : Wrapper for label, which can ignore move events sent from
//           QGraphicsView::scrollContentsBy() method, which,
//           in its turn, called from GraphicsView_ViewPort::pan()
//=======================================================================
class GraphicsView_ViewPort::NameLabel : public QLabel
{
public:
  NameLabel( QWidget* theParent ) 
    : 
  QLabel( theParent ),
  myAcceptMoveEvents( false )
  {
  }
  ~NameLabel() {}

  void setAcceptMoveEvents( bool theFlag )
  {
    myAcceptMoveEvents = theFlag;
  }

protected:
  virtual void moveEvent( QMoveEvent* theEvent )
  {
    if( myAcceptMoveEvents )
      QLabel::moveEvent( theEvent );
    else // return the label to the initial position
    {
      myAcceptMoveEvents = true;
      move( theEvent->oldPos() );
      myAcceptMoveEvents = false;
    }
  }

private:
  bool myAcceptMoveEvents;
};

//================================================================
// Function : createCursors
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::createCursors ()
{
  defCursor   = new QCursor( Qt::ArrowCursor );
  handCursor  = new QCursor( Qt::PointingHandCursor );
  panCursor   = new QCursor( Qt::SizeAllCursor );
  panglCursor = new QCursor( Qt::CrossCursor );

  SUIT_ResourceMgr* rmgr = SUIT_Session::session()->resourceMgr();
  zoomCursor   = new QCursor( rmgr->loadPixmap( "GraphicsView", tr( "ICON_GV_CURSOR_ZOOM" ) ) );
}

//================================================================
// Function : destroyCursors
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::destroyCursors()
{
  delete defCursor;   defCursor   = 0;
  delete handCursor;  handCursor  = 0;
  delete panCursor;   panCursor   = 0;
  delete panglCursor; panglCursor = 0;
  delete zoomCursor;  zoomCursor  = 0;
}

//=======================================================================
// Name    : GraphicsView_ViewPort
// Purpose : Constructor
//=======================================================================
GraphicsView_ViewPort::GraphicsView_ViewPort( QWidget* theParent )
: QGraphicsView( theParent ),
  myNameLabel( 0 ),
  myNamePosition( NP_None ),
  myNameLayout( 0 ),
  myForegroundItem( 0 ),
  myGridItem( 0 ),
  myIsTransforming( false ),
  myHighlightedObject( 0 ),
  myHighlightX( 0 ),
  myHighlightY( 0 ),
  mySelectionIterator( 0 ),
  myRectBand( 0 ),
  myAreSelectionPointsInitialized( false ),
  myIsDragging( false ),
  myIsDragPositionInitialized( false ),
  myIsPulling( false ),
  myPullingObject( 0 )
{
  // scene
  myScene = new GraphicsView_Scene( this );
  setScene( myScene );

  mySceneGap = 20;
  myFitAllGap = 40;
  myIsTraceBoundingRectEnabled = true;

  // interaction flags
  myInteractionFlags = All;

  // background
  setBackgroundBrush( QBrush( Qt::white ) );

  // foreground
  myIsForegroundEnabled = false;
  myForegroundSize = QSizeF( 100, 30 );
  myForegroundMargin = 0.0;
  myForegroundColor = Qt::white;
  myForegroundFrameColor = Qt::black;
  myForegroundFrameLineWidth = 1.0;

  // grid
  myIsGridEnabled = false;
  myGridCellSize = 100;
  myGridLineStyle = Qt::DotLine;
  myGridLineColor = Qt::darkGray;

  // default index method (BspTreeIndex) leads to
  // crashes in QGraphicsView::paintEvent() method
  myScene->setItemIndexMethod( QGraphicsScene::NoIndex );

  // render hints (default - TextAntialiasing only)
  setRenderHints( QPainter::Antialiasing |
                  QPainter::TextAntialiasing |
                  QPainter::SmoothPixmapTransform |
                  QPainter::HighQualityAntialiasing );

  connect( myScene, SIGNAL( gsKeyEvent( QKeyEvent* ) ),
           this, SLOT( onKeyEvent( QKeyEvent* ) ) );
  connect( myScene, SIGNAL( gsMouseEvent( QGraphicsSceneMouseEvent* ) ),
           this, SLOT( onMouseEvent( QGraphicsSceneMouseEvent* ) ) );
  connect( myScene, SIGNAL( gsWheelEvent( QGraphicsSceneWheelEvent* ) ),
           this, SLOT( onWheelEvent( QGraphicsSceneWheelEvent* ) ) );
  connect( myScene, SIGNAL( gsContextMenuEvent( QGraphicsSceneContextMenuEvent* ) ),
           this, SLOT( onContextMenuEvent( QGraphicsSceneContextMenuEvent* ) ) );

  connect( myScene, SIGNAL( gsBoundingRectChanged() ),
           this, SLOT( onBoundingRectChanged() ) );

  initialize();
}

//=======================================================================
// Name    : GraphicsView_ViewPort
// Purpose : Destructor
//=======================================================================
GraphicsView_ViewPort::~GraphicsView_ViewPort()
{
  cleanup();

  if( myScene )
  {
    delete myScene;
    myScene = 0;
  }
}

//================================================================
// Function : initialize
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::initialize()
{
  if ( nCounter++ == 0 )
    createCursors();

  setMouseTracking( true );
  setFocusPolicy( Qt::StrongFocus );
}

//================================================================
// Function : cleanup
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::cleanup()
{
  if ( --nCounter == 0 )
    destroyCursors();
}

//================================================================
// Function : addItem
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::addItem( QGraphicsItem* theItem )
{
  if( GraphicsView_Object* anObject = dynamic_cast<GraphicsView_Object*>( theItem ) )
  {
    int aPriority = anObject->getPriority();
    GraphicsView_ObjectList::iterator anIter, anIterEnd = myObjects.end();
    for( anIter = myObjects.begin(); anIter != anIterEnd; anIter++ )
    {
      if( GraphicsView_Object* anObjectRef = *anIter )
      {
        if( anObjectRef->getPriority() > aPriority )
          break;
      }
    }
    myObjects.insert( anIter, anObject );
  }
  myScene->addItem( theItem );
  onBoundingRectChanged();
}

//================================================================
// Function : removeItem
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::removeItem( QGraphicsItem* theItem )
{
  if( GraphicsView_Object* anObject = dynamic_cast<GraphicsView_Object*>( theItem ) )
  {
    if( myHighlightedObject == anObject )
      myHighlightedObject = 0;
    mySelectedObjects.removeAll( anObject );
    myObjects.removeAll( anObject );
  }
  myScene->removeItem( theItem );
  onBoundingRectChanged();
}

//================================================================
// Function : getObjects
// Purpose  : 
//================================================================
GraphicsView_ObjectList GraphicsView_ViewPort::getObjects( bool theIsSortSelected ) const
{
  if( !theIsSortSelected )
    return myObjects;

  // to append selected objects after their non-selected siblings with similar priority
  int aCurrentPriority = -1;
  GraphicsView_ObjectList aSelectedObjects;
  GraphicsView_ObjectList aTopmostObjects;

  GraphicsView_ObjectList aList;
  GraphicsView_ObjectListIterator anIter( myObjects );
  while( anIter.hasNext() )
  {
    if( GraphicsView_Object* anObject = anIter.next() )
    {
      if( anObject->isOnTop() )
      {
        aTopmostObjects.append( anObject );
        continue;
      }

      int aPriority = anObject->getPriority();
      if( aPriority > aCurrentPriority  )
      {
        if( !aSelectedObjects.isEmpty() )
        {
          aList.append( aSelectedObjects );
          aSelectedObjects.clear();
        }
        aCurrentPriority = aPriority;
      }

      if( anObject->isSelected() )
        aSelectedObjects.append( anObject );
      else
        aList.append( anObject );
    }
  }

  // for selected objects with highest priority,
  // which were not pushed to the result list yet
  if( !aSelectedObjects.isEmpty() )
  {
    aList.append( aSelectedObjects );
    aSelectedObjects.clear();
  }

  aList.append( aTopmostObjects );

  return aList;
}

//================================================================
// Function : objectsBoundingRect
// Purpose  : 
//================================================================
QRectF GraphicsView_ViewPort::objectsBoundingRect( bool theOnlyVisible ) const
{
  QRectF aRect;
  QListIterator<QGraphicsItem*> anIter( items() );
  while( anIter.hasNext() )
  {
    if( GraphicsView_Object* anObject = dynamic_cast<GraphicsView_Object*>( anIter.next() ) )
    {
      if( theOnlyVisible && !anObject->isVisible() )
        continue;

      QRectF anObjectRect = anObject->getRect();
      if( !anObjectRect.isNull() )
      {
        if( aRect.isNull() )
          aRect = anObject->getRect();
        else
          aRect |= anObject->getRect();
      }
    }
  }
  return aRect;
}

//================================================================
// Function : dumpView
// Purpose  : 
//================================================================
QImage GraphicsView_ViewPort::dumpView( bool theWholeScene,
                                        QSizeF theSize )
{
  if( !theWholeScene ) // just grab the view contents
  {
    QPixmap aPixmap = QPixmap::grabWindow( viewport()->winId() );
    return aPixmap.toImage();
  }

  // get a bounding rect of all presented objects
  // (itemsBoundingRect() method is unsuitable)
  QRectF aRect = objectsBoundingRect();
  if( aRect.isNull() )
    return QImage();

  QRectF aTargetRect( 0, 0, aRect.width(), aRect.height() );
  if( theSize.isValid() )
  {
    double aRatioX = theSize.width() / aTargetRect.width();
    double aRatioY = theSize.height() / aTargetRect.height();
    double aRatio = qMin( aRatioX, aRatioY );
    aTargetRect.setWidth( aTargetRect.width() * aRatio );
    aTargetRect.setHeight( aTargetRect.height() * aRatio );
  }

  // render the scene to an image
  QImage anImage( aTargetRect.toRect().size(), QImage::Format_RGB32 );
  QPainter aPainter( &anImage );
  aPainter.setRenderHints( renderHints() );

  myScene->render( &aPainter, aTargetRect, aRect );

  return anImage;
}

//================================================================
// Function : setSceneGap
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setSceneGap( double theSceneGap )
{
  mySceneGap = theSceneGap;
  onBoundingRectChanged();
}

//================================================================
// Function : setFitAllGap
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setFitAllGap( double theFitAllGap )
{
  myFitAllGap = theFitAllGap;
}

//================================================================
// Function : setTraceBoundingRectEnabled
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setTraceBoundingRectEnabled( bool theState )
{
  myIsTraceBoundingRectEnabled = theState;
}

//================================================================
// Function : setInteractionFlags
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setInteractionFlags( const int theFlags )
{
  myInteractionFlags |= theFlags;
}

//================================================================
// Function : clearInteractionFlags
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::clearInteractionFlags( const int theFlags )
{
  myInteractionFlags &= ~theFlags;
}

//================================================================
// Function : testInteractionFlags
// Purpose  : 
//================================================================
bool GraphicsView_ViewPort::testInteractionFlags( const int theFlags ) const
{
  return ( myInteractionFlags & theFlags ) == theFlags;
}

//================================================================
// Function : setViewNameEnabled
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setViewNamePosition( NamePosition thePosition,
                                                 bool theIsForced )
{
  if( theIsForced && !myNameLabel )
    myNameLabel = new NameLabel( viewport() );

  if( !myNameLabel )
    return;

  if( thePosition == NP_None )
  {
    myNameLabel->setVisible( false );
    return;
  }

  if( myNameLayout )
    delete myNameLayout;

  myNameLayout = new QGridLayout( viewport() );
  myNameLayout->setMargin( 10 );
  myNameLayout->setSpacing( 0 );

  int aRow = 0, aColumn = 0;
  switch( thePosition )
  {
    case NP_TopLeft:     aRow = 0; aColumn = 0; break;
    case NP_TopRight:    aRow = 0; aColumn = 1; break;
    case NP_BottomLeft:  aRow = 1; aColumn = 0; break;
    case NP_BottomRight: aRow = 1; aColumn = 1; break;
    default: break;
  }

  myNameLayout->addWidget( myNameLabel, aRow, aColumn );
  myNameLayout->setRowStretch( 1 - aRow, 1 );
  myNameLayout->setColumnStretch( 1 - aColumn, 1 );

  myNameLabel->setVisible( true );
}

//================================================================
// Function : setViewName
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setViewName( const QString& theName )
{
  if( myNameLabel )
    myNameLabel->setText( theName );
}

//================================================================
// Function : backgroundColor
// Purpose  : 
//================================================================
QColor GraphicsView_ViewPort::backgroundColor() const
{
  return backgroundBrush().color();
}

//================================================================
// Function : setBackgroundColor
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setBackgroundColor( const QColor& theColor )
{
  setBackgroundBrush( QBrush( theColor ) );
}

//================================================================
// Function : setForegroundEnabled
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setForegroundEnabled( bool theState )
{
  myIsForegroundEnabled = theState;
}

//================================================================
// Function : setForegroundSize
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setForegroundSize( const QSizeF& theSize )
{
  myForegroundSize = theSize;
}

//================================================================
// Function : setForegroundMargin
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setForegroundMargin( double theMargin )
{
  myForegroundMargin = theMargin;
}

//================================================================
// Function : setForegroundColor
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setForegroundColor( const QColor& theColor )
{
  myForegroundColor = theColor;
}

//================================================================
// Function : setForegroundFrameColor
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setForegroundFrameColor( const QColor& theColor )
{
  myForegroundFrameColor = theColor;
}

//================================================================
// Function : setForegroundFrameLineWidth
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setForegroundFrameLineWidth( double theLineWidth )
{
  myForegroundFrameLineWidth = theLineWidth;
}

//================================================================
// Function : updateForeground
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::updateForeground()
{
  if( myIsForegroundEnabled )
  {
    if( !myForegroundItem )
      myForegroundItem = myScene->addRect( QRectF(), QPen(), QBrush( Qt::white ) );
    myForegroundItem->setZValue( -2 );

    QPointF aPoint = QPointF();
    QRectF aRect( aPoint, myForegroundSize );
    aRect.adjust( -myForegroundMargin, -myForegroundMargin,
                  myForegroundMargin, myForegroundMargin );
    myForegroundItem->setRect( aRect );

    QBrush aBrush = myForegroundItem->brush();
    aBrush.setColor( myForegroundColor );
    myForegroundItem->setBrush( aBrush );

    QPen aPen = myForegroundItem->pen();
    aPen.setColor( myForegroundFrameColor );
    aPen.setWidthF( myForegroundFrameLineWidth );
    myForegroundItem->setPen( aPen );

    myForegroundItem->setVisible( true );

    myScene->setSceneRect( aRect.adjusted( -mySceneGap, -mySceneGap, mySceneGap, mySceneGap ) );
  }
  else
  {
    if( myForegroundItem )
      myForegroundItem->setVisible( false );
  }

  updateGrid(); // foreground size could be changed
}

//================================================================
// Function : setGridEnabled
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setGridEnabled( bool theState )
{
  myIsGridEnabled = theState;
}

//================================================================
// Function : setGridCellSize
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setGridCellSize( int theCellSize )
{
  myGridCellSize = theCellSize;
}

//================================================================
// Function : setGridLineStyle
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setGridLineStyle( int theLineStyle )
{
  myGridLineStyle = theLineStyle;
}

//================================================================
// Function : setGridLineColor
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setGridLineColor( const QColor& theLineColor )
{
  myGridLineColor = theLineColor;
}

//================================================================
// Function : updateGrid
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::updateGrid()
{
  if( myIsGridEnabled )
  {
    if( !myGridItem )
      myGridItem = myScene->addPath( QPainterPath() );
    myGridItem->setZValue( -1 );

    double aWidth = myForegroundSize.width();
    double aHeight = myForegroundSize.height();

    int aGridNbX = int( aWidth / myGridCellSize ) + 1;
    int aGridNbY = int( aHeight / myGridCellSize ) + 1;

    int anIndex;
    QPainterPath aPath;
    for( anIndex = 0; anIndex < aGridNbX; anIndex++ )
    {
      double x = myGridCellSize * (double)anIndex;
      aPath.moveTo( x, 0 );
      aPath.lineTo( x, aHeight );
    }
    for( anIndex = 0; anIndex < aGridNbY; anIndex++ )
    {
      double y = myGridCellSize * (double)anIndex;
      aPath.moveTo( 0, y );
      aPath.lineTo( aWidth, y );
    }
    myGridItem->setPath( aPath );

    QPen aPen = myGridItem->pen();
    aPen.setStyle( (Qt::PenStyle)myGridLineStyle );
    aPen.setColor( myGridLineColor );
    myGridItem->setPen( aPen );

    myGridItem->setVisible( true );
  }
  else
  {
    if( myGridItem )
      myGridItem->setVisible( false );
  }
}

//================================================================
// Function : reset
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::reset()
{
  fitAll();
}

//================================================================
// Function : pan
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::pan( double theDX, double theDY )
{
  myIsTransforming = true;

  if( myNameLabel )
    myNameLabel->setAcceptMoveEvents( false );

  if( QScrollBar* aHBar = horizontalScrollBar() )
    aHBar->setValue( aHBar->value() - theDX );
  if( QScrollBar* aVBar = verticalScrollBar() )
    aVBar->setValue( aVBar->value() + theDY );

  if( myNameLabel )
    myNameLabel->setAcceptMoveEvents( true );

  myIsTransforming = false;
}

//================================================================
// Function : setCenter
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setCenter( double theX, double theY )
{
  myIsTransforming = true;

  setTransform( myCurrentTransform );
  centerOn( theX, theY );

  myIsTransforming = false;
}

//================================================================
// Function : zoom
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::zoom( double theX1, double theY1, double theX2, double theY2 )
{
  myIsTransforming = true;

  double aDX = theX2 - theX1;
  double aDY = theY2 - theY1;
  double aZoom = sqrt( aDX * aDX + aDY * aDY ) / 100 + 1;
  aZoom = ( aDX > 0 ) ?  aZoom : 1 / aZoom;

  QTransform aTransform = transform();
  aTransform.scale( aZoom, aZoom );
  double aM11 = aTransform.m11();
  double aM22 = aTransform.m22();
  // increasing of diagonal coefficients (>300) leads to a crash sometimes
  // at the values of 100 some primitives are drawn incorrectly
  if( qMax( aM11, aM22 ) < 100 )
    setTransform( aTransform );

  myIsTransforming = false;
}

//================================================================
// Function : fitRect
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::fitRect( const QRectF& theRect )
{
  myIsTransforming = true;

  fitInView( theRect, Qt::KeepAspectRatio );

  myIsTransforming = false;
}

//================================================================
// Function : fitSelect
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::fitSelect()
{
  myIsTransforming = true;

  QRectF aGlobalRect;
  for( initSelected(); moreSelected(); nextSelected() )
  {
    if( GraphicsView_Object* aMovingObject = selectedObject() )
    {
      QRectF aRect = aMovingObject->getRect();
      if( aGlobalRect.isNull() )
        aGlobalRect = aRect;
      else
        aGlobalRect |= aRect;
    }
  }

  if( !aGlobalRect.isNull() )
  {
    double aGap = qMax( aGlobalRect.width(), aGlobalRect.height() ) / 5;
    aGlobalRect.adjust( -aGap, -aGap, aGap, aGap );
    fitInView( aGlobalRect, Qt::KeepAspectRatio );
  }

  myIsTransforming = false;
}

//================================================================
// Function : fitAll
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::fitAll( bool theKeepScale )
{
  myIsTransforming = true;

  if( theKeepScale )
    myCurrentTransform = transform();

  double aGap = myFitAllGap;
  QRectF aRect = objectsBoundingRect( true );
  fitInView( aRect.adjusted( -aGap, -aGap, aGap, aGap ), Qt::KeepAspectRatio );

  myIsTransforming = false;
}

//================================================================
// Function : currentBlock
// Purpose  : 
//================================================================
GraphicsView_ViewPort::BlockStatus GraphicsView_ViewPort::currentBlock()
{
  if( isDragging() && !myDragPosition.isNull() )
    return BlockStatus( BS_Selection );

  if( myAreSelectionPointsInitialized && ( myFirstSelectionPoint != myLastSelectionPoint ) )
    return BlockStatus( BS_Selection );

  if( isPulling() )
    return BlockStatus( BS_Selection );

  return BS_NoBlock;
}

//================================================================
// Function : highlight
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::highlight( double theX, double theY )
{
  myHighlightX = theX;
  myHighlightY = theY;

  bool anIsHighlighted = false;
  bool anIsOnObject = false;

  GraphicsView_Object* aPreviousHighlightedObject = myHighlightedObject;
  GraphicsView_Object* aHighlightedObject = 0;

  GraphicsView_ObjectList aList = getObjects( true );
  GraphicsView_ObjectListIterator anIter( aList );
  anIter.toBack(); // objects with higher priority have to be checked earlier
  while( anIter.hasPrevious() )
  {
    if( GraphicsView_Object* anObject = anIter.previous() )
    {
      if( anObject->isVisible() && anObject->isSelectable() )
      {
        QRectF aRect = anObject->getRect();
        if( !aRect.isNull() && aRect.contains( theX, theY ) )
        {
          anIsOnObject = true;
          anIsHighlighted = anObject->highlight( theX, theY );
        }

        if( anIsHighlighted )
        {
          aHighlightedObject = anObject;
          break;
        }
      }
    }
  }

  if( !anIsOnObject )
  {
    anIter = aList;
    while( anIter.hasNext() )
      if( GraphicsView_Object* anObject = anIter.next() )
        anObject->unhighlight();

    myHighlightedObject = 0;
    return;
  }
  else if( !myHighlightedObject && anIsHighlighted )
  {
    myHighlightedObject = aHighlightedObject;
  }
  else if( myHighlightedObject && !anIsHighlighted )
  {
    myHighlightedObject->unhighlight();
    myHighlightedObject = 0;
  }
  else if( myHighlightedObject && anIsHighlighted )
  {
    myHighlightedObject->highlight( theX, theY );
    if( myHighlightedObject != aHighlightedObject )
    {
      myHighlightedObject->unhighlight();
      myHighlightedObject = aHighlightedObject;
    }
  }
}

//================================================================
// Function : clearHighlighted
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::clearHighlighted()
{
  if( myHighlightedObject )
  {
    myHighlightedObject->unhighlight();
    myHighlightedObject = 0;
  }
}

//================================================================
// Function : select
// Purpose  : 
//================================================================
int GraphicsView_ViewPort::select( const QRectF& theRect, bool theIsAppend )
{
  GV_SelectionStatus aStatus = GVSS_Invalid;
  if( theRect.isNull() ) // point selection
  {
    if( myHighlightedObject )
    {
      if( mySelectedObjects.count() == 1 &&
          mySelectedObjects.first() == myHighlightedObject )
        aStatus = GVSS_LocalChanged;

      if( !theIsAppend )
      {
        GraphicsView_ObjectListIterator anIter( mySelectedObjects );
        while( anIter.hasNext() )
          if( GraphicsView_Object* anObject = anIter.next() )
            if( myHighlightedObject != anObject )
              anObject->unselect();

        if( !mySelectedObjects.isEmpty() && aStatus == GVSS_Invalid )
          aStatus = GVSS_GlobalChanged;
        mySelectedObjects.clear();
      } 
      else if( myHighlightedObject->isSelected() && aStatus != GVSS_LocalChanged )
      {
        mySelectedObjects.removeAll( myHighlightedObject );
        myHighlightedObject->unselect();

        if( !mySelectedObjects.isEmpty() && aStatus == GVSS_Invalid )
          aStatus = GVSS_GlobalChanged;

        return aStatus;
      }

      if( myHighlightedObject->select( myHighlightX, myHighlightY, QRectF() ) &&
          mySelectedObjects.indexOf( myHighlightedObject ) == -1 )
      {
        mySelectedObjects.append( myHighlightedObject );
        if( aStatus == GVSS_Invalid )
          aStatus = GVSS_GlobalChanged;
      }
      else if( aStatus == GVSS_LocalChanged )
        aStatus = GVSS_GlobalChanged;

      return aStatus;
    }

    if( !myHighlightedObject )
    {
      if( !theIsAppend )
      {
        GraphicsView_ObjectListIterator anIter( mySelectedObjects );
        while( anIter.hasNext() )
          if( GraphicsView_Object* anObject = anIter.next() )
            if( myHighlightedObject != anObject )
              anObject->unselect();

        if( !mySelectedObjects.isEmpty() )
          aStatus = GVSS_GlobalChanged;
        mySelectedObjects.clear();
      }
      return aStatus;
    }

    return GVSS_NoChanged;
  }
  else // rectangle selection
  {
    aStatus = GVSS_NoChanged;

    bool updateAll = false;
    if( !theIsAppend )
    {
      if( !mySelectedObjects.isEmpty() )
        aStatus = GVSS_GlobalChanged;

      GraphicsView_ObjectListIterator anIter( mySelectedObjects );
      while( anIter.hasNext() )
        if( GraphicsView_Object* anObject = anIter.next() )
          if( myHighlightedObject != anObject )
            anObject->unselect();
      mySelectedObjects.clear();
    }

    QListIterator<QGraphicsItem*> anIter( items() );
    while( anIter.hasNext() )
    {
      if( GraphicsView_Object* anObject = dynamic_cast<GraphicsView_Object*>( anIter.next() ) )
      {
        if( anObject->isVisible() && anObject->isSelectable() )
        {
          bool anIsSelected = false;
          QRectF aRect = anObject->getRect();
          if( theRect.contains( aRect ) )
            anIsSelected = anObject->select( myHighlightX, myHighlightY, theRect );

          if( anIsSelected && mySelectedObjects.indexOf( anObject ) == -1 )
          {
            mySelectedObjects.append( anObject );
            aStatus = GVSS_GlobalChanged;
          }
        }
      }
    }
  }
  return aStatus;
}

//================================================================
// Function : clearSelected
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::clearSelected()
{
  GraphicsView_ObjectListIterator anIter( mySelectedObjects );
  while( anIter.hasNext() )
    if( GraphicsView_Object* anObject = anIter.next() )
      anObject->unselect();
  mySelectedObjects.clear();
}

//================================================================
// Function : setSelected
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::setSelected( GraphicsView_Object* theObject )
{
  if( theObject )
  {
    theObject->setSelected( true );
    mySelectedObjects.append( theObject );
  }
}

//================================================================
// Function : nbSelected
// Purpose  : 
//================================================================
int GraphicsView_ViewPort::nbSelected() const
{
  return mySelectedObjects.count();
}

//================================================================
// Function : initSelected
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::initSelected()
{
  mySelectionIterator = 0;
}

//================================================================
// Function : moreSelected
// Purpose  : 
//================================================================
bool GraphicsView_ViewPort::moreSelected()
{
  return mySelectionIterator < nbSelected();
}

//================================================================
// Function : nextSelected
// Purpose  : 
//================================================================
bool GraphicsView_ViewPort::nextSelected()
{
  if( mySelectionIterator >= 0 && mySelectionIterator < nbSelected() )
  {
    mySelectionIterator++;
    return true;
  }
  return false;
}

//================================================================
// Function : selectedObject
// Purpose  : 
//================================================================
GraphicsView_Object* GraphicsView_ViewPort::selectedObject()
{
  if( mySelectionIterator >= 0 && mySelectionIterator < nbSelected() )
    return mySelectedObjects[ mySelectionIterator ];
  return 0;
}

//================================================================
// Function : startSelectByRect
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::startSelectByRect( int x, int y )
{
  if( !myAreSelectionPointsInitialized )
  {
    myFirstSelectionPoint = QPoint( x, y );
    myLastSelectionPoint = QPoint( x, y );
    myAreSelectionPointsInitialized = true;
  }

  if( !myRectBand )
  {
    myRectBand = new QRubberBand( QRubberBand::Rectangle, this );
    QPalette palette;
    palette.setColor( myRectBand->foregroundRole(), Qt::white );
    myRectBand->setPalette( palette );
  }
  myRectBand->hide();
}

//================================================================
// Function : drawSelectByRect
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::drawSelectByRect( int x, int y )
{
  if( myAreSelectionPointsInitialized )
  {
    myRectBand->hide();

    myLastSelectionPoint.setX( x );
    myLastSelectionPoint.setY( y );

    QRect aRect = selectionRect();
    myRectBand->setGeometry( aRect );
    myRectBand->setVisible( aRect.isValid() );
  }
}

//================================================================
// Function : isSelectByRect
// Purpose  : 
//================================================================
bool GraphicsView_ViewPort::isSelectByRect() const
{
  return myAreSelectionPointsInitialized;
}

//================================================================
// Function : finishSelectByRect
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::finishSelectByRect()
{
  if( myAreSelectionPointsInitialized )
  {
    if( myRectBand )
    {
      myRectBand->hide();
      delete myRectBand;
      myRectBand = 0;
    }

    myAreSelectionPointsInitialized = false;
  }
}

//================================================================
// Function : selectionRect
// Purpose  : 
//================================================================
QRect GraphicsView_ViewPort::selectionRect()
{
  QRect aRect;
  if( myAreSelectionPointsInitialized )
  {
    aRect.setLeft( qMin( myFirstSelectionPoint.x(), myLastSelectionPoint.x() ) );
    aRect.setTop( qMin( myFirstSelectionPoint.y(), myLastSelectionPoint.y() ) );
    aRect.setRight( qMax( myFirstSelectionPoint.x(), myLastSelectionPoint.x() ) );
    aRect.setBottom( qMax( myFirstSelectionPoint.y(), myLastSelectionPoint.y() ) );
  }
  return aRect;
}

//================================================================
// Function : dragObjects
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::dragObjects( QGraphicsSceneMouseEvent* e )
{
  GraphicsView_Object* anObject = getHighlightedObject();

  if( myDragPosition.isNull() )
  {
    myDragPosition = e->scenePos();
    return;
  }

  GraphicsView_ObjectList anObjectsToMove;
  if( anObject && anObject->isMovable() && ( e->buttons() & Qt::LeftButton ) )
  {
    if( anObject->isSelected() )
    {
      for( initSelected(); moreSelected(); nextSelected() )
        if( GraphicsView_Object* aMovingObject = selectedObject() )
          if( aMovingObject->isMovable() )
            anObjectsToMove.append( aMovingObject );
    }
    else
      anObjectsToMove.append( anObject );
  }
  else if( nbSelected() && ( e->buttons() & Qt::MidButton ) )
  {
    for( initSelected(); moreSelected(); nextSelected() )
      if( GraphicsView_Object* aMovingObject = selectedObject() )
        if( aMovingObject->isMovable() )
          anObjectsToMove.append( aMovingObject );
  }

  if( anObjectsToMove.isEmpty() )
    return;

  double aDX = e->scenePos().x() - myDragPosition.x();
  double aDY = e->scenePos().y() - myDragPosition.y();

  bool anIsMovingByXAllowed = true, anIsMovingByYAllowed = true;
  GraphicsView_ObjectListIterator anIter( anObjectsToMove );
  while( anIter.hasNext() )
    if( GraphicsView_Object* aMovingObject = anIter.next() )
    {
      if( !aMovingObject->isMovingByXAllowed( aDX ) )
        anIsMovingByXAllowed = false;
      if( !aMovingObject->isMovingByYAllowed( aDY ) )
        anIsMovingByYAllowed = false;
    }

  if( !anIsMovingByXAllowed && !anIsMovingByYAllowed )
    return; // myDragPosition shouldn't be changed

  if( !anIsMovingByXAllowed )
    aDX = 0;

  if( !anIsMovingByYAllowed )
    aDY = 0;

  anIter = anObjectsToMove;
  while( anIter.hasNext() )
    if( GraphicsView_Object* aMovingObject = anIter.next() )
      aMovingObject->move( aDX, aDY );

  if( anIsMovingByXAllowed )
    myDragPosition.setX( e->scenePos().x() );

  if( anIsMovingByYAllowed )
    myDragPosition.setY( e->scenePos().y() );
}

//================================================================
// Function : startPulling
// Purpose  : 
//================================================================
bool GraphicsView_ViewPort::startPulling( const QPointF& thePoint )
{
  QListIterator<QGraphicsItem*> anIter( items() );
  while( anIter.hasNext() )
  {
    if( GraphicsView_Object* anObject = dynamic_cast<GraphicsView_Object*>( anIter.next() ) )
    {
      QRectF aRect = anObject->getPullingRect();
      if( aRect.contains( thePoint ) && anObject->startPulling( thePoint ) )
      {
        myIsPulling = true;
        myPullingObject = anObject;
        setCursor( *getHandCursor() );
        return true;
      }
    }
  }
  return false;
}

//================================================================
// Function : drawPulling
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::drawPulling( const QPointF& thePoint )
{
  GraphicsView_Object* aLockedObject = 0;

  QListIterator<QGraphicsItem*> anIter( items() );
  while( anIter.hasNext() )
  {
    if( GraphicsView_Object* anObject = dynamic_cast<GraphicsView_Object*>( anIter.next() ) )
    {
      if( !anObject->isVisible() )
        continue;

      QRectF aRect = anObject->getPullingRect();
      if( aRect.contains( thePoint ) && anObject->portContains( thePoint ) )
      {
        aLockedObject = anObject;
        break;
      }
    }
  }

  myPullingObject->pull( thePoint, aLockedObject );
}

//================================================================
// Function : finishPulling
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::finishPulling()
{
  myIsPulling = false;
  myPullingObject->finishPulling();
  setCursor( *getDefaultCursor() );
}

//================================================================
// Function : onBoundingRectChanged
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::onBoundingRectChanged()
{
  if( myIsTraceBoundingRectEnabled )
  {
    QRectF aRect = objectsBoundingRect();
    myScene->setSceneRect( aRect.adjusted( -mySceneGap, -mySceneGap, mySceneGap, mySceneGap ) );
  }
}

//================================================================
// Function : onKeyEvent
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::onKeyEvent( QKeyEvent* e )
{
  emit vpKeyEvent( e );
}

//================================================================
// Function : onMouseEvent
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::onMouseEvent( QGraphicsSceneMouseEvent* e )
{
  emit vpMouseEvent( e );

  bool anIsHandled = false;
  switch( e->type() )
  {
    case QEvent::GraphicsSceneMousePress:
    {
      if( nbSelected() )
        for( initSelected(); moreSelected() && !anIsHandled; nextSelected() )
          if( GraphicsView_Object* anObject = selectedObject() )
            anIsHandled = anObject->handleMousePress( e );

      if( !anIsHandled && testInteractionFlags( Dragging ) )
      {
        bool anAccel = e->modifiers() & GraphicsView_ViewTransformer::accelKey();
        if( ( getHighlightedObject() &&
              getHighlightedObject()->isMovable() &&
              !( anAccel || e->button() == Qt::RightButton ) ) ||
            ( nbSelected() && !anAccel && e->button() == Qt::MidButton ) )
        myIsDragging = true;
      }
      break;
    }
    case QEvent::GraphicsSceneMouseMove:
    {
      if( nbSelected() )
        for( initSelected(); moreSelected() && !anIsHandled; nextSelected() )
          if( GraphicsView_Object* anObject = selectedObject() )
            anIsHandled = anObject->handleMousePress( e );

      if( !anIsHandled && !isPulling() && myIsDragging )
        dragObjects( e );
      break;
    }
    case QEvent::GraphicsSceneMouseRelease:
    {
      if( nbSelected() )
        for( initSelected(); moreSelected() && !anIsHandled; nextSelected() )
          if( GraphicsView_Object* anObject = selectedObject() )
            anIsHandled = anObject->handleMousePress( e );

      if( !anIsHandled && !isPulling() && myIsDragging )
      {
        emit vpObjectBeforeMoving();

        bool anIsMoved = false;
        for( initSelected(); moreSelected(); nextSelected() )
          if( GraphicsView_Object* aMovingObject = selectedObject() )
            anIsMoved = aMovingObject->finishMove() || anIsMoved;

        if( GraphicsView_Object* aMovingObject = getHighlightedObject() )
          anIsMoved = aMovingObject->finishMove() || anIsMoved;

        myIsDragging = false;
        myDragPosition = QPointF();

        emit vpObjectAfterMoving( anIsMoved );
      }
      break;
    }
    case QEvent::GraphicsSceneMouseDoubleClick:
      break; // do nothing, just emit the signal
    default:
      break;
  }
}

//================================================================
// Function : onWheelEvent
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::onWheelEvent( QGraphicsSceneWheelEvent* e )
{
  emit vpWheelEvent( e );
}

//================================================================
// Function : onContextMenuEvent
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::onContextMenuEvent( QGraphicsSceneContextMenuEvent* e )
{
  emit vpContextMenuEvent( e );
}

//================================================================
// Function : scrollContentsBy
// Purpose  : 
//================================================================
void GraphicsView_ViewPort::scrollContentsBy( int theDX, int theDY )
{
  if( myNameLabel )
    myNameLabel->setAcceptMoveEvents( false );

  QGraphicsView::scrollContentsBy( theDX, theDY );

  if( myNameLabel )
    myNameLabel->setAcceptMoveEvents( true );
}
