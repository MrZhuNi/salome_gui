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
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

#include <gp_Vec2d.hxx>

#include <math.h>

#define PI 3.14159265359

//=======================================================================
// name    : GraphicsView_PrsImage
// Purpose : Constructor
//=======================================================================
GraphicsView_PrsImage::GraphicsView_PrsImage()
: GraphicsView_Object(),
  myPixmapItem( 0 ),
  myPreviewPixmapItem( 0 ),
  myImageFrame( 0 ),
  myScaleX( 1.0 ),
  myScaleY( 1.0 ),
  myRotationAngle( 0.0 ),
  myPreviewRotationAngle( 0.0 )
{
}

//=======================================================================
// name    : GraphicsView_PrsImage
// Purpose : Destructor
//=======================================================================
GraphicsView_PrsImage::~GraphicsView_PrsImage()
{
  if( myPreviewPixmapItem )
  {
    delete myPreviewPixmapItem;
    myPreviewPixmapItem = 0;
  }

  if( myImageFrame )
  {
    delete myImageFrame;
    myImageFrame = 0;
  }

  QListIterator<QGraphicsItem*> aChildIter( children() );
  while( aChildIter.hasNext() )
  {
    if( QGraphicsItem* aChild = aChildIter.next() )
    {
      removeFromGroup( aChild );
      if( QGraphicsScene* aScene = aChild->scene() )
        aScene->removeItem( aChild );
      delete aChild;
      aChild = 0;
    }
  }
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
  QImage anImage = myPixmap.toImage();
  return anImage;
}

//================================================================
// Function : getTransformation
// Purpose  : 
//================================================================
QTransform GraphicsView_PrsImage::getTransform() const
{
  double aPosX, aPosY, aScaleX, aScaleY, aRotationAngle;
  getPosition( aPosX, aPosY );
  getScaling( aScaleX, aScaleY );
  getRotationAngle( aRotationAngle );

  QTransform aTransform;
  aTransform.translate( aPosX, aPosY );
  aTransform.scale( aScaleX, aScaleY );
  aTransform.rotate( aRotationAngle );
  return aTransform;
}

//================================================================
// Function : setPosition
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::setPosition( const double thePosX, const double thePosY )
{
  setPos( thePosX, thePosY );
}

//================================================================
// Function : getPosition
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::getPosition( double& thePosX, double& thePosY ) const
{
  thePosX = pos().x();
  thePosY = pos().y();
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
  if( !myImageFrame )
  {
    myImageFrame = new GraphicsView_PrsImageFrame();
    myImageFrame->setPrsImage( this );
  }

  setZValue( 0 );

  QSize aSourceSize = myPixmap.size();
  QSize aScaledSize( aSourceSize.width() * myScaleX,
                     aSourceSize.height() * myScaleY );

  QPixmap aPixmap = myPixmap.scaled( aScaledSize );
  myPixmapItem->setPixmap( aPixmap );

  myPreviewPixmapItem->setPixmap( aPixmap );
  myPreviewPixmapItem->setVisible( false );

  setRotationAroundCenter( this, myRotationAngle );

  myImageFrame->compute();
}

//================================================================
// Function : addTo
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::addTo( GraphicsView_ViewPort* theViewPort )
{
  GraphicsView_Object::addTo( theViewPort );
  theViewPort->addItem( myImageFrame );
  theViewPort->addItem( myPreviewPixmapItem );
}

//================================================================
// Function : removeFrom
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::removeFrom( GraphicsView_ViewPort* theViewPort )
{
  GraphicsView_Object::removeFrom( theViewPort );
  theViewPort->removeItem( myImageFrame );
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
// Function : highlight
// Purpose  : 
//================================================================
bool GraphicsView_PrsImage::highlight( double theX, double theY )
{
  bool anIsHighlighted = GraphicsView_Object::highlight( theX, theY );
  return anIsHighlighted;
}

//================================================================
// Function : unhighlight
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::unhighlight()
{
  GraphicsView_Object::unhighlight();
}

//================================================================
// Function : select
// Purpose  : 
//================================================================
bool GraphicsView_PrsImage::select( double theX, double theY, const QRectF& theRect )
{
  bool anIsSelected = GraphicsView_Object::select( theX, theY, theRect );
  myImageFrame->updateAnchorItems();
  return anIsSelected;
}

//================================================================
// Function : unselect
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::unselect()
{
  GraphicsView_Object::unselect();
  myImageFrame->updateAnchorItems();
}

//================================================================
// Function : setSelected
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::setSelected( bool theState )
{
  GraphicsView_Object::setSelected( theState );
  myImageFrame->updateAnchorItems();
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
  myPreviewPixmapItem->moveBy( theDX, theDY );
}

//================================================================
// Function : finishMove
// Purpose  : 
//================================================================
bool GraphicsView_PrsImage::finishMove()
{
  if( myIsMoving )
  {
    setPos( myPreviewPixmapItem->pos() );
    myImageFrame->setPos( pos() );

    enablePreview( false );
  }
  return GraphicsView_Object::finishMove();
}

//================================================================
// Function : centerPoint
// Purpose  : 
//================================================================
QPointF GraphicsView_PrsImage::centerPoint()
{
  QPointF aPoint = myPixmapItem->boundingRect().center();
  return aPoint;
}

//================================================================
// Function : processResize
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::processResize( const int theAnchor,
                                           const QPointF& thePoint1,
                                           const QPointF& thePoint2 )
{
  if( thePoint1 == thePoint2 )
    return;

  if( !myPreviewPixmapItem->isVisible() )
    enablePreview( true );

  QPixmap aCurrentPixmap = myPixmapItem->pixmap();
  int aWidth = aCurrentPixmap.width();
  int aHeight = aCurrentPixmap.height();

  double aDX = thePoint2.x() - thePoint1.x();
  double aDY = thePoint2.y() - thePoint1.y();

  double anAngle = -1 * myRotationAngle * PI / 180.;

  gp_Pnt2d aPnt1( thePoint1.x(), thePoint1.y() );
  gp_Pnt2d aPnt2( thePoint2.x(), thePoint2.y() );
  gp_Vec2d aVec( aPnt1, aPnt2 );
  gp_Vec2d aVecHor( 10, 0 );

  double aDist = aPnt1.Distance( aPnt2 );
  double anAngle1 = aVec.Angle( aVecHor );
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
      aPosShift = QPointF( aDX, aDY );
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

  aWidth += (int)aSizeShift.x();
  aHeight += (int)aSizeShift.y();
  if( aWidth < 10 || aHeight < 10 ) // tmp
    return;

  QPixmap aPixmap = myPixmap.scaled( aWidth, aHeight );
  myPreviewPixmapItem->setPixmap( aPixmap );

  myPreviewPixmapItem->setPos( pos() + aPosShift );
}

//================================================================
// Function : finishResize
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::finishResize()
{
  QSize aSourceSize = myPixmap.size();
  QSize aScaledSize = myPreviewPixmapItem->pixmap().size();

  myScaleX = ( (double)aScaledSize.width() ) / ( (double)aSourceSize.width() );
  myScaleY = ( (double)aScaledSize.height() ) / ( (double)aSourceSize.height() );

  QPointF aSceneCenter = myPixmapItem->sceneBoundingRect().center();
  QPointF aPreviewSceneCenter = myPreviewPixmapItem->sceneBoundingRect().center();

  QPointF aCenter = myPixmapItem->pixmap().rect().center();
  QPointF aPreviewCenter = myPreviewPixmapItem->pixmap().rect().center();

  QPointF aCenterShift = aSceneCenter - aCenter;
  QPointF aPreviewCenterShift = aPreviewSceneCenter - aPreviewCenter;

  QPointF aPosShift = myPreviewPixmapItem->pos() - pos();
  QPointF aShift = aPreviewCenterShift - aCenterShift - aPosShift;

  myPixmapItem->setPixmap( myPreviewPixmapItem->pixmap() );
  myImageFrame->computeAnchorItems();

  setPos( myPreviewPixmapItem->pos() + aShift );
  setRotationAroundCenter( this, myRotationAngle );

  myImageFrame->setPos( myPreviewPixmapItem->pos() + aShift );
  setRotationAroundCenter( myImageFrame, myRotationAngle );

  enablePreview( false );
}

//================================================================
// Function : processRotate
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::processRotate( const QPointF& thePoint1,
                                           const QPointF& thePoint2 )
{
  if( thePoint1 == thePoint2 )
    return;

  if( !myPreviewPixmapItem->isVisible() )
    enablePreview( true );

  QRectF aRect = getRect();
  QPointF aCenter = aRect.center();

  gp_Pnt2d aPnt0( aCenter.x(), aCenter.y() );
  gp_Pnt2d aPnt1( thePoint1.x(), thePoint1.y() );
  gp_Pnt2d aPnt2( thePoint2.x(), thePoint2.y() );

  gp_Vec2d aVec1( aPnt0, aPnt1 );
  gp_Vec2d aVec2( aPnt0, aPnt2 );

  double anAngle = aVec1.Angle( aVec2 );
  double anAngleDeg = anAngle / PI * 180.;

  myPreviewRotationAngle = myRotationAngle + anAngleDeg;
  setRotationAroundCenter( myPreviewPixmapItem, myPreviewRotationAngle );
}

//================================================================
// Function : finishRotate
// Purpose  : 
//================================================================
void GraphicsView_PrsImage::finishRotate()
{
  myRotationAngle = myPreviewRotationAngle;
  setRotationAroundCenter( this, myRotationAngle );
  setRotationAroundCenter( myImageFrame, myRotationAngle );

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
    myPreviewPixmapItem->setZValue( 100 );
    myPreviewPixmapItem->setOpacity( opacity() / 2. );

    myPreviewPixmapItem->setPos( pos() );
    setRotationAroundCenter( myPreviewPixmapItem, myRotationAngle );
    myPreviewRotationAngle = myRotationAngle;

    myPreviewPixmapItem->setVisible( true );
  }
  else
    myPreviewPixmapItem->setVisible( false );
}
