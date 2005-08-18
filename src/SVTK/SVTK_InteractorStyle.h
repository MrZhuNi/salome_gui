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

#include <vtkInteractorStyle.h>
#include <vtkSmartPointer.h>

class vtkCell;
class vtkRenderWindowInteractor;

#include <qobject.h>
#include <qcursor.h>
#include <qevent.h>

#include <map>

#include "VTKViewer_Filter.h"

#include "SVTK_SelectionEvent.h"
#include "SALOME_Actor.h"

class vtkPicker;
class VTKViewer_RectPicker;

class SALOME_Actor;
class SVTK_Viewer;
class SVTK_Selector;
class SVTK_ViewWindow;

#define VTK_INTERACTOR_STYLE_CAMERA_NONE    0
#define VTK_INTERACTOR_STYLE_CAMERA_ROTATE  1
#define VTK_INTERACTOR_STYLE_CAMERA_PAN     2
#define VTK_INTERACTOR_STYLE_CAMERA_ZOOM    3
#define VTK_INTERACTOR_STYLE_CAMERA_SPIN    4
#define VTK_INTERACTOR_STYLE_CAMERA_FIT        5
#define VTK_INTERACTOR_STYLE_CAMERA_SELECT     6
#define VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN 7

class SVTK_EXPORT SVTK_InteractorStyle : 
  public QObject,
  public VTK::TValidator,
  public vtkInteractorStyle
{
  Q_OBJECT;

 public:
  // Description:
  // This class must be supplied with a vtkRenderWindowInteractor wrapper or
  // parent. This class should not normally be instantiated by application
  // programmers.
  static SVTK_InteractorStyle *New();
  vtkTypeMacro(SVTK_InteractorStyle, vtkInteractorStyle);

  void setViewWindow(SVTK_ViewWindow* theViewWindow);
  void SetRenderWidget(QWidget* theRenderWidget);

  virtual int GetState();

  SVTK_SelectionEvent GetSelectionEvent();

  // redefined in order to add an observer (callback) for custorm event (space mouse event)
  virtual void SetInteractor( vtkRenderWindowInteractor* );

  // redefined in order to cach rendering
  virtual void OnTimer();

  // VTK events
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnMiddleButtonDown();
  virtual void OnMiddleButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();

  // Generic event bindings must be overridden in subclasses
  void OnMouseMove  (int ctrl, int shift, int x, int y);
  void OnLeftButtonDown(int ctrl, int shift, int x, int y);
  void OnLeftButtonUp  (int ctrl, int shift, int x, int y);
  void OnMiddleButtonDown(int ctrl, int shift, int x, int y);
  void OnMiddleButtonUp  (int ctrl, int shift, int x, int y);
  void OnRightButtonDown(int ctrl, int shift, int x, int y);
  void OnRightButtonUp  (int ctrl, int shift, int x, int y);

  void 
  SetFilter( const Handle(VTKViewer_Filter)& );

  Handle(VTKViewer_Filter) 
  GetFilter( const int );  

  bool
  IsFilterPresent( const int );

  void
  RemoveFilter( const int );

  bool
  IsValid( SALOME_Actor* theActor,
	   const int theId,
	   const bool theIsNode = false );
  
  void
  IncrementalPan( const int incrX, const int incrY );

  void
  IncrementalZoom( const int incr );

  void
  IncrementalRotate( const int incrX, const int incrY );

 protected:
  SVTK_InteractorStyle();
  ~SVTK_InteractorStyle();
  SVTK_InteractorStyle(const SVTK_InteractorStyle&) {};
  void operator=(const SVTK_InteractorStyle&) {};

  SVTK_Selector* GetSelector();

  void RotateXY(int dx, int dy);
  void PanXY(int x, int y, int oldX, int oldY);
  void DollyXY(int dx, int dy);
  void SpinXY(int dx, int dy, int oldX, int oldY);
  void fitRect(const int left, const int top, const int right, const int bottom);
  void Place(const int theX, const int theY);
  void TranslateView(int toX, int toY, int fromX, int fromY);

  // custom event handling function (to handle 3d space mouse events)
  static void ProcessEvents( vtkObject* object, unsigned long event,
			     void* clientData, void* callData );

  float MotionFactor;
  float RadianToDegree;                 // constant: for conv from deg to rad
  double myScale;

 public:
  bool eventFilter(QObject* object, QEvent* event);
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
  void onFinishOperation();
  void onOperation(QPoint mousePos);
  void onCursorMove(QPoint mousePos);
  void setCursor(const int operation);

  void onSpaceMouseMove( double* data );
  void onSpaceMouseButton( int button );

  void DecreaseSpeedIncrement();
  void IncreaseSpeedIncrement();
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
  
  SVTK_ViewWindow*          myViewWindow;
  QWidget*                  myRenderWidget;

  vtkSmartPointer<vtkPicker> myPicker;
  vtkSmartPointer<VTKViewer_RectPicker> myRectPicker;
  
  std::map<int, Handle(VTKViewer_Filter)> myFilters;

  //  members from old version
  double                    DeltaElevation;
  double                    DeltaAzimuth;
};

#endif
