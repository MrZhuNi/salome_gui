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
// File:   GraphicsView_Viewer.h
// Author: Oleg UVAROV, Open CASCADE S.A.S. (oleg.uvarov@opencascade.com)
//

#ifndef GRAPHICSVIEW_VIEWER_H
#define GRAPHICSVIEW_VIEWER_H

#include "GraphicsView.h"

#include "GraphicsView_Defs.h"

#include <SUIT_ViewModel.h>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QKeyEvent;

class SUIT_ViewWindow;

class GraphicsView_Selector;
class GraphicsView_Scene;
class GraphicsView_ViewTransformer;
class GraphicsView_ViewFrame;
class GraphicsView_ViewPort;

/*
  Class       : GraphicsView_Viewer
  Description : View model of the graphics view
*/
class GRAPHICSVIEW_API GraphicsView_Viewer: public SUIT_ViewModel
{
  Q_OBJECT

public:
  enum TransformType { NoTransform, Reset, FitAll, FitRect, FitSelect,
                       Zoom, PanGlobal, Pan, FitWidth, UserTransform = 100 };
public:
  GraphicsView_Viewer( const QString& title );
  ~GraphicsView_Viewer();

public:
  virtual SUIT_ViewWindow*      createView( SUIT_Desktop* );

  virtual QString               getType() const { return Type(); }
  static QString                Type() { return "GraphicsView";  }

  virtual void                  contextMenuPopup( QMenu* );

public:
  GraphicsView_Selector*        getSelector();

  GraphicsView_ViewFrame*       getActiveView() const;
  GraphicsView_ViewPort*        getActiveViewPort() const;
  GraphicsView_Scene*           getActiveScene() const;

  void                          activateTransform( int );

  bool                          isInitialized() const { return myIsInitialized; }
  void                          setIsInitialized( bool );

  bool                          isImmediateSelectionEnabled() const { return myIsImmediateSelectionEnabled; }
  void                          setIsImmediateSelectionEnabled( bool );

signals:
  void                          selectionChanged( GV_SelectionChangeStatus );
  void                          wheelScaleChanged();

protected:
  virtual GraphicsView_ViewTransformer* createTransformer( int );

  virtual void                  onTransformationStarted();
  virtual void                  onTransformationFinished();

protected slots:
  virtual void                  onKeyEvent( QKeyEvent* );
  virtual void                  onMouseEvent( QGraphicsSceneMouseEvent* );
  virtual void                  onWheelEvent( QGraphicsSceneWheelEvent* );

  virtual void                  onSelectionDone( GV_SelectionChangeStatus );
  virtual void                  onSelectionCancel();

  virtual void                  onChangeBgColor();

private:
  void                          handleKeyPress( QKeyEvent* );
  void                          handleKeyRelease( QKeyEvent* );
  void                          handleMouseMove( QGraphicsSceneMouseEvent* );
  void                          handleMousePress( QGraphicsSceneMouseEvent* );
  void                          handleMouseRelease( QGraphicsSceneMouseEvent* );
  void                          handleWheel( QGraphicsSceneWheelEvent* );

protected:
  GraphicsView_Selector*        mySelector;
  GraphicsView_ViewTransformer* myTransformer;

  bool                          myIsInitialized;
  bool                          myIsImmediateSelectionEnabled;
};

#endif