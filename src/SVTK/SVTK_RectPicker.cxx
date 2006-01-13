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
//  File   : SVTK_RectPicker.cxx
//  Author : 
//  Module : SALOME

#include "SVTK_RectPicker.h"

#include <vector>

#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkMatrix4x4.h>
#include <vtkRenderer.h>
#include <vtkDataSet.h>
#include <vtkPoints.h>
#include <vtkCamera.h>
#include <vtkCell.h>

//----------------------------------------------------------------------------
namespace
{
  typedef std::vector<vtkIdType> TVectorIds;

  //----------------------------------------------------------------------------
  void
  SelectVisiblePoints(int theSelection[4],
		      vtkRenderer *theRenderer,
		      vtkDataSet *theInput,
		      TVectorIds& theVectorIds,
		      float theTolerance = 0.001)
  {
    theVectorIds.clear();

    vtkIdType aNumPts = theInput->GetNumberOfPoints();
    if(aNumPts < 1)
      return;
    
    theVectorIds.reserve(aNumPts/2 + 1);

    // Grab the composite perspective transform.  This matrix is used to convert
    // each point to view coordinates.  vtkRenderer provides a WorldToView()
    // method but it computes the composite perspective transform each time
    // WorldToView() is called.  This is expensive, so we get the matrix once
    // and handle the transformation ourselves.
    vtkMatrix4x4 *aMatrix = vtkMatrix4x4::New();
    aMatrix->DeepCopy(theRenderer->GetActiveCamera()->
		      GetCompositePerspectiveTransformMatrix(1,0,1));

    // If we have more than a few query points, we grab the z-buffer for the
    // selection region all at once and probe the resulting array.  When we
    // have just a few points, we perform individual z-buffer queries.
    const int SimpleQueryLimit = 25;
    float *aZPtr = NULL;
    if(aNumPts > SimpleQueryLimit){
      aZPtr = theRenderer->GetRenderWindow()->
	GetZbufferData(theSelection[0], theSelection[2], theSelection[1], theSelection[3]);
    }

    float aX[4] = {1.0, 1.0, 1.0, 1.0};
    for(vtkIdType aPntId = 0; aPntId < aNumPts; aPntId++){
      // perform conversion
      theInput->GetPoint(aPntId,aX);
      float aView[4];
      aMatrix->MultiplyPoint(aX,aView);
      if(aView[3] == 0.0)
	continue;
      theRenderer->SetViewPoint(aView[0]/aView[3], 
				aView[1]/aView[3],
				aView[2]/aView[3]);
      theRenderer->ViewToDisplay();
      float aDX[3];
      theRenderer->GetDisplayPoint(aDX);
      bool anIsVisible = false;

      // check whether visible and in selection window 
      if(aDX[0] >= theSelection[0] && aDX[0] <= theSelection[1] &&
         aDX[1] >= theSelection[2] && aDX[1] <= theSelection[3] )
      {
	float aZ = 0.0;
	if(aNumPts > SimpleQueryLimit){
	  // Access the value from the captured zbuffer.  Note, we only
	  // captured a portion of the zbuffer, so we need to offset dx by
	  // the selection window.
	  aZ = aZPtr[int(aDX[0]) - theSelection[0]
		     + (int(aDX[1]) - theSelection[2])
		     *(theSelection[1] - theSelection[0] + 1)];
        }else{
        aZ = theRenderer->GetZ(int(aDX[0]), 
			       int(aDX[1]));
        }
	float aDiff = fabs(aZ-aDX[2]);
	if(aDiff <= theTolerance)
	  anIsVisible = true;
      }

      if(anIsVisible)
	theVectorIds.push_back(aPntId);
    }//for all points

    aMatrix->Delete();

    if(aZPtr)
      delete [] aZPtr;
  }


  //----------------------------------------------------------------------------
  void
  SelectVisibleCells(int theSelection[4],
		     vtkRenderer *theRenderer,
		     vtkDataSet *theInput,
		     TVectorIds& theVectorIds,
		     float theTolerance = 0.001)
  {
    theVectorIds.clear();

    vtkIdType aNumCells = theInput->GetNumberOfCells();
    if(aNumCells < 1)
      return;
    
    theVectorIds.reserve(aNumCells/2 + 1);

    // Grab the composite perspective transform.  This matrix is used to convert
    // each point to view coordinates.  vtkRenderer provides a WorldToView()
    // method but it computes the composite perspective transform each time
    // WorldToView() is called.  This is expensive, so we get the matrix once
    // and handle the transformation ourselves.
    vtkMatrix4x4 *aMatrix = vtkMatrix4x4::New();
    aMatrix->DeepCopy(theRenderer->GetActiveCamera()->
		      GetCompositePerspectiveTransformMatrix(1,0,1));

    // If we have more than a few query points, we grab the z-buffer for the
    // selection region all at once and probe the resulting array.  When we
    // have just a few points, we perform individual z-buffer queries.
    const int SimpleQueryLimit = 25;
    float *aZPtr = NULL;
    if(aNumCells > SimpleQueryLimit){
      aZPtr = theRenderer->GetRenderWindow()->
	GetZbufferData(theSelection[0], theSelection[2], theSelection[1], theSelection[3]);
    }

    float aX[4] = {1.0, 1.0, 1.0, 1.0};
    for(vtkIdType aCellId = 0; aCellId < aNumCells; aCellId++){
      bool anIsVisible = true;
      vtkCell* aCell = theInput->GetCell(aCellId);
      vtkIdType aNumPts = aCell->GetNumberOfPoints();
      for(vtkIdType anId = 0; anId < aNumPts; anId++){
	// perform conversion
	vtkIdType aPntId = aCell->GetPointId(anId);
	theInput->GetPoint(aPntId,aX);
	float aView[4];
	aMatrix->MultiplyPoint(aX,aView);
	if(aView[3] == 0.0)
	  continue;
	theRenderer->SetViewPoint(aView[0]/aView[3], 
				  aView[1]/aView[3],
				  aView[2]/aView[3]);
	theRenderer->ViewToDisplay();
	float aDX[3];
	theRenderer->GetDisplayPoint(aDX);

	// check whether visible and in selection window 
	if(aDX[0] >= theSelection[0] && aDX[0] <= theSelection[1] &&
	   aDX[1] >= theSelection[2] && aDX[1] <= theSelection[3])
	{
	  float aZ = 0.0;
	  if(aNumPts > SimpleQueryLimit){
	    // Access the value from the captured zbuffer.  Note, we only
	    // captured a portion of the zbuffer, so we need to offset dx by
	    // the selection window.
	    aZ = aZPtr[int(aDX[0]) - theSelection[0]
		       + (int(aDX[1]) - theSelection[2])
		       *(theSelection[1] - theSelection[0] + 1)];
	  }else{
	    aZ = theRenderer->GetZ(int(aDX[0]), 
				   int(aDX[1]));
	  }
	  float aDiff = fabs(aZ-aDX[2]);
	  if(aDiff > theTolerance){
	    anIsVisible = false;
	    break;
	  }
	}
      }
      if(anIsVisible)
	theVectorIds.push_back(aCellId);
    }//for all parts
    
    aMatrix->Delete();
    
    if(aZPtr)
      delete [] aZPtr;
  }
}

//----------------------------------------------------------------------------
vtkStandardNewMacro(SVTK_RectPicker);

//----------------------------------------------------------------------------
SVTK_RectPicker
::SVTK_RectPicker()
{}

SVTK_RectPicker
::~SVTK_RectPicker()
{}

int
SVTK_RectPicker
::Pick(float, float, float, vtkRenderer*)
{
  return 0;
}

//----------------------------------------------------------------------------
int
SVTK_RectPicker
::Pick(float selectionPt1[3], float selectionPt2[3], vtkRenderer *ren)
{
  return Pick(selectionPt1[0], selectionPt1[1], selectionPt1[2], 
	      selectionPt2[0], selectionPt2[1], selectionPt2[2],
	      ren);
}

//----------------------------------------------------------------------------
int 
SVTK_RectPicker
::Pick(float selectionX1, float selectionY1, float selectionZ1,
       float selectionX2, float selectionY2, float selectionZ2,
       vtkRenderer *renderer)
{
  return 0;
}
