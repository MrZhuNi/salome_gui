//  SALOME QxGraph : build Supervisor viewer into desktop
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "QxGraph_CanvasView.h"
#include "QxGraph_Canvas.h"
#include "QxGraph_ViewWindow.h"
#include "QxGraph_ActiveItem.h"
#include "QxGraph_Def.h"

#include <qwmatrix.h>

#include <math.h>

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
QxGraph_CanvasView::QxGraph_CanvasView(QxGraph_Canvas* theCanvas, QxGraph_ViewWindow* theViewWindow) :
  QCanvasView(theCanvas, theViewWindow),
  myCurrentItem(0)
{
  printf("Construct QxGraph_CanvasView\n");
  setName("QxGraph_CanvasView");

  myOperation = NOTHING;
  myCursor = cursor();
  mySelectedRect = 0;

  myTimer = new QTimer(this);
  connect(myTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

  viewport()->setMouseTracking(true);
}

/*!
  Destructor
*/
QxGraph_CanvasView::~QxGraph_CanvasView()
{
}

void QxGraph_CanvasView::contentsMousePressEvent(QMouseEvent* theEvent)
{
  myPoint = inverseWorldMatrix().map(theEvent->pos());
  myGlobalPoint = theEvent->globalPos();
  myCurrentItem = 0;

  if ( theEvent->button() == Qt::MidButton && theEvent->state() == Qt::ControlButton
       || 
       myOperation == PANVIEW )
  { // Panning
    if ( myOperation != PANVIEW ) {
      myOperation = PANVIEW;
      myCursor = cursor(); // save old cursor
      QCursor panCursor (Qt::SizeAllCursor);
      setCursor(panCursor);
    }
    return;
  }

  if ( myOperation == PANGLOBAL )
  { // Global panning
    return;
  }

  if ( theEvent->button() == Qt::LeftButton && theEvent->state() == Qt::ControlButton
       || 
       myOperation == ZOOMVIEW )
  { // Zoom
    if ( myOperation != ZOOMVIEW ) {
      myOperation = ZOOMVIEW;
      myCursor = cursor(); // save old cursor
      QPixmap zoomPixmap (imageZoomCursor);
      QCursor zoomCursor (zoomPixmap);
      setCursor(zoomCursor);
    }
    return;
  }

  if ( theEvent->button() == Qt::LeftButton )
  {
    QCanvasItemList aList = canvas()->collisions(myPoint);
    // to move items on canvas view
    for (QCanvasItemList::Iterator it = aList.begin(); it != aList.end(); ++it) {
      QxGraph_ActiveItem* anActItem = dynamic_cast<QxGraph_ActiveItem*>( *it );
      if ( anActItem && anActItem->isMoveable() )
      { // move itself only active items if it is moveable
	anActItem->beforeMoving();
	myCurrentItem = *it;
	return;
      }
    }
  }
}

void QxGraph_CanvasView::contentsMouseMoveEvent(QMouseEvent* theEvent)
{
  QPoint aPoint = inverseWorldMatrix().map(theEvent->pos());
  QPoint aGlobalPoint = theEvent->globalPos();

  if (myTimer->isActive()) myTimer->stop();

  if ( myOperation == PANVIEW )
  { // Panning
    scrollBy(myGlobalPoint.x() - aGlobalPoint.x(),
	     myGlobalPoint.y() - aGlobalPoint.y());
    myGlobalPoint = aGlobalPoint;
    return;
  }

  if ( myOperation == WINDOWFIT )
  { // Fit within rectangle
    int aLX, aTY; //left x and top y
    if (myPoint.x() < aPoint.x()) aLX = myPoint.x();
    else aLX = aPoint.x();
    if (myPoint.y() < aPoint.y()) aTY = myPoint.y();
    else aTY = aPoint.y();
    QRect aRect(aLX, aTY, abs(myPoint.x()-aPoint.x()), abs(myPoint.y()-aPoint.y()));
    QCanvasRectangle* aRect1 = new QCanvasRectangle(aRect, canvas());

    //hide old selected rectangle
    if (mySelectedRect)
      mySelectedRect->hide();
    //draw new selected rectangle
    QPen pen(Qt::black,1,Qt::SolidLine);
    aRect1->setPen(pen);
    aRect1->setZ(3);
    aRect1->show();

    mySelectedRect = aRect1;
    canvas()->update();
  }

  if ( myOperation == ZOOMVIEW )
  { // Zoom
    double dx = aGlobalPoint.x() - myGlobalPoint.x();
    double s = 1. + fabs(dx)/10.;
    if (dx < 0) s = 1./s;
    
    QWMatrix m = worldMatrix();
    m.scale(s, s);
    setWorldMatrix(m);
	
    myGlobalPoint = aGlobalPoint;
    return;
  }

  if ( myCurrentItem )
  { // to move items on canvas view
    if ( myCurrentItem->x() && myCurrentItem->y() ) {
      double cx = myCurrentItem->x() - myPoint.x();
      double cy = myCurrentItem->y() - myPoint.y();
      
      if (aPoint.x()+cx < 0) aPoint.setX(-(int)cx);
      if (aPoint.y()+cy < 0) aPoint.setY(-(int)cy);
    }
    myCurrentItem->moveBy(aPoint.x() - myPoint.x(), 
			  aPoint.y() - myPoint.y());
    myPoint = aPoint;
    canvas()->update();

    // scroll contents if mouse is outside
    QRect r(contentsX(), contentsY(), visibleWidth(), visibleHeight());
    if (!r.contains(theEvent->pos())) {
      int dx = 0, dy = 0;
      if (theEvent->pos().x() < r.left()) dx = theEvent->pos().x() - r.left();
      if (theEvent->pos().x() > r.right()) dx = theEvent->pos().x() - r.right();
      if (theEvent->pos().y() < r.top()) dy = theEvent->pos().y() - r.top();
      if (theEvent->pos().y() > r.bottom()) dy = theEvent->pos().y() - r.bottom();
      scrollBy(dx, dy);
      // start timer to scroll in silent mode
      myDX = dx; myDY = dy;
      myTimer->start(100);
    }
    
    return;
  }
}

void QxGraph_CanvasView::contentsMouseReleaseEvent(QMouseEvent* theEvent)
{
  if (myTimer->isActive()) myTimer->stop();

  if (myCurrentItem)
  { // to move items on canvas view    
    QxGraph_ActiveItem* anActItem = dynamic_cast<QxGraph_ActiveItem*>( myCurrentItem );
    if ( anActItem && anActItem->isMoveable() )
      anActItem->afterMoving();
  }
  myCurrentItem = 0;

  if ( myOperation == PANVIEW )
  { // Panning
    myOperation = NOTHING;
    viewport()->setMouseTracking(true);
    setCursor(myCursor);
  }

  if ( myOperation == PANGLOBAL )
  { // Global panning
    myOperation = NOTHING;
    center( theEvent->x(), theEvent->y() );
    setCursor(myCursor);
  }

  if ( myOperation == WINDOWFIT )
  { // Fit within rectangle
    myOperation = NOTHING;
    
    if (mySelectedRect) {
      mySelectedRect->hide();
      mySelectedRect = 0;
      //canvas()->update();
    }

    //myPoint is the start point for selecting rectangle now
    QPoint anEndPoint = inverseWorldMatrix().map(theEvent->pos());
    int aLX, aTY; //left x and top y
    if (myPoint.x() < anEndPoint.x()) aLX = myPoint.x();
    else aLX = anEndPoint.x();
    if (myPoint.y() < anEndPoint.y()) aTY = myPoint.y();
    else aTY = anEndPoint.y();

    //calculate width and height for new view and new zoom factor
    double aXzoom = ((double)visibleWidth())/((double)(abs(myPoint.x()-anEndPoint.x())));
    double aYzoom = ((double)visibleHeight())/((double)(abs(myPoint.y()-anEndPoint.y())));
    if (aXzoom > aYzoom) aXzoom = aYzoom;
    
    QWMatrix m;
    m.scale(aXzoom, aXzoom);
    setWorldMatrix(m);
    setContentsPos((int)(aLX*aXzoom), (int)(aTY*aYzoom));

    canvas()->update();
    
    viewport()->setMouseTracking(true);
    setCursor(myCursor);
  }

  if ( myOperation == ZOOMVIEW )
  { // Zoom
    myOperation = NOTHING;
    viewport()->setMouseTracking(true);
    setCursor(myCursor);
  }

  if ( theEvent->button() == RightButton )
  { // Background popup
    printf("Background popup\n");
    QContextMenuEvent aEvent( QContextMenuEvent::Mouse,
			      theEvent->pos(), theEvent->globalPos(),
			      theEvent->state() );
    if ( getViewWindow() )
      getViewWindow()->contextPopupEvent(&aEvent); // => emit contextMenuRequested( &aEvent );
  }
}

void QxGraph_CanvasView::contentsMouseDoubleClickEvent(QMouseEvent* theEvent)
{

}

void QxGraph_CanvasView::activateFitAll()
{
  //myOperation = FITALL;
  int w = 0, h = 0;
  QCanvasItemList l = canvas()->allItems();
  for (QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it) {
    QRect r = (*it)->boundingRect();
    if (w < r.right()) w = r.right();
    if (h < r.bottom()) h = r.bottom();
  }
  w += GRAPH_MARGIN; h += GRAPH_MARGIN;
  double s = ((double)visibleWidth())/((double)w);
  double s1 = ((double)visibleHeight())/((double)h);
  if (s > s1) s = s1;

  setContentsPos(0,0);
  QWMatrix m;
  m.scale(s, s);
  setWorldMatrix(m);
  canvas()->update();
  //myOperation = NOTHING;
}

void QxGraph_CanvasView::activateFitRect()
{
  myOperation = WINDOWFIT;
  viewport()->setMouseTracking(false);
  myCursor = cursor(); // save old cursor
  QCursor handCursor (Qt::PointingHandCursor);
  setCursor(handCursor);
}

void QxGraph_CanvasView::activateZoom()
{
  myOperation = ZOOMVIEW;
  viewport()->setMouseTracking(false);
  myCursor = cursor(); // save old cursor
  QPixmap zoomPixmap (imageZoomCursor);
  QCursor zoomCursor (zoomPixmap);
  setCursor(zoomCursor);
}

void QxGraph_CanvasView::activatePanning()
{
  myOperation = PANVIEW;
  viewport()->setMouseTracking(false);
  myCursor = cursor(); // save old cursor
  QCursor panCursor (Qt::SizeAllCursor);
  setCursor(panCursor);
}

void QxGraph_CanvasView::activateGlobalPanning()
{
  myOperation = PANGLOBAL;
  myCursor = cursor(); // save old cursor
  QPixmap globalPanPixmap (imageCrossCursor);
  QCursor glPanCursor (globalPanPixmap);
  setCursor(glPanCursor);
}

void QxGraph_CanvasView::activateReset()
{
  //myOperation = RESETVIEW;
  setContentsPos(0,0);
  QWMatrix m;
  setWorldMatrix(m);
  //myOperation = NOTHING;
}

void QxGraph_CanvasView::onTimeout() 
{
  if (myCurrentItem) {
    scrollBy(myDX, myDY);

    double cx, cy;
    inverseWorldMatrix().map((double)myDX, (double)myDY, &cx, &cy);
    if (myCurrentItem->x()+cx < 0) cx = -myCurrentItem->x();
    if (myCurrentItem->y()+cy < 0) cy = -myCurrentItem->y();
    myCurrentItem->moveBy(cx, cy);
    myPoint.setX(myPoint.x()+(int)cx);
    myPoint.setY(myPoint.y()+(int)cy);
    canvas()->update();
  }
}

QxGraph_ViewWindow* QxGraph_CanvasView::getViewWindow() const
{
  return dynamic_cast<QxGraph_ViewWindow*>( parent() );
}
