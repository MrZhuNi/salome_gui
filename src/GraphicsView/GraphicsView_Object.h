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

#ifndef GRAPHICSVIEW_OBJECT_H
#define GRAPHICSVIEW_OBJECT_H

#include "GraphicsView.h"

#include <QGraphicsItemGroup>

class GraphicsView_ViewPort;

/*
  Class       : GraphicsView_Object
  Description : Base class for all objects displayed at the scene
*/
class GRAPHICSVIEW_API GraphicsView_Object : public QGraphicsItemGroup
{
public:
  GraphicsView_Object( QGraphicsItem* theParent = 0 );
  ~GraphicsView_Object();

  virtual void               compute() = 0;

  virtual void               addTo( GraphicsView_ViewPort* theViewPort );
  virtual void               removeFrom( GraphicsView_ViewPort* theViewPort );

  const QString&             getName() const { return myName; }
  virtual void               setName( const QString& theName );

  virtual int                getPriority() const { return myPriority; }

  virtual bool               isOnTop() const { return myIsOnTop; }
  virtual void               setIsOnTop( bool theIsOnTop ) { myIsOnTop = theIsOnTop; }

  virtual bool               isSelectable() const { return true; }
  virtual bool               isMovable() const { return true; }

  virtual QRectF             getRect() const;

  virtual bool               checkHighlight( double theX, double theY, QCursor& theCursor ) const;

  virtual bool               highlight( double theX, double theY );
  virtual void               unhighlight();
  virtual bool               isHighlighted() const { return myIsHighlighted; }

  virtual bool               select( double theX, double theY, const QRectF& theRect );
  virtual void               unselect();
  virtual bool               isSelected() const { return myIsSelected; }
  virtual void               setSelected( bool theState ) { myIsSelected = theState; }

  virtual void               move( double theDX, double theDY, bool theIsAtOnce = false );
  virtual bool               finishMove();
  virtual bool               isMoving() const { return myIsMoving; }
  virtual bool               isMovingByXAllowed( double theDX ) { return true; }
  virtual bool               isMovingByYAllowed( double theDY ) { return true; }

  virtual bool               updateScale( bool theIsScaleUp, bool theIsCtrl ) { return false; }

  virtual QRectF             getPullingRect() const { return getRect(); }
  virtual bool               portContains( const QPointF& ) { return false; }
  virtual bool               startPulling( const QPointF& ) { return false; }
  virtual void               pull( const QPointF&, GraphicsView_Object* ) {}
  virtual void               finishPulling() {}
  virtual bool               isPulling() { return false; }

  virtual bool               handleMousePress( QGraphicsSceneMouseEvent* ) { return false; }
  virtual bool               handleMouseMove( QGraphicsSceneMouseEvent* ) { return false; }
  virtual bool               handleMouseRelease( QGraphicsSceneMouseEvent* ) { return false; }

  virtual QPointF            centerPoint();

public:
  static void                setRotationAroundCenter( QGraphicsItem* theItem, double theAngle );

protected:
  QCursor*                   getHighlightCursor() const { return myHighlightCursor; }

protected:
  QString                    myName;

  int                        myPriority;
  bool                       myIsOnTop;

  bool                       myIsHighlighted;
  bool                       myIsSelected;

  bool                       myIsMoving;

private:
  QCursor*                   myHighlightCursor;
};

#endif
