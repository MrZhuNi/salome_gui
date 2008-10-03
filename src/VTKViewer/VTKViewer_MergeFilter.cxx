// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
#include "VTKViewer_MergeFilter.h"

#include <vtkCellData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkRectilinearGrid.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredPoints.h>
#include <vtkUnstructuredGrid.h>

vtkCxxRevisionMacro(VTKViewer_MergeFilter, "$Revision$");
vtkStandardNewMacro(VTKViewer_MergeFilter);

class vtkFieldNode
{
public:
  vtkFieldNode(const char* name, vtkDataSet* ptr=0)
    {
      int length = static_cast<int>(strlen(name));
      if (length > 0)
        {
        this->Name = new char[length+1];
        strcpy(this->Name, name);
        }
      else
        {
        this->Name = 0;
        }
      this->Ptr = ptr;
      this->Next = 0;
    }
  ~vtkFieldNode()
    {
      delete[] this->Name;
    }

  const char* GetName()
    {
      return Name;
    }
  vtkDataSet* Ptr;
  vtkFieldNode* Next;
private:
  vtkFieldNode(const vtkFieldNode&) {}
  void operator=(const vtkFieldNode&) {}
  char* Name;
};

class vtkFieldList
{
public:
  vtkFieldList()
    {
      this->First = 0;
      this->Last = 0;
    }
  ~vtkFieldList()
    {
      vtkFieldNode* node = this->First;
      vtkFieldNode* next;
      while(node)
        {
        next = node->Next;
        delete node;
        node = next;
        }
    }


  void Add(const char* name, vtkDataSet* ptr)
    {
      vtkFieldNode* newNode = new vtkFieldNode(name, ptr);
      if (!this->First)
        {
        this->First = newNode;
        this->Last = newNode;
        }
      else
        {
        this->Last->Next = newNode;
        this->Last = newNode;
        }
    }

  friend class vtkFieldListIterator;

private:
  vtkFieldNode* First;
  vtkFieldNode* Last;
};

class vtkFieldListIterator
{
public:
  vtkFieldListIterator(vtkFieldList* list)
    {
      this->List = list;
      this->Position = 0;
    }
  void Begin()
    {
      this->Position = this->List->First;
    }
  void Next()
    {
      if (this->Position)
        {
        this->Position = this->Position->Next;
        }
    }
  int End()
    {
      return this->Position ? 0 : 1;
    }
  vtkFieldNode* Get()
    {
      return this->Position;
    }
  
private:
  vtkFieldNode* Position;
  vtkFieldList* List;
};

//------------------------------------------------------------------------------

// Create object with no input or output.
VTKViewer_MergeFilter::VTKViewer_MergeFilter()
{
  this->FieldList = new vtkFieldList;
  this->SetNumberOfInputPorts(6);
}

VTKViewer_MergeFilter::~VTKViewer_MergeFilter()
{
  delete this->FieldList;
}

vtkDataSet* VTKViewer_MergeFilter::GetGeometry()
{
  if (this->GetNumberOfInputConnections(0) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(0, 0));
}

void VTKViewer_MergeFilter::SetScalars(vtkDataSet *input)
{
  this->SetInput(1, input);
}
vtkDataSet *VTKViewer_MergeFilter::GetScalars()
{
  if (this->GetNumberOfInputConnections(1) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(1, 0));
}

void VTKViewer_MergeFilter::SetVectors(vtkDataSet *input)
{
  this->SetInput(2, input);
}
vtkDataSet *VTKViewer_MergeFilter::GetVectors()
{
  if (this->GetNumberOfInputConnections(2) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(2, 0));
}

void VTKViewer_MergeFilter::SetNormals(vtkDataSet *input)
{
  this->SetInput(3, input);
}
vtkDataSet *VTKViewer_MergeFilter::GetNormals()
{
  if (this->GetNumberOfInputConnections(3) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(3, 0));
}

void VTKViewer_MergeFilter::SetTCoords(vtkDataSet *input)
{
  this->SetInput(4, input);
}
vtkDataSet *VTKViewer_MergeFilter::GetTCoords()
{
  if (this->GetNumberOfInputConnections(4) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(4, 0));
}

void VTKViewer_MergeFilter::SetTensors(vtkDataSet *input)
{
  this->SetInput(5, input);
}
vtkDataSet *VTKViewer_MergeFilter::GetTensors()
{
  if (this->GetNumberOfInputConnections(5) < 1)
    {
    return NULL;
    }
  return vtkDataSet::SafeDownCast(
    this->GetExecutive()->GetInputData(5, 0));
}

void VTKViewer_MergeFilter::AddField(const char* name, vtkDataSet* input)
{
  this->FieldList->Add(name, input);
}

int VTKViewer_MergeFilter::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkInformation *scalarsInfo = inputVector[1]->GetInformationObject(0);
  vtkInformation *vectorsInfo = inputVector[2]->GetInformationObject(0);
  vtkInformation *normalsInfo = inputVector[3]->GetInformationObject(0);
  vtkInformation *tCoordsInfo = inputVector[4]->GetInformationObject(0);
  vtkInformation *tensorsInfo = inputVector[5]->GetInformationObject(0);
  
  // get the input and ouptut
  vtkDataSet *input = vtkDataSet::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *output = vtkDataSet::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *scalarsData = 0;
  vtkDataSet *vectorsData = 0;
  vtkDataSet *normalsData = 0;
  vtkDataSet *tCoordsData = 0;
  vtkDataSet *tensorsData = 0;
  if (scalarsInfo)
    {
    scalarsData = vtkDataSet::SafeDownCast(
      scalarsInfo->Get(vtkDataObject::DATA_OBJECT()));
    }
  if (vectorsInfo)
    {
    vectorsData = vtkDataSet::SafeDownCast(
      vectorsInfo->Get(vtkDataObject::DATA_OBJECT()));
    }
  if (normalsInfo)
    {
    normalsData = vtkDataSet::SafeDownCast(
      normalsInfo->Get(vtkDataObject::DATA_OBJECT()));
    }
  if (tCoordsInfo)
    {
    tCoordsData = vtkDataSet::SafeDownCast(
      tCoordsInfo->Get(vtkDataObject::DATA_OBJECT()));
    }
  if (tensorsInfo)
    {
    tensorsData = vtkDataSet::SafeDownCast(
      tensorsInfo->Get(vtkDataObject::DATA_OBJECT()));
    }

  vtkIdType numPts, numScalars=0, numVectors=0, numNormals=0, numTCoords=0;
  vtkIdType numTensors=0;
  vtkIdType numCells, numCellScalars=0, numCellVectors=0, numCellNormals=0;
  vtkIdType numCellTCoords=0, numCellTensors=0;
  vtkPointData *pd;
  vtkDataArray *scalars = NULL;
  vtkDataArray *vectors = NULL;
  vtkDataArray *normals = NULL;
  vtkDataArray *tcoords = NULL;
  vtkDataArray *tensors = NULL;
  vtkCellData *cd;
  vtkDataArray *cellScalars = NULL;
  vtkDataArray *cellVectors = NULL;
  vtkDataArray *cellNormals = NULL;
  vtkDataArray *cellTCoords = NULL;
  vtkDataArray *cellTensors = NULL;
  vtkPointData *outputPD = output->GetPointData();
  vtkCellData *outputCD = output->GetCellData();
  
  vtkDebugMacro(<<"Merging data!");

  // geometry needs to be copied
  output->CopyStructure(input);
  if ( (numPts = input->GetNumberOfPoints()) < 1 )
    {
    vtkWarningMacro(<<"Nothing to merge!");
    }
  numCells = input->GetNumberOfCells();
  
  if ( scalarsData ) 
    {
    pd = scalarsData->GetPointData();
    scalars = pd->GetScalars();
    if ( scalars != NULL )
      {
      numScalars = scalars->GetNumberOfTuples();
      }
    cd = scalarsData->GetCellData();
    cellScalars = cd->GetScalars();
    if ( cellScalars != NULL )
      {
      numCellScalars = cellScalars->GetNumberOfTuples();
      }
    }

  if ( vectorsData ) 
    {
    pd = vectorsData->GetPointData();
    vectors = pd->GetVectors();
    if ( vectors != NULL )
      {
      numVectors= vectors->GetNumberOfTuples();
      }
    cd = vectorsData->GetCellData();
    cellVectors = cd->GetVectors();
    if ( cellVectors != NULL )
      {
      numCellVectors = cellVectors->GetNumberOfTuples();
      }
    }

  if ( normalsData ) 
    {
    pd = normalsData->GetPointData();
    normals = pd->GetNormals();
    if ( normals != NULL )
      {
      numNormals= normals->GetNumberOfTuples();
      }
    cd = normalsData->GetCellData();
    cellNormals = cd->GetNormals();
    if ( cellNormals != NULL )
      {
      numCellNormals = cellNormals->GetNumberOfTuples();
      }
    }

  if ( tCoordsData ) 
    {
    pd = tCoordsData->GetPointData();
    tcoords = pd->GetTCoords();
    if ( tcoords != NULL )
      {
      numTCoords= tcoords->GetNumberOfTuples();
      }
    cd = tCoordsData->GetCellData();
    cellTCoords = cd->GetTCoords();
    if ( cellTCoords != NULL )
      {
      numCellTCoords = cellTCoords->GetNumberOfTuples();
      }
    }

  if ( tensorsData ) 
    {
    pd = tensorsData->GetPointData();
    tensors = pd->GetTensors();
    if ( tensors != NULL )
      {
      numTensors = tensors->GetNumberOfTuples();
      }
    cd = tensorsData->GetCellData();
    cellTensors = cd->GetTensors();
    if ( cellTensors != NULL )
      {
      numCellTensors = cellTensors->GetNumberOfTuples();
      }
    }

  // merge data only if it is consistent
  if ( numPts == numScalars )
    {
    outputPD->SetScalars(scalars);
    }
  if ( numCells == numCellScalars )
    {
    outputCD->SetScalars(cellScalars);
    }

  if ( numPts == numVectors )
    {
    outputPD->SetVectors(vectors);
    }
  if ( numCells == numCellVectors )
    {
    outputCD->SetVectors(cellVectors);
    }
    
  if ( numPts == numNormals )
    {
    outputPD->SetNormals(normals);
    }
  if ( numCells == numCellNormals )
    {
    outputCD->SetNormals(cellNormals);
    }

  if ( numPts == numTCoords )
    {
    outputPD->SetTCoords(tcoords);
    }
  if ( numCells == numCellTCoords )
    {
    outputCD->SetTCoords(cellTCoords);
    }

  if ( numPts == numTensors )
    {
    outputPD->SetTensors(tensors);
    }
  if ( numCells == numCellTensors )
    {
    outputCD->SetTensors(cellTensors);
    }

  vtkFieldListIterator it(this->FieldList);
  vtkDataArray* da;
  const char* name;
  vtkIdType num;
  for(it.Begin(); !it.End() ; it.Next())
    {
    pd = it.Get()->Ptr->GetPointData();
    cd = it.Get()->Ptr->GetCellData();
    name = it.Get()->GetName();
    if ( (da=pd->GetArray(name)) )
      {
      num = da->GetNumberOfTuples();
      if (num == numPts)
        {
        outputPD->AddArray(da);
        }
      }
    if ( (da=cd->GetArray(name)) )
      {
      num = da->GetNumberOfTuples();
// RKV: Fixed to be able to pass through celldata arrays given by AddField()
// RKV      if (num == numPts)
      if (num == numCells) // RKV
        {
        outputCD->AddArray(da);
        }
      }
    }

  return 1;
}

//----------------------------------------------------------------------------
//  Trick:  Abstract data types that may or may not be the same type
// (structured/unstructured), but the points/cells match up.
// Output/Geometry may be structured while ScalarInput may be 
// unstructured (but really have same triagulation/topology as geometry).
// Just request all the input. Always generate all of the output (todo).
int VTKViewer_MergeFilter::RequestUpdateExtent(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *vtkNotUsed(outputVector))
{
  vtkInformation *inputInfo;
  int idx;
  
  for (idx = 0; idx < 6; ++idx)
    {
    inputInfo = inputVector[idx]->GetInformationObject(0);
    if (inputInfo)
      {
      inputInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(),
                     0);
      inputInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(),
                     1);
      inputInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(),
                     0);
      inputInfo->Set(vtkStreamingDemandDrivenPipeline::EXACT_EXTENT(), 1);
      }
    }
  return 1;
}

int VTKViewer_MergeFilter::FillInputPortInformation(int port, vtkInformation *info)
{
  int retval = this->Superclass::FillInputPortInformation(port, info);
  if (port > 0)
    {
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
    }
  return retval;
}

void VTKViewer_MergeFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

}

