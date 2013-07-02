
#include <ImageComposer_CropOperator.h>
#include <ImageComposer_Image.h>
#include <QPixmap>
#include <QPainter>

/**
  Constructor
  @param theBackground the background color for result image
  @param theRect the cropping rectangle (in the global CS)
*/
ImageComposer_CropOperator::ImageComposer_CropOperator( const QColor& theBackground, const QRect& theRect )
: ImageComposer_Operator( theBackground )
{
  myClipPath.addRect( theRect );
}

/**
  Constructor
  @param theBackground the background color for result image
  @param thePath the cropping path (in the global CS)
*/
ImageComposer_CropOperator::ImageComposer_CropOperator( const QColor& theBackground, const QPainterPath& thePath )
: ImageComposer_Operator( theBackground )
{
  myClipPath = thePath;
}

/**
*/
ImageComposer_CropOperator::~ImageComposer_CropOperator()
{
}

/**
  Return clipping path
  @return clipping path
*/
QPainterPath ImageComposer_CropOperator::clipPath() const
{
  return myClipPath;
}

/**
*/
QString ImageComposer_CropOperator::name() const
{
  return "crop";
}

/**
*/
QRectF ImageComposer_CropOperator::calcResultBoundingRect( const QRectF&, const QRectF& ) const
{
  return myImgClipPath.boundingRect();
}

/**
*/
void ImageComposer_CropOperator::drawResult( QPainter& thePainter,
                                             const ImageComposer_Image& theImage1,
                                             const ImageComposer_Image& ) const
{
  QRectF aBounds = myImgClipPath.boundingRect();
  QTransform aTranslate;
  aTranslate.translate( -aBounds.left(), -aBounds.top() );
  QPainterPath aClipPath = aTranslate.map( myImgClipPath );
  thePainter.setClipPath( aClipPath );
  theImage1.draw( thePainter );
  //thePainter.fillPath( aClipPath, Qt::red );
}

/**
*/
ImageComposer_Image ImageComposer_CropOperator::process( const ImageComposer_Image& theImage1,
                                                         const ImageComposer_Image& theImage2 ) const
{
  QRect anImageRect( 0, 0, theImage1.width(), theImage1.height() );
  QPainterPath anImageBoundsPath;
  anImageBoundsPath.addPolygon( theImage1.transform().mapToPolygon( anImageRect ) );

  const_cast<ImageComposer_CropOperator*>( this )->myImgClipPath =
    theImage1.transform().inverted().map( myClipPath.intersected( anImageBoundsPath ) );

  return ImageComposer_Operator::process( theImage1, theImage2 );
}
