
#include <ImageComposer_Image.h>
#include <ImageComposer_CropOperator.h>
#include <ImageComposer_CutOperator.h>
#include <ImageComposer_FuseOperator.h>
#include <QPainter>

QColor ImageComposer_Image::myDefaultBackground = TRANSPARENT;

/**
  Constructor
*/
ImageComposer_Image::ImageComposer_Image()
{
}

/**
  Destructor
*/
ImageComposer_Image::~ImageComposer_Image()
{
}

/**
  Get current image transformation
  @return current image transformation
*/
QTransform ImageComposer_Image::transform() const
{
  return myTransform;
}

/**
  Change current image transformation
  @param theTransform a new image transformation
*/
void ImageComposer_Image::setTransform( const QTransform& theTransform )
{
  myTransform = theTransform;
}

/**
  Change current image transformation
  @param theDx the image translation along X axis of the global CS
  @param theDy the image translation along Y axis of the global CS
  @param theRotationDeg the image rotation in degrees around the image center
*/
void ImageComposer_Image::setLocalTransform( qreal theDx, qreal theDy, qreal theRotationDeg )
{
  QTransform aTr;
  aTr.translate( width()*0.5, height()*0.5 );
  aTr.rotate( theRotationDeg );
  aTr.translate( -width()*0.5, -height()*0.5 );
  aTr.translate( theDx, theDy );
  myTransform = aTr;
}

/**
  Get image's bounding rectangle in the global CS
  @return image's bounding rectangle in the global CS
*/
QRectF ImageComposer_Image::boundingRect() const
{
  QRect aRect( 0, 0, width(), height() );
  return myTransform.mapToPolygon( aRect ).boundingRect();
}

/**
  Draw the image using the given painter
  @param thePainter the painter for image drawing
*/
void ImageComposer_Image::draw( QPainter& thePainter ) const
{
  thePainter.save();
  thePainter.setTransform( myTransform, true );
  thePainter.drawImage( 0, 0, *this );
  thePainter.restore();
}

/**
  Operator of another image assignment
  @param theImage the image to assign
  @return the assigned image
*/
const ImageComposer_Image& ImageComposer_Image::operator = ( const ImageComposer_Image& theImage )
{
  myTransform = theImage.myTransform;
  QImage::operator = ( ( const QImage& ) theImage );
  return theImage;
}

/**
  Operator of another image assignment
  @param theImage the image to assign
  @return the assigned image
*/
const QImage& ImageComposer_Image::operator = ( const QImage& theImage )
{
  QImage::operator = ( ( const QImage& ) theImage );
  return theImage;
}

/**
  Apply the given operator to the image
  @param theOperator the operator to be applied
  @param theImage the additional image to compose (optional)
  @return the result image
*/
ImageComposer_Image ImageComposer_Image::apply( const ImageComposer_Operator& theOperator,
                                                const ImageComposer_Image& theImage ) const
{
  return theOperator.process( *this, theImage );
}

/**
  Get default background color used for image operators
  @return default background color 
*/
QColor ImageComposer_Image::defaultBackground()
{
  return myDefaultBackground;
}

/**
  Change default background color used for image operators
  @param theDefaultBackground a new default background color 
*/
void ImageComposer_Image::setDefaultBackground( const QColor& theDefaultBackground )
{
  myDefaultBackground = theDefaultBackground;
}

/**
  Operator of image cropping by a rectangle
  @param theRect the rectangle for cropping
  @return cropped image
*/
ImageComposer_Image ImageComposer_Image::operator & ( const QRect& theRect ) const
{
  return apply( ImageComposer_CropOperator( myDefaultBackground, theRect ) );
}

/**
  Operator of image cropping by a path
  @param thePath the path for cropping
  @return cropped image
*/
ImageComposer_Image ImageComposer_Image::operator & ( const QPainterPath& thePath ) const
{
  return apply( ImageComposer_CropOperator( myDefaultBackground, thePath ) );
}

/**
  Operator of image cutting by another image
  @param theImage the image for cutting
  @return cut image
*/
ImageComposer_Image ImageComposer_Image::operator & ( const ImageComposer_Image& theImage ) const
{
  return apply( ImageComposer_CutOperator( myDefaultBackground ), theImage );
}

/**
  Operator of image fusing with another image
  @param theImage the image for fusing
  @return fused image
*/
ImageComposer_Image ImageComposer_Image::operator | ( const ImageComposer_Image& theImage ) const
{
  return apply( ImageComposer_FuseOperator( myDefaultBackground ), theImage );
}
