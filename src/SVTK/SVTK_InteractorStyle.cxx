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
//  File   : SVTK_InteractorStyle.cxx
//  Author : Christophe ATTANASIO
//  Module : SALOME
//  $Header$


#include "SVTK_InteractorStyle.h"

#include "VTKViewer_Utilities.h"
#include "VTKViewer_RectPicker.h"

#include "SVTK_RenderWindow.h"
#include "SVTK_ViewWindow.h"
#include "SVTK_Selection.h"
#include "SVTK_Event.h" 
#include "SVTK_Selector.h"
#include "SVTK_Functor.h"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"

#include "SALOME_Actor.h"

#include "SALOME_ListIteratorOfListIO.hxx"
#include "SALOME_ListIO.hxx"

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkCommand.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkRendererCollection.h>

#include <qapplication.h>
//VRV: porting on Qt 3.0.5
#if QT_VERSION >= 0x030005
#include <qpainter.h>
#endif
//VRV: porting on Qt 3.0.5
#include <algorithm>

// undefining min and max because CASCADE's Xw_Extensions.hxx defines them and
// it clashes with std::min(), std::max() included in utilities.h
#undef min
#undef max
#include "utilities.h"

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

namespace
{
  bool CheckDimensionId(Selection_Mode theMode, SALOME_Actor *theActor, vtkIdType theObjId){
    switch(theMode){
    case CellSelection:
      return true;
    case EdgeSelection:
      return ( theActor->GetObjDimension( theObjId ) == 1 );
    case FaceSelection:
      return ( theActor->GetObjDimension( theObjId ) == 2 );
    case VolumeSelection:
      return ( theActor->GetObjDimension( theObjId ) == 3 );
    };
    return false;
  }
}  
  
//----------------------------------------------------------------------------
vtkStandardNewMacro(SVTK_InteractorStyle);
//----------------------------------------------------------------------------

SVTK_InteractorStyle
::SVTK_InteractorStyle() 
{
  this->MotionFactor = 10.0;
  this->State = VTK_INTERACTOR_STYLE_CAMERA_NONE;
  this->RadianToDegree = 180.0 / vtkMath::Pi();
  this->ForcedState = VTK_INTERACTOR_STYLE_CAMERA_NONE;
  loadCursors();

  // set custom event handling function (to handle 3d space mouse events)
  EventCallbackCommand->SetCallback( SVTK_InteractorStyle::ProcessEvents );

  myPicker = vtkPicker::New();
  myPicker->Delete();

  myRectPicker = VTKViewer_RectPicker::New();
  myRectPicker->Delete();

  // set default values of properties.  user may edit them in preferences.
  mySpeedIncrement = 10;
  mySpaceMouseBtns[0] = 1;
  mySpaceMouseBtns[1] = 2;
  mySpaceMouseBtns[2] = 10;
  mySpaceMouseBtns[3] = 11;
  mySpaceMouseBtns[4] = 9;
}

//----------------------------------------------------------------------------
SVTK_InteractorStyle
::~SVTK_InteractorStyle() 
{
  if(MYDEBUG) INFOS("SVTK_InteractorStyle::~SVTK_InteractorStyle()");
}

//----------------------------------------------------------------------------
SVTK_Selector*
SVTK_InteractorStyle
::GetSelector() 
{
  return mySelector.GetPointer();
}

//----------------------------------------------------------------------------
int
SVTK_InteractorStyle
::GetState()
{
  return State | ForcedState;
}

//----------------------------------------------------------------------------
SVTK_SelectionEvent
SVTK_InteractorStyle
::GetSelectionEvent()
{
  SVTK_SelectionEvent aSelectionEvent;

  int x, y, w, h;
  Interactor->GetEventPosition( x, y );
  Interactor->GetSize( w, h );

  aSelectionEvent.myX = x;
  aSelectionEvent.myY = h - y - 1;
  aSelectionEvent.myIsCtrl = Interactor->GetControlKey();
  aSelectionEvent.myIsShift = Interactor->GetShiftKey();
  aSelectionEvent.mySelectionMode = GetSelector()->SelectionMode();

  return aSelectionEvent;
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::SetSelector( SVTK_Selector* theSelector ) 
{ 
  mySelector = theSelector; 
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::SetRenderWidget(QWidget* theWidget)
{
  myRenderWidget = theWidget;
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::RotateXY(int dx, int dy)
{
  double rxf;
  double ryf;
  vtkCamera *cam;
  
  if (GetCurrentRenderer() == NULL)
    return;
  
  int *size = GetCurrentRenderer()->GetRenderWindow()->GetSize();
  this->DeltaElevation = -20.0 / size[1];
  this->DeltaAzimuth = -20.0 / size[0];
  
  rxf = (double)dx * this->DeltaAzimuth *  this->MotionFactor;
  ryf = (double)dy * this->DeltaElevation * this->MotionFactor;
  
  cam = GetCurrentRenderer()->GetActiveCamera();
  cam->Azimuth(rxf);
  cam->Elevation(ryf);
  cam->OrthogonalizeViewUp();

  GetCurrentRenderer()->ResetCameraClippingRange(); 

  this->Render();
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::PanXY(int x, int y, int oldX, int oldY)
{
  TranslateView(x, y, oldX, oldY);   
  this->Render();
}


//----------------------------------------------------------------------------
void 
SVTK_InteractorStyle
::DollyXY(int dx, int dy)
{
  if (GetCurrentRenderer() == NULL) 
    return;

  double dxf = this->MotionFactor * (double)(dx) / (double)(GetCurrentRenderer()->GetCenter()[1]);
  double dyf = this->MotionFactor * (double)(dy) / (double)(GetCurrentRenderer()->GetCenter()[1]);

  double zoomFactor = pow((double)1.1, dxf + dyf);
  
  vtkCamera *aCam = GetCurrentRenderer()->GetActiveCamera();
  if (aCam->GetParallelProjection())
    aCam->SetParallelScale(aCam->GetParallelScale()/zoomFactor);
  else{
    aCam->Dolly(zoomFactor);
    GetCurrentRenderer()->ResetCameraClippingRange(); 
  }

  this->Render();
}

//----------------------------------------------------------------------------
void 
SVTK_InteractorStyle
::SpinXY(int x, int y, int oldX, int oldY)
{
  vtkCamera *cam;

  if (GetCurrentRenderer() == NULL)
    return;

  double newAngle = atan2((double)(y - GetCurrentRenderer()->GetCenter()[1]),
			  (double)(x - GetCurrentRenderer()->GetCenter()[0]));
  double oldAngle = atan2((double)(oldY -GetCurrentRenderer()->GetCenter()[1]),
			  (double)(oldX - GetCurrentRenderer()->GetCenter()[0]));
  
  newAngle *= this->RadianToDegree;
  oldAngle *= this->RadianToDegree;

  cam = GetCurrentRenderer()->GetActiveCamera();
  cam->Roll(newAngle - oldAngle);
  cam->OrthogonalizeViewUp();
      
  this->Render();
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnMouseMove() 
{
  int x, y;
  this->Interactor->GetEventPosition( x, y );
  this->OnMouseMove( this->Interactor->GetControlKey(),
		     this->Interactor->GetShiftKey(),
		     x, y );
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnLeftButtonDown()
{
  int x, y;
  this->Interactor->GetEventPosition( x, y );
  this->OnLeftButtonDown( this->Interactor->GetControlKey(),
			  this->Interactor->GetShiftKey(),
			  x, y );
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnLeftButtonUp()
{
  int x, y;
  this->Interactor->GetEventPosition( x, y );
  this->OnLeftButtonUp( this->Interactor->GetControlKey(),
			this->Interactor->GetShiftKey(),
			x, y );
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnMiddleButtonDown() 
{
  int x, y;
  this->Interactor->GetEventPosition( x, y );
  this->OnMiddleButtonDown( this->Interactor->GetControlKey(),
			    this->Interactor->GetShiftKey(),
			    x, y );
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnMiddleButtonUp()
{
  int x, y;
  this->Interactor->GetEventPosition( x, y );
  this->OnMiddleButtonUp( this->Interactor->GetControlKey(),
			  this->Interactor->GetShiftKey(),
			  x, y );
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnRightButtonDown() 
{
  int x, y;
  this->Interactor->GetEventPosition( x, y );
  this->OnRightButtonDown( this->Interactor->GetControlKey(),
			   this->Interactor->GetShiftKey(),
			   x, y );
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnRightButtonUp()
{
  int x, y;
  this->Interactor->GetEventPosition( x, y );
  this->OnRightButtonUp( this->Interactor->GetControlKey(),
			 this->Interactor->GetShiftKey(),
			 x, y );
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnMouseMove(int vtkNotUsed(ctrl), 
	      int shift,
	      int x, int y) 
{
  myShiftState = shift;
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
    onOperation(QPoint(x, y));
  else if (ForcedState == VTK_INTERACTOR_STYLE_CAMERA_NONE)
    onCursorMove(QPoint(x, y));
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnLeftButtonDown(int ctrl, int shift, 
		   int x, int y) 
{
  if (this->HasObserver(vtkCommand::LeftButtonPressEvent)) {
    this->InvokeEvent(vtkCommand::LeftButtonPressEvent,NULL);
    return;
  }

  this->FindPokedRenderer(x, y);
  if (GetCurrentRenderer() == NULL)
    return;

  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  myOtherPoint = myPoint = QPoint(x, y);
  if (ForcedState != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    startOperation(ForcedState);
  } else {
    if (ctrl)
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_ZOOM);
    else
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_SELECT);
  }
  return;
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnLeftButtonUp(int vtkNotUsed(ctrl),
		 int shift, 
		 int vtkNotUsed(x),
		 int vtkNotUsed(y))
{
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnMiddleButtonDown(int ctrl,
		     int shift, 
		     int x, int y) 
{
  if (this->HasObserver(vtkCommand::MiddleButtonPressEvent)) 
    {
      this->InvokeEvent(vtkCommand::MiddleButtonPressEvent,NULL);
      return;
    }

  this->FindPokedRenderer(x, y);
  if (GetCurrentRenderer() == NULL)
    return;

  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  myOtherPoint = myPoint = QPoint(x, y);
  if (ForcedState != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    startOperation(ForcedState);
  }
  else {
    if (ctrl)
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_PAN);
  }
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnMiddleButtonUp(int vtkNotUsed(ctrl),
		   int shift, 
		   int vtkNotUsed(x),
		   int vtkNotUsed(y))
{
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnRightButtonDown(int ctrl,
		    int shift, 
		    int x, int y) 
{
  if (this->HasObserver(vtkCommand::RightButtonPressEvent)) 
    {
      this->InvokeEvent(vtkCommand::RightButtonPressEvent,NULL);
      return;
    }

  this->FindPokedRenderer(x, y);
  if (GetCurrentRenderer() == NULL)
    return;

  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  myOtherPoint = myPoint = QPoint(x, y);
  if (ForcedState != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    startOperation(ForcedState);
  }
  else {
    if (ctrl)
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_ROTATE);  
  }
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnRightButtonUp(int vtkNotUsed(ctrl),
		  int shift, 
		  int vtkNotUsed(x),
		  int vtkNotUsed(y))
{
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
}

//----------------------------------------------------------------------------
/* XPM */
const char* imageZoomCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
".#######........................",
"..aaaaaaa.......................",
"................................",
".............#####..............",
"...........##.aaaa##............",
"..........#.aa.....a#...........",
".........#.a.........#..........",
".........#a..........#a.........",
"........#.a...........#.........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
".........#...........#.a........",
".........#a..........#a.........",
".........##.........#.a.........",
"........#####.....##.a..........",
".......###aaa#####.aa...........",
"......###aa...aaaaa.......#.....",
".....###aa................#a....",
"....###aa.................#a....",
"...###aa...............#######..",
"....#aa.................aa#aaaa.",
".....a....................#a....",
"..........................#a....",
"...........................a....",
"................................",
"................................",
"................................",
"................................"};

const char* imageRotateCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
"................................",
"................................",
"........#.......................",
".......#.a......................",
"......#######...................",
".......#aaaaa#####..............",
"........#..##.a#aa##........##..",
".........a#.aa..#..a#.....##.aa.",
".........#.a.....#...#..##.aa...",
".........#a.......#..###.aa.....",
"........#.a.......#a..#aa.......",
"........#a.........#..#a........",
"........#a.........#a.#a........",
"........#a.........#a.#a........",
"........#a.........#a.#a........",
".........#.........#a#.a........",
"........##a........#a#a.........",
"......##.a#.......#.#.a.........",
"....##.aa..##.....##.a..........",
"..##.aa.....a#####.aa...........",
"...aa.........aaa#a.............",
"................#.a.............",
"...............#.a..............",
"..............#.a...............",
"...............a................",
"................................",
"................................",
"................................",
"................................",
"................................"};


//----------------------------------------------------------------------------
// loads cursors for viewer operations - zoom, pan, etc...
void
SVTK_InteractorStyle
::loadCursors()
{
  myDefCursor       = QCursor(ArrowCursor);
  myHandCursor      = QCursor(PointingHandCursor);
  myPanCursor       = QCursor(SizeAllCursor);
  myZoomCursor      = QCursor(QPixmap(imageZoomCursor));
  myRotateCursor    = QCursor(QPixmap(imageRotateCursor));
  mySpinCursor      = QCursor(QPixmap(imageRotateCursor)); // temporarly !!!!!!
  myGlobalPanCursor = QCursor(CrossCursor);
  myCursorState     = false;
}


//----------------------------------------------------------------------------
// event filter - controls mouse and keyboard events during viewer operations
bool
SVTK_InteractorStyle
::eventFilter(QObject* object, QEvent* event)
{
  if (!myRenderWidget) return false;
  if ( (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) && object != myRenderWidget)
  {
    qApp->removeEventFilter(this);
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  return QObject::eventFilter(object, event);
}


//----------------------------------------------------------------------------
// starts Zoom operation (e.g. through menu command)
void
SVTK_InteractorStyle
::startZoom()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_ZOOM);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_ZOOM;
  qApp->installEventFilter(this);
}


//----------------------------------------------------------------------------
// starts Pan operation (e.g. through menu command)
void
SVTK_InteractorStyle
::startPan()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_PAN);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_PAN;
  qApp->installEventFilter(this);
}

//----------------------------------------------------------------------------
// starts Rotate operation (e.g. through menu command)
void 
SVTK_InteractorStyle
::startRotate()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_ROTATE);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_ROTATE;
  qApp->installEventFilter(this);
}


//----------------------------------------------------------------------------
// starts Spin operation (e.g. through menu command)
void
SVTK_InteractorStyle
::startSpin()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_SPIN);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_SPIN;
  qApp->installEventFilter(this);
}



//----------------------------------------------------------------------------
// starts Fit Area operation (e.g. through menu command)
void
SVTK_InteractorStyle
::startFitArea()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_FIT);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_FIT;
  qApp->installEventFilter(this);
}


//----------------------------------------------------------------------------
// starts Global Panning operation (e.g. through menu command)
void
SVTK_InteractorStyle
::startGlobalPan()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN;

  // store current zoom scale
  vtkCamera *cam = GetCurrentRenderer()->GetActiveCamera();
  myScale = cam->GetParallelScale();

  GetCurrentRenderer()->ResetCamera();

  this->Render();
  
  qApp->installEventFilter(this);
}


//----------------------------------------------------------------------------
// fits viewer contents to rect
void
SVTK_InteractorStyle
::fitRect(const int left, 
	  const int top, 
	  const int right, 
	  const int bottom)
{
  if (GetCurrentRenderer() == NULL) 
    return;
 
  // move camera
  int x = (left + right)/2;
  int y = (top + bottom)/2;
  int *aSize = GetCurrentRenderer()->GetRenderWindow()->GetSize();
  int oldX = aSize[0]/2;
  int oldY = aSize[1]/2;
  TranslateView(oldX, oldY, x, y);

  // zoom camera
  double dxf = (double)(aSize[0]) / (double)(abs(right - left));
  double dyf = (double)(aSize[1]) / (double)(abs(bottom - top));
  double zoomFactor = (dxf + dyf)/2 ;

  vtkCamera *aCam = GetCurrentRenderer()->GetActiveCamera();
  if(aCam->GetParallelProjection())
    aCam->SetParallelScale(aCam->GetParallelScale()/zoomFactor);
  else{
    aCam->Dolly(zoomFactor);
    GetCurrentRenderer()->ResetCameraClippingRange();
  }
  
  this->Render();
}


//----------------------------------------------------------------------------
// starts viewer operation (!internal usage!)
void
SVTK_InteractorStyle
::startOperation(int operation)
{
  switch(operation)
  { 
  case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN:
  case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
  case VTK_INTERACTOR_STYLE_CAMERA_PAN:
  case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
  case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
  case VTK_INTERACTOR_STYLE_CAMERA_FIT:
  case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
    State = operation;
    if (State != VTK_INTERACTOR_STYLE_CAMERA_SELECT)
      setCursor(operation);
    onStartOperation();
    break;
  case VTK_INTERACTOR_STYLE_CAMERA_NONE:
  default:
    setCursor(VTK_INTERACTOR_STYLE_CAMERA_NONE);
    State = ForcedState = VTK_INTERACTOR_STYLE_CAMERA_NONE;
    break;
  }
}


//----------------------------------------------------------------------------
// sets proper cursor for window when viewer operation is activated
void
SVTK_InteractorStyle
::setCursor(const int operation)
{
  if (!myRenderWidget) return;
  switch (operation)
  {
    case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
      myRenderWidget->setCursor(myZoomCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_PAN:
      myRenderWidget->setCursor(myPanCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
      myRenderWidget->setCursor(myRotateCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
      myRenderWidget->setCursor(mySpinCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN:
      myRenderWidget->setCursor(myGlobalPanCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
      myRenderWidget->setCursor(myHandCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_NONE:
    default:
      myRenderWidget->setCursor(myDefCursor); 
      myCursorState = false;
      break;
  }
}


//----------------------------------------------------------------------------
// called when viewer operation started (!put necessary initialization here!)
void
SVTK_InteractorStyle
::onStartOperation()
{
  if (!myRenderWidget) return;
  // VSV: LOD actor activisation
  //  this->Interactor->GetRenderWindow()->SetDesiredUpdateRate(this->Interactor->GetDesiredUpdateRate());
  switch (State) {
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(myRenderWidget);
      p.setPen(Qt::lightGray);
      p.setRasterOp(Qt::XorROP);
      p.drawRect(QRect(myPoint, myOtherPoint));
      break;
    }
    case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
    case VTK_INTERACTOR_STYLE_CAMERA_PAN:
    case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
    case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN:
    case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
      break;
  }
}


//----------------------------------------------------------------------------
// called when viewer operation finished (!put necessary post-processing here!)
void
SVTK_InteractorStyle
::onFinishOperation() 
{
  if (!myRenderWidget) 
    return;

  // VSV: LOD actor activisation
  //  rwi->GetRenderWindow()->SetDesiredUpdateRate(rwi->GetStillUpdateRate());

  Selection_Mode aSelectionMode = GetSelector()->SelectionMode();

  switch (State) {
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(myRenderWidget);
      p.setPen(Qt::lightGray);
      p.setRasterOp(Qt::XorROP);
      QRect rect(myPoint, myOtherPoint);
      p.drawRect(rect);
      rect = rect.normalize();
      if (State == VTK_INTERACTOR_STYLE_CAMERA_FIT) {
        // making fit rect opeation 
        int w, h;
        Interactor->GetSize(w, h);
        int x1, y1, x2, y2;
        x1 = rect.left(); 
        y1 = h - rect.top() - 1;
        x2 = rect.right(); 
        y2 = h - rect.bottom() - 1;
        fitRect(x1, y1, x2, y2);
      }
      else {
	VTK::ForEach<SALOME_Actor>(GetCurrentRenderer()->GetActors(),
				   VTK::THighlight<SALOME_Actor>(false));
        if (myPoint == myOtherPoint) {
	  // process point selection
          int w, h, x, y;
          Interactor->GetSize(w, h);
          x = myPoint.x(); 
          y = h - myPoint.y() - 1;

          this->FindPokedRenderer(x, y);
	  Interactor->StartPickCallback();

          myPicker->Pick(x, y, 0.0, GetCurrentRenderer());
	  if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(myPicker->GetActor())){
	    SVTK_SelectionEvent aSelectionEvent = GetSelectionEvent();
	    aSelectionEvent.mySelectionMode = aSelectionMode;
	    aSelectionEvent.myIsRectangle = false;
	    aSActor->Highlight( this, GetSelector(), GetCurrentRenderer(), aSelectionEvent, true );
	  }else{
	    GetSelector()->ClearIObjects();
	  }
        } else {
          //processing rectangle selection
	  Interactor->StartPickCallback();
	  GetSelector()->StartPickCallback();

	  if (!myShiftState) {
	    this->PropPicked = 0;
	    this->HighlightProp( NULL );
	    GetSelector()->ClearIObjects();
	  }

	  // Compute bounds
	  //	  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
	  QRect rect(myPoint, myOtherPoint);
	  rect = rect.normalize();
	  int w, h;
	  Interactor->GetSize(w, h);
	  int x1, y1, x2, y2;
	  x1 = rect.left(); 
	  y1 = h - rect.top() - 1;
	  x2 = rect.right(); 
	  y2 = h - rect.bottom() - 1;

	  myRectPicker->SetTolerance(0.001);
	  myRectPicker->Pick(x1, y1, 0.0, x2, y2, 0.0, GetCurrentRenderer());

	  SVTK_SelectionEvent aSelectionEvent = GetSelectionEvent();
	  aSelectionEvent.mySelectionMode = aSelectionMode;
	  aSelectionEvent.myIsRectangle = true;
	  aSelectionEvent.myLastX = x1;
	  aSelectionEvent.myLastY = y1;
	  
	  vtkActorCollection* aListActors = myRectPicker->GetActors();
	  aListActors->InitTraversal();
	  while(vtkActor* aActor = aListActors->GetNextActor()){
	    if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(aActor)){
	      aSActor->Highlight( this, GetSelector(), GetCurrentRenderer(), aSelectionEvent, true );
	    }
	  }
	}
	Interactor->EndPickCallback();
	GetSelector()->EndPickCallback();
      } 
    } 
    break;
  case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
  case VTK_INTERACTOR_STYLE_CAMERA_PAN:
  case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
  case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
    break;
  case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN: 
    {
      int w, h, x, y;
      Interactor->GetSize(w, h);
      x = myPoint.x(); 
      y = h - myPoint.y() - 1;
      Place(x, y);
    }
    break;
  }

  this->Render();
}


// called during viewer operation when user moves mouse (!put necessary processing here!)
//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::onOperation(QPoint mousePos) 
{
  if (!myRenderWidget) 
    return;

  switch (State) {
  case VTK_INTERACTOR_STYLE_CAMERA_PAN: 
    {
      this->PanXY(mousePos.x(), myPoint.y(), myPoint.x(), mousePos.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_ZOOM: 
    {    
      this->DollyXY(mousePos.x() - myPoint.x(), mousePos.y() - myPoint.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_ROTATE: 
    {
      this->RotateXY(mousePos.x() - myPoint.x(), myPoint.y() - mousePos.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_SPIN: 
    {
      this->SpinXY(mousePos.x(), mousePos.y(), myPoint.x(), myPoint.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN: 
    {    
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    {
      if (!myCursorState)
        setCursor(VTK_INTERACTOR_STYLE_CAMERA_SELECT);
    }
  case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(myRenderWidget);
      p.setPen(Qt::lightGray);
      p.setRasterOp(Qt::XorROP);
      p.drawRect(QRect(myPoint, myOtherPoint));
      myOtherPoint = mousePos;
      p.drawRect(QRect(myPoint, myOtherPoint));
      break;
    }
  }
}

// called when user moves mouse inside viewer window and there is no active viewer operation 
// (!put necessary processing here!)
//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::onCursorMove(QPoint mousePos) 
{
  // processing highlighting
  int w, h, x, y;
  Interactor->GetSize(w, h);
  x = mousePos.x(); y = h - mousePos.y() - 1;

  this->FindPokedRenderer(x,y);

  SVTK_SelectionEvent aSelectionEvent = GetSelectionEvent();
  aSelectionEvent.myX = x;
  aSelectionEvent.myY = y;

  bool anIsChanged = false;
  SALOME_Actor* aLastActor = SALOME_Actor::SafeDownCast(myPicker->GetActor());

  myPicker->Pick(x, y, 0.0, GetCurrentRenderer());
  if(SALOME_Actor* anActor = SALOME_Actor::SafeDownCast(myPicker->GetActor())){
    anIsChanged |= anActor->PreHighlight( this, GetSelector(), GetCurrentRenderer(), aSelectionEvent, true );
    if(aLastActor && aLastActor != anActor)
      aLastActor->PreHighlight( this, GetSelector(), GetCurrentRenderer(), aSelectionEvent, false );
  }
  
  if(anIsChanged)
    this->Render();
}

// called on finsh GlobalPan operation 
//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::Place(const int theX, const int theY) 
{
  if (GetCurrentRenderer() == NULL)
    return;

  //translate view
  int *aSize = GetCurrentRenderer()->GetRenderWindow()->GetSize();
  int centerX = aSize[0]/2;
  int centerY = aSize[1]/2;

  TranslateView(centerX, centerY, theX, theY);

  // restore zoom scale
  vtkCamera *cam = GetCurrentRenderer()->GetActiveCamera();
  cam->SetParallelScale(myScale);
  GetCurrentRenderer()->ResetCameraClippingRange();

  this->Render();
}



// Translates view from Point to Point
//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::TranslateView(int toX, int toY, int fromX, int fromY)
{
  vtkCamera *cam = GetCurrentRenderer()->GetActiveCamera();
  double viewFocus[4], focalDepth, viewPoint[3];
  float newPickPoint[4], oldPickPoint[4], motionVector[3];
  cam->GetFocalPoint(viewFocus);

  this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1],
			      viewFocus[2], viewFocus);
  focalDepth = viewFocus[2];

  this->ComputeDisplayToWorld(double(toX), double(toY),
			      focalDepth, newPickPoint);
  this->ComputeDisplayToWorld(double(fromX),double(fromY),
			      focalDepth, oldPickPoint);
  
  // camera motion is reversed
  motionVector[0] = oldPickPoint[0] - newPickPoint[0];
  motionVector[1] = oldPickPoint[1] - newPickPoint[1];
  motionVector[2] = oldPickPoint[2] - newPickPoint[2];
  
  cam->GetFocalPoint(viewFocus);
  cam->GetPosition(viewPoint);
  cam->SetFocalPoint(motionVector[0] + viewFocus[0],
		     motionVector[1] + viewFocus[1],
		     motionVector[2] + viewFocus[2]);
  cam->SetPosition(motionVector[0] + viewPoint[0],
		   motionVector[1] + viewPoint[1],
		   motionVector[2] + viewPoint[2]);
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::SetFilter( const Handle(VTKViewer_Filter)& theFilter )
{
  myFilters[ theFilter->GetId() ] = theFilter;
}

//----------------------------------------------------------------------------
bool
SVTK_InteractorStyle
::IsFilterPresent( const int theId )
{
  return myFilters.find( theId ) != myFilters.end();
}

//----------------------------------------------------------------------------
void  
SVTK_InteractorStyle
::RemoveFilter( const int theId )
{
  if ( IsFilterPresent( theId ) )
    myFilters.erase( theId );
}

//----------------------------------------------------------------------------
bool
SVTK_InteractorStyle
::IsValid( SALOME_Actor* theActor,
	   const int     theId,
	   const bool    theIsNode )
{
  std::map<int, Handle(VTKViewer_Filter)>::const_iterator anIter;
  for ( anIter = myFilters.begin(); anIter != myFilters.end(); ++anIter )
  {
    const Handle(VTKViewer_Filter)& aFilter = anIter->second;
    if ( theIsNode == aFilter->IsNodeFilter() &&
         !aFilter->IsValid( theActor, theId ) )
      return false;
  }
  return true;
}

//----------------------------------------------------------------------------
Handle(VTKViewer_Filter) 
SVTK_InteractorStyle
::GetFilter( const int theId )
{
  return IsFilterPresent( theId ) ? myFilters[ theId ] : Handle(VTKViewer_Filter)();
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::IncrementalPan( const int incrX, const int incrY )
{
  this->PanXY( incrX, incrY, 0, 0 );
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::IncrementalZoom( const int incr )
{
  this->DollyXY( incr, incr );
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::IncrementalRotate( const int incrX, const int incrY )
{
  this->RotateXY( incrX, -incrY );
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::SetInteractor( vtkRenderWindowInteractor* interactor )
{
  // register EventCallbackCommand as observer of standard events (keypress, mousemove, etc)
  vtkInteractorStyle::SetInteractor( interactor );
 
  if ( interactor ) { 
    // initialize renderer attribute
    FindPokedRenderer( 0, 0 );

    // register EventCallbackCommand as observer of custorm event (3d space mouse event)
    interactor->AddObserver( SpaceMouseMoveEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( SpaceMouseButtonEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( PanLeftEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( PanRightEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( PanUpEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( PanDownEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( ZoomInEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( ZoomOutEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( RotateLeftEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( RotateRightEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( RotateUpEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( RotateDownEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( PlusSpeedIncrementEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( MinusSpeedIncrementEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( SetSpeedIncrementEvent, EventCallbackCommand, Priority );
    interactor->AddObserver( SetSpaceMouseF1Event, EventCallbackCommand, Priority );
    interactor->AddObserver( SetSpaceMouseF2Event, EventCallbackCommand, Priority );
    interactor->AddObserver( SetSpaceMouseF3Event, EventCallbackCommand, Priority );
    interactor->AddObserver( SetSpaceMouseF4Event, EventCallbackCommand, Priority );
    interactor->AddObserver( SetSpaceMouseF5Event, EventCallbackCommand, Priority );
  }
}


//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::OnTimer() 
{
  //vtkInteractorStyle::OnTimer();
  this->Interactor->Render();
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::Render() 
{
  this->Interactor->CreateTimer(VTKI_TIMER_FIRST);
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::onSpaceMouseMove( double* data )
{
  //  printf( "x=%+5.0lf y=%+5.0lf z=%+5.0lf a=%+5.0lf b=%+5.0lf c=%+5.0lf\n",
  //	  data[0], data[1], data[2], data[3], data[4], data[5] );
  
  // general things, do SetCurrentRenderer() within FindPokedRenderer() 
  int x, y;
  Interactor->GetEventPosition( x, y ); // current mouse position (from last mouse move event or any other event)
  FindPokedRenderer( x, y ); // calls SetCurrentRenderer
  
  IncrementalZoom( (int)data[2] );        // 1. push toward / pull backward = zoom out / zoom in
  IncrementalPan(  (int)data[0],  (int)data[1] );// 2. pull up / push down = pan up / down, 3. move left / right = pan left / right
  IncrementalRotate( 0,  (int)data[4] );   // 4. twist the control = rotate around Y axis
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::onSpaceMouseButton( int button )
{
  if ( mySpaceMouseBtns[0] == button )    --mySpeedIncrement;
  if ( mySpaceMouseBtns[1] == button )    ++mySpeedIncrement;
  if ( mySpaceMouseBtns[2] == button )    DecreaseGaussPointMagnification();
  if ( mySpaceMouseBtns[3] == button )    IncreaseGaussPointMagnification();
  if ( mySpaceMouseBtns[4] == button )    DominantCombinedSwitch();
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::DecreaseGaussPointMagnification()
{
  printf( "\n--DecreaseGaussPointMagnification() NOT IMPLEMENTED--\n" );
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::IncreaseGaussPointMagnification()
{
  printf( "\n--IncreaseGaussPointMagnification() NOT IMPLEMENTED--\n" );
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::DominantCombinedSwitch()
{
  printf( "\n--DominantCombinedSwitch() NOT IMPLEMENTED--\n" );
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::ProcessEvents( vtkObject* object,
		 unsigned long event,
		 void* clientData, 
		 void* callData )
{
  if ( event < vtkCommand::UserEvent  )
    vtkInteractorStyle::ProcessEvents( object, event, clientData, callData );

  else if ( clientData ) {
    vtkObject* anObject = reinterpret_cast<vtkObject*>( clientData );
    SVTK_InteractorStyle* self = dynamic_cast<SVTK_InteractorStyle*>( anObject );
    if ( self ) {
      switch ( event ) {
      case SpaceMouseMoveEvent : 
	self->onSpaceMouseMove( (double*)callData ); 
	break;
      case SpaceMouseButtonEvent : 
	self->onSpaceMouseButton( *((int*)callData) ); 
	break;
      case PanLeftEvent: 
	self->IncrementalPan( -self->mySpeedIncrement, 0 );
	break;
      case PanRightEvent:
	self->IncrementalPan( self->mySpeedIncrement, 0 );
	break;
      case PanUpEvent:
	self->IncrementalPan( 0, self->mySpeedIncrement );
	break;
      case PanDownEvent:
	self->IncrementalPan( 0, -self->mySpeedIncrement );
	break;
      case ZoomInEvent:
	self->IncrementalZoom( self->mySpeedIncrement );
	break;
      case ZoomOutEvent:
	self->IncrementalZoom( -self->mySpeedIncrement );
	break;
      case RotateLeftEvent: 
	self->IncrementalRotate( -self->mySpeedIncrement, 0 );
	break;
      case RotateRightEvent:
	self->IncrementalRotate( self->mySpeedIncrement, 0 );
	break;
      case RotateUpEvent:
	self->IncrementalRotate( 0, -self->mySpeedIncrement );
	break;
      case RotateDownEvent:
	self->IncrementalRotate( 0, self->mySpeedIncrement );
	break;
      case PlusSpeedIncrementEvent:
	++(self->mySpeedIncrement);
	break;
      case MinusSpeedIncrementEvent:
	--(self->mySpeedIncrement);
	break;
      case SetSpeedIncrementEvent:
	self->mySpeedIncrement = *((int*)callData);
	break;
      case SetSpaceMouseF1Event:
	self->mySpaceMouseBtns[0] = *((int*)callData);
	break;
      case SetSpaceMouseF2Event:
	self->mySpaceMouseBtns[1] = *((int*)callData);
	break;
      case SetSpaceMouseF3Event:
	self->mySpaceMouseBtns[2] = *((int*)callData);
	break;
      case SetSpaceMouseF4Event:
	self->mySpaceMouseBtns[3] = *((int*)callData);
	break;
      case SetSpaceMouseF5Event:
	self->mySpaceMouseBtns[4] = *((int*)callData);
	break;
      }
    }
  }
}
