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
//  File   : VTKViewer_RenderWindowInteractor.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SVTK_RenderWindowInteractor.h"

#include "SVTK_InteractorStyle.h"
#include "SVTK_RenderWindow.h"
#include "SVTK_Selection.h"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"
#include "SALOME_Actor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// VTK Includes
#include <vtkObjectFactory.h>
#include <vtkPicker.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>

// QT Includes
#include <qkeycode.h>

#include <TColStd_IndexedMapOfInteger.hxx>

#include "utilities.h"

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


SVTK_RenderWindowInteractor* 
SVTK_RenderWindowInteractor
::New() 
{
  vtkObject *ret = vtkObjectFactory::CreateInstance("SVTK_RenderWindowInteractor") ;
  if( ret ) {
    return dynamic_cast<SVTK_RenderWindowInteractor *>(ret) ;
  }
  return new SVTK_RenderWindowInteractor;
}

SVTK_RenderWindowInteractor
::SVTK_RenderWindowInteractor() 
{
  this->Enabled = 0 ;
  this->mTimer = new QTimer( this ) ;
  myDisplayMode = 0;

  myBasicPicker = vtkPicker::New();
  myCellPicker = vtkCellPicker::New();
  myPointPicker = vtkPointPicker::New();
  connect(mTimer, SIGNAL(timeout()), this, SLOT(TimerFunc())) ;
}


SVTK_RenderWindowInteractor
::~SVTK_RenderWindowInteractor() 
{
  if(MYDEBUG) INFOS("SVTK_RenderWindowInteractor::~SVTK_RenderWindowInteractor()");

  delete mTimer ;

  myBasicPicker->Delete();
  myCellPicker->Delete();
  myPointPicker->Delete();
}

//
// We never allow the SVTK_RenderWindowInteractor to control 
// the event loop. The application always has the control. 
//
void
SVTK_RenderWindowInteractor
::Initialize() 
{
  //
  // We cannot do much unless there is a render window 
  // associated with this interactor. 
  //
  if( ! RenderWindow ) {
    vtkErrorMacro(<< "SVTK_RenderWindowInteractor::Initialize(): No render window attached!") ;
    return ;
  }

  //
  // We cannot hand a render window which is not a VTKViewer_RenderWindow. 
  // One way to force this is to use dynamic_cast and hope that 
  // it works. If the dynamic_cast does not work, we flag an error
  // and get the hell out.
  //
  vtkRenderWindow *aRenderWindow = dynamic_cast<vtkRenderWindow *>(RenderWindow) ;
  if( !aRenderWindow ) {
    vtkErrorMacro(<< "SVTK_RenderWindowInteractor::Initialize() can only handle VTKViewer_RenderWindow.") ;
    return ;
  }

  //
  // If the render window has zero size, then set it to a default 
  // value of 300x300.
  // 
  int* aSize = aRenderWindow->GetSize();
  this->Size[0] = ((aSize[0] > 0) ? aSize[0] : 300);
  this->Size[1] = ((aSize[1] > 0) ? aSize[1] : 300);

  this->SetPicker(myBasicPicker);
  //this->SetPicker(vtkPicker::New());

  //SetSelectionTolerance();

  //
  // Enable the interactor. 
  //
  this->Enable() ;

  //
  // Start the rendering of the window. 
  //
  aRenderWindow->Start() ;

  //
  // The interactor has been initialized.
  //
  this->Initialized = 1 ;

  return ;
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::SetInteractorStyle(vtkInteractorObserver *theInteractor)
{
  myInteractorStyle = dynamic_cast<SVTK_InteractorStyle*>(theInteractor);
  vtkRenderWindowInteractor::SetInteractorStyle(theInteractor);
}


void
SVTK_RenderWindowInteractor
::SetSelectionMode(Selection_Mode theMode)
{
  switch(theMode){
  case ActorSelection:
    this->SetPicker(myBasicPicker);
    break;
  case NodeSelection:
    this->SetPicker(myPointPicker);
    break;
  case CellSelection:
  case EdgeSelection:
  case FaceSelection:
  case VolumeSelection:
  case EdgeOfCellSelection:
    this->SetPicker(myCellPicker);
    break;
  }

  myInteractorStyle->OnSelectionModeChanged();
}

void
SVTK_RenderWindowInteractor
::SetSelectionProp(const double& theRed, 
		   const double& theGreen, 
		   const double& theBlue, 
		   const int& theWidth) 
{
  /*
  myCellActor->GetProperty()->SetColor(theRed, theGreen, theBlue);
  myCellActor->GetProperty()->SetLineWidth(theWidth);

  myPointActor->GetProperty()->SetColor(theRed, theGreen, theBlue);
  myPointActor->GetProperty()->SetPointSize(theWidth);
  */
}

void
SVTK_RenderWindowInteractor
::SetSelectionTolerance(const double& theTolNodes, 
			const double& theTolItems)
{
  myTolNodes = theTolNodes;
  myTolItems = theTolItems;

  myBasicPicker->SetTolerance(myTolItems);
  myCellPicker->SetTolerance(myTolItems);
  myPointPicker->SetTolerance(myTolNodes);

}

// ================================== 
void
SVTK_RenderWindowInteractor
::Start() 
{
  //
  // We do not allow this interactor to control the 
  // event loop. Only the QtApplication objects are
  // allowed to do that. 
  //
  vtkErrorMacro(<<"SVTK_RenderWindowInteractor::Start() not allowed to start event loop.") ;
  return ;
}

void
SVTK_RenderWindowInteractor
::UpdateSize(int w, int h) 
{
  // if the size changed send this on to the RenderWindow
  if ((w != this->Size[0])||(h != this->Size[1])) {
    this->Size[0] = w;
    this->Size[1] = h;
    this->RenderWindow->SetSize(w,h);
  }
}

int
SVTK_RenderWindowInteractor
::CreateTimer(int vtkNotUsed(timertype)) 
{
  //
  // Start a one-shot timer for 10ms. 
  //
  mTimer->start(10, TRUE) ;
  return 1 ;
}

int
SVTK_RenderWindowInteractor
::DestroyTimer(void) 
{
  //
  // :TRICKY: Tue May  2 00:17:32 2000 Pagey
  //
  // QTimer will automatically expire after 10ms. So 
  // we do not need to do anything here. In fact, we 
  // should not even Stop() the QTimer here because doing 
  // this will skip some of the processing that the TimerFunc()
  // does and will result in undesirable effects. For 
  // example, this will result in vtkLODActor to leave
  // the models in low-res mode after the mouse stops
  // moving. 
  //
  return 1 ;
}

void
SVTK_RenderWindowInteractor
::TimerFunc() 
{
  if( ! this->Enabled ) {
    return ;
  }

  myInteractorStyle->OnTimer();

  emit RenderWindowModified();
}

void
SVTK_RenderWindowInteractor
::MouseMove(const QMouseEvent *event) 
{
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnMouseMove(0, 0, event->x(), event->y()/*this->Size[1] - event->y() - 1*/) ;
  if (myInteractorStyle->needsRedrawing() )
    emit RenderWindowModified() ; 
}

void
SVTK_RenderWindowInteractor
::LeftButtonPressed(const QMouseEvent *event) 
{
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnLeftButtonDown((event->state() & ControlButton), 
				      (event->state() & ShiftButton), 
				      event->x(), event->y());
}

void
SVTK_RenderWindowInteractor
::LeftButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  this->SetEventInformationFlipY( event->x(), event->y(),
				  ( event->state() & ControlButton ),
				  ( event->state() & ShiftButton ) );

  myInteractorStyle->OnLeftButtonUp( (event->state() & ControlButton), 
				     (event->state() & ShiftButton), 
				     event->x(), event->y() ) ;
}

void 
SVTK_RenderWindowInteractor
::MiddleButtonPressed(const QMouseEvent *event) 
{
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnMiddleButtonDown((event->state() & ControlButton), 
					(event->state() & ShiftButton), 
					event->x(), event->y() ) ;
}

void
SVTK_RenderWindowInteractor
::MiddleButtonReleased(const QMouseEvent *event) 
{
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnMiddleButtonUp( (event->state() & ControlButton), 
				       (event->state() & ShiftButton), 
				       event->x(), event->y() ) ;
}

void
SVTK_RenderWindowInteractor
::RightButtonPressed(const QMouseEvent *event) 
{
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnRightButtonDown( (event->state() & ControlButton), 
					(event->state() & ShiftButton), 
					event->x(), event->y() ) ;
}

void
SVTK_RenderWindowInteractor
::RightButtonReleased(const QMouseEvent *event) 
{
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnRightButtonUp( (event->state() & ControlButton), 
				      (event->state() & ShiftButton), 
				      event->x(), event->y() ) ;

  if(myInteractorStyle->GetState() == VTK_INTERACTOR_STYLE_CAMERA_NONE && !( event->state() & KeyButtonMask )){
    QContextMenuEvent aEvent( QContextMenuEvent::Mouse,
                              event->pos(), event->globalPos(),
                              event->state() );
    emit contextMenuRequested( &aEvent );
  }
}

void
SVTK_RenderWindowInteractor
::ButtonPressed(const QMouseEvent *event) 
{
  return ;
}

void
SVTK_RenderWindowInteractor
::ButtonReleased(const QMouseEvent *event) 
{
  return ;
}


int
SVTK_RenderWindowInteractor
::GetDisplayMode() 
{
  return myDisplayMode;
}

void
SVTK_RenderWindowInteractor
::SetDisplayMode(int theMode)
{
  if(theMode == 0) 
    ChangeRepresentationToWireframe();
  else 
    ChangeRepresentationToSurface();
  myDisplayMode = theMode;
}


void
SVTK_RenderWindowInteractor
::SetDisplayMode(const Handle(SALOME_InteractiveObject)& theIObject, 
		 int theMode)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,int>
			  (&SALOME_Actor::setDisplayMode,theMode));
}


void
SVTK_RenderWindowInteractor
::ChangeRepresentationToWireframe()
{
  ChangeRepresentationToWireframe(GetRenderer()->GetActors());
}

void
SVTK_RenderWindowInteractor
::ChangeRepresentationToSurface()
{
  ChangeRepresentationToSurface(GetRenderer()->GetActors());
}


void
SVTK_RenderWindowInteractor
::ChangeRepresentationToWireframe(vtkActorCollection* theCollection)
{
  using namespace VTK;
  ForEach<SALOME_Actor>(theCollection,
			TSetFunction<SALOME_Actor,int>
			(&SALOME_Actor::setDisplayMode,0));
  emit RenderWindowModified();
}

void
SVTK_RenderWindowInteractor
::ChangeRepresentationToSurface(vtkActorCollection* theCollection)
{
  using namespace VTK;
  ForEach<SALOME_Actor>(theCollection,
			TSetFunction<SALOME_Actor,int>
			(&SALOME_Actor::setDisplayMode,1));
  emit RenderWindowModified();
}


vtkRenderer* 
SVTK_RenderWindowInteractor
::GetRenderer()
{
  vtkRendererCollection * theRenderers =  this->RenderWindow->GetRenderers();
  theRenderers->InitTraversal();
  return theRenderers->GetNextItem();
}


struct TErase{
  VTK::TSetFunction<vtkActor,int> mySetFunction;
  TErase():
    mySetFunction(&vtkActor::SetVisibility,false)
  {}
  void operator()(SALOME_Actor* theActor){
    theActor->SetVisibility(false);
    // Erase dependent actors
    vtkActorCollection* aCollection = vtkActorCollection::New(); 
    theActor->GetChildActors(aCollection);
    VTK::ForEach<vtkActor>(aCollection,mySetFunction);
    aCollection->Delete();
  }
};

void
SVTK_RenderWindowInteractor
::EraseAll()
{   
  using namespace VTK;
  ForEach<SALOME_Actor>(GetRenderer()->GetActors(),
			TErase());

  emit RenderWindowModified() ;
}

void
SVTK_RenderWindowInteractor
::DisplayAll()
{ 
  vtkActorCollection* aCollection = GetRenderer()->GetActors();
  using namespace VTK;
  ForEach<SALOME_Actor>(aCollection,TSetVisibility<SALOME_Actor>(true));

  emit RenderWindowModified() ;
}


void
SVTK_RenderWindowInteractor
::Erase(SALOME_Actor* theActor, bool update)
{
  TErase()(theActor);

  if(update)
    emit RenderWindowModified();
}


void
SVTK_RenderWindowInteractor
::Erase(const Handle(SALOME_InteractiveObject)& theIObject, 
	bool update)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TErase());

  if(update)
    emit RenderWindowModified();
}


struct TRemoveAction{
  vtkRenderer* myRen;
  TRemoveAction(vtkRenderer* theRen): myRen(theRen){}
  void operator()(SALOME_Actor* theActor){
    myRen->RemoveActor(theActor);
  }
};

void
SVTK_RenderWindowInteractor
::Remove(const Handle(SALOME_InteractiveObject)& theIObject, 
	 bool update)
{
  vtkRenderer* aRen = GetRenderer();

  using namespace VTK;
  ForEachIf<SALOME_Actor>(aRen->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TRemoveAction(aRen));
}

void
SVTK_RenderWindowInteractor
::Remove( SALOME_Actor* SActor, bool updateViewer )
{
  if ( SActor != 0 )
  {
    GetRenderer()->RemoveProp( SActor );
    if ( updateViewer )
      emit RenderWindowModified();
  }
}

void
SVTK_RenderWindowInteractor
::RemoveAll( const bool updateViewer )
{
  vtkRenderer* aRenderer = GetRenderer();
  vtkActorCollection* anActors = aRenderer->GetActors();
  if ( anActors )
  {
    anActors->InitTraversal();
    while ( vtkActor *anAct = anActors->GetNextActor() )
    {
      if ( anAct->IsA( "SALOME_Actor" ) )
      {
        SALOME_Actor* aSAct = (SALOME_Actor*)anAct;
        if ( aSAct->hasIO() && aSAct->getIO()->hasEntry() )
          aRenderer->RemoveActor( anAct );
      }
    }

    if ( updateViewer )
      emit RenderWindowModified();
  }
}


float
SVTK_RenderWindowInteractor
::GetTransparency(const Handle(SALOME_InteractiveObject)& theIObject) 
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor)
    return 1.0 - anActor->GetOpacity();
  return -1.0;
}


void
SVTK_RenderWindowInteractor
::SetTransparency(const Handle(SALOME_InteractiveObject)& theIObject, 
		  float theTrans)
{
  float anOpacity = 1.0 - theTrans;
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,float>
			  (&SALOME_Actor::SetOpacity,anOpacity));
}


void
SVTK_RenderWindowInteractor
::Display(SALOME_Actor* theActor, bool update)
{
  GetRenderer()->AddActor(theActor);
  theActor->SetVisibility(true);

  if(update)
    emit RenderWindowModified();
}


void
SVTK_RenderWindowInteractor
::Display(const Handle(SALOME_InteractiveObject)& theIObject, bool update)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetVisibility<SALOME_Actor>(true));

  if(update)
    emit RenderWindowModified() ;
}


void
SVTK_RenderWindowInteractor
::KeyPressed(QKeyEvent *event)
{}

struct TUpdateAction{
  void operator()(vtkActor* theActor){
    theActor->ApplyProperties();
  }
};

void
SVTK_RenderWindowInteractor
::Update() 
{
  vtkRenderer* aRen = GetRenderer();

  using namespace VTK;
  ForEach<vtkActor>(aRen->GetActors(),TUpdateAction());

  aRen->ResetCamera();

  emit RenderWindowModified();  
}

//-----------------
// Color methods
//-----------------

void
SVTK_RenderWindowInteractor
::SetColor(const Handle(SALOME_InteractiveObject)& theIObject,QColor theColor) 
{
  float aColor[3] = {theColor.red()/255., theColor.green()/255., theColor.blue()/255.};
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,const float*>
			  (&SALOME_Actor::SetColor,aColor));
}


QColor
SVTK_RenderWindowInteractor
::GetColor(const Handle(SALOME_InteractiveObject)& theIObject) 
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor){
    float r,g,b;
    anActor->GetColor(r,g,b);
    return QColor(int(r*255),int(g*255),int(b*255));
  }
  return QColor(0,0,0);
}


bool
SVTK_RenderWindowInteractor
::isInViewer(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  return anActor != NULL;
}


bool
SVTK_RenderWindowInteractor
::isVisible(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  return anActor != NULL && anActor->GetVisibility();
}


void
SVTK_RenderWindowInteractor
::rename(const Handle(SALOME_InteractiveObject)& theIObject, QString theName)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,const char*,QString>
			  (&SALOME_Actor::setName,theName.latin1()));
}

SVTK_SelectionEvent
SVTK_RenderWindowInteractor
::GetSelectionEvent()
{
  SVTK_SelectionEvent aSelectionEvent;

  int x, y;
  this->GetEventPosition( x, y );

  aSelectionEvent.X = x;
  aSelectionEvent.Y = y;
  aSelectionEvent.IsCtrl = this->GetControlKey();
  aSelectionEvent.IsShift = this->GetShiftKey();

  return aSelectionEvent;
}
