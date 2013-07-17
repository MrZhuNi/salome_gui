
#ifndef IMAGE_COMPOSER_FUSE_OPERATOR_HEADER
#define IMAGE_COMPOSER_FUSE_OPERATOR_HEADER

#include <ImageComposer_Operator.h>
#include <QColor>

/**
  \class ImageComposer_FuseOperator
  Implementation of the fusing operator
*/
class IMAGE_COMPOSER_API ImageComposer_FuseOperator : public ImageComposer_Operator
{
public:
  ImageComposer_FuseOperator();
  virtual ~ImageComposer_FuseOperator();

  virtual QString name() const;

protected:
  virtual QRectF calcResultBoundingRect( const QRectF& theImage1Bounds, 
                                         const QRectF& theImage2Bounds ) const;
  virtual void drawResult( QPainter& thePainter, const ImageComposer_Image& theImage1,
                                                 const ImageComposer_Image& theImage2 ) const;
};

#endif
