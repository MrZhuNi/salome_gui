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

#ifndef SVTK_ACTOR_H
#define SVTK_ACTOR_H

#include <TColStd_IndexedMapOfInteger.hxx>

class vtkRenderer;
class vtkShrinkFilter;
class vtkDataSetMapper;
class vtkUnstructuredGrid;

#include "SVTK.h"
#include "SALOME_Actor.h"

//! This class used for internal SVTK package purpose (highlight and prehighlight)
class SVTK_EXPORT SVTK_Actor : public VTKViewer_Actor
{
public:
  virtual ~SVTK_Actor();

  static SVTK_Actor* New();

  vtkTypeMacro(SVTK_Actor,VTKViewer_Actor);

  void  SetShrinkFactor(float value);
  virtual void SetShrink(); 
  virtual void UnShrink(); 

  //! Allow to recostruct selected cells from source SALOME_Actor and map of subindexes
  void MapCells(SALOME_Actor* theMapActor, 
		const TColStd_IndexedMapOfInteger& theMapIndex);

  //! Allow to recostruct selected points from source SALOME_Actor and map of subindexes
  void MapPoints(SALOME_Actor* theMapActor, 
		 const TColStd_IndexedMapOfInteger& theMapIndex);

  //! Allow to recostruct selected edges from source SALOME_Actor and map of subindexes
  void MapEdge(SALOME_Actor* theMapActor, 
	       const TColStd_IndexedMapOfInteger& theMapIndex);

  const TColStd_IndexedMapOfInteger&
  GetMapIndex() const;

 protected:
  TColStd_IndexedMapOfInteger myMapIndex;

  vtkUnstructuredGrid* myUnstructuredGrid;
  vtkDataSetMapper* myMapper;
  vtkRenderer* myRenderer;

  vtkShrinkFilter* myShrinkFilter;
  bool myIsShrinkable;
  bool myIsShrunk;

  SVTK_Actor();
};

#endif
