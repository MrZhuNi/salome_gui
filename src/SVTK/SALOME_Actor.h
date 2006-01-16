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
class vtkOutlineSource;
class vtkRenderer;
class vtkInteractorStyle;
class vtkRenderWindowInteractor;

class VTKViewer_Transform;
class VTKViewer_GeometryFilter;
class VTKViewer_TransformFilter;
class VTKViewer_PassThroughFilter;

class SVTK_Actor;
class SVTK_RectPicker;
class SVTK_InteractorStyle;

extern int SALOME_POINT_SIZE;
extern int SALOME_LINE_WIDTH;

class SALOME_Actor;

//! The class is a basic one for all SALOME VTK presentation.
/*!
  It provide highlight and prehighlight capabilites,
  common way to publish and remove VTK presentation, 
  mapping of VTK and object IDs and so on.
 */
class SVTK_EXPORT SALOME_Actor : public VTKViewer_Actor 
{
 public:
  static SALOME_Actor* New();
  
  vtkTypeMacro(SALOME_Actor,VTKViewer_Actor);

  //----------------------------------------------------------------------------
  //! To check, if the #SALOME_Actor has a reference to #SALOME_InteractiveObject
  virtual
  Standard_Boolean 
  hasIO();

  //! Get correspoinding reference to #SALOME_InteractiveObject
  virtual 
  const Handle(SALOME_InteractiveObject)& 
  getIO(); 

  //! Set reference to #SALOME_InteractiveObject
  virtual
  void
  setIO(const Handle(SALOME_InteractiveObject)& theIO);

  //! Get its name
  virtual 
  const char* 
  getName();

  //! Name the #SALOME_Actor
  virtual
  void
  setName(const char* theName);

  //----------------------------------------------------------------------------
  //! Change opacity
  virtual
  void
  SetOpacity(float theOpacity);

  //! Get current opacity
  virtual
  float 
  GetOpacity();

  //! Change color
  virtual
  void
  SetColor(float r,float g,float b);

  //! Get current color
  virtual
  void
  GetColor(float& r,float& g,float& b);

  //! Change color
  virtual
  void
  SetColor(const float theRGB[3]);

  //----------------------------------------------------------------------------
  // For selection mapping purpose
  //! Maps VTK index of a node to corresponding object index
  virtual
  int 
  GetNodeObjId(int theVtkID);

  //! Get coordinates of a node for given object index
  virtual
  float*
  GetNodeCoord(int theObjID);

  //! Maps VTK index of a cell to corresponding object index
  virtual 
  int
  GetElemObjId(int theVtkID);

  //! Get corresponding #vtkCell for given object index
  virtual
  vtkCell* 
  GetElemCell(int theObjID);

  //----------------------------------------------------------------------------
  //! Get dimension of corresponding mesh element
  virtual
  int
  GetObjDimension( const int theObjId );

  //! To insert some additional filters and then sets the given #vtkMapper
  virtual
  void
  SetMapper(vtkMapper* theMapper); 

  //! Allows to get initial #vtkDataSet
  virtual
  vtkDataSet* 
  GetInput(); 

  //! Apply view transformation
  virtual
  void
  SetTransform(VTKViewer_Transform* theTransform); 

  //! Apply additional position
  virtual
  void
  SetPosition(float _arg1, float _arg2, float _arg3);

  //! Apply additional position
  virtual
  void
  SetPosition(float _arg[3]);

  //! To calculatate last modified time
  virtual
  unsigned long int
  GetMTime();

  //----------------------------------------------------------------------------
  //! Set representation (VTK_SURFACE, VTK_POINTS, VTK_WIREFRAME and so on)
  virtual
  void
  SetRepresentation(int theMode);

  //! Get current representation mode
  virtual
  int
  GetRepresentation();

  //! Get current display mode (obsolete)
  virtual
  int
  getDisplayMode();

  //! Set display mode (obsolete)
  virtual
  void
  setDisplayMode(int theMode);

  //----------------------------------------------------------------------------
  //! Set infinive flag
  /*!
    Infinitive means actor without size (point for example),
    which is not taken into account in calculation of boundaries of the scene
  */
  void
  SetInfinitive(bool theIsInfinite);

  //! Get infinive flag
  virtual
  bool
  IsInfinitive();
    
  //! To calcualte current bounding box
  virtual
  float* 
  GetBounds();

  //! To calcualte current bounding box
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
  //! Set ResolveCoincidentTopology flag
  void
  SetResolveCoincidentTopology(bool theIsResolve);

  //! Set ResolveCoincidentTopology parameters
  void
  SetPolygonOffsetParameters(float factor, float units);

  //! Get current ResolveCoincidentTopology parameters
  void
  GetPolygonOffsetParameters(float& factor, float& units);

  virtual
  void
  Render(vtkRenderer *, vtkMapper *);

  //----------------------------------------------------------------------------
  //! Get current shrink factor
  virtual
  float
  GetShrinkFactor();

  //! Is the actor is shrunkable
  virtual
  bool
  IsShrunkable();

  //! Is the actor is shrunk
  virtual
  bool
  IsShrunk();

  //! Insert shrink filter into pipeline
  virtual
  void
  SetShrink();

  //! Remove shrink filter from pipeline
  virtual
  void
  UnShrink();

  //----------------------------------------------------------------------------
  //! Visibility management
  virtual
  void
  SetVisibility( int );

  //----------------------------------------------------------------------------
  //! To publish the actor an all its internal devices
  virtual
  void
  AddToRender(vtkRenderer* theRendere); 

  //! To remove the actor an all its internal devices
  virtual
  void
  RemoveFromRender(vtkRenderer* theRendere);

  //! Get reference on renderer where it is published
  vtkRenderer*
  GetRenderer();

  //! Used to obtain all dependent actors
  virtual
  void
  GetChildActors(vtkActorCollection*);

  //----------------------------------------------------------------------------
  //! To set interactor in order to use #vtkInteractorObserver devices
  virtual
  void
  SetInteractor(vtkRenderWindowInteractor* theInteractor);

  //! Put a request to redraw the view 
  virtual
  void
  Update();

  //----------------------------------------------------------------------------
  //! Ask, if the descendant of the SALOME_Actor will implement its own highlight or not
  virtual
  bool
  hasHighlight(); 

  //! Ask, if the SALOME_Actor is already highlighted
  virtual
  bool
  isHighlighted();

  //! Set preselection mode
  virtual
  void
  SetPreSelected(bool thePreselect = false);

  //----------------------------------------------------------------------------
  //! Set selector in order to the actor at any time can restore current selection
  virtual
  void
  SetSelector(SVTK_Selector* theSelector);

  //! Just to update visibility of the highlight devices
  virtual
  void
  highlight(bool theHighlight);  

  //! To map current selection to VTK representation
  virtual
  void
  Highlight(bool theHighlight);  

  //----------------------------------------------------------------------------
  //! To process prehighlight (called from #SVTK_InteractorStyle)
  virtual
  bool
  PreHighlight(vtkInteractorStyle* theInteractorStyle, 
	       SVTK_SelectionEvent* theSelectionEvent,
	       bool theIsHighlight);

  //! To process highlight (called from #SVTK_InteractorStyle)
  virtual 
  bool
  Highlight(vtkInteractorStyle* theInteractorStyle, 
	    SVTK_SelectionEvent* theSelectionEvent,
	    bool theIsHighlight);

  //----------------------------------------------------------------------------
  //! To set up a picker for nodal selection (initialized by #SVTK_Renderer::AddActor)
  void
  SetPointPicker(vtkPointPicker* thePointPicker); 

  //! To set up a picker for cell selection (initialized by #SVTK_Renderer::AddActor)
  void
  SetCellPicker(vtkCellPicker* theCellPicker); 

  //! To set up a picker for point rectangle selection (initialized by #SVTK_Renderer::AddActor)
  void
  SetPointRectPicker(SVTK_RectPicker* theRectPicker);

  //! To set up a picker for cell rectangle selection (initialized by #SVTK_Renderer::AddActor)
  void
  SetCellRectPicker(SVTK_RectPicker* theRectPicker);

  //----------------------------------------------------------------------------
  //! To set up a prehighlight property (initialized by #SVTK_Renderer::AddActor)
  void
  SetPreHighlightProperty(vtkProperty* theProperty);

  //! To set up a highlight property (initialized by #SVTK_Renderer::AddActor)
  void
  SetHighlightProperty(vtkProperty* theProperty);

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
  vtkProperty *PreviewProperty;
  bool myIsPreselected;
  bool myIsHighlighted;
  Selection_Mode mySelectionMode;
  vtkSmartPointer<SVTK_Selector> mySelector;

  //----------------------------------------------------------------------------
  // Highlight/ Prehighlight devices
  vtkSmartPointer<vtkPointPicker> myPointPicker;
  vtkSmartPointer<vtkCellPicker> myCellPicker;

  vtkSmartPointer<SVTK_RectPicker> myPointRectPicker;
  vtkSmartPointer<SVTK_RectPicker> myCellRectPicker;

  vtkSmartPointer<SVTK_Actor> myPreHighlightActor;
  vtkSmartPointer<SVTK_Actor> myHighlightActor;

  vtkSmartPointer<VTKViewer_Actor> myOutlineActor;
  vtkSmartPointer<vtkOutlineSource> myOutline;
};

#endif // SALOME_ACTOR_H
