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
#include "SVTK_SpaceMouseEvent.h" 
#include "SVTK_Selector.h"

#include "SALOME_Actor.h"

#include "SALOME_ListIteratorOfListIO.hxx"
#include "SALOME_ListIO.hxx"

#include "SUIT_Session.h"
#include "SUIT_ResourceMgr.h"

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
  myViewWindow = NULL;
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
  return myViewWindow->GetSelector();
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

  aSelectionEvent.X = x;
  aSelectionEvent.Y = h - y - 1;
  aSelectionEvent.IsCtrl = Interactor->GetControlKey();
  aSelectionEvent.IsShift = Interactor->GetShiftKey();

  return aSelectionEvent;
}

//----------------------------------------------------------------------------
void 
SVTK_InteractorStyle
::setViewWindow(SVTK_ViewWindow* theViewWindow)
{
  myViewWindow = theViewWindow;
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::setGUIWindow(QWidget* theWindow)
{
  myGUIWindow = theWindow;
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::RotateXY(int dx, int dy)
{
  double rxf;
  double ryf;
  vtkCamera *cam;
  
  if (this->CurrentRenderer == NULL)
    {
      return;
    }
  
  int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();
  this->DeltaElevation = -20.0 / size[1];
  this->DeltaAzimuth = -20.0 / size[0];
  
  rxf = (double)dx * this->DeltaAzimuth *  this->MotionFactor;
  ryf = (double)dy * this->DeltaElevation * this->MotionFactor;
  
  cam = this->CurrentRenderer->GetActiveCamera();
  cam->Azimuth(rxf);
  cam->Elevation(ryf);
  cam->OrthogonalizeViewUp();
  ::ResetCameraClippingRange(this->CurrentRenderer); 
  //this->Interactor->Render();
  myGUIWindow->update();
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::PanXY(int x, int y, int oldX, int oldY)
{
  TranslateView(x, y, oldX, oldY);   
  //this->Interactor->Render();
  myGUIWindow->update();
}


//----------------------------------------------------------------------------
void 
SVTK_InteractorStyle
::DollyXY(int dx, int dy)
{
  if (this->CurrentRenderer == NULL) return;

  double dxf = this->MotionFactor * (double)(dx) / (double)(this->CurrentRenderer->GetCenter()[1]);
  double dyf = this->MotionFactor * (double)(dy) / (double)(this->CurrentRenderer->GetCenter()[1]);

  double zoomFactor = pow((double)1.1, dxf + dyf);
  
  vtkCamera *aCam = this->CurrentRenderer->GetActiveCamera();
  if (aCam->GetParallelProjection())
    aCam->SetParallelScale(aCam->GetParallelScale()/zoomFactor);
  else{
    aCam->Dolly(zoomFactor);
    ::ResetCameraClippingRange(this->CurrentRenderer);
  }

  //this->Interactor->Render();
  myGUIWindow->update();
}

//----------------------------------------------------------------------------
void 
SVTK_InteractorStyle
::SpinXY(int x, int y, int oldX, int oldY)
{
  vtkCamera *cam;

  if (this->CurrentRenderer == NULL)
    {
      return;
    }

  double newAngle = atan2((double)(y - this->CurrentRenderer->GetCenter()[1]),
			  (double)(x - this->CurrentRenderer->GetCenter()[0]));
  double oldAngle = atan2((double)(oldY -this->CurrentRenderer->GetCenter()[1]),
			  (double)(oldX - this->CurrentRenderer->GetCenter()[0]));
  
  newAngle *= this->RadianToDegree;
  oldAngle *= this->RadianToDegree;

  cam = this->CurrentRenderer->GetActiveCamera();
  cam->Roll(newAngle - oldAngle);
  cam->OrthogonalizeViewUp();
      
  //this->Interactor->Render();
  myGUIWindow->update();
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

  if( needsRedrawing() )
    emit RenderWindowModified() ; 
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
  if (this->CurrentRenderer == NULL) {
    return;
  }
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
  if (this->CurrentRenderer == NULL)
    {
      return;
    }
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
  if (this->CurrentRenderer == NULL)
    {
      return;
    }
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
  if (!myGUIWindow) return false;
  if ( (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) && object != myGUIWindow)
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
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
  myScale = cam->GetParallelScale();

  if (myViewWindow) myViewWindow->onFitAll();

  if (myGUIWindow) myGUIWindow->update();
  
  qApp->installEventFilter(this);
}


//----------------------------------------------------------------------------
// returns TRUE if needs redrawing
bool
SVTK_InteractorStyle
::needsRedrawing()
{
  return State == VTK_INTERACTOR_STYLE_CAMERA_ZOOM   ||
         State == VTK_INTERACTOR_STYLE_CAMERA_PAN    ||
         State == VTK_INTERACTOR_STYLE_CAMERA_ROTATE ||
         State == VTK_INTERACTOR_STYLE_CAMERA_SPIN   ||
         State == VTK_INTERACTOR_STYLE_CAMERA_NONE;
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
  if (this->CurrentRenderer == NULL) return;
 
  // move camera
  int x = (left + right)/2;
  int y = (top + bottom)/2;
  int *aSize = this->CurrentRenderer->GetRenderWindow()->GetSize();
  int oldX = aSize[0]/2;
  int oldY = aSize[1]/2;
  TranslateView(oldX, oldY, x, y);

  // zoom camera
  double dxf = (double)(aSize[0]) / (double)(abs(right - left));
  double dyf = (double)(aSize[1]) / (double)(abs(bottom - top));
  double zoomFactor = (dxf + dyf)/2 ;

  vtkCamera *aCam = this->CurrentRenderer->GetActiveCamera();
  if(aCam->GetParallelProjection())
    aCam->SetParallelScale(aCam->GetParallelScale()/zoomFactor);
  else{
    aCam->Dolly(zoomFactor);
    ::ResetCameraClippingRange(this->CurrentRenderer);
  }
  
  myGUIWindow->update();
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
  if (!myGUIWindow) return;
  switch (operation)
  {
    case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
      myGUIWindow->setCursor(myZoomCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_PAN:
      myGUIWindow->setCursor(myPanCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
      myGUIWindow->setCursor(myRotateCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
      myGUIWindow->setCursor(mySpinCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN:
      myGUIWindow->setCursor(myGlobalPanCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
      myGUIWindow->setCursor(myHandCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_NONE:
    default:
      myGUIWindow->setCursor(myDefCursor); 
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
  if (!myGUIWindow) return;
  // VSV: LOD actor activisation
  //  this->Interactor->GetRenderWindow()->SetDesiredUpdateRate(this->Interactor->GetDesiredUpdateRate());
  switch (State) {
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(myGUIWindow);
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
  if (!myGUIWindow) 
    return;

  // VSV: LOD actor activisation
  //  rwi->GetRenderWindow()->SetDesiredUpdateRate(rwi->GetStillUpdateRate());

  Selection_Mode aSelectionMode = myViewWindow->SelectionMode();

  switch (State) {
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(myGUIWindow);
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
	myViewWindow->unHighlightAll();
        if (myPoint == myOtherPoint) {
	  // process point selection
          int w, h, x, y;
          Interactor->GetSize(w, h);
          x = myPoint.x(); 
          y = h - myPoint.y() - 1;

          this->FindPokedRenderer(x, y);
	  Interactor->StartPickCallback();

          myPicker->Pick(x, y, 0.0, this->CurrentRenderer);
	  if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(myPicker->GetActor())){
	    SVTK_SelectionEvent aSelectionEvent = GetSelectionEvent();
	    aSelectionEvent.SelectionMode = aSelectionMode;
	    aSelectionEvent.IsRectangle = false;
	    aSActor->Highlight( this, GetSelector(), this->CurrentRenderer, aSelectionEvent, true );
	  }else{
	    GetSelector()->ClearIObjects();
	  }
        } else {
          //processing rectangle selection
	  Interactor->StartPickCallback();

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
	  myRectPicker->Pick(x1, y1, 0.0, x2, y2, 0.0, this->CurrentRenderer);

	  SVTK_SelectionEvent aSelectionEvent = GetSelectionEvent();
	  aSelectionEvent.SelectionMode = aSelectionMode;
	  aSelectionEvent.IsRectangle = true;
	  aSelectionEvent.LastX = x1;
	  aSelectionEvent.LastY = y1;
	  
	  vtkActorCollection* aListActors = myRectPicker->GetActors();
	  aListActors->InitTraversal();
	  while(vtkActor* aActor = aListActors->GetNextActor()){
	    if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(aActor)){
	      aSActor->Highlight( this, GetSelector(), this->CurrentRenderer, aSelectionEvent, true );
	    }
	  }
	}
	Interactor->EndPickCallback();
	myViewWindow->onSelectionChanged();
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
  if (myGUIWindow) myGUIWindow->update();

}


// called during viewer operation when user moves mouse (!put necessary processing here!)
//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::onOperation(QPoint mousePos) 
{
  if (!myGUIWindow) return;
  int w, h;
  GetInteractor()->GetSize(w, h);
  switch (State) {
  case VTK_INTERACTOR_STYLE_CAMERA_PAN: 
    {
      // processing panning
      //this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->PanXY(mousePos.x(), myPoint.y(), myPoint.x(), mousePos.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_ZOOM: 
    {    
      // processing zooming
      //this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->DollyXY(mousePos.x() - myPoint.x(), mousePos.y() - myPoint.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_ROTATE: 
    {
      // processing rotation
      //this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->RotateXY(mousePos.x() - myPoint.x(), myPoint.y() - mousePos.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_SPIN: 
    {
      // processing spinning
      //this->FindPokedCamera(mousePos.x(), mousePos.y());
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
      QPainter p(myGUIWindow);
      p.setPen(Qt::lightGray);
      p.setRasterOp(Qt::XorROP);
      p.drawRect(QRect(myPoint, myOtherPoint));
      myOtherPoint = mousePos;
      p.drawRect(QRect(myPoint, myOtherPoint));
      break;
    }
  }
  this->LastPos[0] = mousePos.x();
  this->LastPos[1] = h - mousePos.y() - 1;
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
  Interactor->StartPickCallback();

  SVTK_SelectionEvent aSelectionEvent = GetSelectionEvent();
  aSelectionEvent.SelectionMode = myViewWindow->SelectionMode();
  aSelectionEvent.X = x;
  aSelectionEvent.Y = y;

  if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(myPicker->GetActor()))
    aSActor->PreHighlight( this, GetSelector(), this->CurrentRenderer, aSelectionEvent, false );

  myPicker->Pick(x, y, 0.0, this->CurrentRenderer);
  if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(myPicker->GetActor()))
    aSActor->PreHighlight( this, GetSelector(), this->CurrentRenderer, aSelectionEvent, true );
  
  Interactor->EndPickCallback();
  //Interactor->Render();
  myGUIWindow->update();
  
  this->LastPos[0] = x;
  this->LastPos[1] = y;
}

// called on finsh GlobalPan operation 
//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::Place(const int theX, const int theY) 
{
  if (this->CurrentRenderer == NULL) {
    return;
  }

  //translate view
  int *aSize = this->CurrentRenderer->GetRenderWindow()->GetSize();
  int centerX = aSize[0]/2;
  int centerY = aSize[1]/2;

  TranslateView(centerX, centerY, theX, theY);

  // restore zoom scale
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
  cam->SetParallelScale(myScale);
  ::ResetCameraClippingRange(this->CurrentRenderer);

  if (myGUIWindow) myGUIWindow->update();

}



// Translates view from Point to Point
//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::TranslateView(int toX, int toY, int fromX, int fromY)
{
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
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

  // register EventCallbackCommand as observer of custorm event (3d space mouse event)
  interactor->AddObserver( SpaceMouseMoveEvent,   EventCallbackCommand, Priority );
  interactor->AddObserver( SpaceMouseButtonEvent, EventCallbackCommand, Priority );
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
  //  printf( "Button pressed [%d]  \n", button );
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  if ( resMgr->integerValue( "VISU", "spacemouse_func1_btn" ) == button )
    DecreaseSpeedIncrement();
  if ( resMgr->integerValue( "VISU", "spacemouse_func2_btn" ) == button )
    IncreaseSpeedIncrement();
  if ( resMgr->integerValue( "VISU", "spacemouse_func3_btn" ) == button )
    DecreaseGaussPointMagnification();
  if ( resMgr->integerValue( "VISU", "spacemouse_func4_btn" ) == button )
    IncreaseGaussPointMagnification();
  if ( resMgr->integerValue( "VISU", "spacemouse_func5_btn" ) == button )
    DominantCombinedSwitch();
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::DecreaseSpeedIncrement()
{
  printf( "\n--DecreaseSpeedIncrement() NOT IMPLEMENTED--\n" );
  
}

//----------------------------------------------------------------------------
void
SVTK_InteractorStyle
::IncreaseSpeedIncrement()
{
  printf( "\n--IncreaseSpeedIncrement() NOT IMPLEMENTED--\n" );
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
  if ( event != SpaceMouseMoveEvent && event != SpaceMouseButtonEvent )
    vtkInteractorStyle::ProcessEvents( object, event, clientData, callData );

  else if ( clientData && callData ) {
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
      }
    }
  }
}

