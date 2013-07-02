
#include <ImageComposer_CutOperator.h>
#include <ImageComposer_Image.h>
#include <QString>
#include <QPixmap>
#include <QPainter>

/**
  Constructor
  @param theBackground the background color for result image
*/
ImageComposer_CutOperator::ImageComposer_CutOperator( const QColor& theBackground )
  : ImageComposer_Operator( theBackground )
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
