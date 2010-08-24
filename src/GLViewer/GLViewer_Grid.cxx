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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  Author : OPEN CASCADE
//

// File:      GLViewer_Grid.cxx
// Created:   November, 2004

//#include <GLViewerAfx.h>
#include "GLViewer_Grid.h"
#include "GLViewer_Defs.h"

#include <Precision.hxx>

/*!
  Default constructor
*/
GLViewer_Grid::GLViewer_Grid() :
  myIsEnabled( GL_TRUE ),
  myGridList( 0 ),
  myGridHeight( (GLfloat)0.0 ),
  myGridWidth( (GLfloat)0.0 ),
  myWinWidth( (GLfloat)0.0 ),
  myWinHeight( (GLfloat)0.0 ),
  myXSize( (GLfloat)0.0 ),
  myYSize( (GLfloat)0.0 ),
  myXPan( (GLfloat)0.0 ),
  myYPan( (GLfloat)0.0 ),
  myXScale( (GLfloat)1.0 ),
  myYScale( (GLfloat)1.0 ),
  myLineWidth( (GLfloat)0.05 ),
  myLineType( Solid ),
  myAxisLineWidth( (GLfloat)1.5 ),
  myAxisLineType( Solid ),
  myCenterRadius( 5 ),
  myScaleFactor( (GLfloat)10.0 ),
  myScaleRatio( (GLfloat)5.0 ),
  myIsUpdate( GL_FALSE )
{
  myLineColor[0] = 0.5;
  myLineColor[1] = 0.5;
  myLineColor[2] = 0.5;
  myAxisLineColor[0] = 0.75;
  myAxisLineColor[1] = 0.75;
  myAxisLineColor[2] = 0.75;
}

/*!
  Destructor
*/
GLViewer_Grid::~GLViewer_Grid()
{
}

/*!
  Performs OpenGL drawing
*/
void GLViewer_Grid::draw()
{
  if ( !isEnabled() )
    return;

  if ( myGridList == 0 || myIsUpdate )
    initList();

  // disable smoothing (if enabled)
  glPushAttrib( GL_ENABLE_BIT | GL_HINT_BIT );
	glDisable( GL_POINT_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POLYGON_SMOOTH );
	glBlendFunc( GL_ONE, GL_ZERO );
	glDisable( GL_BLEND );

  glCallList( myGridList );

  // restore previous state of smoothing
  glPopAttrib();
}

/*!
  Sets grid enabled
  \param state - enable state
*/
void GLViewer_Grid::setEnabled( GLboolean state )
{
  if( myIsEnabled == state )
    return;

  myIsEnabled = state;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets grid width
  \param w - new grid width
*/
void GLViewer_Grid::setGridWidth( float w )
{
  if( myGridWidth == w )
    return;

  myGridWidth = w;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets grid height
  \param h - new grid height
*/
void GLViewer_Grid::setGridHeight( float h )
{
  if( myGridHeight == h )
    return;

  myGridHeight = h;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets parameters of grid by zoom coefficient and window size
  \param w - window width
  \param h - window height
  \param zoom - zoom coefficient
*/
void GLViewer_Grid::setResize( float w, float h, float zoom )
{
  if( myWinWidth == w && myWinHeight == h && zoom == 1.0 )
    return;

  myGridWidth = myGridWidth + ( w - myWinWidth ) * myXScale; 
  myGridHeight = myGridHeight + ( h - myWinHeight ) * myYScale;
  myWinWidth = w;
  myWinHeight = h;
  setZoom( zoom );
  myIsUpdate = GL_TRUE;
}

/*!
  Sets grid size along X and Y axis
  \param xSize - size along X axis
  \param ySize - size along Y axis
*/
void GLViewer_Grid::setSize( float xSize, float ySize )
{
  if( myXSize == xSize && myYSize == ySize )
    return;
  
  myXSize = xSize;
  myYSize = ySize;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets panning of grid
  \param xPan - panning along X axis
  \param yPan - panning along Y axis
*/
void GLViewer_Grid::setPan( float xPan, float yPan )
{
  if( myXPan == xPan && myYPan == yPan )
    return;
 
  myXPan = xPan;
  myYPan = yPan;
  myIsUpdate = GL_TRUE; 
}

/*!
  Sets zoom 
  \param zoom - new coefficient of zooming
*/
bool GLViewer_Grid::setZoom( float zoom )
{
  if( zoom == 1.0 )
    return true;
  
  //backup values
  float bXScale = myXScale;
  float bYScale = myYScale;

  myXScale /= zoom; 
  myYScale /= zoom;

  if( fabs(myXScale) < Precision::Confusion() || fabs(myYScale) < Precision::Confusion() )
  { //undo
    myXScale = bXScale;
    myYScale = bYScale;
    return false;
  }
  
  myGridWidth /= zoom; 
  myGridHeight /= zoom;  
  myIsUpdate = GL_TRUE;
  return true;
}

/*!
  Sets line width
  \param value - new line width
*/
void GLViewer_Grid::setLineWidth( float value )
{
  if( myLineWidth == value )
    return;

  myLineWidth = value;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets line type
  \param type - new line type
*/
void GLViewer_Grid::setLineType( LineType type )
{
  if( myLineType == type )
    return;

  myLineType = type;
  myIsUpdate = GL_TRUE;
}

/*!
  Changes color of grid
  \param r, g, b - components of color
*/
void GLViewer_Grid::setLineColor( GLfloat r, GLfloat g, GLfloat b )
{
  if( myLineColor[0] == r && myLineColor[1] == g && myLineColor[2] == b )
    return;

  myLineColor[0] = r;
  myLineColor[1] = g;
  myLineColor[2] = b;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets axis line width
  \param value - new axis line width
*/
void GLViewer_Grid::setAxisLineWidth( float value )
{
  if( myAxisLineWidth == value )
    return;

  myAxisLineWidth = value;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets axis line type
  \param type - new axis line type
*/
void GLViewer_Grid::setAxisLineType( LineType type )
{
  if( myAxisLineType == type )
    return;

  myAxisLineType = type;
  myIsUpdate = GL_TRUE;
}

/*!
  Changes color of axis
  \param r, g, b - components of color
*/
void GLViewer_Grid::setAxisLineColor( GLfloat r, GLfloat g, GLfloat b )
{
  if( myAxisLineColor[0] == r && myAxisLineColor[1] == g && myAxisLineColor[2] == b )
    return;

  myAxisLineColor[0] = r;
  myAxisLineColor[1] = g;
  myAxisLineColor[2] = b;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets Radius of center point( begin coords )
  \param r - new radius
*/
void GLViewer_Grid::setCenterRadius( int r )
{
  if( myCenterRadius == r )
    return;

  myCenterRadius = r;
  myIsUpdate = GL_TRUE;
}

/*!
  Sets grid scale factor
  \param scaleFactor - scale factor
*/
void GLViewer_Grid::setScaleFactor( float scaleFactor )
{
  if( myScaleFactor == scaleFactor )
    return;
 
  myScaleFactor = scaleFactor;
  myIsUpdate = GL_TRUE; 
}

/*!
  Sets grid scale ratio
  \param scaleFactor - scale factor
*/
void GLViewer_Grid::setScaleRatio( float scaleRatio )
{
  if( myScaleRatio == scaleRatio )
    return;
 
  myScaleRatio = scaleRatio;
  myIsUpdate = GL_TRUE; 
}

/*!
  Gets dash pattern by line type
  \param type - line type
*/
GLushort GLViewer_Grid::getDashPattern( const LineType type )
{
  switch( type )
  {
    case LongDash:  return 0xF0F0;
    case ShortDash: return 0xCCCC;
    case Dot:       return 0x8888;
  }
  return 0;
}

/*!
  Initialize grid display list
*/
bool GLViewer_Grid::initList()
{
  myIsUpdate = GL_FALSE;
   
  if( myXSize == (GLfloat)0.0 )
      myXSize = (GLfloat)0.1;
  if( myYSize == (GLfloat)0.0 )
      myYSize = (GLfloat)0.1;

label:
  if( ( myXSize >= myGridWidth / myScaleRatio ) &&
      ( myYSize >= myGridHeight / myScaleRatio ) )
  { //zoom in
    myXSize /= myScaleFactor;
    myYSize /= myScaleFactor;
    goto label;
  }
  else if( ( myXSize * myScaleFactor < myGridWidth / myScaleRatio ) ||
           ( myYSize * myScaleFactor < myGridHeight / myScaleRatio ) )
  { //zoom out
    myXSize *= myScaleFactor;
    myYSize *= myScaleFactor;
    goto label;
  }

  //int n = myGridWidth / myXSize;
  //int m = myGridHeight / myYSize;
  // do not initialise integer by float
  //if( ( n != 0 ) || ( m != 0 ) ) 
  if( ( myGridWidth > 0.5 * myXSize ) || ( myGridHeight > 0.5 * myYSize ) )
  { 
    if ( myGridList != 0 )  
    { 
      glDeleteLists( myGridList, 1 );
      // temporarily commented (too cruel)
      //if ( glGetError() != GL_NO_ERROR )
      //  return FALSE;
    } 

    float xLoc = (int)(myXPan / myXSize) * myXSize; 
    float yLoc = (int)(myYPan / myYSize) * myYSize; 
 
    myGridList = glGenLists( 1 ); 
    glNewList( myGridList, GL_COMPILE ); 

    glColor3f( myLineColor[0], myLineColor[1], myLineColor[2] );  
    glLineWidth( myLineWidth ); 

    GLushort aLineDashPattern = getDashPattern( myLineType );
    if( aLineDashPattern )
    {
      glEnable( GL_LINE_STIPPLE );
      glLineStipple( 1, aLineDashPattern );
    }

    glBegin( GL_LINES ); 
    for( int j = 0; ( j-1 ) * myXSize <= myGridWidth / 2; j++ )
    { 
      glVertex2d( -myXSize * j - xLoc, -myGridHeight / 2 - myYSize - yLoc );
      glVertex2d( -myXSize * j - xLoc,  myGridHeight / 2 + myYSize - yLoc );
      if( j != 0 )
      {
        glVertex2d(  myXSize * j - xLoc, -myGridHeight / 2 - myYSize - yLoc );
        glVertex2d(  myXSize * j - xLoc,  myGridHeight / 2 + myYSize - yLoc );
      }
    }
    for( int i = 0; ( i-1 ) * myYSize <= myGridHeight / 2; i++ )  
    {
      glVertex2d( -myGridWidth / 2 - myXSize - xLoc, -myYSize * i - yLoc );
      glVertex2d(  myGridWidth / 2 + myXSize - xLoc, -myYSize * i - yLoc );
      if( i != 0 )
      {
        glVertex2d( -myGridWidth / 2 - myXSize - xLoc,  myYSize * i - yLoc );
        glVertex2d(  myGridWidth / 2 + myXSize - xLoc,  myYSize * i - yLoc );
      }
    }
    glEnd();

    if( aLineDashPattern )
      glDisable( GL_LINE_STIPPLE );

    glColor3f( myAxisLineColor[0], myAxisLineColor[1], myAxisLineColor[2] );
    glLineWidth( myAxisLineWidth );

    GLushort anAxisLineDashPattern = getDashPattern( myAxisLineType );
    if( anAxisLineDashPattern )
    {
      glEnable( GL_LINE_STIPPLE );
      glLineStipple( 1, anAxisLineDashPattern );
    }

    glBegin( GL_LINES );
    glVertex2d(  myGridWidth / 2 + myXSize - xLoc, 0 ); 
    glVertex2d( -myGridWidth / 2 - myXSize - xLoc, 0 ); 
    glVertex2d( 0,  myGridHeight / 2 + myYSize - yLoc );
    glVertex2d( 0, -myGridHeight / 2 - myYSize - yLoc );    
    glEnd();

    if( anAxisLineDashPattern )
      glDisable( GL_LINE_STIPPLE );

    if( myCenterRadius > 0 )
    {
      glBegin( GL_LINE_LOOP ); 
      double angle = 0.0;
      for ( int k = 0; k < SEGMENTS; k++ )     
      { 
        glVertex2f( cos(angle) * myCenterRadius * myXScale,
                    sin(angle) * myCenterRadius * myYScale ); 
        angle += STEP; 
      } 
      glEnd();
    }

    glEndList();
  }
  return TRUE;
}
