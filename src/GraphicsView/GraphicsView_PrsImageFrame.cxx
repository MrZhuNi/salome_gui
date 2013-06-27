// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "GraphicsView_PrsImageFrame.h"

#include "GraphicsView_PrsImage.h"

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>

#include <QCursor>
#include <QPainter>

#define ANCHOR_RADIUS 10

//=======================================================================
// name    : GraphicsView_PrsImageFrame
// Purpose : Constructor
//=======================================================================
GraphicsView_PrsImageFrame::GraphicsView_PrsImageFrame()
: GraphicsView_Object(),
  myPrsImage( 0 ),
  myIsPulling( false ),
  myPullingAnchor( Undefined )
{
  myVerCursor = new QCursor( Qt::SizeVerCursor );
  myHorCursor = new QCursor( Qt::SizeHorCursor );
  myBDiagCursor = new QCursor( Qt::SizeBDiagCursor );
  myFDiagCursor = new QCursor( Qt::SizeFDiagCursor );

  SUIT_ResourceMgr* rmgr = SUIT_Session::session()->resourceMgr();
  myRotateCursor = new QCursor( rmgr->loadPixmap( "GraphicsView", QObject::tr( "ICON_GV_ROTATE" ) ) );
}

//=======================================================================
// name    : GraphicsView_PrsImageFrame
// Purpose : Destructor
//=======================================================================
GraphicsView_PrsImageFrame::~GraphicsView_PrsImageFrame()
{
  if( myVerCursor )
  {
    delete myVerCursor;
    myVerCursor = 0;
  }
  if( myHorCursor )
  {
    delete myHorCursor;
    myHorCursor = 0;
  }
  if( myBDiagCursor )
  {
    delete myBDiagCursor;
    myBDiagCursor = 0;
  }
  if( myFDiagCursor )
  {
    delete myFDiagCursor;
    myFDiagCursor = 0;
  }
  if( myRotateCursor )
  {
    delete myRotateCursor;
    myRotateCursor = 0;
  }
}

//================================================================
// Function : boundingRect
// Purpose  : 
//================================================================
QRectF GraphicsView_PrsImageFrame::boundingRect() const
{
  QRectF aRect;
  AnchorMapIterator anIter( myAnchorMap );
  while( anIter.hasNext() )
  {
    if( QGraphicsEllipseItem* anAnchorItem = anIter.next().value() )
    {
      QRectF anAnchorRect = anAnchorItem->boundingRect();
      if( !anAnchorRect.isNull() )
      {
        if( aRect.isNull() )
          aRect = anAnchorRect;
        else
          aRect |= anAnchorRect;
      }
    }
  }
  return aRect;
}

//================================================================
// Function : compute
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::compute()
{
  if( myAnchorMap.isEmpty() )
  {
    for( int aType = Top; aType <= TopMost; aType++ )
    {
      UnscaledGraphicsEllipseItem* anAnchorItem = new UnscaledGraphicsEllipseItem( this );
      //anAnchorItem->setFlag( QGraphicsItem::ItemIgnoresTransformations, true );

      Qt::GlobalColor aColor = Qt::white;
      if( aType == TopMost )
        aColor = Qt::green;
      anAnchorItem->setBrush( QBrush( aColor ) );

      myAnchorMap.insert( aType, anAnchorItem );

      addToGroup( anAnchorItem );
    }
  }

  setZValue( 1000 );

  computeAnchorItems();
  updateAnchorItems();
}

//================================================================
// Function : checkHighlight
// Purpose  : 
//================================================================
bool GraphicsView_PrsImageFrame::checkHighlight( double theX, double theY, QCursor& theCursor ) const
{
  AnchorMapIterator anIter( myAnchorMap );
  while( anIter.hasNext() )
  {
    int aType = anIter.next().key();
    if( QGraphicsEllipseItem* anAnchorItem = anIter.value() )
    {
      QRectF aRect = anAnchorItem->sceneBoundingRect();
      if( aRect.contains( QPointF( theX, theY ) ) )
      {
        if( aType >= Top && aType <= BottomRight )
        {
          if( QCursor* aCursor = getResizeCursor( aType ) )
            theCursor = *aCursor;
        }
        else if( aType == TopMost )
          theCursor = *getRotateCursor();
        return true;
      }
    }
  }
  return false;
}

//================================================================
// Function : getPullingRect
// Purpose  : 
//================================================================
QRectF GraphicsView_PrsImageFrame::getPullingRect() const
{
  return getRect();
}

//================================================================
// Function : startPulling
// Purpose  : 
//================================================================
bool GraphicsView_PrsImageFrame::startPulling( const QPointF& thePoint )
{
  if( !isVisible() )
    return false;

  AnchorMapIterator anIter( myAnchorMap );
  while( anIter.hasNext() )
  {
    int aType = anIter.next().key();
    if( UnscaledGraphicsEllipseItem* anAnchorItem = anIter.value() )
    {
      QRectF aRect = anAnchorItem->sceneBoundingRect();
      if( aRect.contains( thePoint ) )
      {
        myPullingAnchor = aType;
        myPullingPoint = sceneTransform().map( anAnchorItem->getBasePoint() );
        return true;
      }
    }
  }
  return false;
}

//================================================================
// Function : pull
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::pull( const QPointF& thePoint, GraphicsView_Object* )
{
  if( !myPrsImage )
    return;

  if( myPullingAnchor >= Top && myPullingAnchor <= BottomRight )
    myPrsImage->processResize( myPullingAnchor, myPullingPoint, thePoint );
  else if( myPullingAnchor == TopMost )
    myPrsImage->processRotate( myPullingPoint, thePoint );
}

//================================================================
// Function : finishPulling
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::finishPulling()
{
  if( !myPrsImage )
    return;

  if( myPullingAnchor >= Top && myPullingAnchor <= BottomRight )
    myPrsImage->finishResize();
  else if( myPullingAnchor == TopMost )
    myPrsImage->finishRotate();
}

//================================================================
// Function : centerPoint
// Purpose  : 
//================================================================
QPointF GraphicsView_PrsImageFrame::centerPoint()
{
  QPointF aPoint1 = myAnchorMap[ Top ]->getBasePoint();
  QPointF aPoint2 = myAnchorMap[ Bottom ]->getBasePoint();
  QPointF aPoint3 = myAnchorMap[ Left ]->getBasePoint();
  QPointF aPoint4 = myAnchorMap[ Right ]->getBasePoint();
  return ( aPoint1 + aPoint2 + aPoint3 + aPoint4 ) /4.;
}

//================================================================
// Function : setPrsImage
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::setPrsImage( GraphicsView_PrsImage* thePrsImage )
{
  myPrsImage = thePrsImage;
}

//================================================================
// Function : computeAnchorItems
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::computeAnchorItems()
{
  if( !myPrsImage )
    return;

  QRectF aRect = myPrsImage->boundingRect();

  QMap<int, QPointF> anAnchorPointMap;
  anAnchorPointMap[ Top ] = ( aRect.topLeft() + aRect.topRight() ) / 2;
  anAnchorPointMap[ Bottom ] = ( aRect.bottomLeft() + aRect.bottomRight() ) / 2;
  anAnchorPointMap[ Left ] = ( aRect.topLeft() + aRect.bottomLeft() ) / 2;
  anAnchorPointMap[ Right ] = ( aRect.topRight() + aRect.bottomRight() ) / 2;
  anAnchorPointMap[ TopLeft ] = aRect.topLeft();
  anAnchorPointMap[ TopRight ] = aRect.topRight();
  anAnchorPointMap[ BottomLeft ] = aRect.bottomLeft();
  anAnchorPointMap[ BottomRight ] = aRect.bottomRight();

  // tmp
  anAnchorPointMap[ TopMost ] = ( aRect.topLeft() + aRect.topRight() ) / 2 -
                                QPointF( 0, 4 * ANCHOR_RADIUS );

  qreal ar = ANCHOR_RADIUS;
  QMapIterator<int, QPointF> anIter( anAnchorPointMap );
  while( anIter.hasNext() )
  {
    int anAnchorType = anIter.next().key();
    const QPointF& anAnchorPoint = anIter.value();

    QRectF anAnchorRect( anAnchorPoint - QPointF( ar, ar ), QSizeF( ar * 2, ar * 2 ) );
    myAnchorMap[ anAnchorType ]->setRect( anAnchorRect );
    myAnchorMap[ anAnchorType ]->setBasePoint( anAnchorPoint );
  }
}

//================================================================
// Function : updateAnchorItems
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::updateAnchorItems()
{
  if( !myPrsImage )
    return;

  bool anIsSelected = myPrsImage->isSelected();
  if( anIsSelected )
  {
    double aPosX, aPosY, aRotationAngle;
    myPrsImage->getPosition( aPosX, aPosY );
    myPrsImage->getRotationAngle( aRotationAngle );

    setPos( aPosX, aPosY );
    setRotationAroundCenter( this, aRotationAngle );
  }
  setVisible( anIsSelected );
}

//================================================================
// Function : getResizeCursor
// Purpose  : 
//================================================================
QCursor* GraphicsView_PrsImageFrame::getResizeCursor( const int theAnchor ) const
{
  if( !myPrsImage )
    return 0;

  double aRotationAngle = 0;
  myPrsImage->getRotationAngle( aRotationAngle );

  int anAngle = (int)aRotationAngle;
  anAngle = anAngle % 360;
  anAngle += 360; // to avoid negative values
  anAngle = anAngle % 360; // 0 <= anAngle <= 359

  int aShift = 0;
  switch( theAnchor )
  {
    case Top:         aShift = 0; break;
    case TopRight:    aShift = 45; break;
    case Right:       aShift = 90; break;
    case BottomRight: aShift = 135; break;
    case Bottom:      aShift = 180; break;
    case BottomLeft:  aShift = 225; break;
    case Left:        aShift = 270; break;
    case TopLeft:     aShift = 315; break;
  }
  anAngle += aShift;
  anAngle = anAngle % 360;

  // 360 = 8 sectors of 45 degrees
  if( anAngle <= 22 || anAngle >= 338 )
    return getVerCursor();
  if( anAngle >= 23 && anAngle <= 67 )
    return getBDiagCursor();
  if( anAngle >= 68 && anAngle <= 112 )
    return getHorCursor();
  if( anAngle >= 113 && anAngle <= 157 )
    return getFDiagCursor();
  if( anAngle >= 158 && anAngle <= 202 )
    return getVerCursor();
  if( anAngle >= 203 && anAngle <= 247 )
    return getBDiagCursor();
  if( anAngle >= 248 && anAngle <= 292 )
    return getHorCursor();
  if( anAngle >= 293 && anAngle <= 337 )
    return getFDiagCursor();
  return 0;
}

//=======================================================================
// name    : GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem
// Purpose : Constructor
//=======================================================================
GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem::UnscaledGraphicsEllipseItem( QGraphicsItem* theParent )
: QGraphicsEllipseItem( theParent )
{
}

//=======================================================================
// name    : GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem
// Purpose : Destructor
//=======================================================================
GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem::~UnscaledGraphicsEllipseItem()
{
}

//================================================================
// Function : GenerateTranslationOnlyTransform
// Purpose  : 
//================================================================
static QTransform GenerateTranslationOnlyTransform( const QTransform &theOriginalTransform,
                                                    const QPointF &theTargetPoint )
{
  qreal dx = theOriginalTransform.m11() * theTargetPoint.x() - theTargetPoint.x() +
             theOriginalTransform.m21() * theTargetPoint.y() +
             theOriginalTransform.m31();
  qreal dy = theOriginalTransform.m22() * theTargetPoint.y() - theTargetPoint.y() +
             theOriginalTransform.m12() * theTargetPoint.x() +
             theOriginalTransform.m32();
  return QTransform::fromTranslate( dx, dy );
}

//================================================================
// Function : paint
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem::paint(
  QPainter* thePainter,
  const QStyleOptionGraphicsItem* theOption,
  QWidget* theWidget )
{
  //thePainter->save();
  //thePainter->setTransform( GenerateTranslationOnlyTransform( thePainter->transform(),
  //                                                            myBasePoint ) );
  QGraphicsEllipseItem::paint( thePainter, theOption, theWidget );
  //thePainter->restore();
}
