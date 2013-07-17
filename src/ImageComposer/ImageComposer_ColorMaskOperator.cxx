
#include <ImageComposer_ColorMaskOperator.h>
#include <ImageComposer_Image.h>
#include <QRectF>
#include <QRgb>
#include <QPixmap>
#include <QPainter>

/**
  Constructor
*/
ImageComposer_ColorMaskOperator::ImageComposer_ColorMaskOperator()
: ImageComposer_Operator(),
  myRefColor( Qt::black ), myIsMakeTransparent( false ),
  myRGBThreshold( 0 ), myAlphaThreshold( 0 )
{
}

/**
*/
ImageComposer_ColorMaskOperator::~ImageComposer_ColorMaskOperator()
{
}

/**
  Set operator arguments
  @param theRefColor the color to the searched (the color for mask)
  @param isMakeTransparent the boolean flag controlling if the pixels with matching color
                           should be made transparent or one with non-matching color
  @param theRGBThreshold the threshold for RGB components
  @param theAlphaThreshold the threshold for Alpha component
*/
void ImageComposer_ColorMaskOperator::setArgs( const QColor& theRefColor,
                                               bool isMakeTransparent,
                                               int theRGBThreshold,
                                               int theAlphaThreshold )
{
  myRefColor = theRefColor;
  myIsMakeTransparent = isMakeTransparent;
  myRGBThreshold = theRGBThreshold;
  myAlphaThreshold = theAlphaThreshold;
}

/**
*/
QString ImageComposer_ColorMaskOperator::name() const
{
  return "colormask";
}

/**
*/
QRectF ImageComposer_ColorMaskOperator::calcResultBoundingRect( const QRectF& theImage1Bounds, 
                                                                const QRectF& ) const
{
  return theImage1Bounds;
}

/**
*/
void ImageComposer_ColorMaskOperator::drawResult( QPainter& thePainter,
                                                  const ImageComposer_Image& theImage1,
                                                  const ImageComposer_Image& ) const
{
  QImage anImage = theImage1.convertToFormat( QImage::Format_ARGB32 );
  int aRMin = myRefColor.red()    - myRGBThreshold;
  int aRMax = myRefColor.red()    + myRGBThreshold;
  int aGMin = myRefColor.green()  - myRGBThreshold;
  int aGMax = myRefColor.green()  + myRGBThreshold;
  int aBMin = myRefColor.blue()   - myRGBThreshold;
  int aBMax = myRefColor.blue()   + myRGBThreshold;
  int anAMin = myRefColor.alpha() - myAlphaThreshold;
  int anAMax = myRefColor.alpha() + myAlphaThreshold;

  QRgb aTransparent = TRANSPARENT.rgba();

  for( int y = 0, aMaxY = anImage.height(); y < aMaxY; y++ )
    for( int x = 0, aMaxX = anImage.width(); x < aMaxX; x++ )
    {
      QRgb* aLine = ( QRgb* )anImage.scanLine( y );
      int aRed    = qRed( aLine[x] );
      int aGreen  = qGreen( aLine[x] );
      int aBlue   = qBlue( aLine[x] );
      int anAlpha = qAlpha( aLine[x] );
      bool isInRange = ( anAMin <= anAlpha && anAlpha <= anAMax )
                    && (  aRMin <= aRed    && aRed    <=  aRMax )
                    && (  aGMin <= aGreen  && aGreen  <=  aGMax )
                    && (  aBMin <= aBlue   && aBlue   <=  aBMax );
      if( myIsMakeTransparent == isInRange )
        aLine[x] = aTransparent;
    }

  ImageComposer_Image aResult;
  aResult = anImage;
  aResult.setTransform( theImage1.transform() );
  aResult.draw( thePainter );
}

void ImageComposer_ColorMaskOperator::storeArgs( QDataStream& theStream ) const
{
  ImageComposer_Operator::storeArgs( theStream );
  theStream << myRefColor;
  theStream << myIsMakeTransparent;
  theStream << myRGBThreshold;
  theStream << myAlphaThreshold;
}

void ImageComposer_ColorMaskOperator::restoreArgs( QDataStream& theStream )
{
  ImageComposer_Operator::restoreArgs( theStream );
  theStream >> myRefColor;
  theStream >> myIsMakeTransparent;
  theStream >> myRGBThreshold;
  theStream >> myAlphaThreshold;
}
