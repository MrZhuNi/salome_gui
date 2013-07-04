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

#include "GraphicsView_PrsImage.h"

#include "GraphicsView_PrsImageFrame.h"
#include "GraphicsView_ViewPort.h"

#include <QCursor>
#include <QGraphicsView>
#include <QPainter>
#include <QVector2D>

#include <math.h>

#define PREVIEW_Z_VALUE 2000
#define EPSILON         1e-6
#define PI              3.14159265359

//=======================================================================
// name    : GraphicsView_PrsImage
// Purpose : Constructor
//=======================================================================
GraphicsView_PrsImage::GraphicsView_PrsImage()
: GraphicsView_Object(),
  myIsLockAspectRatio( false ),
  myPixmapItem( 0 ),
  myPreviewPixmapItem( 0 ),
  myPrsImageFrame( 0 ),
  myPosX( 0.0 ),
  myPosY( 0.0 ),
  myScaleX( 1.0 ),
  myScaleY( 1.0 ),
  myRotationAngle( 0.0 ),
  myPreviewPosX( 0.0 ),
  myPreviewPosY( 0.0 ),
  myPreviewScaleX( 1.0 ),
  myPreviewScaleY( 1.0 ),
  myPreviewRotationAngle( 0.0 )
{
}

//=======================================================================
// name    : GraphicsView_PrsImage
// Purpose : Destructor
//=======================================================================
GraphicsView_PrsImage::~GraphicsView_PrsImage()
{
  if( myPrsImageFrame )
  {
    delete myPrsImageFrame;
    myPrsImageFrame = 0;
  }
}

//================================================================
// Function : updateTransform
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::updateTransform()
{
  QTransform aTransform = getTransform();

  setTransform( aTransform );
  myPrsImageFrame->setTransform( aTransform );

  // for anchors
  myPrsImageFrame->setScaling( myScaleX, myScaleY );
  myPrsImageFrame->setRotationAngle( myRotationAngle );

  aTransform = getTransform( true );
  myPreviewPixmapItem->setTransform( aTransform );
}

//================================================================
// Function : setImage
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::setImage( const QImage& theImage )
{
  myPixmap = QPixmap::fromImage( theImage );
}

//================================================================
// Function : getImage
// Purpose  : 
//================================================================
QImage GraphicsView_PrsImage::getImage() const
{
  return myPixmap.toImage();
}

//================================================================
// Function : getTransform
// Purpose  : 
//================================================================
QTransform GraphicsView_PrsImage::getTransform( const bool theIsPreview ) const
{
  double aPosX = theIsPreview ? myPreviewPosX : myPosX;
  double aPosY = theIsPreview ? myPreviewPosY : myPosY;
  double aScaleX = theIsPreview ? myPreviewScaleX : myScaleX;
  double aScaleY = theIsPreview ? myPreviewScaleY : myScaleY;
  double aRotationAngle = theIsPreview ? myPreviewRotationAngle : myRotationAngle;

  QTransform aTransform;
  aTransform.translate( aPosX, aPosY );
  aTransform.rotate( aRotationAngle );
  aTransform.scale( aScaleX, aScaleY );
  return aTransform;
}

//================================================================
// Function : setPosition
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::setPosition( const double thePosX, const double thePosY )
{
  myPosX = thePosX;
  myPosY = thePosY;
}

//================================================================
// Function : getPosition
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::getPosition( double& thePosX, double& thePosY ) const
{
  thePosX = myPosX;
  thePosY = myPosY;
}

//================================================================
// Function : setScaling
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::setScaling( const double theScaleX, const double theScaleY )
{
  myScaleX = theScaleX;
  myScaleY = theScaleY;
}

//================================================================
// Function : getScaling
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::getScaling( double& theScaleX, double& theScaleY ) const
{
  theScaleX = myScaleX;
  theScaleY = myScaleY;
}

//================================================================
// Function : setRotationAngle
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::setRotationAngle( const double theRotationAngle )
{
  myRotationAngle = theRotationAngle;
}

//================================================================
// Function : getRotationAngle
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::getRotationAngle( double& theRotationAngle ) const
{
  theRotationAngle = myRotationAngle;
}

//================================================================
// Function : setIsLockAspectRatio
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::setIsLockAspectRatio( const bool theIsLockAspectRatio )
{
  myIsLockAspectRatio = theIsLockAspectRatio;
}

//================================================================
// Function : getIsLockAspectRatio
// Purpose  : 
//================================================================
bool GraphicsView_PrsImage::getIsLockAspectRatio() const
{
  return myIsLockAspectRatio;
}

//================================================================
// Function : boundingRect
// Purpose  : 
//================================================================
QRectF GraphicsView_PrsImage::boundingRect() const
{
  return myPixmapItem->boundingRect();
}

//================================================================
// Function : compute
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::compute()
{
  if( !myPixmapItem )
  {
    myPixmapItem = new QGraphicsPixmapItem( this );
    addToGroup( myPixmapItem );
  }
  if( !myPreviewPixmapItem )
  {
    myPreviewPixmapItem = new QGraphicsPixmapItem();
    //addToGroup( myPreviewPixmapItem ); // don't add
  }
  if( !myPrsImageFrame )
  {
    myPrsImageFrame = new GraphicsView_PrsImageFrame();
    myPrsImageFrame->setPrsImage( this );
  }

  myPixmapItem->setPixmap( myPixmap );

  myPreviewPixmapItem->setPixmap( myPixmap );
  myPreviewPixmapItem->setVisible( false );

  myPrsImageFrame->compute();

  updateTransform();
}

//================================================================
// Function : addTo
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::addTo( GraphicsView_ViewPort* theViewPort )
{
  GraphicsView_Object::addTo( theViewPort );
  theViewPort->addItem( myPrsImageFrame );
  theViewPort->addItem( myPreviewPixmapItem );

  double aZValue = 0;
  GraphicsView_ObjectListIterator anIter( theViewPort->getObjects() );
  while( anIter.hasNext() )
  {
    if( GraphicsView_PrsImage* aPrs = dynamic_cast<GraphicsView_PrsImage*>( anIter.next() ) )
    {
      double aZValueRef = aPrs->zValue();
      aZValue = qMax( aZValue, aZValueRef );
    }
  }
  setZValue( aZValue + 1 );
}

//================================================================
// Function : removeFrom
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::removeFrom( GraphicsView_ViewPort* theViewPort )
{
  GraphicsView_Object::removeFrom( theViewPort );
  theViewPort->removeItem( myPrsImageFrame );
  theViewPort->removeItem( myPreviewPixmapItem );
}

//================================================================
// Function : checkHighlight
// Purpose  : 
//================================================================
bool GraphicsView_PrsImage::checkHighlight( double theX, double theY, QCursor& theCursor ) const
{
  QRect aRect = myPixmapItem->boundingRect().toRect();
  QPolygon aPolygon = sceneTransform().mapToPolygon( aRect );
  if( aPolygon.containsPoint( QPoint( theX, theY ), Qt::OddEvenFill ) )
  {
    theCursor = *getHighlightCursor();
    return true;
  }
  return false;
}

//================================================================
// Function : select
// Purpose  : 
//================================================================
bool GraphicsView_PrsImage::select( double theX, double theY, const QRectF& theRect )
{
  bool anIsSelected = GraphicsView_Object::select( theX, theY, theRect );
  myPrsImageFrame->updateVisibility();
  return anIsSelected;
}

//================================================================
// Function : unselect
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::unselect()
{
  GraphicsView_Object::unselect();
  myPrsImageFrame->updateVisibility();
}

//================================================================
// Function : setSelected
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::setSelected( bool theState )
{
  GraphicsView_Object::setSelected( theState );
  myPrsImageFrame->updateVisibility();
}

//================================================================
// Function : move
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::move( double theDX, double theDY, bool theIsAtOnce )
{
  if( theIsAtOnce )
  {
    finishMove();
    return;
  }

  if( !myIsMoving )
    enablePreview( true );

  myIsMoving = true;

  myPreviewPosX += theDX;
  myPreviewPosY += theDY;
  updateTransform();
}

//================================================================
// Function : finishMove
// Purpose  : 
//================================================================
bool GraphicsView_PrsImage::finishMove()
{
  if( myIsMoving )
  {
    myPosX = myPreviewPosX;
    myPosY = myPreviewPosY;
    updateTransform();

    enablePreview( false );
  }
  return GraphicsView_Object::finishMove();
}

//================================================================
// Function : processResize
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::processResize( const int theAnchor,
                                           const double theDX,
                                           const double theDY )
{
  if( fabs( theDX ) < EPSILON && fabs( theDY ) < EPSILON )
    return;

  if( !myPreviewPixmapItem->isVisible() )
    enablePreview( true );

  double anAngle = -1 * myRotationAngle * PI / 180.;

  QVector2D aVec( theDX, theDY );
  QVector2D aVecHor( 10, 0 );

  double aDist = sqrt( theDX * theDX + theDY * theDY );
  double anAngle1 = computeAngle( aVec, aVecHor );
  double anAngle2 = anAngle1 - anAngle;

  double aSizeDX = aDist * cos( anAngle2 );
  double aSizeDY = -aDist * sin( anAngle2 );

  QPointF aPosShift;
  QPointF aSizeShift;
  switch( theAnchor )
  {
    case GraphicsView_PrsImageFrame::Top:
      aPosShift = QPointF( aSizeDY * sin( anAngle ), aSizeDY * cos( anAngle ) );
      aSizeShift = QPointF( 0, -aSizeDY );
      break;
    case GraphicsView_PrsImageFrame::Bottom:
      aPosShift = QPointF( 0, 0 );
      aSizeShift = QPointF( 0, aSizeDY );
      break;
    case GraphicsView_PrsImageFrame::Left:
      aPosShift = QPointF( aSizeDX * cos( anAngle ), -aSizeDX * sin( anAngle ) );
      aSizeShift = QPointF( -aSizeDX, 0 );
      break;
    case GraphicsView_PrsImageFrame::Right:
      aPosShift = QPointF( 0, 0 );
      aSizeShift = QPointF( aSizeDX, 0 );
      break;
    case GraphicsView_PrsImageFrame::TopLeft:
      aPosShift = QPointF( theDX, theDY );
      aSizeShift = QPointF( -aSizeDX, -aSizeDY );
      break;
    case GraphicsView_PrsImageFrame::TopRight:
      aPosShift = QPointF( aSizeDY * sin( anAngle ), aSizeDY * cos( anAngle ) );
      aSizeShift = QPointF( aSizeDX, -aSizeDY );
      break;
    case GraphicsView_PrsImageFrame::BottomLeft:
      aPosShift = QPointF( aSizeDX * cos( anAngle ), -aSizeDX * sin( anAngle ) );
      aSizeShift = QPointF( -aSizeDX, aSizeDY );
      break;
    case GraphicsView_PrsImageFrame::BottomRight:
      aPosShift = QPointF( 0, 0 );
      aSizeShift = QPointF( aSizeDX, aSizeDY );
      break;
  }

  double aWidth = (double)myPixmap.width() * myScaleX;
  double aHeight = (double)myPixmap.height() * myScaleY;

  double aNewWidth = aWidth + aSizeShift.x();
  double aNewHeight = aHeight + aSizeShift.y();

  myPreviewScaleX = myScaleX * aNewWidth / aWidth;
  myPreviewScaleY = myScaleY * aNewHeight / aHeight;

  myPreviewPosX = myPosX + aPosShift.x();
  myPreviewPosY = myPosY + aPosShift.y();

  updateTransform();
}

//================================================================
// Function : finishResize
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::finishResize()
{
  myPosX = myPreviewPosX;
  myPosY = myPreviewPosY;

  myScaleX = myPreviewScaleX;
  myScaleY = myPreviewScaleY;
  updateTransform();

  enablePreview( false );
}

//================================================================
// Function : computeRotationAngle
// Purpose  : 
//================================================================
double GraphicsView_PrsImage::computeRotationAngle( const QPointF& thePoint1,
                                                    const QPointF& thePoint2 ) const
{
  QRectF aRect = getRect();
  QPointF aCenter = aRect.center();

  QVector2D aVec1( thePoint1 - aCenter );
  QVector2D aVec2( thePoint2 - aCenter );

  double anAngle = computeAngle( aVec1, aVec2 );
  double anAngleDeg = anAngle / PI * 180.;
  return anAngleDeg;
}

//================================================================
// Function : processRotate
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::processRotate( const double theAngle )
{
  if( fabs( theAngle ) < EPSILON )
    return;

  if( !myPreviewPixmapItem->isVisible() )
    enablePreview( true );

  myPreviewRotationAngle = myRotationAngle + theAngle;

  QPointF aCenter( (double)myPixmap.width() / 2.,
                   (double)myPixmap.height() / 2. );

  myPreviewPosX = myPosX;
  myPreviewPosY = myPosY;
  QTransform aTransform1 = getTransform();
  QTransform aTransform2 = getTransform( true );

  QPointF aPoint1 = aTransform1.map( aCenter );
  QPointF aPoint2 = aTransform2.map( aCenter );
  QPointF aDiff = aPoint2 - aPoint1;

  myPreviewPosX = myPosX - aDiff.x();
  myPreviewPosY = myPosY - aDiff.y();

  updateTransform();
}

//================================================================
// Function : finishRotate
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::finishRotate()
{
  myPosX = myPreviewPosX;
  myPosY = myPreviewPosY;
  myRotationAngle = myPreviewRotationAngle;
  updateTransform();

  enablePreview( false );
}

//================================================================
// Function : enablePreview
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::enablePreview( const bool theState )
{
  if( theState )
  {
    myPreviewPixmapItem->setZValue( PREVIEW_Z_VALUE );
    myPreviewPixmapItem->setOpacity( opacity() / 2. );

    myPreviewPosX = myPosX;
    myPreviewPosY = myPosY;
    myPreviewScaleX = myScaleX;
    myPreviewScaleY = myScaleY;
    myPreviewRotationAngle = myRotationAngle;

    myPreviewPixmapItem->setVisible( true );
  }
  else
    myPreviewPixmapItem->setVisible( false );
}

//================================================================
// Function : computeAngle
// Purpose  : 
//================================================================
double GraphicsView_PrsImage::computeAngle( const QVector2D& theVector1,
                                            const QVector2D& theVector2 )
{
  if( theVector1.isNull() || theVector2.isNull() )
    return 0.0;

  double aDotProduct = QVector2D::dotProduct( theVector1, theVector2 );
  double aCos = aDotProduct / theVector1.length() / theVector2.length();

  double aCrossProduct = theVector1.x() * theVector2.y() - theVector1.y() * theVector2.x();

  double anAngle = acos( aCos );
  if( aCrossProduct < 0 )
    anAngle *= -1;

  return anAngle;
}
