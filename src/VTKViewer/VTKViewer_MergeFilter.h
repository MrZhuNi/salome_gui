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
#ifndef __VTKViewer_MergeFilter_h
#define __VTKViewer_MergeFilter_h

#include "VTKViewer.h" // RKV
#include "vtkDataSetToDataSetFilter.h"

class vtkFieldList;

// .NAME VTKViewer_MergeFilter - extract separate components of data from different datasets
// .SECTION Description
// VTKViewer_MergeFilter is a filter that extracts separate components of data from
// different datasets and merges them into a single dataset. The output from
// this filter is of the same type as the input (i.e., vtkDataSet.) It treats 
// both cell and point data set attributes.

// RKV: The source is vtkMergeFilter.
// RKV: Fixed to be able to pass through celldata arrays given by AddField()

//RKV class VTK_GRAPHICS_EXPORT VTKViewer_MergeFilter : public vtkDataSetToDataSetFilter
class VTKVIEWER_EXPORT VTKViewer_MergeFilter : public vtkDataSetToDataSetFilter // RKV
{
public:
  static VTKViewer_MergeFilter *New();
  vtkTypeRevisionMacro(VTKViewer_MergeFilter,vtkDataSetToDataSetFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify object from which to extract geometry information.
  void SetGeometry(vtkDataSet *input) {this->SetInput(input);};
  vtkDataSet *GetGeometry() {return this->GetInput();};

  // Description:
  // Specify object from which to extract scalar information.
  void SetScalars(vtkDataSet *);
  vtkDataSet *GetScalars();

  // Description:
  // Set / get the object from which to extract vector information.
  void SetVectors(vtkDataSet *);
  vtkDataSet *GetVectors();
  
  // Description:
  // Set / get the object from which to extract normal information.
  void SetNormals(vtkDataSet *);
  vtkDataSet *GetNormals();
  
  // Description:
  // Set / get the object from which to extract texture coordinates
  // information.
  void SetTCoords(vtkDataSet *);
  vtkDataSet *GetTCoords();

  // Description:
  // Set / get the object from which to extract tensor data.
  void SetTensors(vtkDataSet *);
  vtkDataSet *GetTensors();

  // Description:
  // Set the object from which to extract a field and the name
  // of the field
  void AddField(const char* name, vtkDataSet* input);

protected:
  VTKViewer_MergeFilter();
  ~VTKViewer_MergeFilter();

  // Usual data generation method
  void Execute();
  void ComputeInputUpdateExtents(vtkDataObject *data);

  vtkFieldList* FieldList;
private:
  VTKViewer_MergeFilter(const VTKViewer_MergeFilter&);  // Not implemented.
  void operator=(const VTKViewer_MergeFilter&);  // Not implemented.
  };

#endif


