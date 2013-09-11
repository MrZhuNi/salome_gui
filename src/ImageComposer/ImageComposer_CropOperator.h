
#ifndef IMAGE_COMPOSER_CROP_OPERATOR_HEADER
#define IMAGE_COMPOSER_CROP_OPERATOR_HEADER

#include "ImageComposer_Operator.h"
#include <QColor>
#include <QPainterPath>

/**
  \class ImageComposer_CropOperator
  Implementation of the cropping operator
*/
class IMAGE_COMPOSER_API ImageComposer_CropOperator : public ImageComposer_Operator
{
public:
  ImageComposer_CropOperator();
  virtual ~ImageComposer_CropOperator();

  static QString Type() { return "crop"; }

  virtual QString name() const;

  virtual ImageComposer_Image process( const QVariant& theObj1,
                                       const QVariant& theObj2 ) const;

protected:
  virtual QRectF calcResultBoundingRect( const QVariant& theObj1, 
                                         const QVariant& theObj2 ) const;
  virtual void drawResult( QPainter& thePainter, const QVariant& theObj1,
                                                 const QVariant& theObj2 ) const;

private:
  friend class ImageComposerTests_TestOperators;
};

#endif
