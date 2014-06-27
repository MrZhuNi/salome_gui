// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "SALOME_ExtractGeometry.h"

#include <vtkCell.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkIdList.h>
#include <vtkImplicitFunction.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkUnstructuredGrid.h>

#include <vtkInformation.h>
#include <vtkInformationVector.h>

#include <vtkImplicitBoolean.h>
#include <vtkImplicitFunctionCollection.h>

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

#define TOLERANCE 1e-3 // selected empirically

//----------------------------------------------------------------------------
vtkStandardNewMacro(SALOME_ExtractGeometry);


//----------------------------------------------------------------------------
SALOME_ExtractGeometry
::SALOME_ExtractGeometry():
  myStoreMapping(false),
  myIsDoneShallowCopy(false)
{}

SALOME_ExtractGeometry
::~SALOME_ExtractGeometry()
{}


//----------------------------------------------------------------------------
vtkImplicitBoolean* 
SALOME_ExtractGeometry
::GetImplicitBoolean() 
{
  return myImplicitBoolean.GetPointer();
}


void
SALOME_ExtractGeometry
::SetImplicitFunction(vtkImplicitFunction* theImplicitFunction)  
{
  myImplicitBoolean = dynamic_cast<vtkImplicitBoolean*>(theImplicitFunction);
  Superclass::SetImplicitFunction(theImplicitFunction);
}


//----------------------------------------------------------------------------
void 
SALOME_ExtractGeometry
::SetStoreMapping(bool theStoreMapping)
{
  if(myStoreMapping != theStoreMapping){
    myStoreMapping = theStoreMapping;
    Modified();
  }
}

bool 
SALOME_ExtractGeometry
::GetStoreMapping() const
{
  return myStoreMapping;
}


//----------------------------------------------------------------------------
vtkIdType
SALOME_ExtractGeometry
::GetElemVTKId(vtkIdType theID)
{
  if(!myStoreMapping || myIsDoneShallowCopy)
    return theID;

  vtkIdType iEnd = myElemVTK2ObjIds.size();
  for(vtkIdType i = 0; i < iEnd; i++)
    if(myElemVTK2ObjIds[i] == theID)
      return i;

  return -1;
}

vtkIdType
SALOME_ExtractGeometry
::GetNodeVTKId(vtkIdType theID)
{
  if(!myStoreMapping || myIsDoneShallowCopy)
    return theID;

  vtkIdType iEnd = myNodeVTK2ObjIds.size();
  for(vtkIdType i = 0; i < iEnd; i++)
    if(myNodeVTK2ObjIds[i] == theID)
      return i;

  return -1;
}


//----------------------------------------------------------------------------
vtkIdType
SALOME_ExtractGeometry
::GetElemObjId(vtkIdType theVtkID)
{
  if(!myStoreMapping || myIsDoneShallowCopy)
    return theVtkID;

  if(theVtkID < (vtkIdType)myElemVTK2ObjIds.size())
    return myElemVTK2ObjIds[theVtkID];

  return -1;
}


vtkIdType
SALOME_ExtractGeometry
::GetNodeObjId(vtkIdType theVtkID)
{
  if(!myStoreMapping || myIsDoneShallowCopy)
    return theVtkID;

  if(theVtkID < (vtkIdType)myNodeVTK2ObjIds.size())
    return myNodeVTK2ObjIds[theVtkID];

  return -1;
}


//----------------------------------------------------------------------------
int
SALOME_ExtractGeometry
::RequestData(vtkInformation *request,
              vtkInformationVector **inputVector,
              vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and ouptut
  vtkDataSet *input = vtkDataSet::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  myElemVTK2ObjIds.clear();
  myNodeVTK2ObjIds.clear();
  //
  myIsDoneShallowCopy = !this->ImplicitFunction;

  if(!myIsDoneShallowCopy && myImplicitBoolean.GetPointer())
    if(vtkImplicitFunctionCollection* aFunction = myImplicitBoolean->GetFunction())
      myIsDoneShallowCopy = aFunction->GetNumberOfItems() == 0;

  if(myIsDoneShallowCopy){
    output->ShallowCopy(input);
    return 1;
  }
  
  return RequestData2(request,inputVector,outputVector);
}


//----------------------------------------------------------------------------
int
SALOME_ExtractGeometry
::RequestData2(vtkInformation *vtkNotUsed(request),
               vtkInformationVector **inputVector,
               vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and ouptut
  vtkDataSet *input = vtkDataSet::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkIdType ptId, numPts, numCells, i, cellId, newCellId, newId, *pointMap;
  vtkIdList *cellPts;
  vtkCell *cell;
  int numCellPts;
  vtkFloatingPointType *x;
  vtkFloatingPointType multiplier;
  vtkPoints *newPts;
  vtkIdList *newCellPts;
  vtkPointData *pd = input->GetPointData();
  vtkCellData *cd = input->GetCellData();
  vtkPointData *outputPD = output->GetPointData();
  vtkCellData *outputCD = output->GetCellData();
  int npts;
  numCells = input->GetNumberOfCells();
  numPts = input->GetNumberOfPoints();

  if ( ! this->ImplicitFunction )
    {
    vtkErrorMacro(<<"No implicit function specified");
    return 0;
    }

  newCellPts = vtkIdList::New();
  newCellPts->Allocate(VTK_CELL_SIZE);

  if ( this->ExtractInside )
    {
    multiplier = 1.0;
    }
  else
    {
    multiplier = -1.0;
    }

  // Loop over all points determining whether they are inside the
  // implicit function. Copy the points and point data if they are.
  //
  pointMap = new vtkIdType[numPts]; // maps old point ids into new
  for (i=0; i < numPts; i++)
    {
    pointMap[i] = -1;
    }

  output->Allocate(numCells/4); //allocate storage for geometry/topology
  newPts = vtkPoints::New();
  newPts->Allocate(numPts/4,numPts);
  outputPD->CopyAllocate(pd);
  outputCD->CopyAllocate(cd);
  vtkFloatArray *newScalars = NULL;

  if(myStoreMapping){
    myElemVTK2ObjIds.reserve(numCells);
    myNodeVTK2ObjIds.reserve(numPts);
  }

  if ( ! this->ExtractBoundaryCells )
    {
    for ( ptId=0; ptId < numPts; ptId++ )
      {
      x = input->GetPoint(ptId);
      // Value 0.0 below has replaced with TOLERANCE (to extract the points,
      // which have a negative value or a value close enough to zero).
      if ( (this->ImplicitFunction->FunctionValue(x)*multiplier) < TOLERANCE ) // 0.0
        {
        newId = newPts->InsertNextPoint(x);
        pointMap[ptId] = newId;
        if(myStoreMapping)
          myNodeVTK2ObjIds.push_back(ptId);
        outputPD->CopyData(pd,ptId,newId);
        }
      }
    }
  else
    {
    // To extract boundary cells, we have to create supplemental information
    if ( this->ExtractBoundaryCells )
      {
      vtkFloatingPointType val;
      newScalars = vtkFloatArray::New();
      newScalars->SetNumberOfValues(numPts);

      for (ptId=0; ptId < numPts; ptId++ )
        {
        x = input->GetPoint(ptId);
        val = this->ImplicitFunction->FunctionValue(x) * multiplier;
        newScalars->SetValue(ptId, val);
        if ( val < 0.0 )
          {
          newId = newPts->InsertNextPoint(x);
          pointMap[ptId] = newId;
          if(myStoreMapping)
            myNodeVTK2ObjIds.push_back(ptId);
          outputPD->CopyData(pd,ptId,newId);
          }
        }
      }
    }

  // Now loop over all cells to see whether they are inside implicit
  // function (or on boundary if ExtractBoundaryCells is on).
  //
  for (cellId=0; cellId < numCells; cellId++)
    {
    cell = input->GetCell(cellId);
    cellPts = cell->GetPointIds();
    numCellPts = cell->GetNumberOfPoints();

    newCellPts->Reset();
    if ( ! this->ExtractBoundaryCells ) //requires less work
      {
      for ( npts=0, i=0; i < numCellPts; i++, npts++)
        {
        ptId = cellPts->GetId(i);
        if ( pointMap[ptId] < 0 )
          {
          break; //this cell won't be inserted
          }
        else
          {
          newCellPts->InsertId(i,pointMap[ptId]);
          }
        }
      } //if don't want to extract boundary cells

    else //want boundary cells
      {
      for ( npts=0, i=0; i < numCellPts; i++ )
        {
        ptId = cellPts->GetId(i);
        if ( newScalars->GetValue(ptId) <= 0.0 )
          {
          npts++;
          }
        }
      if ( npts > 0 )
        {
        for ( i=0; i < numCellPts; i++ )
          {
          ptId = cellPts->GetId(i);
          if ( pointMap[ptId] < 0 )
            {
            x = input->GetPoint(ptId);
            newId = newPts->InsertNextPoint(x);
            pointMap[ptId] = newId;
            if(myStoreMapping)
              myNodeVTK2ObjIds.push_back(ptId);
            outputPD->CopyData(pd,ptId,newId);
            }
          newCellPts->InsertId(i,pointMap[ptId]);
          }
        }//a boundary or interior cell
      }//if mapping boundary cells

    if ( npts >= numCellPts || (this->ExtractBoundaryCells && npts > 0) )
      {
      newCellId = output->InsertNextCell(cell->GetCellType(),newCellPts);
      if(myStoreMapping)
        myElemVTK2ObjIds.push_back(cellId);
      outputCD->CopyData(cd,cellId,newCellId);
      }
    }//for all cells

  // Update ourselves and release memory
  //
  delete [] pointMap;
  newCellPts->Delete();
  output->SetPoints(newPts);
  newPts->Delete();

  if ( this->ExtractBoundaryCells )
    {
    newScalars->Delete();
    }

  output->Squeeze();

  return 1;
}
