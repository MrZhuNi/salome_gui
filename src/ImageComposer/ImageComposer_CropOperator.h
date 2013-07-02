
#ifndef IMAGE_COMPOSER_CROP_OPERATOR_HEADER
#define IMAGE_COMPOSER_CROP_OPERATOR_HEADER

#include <ImageComposer_Operator.h>
#include <QColor>
#include <QPainterPath>

/**
  \class ImageComposer_CropOperator
  Implementation of the cropping operator
*/
class IMAGE_COMPOSER_API ImageComposer_CropOperator : public ImageComposer_Operator
{
public:
  ImageComposer_CropOperator( const QColor& theBackground, const QRect& );
  ImageComposer_CropOperator( const QColor& theBackground, const QPainterPath& );
  virtual ~ImageComposer_CropOperator();

  QPainterPath clipPath() const;

  virtual QString name() const;
  virtual ImageComposer_Image process( const ImageComposer_Image& theImage1,
                                       const ImageComposer_Image& theImage2 ) const;

protected:
  virtual QRectF calcResultBoundingRect( const QRectF& theImage1Bounds, 
                                         const QRectF& theImage2Bounds ) const;
  virtual void drawResult( QPainter& thePainter, const ImageComposer_Image& theImage1,
                                                 const ImageComposer_Image& theImage2 ) const;

private:
  QPainterPath myClipPath;     ///< the clipping path specified initially
  QPainterPath myImgClipPath;  ///< the clipping path mapped to first image's local CS
};

#endif
