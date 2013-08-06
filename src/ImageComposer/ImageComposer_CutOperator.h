
#ifndef IMAGE_COMPOSER_CUT_OPERATOR_HEADER
#define IMAGE_COMPOSER_CUT_OPERATOR_HEADER

#include "ImageComposer_Operator.h"
#include <QColor>

/**
  \class ImageComposer_CutOperator
  Implementation of the cutting operator
*/
class IMAGE_COMPOSER_API ImageComposer_CutOperator : public ImageComposer_Operator
{
public:
  ImageComposer_CutOperator();
  virtual ~ImageComposer_CutOperator();

  virtual QString name() const;

protected:
  virtual QRectF calcResultBoundingRect( const QRectF& theImage1Bounds, 
                                         const QRectF& theImage2Bounds ) const;
  virtual void drawResult( QPainter& thePainter, const ImageComposer_Image& theImage1,
                                                 const ImageComposer_Image& theImage2 ) const;
};

#endif
