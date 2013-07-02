// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef GRAPHICSVIEW_PRSIMAGEFRAME_H
#define GRAPHICSVIEW_PRSIMAGEFRAME_H

#include "GraphicsView.h"

#include "GraphicsView_Object.h"

#include <QGraphicsEllipseItem>

class GraphicsView_PrsImage;

/*
  Class       : GraphicsView_PrsImageFrame
  Description : Presentation for image frame object
*/
class GRAPHICSVIEW_API GraphicsView_PrsImageFrame : public GraphicsView_Object
{
public:
  class UnscaledGraphicsEllipseItem;

  enum AnchorType { Undefined = 0, TopMost, Top, Bottom, Left, Right,
                    TopLeft, TopRight, BottomLeft, BottomRight };

  typedef QMap        <int, UnscaledGraphicsEllipseItem*> AnchorMap;
  typedef QMapIterator<int, UnscaledGraphicsEllipseItem*> AnchorMapIterator;

public:
  GraphicsView_PrsImageFrame();
  virtual ~GraphicsView_PrsImageFrame();

public:
  // from QGraphicsItem
  virtual QRectF                  boundingRect() const;

  // from GraphicsView_Object
  virtual void                    compute();

  virtual bool                    hasSpecificZValue() const { return true; }

  virtual bool                    isMovable() const { return false; }

  virtual bool                    checkHighlight( double theX, double theY, QCursor& theCursor ) const;

  virtual QRectF                  getPullingRect() const;
  virtual bool                    portContains( const QPointF& ) { return false; } // useless
  virtual bool                    startPulling( const QPointF& );
  virtual void                    pull( const QPointF&,
                                        GraphicsView_Object*,
                                        const GraphicsView_ObjectList& );
  virtual void                    finishPulling( const GraphicsView_ObjectList& );
  virtual bool                    isPulling() { return myIsPulling; }

  virtual QPointF                 centerPoint();

public:
  void                            setPrsImage( GraphicsView_PrsImage* );

  void                            computeAnchorItems();
  void                            updateAnchorItems();

  void                            setRotationAngle( const double theRotationAngle );

protected:
  QCursor*                        getVerCursor() const { return myVerCursor; }
  QCursor*                        getHorCursor() const { return myHorCursor; }
  QCursor*                        getBDiagCursor() const { return myBDiagCursor; }
  QCursor*                        getFDiagCursor() const { return myFDiagCursor; }
  QCursor*                        getRotateCursor() const { return myRotateCursor; }

  QCursor*                        getResizeCursor( const int theAnchor ) const;

protected:
  GraphicsView_PrsImage*          myPrsImage;

  bool                            myIsPulling;
  int                             myPullingAnchor;
  QPointF                         myPullingPoint;

  AnchorMap                       myAnchorMap;

private:
  QCursor*                        myVerCursor;
  QCursor*                        myHorCursor;
  QCursor*                        myBDiagCursor;
  QCursor*                        myFDiagCursor;
  QCursor*                        myRotateCursor;
};

/*
  Class       : UnscaledGraphicsEllipseItem
  Description : Class for unscaled ellipse item
*/
class GraphicsView_PrsImageFrame::UnscaledGraphicsEllipseItem : public QGraphicsEllipseItem
{
public:
  UnscaledGraphicsEllipseItem( QGraphicsItem* );
  virtual ~UnscaledGraphicsEllipseItem();

public:
  void           setBasePoint( const QPointF& thePoint ) { myBasePoint = thePoint; }
  const QPointF& getBasePoint() const { return myBasePoint; }

  void           setOffset( const QPointF& theOffset ) { myOffset = theOffset; }
  const QPointF& getOffset() const { return myOffset; }

  void           setRotationAngle( const double theAngle ) { myRotationAngle = theAngle; }

public:
  virtual QRectF boundingRect() const;
  virtual void   paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* );

private:
  QPointF        myBasePoint;
  QPointF        myOffset;

  double         myRotationAngle;
};

#endif
