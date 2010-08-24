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

// File:      GLViewer_Grid.h
// Created:   November, 2004

#ifndef GLVIEWER_GRID_H
#define GLVIEWER_GRID_H

#ifdef WIN32
#include "windows.h"
#endif

#include "GLViewer.h"

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

/*! Class GLViewer_Grid
  2D rectangular grid for GLViewer
  Grid is adapt cells for current view
*/
class GLVIEWER_API GLViewer_Grid 
{
public:
  enum LineType { Solid = 0, LongDash, ShortDash, Dot };

public:
  GLViewer_Grid();
  ~GLViewer_Grid();

  //! Draws grid
  void                   draw();

  //! Sets grid enabled
  void                   setEnabled( GLboolean );

  //! Returns grid enable state
  GLboolean              isEnabled() const { return myIsEnabled; }

  //! Sets grid width
  void                   setGridWidth( float );

  //! Sets grid height
  void                   setGridHeight( float );

  //! Recomputes grid in new size and scale of view
  void                   setResize( float, float, float );

  //! Sets steps along x and y directions
  void                   setSize( float, float );

  //! Sets offset along x and y direction
  void                   setPan( float, float );

  //! Sets common scale factor along x and y direction
  bool                   setZoom( float );

  //! Sets line width
  void                   setLineWidth( float );

  //! Sets line type
  void                   setLineType( LineType );

  //! Sets line color in RGB format
  void                   setLineColor( GLfloat, GLfloat, GLfloat );

  //! Sets axis line width
  void                   setAxisLineWidth( float );

  //! Sets axis line type
  void                   setAxisLineType( LineType );

  //! Sets axis line color in RGB format
  void                   setAxisLineColor( GLfloat, GLfloat, GLfloat );

  //! Sets radius of center point
  void                   setCenterRadius( int );

  //! Sets grid scale factor
  void                   setScaleFactor( float );

  //! Sets grid scale ratio
  void                   setScaleRatio( float );

protected:
  //! Initialize grid display list
  bool                   initList();

  //! Gets dash pattern by line type
  static GLushort        getDashPattern( const LineType );

protected:
  GLboolean              myIsEnabled;

  GLuint                 myGridList;
  GLboolean              myIsUpdate;

  GLfloat                myGridWidth;
  GLfloat                myGridHeight;
  GLfloat                myWinWidth;
  GLfloat                myWinHeight;
  GLfloat                myXSize;
  GLfloat                myYSize;

  GLfloat                myXPan;
  GLfloat                myYPan;
  GLfloat                myXScale;
  GLfloat                myYScale;

  GLfloat                myLineWidth;
  LineType               myLineType;
  GLfloat                myLineColor[3];

  GLfloat                myAxisLineWidth;
  LineType               myAxisLineType;
  GLfloat                myAxisLineColor[3];

  GLint                  myCenterRadius;

  GLfloat                myScaleFactor;
  GLfloat                myScaleRatio;
};

#ifdef WIN32
//#pragma warning ( default:4251 )
#endif

#endif
