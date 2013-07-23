
#include "ImageComposer_CutOperator.h"
#include "ImageComposer_Image.h"
#include <QString>
#include <QPixmap>
#include <QPainter>

/**
  Constructor
*/
ImageComposer_CutOperator::ImageComposer_CutOperator()
  : ImageComposer_Operator()
{
}

/**
*/
ImageComposer_CutOperator::~ImageComposer_CutOperator()
{
}

/**
*/
QString ImageComposer_CutOperator::name() const
{
  return "cut";
}

/**
*/
QRectF ImageComposer_CutOperator::calcResultBoundingRect( const QRectF& theImage1Bounds, 
                                                          const QRectF& ) const
{
  return theImage1Bounds;
}

/**
*/
void ImageComposer_CutOperator::drawResult( QPainter& thePainter,
                                            const ImageComposer_Image& theImage1,
                                            const ImageComposer_Image& theImage2 ) const
{
  theImage1.draw( thePainter );
  theImage2.draw( thePainter );
}
