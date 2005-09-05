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
//  File   : SALOME_Actor.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SALOME_ACTOR_H
#define SALOME_ACTOR_H

#include "SVTK_SelectionEvent.h"
#include "SVTK_Selector.h"
#include "SVTK.h"

#include "VTKViewer_Actor.h"

#include "SALOME_InteractiveObject.hxx"

#include <string>
#include <vector>

class vtkCell;
class vtkPicker;
class vtkPointPicker;
class vtkCellPicker;
class vtkDataSet;
class vtkPolyData;
class vtkCamera;
class vtkProperty;
class vtkPolyDataMapper;
class vtkOutlineSource;
class vtkRenderer;

class VTKViewer_Transform;
class VTKViewer_GeometryFilter;
class VTKViewer_TransformFilter;
class VTKViewer_PassThroughFilter;
class VTKViewer_CellRectPicker;

class SVTK_Actor;
class SVTK_InteractorStyle;

extern int SALOME_POINT_SIZE;

class SALOME_Actor;

class SVTK_EXPORT SALOME_Actor : public VTKViewer_Actor 
{
 public:
  static SALOME_Actor* New();
  
  vtkTypeMacro(SALOME_Actor,vtkLODActor);

  virtual
  Standard_Boolean 
  hasIO() 
  { 
    return !myIO.IsNull(); 
  }

  virtual 
  const Handle(SALOME_InteractiveObject)& 
  getIO()
  { 
    return myIO; 
  } 

  virtual
  void
  setIO(const Handle(SALOME_InteractiveObject)& theIO) 
  { 
    myIO = theIO; 
  }

  virtual 
  const char* 
  getName() 
  { 
    return myName.c_str(); 
  }

  virtual
  void
  setName(const char* theName)
  {
    if(hasIO())	
      myIO->setName(theName);
    myName = theName;
  }

  // To generate highlight automaticaly
  virtual
  bool
  hasHighlight() 
  { 
    return false; 
  } 

  virtual
  void
  highlight( bool theHighlight, int theSelectionMode );

  virtual
  void
  highlight(bool theHighlight) 
  { 
    highlight(theHighlight,ActorSelection); 
  }  

  virtual
  bool
  isHighlighted() 
  { 
    return myIsHighlighted; 
  }

  virtual
  void
  SetOpacity(float theOpacity);

  virtual
  float 
  GetOpacity();

  virtual
  void
  SetColor(float r,float g,float b);

  virtual
  void
  GetColor(float& r,float& g,float& b);

  virtual
  void
  SetColor(const float theRGB[3])
  { 
    SetColor(theRGB[0],theRGB[1],theRGB[2]);
  }

  vtkSetObjectMacro(PreviewProperty,vtkProperty);

  virtual
  void
  SetPreSelected(bool thePreselect = false) 
  { 
    myIsPreselected = thePreselect;
  }

  // Used to obtain all dependent actors
  virtual
  void
  GetChildActors(vtkActorCollection*) 
  {};
  
  // For selection mapping purpose
  virtual
  int 
  GetNodeObjId(int theVtkID)
  { 
    return theVtkID;
  }

  virtual
  float*
  GetNodeCoord(int theObjID);

  virtual 
  int
  GetElemObjId(int theVtkID) 
  { 
    return theVtkID;
  }

  virtual
  vtkCell* 
  GetElemCell(int theObjID);

  virtual
  int
  GetObjDimension( const int theObjId );

  virtual
  void
  SetMapper(vtkMapper* theMapper); 

  virtual
  vtkDataSet* 
  GetInput(); 

  virtual
  void
  SetTransform(VTKViewer_Transform* theTransform); 

  virtual
  unsigned long int
  GetMTime();

  virtual
  void
  SetRepresentation(int theMode);

  virtual
  int
  GetRepresentation();

  virtual
  int
  getDisplayMode();

  virtual
  void
  setDisplayMode(int theMode);

  // Infinitive means actor without size (point for example),
  // which is not taken into account in calculation of boundaries of the scene
  void
  SetInfinitive(bool theIsInfinite);

  virtual
  bool
  IsInfinitive();
    
  virtual
  float* 
  GetBounds();

  void
  GetBounds(float bounds[6]);

  void
  SetResolveCoincidentTopology(bool theIsResolve);

  void
  SetPolygonOffsetParameters(float factor, float units);

  void
  GetPolygonOffsetParameters(float& factor, float& units);

  virtual
  void
  Render(vtkRenderer *, vtkMapper *);

  virtual
  float
  GetShrinkFactor() 
  { 
    return 1.0;
  }

  virtual
  bool
  IsShrunkable() 
  { 
    return false;
  }

  virtual
  bool
  IsShrunk() 
  { 
    return false;
  }

  virtual
  void
  SetShrink() 
  {} 

  virtual
  void
  UnShrink() 
  {}

  virtual
  bool
  IsSetCamera() const 
  { 
    return false; 
  }

  virtual
  bool
  IsResizable() const 
  { 
    return false; 
  }

  virtual
  void
  SetSize( const float ) 
  {}

  virtual
  void 
  SetCamera( vtkCamera* ) 
  {}

  virtual
  void
  SetVisibility( int );

  virtual
  void
  AddToRender( vtkRenderer* ); 

  virtual
  void
  RemoveFromRender( vtkRenderer* );

  virtual
  bool
  PreHighlight(	SVTK_Selector*, 
		vtkRenderer*, 
		SVTK_SelectionEvent, 
		bool );
  virtual 
  bool
  Highlight( SVTK_Selector*, 
	     vtkRenderer*, 
	     SVTK_SelectionEvent, 
	     bool );

  vtkProperty* 
  getPointProperty()
  { 
    return myPointProperty; 
  }

  vtkProperty* 
  getCellProperty()
  { 
    return myCellProperty; 
  }

  vtkProperty* 
  getEdgeProperty()
  { 
    return myEdgeProperty; 
  }

  vtkPointPicker* 
  getPointPicker() 
  { 
    return myPointPicker; 
  }

  vtkCellPicker* 
  getCellPicker() 
  { 
    return myCellPicker; 
  }

  VTKViewer_CellRectPicker* 
  getCellRectPicker() 
  { 
    return myCellRectPicker; 
  }

 protected:
  bool myIsResolveCoincidentTopology;
  float myPolygonOffsetFactor;
  float myPolygonOffsetUnits;

  Handle(SALOME_InteractiveObject) myIO;
  std::string myName;

  vtkProperty *PreviewProperty;
  bool myIsPreselected;
  bool myIsHighlighted;
  int mySelectionMode;

  float myOpacity;
  int myDisplayMode;
  bool myIsInfinite;

  bool myStoreMapping;
  VTKViewer_GeometryFilter *myGeomFilter;
  VTKViewer_TransformFilter *myTransformFilter;
  std::vector<VTKViewer_PassThroughFilter*> myPassFilter;

  int myRepresentation;
  vtkProperty *myProperty;

  void
  InitPipeLine(vtkMapper* theMapper); 

  SALOME_Actor();
  ~SALOME_Actor();

 protected:
  int GetEdgeId( vtkPicker*, int );

  vtkProperty* myPointProperty;
  vtkProperty* myCellProperty;
  vtkProperty* myEdgeProperty;

  vtkPointPicker* myPointPicker;
  vtkCellPicker* myCellPicker;
  VTKViewer_CellRectPicker* myCellRectPicker;

  SVTK_Actor* myPreHighlightActor;
  SVTK_Actor* myHighlightActor;

  vtkActor* myOutlineActor;
  vtkOutlineSource* myOutline;
  vtkPolyDataMapper* myOutlineMapper;
};

#endif // SALOME_ACTOR_H
