
#ifndef IMAGE_COMPOSER_OPERATOR_HEADER
#define IMAGE_COMPOSER_OPERATOR_HEADER

#include <ImageComposer.h>
#include <QColor>

class QString;
class QRectF;
class QPainter;
class QTransform;
class ImageComposer_Image;

const QColor TRANSPARENT( 255, 255, 255, 0 );

/**
  \class ImageComposer_Operator
  Implementation of the base abstract operation for image composing
*/
class IMAGE_COMPOSER_API ImageComposer_Operator
{
public:
  ImageComposer_Operator();
  virtual ~ImageComposer_Operator();

  void setArgs( const QColor& theBackground );
  QByteArray getBinArgs() const;
  void setBinArgs( const QByteArray& );

  virtual QString name() const;
  virtual ImageComposer_Image process( const ImageComposer_Image& theImage1,
                                       const ImageComposer_Image& theImage2 ) const;

protected:
  /**
    Calculate bounding rectangle for the result image
    @param theImage1Bounds bounding rectangle of the first image
    @param theImage2Bounds bounding rectangle of the second image
    @return calculated bounding rectangle
  */
  virtual QRectF calcResultBoundingRect( const QRectF& theImage1Bounds, 
                                         const QRectF& theImage2Bounds ) const = 0;

  /**
    Draw result image using the given painter
    @param thePainter the painter on the result image
    @param theImage1 the first image to compose
    @param theImage2 the second image to compose
  */
  virtual void drawResult( QPainter& thePainter, const ImageComposer_Image& theImage1,
                                                 const ImageComposer_Image& theImage2 ) const = 0;

  virtual void storeArgs( QDataStream& theStream ) const;
  virtual void restoreArgs( QDataStream& theStream );

private:
  friend class ImageComposerTests_TestOperators;

  QColor myBackground;  ///< the background color for result image
};

#endif
