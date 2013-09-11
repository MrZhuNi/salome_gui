
#include "ImageComposer_Operator.h"
#include "ImageComposer_Image.h"
#include <QPixmap>
#include <QPainter>

/**
  Constructor
*/
ImageComposer_Operator::ImageComposer_Operator()
: myBackground( TRANSPARENT )
{
}

/**
  Destructor
*/
ImageComposer_Operator::~ImageComposer_Operator()
{
}

/**
  Get operator arguments
  @param theBackground the background color for result image
*/
void ImageComposer_Operator::getArgs( QColor& theBackground ) const
{
  theBackground = myBackground;
}

/**
  Set operator arguments
  @param theBackground the background color for result image
*/
void ImageComposer_Operator::setArgs( const QColor& theBackground )
{
  myBackground = theBackground;
}

/**
  Perform the composing of images
  @param theImage1 the first image to compose
  @param theImage2 the second image to compose
  @return the result image
*/
ImageComposer_Image ImageComposer_Operator::process( const ImageComposer_Image& theImage1,
                                                     const ImageComposer_Image& theImage2 ) const
{
  ImageComposer_Image anImage1 = theImage1;
  ImageComposer_Image anImage2 = theImage2;
  QTransform aInvTransform = anImage1.transform().inverted();
  anImage1.setTransform( anImage1.transform() * aInvTransform );
  if( !anImage2.isNull() )
    anImage2.setTransform( anImage2.transform() * aInvTransform );
  
  QRectF aBounds1 = anImage1.boundingRect();
  QRectF aBounds2;
  if( !anImage2.isNull() )
    aBounds2 = anImage2.boundingRect();
  QRectF aBounds = calcResultBoundingRect( aBounds1, aBounds2 );

  QTransform aTranslate;
  aTranslate.translate( -aBounds.left(), -aBounds.top() );
  anImage1.setTransform( anImage1.transform() * aTranslate );
  anImage2.setTransform( anImage2.transform() * aTranslate );

  QPixmap aResultImage( int(aBounds.width()), int(aBounds.height()) );
  aResultImage.fill( myBackground );

  QPainter aPainter( &aResultImage );
  aPainter.setRenderHint( QPainter::SmoothPixmapTransform, true );
  aPainter.setRenderHint( QPainter::Antialiasing, true );
  aPainter.setRenderHint( QPainter::HighQualityAntialiasing, true );

  drawResult( aPainter, anImage1, anImage2 );

  ImageComposer_Image aResult;
  aResult = aResultImage.toImage();
  QTransform aResultTransform = theImage1.transform();
  aResultTransform.translate( aBounds.left(), aBounds.top() );
  aResult.setTransform( aResultTransform );
  return aResult;
}

/**
  Get the operator's arguments in the form of a binary array
  @return the binary array with arguments
*/
QByteArray ImageComposer_Operator::getBinArgs() const
{
  QByteArray aData;
  QDataStream aStream( &aData, QIODevice::WriteOnly );
  storeArgs( aStream );
  return aData;
}

/**
  Set the operator's arguments in the form of a binary array
  @param theData the binary array with arguments
*/
void ImageComposer_Operator::setBinArgs( const QByteArray& theData )
{
  QDataStream aStream( theData );
  restoreArgs( aStream );
}

QStringList ImageComposer_Operator::dumpArgsToPython( QString& theArrayName ) const
{
  QStringList aResList;

  if ( theArrayName.isEmpty() )
    theArrayName = "composer_args";

  QString aStreamName = theArrayName + "_stream";

  aResList << QString( "%1 = QByteArray();" ).arg( theArrayName );
  aResList << QString( "%1 = QDataStream( %2, QIODevice.WriteOnly );" )
              .arg( aStreamName ).arg( theArrayName );

  //Dump background color
  aResList << QString( "" );

  aResList << QString( "background_color = QColor( %1, %2, %3, %4 );" )
              .arg( myBackground.red() ).arg( myBackground.green() )
              .arg( myBackground.blue() ).arg( myBackground.alpha() );

  aResList << QString( "%1 << background_color;" ).arg( aStreamName );

  return aResList;
}

/**
  Store the operator's arguments to the stream
  @param theStream the stream for storing
*/
void ImageComposer_Operator::storeArgs( QDataStream& theStream ) const
{
  theStream << myBackground;
}

/**
  Restore the operator's arguments from the stream
  @param theStream the stream for restoring
*/
void ImageComposer_Operator::restoreArgs( QDataStream& theStream )
{
  theStream >> myBackground;
}
