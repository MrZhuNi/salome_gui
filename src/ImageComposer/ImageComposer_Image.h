
#ifndef IMAGE_COMPOSER_IMAGE_HEADER
#define IMAGE_COMPOSER_IMAGE_HEADER

#include <QImage>
#include <ImageComposer.h>

class ImageComposer_Operator;

/**
  \class ImageComposer_Image
  Implementation of image in the global coordinate system
*/
class IMAGE_COMPOSER_API ImageComposer_Image : public QImage
{
public:
  ImageComposer_Image();
  ~ImageComposer_Image();

  void draw( QPainter& thePainter ) const;

  QTransform transform() const;
  void setTransform( const QTransform& );
  void setLocalTransform( qreal theDx, qreal theDy, qreal theRotationDeg );

  QRectF boundingRect() const;

  const ImageComposer_Image& operator = ( const ImageComposer_Image& theImage );
  const QImage& operator = ( const QImage& theImage );

  ImageComposer_Image apply( const ImageComposer_Operator& theOperator,
                             const ImageComposer_Image& theImage = ImageComposer_Image() ) const;

  static QColor defaultBackground();
  static void setDefaultBackground( const QColor& );
  ImageComposer_Image operator & ( const QRect& ) const;
  ImageComposer_Image operator & ( const QPainterPath& ) const;
  ImageComposer_Image operator & ( const ImageComposer_Image& ) const;
  ImageComposer_Image operator | ( const ImageComposer_Image& ) const;

private:
  QTransform myTransform;            ///< the image transformation in the global CS
  static QColor myDefaultBackground; ///< the default background color to be used in operators
};

#endif
