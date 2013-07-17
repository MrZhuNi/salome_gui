
#include <ImageComposer_FuseOperator.h>
#include <ImageComposer_Image.h>
#include <QString>
#include <QPixmap>
#include <QPainter>

/**
  Constructor
*/
ImageComposer_FuseOperator::ImageComposer_FuseOperator()
  : ImageComposer_Operator()
{
}

/**
*/
ImageComposer_FuseOperator::~ImageComposer_FuseOperator()
{
}

/**
*/
QString ImageComposer_FuseOperator::name() const
{
  return "fuse";
}

/**
*/
QRectF ImageComposer_FuseOperator::calcResultBoundingRect( const QRectF& theImage1Bounds, 
                                                           const QRectF& theImage2Bounds ) const
{
  return theImage1Bounds.united( theImage2Bounds );
}

/**
*/
void ImageComposer_FuseOperator::drawResult( QPainter& thePainter,
                                             const ImageComposer_Image& theImage1,
                                             const ImageComposer_Image& theImage2 ) const
{
  theImage1.draw( thePainter );
  theImage2.draw( thePainter );
}
