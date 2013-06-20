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

#ifndef GRAPHICSVIEW_VIEWPORT_H
#define GRAPHICSVIEW_VIEWPORT_H

#include "GraphicsView.h"

#include "GraphicsView_Defs.h"

#include <QGraphicsView>

class QGridLayout;
class QRubberBand;

class GraphicsView_Object;
class GraphicsView_Scene;

/*
  Class       : GraphicsView_ViewPort
  Description : View port of the graphics view
*/
class GRAPHICSVIEW_API GraphicsView_ViewPort : public QGraphicsView
{
  Q_OBJECT

public:
  class NameLabel;

  enum InteractionFlags
  {
    Highlighting = 0x0001, // not implemented yet
    Selecting    = 0x0002, // not implemented yet
    Dragging     = 0x0004,
    Pulling      = 0x0008,
    WheelScaling = 0x0010,
    EditFlags    = Dragging | Pulling | WheelScaling,
    AllFlags     = Highlighting | Selecting | Dragging | Pulling | WheelScaling
  };

  enum BlockStatus
  {
    BS_NoBlock   = 0x0000,
    BS_Selection = 0x0001, // and highlighting
    BS_Dragging  = 0x0002  // currently unused
  };

  enum NamePosition
  {
    NP_None         = 0,
    NP_TopLeft      = 1,
    NP_TopRight     = 2,
    NP_BottomLeft   = 3,
    NP_BottomRight  = 4
  };

public:
  GraphicsView_ViewPort( QWidget* theParent );
  ~GraphicsView_ViewPort();

public:
  GraphicsView_Scene*              getScene() { return myScene; }
  void                             addItem( QGraphicsItem* theItem );
  void                             removeItem( QGraphicsItem* theItem );

  GraphicsView_ObjectList          getObjects( bool theIsSortSelected = false ) const;

  QRectF                           objectsBoundingRect( bool theOnlyVisible = false ) const;

  QImage                           dumpView( bool theWholeScene = false,
                                             QSizeF theSize = QSizeF() );

public:
  // scene
  void                             setSceneGap( double theSceneGap );
  void                             setFitAllGap( double theFitAllGap );
  void                             setTraceBoundingRectEnabled( bool theState );

  // interaction flags
  void                             setInteractionFlags( const int );
  void                             clearInteractionFlags( const int );
  bool                             testInteractionFlags( const int ) const;

  // view name
  void                             setViewNamePosition( NamePosition thePosition,
                                                        bool theIsForced = false );
  void                             setViewName( const QString& theName );

  // background / foreground
  QColor                           backgroundColor() const;
  void                             setBackgroundColor( const QColor& theColor );

  bool                             isForegroundEnabled() const { return myIsForegroundEnabled; }
  void                             setForegroundEnabled( bool theState );

  QSizeF                           foregroundSize() const { return myForegroundSize; }
  void                             setForegroundSize( const QSizeF& theRect );

  double                           foregroundMargin() const { return myForegroundMargin; }
  void                             setForegroundMargin( double theMargin );

  QColor                           foregroundColor() const { return myForegroundColor; }
  void                             setForegroundColor( const QColor& theColor );

  QColor                           foregroundFrameColor() const { return myForegroundFrameColor; }
  void                             setForegroundFrameColor( const QColor& theColor );

  double                           foregroundFrameLineWidth() const { return myForegroundFrameLineWidth; }
  void                             setForegroundFrameLineWidth( double theLineWidth );

  void                             updateForeground();

  // grid
  void                             setGridEnabled( bool theState );
  void                             setGridCellSize( int theCellSize );
  void                             setGridLineStyle( int theLineStyle );
  void                             setGridLineColor( const QColor& theLineColor );
  void                             updateGrid();

  // transformation
  void                             reset();
  void                             pan( double theDX, double theDY );
  void                             setCenter( double theX, double theY );
  void                             zoom( double theX1, double theY1, double theX2, double theY2 );
  void                             fitRect( const QRectF& theRect );
  void                             fitSelect();
  void                             fitAll( bool theKeepScale = false );
  void                             fitWidth();

  bool                             isTransforming() const { return myIsTransforming; }

  // block status
  BlockStatus                      currentBlock();

  // highlighting
  virtual void                     highlight( double theX, double theY );
  void                             clearHighlighted();

  GraphicsView_Object*             getHighlightedObject() const { return myHighlightedObject; }

  // selection
  virtual int                      select( const QRectF& theRect, bool theIsAppend );
  void                             clearSelected();
  void                             setSelected( GraphicsView_Object* theObject );

  int                              nbSelected() const;
  void                             initSelected();
  bool                             moreSelected();
  bool                             nextSelected();
  GraphicsView_Object*             selectedObject();

  const GraphicsView_ObjectList&   getSelectedObjects() const { return mySelectedObjects; }


  // rectangle selection
  void                             startSelectByRect( int x, int y );
  void                             drawSelectByRect( int x, int y );
  void                             finishSelectByRect();
  bool                             isSelectByRect() const;
  QRect                            selectionRect();

  // dragging
  bool                             isDragging() { return myIsDragging; }

  // pulling
  bool                             startPulling( const QPointF& );
  void                             drawPulling( const QPointF& );
  void                             finishPulling();
  bool                             isPulling() const { return myIsPulling; }

public:
  static void                      createCursors();
  static void                      destroyCursors();
  static QCursor*                  getDefaultCursor() { return defCursor; }
  static QCursor*                  getHandCursor() { return handCursor; }
  static QCursor*                  getPanCursor() { return panCursor; }
  static QCursor*                  getPanglCursor() { return panglCursor; }
  static QCursor*                  getZoomCursor() { return zoomCursor; }

public slots:
  void                             onBoundingRectChanged();

protected slots:
  void                             onKeyEvent( QKeyEvent* );
  void                             onMouseEvent( QGraphicsSceneMouseEvent* );
  void                             onWheelEvent( QGraphicsSceneWheelEvent* );
  void                             onContextMenuEvent( QGraphicsSceneContextMenuEvent* );

protected:
  virtual void                     scrollContentsBy( int theDX, int theDY );

signals:
  void                             vpKeyEvent( QKeyEvent* );
  void                             vpMouseEvent( QGraphicsSceneMouseEvent* );
  void                             vpWheelEvent( QGraphicsSceneWheelEvent* );
  void                             vpContextMenuEvent( QGraphicsSceneContextMenuEvent* );

  void                             vpObjectBeforeMoving();
  void                             vpObjectAfterMoving( bool );

private:
  void                             initialize();
  void                             cleanup();

  void                             dragObjects( QGraphicsSceneMouseEvent* );

private:
  static int                       nCounter;
  static QCursor*                  defCursor;
  static QCursor*                  handCursor;
  static QCursor*                  panCursor;
  static QCursor*                  panglCursor;
  static QCursor*                  zoomCursor;

private:
  // scene
  GraphicsView_Scene*              myScene;
  double                           mySceneGap;
  double                           myFitAllGap;
  bool                             myIsTraceBoundingRectEnabled;
  GraphicsView_ObjectList          myObjects;

  // interaction flags
  int                              myInteractionFlags;

  // view name
  NameLabel*                       myNameLabel;
  NamePosition                     myNamePosition;
  QGridLayout*                     myNameLayout;

  // foreground
  bool                             myIsForegroundEnabled;
  QSizeF                           myForegroundSize;
  double                           myForegroundMargin;
  QColor                           myForegroundColor;
  QColor                           myForegroundFrameColor;
  double                           myForegroundFrameLineWidth;
  QGraphicsRectItem*               myForegroundItem;

  // grid
  bool                             myIsGridEnabled;
  int                              myGridCellSize;
  int                              myGridLineStyle;
  QColor                           myGridLineColor;
  QGraphicsPathItem*               myGridItem;

  // transformation
  bool                             myIsTransforming;
  QTransform                       myCurrentTransform;

  // highlighting
  GraphicsView_Object*             myHighlightedObject;
  double                           myHighlightX;
  double                           myHighlightY;

  // selection
  GraphicsView_ObjectList          mySelectedObjects;
  int                              mySelectionIterator;

  // rectangle selection
  QRubberBand*                     myRectBand;
  QPoint                           myFirstSelectionPoint;
  QPoint                           myLastSelectionPoint;
  bool                             myAreSelectionPointsInitialized;

  // dragging
  int                              myIsDragging;
  QPointF                          myDragPosition;
  bool                             myIsDragPositionInitialized;

  // pulling
  bool                             myIsPulling;
  GraphicsView_Object*             myPullingObject;
};

#endif
