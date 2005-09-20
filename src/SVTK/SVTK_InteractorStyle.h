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
//  File   : SVTK_InteractorStyle.h
//  Author : Christophe ATTANASIO
//  Module : SALOME
//  $Header$

#ifndef __SVTK_InteractorStyle_h
#define __SVTK_InteractorStyle_h

#include "SVTK.h"

#include "SVTK_SelectionEvent.h"

#include <boost/shared_ptr.hpp>

#include <vtkInteractorStyle.h>
#include <vtkSmartPointer.h>

#include <qcursor.h>
#include <qevent.h>

#include <map>

class vtkCell;
class vtkPicker;
class VTKViewer_RectPicker;

class SVTK_Selector;
class SVTK_GenericRenderWindowInteractor;

#define VTK_INTERACTOR_STYLE_CAMERA_NONE    0
#define VTK_INTERACTOR_STYLE_CAMERA_ROTATE  1
#define VTK_INTERACTOR_STYLE_CAMERA_PAN     2
#define VTK_INTERACTOR_STYLE_CAMERA_ZOOM    3
#define VTK_INTERACTOR_STYLE_CAMERA_SPIN    4
#define VTK_INTERACTOR_STYLE_CAMERA_FIT        5
#define VTK_INTERACTOR_STYLE_CAMERA_SELECT     6
#define VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN 7

class SVTK_EXPORT SVTK_InteractorStyle: public vtkInteractorStyle
{
 public:
  // Description:
  // This class must be supplied with a vtkRenderWindowInteractor wrapper or
  // parent. This class should not normally be instantiated by application
  // programmers.
  static SVTK_InteractorStyle *New();
  vtkTypeMacro(SVTK_InteractorStyle, vtkInteractorStyle);

  virtual
  int
  GetState();

  typedef boost::shared_ptr<SVTK_SelectionEvent> PSelectionEvent;

  virtual
  SVTK_SelectionEvent*
  GetSelectionEvent();

  virtual
  SVTK_SelectionEvent*
  GetSelectionEventFlipY();

  // redefined in order to add an observer (callback) for custorm event (space mouse event)
  virtual
  void
  SetInteractor( vtkRenderWindowInteractor* );

  virtual 
  void
  Render();

  // redefined in order to cach rendering
  virtual
  void
  OnTimer();

  // VTK events
  virtual
  void
  OnConfigure();

  virtual 
  void
  OnMouseMove();

  virtual
  void
  OnLeftButtonDown();

  virtual
  void
  OnLeftButtonUp();

  virtual
  void
  OnMiddleButtonDown();

  virtual
  void
  OnMiddleButtonUp();

  virtual
  void
  OnRightButtonDown();

  virtual
  void
  OnRightButtonUp();

 protected:
  SVTK_InteractorStyle();
  ~SVTK_InteractorStyle();

  QWidget*
  GetRenderWidget();

  SVTK_Selector* 
  GetSelector();

  // Generic event bindings must be overridden in subclasses
  virtual void OnMouseMove  (int ctrl, int shift, int x, int y);
  virtual void OnLeftButtonDown(int ctrl, int shift, int x, int y);
  virtual void OnLeftButtonUp  (int ctrl, int shift, int x, int y);
  virtual void OnMiddleButtonDown(int ctrl, int shift, int x, int y);
  virtual void OnMiddleButtonUp  (int ctrl, int shift, int x, int y);
  virtual void OnRightButtonDown(int ctrl, int shift, int x, int y);
  virtual void OnRightButtonUp  (int ctrl, int shift, int x, int y);

  void RotateXY(int dx, int dy);
  void PanXY(int x, int y, int oldX, int oldY);
  void DollyXY(int dx, int dy);
  void SpinXY(int dx, int dy, int oldX, int oldY);
  void fitRect(const int left, const int top, const int right, const int bottom);
  void Place(const int theX, const int theY);
  void TranslateView(int toX, int toY, int fromX, int fromY);

  void
  IncrementalPan( const int incrX, const int incrY );

  void
  IncrementalZoom( const int incr );

  void
  IncrementalRotate( const int incrX, const int incrY );

  // custom event handling function (to handle 3d space mouse events)
  static void ProcessEvents( vtkObject* object, unsigned long event,
			     void* clientData, void* callData );

  float MotionFactor;
  float RadianToDegree;                 // constant: for conv from deg to rad
  double myScale;

 public:
  void startZoom();
  void startPan();
  void startGlobalPan();
  void startRotate();
  void startFitArea();
  void startSpin();

 protected:
  void loadCursors();
  void startOperation(int operation);
  void onStartOperation();
  virtual void onFinishOperation();
  void onOperation(QPoint mousePos);
  void onCursorMove(QPoint mousePos);
  void setCursor(const int operation);

  void onSpaceMouseMove( double* data );
  void onSpaceMouseButton( int button );

  void DecreaseGaussPointMagnification();
  void IncreaseGaussPointMagnification();
  void DominantCombinedSwitch();
  
 protected:
  QCursor                   myDefCursor;
  QCursor                   myPanCursor;
  QCursor                   myZoomCursor;
  QCursor                   myRotateCursor;
  QCursor                   mySpinCursor;
  QCursor                   myHandCursor;
  QCursor                   myGlobalPanCursor;
  QPoint                    myPoint;
  QPoint                    myOtherPoint;
  bool                      myCursorState;
  bool                      myShiftState;
  int                       ForcedState;

  int                       mySpeedIncrement; // "increment" for pan/rotate/zoom operations
  int                       mySpaceMouseBtns[5];  // space_mouse_button assigned for functions 1..5 :
  // [1] Decrease Speed Increment
  // [2] Increase Speed Increment
  // [3] Decrease Gauss Point Magnification
  // [4] Increase Gauss Point Magnification
  // [5] Dominant Combined Switch
  
  QWidget* myRenderWidget;
  vtkSmartPointer<SVTK_GenericRenderWindowInteractor> myInteractor;

  PSelectionEvent mySelectionEvent;

  vtkSmartPointer<vtkPicker> myPicker;
  vtkSmartPointer<VTKViewer_RectPicker> myRectPicker;
};

#endif
