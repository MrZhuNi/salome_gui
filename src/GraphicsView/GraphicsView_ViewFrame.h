// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
// File:   GraphicsView_ViewFrame.h
// Author: Oleg UVAROV, Open CASCADE S.A.S. (oleg.uvarov@opencascade.com)
//

#ifndef GRAPHICSVIEW_VIEWFRAME_H
#define GRAPHICSVIEW_VIEWFRAME_H

#include "GraphicsView.h"

#include <SUIT_ViewWindow.h>

class QGraphicsSceneContextMenuEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

class SUIT_Desktop;

class GraphicsView_Viewer;
class GraphicsView_ViewPort;

/*
  Class       : GraphicsView_ViewFrame
  Description : View frame of the graphics view
*/
class GRAPHICSVIEW_API GraphicsView_ViewFrame : public SUIT_ViewWindow
{
  Q_OBJECT

public:
  enum { DumpId, FitAllId, FitRectId, FitSelectId, ZoomId, PanId, GlobalPanId, ResetId };

public:
  GraphicsView_ViewFrame( SUIT_Desktop*, GraphicsView_Viewer* );
  ~GraphicsView_ViewFrame();

public:
  GraphicsView_Viewer*    getViewer() const { return myViewer; }
  GraphicsView_ViewPort*  getViewPort() const  { return myViewPort; }

  virtual QImage          dumpView();

protected slots:
  void                    onViewPan();
  void                    onViewZoom();
  void                    onViewFitAll();
  void                    onViewFitArea();
  void                    onViewFitSelect();
  void                    onViewGlobalPan();
  void                    onViewReset();

private slots:
  void                    mouseEvent( QGraphicsSceneMouseEvent* );
  void                    wheelEvent( QGraphicsSceneWheelEvent* );
  void                    contextMenuEvent( QGraphicsSceneContextMenuEvent* );

signals:
  void                    mousePressed( QGraphicsSceneMouseEvent* );
  void                    mouseMoving( QGraphicsSceneMouseEvent* );
  void                    mouseReleased( QGraphicsSceneMouseEvent* );
  void                    mouseDoubleClicked( QGraphicsSceneMouseEvent* );
  void                    wheeling( QGraphicsSceneWheelEvent* );

private:
  void                    createActions();
  void                    createToolBar();

private:
  GraphicsView_Viewer*    myViewer;
  GraphicsView_ViewPort*  myViewPort;
};

#endif
