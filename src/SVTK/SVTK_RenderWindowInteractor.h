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
//  File   : SVTK_RenderWindowInteractor.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SVTK_RenderWindowInteractor_h
#define SVTK_RenderWindowInteractor_h

#include "SVTK.h"
#include "SVTK_Selection.h"
#include "SVTK_RenderWindow.h"

#include "SALOME_InteractiveObject.hxx"

#include <vtkSmartPointer.h>
#include <vtkGenericRenderWindowInteractor.h>

#include <qvaluestack.h>

class QTimer;

class vtkCallbackCommand;
class vtkActorCollection;
class vtkGenericRenderWindowInteractor;

class SALOME_Actor;
class SVTK_Selector;
class SVTK_InteractorStyle;

// ------------------------------------------------------------
// :TRICKY: Fri Apr 21 22:19:27 2000 Pagey
// The Signal/Slot mechanism used by Qt requires that QObject 
// appear as the first class when using multiple inheritance. 
// Hence the order of the two classes QObject and vtkRenderWindowInteractor
// matters here. Be careful not to change it by accident. 
// ------------------------------------------------------------
class SVTK_EXPORT QtRenderWindowInteractor: 
 public QObject,
 public vtkGenericRenderWindowInteractor
{
  Q_OBJECT;

 public:
  static QtRenderWindowInteractor* New();
  vtkTypeMacro(QtRenderWindowInteractor,vtkGenericRenderWindowInteractor);

  virtual
  int
  CreateTimer( int ) ; 

  virtual
  int
  DestroyTimer() ; 

 protected slots:
  virtual
  void
  OnTimeOut();

 protected:
  QtRenderWindowInteractor();
  ~QtRenderWindowInteractor();

  QTimer* myTimer ;
};


// ------------------------------------------------------------
class SVTK_EXPORT QVTK_RenderWindowInteractor: public QWidget
{
  Q_OBJECT;

 public:
  QVTK_RenderWindowInteractor(QWidget* theParent, 
			      const char* theName);

  ~QVTK_RenderWindowInteractor();

  vtkRenderWindowInteractor* 
  GetDevice();

  void
  SetRenderWindow(vtkRenderWindow *theRenderWindow);

  vtkRenderWindow*
  GetRenderWindow();

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
  virtual void contextMenuEvent( QContextMenuEvent * e );

  // reimplemented from QWidget in order to set window - receiver
  // of space mouse events. 
  virtual void focusInEvent( QFocusEvent* );
  virtual void focusOutEvent( QFocusEvent* );

  virtual bool x11Event( XEvent *e );

  vtkSmartPointer<vtkGenericRenderWindowInteractor> myInteractor;
};


// ------------------------------------------------------------
class SVTK_EXPORT SVTK_RenderWindowInteractor: public QVTK_RenderWindowInteractor
{
  Q_OBJECT;

 public:
  SVTK_RenderWindowInteractor(QWidget* theParent, 
			      const char* theName);

  ~SVTK_RenderWindowInteractor();

  SVTK_Selector* GetSelector();
  void SetSelector(SVTK_Selector* theSelector);

  SVTK_InteractorStyle* GetInteractorStyle();
  void PushInteractorStyle(SVTK_InteractorStyle* theStyle);
  void PopInteractorStyle();

  int SelectionMode() const;
  void SetSelectionMode(int theMode);

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
  InitInteractorStyle(SVTK_InteractorStyle* theStyle);

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

  vtkSmartPointer<SVTK_Selector> mySelector;

  typedef vtkSmartPointer<SVTK_InteractorStyle> PInteractorStyle;
  QValueStack<PInteractorStyle> myInteractorStyles;
};

#endif
