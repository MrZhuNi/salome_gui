//  SALOME VTKViewer : build VTK viewer into Salome desktop
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
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
//
//
//  File   : 
//  Author : 
//  Module : SALOME
//  $Header$

#ifndef SVTK_RenderWindowInteractor_h
#define SVTK_RenderWindowInteractor_h

#include "SVTK.h"
#include "SVTK_Selection.h"

#include <vtkSmartPointer.h>
#include <qwidget.h>
#include <stack>

class vtkGenericRenderWindowInteractor;
class vtkInteractorStyle;
class vtkCallbackCommand;
class vtkRenderWindow;
class vtkRenderer;
class vtkObject;

class SVTK_Selector;
class SVTK_Renderer;

// ------------------------------------------------------------
class SVTK_EXPORT QVTK_RenderWindowInteractor: public QWidget
{
  Q_OBJECT;

 public:
  QVTK_RenderWindowInteractor(QWidget* theParent, 
			      const char* theName);

  ~QVTK_RenderWindowInteractor();

  virtual
  void
  Initialize(vtkGenericRenderWindowInteractor* theDevice);

  vtkGenericRenderWindowInteractor* 
  GetDevice();

  vtkRenderWindow*
  getRenderWindow();

  virtual
  void
  InvokeEvent(unsigned long theEvent, void* theCallData);

 public slots:
  virtual void show();
  virtual void polish();
  virtual void resize(int w, int h);

 protected:
  virtual void paintEvent( QPaintEvent* );
  virtual void resizeEvent( QResizeEvent* );

  virtual void mouseMoveEvent( QMouseEvent* );
  virtual void mousePressEvent( QMouseEvent* );
  virtual void mouseReleaseEvent( QMouseEvent* );
  virtual void mouseDoubleClickEvent( QMouseEvent* );
  virtual void wheelEvent( QWheelEvent* );
  virtual void keyPressEvent( QKeyEvent* );
  virtual void keyReleaseEvent( QKeyEvent* );
  virtual void enterEvent( QEvent * );
  virtual void leaveEvent( QEvent * );

  virtual void contextMenuEvent( QContextMenuEvent * e );

  // reimplemented from QWidget in order to set window - receiver
  // of space mouse events. 
  virtual void focusInEvent( QFocusEvent* );
  virtual void focusOutEvent( QFocusEvent* );

  virtual bool x11Event( XEvent *e );

  QWidget* myPreviousFocusWidget;
  vtkSmartPointer<vtkRenderWindow> myRenderWindow;
  vtkSmartPointer<vtkGenericRenderWindowInteractor> myDevice;
};


// ------------------------------------------------------------
class SVTK_EXPORT SVTK_RenderWindowInteractor: public QVTK_RenderWindowInteractor
{
  Q_OBJECT;

 public:
  SVTK_RenderWindowInteractor(QWidget* theParent, 
			      const char* theName);

  ~SVTK_RenderWindowInteractor();

  virtual
  void
  Initialize(vtkGenericRenderWindowInteractor* theDevice,
	     SVTK_Renderer* theRenderer,
	     SVTK_Selector* theSelector);

  //----------------------------------------------------------------------------
  SVTK_Renderer* 
  GetRenderer();

  vtkRenderer* 
  getRenderer();

  //----------------------------------------------------------------------------
  vtkInteractorStyle* 
  GetInteractorStyle();

  void
  PushInteractorStyle(vtkInteractorStyle* theStyle);

  void
  PopInteractorStyle();

  //----------------------------------------------------------------------------
  SVTK_Selector* 
  GetSelector();

  Selection_Mode 
  SelectionMode() const;

  void 
  SetSelectionMode(Selection_Mode theMode);

 public:
  void
  onEmitSelectionChanged();

 public:
 signals:
  void MouseMove( QMouseEvent* );
  void MouseButtonPressed( QMouseEvent* );
  void MouseButtonReleased( QMouseEvent* );
  void MouseDoubleClicked( QMouseEvent* );
  void ButtonPressed(const QMouseEvent *event);
  void ButtonReleased(const QMouseEvent *event);
  void WheelMoved( QWheelEvent* );
  void KeyPressed( QKeyEvent* );
  void KeyReleased( QKeyEvent* );
  void contextMenuRequested( QContextMenuEvent *e );

  void selectionChanged();

 protected:
  virtual void mouseMoveEvent( QMouseEvent* );
  virtual void mousePressEvent( QMouseEvent* );
  virtual void mouseReleaseEvent( QMouseEvent* );
  virtual void mouseDoubleClickEvent( QMouseEvent* );
  virtual void wheelEvent( QWheelEvent* );
  virtual void keyPressEvent( QKeyEvent* );
  virtual void keyReleaseEvent( QKeyEvent* );
  virtual void contextMenuEvent( QContextMenuEvent * e );

  void
  SetRenderer(SVTK_Renderer *theRenderer);

  void
  SetSelector(SVTK_Selector* theSelector);

  void
  InitInteractorStyle(vtkInteractorStyle* theStyle);

  //----------------------------------------------------------------
  // Main process VTK event method
  static
  void
  ProcessEvents(vtkObject* theObject, 
		unsigned long theEvent,
		void* theClientData, 
		void* theCallData);

  // Used to process VTK events
  vtkSmartPointer<vtkCallbackCommand> myEventCallbackCommand;

  // Priority at which events are processed
  float myPriority;

  //----------------------------------------------------------------
  vtkSmartPointer<SVTK_Selector> mySelector;

  vtkSmartPointer<SVTK_Renderer> myRenderer;

  typedef vtkSmartPointer<vtkInteractorStyle> PInteractorStyle;
  typedef std::stack<PInteractorStyle> TInteractorStyles;
  TInteractorStyles myInteractorStyles;
};


#endif
