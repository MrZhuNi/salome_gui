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

#include <math.h>

#define FRAME_Z_VALUE   1000
#define ANCHOR_RADIUS   3
#define PI              3.14159265359

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
    for( int aType = TopMost; aType <= BottomRight; aType++ )
    {
      UnscaledGraphicsEllipseItem* anAnchorItem = new UnscaledGraphicsEllipseItem( this );

      Qt::GlobalColor aColor = Qt::white;
      if( aType == TopMost )
        aColor = Qt::green;
      anAnchorItem->setBrush( QBrush( aColor ) );

      myAnchorMap.insert( aType, anAnchorItem );

      addToGroup( anAnchorItem );
    }
  }

  setZValue( FRAME_Z_VALUE );

  computeAnchorItems();
  updateVisibility();
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
void GraphicsView_PrsImageFrame::pull( const QPointF& thePoint,
                                       GraphicsView_Object* theLockedObject, // unused
                                       const GraphicsView_ObjectList& theSyncObjects )
{
  if( !myPrsImage )
    return;

  if( thePoint == myPullingPoint )
    return;

  double aDX = thePoint.x() - myPullingPoint.x();
  double aDY = thePoint.y() - myPullingPoint.y();

  double aDAngle = myPrsImage->computeRotationAngle( myPullingPoint, thePoint );

  if( myPullingAnchor >= Top && myPullingAnchor <= BottomRight )
    myPrsImage->processResize( myPullingAnchor, aDX, aDY );
  else if( myPullingAnchor == TopMost )
    myPrsImage->processRotate( aDAngle );

  QRectF aRect = myPrsImage->getRect();
  double anAngle = 0;
  myPrsImage->getRotationAngle( anAngle );

  GraphicsView_ObjectListIterator anIter( theSyncObjects );
  while( anIter.hasNext() )
  {
    if( GraphicsView_PrsImage* aPrsImage = dynamic_cast<GraphicsView_PrsImage*>( anIter.next() ) )
    {
      if( aPrsImage != myPrsImage )
      {
        if( myPullingAnchor >= Top && myPullingAnchor <= BottomRight )
        {
          QRectF aRectRef = aPrsImage->getRect();
          double anAngleRef = 0;
          aPrsImage->getRotationAngle( anAngleRef );

          double aDXRef = aDX * aRectRef.width() / aRect.width();
          double aDYRef = aDY * aRectRef.height() / aRect.height();

          double anAngleDiff = ( anAngleRef - anAngle ) * PI / 180.;
          double aDXRefTrans = aDXRef * cos( anAngleDiff ) - aDYRef * sin( anAngleDiff );
          double aDYRefTrans = aDXRef * sin( anAngleDiff ) + aDYRef * cos( anAngleDiff );

          aPrsImage->processResize( myPullingAnchor, aDXRefTrans, aDYRefTrans );
        }
        else if( myPullingAnchor == TopMost )
          aPrsImage->processRotate( aDAngle );
      }
    }
  }
}

//================================================================
// Function : finishPulling
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::finishPulling( const GraphicsView_ObjectList& theSyncObjects )
{
  if( !myPrsImage )
    return;

  if( myPullingAnchor >= Top && myPullingAnchor <= BottomRight )
    myPrsImage->finishResize();
  else if( myPullingAnchor == TopMost )
    myPrsImage->finishRotate();

  GraphicsView_ObjectListIterator anIter( theSyncObjects );
  while( anIter.hasNext() )
  {
    if( GraphicsView_PrsImage* aPrsImage = dynamic_cast<GraphicsView_PrsImage*>( anIter.next() ) )
    {
      if( aPrsImage != myPrsImage )
      {
        if( myPullingAnchor >= Top && myPullingAnchor <= BottomRight )
          aPrsImage->finishResize();
        else if( myPullingAnchor == TopMost )
          aPrsImage->finishRotate();
      }
    }
  }
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
  anAnchorPointMap[ TopMost ] = ( aRect.topLeft() + aRect.topRight() ) / 2;
  anAnchorPointMap[ Top ] = ( aRect.topLeft() + aRect.topRight() ) / 2;
  anAnchorPointMap[ Bottom ] = ( aRect.bottomLeft() + aRect.bottomRight() ) / 2;
  anAnchorPointMap[ Left ] = ( aRect.topLeft() + aRect.bottomLeft() ) / 2;
  anAnchorPointMap[ Right ] = ( aRect.topRight() + aRect.bottomRight() ) / 2;
  anAnchorPointMap[ TopLeft ] = aRect.topLeft();
  anAnchorPointMap[ TopRight ] = aRect.topRight();
  anAnchorPointMap[ BottomLeft ] = aRect.bottomLeft();
  anAnchorPointMap[ BottomRight ] = aRect.bottomRight();

  qreal ar = ANCHOR_RADIUS;
  QMapIterator<int, QPointF> anIter( anAnchorPointMap );
  while( anIter.hasNext() )
  {
    int anAnchorType = anIter.next().key();
    const QPointF& anAnchorPoint = anIter.value();

    QRectF anAnchorRect( anAnchorPoint - QPointF( ar, ar ), QSizeF( ar * 2, ar * 2 ) );
    myAnchorMap[ anAnchorType ]->setRect( anAnchorRect );
    myAnchorMap[ anAnchorType ]->setBasePoint( anAnchorPoint );

    if( anAnchorType == TopMost )
      myAnchorMap[ anAnchorType ]->setOffset( QPointF( 0, -6 * ANCHOR_RADIUS ) );
  }
}

//================================================================
// Function : updateVisibility
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::updateVisibility()
{
  setVisible( myPrsImage && myPrsImage->isSelected() );
}

//================================================================
// Function : setScaling
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::setScaling( const double theScaleX,
                                             const double theScaleY )
{
  AnchorMapIterator anIter( myAnchorMap );
  while( anIter.hasNext() )
    if( UnscaledGraphicsEllipseItem* anAnchorItem = anIter.next().value() )
      anAnchorItem->setScaling( theScaleX, theScaleY );
}

//================================================================
// Function : setRotationAngle
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::setRotationAngle( const double theRotationAngle )
{
  AnchorMapIterator anIter( myAnchorMap );
  while( anIter.hasNext() )
    if( UnscaledGraphicsEllipseItem* anAnchorItem = anIter.next().value() )
      anAnchorItem->setRotationAngle( theRotationAngle );
}

//================================================================
// Function : getResizeCursor
// Purpose  : 
//================================================================
QCursor* GraphicsView_PrsImageFrame::getResizeCursor( const int theAnchor ) const
{
  if( !myPrsImage )
    return 0;

  double aScaleX, aScaleY, aRotationAngle;
  myPrsImage->getScaling( aScaleX, aScaleY );
  myPrsImage->getRotationAngle( aRotationAngle );

  int anAngle = (int)aRotationAngle; // -inf <= anAngle <= inf
  anAngle = anAngle % 360;           // -359 <= anAngle <= 359
  anAngle = ( anAngle + 360 ) % 360; //    0 <= anAngle <= 359

  int aSign = aScaleX * aScaleY < 0 ? -1 : 1;

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
  anAngle += aSign * aShift;         // -315 <= anAngle <= 674
  anAngle = ( anAngle + 360 ) % 360; //    0 <= anAngle <= 359

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
: QGraphicsEllipseItem( theParent ),
  myScaleX( 1.0 ),
  myScaleY( 1.0 ),
  myRotationAngle( 0.0 )
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
// Function : boundingRect
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem::
  setScaling( const double theScaleX, const double theScaleY )
{
  myScaleX = theScaleX;
  myScaleY = theScaleY;
}

//================================================================
// Function : boundingRect
// Purpose  : 
//================================================================
void GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem::
  setRotationAngle( const double theRotationAngle )
{
  myRotationAngle = theRotationAngle;
}

//================================================================
// Function : boundingRect
// Purpose  : 
//================================================================
QRectF GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem::boundingRect() const
{
  QRectF aRect = QGraphicsEllipseItem::boundingRect();

  GraphicsView_Object* aParent = dynamic_cast<GraphicsView_Object*>( parentItem() );
  if( !aParent )
    return aRect;

  QTransform aTransform = aParent->getViewTransform();
  double aScale = aTransform.m11(); // same as m22(), viewer specific
  if( fabs( aScale ) < 1e-10 ||
      fabs( myScaleX ) < 1e-10 ||
      fabs( myScaleY ) < 1e-10 )
    return aRect;

  QPointF aCenter = aRect.center();
  double aWidth = aRect.width() / aScale / myScaleX;
  double aHeight = aRect.height() / aScale / myScaleY;

  double anOffsetX = myOffset.x() / aScale / fabs( myScaleX );
  double anOffsetY = myOffset.y() / aScale / fabs( myScaleY );

  aRect = QRectF( aCenter.x() - aWidth / 2 + anOffsetX,
                  aCenter.y() - aHeight / 2 + anOffsetY,
                  aWidth, aHeight );

  // to do
  //QRectF aBaseRect( myBasePoint, QSizeF( 1, 1 ) );
  //aRect |= aBaseRect;

  return aRect;
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
  // draw a connection line (mainly, for top-most anchor)
  //thePainter->drawLine( myBasePoint, boundingRect().center() );

  thePainter->save();
  thePainter->setTransform( GenerateTranslationOnlyTransform( thePainter->transform(),
                                                              myBasePoint ) );

  double anOffsetX = myOffset.x();
  double anOffsetY = myOffset.y();
  if( myScaleX < 0 )
    anOffsetX *= -1;
  if( myScaleY < 0 )
    anOffsetY *= -1;

  double anAngle = myRotationAngle * PI / 180.;
  double aDX = anOffsetX * cos( anAngle ) - anOffsetY * sin( anAngle );
  double aDY = anOffsetX * sin( anAngle ) + anOffsetY * cos( anAngle );
  thePainter->translate( aDX, aDY );

  QGraphicsEllipseItem::paint( thePainter, theOption, theWidget );
  thePainter->restore();

  // for debug
  //thePainter->save();
  //thePainter->setPen( QPen( Qt::magenta ) );
  //thePainter->drawRect( boundingRect() );
  //thePainter->restore();
}
