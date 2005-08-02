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
 
#include "SVTK_Actor.h"
#include "SVTK_InteractorStyle.h"

// SALOME Includes
//#include "utilities.h"

// VTK Includes
#include <vtkCell.h>
#include <vtkPicker.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkObjectFactory.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

//using namespace std;


#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

int SALOME_POINT_SIZE = 3;


vtkStandardNewMacro(SALOME_Actor);


SALOME_Actor::SALOME_Actor(){
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
  myPreHighlightActor = SVTK_Actor::New(); 
  myPreHighlightActor->GetProperty()->SetColor(0,1,0);
  myPreHighlightActor->GetProperty()->SetPointSize(15);
  myPreHighlightActor->GetProperty()->SetRepresentationToPoints();

  myPointProperty = vtkProperty::New();
  myPointProperty->SetColor(1,0,1);
  myPointProperty->SetPointSize(15);
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
}


SALOME_Actor::~SALOME_Actor(){
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
  myHighlightActor->SetProperty( NULL );
}


void SALOME_Actor::AddToRender(vtkRenderer* theRenderer){
  theRenderer->AddActor(this);

  // from VISU
  theRenderer->AddActor( myPreHighlightActor );
  theRenderer->AddActor( myHighlightActor );
}

void SALOME_Actor::RemoveFromRender(vtkRenderer* theRenderer){
  theRenderer->RemoveActor(this);

  // from VISU
  theRenderer->RemoveActor( myPreHighlightActor );
  theRenderer->RemoveActor( myHighlightActor );
}


void SALOME_Actor::SetTransform(VTKViewer_Transform* theTransform){
  myTransformFilter->SetTransform(theTransform);
}


void SALOME_Actor::SetMapper(vtkMapper* theMapper){
  InitPipeLine(theMapper);
}

void SALOME_Actor::InitPipeLine(vtkMapper* theMapper){
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


void SALOME_Actor::Render(vtkRenderer *ren, vtkMapper* m){
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


void SALOME_Actor::SetResolveCoincidentTopology(bool theIsResolve) {
  myIsResolveCoincidentTopology = theIsResolve;
}

void SALOME_Actor::SetPolygonOffsetParameters(float factor, float units){
  myPolygonOffsetFactor = factor;
  myPolygonOffsetUnits = units;
}

void SALOME_Actor::GetPolygonOffsetParameters(float& factor, float& units){
  factor = myPolygonOffsetFactor;
  units = myPolygonOffsetUnits;
}


vtkDataSet* SALOME_Actor::GetInput(){
  return myPassFilter.front()->GetOutput();
}


unsigned long int SALOME_Actor::GetMTime(){
  unsigned long mTime = this->Superclass::GetMTime();
  unsigned long time = myTransformFilter->GetMTime();
  mTime = ( time > mTime ? time : mTime );
  if(vtkDataSet *aDataSet = myPassFilter[0]->GetInput()){
    time = aDataSet->GetMTime();
    mTime = ( time > mTime ? time : mTime );
  }
  return mTime;
}


void SALOME_Actor::SetRepresentation(int theMode) { 
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

int SALOME_Actor::GetRepresentation(){ 
  return myRepresentation;
}


vtkCell* SALOME_Actor::GetElemCell(int theObjID){
  return GetInput()->GetCell(theObjID);
}


float* SALOME_Actor::GetNodeCoord(int theObjID){
  return GetInput()->GetPoint(theObjID);
}


//=================================================================================
// function : GetObjDimension
// purpose  : Return object dimension.
//            Virtual method shoulb be redifined by derived classes
//=================================================================================
int SALOME_Actor::GetObjDimension( const int theObjId )
{
  if ( vtkCell* aCell = GetElemCell(theObjId) )
    return aCell->GetCellDimension();
  return 0;
}


bool SALOME_Actor::IsInfinitive(){ 
  return myIsInfinite; 
}


void SALOME_Actor::SetOpacity(float theOpacity){ 
  myOpacity = theOpacity;
  GetProperty()->SetOpacity(theOpacity);
}

float SALOME_Actor::GetOpacity(){
  return myOpacity;
}


void SALOME_Actor::SetColor(float r,float g,float b){
  GetProperty()->SetColor(r,g,b);
}

void SALOME_Actor::GetColor(float& r,float& g,float& b){
  float aColor[3];
  GetProperty()->GetColor(aColor);
  r = aColor[0];
  g = aColor[1];
  b = aColor[2];
}


int SALOME_Actor::getDisplayMode(){ 
  return myDisplayMode; 
}

void SALOME_Actor::setDisplayMode(int theMode){ 
  SetRepresentation(theMode+1); 
  myDisplayMode = GetRepresentation() - 1;
}

// from VISU

//----------------------------------------------------------------
void SALOME_Actor::highlight( bool theHighlight )
{
  //cout << "SALOME_Actor::highlight " << ( theHighlight ? "on" : "off" ) << endl;

  myHighlightActor->SetVisibility( theHighlight );
}

//----------------------------------------------------------------
void SALOME_Actor::SetVisibility( int theVisibility )
{
  vtkProp::SetVisibility( theVisibility );

  myHighlightActor->SetVisibility( theVisibility && isHighlighted() );
}

//----------------------------------------------------------------
bool SALOME_Actor::PreHighlight( SVTK_InteractorStyle* theInteractorStyle,
				 const int& theIndex )
{
  //cout << "SALOME_Actor::PreHighlight : " << theIndex << endl;

  if( theIndex == -1 )
  {
    myPreHighlightActor->SetVisibility( false );
  }
  else
  {
    TColStd_IndexedMapOfInteger aMapIndex;
    aMapIndex.Add( theIndex );

    myPreHighlightActor->SetVisibility( true );
    myPreHighlightActor->MapPoints( this, aMapIndex );
  }

  return false;
}

//----------------------------------------------------------------
bool SALOME_Actor::Highlight( SVTK_InteractorStyle* theInteractorStyle,
			      SVTK_Selector* theSelector,
			      vtkRenderer* theRenderer,
			      SVTK_SelectionEvent theSelectionEvent,
			      bool theIsHighlight,
			      bool theIsUpdate )
{
  Selection_Mode aSelectionMode = theSelectionEvent.SelectionMode;
  float x = theSelectionEvent.X;
  float y = theSelectionEvent.Y;
  float z = 0.0;
  bool isShift = theSelectionEvent.IsShift;

  if( aSelectionMode == NodeSelection )
  {
    //vtkSmartPointer<VTKViewer_CellRectPicker> picker = VTKViewer_CellRectPicker::New();
    vtkPointPicker* aPicker = vtkPointPicker::New();

    aPicker->Pick( x, y, z, theRenderer );

    int aVtkId = aPicker->GetPointId();
    if( aVtkId >= 0 && hasIO() && theInteractorStyle->IsValid( this, aVtkId, true ) )
    {
      int anObjId = GetNodeObjId( aVtkId );
      if( anObjId >= 0 )
      {
	if( theSelector->IsSelected( myIO ) )
	{
	  // This IO is already in the selection
	  theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	}
	else
	{
	  if( !isShift )
	  {
	    theInteractorStyle->HighlightProp( NULL );
	    theSelector->ClearIObjects();
	  }
	  theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	  theSelector->AddIObject( this );
	}
      }
    }
    aPicker->Delete();
  }
  else if( aSelectionMode == CellSelection )
  {
    vtkCellPicker* aPicker = vtkCellPicker::New();

    aPicker->Pick( x, y, z, theRenderer );
    
    int aVtkId = aPicker->GetCellId();
    if( aVtkId >= 0 && hasIO() && theInteractorStyle->IsValid( this, aVtkId ) )
    {
      int anObjId = GetElemObjId( aVtkId );
      if( anObjId >= 0 )
      {
	//if( theInteractorStyle->CheckDimensionId( aSelectionMode, this, anObjId ) )
	{
	  if( theSelector->IsSelected( myIO ) )
	  {
	    // This IO is already in the selection
	    theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	  }
	  else
	  {
	    if( !isShift )
	    {
	      theInteractorStyle->HighlightProp( NULL );
	      theSelector->ClearIObjects();
	    }
	    theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
	    theSelector->AddIObject( this );
	  }
	}
      }
    }
    aPicker->Delete();
  }
  else if( aSelectionMode == EdgeOfCellSelection )
  {
    vtkCellPicker* aPicker = vtkCellPicker::New();

    aPicker->Pick( x, y, z, theRenderer );
    
    int aVtkId = aPicker->GetCellId();
    if( aVtkId >= 0 && hasIO() && theInteractorStyle->IsValid( this, aVtkId ) )
    {
      int anObjId = GetElemObjId( aVtkId );
      if( anObjId >= 0 )
      {
	if( !isShift )
	{
	  theInteractorStyle->HighlightProp( NULL );
	  theSelector->ClearIObjects();
	}
	int anEdgeId = 0;//theInteractorStyle->GetEdgeId( aPicker, this, anObjId );
	if( anEdgeId >= 0 )
	{
	  theSelector->AddOrRemoveIndex( myIO, anObjId, false );
	  theSelector->AddOrRemoveIndex( myIO, -anEdgeId-1, true );
	  theSelector->AddIObject( this );
	} 
      }
    }
    aPicker->Delete();
  }
  else if( aSelectionMode == ActorSelection )
  {
    if ( hasIO() )
    {
      //theInteractorStyle->PropPicked++;
      if( theSelector->IsSelected( myIO ) )
      {
	// This IO is already in the selection
	if( isShift )
	  theSelector->RemoveIObject( this );
      }
      else
      {
	if( !isShift )
	{
	  theInteractorStyle->HighlightProp( NULL );
	  theSelector->ClearIObjects();
	}
	theSelector->AddIObject( this );
      }
    }
  }

  TColStd_IndexedMapOfInteger aMapIndex;
  theSelector->GetIndex( getIO(), aMapIndex );
  /*
  cout << "SALOME_Actor::Highlight : ";
  for( int i = 1, n = aMapIndex.Extent(); i <= n; i++ )
    cout << aMapIndex(i) << " ";
  cout << endl;
  cout << "SelectionMode : ";
  switch( aSelectionMode )
  {
    case NodeSelection: cout << "Nodes" << endl; break;
    case CellSelection: cout << "Cells" << endl; break;
    case ActorSelection: cout << "Actor" << endl; break;
    default: cout << "Other" << endl; break;
  }
  cout << "IsHighlight : " << ( theIsHighlight ? "true" : "false" ) << endl;
  cout << "IsUpdate : " << ( theIsUpdate ? "true" : "false" ) << endl;
  */

  if( GetVisibility() && theIsHighlight )
    theInteractorStyle->HighlightProp( this );
  else if( !theIsHighlight )
    theInteractorStyle->HighlightProp( NULL );

  //if( aSelectionMode != ActorSelection && !hasHighlight() )
  //  return false;

  switch( aSelectionMode )
  {
    case NodeSelection:
      myHighlightActor->SetProperty( myPointProperty );
      myHighlightActor->MapPoints( this, aMapIndex );
      highlight( theIsHighlight );
      break;
    case EdgeOfCellSelection:
      myHighlightActor->SetProperty( myEdgeProperty );
      myHighlightActor->MapEdge( this, aMapIndex );
      highlight( theIsHighlight );
      break;
    case CellSelection:
    case EdgeSelection:
    case FaceSelection:
    case VolumeSelection:
      myHighlightActor->SetProperty( myCellProperty );
      myHighlightActor->MapCells( this, aMapIndex );
      highlight( theIsHighlight );
      break;
    case ActorSelection:
      highlight( false );
      break;
  }

  return false;
}

//----------------------------------------------------------------
bool SALOME_Actor::Unhighlight( SVTK_InteractorStyle* theInteractorStyle,
				 SVTK_Selector* theSelector,
				 bool theIsUpdate )
{

  theInteractorStyle->HighlightProp( NULL );
  theSelector->ClearIObjects();
}
