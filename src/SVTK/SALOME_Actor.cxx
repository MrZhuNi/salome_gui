//  SALOME OBJECT : implementation of interactive object visualization for OCC and VTK viewers
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
//  File   : SALOME_Actor.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class SALOME_Actor SALOME_Actor.h
  \brief Abstract class of SALOME Objects in VTK.
*/


#include "SALOME_Actor.h"

#include "VTKViewer_Transform.h"
#include "VTKViewer_TransformFilter.h"
#include "VTKViewer_PassThroughFilter.h"
#include "VTKViewer_GeometryFilter.h"
#include "VTKViewer_CellRectPicker.h"

#include "SVTK_Actor.h"

// VTK Includes
#include <vtkCell.h>
#include <vtkLine.h>
#include <vtkPicker.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkObjectFactory.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkOutlineSource.h>

#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>

#include <TColStd_MapOfInteger.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>

using namespace std;

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

int SALOME_POINT_SIZE = 5;
int SALOME_LINE_WIDTH = 3;

//----------------------------------------------------------------------------
namespace
{
  int
  GetEdgeId(SALOME_Actor* theActor,
	    vtkPicker* thePicker, 
	    int theObjId)
  {
    int anEdgeId = 0;
    if (vtkCell* aPickedCell = theActor->GetElemCell(theObjId)) {
      float aPickPosition[3];
      thePicker->GetPickPosition(aPickPosition);
      float aMinDist = 1000000.0, aDist = 0;
      for (int i = 0, iEnd = aPickedCell->GetNumberOfEdges(); i < iEnd; i++){
	if(vtkLine* aLine = vtkLine::SafeDownCast(aPickedCell->GetEdge(i))){
	  int subId;  float pcoords[3], closestPoint[3], weights[3];
	  aLine->EvaluatePosition(aPickPosition,closestPoint,subId,pcoords,aDist,weights);
	  if (aDist < aMinDist) {
	    aMinDist = aDist;
	    anEdgeId = -1 - i;
	  }
	}
      }
    }
    return anEdgeId;
  }

  inline
  bool
  CheckDimensionId(Selection_Mode theMode, 
		   SALOME_Actor *theActor, 
		   vtkIdType theObjId)
  {
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
vtkStandardNewMacro(SALOME_Actor);


//----------------------------------------------------------------------------
SALOME_Actor
::SALOME_Actor():
  myRenderer(NULL),
  myInteractor(NULL),
  mySelectionMode(ActorSelection),
  myIsHighlighted(false),
  myIsPreselected(false),
  myRepresentation(VTK_SURFACE),
  myDisplayMode(1),
  myProperty(vtkProperty::New()),
  PreviewProperty(NULL),
  myIsInfinite(false),
  myIsResolveCoincidentTopology(true),
  myStoreMapping(false),
  myGeomFilter(VTKViewer_GeometryFilter::New()),
  myTransformFilter(VTKViewer_TransformFilter::New()),
  myPreHighlightActor(SVTK_Actor::New()),
  myHighlightActor(SVTK_Actor::New()),
  myOutline(vtkOutlineSource::New()),
  myOutlineActor(VTKViewer_Actor::New())
{
  vtkMapper::GetResolveCoincidentTopologyPolygonOffsetParameters(myPolygonOffsetFactor,
								 myPolygonOffsetUnits);

  for(int i = 0; i < 6; i++)
    myPassFilter.push_back(VTKViewer_PassThroughFilter::New());

  myPreHighlightActor->Delete();
  myPreHighlightActor->Initialize();
  myPreHighlightActor->PickableOff();
  myPreHighlightActor->SetVisibility( false );

  myHighlightActor->Delete();
  myHighlightActor->Initialize();
  myHighlightActor->PickableOff();
  myHighlightActor->SetVisibility( false );

  myOutline->Delete();

  vtkPolyDataMapper* anOutlineMapper = vtkPolyDataMapper::New();
  anOutlineMapper->SetInput(myOutline->GetOutput());

  myOutlineActor->Delete();
  myOutlineActor->SetMapper( anOutlineMapper );
  anOutlineMapper->Delete();

  myOutlineActor->PickableOff();
  myOutlineActor->DragableOff();
  myOutlineActor->GetProperty()->SetColor(1.0,0.0,0.0);
  myOutlineActor->GetProperty()->SetAmbient(1.0);
  myOutlineActor->GetProperty()->SetDiffuse(0.0);
  myOutlineActor->SetVisibility( false );
}


//----------------------------------------------------------------------------
SALOME_Actor
::~SALOME_Actor()
{
  SetPreviewProperty(NULL);

  myGeomFilter->UnRegisterAllOutputs(); 
  myGeomFilter->Delete();

  myTransformFilter->UnRegisterAllOutputs();
  myTransformFilter->Delete();

  for(int i = 0, iEnd = myPassFilter.size(); i < iEnd; i++){
    if(myPassFilter[i]){
      myPassFilter[i]->UnRegisterAllOutputs(); 
      myPassFilter[i]->Delete();
    }
  }

  myProperty->Delete();
}


//----------------------------------------------------------------------------
Standard_Boolean 
SALOME_Actor
::hasIO() 
{ 
  return !myIO.IsNull(); 
}

const Handle(SALOME_InteractiveObject)& 
SALOME_Actor
::getIO()
{ 
  return myIO; 
}

void
SALOME_Actor
::setIO(const Handle(SALOME_InteractiveObject)& theIO) 
{ 
  myIO = theIO; 
}

const char* 
SALOME_Actor
::getName() 
{ 
  return myName.c_str(); 
}

void
SALOME_Actor
::setName(const char* theName)
{
  if(hasIO())	
    myIO->setName(theName);
  myName = theName;
}


//----------------------------------------------------------------------------
void
SALOME_Actor
::AddToRender(vtkRenderer* theRenderer)
{
  myRenderer = theRenderer;

  theRenderer->AddActor(this);

  theRenderer->AddActor( myPreHighlightActor.GetPointer() );
  theRenderer->AddActor( myHighlightActor.GetPointer() );
  theRenderer->AddActor( myOutlineActor.GetPointer() );
}

void 
SALOME_Actor
::RemoveFromRender(vtkRenderer* theRenderer)
{
  theRenderer->RemoveActor(this);

  theRenderer->RemoveActor( myPreHighlightActor.GetPointer() );
  theRenderer->RemoveActor( myHighlightActor.GetPointer() );
  theRenderer->RemoveActor( myOutlineActor.GetPointer() );
}

vtkRenderer*
SALOME_Actor
::GetRenderer()
{
  return myRenderer;
}

void
SALOME_Actor
::GetChildActors(vtkActorCollection*) 
{}


//----------------------------------------------------------------------------
void
SALOME_Actor
::SetInteractor(vtkRenderWindowInteractor* theInteractor)
{
  myInteractor = theInteractor;
}

void
SALOME_Actor
::Update()
{
  myInteractor->CreateTimer(VTKI_TIMER_UPDATE);    
}


//----------------------------------------------------------------------------
void
SALOME_Actor
::SetTransform(VTKViewer_Transform* theTransform)
{
  Superclass::SetTransform(theTransform);

  myPreHighlightActor->SetTransform(theTransform);
  myHighlightActor->SetTransform(theTransform);
  myOutlineActor->SetTransform(theTransform);
}


void
SALOME_Actor
::SetPosition(float _arg1, float _arg2, float _arg3)
{
  Superclass::SetPosition(_arg1,_arg2,_arg3);

  myPreHighlightActor->SetPosition(_arg1,_arg2,_arg3);
  myHighlightActor->SetPosition(_arg1,_arg2,_arg3);
  myOutlineActor->SetPosition(_arg1,_arg2,_arg3);
}


void
SALOME_Actor
::SetPosition(float _arg[3])
{
  SetPosition(_arg[0],_arg[1],_arg[2]);
}


void
SALOME_Actor
::SetMapper(vtkMapper* theMapper)
{
  InitPipeLine(theMapper);
}

void
SALOME_Actor
::InitPipeLine(vtkMapper* theMapper)
{
  if(theMapper){
    int anId = 0;
    myPassFilter[ anId ]->SetInput( theMapper->GetInput() );
    myPassFilter[ anId + 1]->SetInput( myPassFilter[ anId ]->GetOutput() );
    
    anId++; // 1
    myGeomFilter->SetStoreMapping( myStoreMapping );
    myGeomFilter->SetInput( myPassFilter[ anId ]->GetOutput() );

    anId++; // 2
    myPassFilter[ anId ]->SetInput( myGeomFilter->GetOutput() ); 
    myPassFilter[ anId + 1 ]->SetInput( myPassFilter[ anId ]->GetOutput() );

    anId++; // 3
    myTransformFilter->SetInput( myPassFilter[ anId ]->GetPolyDataOutput() );

    anId++; // 4
    myPassFilter[ anId ]->SetInput( myTransformFilter->GetOutput() );
    myPassFilter[ anId + 1 ]->SetInput( myPassFilter[ anId ]->GetOutput() );

    anId++; // 5
    if(vtkDataSetMapper* aMapper = dynamic_cast<vtkDataSetMapper*>(theMapper)){
      aMapper->SetInput(myPassFilter[anId]->GetOutput());
    }else if(vtkPolyDataMapper* aMapper = dynamic_cast<vtkPolyDataMapper*>(theMapper)){
      aMapper->SetInput(myPassFilter[anId]->GetPolyDataOutput());
    }
  }
  Superclass::SetMapper(theMapper);
}


//----------------------------------------------------------------------------
void
SALOME_Actor
::Render(vtkRenderer *ren, vtkMapper* m)
{
  if(myIsResolveCoincidentTopology){
    int aResolveCoincidentTopology = vtkMapper::GetResolveCoincidentTopology();
    float aFactor, aUnit; 
    vtkMapper::GetResolveCoincidentTopologyPolygonOffsetParameters(aFactor,aUnit);
    
    vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();
    vtkMapper::SetResolveCoincidentTopologyPolygonOffsetParameters(myPolygonOffsetFactor,
								   myPolygonOffsetUnits);
    Superclass::Render(ren,m);
    
    vtkMapper::SetResolveCoincidentTopologyPolygonOffsetParameters(aFactor,aUnit);
    vtkMapper::SetResolveCoincidentTopology(aResolveCoincidentTopology);
  }else{
    Superclass::Render(ren,m);
  }
}


void
SALOME_Actor
::SetResolveCoincidentTopology(bool theIsResolve) 
{
  myIsResolveCoincidentTopology = theIsResolve;
}

void
SALOME_Actor
::SetPolygonOffsetParameters(float factor, float units)
{
  myPolygonOffsetFactor = factor;
  myPolygonOffsetUnits = units;
}

void
SALOME_Actor
::GetPolygonOffsetParameters(float& factor, float& units)
{
  factor = myPolygonOffsetFactor;
  units = myPolygonOffsetUnits;
}


//----------------------------------------------------------------------------
float
SALOME_Actor
::GetShrinkFactor() 
{ 
  return 1.0;
}

bool
SALOME_Actor
::IsShrunkable() 
{ 
  return false;
}

bool
SALOME_Actor
::IsShrunk() 
{ 
  return false;
}

void
SALOME_Actor
::SetShrink() 
{} 

void
SALOME_Actor
::UnShrink() 
{}


//----------------------------------------------------------------------------
vtkDataSet* 
SALOME_Actor
::GetInput()
{
  return myPassFilter.front()->GetOutput();
}


unsigned long int
SALOME_Actor
::GetMTime()
{
  unsigned long mTime = this->Superclass::GetMTime();
  unsigned long time = myTransformFilter->GetMTime();
  mTime = ( time > mTime ? time : mTime );
  if(vtkDataSet *aDataSet = myPassFilter[0]->GetInput()){
    time = aDataSet->GetMTime();
    mTime = ( time > mTime ? time : mTime );
  }
  return mTime;
}


//----------------------------------------------------------------------------
void
SALOME_Actor
::SetRepresentation(int theMode) 
{ 
  switch(myRepresentation){
  case VTK_POINTS : 
  case VTK_SURFACE : 
    myProperty->DeepCopy(GetProperty());
  }    
  switch(theMode){
  case VTK_POINTS : 
  case VTK_SURFACE : 
    GetProperty()->DeepCopy(myProperty);
    break;
  default:
    GetProperty()->SetAmbient(1.0);
    GetProperty()->SetDiffuse(0.0);
    GetProperty()->SetSpecular(0.0);
  }
  switch(theMode){
  case 3 : 
    myGeomFilter->SetInside(true);
    myGeomFilter->SetWireframeMode(true);
    GetProperty()->SetRepresentation(1);
    break;
  case VTK_POINTS : 
    GetProperty()->SetPointSize(SALOME_POINT_SIZE);  
    GetProperty()->SetRepresentation(theMode);
    myGeomFilter->SetWireframeMode(false);
    myGeomFilter->SetInside(false);
    break;
  case VTK_WIREFRAME : 
    GetProperty()->SetRepresentation(theMode);
    myGeomFilter->SetWireframeMode(true);
    myGeomFilter->SetInside(false);
    break;
  case VTK_SURFACE : 
    GetProperty()->SetRepresentation(theMode);
    myGeomFilter->SetWireframeMode(false);
    myGeomFilter->SetInside(false);
    break;
  }
  myRepresentation = theMode;
}

int
SALOME_Actor
::GetRepresentation()
{ 
  return myRepresentation;
}


//----------------------------------------------------------------------------
int 
SALOME_Actor
::GetNodeObjId(int theVtkID)
{ 
  return theVtkID;
}

float* 
SALOME_Actor
::GetNodeCoord(int theObjID)
{
  return GetInput()->GetPoint(theObjID);
}

vtkCell* 
SALOME_Actor
::GetElemCell(int theObjID)
{
  return GetInput()->GetCell(theObjID);
}

int
SALOME_Actor
::GetElemObjId(int theVtkID) 
{ 
  return theVtkID;
}


//=================================================================================
// function : GetObjDimension
// purpose  : Return object dimension.
//            Virtual method shoulb be redifined by derived classes
//=================================================================================
int
SALOME_Actor
::GetObjDimension( const int theObjId )
{
  if ( vtkCell* aCell = GetElemCell(theObjId) )
    return aCell->GetCellDimension();
  return 0;
}


void
SALOME_Actor
::SetInfinitive(bool theIsInfinite)
{ 
  myIsInfinite = theIsInfinite;
}


bool
SALOME_Actor
::IsInfinitive()
{ 
  return myIsInfinite; 
}


float* 
SALOME_Actor
::GetBounds()
{
  return Superclass::GetBounds();
}


void
SALOME_Actor
::GetBounds(float theBounds[6])
{
  Superclass::GetBounds(theBounds);
}


//----------------------------------------------------------------------------
bool
SALOME_Actor
::IsSetCamera() const 
{ 
  return false; 
}

bool
SALOME_Actor
::IsResizable() const 
{ 
  return false; 
}

void
SALOME_Actor
::SetSize( const float ) 
{}


void 
SALOME_Actor
::SetCamera( vtkCamera* ) 
{}

//----------------------------------------------------------------------------
void
SALOME_Actor
::SetOpacity(float theOpacity)
{ 
  myOpacity = theOpacity;
  GetProperty()->SetOpacity(theOpacity);
}

float
SALOME_Actor
::GetOpacity()
{
  return myOpacity;
}


void
SALOME_Actor
::SetColor(float r,float g,float b)
{
  GetProperty()->SetColor(r,g,b);
}

void
SALOME_Actor
::SetColor(const float theRGB[3])
{ 
  SetColor(theRGB[0],theRGB[1],theRGB[2]);
}

void
SALOME_Actor
::GetColor(float& r,float& g,float& b)
{
  float aColor[3];
  GetProperty()->GetColor(aColor);
  r = aColor[0];
  g = aColor[1];
  b = aColor[2];
}


//----------------------------------------------------------------------------
int
SALOME_Actor
::getDisplayMode()
{ 
  return myDisplayMode; 
}

void
SALOME_Actor::setDisplayMode(int theMode)
{ 
  SetRepresentation(theMode+1); 
  myDisplayMode = GetRepresentation() - 1;
}


//----------------------------------------------------------------
void
SALOME_Actor
::SetVisibility( int theVisibility )
{
  Superclass::SetVisibility( theVisibility );

  myOutlineActor->SetVisibility( theVisibility && isHighlighted() && !hasHighlight() );

  myPreHighlightActor->SetVisibility( theVisibility && myIsPreselected );

  if(mySelector.GetPointer() && hasIO()){
    if(mySelector->SelectionMode() != ActorSelection){
      int aHasIndex = mySelector->HasIndex( getIO() );
      myHighlightActor->SetVisibility( theVisibility && isHighlighted() && aHasIndex);
    }
  }
}


//----------------------------------------------------------------------------
bool
SALOME_Actor
::hasHighlight() 
{ 
  return false; 
} 

bool
SALOME_Actor
::isHighlighted() 
{ 
  return myIsHighlighted; 
}

void
SALOME_Actor
::SetPreSelected(bool thePreselect) 
{ 
  myIsPreselected = thePreselect;
}


//----------------------------------------------------------------
void
SALOME_Actor
::SetSelector(SVTK_Selector* theSelector)
{
  mySelector = theSelector;
}

void
SALOME_Actor
::Highlight(bool theIsHighlight)
{
  mySelectionMode = mySelector->SelectionMode();
  myHighlightActor->SetVisibility( false );
  myOutlineActor->SetVisibility( false );

  if(mySelector.GetPointer()){
    if(mySelectionMode != ActorSelection){
      TColStd_IndexedMapOfInteger aMapIndex;
      mySelector->GetIndex( getIO(), aMapIndex );
      switch( mySelectionMode ){
      case NodeSelection:
	myHighlightActor->GetProperty()->SetRepresentationToPoints();
	myHighlightActor->MapPoints( this, aMapIndex );
	break;
      case EdgeOfCellSelection:
	myHighlightActor->GetProperty()->SetRepresentationToWireframe();
	myHighlightActor->MapEdge( this, aMapIndex );
	break;
      case CellSelection:
      case EdgeSelection:
      case FaceSelection:
      case VolumeSelection:
	myHighlightActor->GetProperty()->SetRepresentationToSurface();
	myHighlightActor->MapCells( this, aMapIndex );
	break;
      }
      myHighlightActor->SetVisibility( GetVisibility() && theIsHighlight );
    }
  }

  highlight(theIsHighlight);
}

void
SALOME_Actor
::highlight(bool theIsHighlight)
{
  myOutline->SetBounds( GetInput()->GetBounds() );
  myOutlineActor->SetVisibility( GetVisibility() && theIsHighlight );
  myIsHighlighted = theIsHighlight; 
}


//----------------------------------------------------------------
bool
SALOME_Actor
::PreHighlight(vtkInteractorStyle *theInteractorStyle, 
	       SVTK_SelectionEvent* theSelectionEvent,
	       bool theIsHighlight)
{
  vtkRenderer *aRenderer = theInteractorStyle->GetCurrentRenderer();
  //
  myPreHighlightActor->SetVisibility( false );
  bool anIsPreselected = myIsPreselected;
  
  Selection_Mode aSelectionMode = theSelectionEvent->mySelectionMode;
  bool anIsChanged = (mySelectionMode != aSelectionMode);

  float x = theSelectionEvent->myX;
  float y = theSelectionEvent->myY;
  float z = 0.0;

  if( !theIsHighlight ) {
    SetPreSelected( false );
    vtkActorCollection* theActors = aRenderer->GetActors();
    theActors->InitTraversal();
    while( vtkActor *ac = theActors->GetNextActor() )
      if( SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac ) )
	if( anActor->hasIO() && myIO->isSame( anActor->getIO() ) )
	  anActor->SetPreSelected( false );

  }else{
    switch(aSelectionMode){
    case NodeSelection: 
    {
      myPointPicker->Pick( x, y, z, aRenderer );
      
      int aVtkId = myPointPicker->GetPointId();
      if( aVtkId >= 0 && mySelector->IsValid( this, aVtkId, true ) ) {
	int anObjId = GetNodeObjId( aVtkId );
	myIsPreselected = (anObjId >= 0);
	if(myIsPreselected){
	  const TColStd_IndexedMapOfInteger& aMapIndex = myPreHighlightActor->GetMapIndex();
	  int anExtent = aMapIndex.Extent();
	  anIsChanged |= (anExtent == 0 || anExtent > 0 && anObjId != aMapIndex(1));
	  if(anIsChanged){
	    TColStd_IndexedMapOfInteger aMapIndex;
	    aMapIndex.Add( anObjId );
	    
	    myPreHighlightActor->GetProperty()->SetRepresentationToPoints();
	    myPreHighlightActor->MapPoints( this, aMapIndex );
	  }
	  myPreHighlightActor->SetVisibility( true );
	}
      }
      break;
    }
    case CellSelection: 
    case EdgeSelection:
    case FaceSelection:
    case VolumeSelection: 
    {
      myCellPicker->Pick( x, y, z, aRenderer );
      
      int aVtkId = myCellPicker->GetCellId();
      if ( aVtkId >= 0 && mySelector->IsValid( this, aVtkId ) && hasIO() ) {
	int anObjId = GetElemObjId (aVtkId );
	if ( anObjId >= 0 ) {
	  myIsPreselected = CheckDimensionId(aSelectionMode,this,anObjId);
	  if(myIsPreselected){
	    const TColStd_IndexedMapOfInteger& aMapIndex = myPreHighlightActor->GetMapIndex();
	    int anExtent = aMapIndex.Extent();
	    anIsChanged |= (anExtent == 0 || anExtent > 0 && anObjId != aMapIndex(1));
	    if(anIsChanged){
	      TColStd_IndexedMapOfInteger aMapIndex;
	      aMapIndex.Add( anObjId );
	      
	      myPreHighlightActor->GetProperty()->SetRepresentationToSurface();
	      myPreHighlightActor->MapCells( this, aMapIndex );
	    }
	    myPreHighlightActor->SetVisibility( true );
	  }
	}
      }
      break;
    }
    case EdgeOfCellSelection:
    {
      myCellPicker->Pick( x, y, z, aRenderer );
      
      int aVtkId = myCellPicker->GetCellId();
      if ( aVtkId >= 0 && mySelector->IsValid( this, aVtkId )) {
	int anObjId = GetElemObjId( aVtkId );
	if ( anObjId >= 0 ) {
	  int anEdgeId = GetEdgeId(this,myCellPicker.GetPointer(),anObjId);
	  myIsPreselected = anEdgeId < 0;
	  if(myIsPreselected){
	    const TColStd_IndexedMapOfInteger& aMapIndex = myPreHighlightActor->GetMapIndex();
	    int anExtent = aMapIndex.Extent();
	    anIsChanged |= (anExtent != 2);
	    anIsChanged |= (anExtent == 0 && (anObjId != aMapIndex(1) || anEdgeId != aMapIndex(2)));
	    if(anIsChanged){
	      TColStd_IndexedMapOfInteger aMapIndex;
	      aMapIndex.Add( anObjId );
	      aMapIndex.Add( anEdgeId );
	
	      myPreHighlightActor->GetProperty()->SetRepresentationToWireframe();
	      myPreHighlightActor->MapEdge( this, aMapIndex );
	    }
	    myPreHighlightActor->SetVisibility( true );
	  }
	}
      }
      break;
    }
    case ActorSelection : 
    {
      if( !mySelector->IsSelected( myIO ) ) {
	SetPreSelected( true );

	vtkActorCollection* theActors = aRenderer->GetActors();
	theActors->InitTraversal();
	while( vtkActor *anAct = theActors->GetNextActor() ) {
	  if( anAct != this )
	    if( SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( anAct ) )
	      if( anActor->hasIO() && myIO->isSame( anActor->getIO() ) )
		anActor->SetPreSelected( true );
	}
      }
    }
    default:
      break;
    }
  }

  mySelectionMode = aSelectionMode;
  anIsChanged |= (anIsPreselected != myIsPreselected);

  return anIsChanged;
}


//----------------------------------------------------------------
bool
SALOME_Actor
::Highlight(vtkInteractorStyle *theInteractorStyle, 
	    SVTK_SelectionEvent* theSelectionEvent,
	    bool theIsHighlight)
{
  myOutlineActor->SetVisibility( false );
  myHighlightActor->SetVisibility( false );

  vtkRenderer *aRenderer = theInteractorStyle->GetCurrentRenderer();
  //
  Selection_Mode aSelectionMode = theSelectionEvent->mySelectionMode;
  bool isShift = theSelectionEvent->myIsShift;

  float x = theSelectionEvent->myX;
  float y = theSelectionEvent->myY;
  float z = 0.0;

  if( !theSelectionEvent->myIsRectangle ) {
    switch(aSelectionMode){
    case NodeSelection: {
      myPointPicker->Pick( x, y, z, aRenderer );

      int aVtkId = myPointPicker->GetPointId();
      if( aVtkId >= 0 && mySelector->IsValid( this, aVtkId, true ) ) {
	int anObjId = GetNodeObjId( aVtkId );
	if( anObjId >= 0 ) {
	  if( mySelector->IsSelected( myIO ) )
	    mySelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	  else {
	    if( !isShift ) {
	      mySelector->ClearIObjects();
	    }
	    mySelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	    mySelector->AddIObject( this );
	  }
	}
      }
      break;
    }
    case CellSelection: 
    case EdgeSelection:
    case FaceSelection:
    case VolumeSelection: 
    {
      myCellPicker->Pick( x, y, z, aRenderer );
    
      int aVtkId = myCellPicker->GetCellId();
      if( aVtkId >= 0 && mySelector->IsValid( this, aVtkId ) ) {
	int anObjId = GetElemObjId( aVtkId );
	if( anObjId >= 0 ) {
	  if ( CheckDimensionId(aSelectionMode,this,anObjId) ) {
	    if( mySelector->IsSelected( myIO ) )
	      mySelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	    else {
	      if( !isShift ) {
		mySelector->ClearIObjects();
	      }
	      mySelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	      mySelector->AddIObject( this );
	    }
	  }
	}
      }
      break;
    }
    case EdgeOfCellSelection: 
    {
      myCellPicker->Pick( x, y, z, aRenderer );
    
      int aVtkId = myCellPicker->GetCellId();
      if( aVtkId >= 0 && mySelector->IsValid( this, aVtkId ) ) {
	int anObjId = GetElemObjId( aVtkId );
	if( anObjId >= 0 ) {
	  if( !isShift ) {
	    mySelector->ClearIObjects();
	  }
	  int anEdgeId = GetEdgeId(this,myCellPicker.GetPointer(),anObjId);
	  if( anEdgeId >= 0 ) {
	    mySelector->AddOrRemoveIndex( myIO, anObjId, false );
	    mySelector->AddOrRemoveIndex( myIO, -anEdgeId-1, true );
	    mySelector->AddIObject( this );
	  } 
	}
      }
      break;
    }
    case ActorSelection : 
    {
      if( mySelector->IsSelected( myIO ) && isShift )
	mySelector->RemoveIObject( this );
      else {
	if( !isShift ) {
	  mySelector->ClearIObjects();
	}
	mySelector->AddIObject( this );
      }
      break;
    }
    default:
      break;
    }
  }else{
    float xLast = theSelectionEvent->myLastX;
    float yLast = theSelectionEvent->myLastY;
    float zLast = 0.0;

    float x1 = x < xLast ? x : xLast;
    float y1 = y < yLast ? y : yLast;
    float z1 = z < zLast ? z : zLast;
    float x2 = x > xLast ? x : xLast;
    float y2 = y > yLast ? y : yLast;
    float z2 = z > zLast ? z : zLast;

    switch(aSelectionMode){
    case NodeSelection: {
      if( vtkDataSet* aDataSet = GetInput() ) {
	TColStd_MapOfInteger anIndices;
	for( int i = 0; i < aDataSet->GetNumberOfPoints(); i++)	{
	  float aPoint[3];
	  aDataSet->GetPoint( i, aPoint );

	  float aPnt[3];
	  aRenderer->SetWorldPoint( aPoint[0], aPoint[1], aPoint[2], 1.0 );
	  aRenderer->WorldToDisplay();
	  aRenderer->GetDisplayPoint( aPnt );

	  if( aPnt[0] > x1 && aPnt[0] < x2 && aPnt[1] > y1 && aPnt[1] < y2 ) {
	    float aDisp[3];
	    aRenderer->SetWorldPoint( aPoint[0], aPoint[1], aPoint[2], 1.0 );
	    aRenderer->WorldToDisplay();
	    aRenderer->GetDisplayPoint( aDisp );

	    if( myPointPicker->Pick( aDisp[0], aDisp[1], 0.0, aRenderer ) ) {
	      if( vtkActorCollection* anActorCollection = myPointPicker->GetActors() ) {
		if( anActorCollection->IsItemPresent( this ) ) {
		  float aPickedPoint[3];
		  myPointPicker->GetMapperPosition( aPickedPoint );
		  vtkIdType aVtkId = aDataSet->FindPoint( aPickedPoint );
		  if( aVtkId >= 0 && mySelector->IsValid( this, aVtkId, true ) ) {
		    int anObjId = GetNodeObjId( aVtkId );
		    anIndices.Add( anObjId );
		  }
		}
	      }
	    }
	  }
	}
	if( !anIndices.IsEmpty() ) {
	  mySelector->AddOrRemoveIndex( myIO, anIndices, true );
	  mySelector->AddIObject( this );
	  anIndices.Clear();
	}
	else
	  mySelector->RemoveIObject( this );
      }
      break;
    }
    case ActorSelection :
    {
      float aPnt[3];
      float* aBounds = GetBounds();

      bool picked = true;
      for( int i = 0; i <= 1; i++ ) {
	for( int j = 2; j <= 3; j++ ) {
	  for( int k = 4; k <= 5; k++ ) {
	    aRenderer->SetWorldPoint( aBounds[ i ], aBounds[ j ], aBounds[ k ], 1.0 );
	    aRenderer->WorldToDisplay();
	    aRenderer->GetDisplayPoint( aPnt );

	    if( aPnt[0] < x1 || aPnt[0] > x2 || aPnt[1] < y1 || aPnt[1] > y2 ) {
	      picked = false;
	      break;
	    }
	  }
	}
      }

      if( picked )
	mySelector->AddIObject(this);

      break;
    }
    case CellSelection: 
    case EdgeSelection:
    case FaceSelection:
    case VolumeSelection: 
    {
      myCellRectPicker->SetTolerance( 0.001 );
      myCellRectPicker->Pick( x1, y1, z1, x2, y2, z2, aRenderer );

      VTKViewer_CellDataSet aCellList = myCellRectPicker->GetCellData( this );
      TColStd_MapOfInteger anIndexes;
      if( !aCellList.empty() ) {
	VTKViewer_CellDataSet::iterator anIter = aCellList.begin();
	for(; anIter != aCellList.end(); ++anIter ) {
	  int aCellId = anIter->cellId;
	  if ( !mySelector->IsValid( this, aCellId ) )
	    continue;

	  int anObjId = GetElemObjId( aCellId );
	  if( anObjId != -1 )
	    if ( CheckDimensionId(aSelectionMode,this,anObjId) ) {
	      anIndexes.Add(anObjId);
	    }
	}
      }
      mySelector->AddOrRemoveIndex( myIO, anIndexes, true );
      mySelector->AddIObject( this );
    }
    default:
      break;
    }
  }

  mySelectionMode = aSelectionMode;

  return true;
}

//----------------------------------------------------------------------------
void
SALOME_Actor
::SetPointPicker(vtkPointPicker* thePointPicker) 
{
  myPointPicker = thePointPicker;
}

void
SALOME_Actor
::SetCellPicker(vtkCellPicker* theCellPicker) 
{
  myCellPicker = theCellPicker;
}

void
SALOME_Actor
::SetCellRectPicker(VTKViewer_CellRectPicker* theCellRectPicker) 
{
  myCellRectPicker = theCellRectPicker;
}

//----------------------------------------------------------------------------
void
SALOME_Actor
::SetPreHighlightProperty(vtkProperty* theProperty) 
{
  myPreHighlightActor->SetProperty(theProperty);
}

void
SALOME_Actor
::SetHighlightProperty(vtkProperty* theProperty) 
{
  myHighlightActor->SetProperty(theProperty);
}
