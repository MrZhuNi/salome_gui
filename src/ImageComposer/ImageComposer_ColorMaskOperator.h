
#ifndef IMAGE_COMPOSER_COLOR_MASK_OPERATOR_HEADER
#define IMAGE_COMPOSER_COLOR_MASK_OPERATOR_HEADER

#include "ImageComposer_Operator.h"
#include <QColor>

/**
  \class ImageComposer_ColorMaskOperator
  Implementation of the color mask operator
*/
class IMAGE_COMPOSER_API ImageComposer_ColorMaskOperator : public ImageComposer_Operator
{
public:
  ImageComposer_ColorMaskOperator();
  virtual ~ImageComposer_ColorMaskOperator();

  void setArgs( const QColor& theRefColor,
                bool isMakeTransparent,
                int theRGBThreshold,
                int theAlphaThreshold );

  virtual QString name() const;

protected:
  virtual QRectF calcResultBoundingRect( const QRectF& theImage1Bounds, 
                                         const QRectF& theImage2Bounds ) const;
  virtual void drawResult( QPainter& thePainter, const ImageComposer_Image& theImage1,
                                                 const ImageComposer_Image& theImage2 ) const;

  virtual void storeArgs( QDataStream& theStream ) const;
  virtual void restoreArgs( QDataStream& theStream );

private:
  friend class ImageComposerTests_TestOperators;

  QColor myRefColor;         ///< the color to the searched (the color for mask)
  bool myIsMakeTransparent;  ///< the boolean flag controlling if the pixels with matching color
                             ///< should be made transparent or one with non-matching color
  int myRGBThreshold;        ///< the threshold for RGB components
  int myAlphaThreshold;      ///< the threshold for Alpha component
};

#endif
