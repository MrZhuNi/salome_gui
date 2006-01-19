//  Copyright (C) 2005 OPEN CASCADE
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org
//
//  Author : OPEN CASCADE
//

// File:      GLViewer_Drawer.cxx
// Created:   November, 2004

//#include <GLViewerAfx.h>
#include "GLViewer_Drawer.h"
#include "GLViewer_Object.h"
#include "GLViewer_Text.h"
#include "GLViewer_ViewFrame.h"
#include "GLViewer_ViewPort2d.h"

#ifndef WIN32
#include <GL/glx.h>
#endif

#include <qimage.h>
#include <qpainter.h>

#define TEXT_GAP    5
// Two texture components for texmapped fonts: luminance and alpha
#define NB_TEX_COMP 2
// A font is split into rows each containing 32 characters
#define TEX_ROW_LEN 32
// Gap in pixels between two character rows in a font texture
#define TEX_ROW_GAP 2

GLfloat modelMatrix[16];

//================================================================
// Class       : GLViewer_TexFont
// Description : 
//================================================================
//! code of first font symbol
static int FirstSymbolNumber = 32;
//! code of last font symbol
static int LastSymbolNumber = 127;

QMap<GLViewer_TexFindId,GLViewer_TexIdStored> GLViewer_TexFont::TexFontBase;
QMap<GLViewer_TexFindId,GLuint>               GLViewer_TexFont::BitmapFontCache; 

//=======================================================================
// Function: clearTextBases
// Purpose :
//=======================================================================
void GLViewer_TexFont::clearTextBases()
{
  //cout << "Clear font map" << endl;
  TexFontBase.clear();
  BitmapFontCache.clear();
}

//======================================================================
// Function: GLViewer_TexFont
// Purpose :
//=======================================================================
GLViewer_TexFont::GLViewer_TexFont()
: myMaxRowWidth( 0 ), myFontHeight( 0 )
{
    myQFont = QFont::defaultFont();
    mySeparator = 2;
    myIsResizeable = false;
    myMinMagFilter = GL_LINEAR;

    init();
}

//======================================================================
// Function: GLViewer_TexFont
// Purpose :
//=======================================================================
GLViewer_TexFont::GLViewer_TexFont( QFont* theFont, int theSeparator, bool theIsResizeable, GLuint theMinMagFilter )
: myMaxRowWidth( 0 ), myFontHeight( 0 )
{
    myQFont = *theFont;
    mySeparator = theSeparator;
    myIsResizeable = theIsResizeable;
    myMinMagFilter = theMinMagFilter;

    init();
}

//======================================================================
// Function: ~GLViewer_TexFont
// Purpose :
//=======================================================================
GLViewer_TexFont::~GLViewer_TexFont()
{
    delete[] myWidths;
    delete[] myPositions;
} 

//======================================================================
// Function: init
// Purpose :
//=======================================================================
void GLViewer_TexFont::init()
{
    myNbSymbols = LastSymbolNumber - FirstSymbolNumber + 1;

    // It is unsafe to draw all characters in a single row -
    // this leads to problems on some graphic cards with small GL_MAX_TEXTURE_SIZE.
    // So splitting the characters into rows each containing 32 characters (or less).
    // Assuming contant height of each row (64 pixels) to simplify texture mapping.
    // However, this can be improved if necessary.
    QFontMetrics aFM( myQFont ); 
    myFontHeight = aFM.height();
    
    myWidths    = new int[myNbSymbols];
    myPositions = new int[myNbSymbols];

    for( int i = 0, k = FirstSymbolNumber, aWidth = 0; i < myNbSymbols; i++, k++ )
    {
        // is it time to start a new row?
        if ( !( i % TEX_ROW_LEN ) )
        {
          if( aWidth > myMaxRowWidth )
            myMaxRowWidth = aWidth;
          aWidth = 0;
        }
        myWidths[i]    = aFM.width( k );
        myPositions[i] = aWidth;
        aWidth += myWidths[i] + 2;
    }

    myTexFontWidth  = 0;
    myTexFontHeight = 0;
}
  
//======================================================================
// Function: generateTexture
// Purpose :
//=======================================================================
bool GLViewer_TexFont::generateTexture()
{
    GLViewer_TexFindId aFindFont;
    aFindFont.myFontFamily = myQFont.family();//myQFont.toString();
    aFindFont.myIsBold = myQFont.bold();
    aFindFont.myIsItal = myQFont.italic();
    aFindFont.myIsUndl = myQFont.underline();
    aFindFont.myPointSize = myQFont.pointSize();
    aFindFont.myViewPortId = (int)QGLContext::currentContext();
        
    if( TexFontBase.contains( aFindFont ) )
    {
        GLViewer_TexIdStored aTexture = TexFontBase[ aFindFont ];
        myTexFont = aTexture.myTexFontId;
        myTexFontWidth = aTexture.myTexFontWidth;
        myTexFontHeight = aTexture.myTexFontHeight;
    }    
    else    
    {
        // Adding some pixels to have a gap between rows
        int aRowPixelHeight = myFontHeight + TEX_ROW_GAP;
        int aDescent = QFontMetrics( myQFont ).descent();

        int aNumRows = myNbSymbols / TEX_ROW_LEN;
        if ( myNbSymbols % TEX_ROW_LEN ) 
          aNumRows++;
        int pixelsHight = aNumRows * aRowPixelHeight;

        myTexFontWidth  = 64;
        myTexFontHeight = 64;

        while( myTexFontWidth < myMaxRowWidth )
            myTexFontWidth <<= 1;
        while( myTexFontHeight < pixelsHight )
            myTexFontHeight <<= 1;
        
        // Checking whether the texture dimensions for the requested font
        // do not exceed the maximum size supported by the OpenGL implementation
        int maxSize;
        glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxSize );
        if ( myTexFontWidth > maxSize || myTexFontHeight > maxSize )
          return false;

        QPixmap aPixmap( myTexFontWidth, myTexFontHeight );
        aPixmap.fill( QColor( 0, 0, 0) );
        QPainter aPainter( &aPixmap );
        aPainter.setFont( myQFont );
        int row;
        for( int l = 0; l < myNbSymbols; l++  )
        {
            row = l / TEX_ROW_LEN;
            QString aLetter;
            aLetter += (char)(FirstSymbolNumber + l);
            aPainter.setPen( QColor( 255,255,255) );
            aPainter.drawText( myPositions[l], ( row + 1 ) * aRowPixelHeight - aDescent, aLetter );
        }
    
        QImage aImage = aPixmap.convertToImage();

        //int qqq = 0;
        //if (qqq)
        //  aImage.save("w:\\work\\CATHARE\\texture.png", "PNG");

        char* pixels = new char[myTexFontWidth * myTexFontHeight * NB_TEX_COMP];

        for( int i = 0; i < myTexFontHeight; i++ )
        {            
            for( int j = 0; j < myTexFontWidth;  j++ )
            {
                int aRed = qRed( aImage.pixel( j, myTexFontHeight - i - 1 ) );
                int aGreen = qGreen( aImage.pixel( j, myTexFontHeight - i - 1 ) );
                int aBlue = qBlue( aImage.pixel( j, myTexFontHeight - i - 1 ) );
          
                if( aRed != 0 || aGreen != 0 || aBlue != 0 )
                {
                    pixels[i * myTexFontWidth * NB_TEX_COMP + j * NB_TEX_COMP] = (GLubyte)( (aRed + aGreen + aBlue)/3 );
                    pixels[i * myTexFontWidth * NB_TEX_COMP + j * NB_TEX_COMP + 1]= (GLubyte) 255;
                }
                else
                {
                    pixels[i * myTexFontWidth * NB_TEX_COMP + j * NB_TEX_COMP] = (GLubyte) 0;
                    pixels[i * myTexFontWidth * NB_TEX_COMP + j * NB_TEX_COMP + 1]= (GLubyte) 0;
                }                
            }
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &myTexFont);
        glBindTexture(GL_TEXTURE_2D, myTexFont);  
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, myMinMagFilter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, myMinMagFilter);
        glTexImage2D(GL_TEXTURE_2D, 
                     0, 
                     GL_INTENSITY, 
                     myTexFontWidth,
                     myTexFontHeight, 
                     0, 
                     GL_LUMINANCE_ALPHA, 
                     GL_UNSIGNED_BYTE, 
                     pixels);
    
        delete[] pixels;
        
        GLViewer_TexIdStored aTexture;
        aTexture.myTexFontId = myTexFont;
        aTexture.myTexFontWidth = myTexFontWidth;
        aTexture.myTexFontHeight = myTexFontHeight;

        TexFontBase.insert( aFindFont, aTexture );
    }
    return true;
}

//======================================================================
// Function: drawString
// Purpose :
//=======================================================================
void GLViewer_TexFont::drawString( QString theStr, GLdouble theX , GLdouble theY, GLfloat theScale )
{
    // Adding some pixels to have a gap between rows
    int aRowPixelHeight = myFontHeight + TEX_ROW_GAP;

    float aXScale = 1.f, aYScale = 1.f;
    if ( !myIsResizeable )
    {
      glGetFloatv (GL_MODELVIEW_MATRIX, modelMatrix);
      aXScale = modelMatrix[0];
      aYScale = modelMatrix[5];     
    } 
    else if ( theScale > 0.f )
    {
      aXScale = aXScale / theScale;
      aYScale = aYScale / theScale;
    }

    // store attributes
    glPushAttrib( GL_ENABLE_BIT | GL_TEXTURE_BIT );

    glEnable(GL_TEXTURE_2D);
    glPixelTransferi(GL_MAP_COLOR, 0);

    glAlphaFunc(GL_GEQUAL, 0.05F);
    glEnable(GL_ALPHA_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, myTexFont);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBegin(GL_QUADS);

    float aLettBeginS, aLettEndS, aLettBeginT, aLettEndT;
    float aDY = ( aRowPixelHeight - 1 ) / aYScale, aDX;
    char aLetter;
    int aLettIndex, row;
    for( int i = 0; i < theStr.length(); i++ )
    {
        aLetter    = theStr.data()[i];
        aLettIndex = (int)aLetter - FirstSymbolNumber;
        row        = aLettIndex / TEX_ROW_LEN;

        aLettBeginS = (float)myPositions[aLettIndex] / ( (float)myTexFontWidth - 1.f );
        aLettEndS   = aLettBeginS + ( (float)myWidths[aLettIndex] - 1.f ) / ( (float)myTexFontWidth - 1.f );
        aLettBeginT = ( myTexFontHeight - ( row + 1 ) * aRowPixelHeight ) / ( (float)myTexFontHeight - 1.f ); 
        aLettEndT   = aLettBeginT + ( (float)aRowPixelHeight - 1.f ) / ( (float)myTexFontHeight - 1.f );

        aDX = ( (float)myWidths[aLettIndex] - 1.f ) / aXScale;

        glTexCoord2f( aLettBeginS, aLettBeginT ); glVertex3f( theX,       theY,       1.f );
        glTexCoord2f( aLettBeginS, aLettEndT   ); glVertex3f( theX,       theY + aDY, 1.f );
        glTexCoord2f( aLettEndS,   aLettEndT   ); glVertex3f( theX + aDX, theY + aDY, 1.f );
        glTexCoord2f( aLettEndS,   aLettBeginT ); glVertex3f( theX + aDX, theY,       1.f );

        theX += aDX + mySeparator / aXScale;
    }

    glEnd();
    // restore attributes
    glPopAttrib();
}

//======================================================================
// Function: getStringWidth
// Purpose :
//=======================================================================
int GLViewer_TexFont::getStringWidth( QString theStr )
{
    int aWidth = 0;
    for( int i = 0; i < theStr.length(); i ++ )
    {
        char aLetter = theStr.data()[i];
        int aLettIndex = (int)aLetter - FirstSymbolNumber;
        aWidth += myWidths[aLettIndex] + mySeparator;
    }

    return aWidth;
}

//======================================================================
// Function: getStringHeight
// Purpose :
//=======================================================================
int GLViewer_TexFont::getStringHeight()
{
    QFontMetrics aFM( myQFont );
    return aFM.height();
}

//! function for generation list base for bitmap fonts
static GLuint displayListBase( QFont* theFont )
{
  if ( !theFont )
    return 0;
  GLuint aList = 0;
  //static QMap<GLViewer_TexFindId, GLuint> fontCache;
  GLViewer_TexFindId aFindFont;
  aFindFont.myFontFamily = theFont->family();//theFont->toString();
  aFindFont.myIsBold = theFont->bold();
  aFindFont.myIsItal = theFont->italic();
  aFindFont.myIsUndl = theFont->underline();
  aFindFont.myPointSize = theFont->pointSize();

#ifdef WIN32
  HGLRC ctx = ::wglGetCurrentContext();
  if ( !ctx )
    return aList;  
  
  aFindFont.myViewPortId = (int)ctx;

  if ( GLViewer_TexFont::BitmapFontCache.contains( aFindFont ) )
    aList = GLViewer_TexFont::BitmapFontCache[aFindFont];
  else
  {
    GLuint listBase = 0;
    QMap<GLViewer_TexFindId, GLuint>::iterator it = GLViewer_TexFont::BitmapFontCache.begin();
    for ( ; it != GLViewer_TexFont::BitmapFontCache.end(); ++it )
    {
      if ( it.key().myViewPortId == (int)ctx && it.data() > listBase )
        listBase = it.data();
    }
    listBase += 256;

    HDC glHdc = ::wglGetCurrentDC();
    ::SelectObject( glHdc, theFont->handle() );
    if ( !::wglUseFontBitmaps( glHdc, 0, 256, listBase ) )
      listBase = 0;
    aList = listBase;
    GLViewer_TexFont::BitmapFontCache[aFindFont] = aList;
  }
#else //X Window
  Display* aDisp = glXGetCurrentDisplay();
  if( !aDisp )
  {
#ifdef _DEBUG_
    printf( "Can't find current dislay\n" );
#endif
    return aList;
  }
  
  GLXContext aCont = glXGetCurrentContext();
  if( !aCont )
  {
#ifdef _DEBUG_
    printf( "Can't find current context\n" );
#endif
    return aList;
  }

  aFindFont.myViewPortId = (int)aCont;

  if ( GLViewer_TexFont::BitmapFontCache.contains( aFindFont ) )
    aList = GLViewer_TexFont::BitmapFontCache[aFindFont];
  else
  {
    GLuint listBase = 0;
    QMap<GLViewer_TexFindId, GLuint>::iterator it = GLViewer_TexFont::BitmapFontCache.begin();
    for ( ; it != GLViewer_TexFont::BitmapFontCache.end(); ++it )
    {
      if ( it.key().myViewPortId == (int)aCont && it.data() > listBase )
        listBase = it.data();
    }
    listBase += 256;
    
    //glXUseXFont( (Font)(theFont->handle()), 0, 256, listBase );
    int aFontCont = 0;
    QString aFontDef = theFont->toString();
    char** xFontList = XListFonts( aDisp, aFontDef.latin1()/*aFindFont.myFontString.data()*/, 1, &aFontCont  );
    if( !theFont->handle() )
    {       
#ifdef _DEBUG_
      printf( "Can't load font %s. loading default font....\n", aFontDef.latin1()/*aFindFont.myFontString.data()*/ );
#endif
      QString aFontMask ("-*-*-*-r-*-*-");
      aFontMask += aFontDef/*aFindFont.myFontString*/.section( ',', 1, 1 );
#ifdef _DEBUG_
      printf( "Height of Default font: %s\n", aFontDef/*aFindFont.myFontString*/.section( ',', 1, 1 ).data() );
#endif
      aFontMask += "-*-*-*-m-*-*-*";
      xFontList = XListFonts( aDisp, aFontMask.data()/*"-*-*-*-r-*-*-12-*-*-*-m-*-*-*"*/, 1, &aFontCont  );
      if( aFontCont == 0 )
      {      
#ifdef _DEBUG_
        printf( "Can't load default font\n" );
#endif
        return 0;
      }
      glXUseXFont( (Font)(XLoadFont( aDisp,xFontList[0] )), 0, 256, listBase );
    }
    else
      glXUseXFont( (Font)(theFont->handle()), 0, 256, listBase );
    
    aList = listBase;
    GLViewer_TexFont::BitmapFontCache[aFindFont] = aList;
  }

#endif

  return aList;
}

/***************************************************************************
**  Class:   GLViewer_Drawer
**  Descr:   Drawer for GLViewer_Object
**  Module:  GLViewer
**  Created: UI team, 01.10.01
****************************************************************************/
//======================================================================
// Function: GLViewer_Drawer
// Purpose :
//=======================================================================
GLViewer_Drawer::GLViewer_Drawer()
: myFont( "Helvetica", 10, QFont::Bold )
{
  myXScale = myYScale = 0.0;
  myObjects.clear();
  myTextList = 0/*-1*/;
  myObjectType = "GLViewer_Object";
  myPriority = 0;
  myTextFormat = DTF_BITMAP;
  myTextScale = 0.125;
}

//======================================================================
// Function: ~GLViewer_Drawer
// Purpose :
//=======================================================================
GLViewer_Drawer::~GLViewer_Drawer()
{
  myObjects.clear();
  glDeleteLists( myTextList, 1 );
}

//======================================================================
// Function: destroyAllTextures
// Purpose :
//=======================================================================
void GLViewer_Drawer::destroyAllTextures()
{
    QMap<GLViewer_TexFindId,GLViewer_TexIdStored>::Iterator anIt= GLViewer_TexFont::TexFontBase.begin();
    QMap<GLViewer_TexFindId,GLViewer_TexIdStored>::Iterator anEndIt= GLViewer_TexFont::TexFontBase.end();

    for( ; anIt != anEndIt; anIt++ )
        glDeleteTextures( 1, &(anIt.data().myTexFontId) );
}

//=======================================================================
// Function: setAntialiasing
// Purpose : The function enables and disables antialiasing in Open GL (for points, lines and polygons).
//=======================================================================
void GLViewer_Drawer::setAntialiasing(const bool on)
{
	if (on)
	{
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		glEnable (GL_BLEND);
	}
	else
	{
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		glBlendFunc (GL_ONE, GL_ZERO);
		glDisable (GL_BLEND);
	}
}

//======================================================================
// Function: loadTexture
// Purpose :
//=======================================================================
GLuint GLViewer_Drawer::loadTexture( const QString& fileName,
                                     GLint* x_size,
                                     GLint* y_size,
                                     GLint* t_size )
{
    QImage buf;
    if ( fileName.isEmpty() || !buf.load( fileName ) )
        return 0;

    int w = buf.width();
    int h = buf.height();

    int size = 16;
    while( size < w || size < h )
        size = size * 2;

    GLuint texture;
    GLubyte* pixels = new GLubyte[ size * size * 4 ];

    for( int i = 0; i < size; i++ )
    {            
        for( int j = 0; j < size; j++ )
        {
            GLubyte r, g, b, a;
            if( j < w && i < h )
            {
                QRgb pixel = buf.pixel( j, h - i - 1 );
                r = (GLubyte)qRed( pixel );
                g = (GLubyte)qGreen( pixel );
                b = (GLubyte)qBlue( pixel );
                a = (GLubyte)qAlpha( pixel );
            }
            else
            {
                r = (GLubyte)255;
                g = (GLubyte)255;
                b = (GLubyte)255;
                a = (GLubyte)255;
            }

            int index = 4 * ( i * size + j );
            pixels[ index ] = r;
            pixels[ index + 1 ] = g;
            pixels[ index + 2 ] = b;
            pixels[ index + 3 ] = a;
        }
    }

    //initialize texture
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    delete[] pixels;

    if ( x_size )
      *(x_size) = w;

    if ( y_size )
      *(y_size) = h;

    if ( t_size )
      *(t_size) = size;

    return texture;
}

//======================================================================
// Function: drawTexture
// Purpose :
//=======================================================================
void GLViewer_Drawer::drawTexture( GLuint texture, GLint size, GLfloat x, GLfloat y )
{
    /*float xScale = myXScale;
    float yScale = myYScale;

    glColor4f( 1.0, 1.0, 1.0, 1.0 );

    glEnable( GL_TEXTURE_2D );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glAlphaFunc( GL_GREATER, 0.95F );
    glEnable( GL_ALPHA_TEST );
    
    glBindTexture( GL_TEXTURE_2D, texture );
    glBegin( GL_QUADS );

    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x-size/2./xScale, y-size/2./yScale, 0.0 );

    glTexCoord2f( 0.0, 1.0 );
    glVertex3f( x-size/2./xScale, y+size/2./yScale, 0.0 );

    glTexCoord2f( 1.0, 1.0 );
    glVertex3f( x+size/2./xScale, y+size/2./yScale, 0.0 );

    glTexCoord2f( 1.0, 0.0 );
    glVertex3f( x+size/2./xScale, y-size/2./yScale, 0.0 );
    
    glEnd();
    glFlush();

    glDisable( GL_ALPHA_TEST );
    glDisable( GL_TEXTURE_2D );*/

  drawTexture( texture, size, size, x, y );
}

//======================================================================
// Function: drawTexture
// Purpose :
//=======================================================================
void GLViewer_Drawer::drawTexture( GLuint texture, GLint x_size, GLint y_size, GLfloat x, GLfloat y )
{
    /*float xScale = myXScale;
    float yScale = myYScale;

    glColor4f( 1.0, 1.0, 1.0, 1.0 );

    glEnable( GL_TEXTURE_2D );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glAlphaFunc( GL_GREATER, 0.95F );
    glEnable( GL_ALPHA_TEST );
    
    glBindTexture( GL_TEXTURE_2D, texture );
    glBegin( GL_QUADS );

    glTexCoord2f( 0.0, 0.0 );
    glVertex3f( x-x_size/2./xScale, y-y_size/2./yScale, 0.0 );

    glTexCoord2f( 0.0, 1.0 );
    glVertex3f( x-x_size/2./xScale, y+y_size/2./yScale, 0.0 );

    glTexCoord2f( 1.0, 1.0 );
    glVertex3f( x+x_size/2./xScale, y+y_size/2./yScale, 0.0 );

    glTexCoord2f( 1.0, 0.0 );
    glVertex3f( x+x_size/2./xScale, y-y_size/2./yScale, 0.0 );
    
    glEnd();
    glFlush();

    glDisable( GL_ALPHA_TEST );
    glDisable( GL_TEXTURE_2D );*/
  drawTexturePart( texture, 1.0, 1.0, x_size, y_size, x, y );
}

//======================================================================
// Function: drawTexture
// Purpose :
//=======================================================================
void GLViewer_Drawer::drawTexturePart( GLuint texture,
                                       GLfloat x_ratio,
                                       GLfloat y_ratio,
                                       GLfloat x_size,
                                       GLfloat y_size,
                                       GLfloat x,
                                       GLfloat y,
                                       GLfloat scale )
{
  if( !texture )
    return;

  float xScale = scale > 0. ? 1./scale : myXScale;
  float yScale = scale > 0. ? 1./scale : myYScale;

  glColor4f( 1.0, 1.0, 1.0, 1.0 );


  glEnable( GL_TEXTURE_2D );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
  bool hasAlpha = glIsEnabled( GL_ALPHA_TEST );
  glDisable( GL_ALPHA_TEST );

  glBindTexture( GL_TEXTURE_2D, texture );
  glBegin( GL_QUADS );

  glTexCoord2f( 0.0, 0.0 );
  glVertex3f( x-x_size/2./xScale, y-y_size/2./yScale, 0.0 );

  glTexCoord2f( 0.0, y_ratio );
  glVertex3f( x-x_size/2./xScale, y+y_size/2./yScale, 0.0 );

  glTexCoord2f( x_ratio, y_ratio );
  glVertex3f( x+x_size/2./xScale, y+y_size/2./yScale, 0.0 );

  glTexCoord2f( x_ratio, 0.0 );
  glVertex3f( x+x_size/2./xScale, y-y_size/2./yScale, 0.0 );
  
  glEnd();
  glFlush();

  if ( hasAlpha )
    glEnable( GL_ALPHA_TEST );

  glDisable( GL_TEXTURE_2D );
}

//======================================================================
// Function: drawText
// Purpose :
//=======================================================================
void GLViewer_Drawer::drawText( const QString& text, GLfloat xPos, GLfloat yPos,
                                const QColor& color, QFont* theFont, int theSeparator, DisplayTextFormat theFormat )
{
  glColor3f( ( GLfloat )color.red() / 255, 
             ( GLfloat )color.green() / 255, 
             ( GLfloat )color.blue() / 255 );

  if( theFormat != DTF_BITMAP )
  {
    GLViewer_TexFont aTexFont( theFont, theSeparator, theFormat == DTF_TEXTURE_SCALABLE, GL_LINEAR );
    // Font texture was not found or generated --> cannot draw text
    if ( !aTexFont.generateTexture() )
      return;

    if ( theFormat == DTF_TEXTURE_SCALABLE )
      aTexFont.drawString( text, xPos, yPos, textScale() );
    else
      aTexFont.drawString( text, xPos, yPos );
  }
  else
  {
    glRasterPos2f( xPos, yPos );
    glListBase( displayListBase( theFont ) );
    glCallLists( text.length(), GL_UNSIGNED_BYTE, text.local8Bit().data() );
  }
}

//======================================================================
// Function: drawText
// Purpose :
//=======================================================================
void GLViewer_Drawer::drawText( GLViewer_Object* theObject )
{
  if( !theObject )
    return;

  GLViewer_Text* aText = theObject->getGLText();
  if( !aText )
    return;

  GLfloat aPosX, aPosY;
  aText->getPosition( aPosX, aPosY );
  // get temporary copy of font
  QFont aTmpVarFont = aText->getFont();
  drawText( aText->getText(), aPosX, aPosY, aText->getColor(), &aTmpVarFont, aText->getSeparator(), aText->getDisplayTextFormat() );
}

//======================================================================
// Function: drawGLText
// Purpose :
//=======================================================================
void GLViewer_Drawer::drawGLText( QString text, float x, float y,
                                  int hPosition, int vPosition, QColor color, bool smallFont )
{
  QFont aFont( myFont );
  if( smallFont )
    aFont.setPointSize( aFont.pointSize() * 0.8 );

  GLfloat scale = textScale() > 0. ? textScale() : 1.;

  QFontMetrics aFontMetrics( aFont );
  float width  = myTextFormat == DTF_TEXTURE_SCALABLE ? aFontMetrics.width( text ) * scale : aFontMetrics.width( text ) / myXScale;
  float height = myTextFormat == DTF_TEXTURE_SCALABLE ? aFontMetrics.height() * scale : aFontMetrics.height() / myYScale;
  float gap = 5 / myXScale;

  switch( hPosition )
  {
      case GLText_Left   : x -= ( gap + width ); break;
      case GLText_Center : x -= width / 2; break;
      case GLText_Right  : x += gap; break;
      default : break;
  }

  switch( vPosition )
  {
      case GLText_Top    : y += height * 0.5; break;
      case GLText_Center : y -= height * 0.5; break;
      case GLText_Bottom : y -= height * 1.5; break;
      default : break;
  }

  drawText( text, x, y, color, &aFont, 2, myTextFormat );
}

//======================================================================
// Function: textRect
// Purpose :
//=======================================================================
GLViewer_Rect GLViewer_Drawer::textRect( const QString& text ) const
{
  GLfloat scale = textScale() > 0. ? textScale() : 1.;

  QFontMetrics aFontMetrics( myFont );
  float width  = myTextFormat == DTF_TEXTURE_SCALABLE ? aFontMetrics.width( text ) * scale : aFontMetrics.width( text );
  float height = myTextFormat == DTF_TEXTURE_SCALABLE ? aFontMetrics.height() * scale : aFontMetrics.height();

  return GLViewer_Rect( 0, width, height, 0 );
}

//======================================================================
// Function: drawRectangle
// Purpose :
//=======================================================================
void GLViewer_Drawer::drawRectangle( GLViewer_Rect* rect, QColor color )
{
  if( !rect )
    return;

  float x1 = rect->left();
  float x2 = rect->right();
  float y1 = rect->bottom();
  float y2 = rect->top();
  
  glColor3f( ( GLfloat )color.red() / 255,
    ( GLfloat )color.green() / 255,
    ( GLfloat )color.blue() / 255 );
  glLineWidth( 1.0 );
  
  glBegin( GL_LINE_LOOP );
  glVertex2f( x1, y1 );
  glVertex2f( x1, y2 );
  glVertex2f( x2, y2 );
  glVertex2f( x2, y1 );
  glEnd();
}

//======================================================================
// Function: translateToHPGL
// Purpose :
//=======================================================================
bool GLViewer_Drawer::translateToHPGL( QFile& hFile, GLViewer_CoordSystem* aViewerCS, GLViewer_CoordSystem* aHPGLCS )
{
    bool result = true;
    for( int i=0, n=myObjects.count(); i<n; i++ ) 
        result &= myObjects[i]->translateToHPGL( hFile, aViewerCS, aHPGLCS );
    return result;
}

//======================================================================
// Function: translateToPS
// Purpose :
//=======================================================================
bool GLViewer_Drawer::translateToPS( QFile& hFile, GLViewer_CoordSystem* aViewerCS, GLViewer_CoordSystem* aPSCS )
{
    bool result = true;
    for( int i=0, n=myObjects.count(); i<n; i++ ) 
        result &= myObjects[i]->translateToPS( hFile, aViewerCS, aPSCS );
    return result;
}

#ifdef WIN32
//======================================================================
// Function: translateToEMF
// Purpose :
//=======================================================================
bool GLViewer_Drawer::translateToEMF( HDC hDC, GLViewer_CoordSystem* aViewerCS, GLViewer_CoordSystem* aEMFCS )
{
    bool result = true;
    for( int i=0, n=myObjects.count(); i<n; i++ ) 
        result &= myObjects[i]->translateToEMF( hDC, aViewerCS, aEMFCS );
    return result;
}
#endif
