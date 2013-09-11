
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

  static QString Type() { return "cut"; }

  virtual QString name() const;

protected:
  virtual QRectF calcResultBoundingRect( const QVariant& theObj1, 
                                         const QVariant& theObj2 ) const;
  virtual void drawResult( QPainter& thePainter, const QVariant& theObj1,
                                                 const QVariant& theObj2 ) const;
};

#endif
