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
//  Module :
//  $Header$

#include "SVTK_Renderer.h"

#include "SVTK_Trihedron.h"
#include "SVTK_CubeAxesActor2D.h"

#include "SALOME_Actor.h"
#include "VTKViewer_Actor.h"
#include "VTKViewer_Transform.h"
#include "VTKViewer_Utilities.h"

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkTextProperty.h>
#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>

// undefining min and max because CASCADE's defines them and
// it clashes with std::min(), std::max() included in utilities.h
#undef min
#undef max

#include "utilities.h"

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


//----------------------------------------------------------------------------
vtkStandardNewMacro(SVTK_Renderer);

//----------------------------------------------------------------------------
SVTK_Renderer
::SVTK_Renderer():
  myDevice(vtkRenderer::New()),
  myInteractor(NULL),
  myTransform(VTKViewer_Transform::New()),
  myCubeAxes(SVTK_CubeAxesActor2D::New()),
  myTrihedron(SVTK_Trihedron::New()),
  myTrihedronSize(105),
  myPriority(0.0),
  myEventCallbackCommand(vtkCallbackCommand::New())
{
  if(MYDEBUG) INFOS("SVTK_Renderer() - "<<this);

  myDevice->Delete();
  myTransform->Delete();
  myTrihedron->Delete();
  myCubeAxes->Delete();
  myEventCallbackCommand->Delete();

  myTrihedron->AddToRender(GetDevice());
  GetDevice()->AddProp(GetCubeAxes());

  myBndBox[0] = myBndBox[2] = myBndBox[4] = 0;
  myBndBox[1] = myBndBox[3] = myBndBox[5] = myTrihedron->GetSize();

  myCubeAxes->SetBounds(myBndBox);
  myCubeAxes->SetCamera(GetDevice()->GetActiveCamera());

  myCubeAxes->SetLabelFormat("%6.4g");
  myCubeAxes->SetFlyModeToOuterEdges(); // ENK remarks: it must bee
  myCubeAxes->SetFontFactor(0.8);
  myCubeAxes->SetCornerOffset(0);
  myCubeAxes->SetScaling(0);
  myCubeAxes->SetNumberOfLabels(5);
  myCubeAxes->VisibilityOff();
  myCubeAxes->SetTransform(GetTransform());

  vtkTextProperty* aTextProp = vtkTextProperty::New();
  aTextProp->SetColor(1, 1, 1);
  aTextProp->ShadowOn();
  myCubeAxes->SetAxisTitleTextProperty(aTextProp);
  myCubeAxes->SetAxisLabelTextProperty(aTextProp);
  aTextProp->Delete();

  GetDevice()->GetActiveCamera()->ParallelProjectionOn();
  GetDevice()->LightFollowCameraOn();
  GetDevice()->TwoSidedLightingOn();

  myEventCallbackCommand->SetClientData(this);
  myEventCallbackCommand->SetCallback(SVTK_Renderer::ProcessEvents);
  GetDevice()->AddObserver(vtkCommand::ConfigureEvent,
			   myEventCallbackCommand.GetPointer(), 
			   myPriority);
  GetDevice()->AddObserver(vtkCommand::ResetCameraEvent,
			   myEventCallbackCommand.GetPointer(), 
			   myPriority);
  GetDevice()->AddObserver(vtkCommand::ResetCameraClippingRangeEvent,
			   myEventCallbackCommand.GetPointer(), 
			   myPriority);
}

SVTK_Renderer
::~SVTK_Renderer()
{
  if(MYDEBUG) INFOS("~SVTK_Renderer() - "<<this);
}


void 
SVTK_Renderer
::ProcessEvents(vtkObject* vtkNotUsed(theObject), 
		unsigned long theEvent,
		void* theClientData, 
		void* vtkNotUsed(theCallData))
{
  SVTK_Renderer* self = reinterpret_cast<SVTK_Renderer*>(theClientData);

  switch(theEvent){
  case vtkCommand::ConfigureEvent:
    self->OnResetView();
    break;
  case vtkCommand::ResetCameraEvent:
    self->OnFitAll();
    break;
  case vtkCommand::ResetCameraClippingRangeEvent:
    self->OnResetClippingRange();
    break;
  }
}

//----------------------------------------------------------------------------
vtkRenderer* 
SVTK_Renderer
::GetDevice()
{
  return myDevice.GetPointer();
}

void 
SVTK_Renderer
::SetInteractor(vtkRenderWindowInteractor* theInteractor)
{
  myInteractor = theInteractor;
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::AddActor(VTKViewer_Actor* theActor)
{
  if(SALOME_Actor* anActor = dynamic_cast<SALOME_Actor*>(theActor))
    anActor->SetInteractor(myInteractor);
  theActor->SetTransform(GetTransform());
  theActor->AddToRender(GetDevice());
  AdjustActors();
}

void
SVTK_Renderer
::RemoveActor(VTKViewer_Actor* theActor)
{
  theActor->SetTransform(NULL);
  theActor->RemoveFromRender(GetDevice());
  AdjustActors();
}

VTKViewer_Transform* 
SVTK_Renderer
::GetTransform()
{
  return myTransform.GetPointer();
}

void
SVTK_Renderer
::GetScale( double theScale[3] ) 
{
  myTransform->GetMatrixScale( theScale );
}

void
SVTK_Renderer
::SetScale( double theScale[3] ) 
{
  myTransform->SetMatrixScale( theScale[0], theScale[1], theScale[2] );
  AdjustActors();
}


//----------------------------------------------------------------------------
/*! If parameter theIsForcedUpdate is true, recalculate parameters for
 *  trihedron and cube axes, even if trihedron and cube axes is invisible.
 */

inline
bool
CheckBndBox(const float theBounds[6])
{
  if(theBounds[0] > -VTK_LARGE_FLOAT && theBounds[1] < VTK_LARGE_FLOAT &&
     theBounds[2] > -VTK_LARGE_FLOAT && theBounds[3] < VTK_LARGE_FLOAT &&
     theBounds[4] > -VTK_LARGE_FLOAT && theBounds[5] < VTK_LARGE_FLOAT)
    return true;
  return false;
}

bool
SVTK_Renderer
::OnAdjustActors()
{
  bool aTDisplayed = IsTrihedronDisplayed();
  bool aCDisplayed = IsCubeAxesDisplayed();

  float aNewBndBox[6];
  aNewBndBox[ 0 ] = aNewBndBox[ 2 ] = aNewBndBox[ 4 ] = VTK_LARGE_FLOAT;
  aNewBndBox[ 1 ] = aNewBndBox[ 3 ] = aNewBndBox[ 5 ] = -VTK_LARGE_FLOAT;

  int aVisibleNum = myTrihedron->GetVisibleActorCount(GetDevice());
  if(aVisibleNum){
    if(aTDisplayed)
      myTrihedron->VisibilityOff();

    if(aCDisplayed) 
      myCubeAxes->VisibilityOff();

    // if the new trihedron size have sufficient difference, then apply the value
    double aSize = myTrihedron->GetSize();
    ComputeTrihedronSize(GetDevice(),aSize,aSize,myTrihedronSize);
    myTrihedron->SetSize(aSize);

    // iterate through displayed objects and set size if necessary
    vtkActorCollection* anActors = GetDevice()->GetActors();
    anActors->InitTraversal();
    while(vtkActor* anAct = anActors->GetNextActor()){
      if(SALOME_Actor* anActor = dynamic_cast<SALOME_Actor*>(anAct)){
	if(anActor->IsResizable())
	  anActor->SetSize(0.5*aSize);
        if(anActor->GetVisibility() && !anActor->IsInfinitive()){
	  float *aBounds = anActor->GetBounds();
          if(CheckBndBox(aBounds))
	    for(int i = 0; i < 5; i = i + 2){
	      if(aBounds[i] < aNewBndBox[i]) 
		aNewBndBox[i] = aBounds[i];
	      if(aBounds[i+1] > aNewBndBox[i+1]) 
		aNewBndBox[i+1] = aBounds[i+1];
	    }
        }
      }
    }

    if(aTDisplayed) 
      myTrihedron->VisibilityOn();

    if(aCDisplayed) 
      myCubeAxes->VisibilityOn();
    
  }else{
    double aSize = myTrihedron->GetSize();
    aNewBndBox[0] = aNewBndBox[2] = aNewBndBox[4] = 0;
    aNewBndBox[1] = aNewBndBox[3] = aNewBndBox[5] = aSize;
  }
  
  if(CheckBndBox(aNewBndBox)){
    for(int i = 0; i < 6; i++)
      myBndBox[i] = aNewBndBox[i];
    myCubeAxes->SetBounds(myBndBox);
    return true;
  }

  return false;
}

void
SVTK_Renderer
::AdjustActors()
{
  if(OnAdjustActors())
    ::ResetCameraClippingRange(GetDevice());
}

void
SVTK_Renderer
::SetTrihedronSize(int theSize)
{
  if(myTrihedronSize != theSize){
    myTrihedronSize = theSize;
    AdjustActors();
  }
}

int
SVTK_Renderer
::GetTrihedronSize() const
{
  return myTrihedronSize;
}


//----------------------------------------------------------------------------
VTKViewer_Trihedron* 
SVTK_Renderer
::GetTrihedron()
{
  return myTrihedron.GetPointer();
}

bool
SVTK_Renderer
::IsTrihedronDisplayed()
{
  return myTrihedron->GetVisibility() == VTKViewer_Trihedron::eOn;
}

void 
SVTK_Renderer
::OnViewTrihedron()
{
  if(IsTrihedronDisplayed())
    myTrihedron->VisibilityOff();
  else
    myTrihedron->VisibilityOn();
}

void
SVTK_Renderer
::OnAdjustTrihedron()
{   
  AdjustActors();
}


//----------------------------------------------------------------------------
SVTK_CubeAxesActor2D* 
SVTK_Renderer
::GetCubeAxes()
{
  return myCubeAxes.GetPointer();
}

bool
SVTK_Renderer
::IsCubeAxesDisplayed()
{
  return myCubeAxes->GetVisibility() == 1;
}

void
SVTK_Renderer
::OnViewCubeAxes()
{
  if(IsCubeAxesDisplayed())
    myCubeAxes->VisibilityOff();
  else
    myCubeAxes->VisibilityOn();
}

void
SVTK_Renderer
::OnAdjustCubeAxes()
{   
  AdjustActors();
}


//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnResetView()
{
  int aTrihedronIsVisible = IsTrihedronDisplayed();
  int aCubeAxesIsVisible = IsCubeAxesDisplayed();

  myTrihedron->SetVisibility( VTKViewer_Trihedron::eOnlyLineOn );
  myCubeAxes->SetVisibility(0);

  ::ResetCamera(GetDevice(),true);  
  vtkCamera* aCamera = GetDevice()->GetActiveCamera();
  aCamera->SetPosition(1,-1,1);
  aCamera->SetViewUp(0,0,1);
  ::ResetCamera(GetDevice(),true);  

  if(aTrihedronIsVisible) 
    myTrihedron->VisibilityOn();
  else
    myTrihedron->VisibilityOff();

  if(aCubeAxesIsVisible) 
    myCubeAxes->VisibilityOn();
  else
    myCubeAxes->VisibilityOff();

  static float aCoeff = 3.0;
  aCamera->SetParallelScale(aCoeff*aCamera->GetParallelScale());
}


//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnFitAll()
{
  int aTrihedronWasVisible = false;
  int aCubeAxesWasVisible = false;

  aTrihedronWasVisible = IsTrihedronDisplayed();
  if(aTrihedronWasVisible)
    myTrihedron->VisibilityOff();

  aCubeAxesWasVisible = IsCubeAxesDisplayed();
  if(aCubeAxesWasVisible)
    myCubeAxes->VisibilityOff();

  if(myTrihedron->GetVisibleActorCount(GetDevice())){
    myTrihedron->VisibilityOff();
    myCubeAxes->VisibilityOff();
    ::ResetCamera(GetDevice());
  }else{
    myTrihedron->SetVisibility(VTKViewer_Trihedron::eOnlyLineOn);
    myCubeAxes->SetVisibility(2);
    ::ResetCamera(GetDevice(),true);
  }

  if(aTrihedronWasVisible)
    myTrihedron->VisibilityOn();
  else
    myTrihedron->VisibilityOff();
  
  if(aCubeAxesWasVisible)
    myCubeAxes->VisibilityOn();
  else
    myCubeAxes->VisibilityOff();

  ::ResetCameraClippingRange(GetDevice());
}


//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnResetClippingRange()
{
  return;
  ::ResetCameraClippingRange(GetDevice());
}


//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnFrontView()
{
  vtkCamera* aCamera = GetDevice()->GetActiveCamera();
  aCamera->SetPosition(1,0,0);
  aCamera->SetViewUp(0,0,1);
  aCamera->SetFocalPoint(0,0,0);
  this->OnFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnBackView()
{
  vtkCamera* aCamera = GetDevice()->GetActiveCamera();
  aCamera->SetPosition(-1,0,0);
  aCamera->SetViewUp(0,0,1);
  aCamera->SetFocalPoint(0,0,0);
  this->OnFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnTopView()
{
  vtkCamera* aCamera = GetDevice()->GetActiveCamera();
  aCamera->SetPosition(0,0,1);
  aCamera->SetViewUp(0,1,0);
  aCamera->SetFocalPoint(0,0,0);
  this->OnFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnBottomView()
{
  vtkCamera* aCamera = GetDevice()->GetActiveCamera();
  aCamera->SetPosition(0,0,-1);
  aCamera->SetViewUp(0,1,0);
  aCamera->SetFocalPoint(0,0,0);
  this->OnFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnLeftView()
{
  vtkCamera* aCamera = GetDevice()->GetActiveCamera(); 
  aCamera->SetPosition(0,-1,0);
  aCamera->SetViewUp(0,0,1);
  aCamera->SetFocalPoint(0,0,0);
  this->OnFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::OnRightView()
{
  vtkCamera* aCamera = GetDevice()->GetActiveCamera();
  aCamera->SetPosition(0,1,0);
  aCamera->SetViewUp(0,0,1);
  aCamera->SetFocalPoint(0,0,0);
  this->OnFitAll();
}
