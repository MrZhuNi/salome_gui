/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME VTKViewer_MergeFilter - extract separate components of data from different datasets
// .SECTION Description
// VTKViewer_MergeFilter is a filter that extracts separate components of data from
// different datasets and merges them into a single dataset. The output from
// this filter is of the same type as the input (i.e., vtkDataSet.) It treats 
// both cell and point data set attributes.

// RKV: The source is vtkMergeFilter.
// RKV: Fixed to be able to pass through celldata arrays given by AddField()

#ifndef __VTKViewer_MergeFilter_h
#define __VTKViewer_MergeFilter_h

#include "VTKViewer.h" // RKV
#include "vtkDataSetToDataSetFilter.h"

class vtkFieldList;

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


