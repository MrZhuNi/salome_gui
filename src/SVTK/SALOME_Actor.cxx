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
#include "SVTK_InteractorStyle.h"

// SALOME Includes
//#include "utilities.h"

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

#include <TColStd_MapOfInteger.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>

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
  myPointPicker = vtkPointPicker::New();
  myCellPicker = vtkCellPicker::New();
  myCellRectPicker = VTKViewer_CellRectPicker::New();

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
  myPointPicker->Delete();
  myCellPicker->Delete();
  myCellRectPicker->Delete();
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
  //cout << "SALOME_Actor::SetVisibility " << ( theVisibility ? "on" : "off" ) << endl;
  vtkProp::SetVisibility( theVisibility );

  myHighlightActor->SetVisibility( theVisibility && isHighlighted() );
}

//----------------------------------------------------------------
int SALOME_Actor::GetEdgeId( vtkPicker* thePicker, int theObjId )
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
bool SALOME_Actor::PreHighlight( SVTK_InteractorStyle* theInteractorStyle,
				 SVTK_Selector* theSelector,
				 vtkRenderer* theRenderer,
				 SVTK_SelectionEvent theSelectionEvent,
				 bool theIsHighlight )
{
  Selection_Mode aSelectionMode = theSelectionEvent.SelectionMode;
  float x = theSelectionEvent.X;
  float y = theSelectionEvent.Y;
  float z = 0.0;
  /*
  cout << "SALOME_Actor::PreHighlight" << endl;
  cout << "SelectionMode : ";
  switch( aSelectionMode )
  {
    case NodeSelection: cout << "Nodes" << endl; break;
    case CellSelection: cout << "Cells" << endl; break;
    case ActorSelection: cout << "Actor" << endl; break;
    default: cout << "Other" << endl; break;
  }
  cout << "IsHighlight : " << ( theIsHighlight ? "true" : "false" ) << endl;
  */
  if( !theIsHighlight )
  {
    myPreHighlightActor->SetVisibility( false );

    vtkActorCollection* theActors = theRenderer->GetActors();
    theActors->InitTraversal();
    while( vtkActor *ac = theActors->GetNextActor() )
      if( SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac ) )
	anActor->SetPreSelected( false );

    return false;
  }

  if( aSelectionMode == NodeSelection )
  {
    myPointPicker->Pick( x, y, z, theRenderer );

    int aVtkId = myPointPicker->GetPointId();
    if( aVtkId >= 0 && theInteractorStyle->IsValid( this, aVtkId, true ) && hasIO() )
    {
      int anObjId = GetNodeObjId( aVtkId );
      TColStd_IndexedMapOfInteger aMapIndex;
      aMapIndex.Add( anObjId );
      //cout << "Index : " << anObjId << endl;

      myPreHighlightActor->GetProperty()->SetRepresentationToPoints();
      myPreHighlightActor->SetVisibility( true );
      myPreHighlightActor->MapPoints( this, aMapIndex );
    }
  }
  else if( aSelectionMode == CellSelection )
  {
    myCellPicker->Pick( x, y, z, theRenderer );

    int aVtkId = myCellPicker->GetCellId();
    if ( aVtkId >= 0 && theInteractorStyle->IsValid( this, aVtkId ) && hasIO() )
    {
      int anObjId = GetElemObjId (aVtkId );
      TColStd_IndexedMapOfInteger aMapIndex;
      aMapIndex.Add( anObjId );
      //cout << "Index : " << anObjId << endl;

      myPreHighlightActor->GetProperty()->SetRepresentationToSurface();
      myPreHighlightActor->SetVisibility( true );
      myPreHighlightActor->MapCells( this, aMapIndex );
    }
  }
  else if( aSelectionMode == EdgeOfCellSelection )
  {
    myCellPicker->Pick( x, y, z, theRenderer );

    int aVtkId = myCellPicker->GetCellId();
    if ( aVtkId >= 0 && theInteractorStyle->IsValid( this, aVtkId ) && hasIO() )
    {
      int anObjId = GetElemObjId( aVtkId );
      int anEdgeId = GetEdgeId( myCellPicker, anObjId );
      TColStd_IndexedMapOfInteger aMapIndex;
      aMapIndex.Add( anObjId );
      aMapIndex.Add( anEdgeId );
      //cout << "Index : " << anObjId << " " << anEdgeId << endl;

      myPreHighlightActor->GetProperty()->SetRepresentationToWireframe();
      myPreHighlightActor->SetVisibility( true );
      myPreHighlightActor->MapEdge( this, aMapIndex );
    }
  }
  else
  {
    if( hasIO() && !theSelector->IsSelected( myIO ) )
    {
      vtkActorCollection* theActors = theRenderer->GetActors();
      theActors->InitTraversal();
      while( vtkActor *ac = theActors->GetNextActor() )
      {
	if( SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac ) )
	  if( anActor->hasIO() && myIO->isSame( anActor->getIO() ) )
	    anActor->SetPreSelected( true );
      }
    }
  }

  return true;
}

//----------------------------------------------------------------
bool SALOME_Actor::Highlight( SVTK_InteractorStyle* theInteractorStyle,
			      SVTK_Selector* theSelector,
			      vtkRenderer* theRenderer,
			      SVTK_SelectionEvent theSelectionEvent,
			      bool theIsHighlight )
{
  Selection_Mode aSelectionMode = theSelectionEvent.SelectionMode;
  float x1 = theSelectionEvent.X;
  float y1 = theSelectionEvent.Y;
  float z1 = 0.0;
  float x2 = theSelectionEvent.LastX;
  float y2 = theSelectionEvent.LastY;
  float z2 = 0.0;
  bool isShift = theSelectionEvent.IsShift;
  bool isRectangle = theSelectionEvent.IsRectangle;

  if( !isRectangle )
  {
    if( aSelectionMode == NodeSelection )
    {
      myPointPicker->Pick( x1, y1, z1, theRenderer );

      int aVtkId = myPointPicker->GetPointId();
      if( aVtkId >= 0 && hasIO() && theInteractorStyle->IsValid( this, aVtkId, true ) )
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
	      theInteractorStyle->HighlightProp( NULL );
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
      myCellPicker->Pick( x1, y1, z1, theRenderer );
    
      int aVtkId = myCellPicker->GetCellId();
      if( aVtkId >= 0 && hasIO() && theInteractorStyle->IsValid( this, aVtkId ) )
      {
	int anObjId = GetElemObjId( aVtkId );
	if( anObjId >= 0 )
	{
	  //if( theInteractorStyle->CheckDimensionId( aSelectionMode, this, anObjId ) )
	  {
	    if( theSelector->IsSelected( myIO ) )
	      theSelector->AddOrRemoveIndex( myIO, anObjId, isShift );
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
    }
    else if( aSelectionMode == EdgeOfCellSelection )
    {
      myCellPicker->Pick( x1, y1, z1, theRenderer );
    
      int aVtkId = myCellPicker->GetCellId();
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
	  int anEdgeId = 0;//theInteractorStyle->GetEdgeId( myCellPicker, this, anObjId );
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
	//theInteractorStyle->PropPicked++;
	if( theSelector->IsSelected( myIO ) && isShift )
	  theSelector->RemoveIObject( this );
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
	  theRenderer->SetWorldPoint( aPoint[0], aPoint[1], aPoint[2], 1.0 );
	  theRenderer->WorldToDisplay();
	  theRenderer->GetDisplayPoint( aPnt );

	  if( aPnt[0] > x2 && aPnt[0] < x1 && aPnt[1] > y1 && aPnt[1] < y2 )
	  //if( theInteractorStyle->IsInRect( aPoint, x2, y2, x1, y1 ) )
	  {
	    float aDisp[3];
	    //theInteractorStyle->ComputeWorldToDisplay( aPoint[0], aPoint[1] ,aPoint[2], aDisp );
	    theRenderer->SetWorldPoint( aPoint[0], aPoint[1], aPoint[2], 1.0 );
	    theRenderer->WorldToDisplay();
	    theRenderer->GetDisplayPoint( aDisp );

	    if( myPointPicker->Pick( aDisp[0], aDisp[1], 0.0, theRenderer ) )
	    {
	      if( vtkActorCollection* anActorCollection = myPointPicker->GetActors() )
	      {
		if( anActorCollection->IsItemPresent( this ) ) 
		{
		  float aPickedPoint[3];
		  myPointPicker->GetMapperPosition( aPickedPoint );
		  vtkIdType aVtkId = aDataSet->FindPoint( aPickedPoint );
		  if( aVtkId >= 0 && theInteractorStyle->IsValid( this, aVtkId, true ) )
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
      myCellRectPicker->Pick( x2, y2, z2, x1, y1, z1, theRenderer );

      VTKViewer_CellDataSet cellList = myCellRectPicker->GetCellData( this );
      TColStd_MapOfInteger anIndexes;
      if( !cellList.empty() )
      {
	VTKViewer_CellDataSet::iterator it;
	for( it = cellList.begin(); it != cellList.end(); ++it )
	{
	  int aCellId = (*it).cellId;
	  if ( !theInteractorStyle->IsValid( this, aCellId ) )
	    continue;

	  int anObjId = GetElemObjId( aCellId );
	  if( anObjId != -1 )
	  {
	    //if( CheckDimensionId( aSelectionMode, this ,anObjId ) )
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
  cout << "IsRectangle : " << ( isRectangle ? "true" : "false" ) << endl;
  cout << "IsHighlight : " << ( theIsHighlight ? "true" : "false" ) << endl;
  */

  if( GetVisibility() && theIsHighlight )
    theInteractorStyle->HighlightProp( this );
  else if( !theIsHighlight )
    theInteractorStyle->HighlightProp( NULL );

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

  return true;
}

/*
/// Checks: is the given Actor within display coordinates?
bool
SVTK_InteractorStyle
::IsInRect(vtkActor* theActor, 
	   const int left, const int top, 
	   const int right, const int bottom)
{
  float* aBounds = theActor->GetBounds();
  float aMin[3], aMax[3];
  ComputeWorldToDisplay(aBounds[0], aBounds[2], aBounds[4], aMin);
  ComputeWorldToDisplay(aBounds[1], aBounds[3], aBounds[5], aMax);
  if (aMin[0] > aMax[0]) {
    float aBuf = aMin[0];
    aMin[0] = aMax[0];
    aMax[0] = aBuf;
  }
  if (aMin[1] > aMax[1]) {
    float aBuf = aMin[1];
    aMin[1] = aMax[1];
    aMax[1] = aBuf;    
  }

  return ((aMin[0]>left) && (aMax[0]<right) && (aMin[1]>bottom) && (aMax[1]<top));
}


/// Checks: is the given Cell within display coordinates?
bool
SVTK_InteractorStyle
::IsInRect(vtkCell* theCell, 
	   const int left, const int top, 
	   const int right, const int bottom)
{
  float* aBounds = theCell->GetBounds();
  float aMin[3], aMax[3];
  ComputeWorldToDisplay(aBounds[0], aBounds[2], aBounds[4], aMin);
  ComputeWorldToDisplay(aBounds[1], aBounds[3], aBounds[5], aMax);
  if (aMin[0] > aMax[0]) {
    float aBuf = aMin[0];
    aMin[0] = aMax[0];
    aMax[0] = aBuf;
  }
  if (aMin[1] > aMax[1]) {
    float aBuf = aMin[1];
    aMin[1] = aMax[1];
    aMax[1] = aBuf;    
  }

  return ((aMin[0]>left) && (aMax[0]<right) && (aMin[1]>bottom) && (aMax[1]<top));
}


bool
SVTK_InteractorStyle
::IsInRect(float* thePoint, 
	   const int left, const int top, 
	   const int right, const int bottom)
{
  float aPnt[3];
  ComputeWorldToDisplay(thePoint[0], thePoint[1], thePoint[2], aPnt);

  return ((aPnt[0]>left) && (aPnt[0]<right) && (aPnt[1]>bottom) && (aPnt[1]<top));
}
*/
