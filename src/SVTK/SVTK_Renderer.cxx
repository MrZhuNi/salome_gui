#include "SVTK_Renderer.h"

#include "SVTK_Trihedron.h"
#include "SVTK_CubeAxesActor2D.h"

#include "VTKViewer_Actor.h"
#include "SALOME_Actor.h"

#include "VTKViewer_Transform.h"
#include "VTKViewer_Utilities.h"

#include "SUIT_Session.h"
#include "SUIT_ResourceMgr.h"

#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>


//----------------------------------------------------------------------------
vtkStandardNewMacro(SVTK_Renderer);

//----------------------------------------------------------------------------
SVTK_Renderer
::SVTK_Renderer():
  myTransform(VTKViewer_Transform::New()),
  myCubeAxes(SVTK_CubeAxesActor2D::New()),
  myTrihedron(SVTK_Trihedron::New()),
  myTrihedronSize(105),
  myPriority(0.0),
  myEventCallbackCommand(vtkCallbackCommand::New())
{
  myTransform->Delete();
  myTrihedron->Delete();
  myCubeAxes->Delete();
  myEventCallbackCommand->Delete();

  myTrihedron->AddToRender(this);
  this->AddProp(GetCubeAxes());

  vtkTextProperty* aTextProp = vtkTextProperty::New();
  aTextProp->SetColor(1, 1, 1);
  aTextProp->ShadowOn();
  
  float aBndBox[6];
  aBndBox[0] = aBndBox[2] = aBndBox[4] = 0;
  aBndBox[1] = aBndBox[3] = aBndBox[5] = myTrihedron->GetSize();
  myCubeAxes->SetBounds(aBndBox);
  myCubeAxes->SetCamera(this->GetActiveCamera());

  myCubeAxes->SetLabelFormat("%6.4g");
  myCubeAxes->SetFlyModeToOuterEdges(); // ENK remarks: it must bee
  myCubeAxes->SetFontFactor(0.8);
  myCubeAxes->SetAxisTitleTextProperty(aTextProp);
  myCubeAxes->SetAxisLabelTextProperty(aTextProp);
  myCubeAxes->SetCornerOffset(0);
  myCubeAxes->SetScaling(0);
  myCubeAxes->SetNumberOfLabels(5);
  myCubeAxes->VisibilityOff();
  myCubeAxes->SetTransform(GetTransform());
  aTextProp->Delete();

  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  QColor aColor = aResMgr->colorValue("VTKViewer","background",QColor());
  this->SetBackground(aColor.red()/255.0,
		      aColor.green()/255.0,
		      aColor.blue()/255.0);

  this->GetActiveCamera()->ParallelProjectionOn();
  this->LightFollowCameraOn();
  this->TwoSidedLightingOn();

  myEventCallbackCommand->SetClientData(this);
  myEventCallbackCommand->SetCallback(SVTK_Renderer::ProcessEvents);
  this->AddObserver(vtkCommand::ResetCameraEvent,
		    myEventCallbackCommand.GetPointer(), 
		    myPriority);
}

SVTK_Renderer
::~SVTK_Renderer()
{}


void 
SVTK_Renderer
::ProcessEvents(vtkObject* vtkNotUsed(theObject), 
		unsigned long theEvent,
		void* theClientData, 
		void* vtkNotUsed(theCallData))
{
  SVTK_Renderer* self = reinterpret_cast<SVTK_Renderer*>(theClientData);

  switch(theEvent){
  case vtkCommand::ResetCameraEvent:
    self->onFitAll();
    break;
  }
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
::isCubeAxesDisplayed()
{
  return myCubeAxes->GetVisibility() == 1;
}

VTKViewer_Trihedron* 
SVTK_Renderer
::GetTrihedron()
{
  return myTrihedron.GetPointer();
}

bool
SVTK_Renderer
::isTrihedronDisplayed()
{
  return myTrihedron->GetVisibility() == VTKViewer_Trihedron::eOn;
}

int
SVTK_Renderer
::GetTrihedronSize() const
{
  return myTrihedronSize;
}

void
SVTK_Renderer
::SetTrihedronSize(int theSize)
{
  if(myTrihedronSize != theSize){
    myTrihedronSize = theSize;
    AdjustTrihedrons(true);
  }
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

void
SVTK_Renderer
::AdjustTrihedrons(const bool theIsForced)
{
  bool aTDisplayed = isTrihedronDisplayed();
  bool aCDisplayed = isCubeAxesDisplayed();

  if(!aCDisplayed && !aTDisplayed && !theIsForced)
    return;

  float aBndBox[ 6 ];
  myCubeAxes->GetBounds(aBndBox);

  float aNewBndBox[6];
  aNewBndBox[ 0 ] = aNewBndBox[ 2 ] = aNewBndBox[ 4 ] = VTK_LARGE_FLOAT;
  aNewBndBox[ 1 ] = aNewBndBox[ 3 ] = aNewBndBox[ 5 ] = -VTK_LARGE_FLOAT;

  int aVisibleNum = myTrihedron->GetVisibleActorCount(this);
  if(aVisibleNum){
    // if the new trihedron size have sufficient difference, then apply the value
    double aNewSize = 100;
    double anOldSize = myTrihedron->GetSize();

    if(aTDisplayed)
      myTrihedron->VisibilityOff();

    if(aCDisplayed) 
      myCubeAxes->VisibilityOff();

    static SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
    float aSizeInPercents = aResMgr->doubleValue("VTKViewer","trihedron_size", 105);

    ComputeTrihedronSize(this,aNewSize,anOldSize,aSizeInPercents);

    myTrihedron->SetSize(aNewSize);

    // iterate through displayed objects and set size if necessary
    vtkActorCollection* anActors = this->GetActors();
    anActors->InitTraversal();
    while(vtkActor* anActor = anActors->GetNextActor()){
      if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast( anActor )){
	if(aSActor->IsResizable())
	  aSActor->SetSize(0.5 * aNewSize);
        if(aSActor->GetVisibility() && !aSActor->IsInfinitive()){
	  float *aBounds = aSActor->GetBounds();
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
  
  if(CheckBndBox(aNewBndBox))
    myCubeAxes->SetBounds(aNewBndBox);

  ::ResetCameraClippingRange(this);
}


//----------------------------------------------------------------------------
void 
SVTK_Renderer
::onViewTrihedron()
{
  if(isTrihedronDisplayed())
    myTrihedron->VisibilityOff();
  else
    myTrihedron->VisibilityOn();
}

void
SVTK_Renderer
::onViewCubeAxes()
{
  if(isCubeAxesDisplayed())
    myCubeAxes->VisibilityOff();
  else
    myCubeAxes->VisibilityOn();
}


//----------------------------------------------------------------------------
void
SVTK_Renderer
::onAdjustTrihedron()
{   
  AdjustTrihedrons(false);
}

void
SVTK_Renderer
::onAdjustCubeAxes()
{   
  AdjustTrihedrons(false);
}


//----------------------------------------------------------------------------
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
}


//----------------------------------------------------------------------------
void
SVTK_Renderer
::AddActor(VTKViewer_Actor* theActor)
{
  theActor->SetTransform(GetTransform());
  theActor->AddToRender(this);
}

void
SVTK_Renderer
::RemoveActor(VTKViewer_Actor* theActor)
{
  theActor->RemoveFromRender(this);
}


//----------------------------------------------------------------------------
void
SVTK_Renderer
::onResetView()
{
  int aTrihedronIsVisible = isTrihedronDisplayed();
  int aCubeAxesIsVisible = isCubeAxesDisplayed();

  myTrihedron->SetVisibility( VTKViewer_Trihedron::eOnlyLineOn );
  myCubeAxes->SetVisibility(0);

  ::ResetCamera(this,true);  
  vtkCamera* aCamera = this->GetActiveCamera();
  aCamera->SetPosition(1,-1,1);
  aCamera->SetViewUp(0,0,1);
  ::ResetCamera(this,true);  

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
::onFitAll()
{
  int aTrihedronWasVisible = false;
  int aCubeAxesWasVisible = false;

  aTrihedronWasVisible = isTrihedronDisplayed();
  if(aTrihedronWasVisible)
    myTrihedron->VisibilityOff();

  aCubeAxesWasVisible = isCubeAxesDisplayed();
  if(aCubeAxesWasVisible)
    myCubeAxes->VisibilityOff();

  if(myTrihedron->GetVisibleActorCount(this)){
    myTrihedron->VisibilityOff();
    myCubeAxes->VisibilityOff();
    ::ResetCamera(this);
  }else{
    myTrihedron->SetVisibility(VTKViewer_Trihedron::eOnlyLineOn);
    myCubeAxes->SetVisibility(2);
    ::ResetCamera(this,true);
  }

  if(aTrihedronWasVisible)
    myTrihedron->VisibilityOn();
  else
    myTrihedron->VisibilityOff();
  
  if(aCubeAxesWasVisible)
    myCubeAxes->VisibilityOn();
  else
    myCubeAxes->VisibilityOff();

  ::ResetCameraClippingRange(this);
}


//----------------------------------------------------------------------------
void
SVTK_Renderer
::onFrontView()
{
  vtkCamera* aCamera = this->GetActiveCamera();
  aCamera->SetPosition(1,0,0);
  aCamera->SetViewUp(0,0,1);
  aCamera->SetFocalPoint(0,0,0);
  this->onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::onBackView()
{
  vtkCamera* aCamera = this->GetActiveCamera();
  aCamera->SetPosition(-1,0,0);
  aCamera->SetViewUp(0,0,1);
  aCamera->SetFocalPoint(0,0,0);
  this->onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::onTopView()
{
  vtkCamera* aCamera = this->GetActiveCamera();
  aCamera->SetPosition(0,0,1);
  aCamera->SetViewUp(0,1,0);
  aCamera->SetFocalPoint(0,0,0);
  this->onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::onBottomView()
{
  vtkCamera* aCamera = this->GetActiveCamera();
  aCamera->SetPosition(0,0,-1);
  aCamera->SetViewUp(0,1,0);
  aCamera->SetFocalPoint(0,0,0);
  this->onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::onLeftView()
{
  vtkCamera* aCamera = this->GetActiveCamera(); 
  aCamera->SetPosition(0,-1,0);
  aCamera->SetViewUp(0,0,1);
  aCamera->SetFocalPoint(0,0,0);
  this->onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_Renderer
::onRightView()
{
  vtkCamera* aCamera = this->GetActiveCamera();
  aCamera->SetPosition(0,1,0);
  aCamera->SetViewUp(0,0,1);
  aCamera->SetFocalPoint(0,0,0);
  this->onFitAll();
}
