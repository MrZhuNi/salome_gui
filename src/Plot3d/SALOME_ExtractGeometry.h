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

#ifndef SALOME_ExtractGeometry_H
#define SALOME_ExtractGeometry_H

#include "Plot3d.h"

#include <vtkExtractGeometry.h>
#include <vtkSmartPointer.h>

#include <vector>

class vtkImplicitBoolean;

class PLOT3D_EXPORT SALOME_ExtractGeometry : public vtkExtractGeometry
{
public:
  vtkTypeMacro(SALOME_ExtractGeometry, vtkExtractGeometry);

  static 
  SALOME_ExtractGeometry*
  New();

  virtual
  void
  SetImplicitFunction(vtkImplicitFunction* theImplicitFunction); 

  vtkImplicitBoolean* 
  GetImplicitBoolean();

  bool 
  GetStoreMapping() const;

  void
  SetStoreMapping(bool theStoreMapping);

  virtual 
  vtkIdType 
  GetNodeObjId(vtkIdType theID);

  virtual
  vtkIdType
  GetElemObjId(vtkIdType theID);

  virtual
  vtkIdType
  GetNodeVTKId(vtkIdType theID);

  virtual
  vtkIdType
  GetElemVTKId(vtkIdType theID);

protected:
  SALOME_ExtractGeometry();
  ~SALOME_ExtractGeometry();

  // Usual data generation method
  virtual
  int
  RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  virtual
  int
  RequestData2(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  bool myStoreMapping;
  bool myIsDoneShallowCopy;

  typedef std::vector<vtkIdType> TVectorId;
  TVectorId myElemVTK2ObjIds;
  TVectorId myNodeVTK2ObjIds;

  vtkSmartPointer<vtkImplicitBoolean> myImplicitBoolean;

  SALOME_ExtractGeometry(const SALOME_ExtractGeometry&);  // Not implemented.
  void operator=(const SALOME_ExtractGeometry&);  // Not implemented.
};

#endif
