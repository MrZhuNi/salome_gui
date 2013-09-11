
#ifndef IMAGE_COMPOSER_FUSE_OPERATOR_HEADER
#define IMAGE_COMPOSER_FUSE_OPERATOR_HEADER

#include "ImageComposer_Operator.h"
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

  static QString Type() { return "fuse"; }

  virtual QString name() const;

protected:
  virtual QRectF calcResultBoundingRect( const QVariant& theObj1, 
                                         const QVariant& theObj2 ) const;
  virtual void drawResult( QPainter& thePainter, const QVariant& theObj1,
                                                 const QVariant& theObj2 ) const;
};

#endif
