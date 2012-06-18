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
#ifndef VTKVIEWER_GLYPH3D_H
#define VTKVIEWER_GLYPH3D_H

#include "VTKViewer.h"

#include <vtkGlyph3d.h>

class VTKVIEWER_EXPORT VTKViewer_Glyph3D : public vtkGlyph3D
{
public:
  vtkTypeMacro(VTKViewer_Glyph3D,vtkGlyph3D);

  // Description
  // Construct object with scaling on, scaling mode is by scalar value,
  // scale factor = 1.0, the range is (0,1), orient geometry is on, and
  // orientation is by vector. Clamping and indexing are turned off. No
  // initial sources are defined.
  static VTKViewer_Glyph3D *New();

  // Description:
  // Enable/disable the generation of cell data as part of the output.
  // The cell data at each cell will match the point data of the input
  // at the glyphed point.
  vtkSetMacro(FillCellData,int);
  vtkGetMacro(FillCellData,int);
  vtkBooleanMacro(FillCellData,int);

protected:
  VTKViewer_Glyph3D();
  ~VTKViewer_Glyph3D();

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  //virtual int RequestUpdateExtent(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  //virtual int FillInputPortInformation(int, vtkInformation *);

  int FillCellData; // whether to fill output cell data

private:
  VTKViewer_Glyph3D(const VTKViewer_Glyph3D&);  // Not implemented.
  void operator=(const VTKViewer_Glyph3D&);  // Not implemented.
};

#endif
