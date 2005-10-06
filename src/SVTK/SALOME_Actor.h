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

#include "SALOME_InteractiveObject.hxx"

// undefining min and max because CASCADE's defines them and
// it clashes with std::min(), std::max()
#undef min
#undef max

#include "VTKViewer_Actor.h"

#include <string>
#include <vector>

#include <vtkSmartPointer.h>

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
class vtkInteractorStyle;
class vtkRenderWindowInteractor;

class VTKViewer_Transform;
class VTKViewer_GeometryFilter;
class VTKViewer_TransformFilter;
class VTKViewer_PassThroughFilter;
class VTKViewer_CellRectPicker;

class SVTK_Actor;
class SVTK_InteractorStyle;

extern int SALOME_POINT_SIZE;
extern int SALOME_LINE_WIDTH;

class SALOME_Actor;

class SVTK_EXPORT SALOME_Actor : public VTKViewer_Actor 
{
 public:
  static SALOME_Actor* New();
  
  vtkTypeMacro(SALOME_Actor,vtkLODActor);

  //----------------------------------------------------------------------------
  virtual
  Standard_Boolean 
  hasIO();

  virtual 
  const Handle(SALOME_InteractiveObject)& 
  getIO(); 

  virtual
  void
  setIO(const Handle(SALOME_InteractiveObject)& theIO);

  virtual 
  const char* 
  getName();

  virtual
  void
  setName(const char* theName);

  //----------------------------------------------------------------------------
  // To generate highlight automaticaly
  virtual
  bool
  hasHighlight(); 

  virtual
  void
  highlight(bool theHighlight, 
	    Selection_Mode theSelectionMode);

  virtual
  void
  highlight(bool theHighlight, 
	    SVTK_Selector* theSelector);

  virtual
  void
  highlight(bool theHighlight);  

  virtual
  bool
  isHighlighted();

  virtual
  void
  SetPreSelected(bool thePreselect = false);

  //----------------------------------------------------------------------------
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
  SetColor(const float theRGB[3]);

  //----------------------------------------------------------------------------
  // For selection mapping purpose
  virtual
  int 
  GetNodeObjId(int theVtkID);

  virtual
  float*
  GetNodeCoord(int theObjID);

  virtual 
  int
  GetElemObjId(int theVtkID);

  virtual
  vtkCell* 
  GetElemCell(int theObjID);

  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  virtual
  bool
  IsSetCamera() const;

  virtual
  bool
  IsResizable() const;

  virtual
  void
  SetSize( const float );

  virtual
  void 
  SetCamera( vtkCamera* );

  //----------------------------------------------------------------------------
  void
  SetResolveCoincidentTopology(bool theIsResolve);

  void
  SetPolygonOffsetParameters(float factor, float units);

  void
  GetPolygonOffsetParameters(float& factor, float& units);

  virtual
  void
  Render(vtkRenderer *, vtkMapper *);

  //----------------------------------------------------------------------------
  virtual
  float
  GetShrinkFactor();

  virtual
  bool
  IsShrunkable();

  virtual
  bool
  IsShrunk();

  virtual
  void
  SetShrink();

  virtual
  void
  UnShrink();

  //----------------------------------------------------------------------------
  virtual
  void
  SetVisibility( int );

  //----------------------------------------------------------------------------
  virtual
  void
  AddToRender(vtkRenderer* theRendere); 

  virtual
  void
  RemoveFromRender(vtkRenderer* theRendere);

  vtkRenderer*
  GetRenderer();

  // Used to obtain all dependent actors
  virtual
  void
  GetChildActors(vtkActorCollection*);
  
  //----------------------------------------------------------------------------
  virtual
  void
  SetInteractor(vtkRenderWindowInteractor* theInteractor);

  virtual
  void
  Update();

  //----------------------------------------------------------------------------
  virtual
  bool
  PreHighlight(SVTK_Selector* theSelector, 
	       vtkInteractorStyle* theInteractorStyle, 
	       SVTK_SelectionEvent* theSelectionEvent,
	       bool theIsHighlight);

  virtual 
  bool
  Highlight(SVTK_Selector* theSelector, 
	    vtkInteractorStyle* theInteractorStyle, 
	    SVTK_SelectionEvent* theSelectionEvent,
	    bool theIsHighlight);

  virtual
  void
  SetSelectionProp(const double& theRed = 1, 
		   const double& theGreen = 1,
		   const double& theBlue = 0, 
		   const int& theWidth = 5);

  virtual
  void
  SetPreselectionProp(const double& theRed = 0, 
		      const double& theGreen = 1,
		      const double& theBlue = 1, 
		      const int& theWidth = 5);

  virtual
  void
  SetSelectionTolerance(const double& theTolNodes = 0.025, 
			const double& theTolCell = 0.001);

  vtkSetObjectMacro(PreviewProperty,vtkProperty);

 protected:
  //----------------------------------------------------------------------------
  vtkRenderWindowInteractor* myInteractor;
  vtkRenderer* myRenderer;

  bool myIsResolveCoincidentTopology;
  float myPolygonOffsetFactor;
  float myPolygonOffsetUnits;

  Handle(SALOME_InteractiveObject) myIO;
  std::string myName;

  vtkProperty *PreviewProperty;
  bool myIsPreselected;
  bool myIsHighlighted;
  Selection_Mode mySelectionMode;

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
  //----------------------------------------------------------------------------
  // Highlight/ Prehighlight devices
  vtkSmartPointer<vtkPointPicker> myPointPicker;
  vtkSmartPointer<vtkCellPicker> myCellPicker;
  vtkSmartPointer<VTKViewer_CellRectPicker> myCellRectPicker;

  vtkSmartPointer<vtkProperty> myPreHighlightProperty;
  vtkSmartPointer<SVTK_Actor> myPreHighlightActor;

  vtkSmartPointer<vtkProperty> myHighlightProperty;
  vtkSmartPointer<SVTK_Actor> myHighlightActor;

  vtkSmartPointer<vtkActor> myOutlineActor;
  vtkSmartPointer<vtkOutlineSource> myOutline;
  vtkSmartPointer<vtkPolyDataMapper> myOutlineMapper;
};

#endif // SALOME_ACTOR_H
