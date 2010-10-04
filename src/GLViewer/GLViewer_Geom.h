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

// File:      GLViewer_Geom.h
// Created:   November, 2004

#ifndef GLVIEWER_GEOM_H
#define GLVIEWER_GEOM_H

#include "GLViewer.h"

#include <QRect>
#include <QRectF>
#include <math.h>

//using namespace QGL;

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include <QtOpenGL>

/*! Struct GLViewer_Pnt
* Substitution of QPoint for OpenGL
*/

struct GLVIEWER_API GLViewer_Pnt
{
public:
  GLViewer_Pnt() : myX( 0. ), myY( 0. ) {};
  GLViewer_Pnt( GLfloat theX, GLfloat theY ) : myX( theX ), myY( theY ) {}
  
  GLfloat x() const { return myX; }
  GLfloat y() const { return myY; }
  void    setX( GLfloat theX ) { myX = theX; }
  void    setY( GLfloat theY ) { myY = theY; }
  void    setXY( GLfloat theX, GLfloat theY ) { myX = theX; myY = theY; }
  void    move( GLfloat theDX, GLfloat theDY ) { myX += theDX; myY += theDY; }
  
private:
  GLfloat myX;
  GLfloat myY;
};

typedef QList<GLViewer_Pnt> GLViewer_PntList;

/*! Class  GLViewer_Rect
*  Substitution of QRect for OpenGL
*/

class GLVIEWER_API GLViewer_Rect
{
public:
  GLViewer_Rect(): myLeft(0.0), myRight(0.0), myBottom(0.0), myTop(0.0) {}
  GLViewer_Rect( float theLeft, float theRight, float theBottom, float theTop )
    : myLeft(theLeft), myRight(theRight), myBottom(theBottom), myTop(theTop) {}
  GLViewer_Rect( QRect theRect ) {
    myLeft = ( float )theRect.left(); myRight = ( float )theRect.right();
    myBottom = ( float )theRect.bottom(); myTop = ( float )theRect.top(); }
  GLViewer_Rect( QRectF theRect ) {
    myLeft = theRect.left(); myRight = theRect.right();
    myBottom = theRect.bottom(); myTop = theRect.top(); }
  
  float       left() const { return myLeft; }
  float       right() const { return myRight; }
  float       bottom() const { return myBottom; }
  float       top() const { return myTop; }

  float       width() const { return fabs( myRight - myLeft ); }
  float       height() const { return fabs( myTop - myBottom ); }
  
  void        setLeft( float theLeft ) { myLeft = theLeft; }
  void        setRight( float theRight ) { myRight = theRight; }
  void        setBottom( float theBottom ) { myBottom = theBottom; }
  void        setTop( float theTop ) { myTop = theTop; }
  
  void        setCoords( float theLeft, float theRight, float theBottom, float theTop )
  { myLeft = theLeft; myRight = theRight; myBottom = theBottom; myTop = theTop; }
  
  //! \warning This method translate only rect format
  QRect       toQRect() { return QRect( ( int )myLeft, ( int )myBottom,
                                        ( int )( myRight - myLeft ),
                                        ( int )( myTop - myBottom ) ); }

  //! \warning This method translate only rect format
  QRectF      toQRectF() { return QRectF( myLeft, myBottom,
                                          myRight - myLeft,
                                          myTop - myBottom ); }

  //! On/off empty status
  void        setIsEmpty( bool on ) { myIsEmpty = on; }
  //! Checks empty status
  bool        isEmpty() const { return myIsEmpty; }

  //! Checks null status
  bool        isNull() const { return myLeft == 0.0 && myRight == 0.0 && myBottom == 0.0 && myTop == 0.0; }
  //! Checks valid status
  bool        isValid() const { return ( myLeft < myRight && myBottom < myTop ); }

  //! Checks that the rectangle contains point
  bool        contains( const GLViewer_Pnt& thePnt ) const;

  //! Checks that the rectangle contains another rectangle
  bool        contains( const GLViewer_Rect& theRect ) const;

  //! Checks that X projection of the rectangle contains X projection of another rectangle
  bool        containsByX( const GLViewer_Rect& theRect ) const;

  //! Checks that Y projection of the rectangle contains Y projection of another rectangle
  bool        containsByY( const GLViewer_Rect& theRect ) const;

  //! Moves the rectangle
  void        move( const float theDX, const float theDY );

protected:
  float       myLeft;
  float       myRight;
  float       myBottom;
  float       myTop;

  bool        myIsEmpty;
};

/*! Class GLViewer_Segment
* Segment for 2d detection
*/

class GLVIEWER_API GLViewer_Segment
{
public:
  GLViewer_Segment( const GLViewer_Pnt& thePnt1, 
                    const GLViewer_Pnt& thePnt2 );
  
  //! Ordinary segment construction
  /*!Construction of a ray with given equation Ax + By + C = 0 */

  GLViewer_Segment( const GLViewer_Pnt& thePnt, 
                    const GLfloat theA, 
                    const GLfloat theB,
                    const GLfloat theC );
  ~GLViewer_Segment();

  bool              HasIntersection( const GLViewer_Segment& theOther ) const;
  // Detects intersection with another segment or ray

private:
  GLViewer_Pnt      myPnt1;
  GLViewer_Pnt      myPnt2;
  GLfloat           myA;
  GLfloat           myB;
  GLfloat           myC;
};

/*! Class  GLViewer_Poly
* Polygon for 2d detection
*/

class GLVIEWER_API GLViewer_Poly 
{
public:
  GLViewer_Poly( const GLViewer_PntList* thePoints );
  virtual ~GLViewer_Poly();

  //! Adds point to polygon
  void              AddPoint( const GLViewer_Pnt& thePnt ) { myPoints->append( thePnt ); }

  //! Returns number of point
  int               Count() const { return myPoints->count(); }

  //! Returns true if a point lies inside this polygon
  virtual bool      IsIn( const GLViewer_Pnt& thePnt ) const;

  //! Returns true if a other polygon covers this polygon  
  virtual bool      IsCovers( const GLViewer_Poly& thePoly ) const;

  //! Likes the above function
  virtual bool      IsCovers( const GLViewer_Rect& theRect ) const;
  
  // Returns true if intersection of this polygon with a segment or a ray not empty
  virtual bool      HasIntersection( const GLViewer_Segment& theSegment ) const;

private:
  GLViewer_PntList* myPoints;
};

#ifdef WIN32
//#pragma warning ( default:4251 )
#endif

#endif
