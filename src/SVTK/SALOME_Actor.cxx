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

#include <TColStd_MapOfInteger.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>

#include "utilities.h"

using namespace std;

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

int SALOME_POINT_SIZE = 3;


//----------------------------------------------------------------------------
vtkStandardNewMacro(SALOME_Actor);


//----------------------------------------------------------------------------
SALOME_Actor
::SALOME_Actor()
{
  mySelectionMode = ActorSelection;
  myIsHighlighted = myIsPreselected = false;

  myRepresentation = 1;
  myDisplayMode = myRepresentation - 1;

  myProperty = vtkProperty::New();
  PreviewProperty = NULL;

  myIsInfinite = false;

  myIsResolveCoincidentTopology = true;

  vtkMapper::GetResolveCoincidentTopologyPolygonOffsetParameters(myPolygonOffsetFactor,
								 myPolygonOffsetUnits);
  myStoreMapping = false;
  myGeomFilter = VTKViewer_GeometryFilter::New();

  myTransformFilter = VTKViewer_TransformFilter::New();

  for(int i = 0; i < 6; i++)
    myPassFilter.push_back(VTKViewer_PassThroughFilter::New());

  // from VISU
  myPointPicker = vtkPointPicker::New();
  myCellPicker = vtkCellPicker::New();
  myCellRectPicker = VTKViewer_CellRectPicker::New();

  myPreHighlightActor = SVTK_Actor::New(); 
  myPreHighlightActor->GetProperty()->SetColor(0,1,1);
  myPreHighlightActor->GetProperty()->SetPointSize(5);
  myPreHighlightActor->GetProperty()->SetRepresentationToPoints();

  myPointProperty = vtkProperty::New();
  myPointProperty->SetColor(1,1,0);
  myPointProperty->SetPointSize(5);
  myPointProperty->SetRepresentationToPoints();

  myCellProperty = vtkProperty::New();
  myCellProperty->SetColor(1,1,0);
  myCellProperty->SetPointSize(5);
  myCellProperty->SetRepresentationToSurface();

  myEdgeProperty = vtkProperty::New();
  myEdgeProperty->SetColor(1,0,0);
  myEdgeProperty->SetPointSize(5);
  myEdgeProperty->SetRepresentationToWireframe();

  myHighlightActor = SVTK_Actor::New(); 
  myHighlightActor->PickableOff();
  myHighlightActor->SetProperty( myPointProperty );


  myOutline = vtkOutlineSource::New();

  myOutlineMapper = vtkPolyDataMapper::New();
  myOutlineMapper->SetInput( myOutline->GetOutput() );

  myOutlineActor = vtkActor::New();
  myOutlineActor->PickableOff();
  myOutlineActor->DragableOff();
  myOutlineActor->SetMapper( myOutlineMapper );
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

  // from VISU
  myPointPicker->Delete();
  myCellPicker->Delete();
  myCellRectPicker->Delete();
  myHighlightActor->Delete();

  myOutline->Delete();
  myOutlineMapper->Delete();
  myOutlineActor->Delete();
}


//----------------------------------------------------------------------------
void
SALOME_Actor
::AddToRender(vtkRenderer* theRenderer)
{
  theRenderer->AddActor(this);

  // from VISU
  theRenderer->AddActor( myPreHighlightActor );
  theRenderer->AddActor( myHighlightActor );
  theRenderer->AddActor( myOutlineActor );
}

void 
SALOME_Actor
::RemoveFromRender(vtkRenderer* theRenderer)
{
  theRenderer->RemoveActor(this);

  // from VISU
  theRenderer->RemoveActor( myPreHighlightActor );
  theRenderer->RemoveActor( myHighlightActor );
  theRenderer->RemoveActor( myOutlineActor );
}


//----------------------------------------------------------------------------
void
SALOME_Actor
::SetTransform(VTKViewer_Transform* theTransform)
{
  myTransformFilter->SetTransform(theTransform);
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
  vtkLODActor::SetMapper(theMapper);
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
    vtkLODActor::Render(ren,m);
    
    vtkMapper::SetResolveCoincidentTopologyPolygonOffsetParameters(aFactor,aUnit);
    vtkMapper::SetResolveCoincidentTopology(aResolveCoincidentTopology);
  }else{
    vtkLODActor::Render(ren,m);
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
SALOME_Actor::GetPolygonOffsetParameters(float& factor, float& units)
{
  factor = myPolygonOffsetFactor;
  units = myPolygonOffsetUnits;
}


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
    GetProperty()->SetRepresentation(1);
    break;
  case VTK_POINTS : 
    GetProperty()->SetPointSize(SALOME_POINT_SIZE);  
  default :
    GetProperty()->SetRepresentation(theMode);
    myGeomFilter->SetInside(false);
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
vtkCell* 
SALOME_Actor
::GetElemCell(int theObjID)
{
  return GetInput()->GetCell(theObjID);
}


float* 
SALOME_Actor::GetNodeCoord(int theObjID)
{
  return GetInput()->GetPoint(theObjID);
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
  if(myIsInfinite){
    Bounds[0] = Bounds[2] = Bounds[4] = VTK_LARGE_FLOAT;
    Bounds[1] = Bounds[3] = Bounds[5] = -VTK_LARGE_FLOAT;
  }
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
  if(myIsInfinite)
    return Bounds;
  else
    return Superclass::GetBounds();
}


void
SALOME_Actor
::GetBounds(float bounds[6])
{
  this->GetBounds();
  for (int i=0; i<6; i++)
    bounds[i] = this->Bounds[i];
}


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
::GetColor(float& r,float& g,float& b)
{
  float aColor[3];
  GetProperty()->GetColor(aColor);
  r = aColor[0];
  g = aColor[1];
  b = aColor[2];
}


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
::highlight( bool theHighlight, int theSelectionMode )
{
  myIsHighlighted = theHighlight; 

  if( !GetVisibility() )
    return;

  myOutline->SetBounds( GetBounds() );
  myOutlineActor->SetVisibility( theHighlight );

  myHighlightActor->SetVisibility( theHighlight && theSelectionMode != ActorSelection );
}

//----------------------------------------------------------------
void
SALOME_Actor
::SetVisibility( int theVisibility )
{
  vtkProp::SetVisibility( theVisibility );

  myOutlineActor->SetVisibility( theVisibility && isHighlighted() );
  myHighlightActor->SetVisibility( theVisibility && isHighlighted() );
  myPreHighlightActor->SetVisibility( theVisibility && myIsPreselected );
}


//----------------------------------------------------------------
int
SALOME_Actor
::GetEdgeId( vtkPicker* thePicker, int theObjId )
{
  int anEdgeId = -1;
  if (vtkCell* aPickedCell = GetElemCell(theObjId)) {
    float aPickPosition[3];
    thePicker->GetPickPosition(aPickPosition);
    float aMinDist = 1000000.0, aDist = 0;
    for (int i = 0, iEnd = aPickedCell->GetNumberOfEdges(); i < iEnd; i++){
      if(vtkLine* aLine = vtkLine::SafeDownCast(aPickedCell->GetEdge(i))){
	int subId;  float pcoords[3], closestPoint[3], weights[3];
	aLine->EvaluatePosition(aPickPosition,closestPoint,subId,pcoords,aDist,weights);
	if (aDist < aMinDist) {
	  aMinDist = aDist;
	  anEdgeId = i;
	}
      }
    }
  }
  return anEdgeId;
}

//----------------------------------------------------------------
bool
SALOME_Actor
::PreHighlight( SVTK_Selector* theSelector,
		//vtkRenderer* theRenderer,
		vtkInteractorStyle *theIS, 
		SVTK_SelectionEvent theSelectionEvent,
		bool theIsHighlight )
{
  vtkRenderer *aRenderer=theIS->GetCurrentRenderer();
  //
  myPreHighlightActor->SetVisibility( false );
    
  bool anIsSelectionModeChanged = (theSelectionEvent.mySelectionMode != mySelectionMode);
  if(!anIsSelectionModeChanged && mySelectionMode == ActorSelection)
    return false;

  mySelectionMode = theSelectionEvent.mySelectionMode;

  float x = theSelectionEvent.myX;
  float y = theSelectionEvent.myY;
  float z = 0.0;


  bool anIsChanged = false;
  if( !theIsHighlight )
  {
    vtkActorCollection* theActors = aRenderer->GetActors();
    theActors->InitTraversal();
    while( vtkActor *ac = theActors->GetNextActor() )
      if( SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac ) )
	if( anActor->hasIO() && myIO->isSame( anActor->getIO() ) )
	  anActor->SetPreSelected( false );

    myIsPreselected = theIsHighlight;
    anIsChanged = true;
  }else{
    switch(mySelectionMode){
    case NodeSelection: 
    {
      myPointPicker->Pick( x, y, z, aRenderer );
      
      int aVtkId = myPointPicker->GetPointId();
      if( aVtkId >= 0 && theSelector->IsValid( this, aVtkId, true ) && hasIO() )
      {
	int anObjId = GetNodeObjId( aVtkId );
	TColStd_IndexedMapOfInteger aMapIndex;
	aMapIndex.Add( anObjId );
	
	myPreHighlightActor->GetProperty()->SetRepresentationToPoints();
	myPreHighlightActor->SetVisibility( true );
	myPreHighlightActor->MapPoints( this, aMapIndex );

	myIsPreselected = theIsHighlight;
	anIsChanged = true;
      }
      break;
    }
    case CellSelection:
    {
      myCellPicker->Pick( x, y, z, aRenderer );
      
      int aVtkId = myCellPicker->GetCellId();
      if ( aVtkId >= 0 && theSelector->IsValid( this, aVtkId ) && hasIO() )
      {
	int anObjId = GetElemObjId (aVtkId );
	TColStd_IndexedMapOfInteger aMapIndex;
	aMapIndex.Add( anObjId );

	myPreHighlightActor->GetProperty()->SetRepresentationToSurface();
	myPreHighlightActor->SetVisibility( true );
	myPreHighlightActor->MapCells( this, aMapIndex );

	myIsPreselected = theIsHighlight;
	anIsChanged = true;
      }
      break;
    }
    case EdgeOfCellSelection:
    {
      myCellPicker->Pick( x, y, z, aRenderer );
      
      int aVtkId = myCellPicker->GetCellId();
      if ( aVtkId >= 0 && theSelector->IsValid( this, aVtkId ) && hasIO() )
      {
	int anObjId = GetElemObjId( aVtkId );
	int anEdgeId = GetEdgeId( myCellPicker, anObjId );
	TColStd_IndexedMapOfInteger aMapIndex;
	aMapIndex.Add( anObjId );
	aMapIndex.Add( anEdgeId );
	
	myPreHighlightActor->GetProperty()->SetRepresentationToWireframe();
	myPreHighlightActor->SetVisibility( true );
	myPreHighlightActor->MapEdge( this, aMapIndex );

	myIsPreselected = theIsHighlight;
	anIsChanged = true;
      }
      break;
    }
    default:
    {
      myPreHighlightActor->SetVisibility( true );

      if( hasIO() && !theSelector->IsSelected( myIO ) )
      {
	vtkActorCollection* theActors = aRenderer->GetActors();
	theActors->InitTraversal();
	while( vtkActor *ac = theActors->GetNextActor() )
        {
	  if( SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac ) )
	    if( anActor->hasIO() && myIO->isSame( anActor->getIO() ) )
	      anActor->SetPreSelected( true );
	}
      }
      myIsPreselected = theIsHighlight;
      anIsChanged = true;
    }
    }
  }

  return anIsChanged;
}
//----------------------------------------------------------------
bool
SALOME_Actor
::Highlight( SVTK_Selector* theSelector,
	     vtkInteractorStyle* theIS,
	     SVTK_SelectionEvent theSelectionEvent,
	     bool theIsHighlight )
{
  vtkRenderer *aRenderer=theIS->GetCurrentRenderer();
  
  //
  int aSelectionMode = theSelectionEvent.mySelectionMode;
  float x1 = theSelectionEvent.myX;
  float y1 = theSelectionEvent.myY;
  float z1 = 0.0;
  float x2 = theSelectionEvent.myLastX;
  float y2 = theSelectionEvent.myLastY;
  float z2 = 0.0;
  bool isShift = theSelectionEvent.myIsShift;
  bool isRectangle = theSelectionEvent.myIsRectangle;

  if( !isRectangle )
  {
    if( aSelectionMode == NodeSelection )
    {
      myPointPicker->Pick( x1, y1, z1, aRenderer );

      int aVtkId = myPointPicker->GetPointId();
      if( aVtkId >= 0 && hasIO() && theSelector->IsValid( this, aVtkId, true ) )
      {
	int anObjId = GetNodeObjId( aVtkId );
	if( anObjId >= 0 )
	{
	  if( theSelector->IsSelected( myIO ) )
	    theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	  else
	  {
	    if( !isShift )
	    {
	      theSelector->ClearIObjects();
	    }
	    theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	    theSelector->AddIObject( this );
	  }
	}
      }
    }
    else if( aSelectionMode == CellSelection )
    {
      myCellPicker->Pick( x1, y1, z1, aRenderer );
    
      int aVtkId = myCellPicker->GetCellId();
      if( aVtkId >= 0 && hasIO() && theSelector->IsValid( this, aVtkId ) )
      {
	int anObjId = GetElemObjId( aVtkId );
	if( anObjId >= 0 )
	{
	  {
	    if( theSelector->IsSelected( myIO ) )
	      theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	    else
	    {
	      if( !isShift )
	      {
		theSelector->ClearIObjects();
	      }
	      theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	      theSelector->AddIObject( this );
	    }
	  }
	}
      }
    }
    else if( aSelectionMode == EdgeOfCellSelection )
    {
      myCellPicker->Pick( x1, y1, z1, aRenderer );
    
      int aVtkId = myCellPicker->GetCellId();
      if( aVtkId >= 0 && hasIO() && theSelector->IsValid( this, aVtkId ) )
      {
	int anObjId = GetElemObjId( aVtkId );
	if( anObjId >= 0 )
        {
	  if( !isShift )
	  {
	    theSelector->ClearIObjects();
	  }
	  int anEdgeId = GetEdgeId( myCellPicker, anObjId );
	  if( anEdgeId >= 0 )
	  {
	    theSelector->AddOrRemoveIndex( myIO, anObjId, false );
	    theSelector->AddOrRemoveIndex( myIO, -anEdgeId-1, true );
	    theSelector->AddIObject( this );
	  } 
	}
      }
    }
    else if( aSelectionMode == ActorSelection )
    {
      if ( hasIO() )
      {
	if( theSelector->IsSelected( myIO ) && isShift )
	  theSelector->RemoveIObject( this );
	else
	{
	  if( !isShift )
	  {
	    theSelector->ClearIObjects();
	  }
	  theSelector->AddIObject( this );
	}
      }
    }
  }
  else if( isRectangle )
  {
    if( aSelectionMode == NodeSelection && hasIO() && !myIO.IsNull() )
    {
      if( vtkDataSet* aDataSet = GetInput() )
      {
	TColStd_MapOfInteger anIndices;
	for( int i = 0; i < aDataSet->GetNumberOfPoints(); i++)
	{
	  float aPoint[3];
	  aDataSet->GetPoint( i, aPoint );

	  float aPnt[3];
	  aRenderer->SetWorldPoint( aPoint[0], aPoint[1], aPoint[2], 1.0 );
	  aRenderer->WorldToDisplay();
	  aRenderer->GetDisplayPoint( aPnt );

	  if( aPnt[0] > x2 && aPnt[0] < x1 && aPnt[1] > y1 && aPnt[1] < y2 )
	  {
	    float aDisp[3];
	    aRenderer->SetWorldPoint( aPoint[0], aPoint[1], aPoint[2], 1.0 );
	    aRenderer->WorldToDisplay();
	    aRenderer->GetDisplayPoint( aDisp );

	    if( myPointPicker->Pick( aDisp[0], aDisp[1], 0.0, aRenderer ) )
	    {
	      if( vtkActorCollection* anActorCollection = myPointPicker->GetActors() )
	      {
		if( anActorCollection->IsItemPresent( this ) ) 
		{
		  float aPickedPoint[3];
		  myPointPicker->GetMapperPosition( aPickedPoint );
		  vtkIdType aVtkId = aDataSet->FindPoint( aPickedPoint );
		  if( aVtkId >= 0 && theSelector->IsValid( this, aVtkId, true ) )
		  {
		    int anObjId = GetNodeObjId( aVtkId );
		    anIndices.Add( anObjId );
		  }
		}
	      }
	    }
	  }
	}
	if( !anIndices.IsEmpty() )
	{
	  theSelector->AddOrRemoveIndex( myIO, anIndices, false );
	  theSelector->AddIObject( this );
	  anIndices.Clear();
	}
	else
	  theSelector->RemoveIObject( this );
      }
    }
    else if( aSelectionMode != ActorSelection && hasIO() )
    {
      myCellRectPicker->SetTolerance( 0.001 );
      myCellRectPicker->Pick( x2, y2, z2, x1, y1, z1, aRenderer );

      VTKViewer_CellDataSet cellList = myCellRectPicker->GetCellData( this );
      TColStd_MapOfInteger anIndexes;
      if( !cellList.empty() )
      {
	VTKViewer_CellDataSet::iterator it;
	for( it = cellList.begin(); it != cellList.end(); ++it )
	{
	  int aCellId = (*it).cellId;
	  if ( !theSelector->IsValid( this, aCellId ) )
	    continue;

	  int anObjId = GetElemObjId( aCellId );
	  if( anObjId != -1 )
	  {
	    anIndexes.Add( anObjId );
	  }
	}
      }
      theSelector->AddOrRemoveIndex( myIO, anIndexes, true );
      theSelector->AddIObject( this );
    }
    else if( aSelectionMode == ActorSelection && hasIO() )
    {
      theSelector->AddIObject( this );
    }
  }

  TColStd_IndexedMapOfInteger aMapIndex;
  theSelector->GetIndex( getIO(), aMapIndex );

  switch( aSelectionMode )
  {
    case NodeSelection:
      myHighlightActor->SetProperty( myPointProperty );
      myHighlightActor->MapPoints( this, aMapIndex );
      break;
    case EdgeOfCellSelection:
      myHighlightActor->SetProperty( myEdgeProperty );
      myHighlightActor->MapEdge( this, aMapIndex );
      break;
    case CellSelection:
    case EdgeSelection:
    case FaceSelection:
    case VolumeSelection:
      myHighlightActor->SetProperty( myCellProperty );
      myHighlightActor->MapCells( this, aMapIndex );
      break;
    case ActorSelection:
      break;
  }

  highlight( theIsHighlight, aSelectionMode );

  return true;
}

