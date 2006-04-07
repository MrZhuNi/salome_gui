//  SALOME VTKViewer : build VTK viewer into Salome desktop
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
//  File   : 
//  Author : 
//  Module : SALOME
//  $Header$

#ifndef SVTK_Renderer_h
#define SVTK_Renderer_h

#include "SVTK.h"

#include <vtkObject.h>
#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkCallbackCommand;
class vtkRenderWindowInteractor;

class vtkPicker;
class vtkPointPicker;
class vtkCellPicker;
class vtkProperty;

class SVTK_RectPicker;

class VTKViewer_Trihedron;
class VTKViewer_Transform;
class SVTK_CubeAxesActor2D;
class VTKViewer_Actor;
class SVTK_Selector;


//! The class is a container for #vtkRenderer instance.
/*!
  Main goal of the class is to apply common behaviour to all #SALOME_Actor, like
  selection and preselection colors.
  Also, the class is responsible for management of internal actors like trihedron an so on.
 */
//============================================================================
class SVTK_EXPORT SVTK_Renderer : public vtkObject
{
 public:
  vtkTypeMacro(SVTK_Renderer,vtkObject);
  static SVTK_Renderer* New();

  //----------------------------------------------------------------------------
  //! Get its device
  vtkRenderer* 
  GetDevice();

  //! Initialize the class
  virtual
  void 
  Initialize(vtkRenderWindowInteractor* theInteractor,
	     SVTK_Selector* theSelector);

  //----------------------------------------------------------------------------
  //! This method publishes pointed actor into the renderer
  virtual
  void 
  AddActor(VTKViewer_Actor* theActor);

  //! This method removes pointed actor from the renderer
  virtual
  void 
  RemoveActor(VTKViewer_Actor* theActor);

  //! Get special container that keeps scaling of the scene 
  VTKViewer_Transform* 
  GetTransform();

  //! This method allow to apply a scale on the whole scene
  virtual
  void
  SetScale( double theScale[3] );

  //! This method allow to get a scale that is applied on the whole scene
  void
  GetScale( double theScale[3] );

  //----------------------------------------------------------------------------
  //! Applies color and size (PointSize and LineWidth) of primitives in selection mode
  void
  SetSelectionProp(const double& theRed = 1, 
		   const double& theGreen = 1,
		   const double& theBlue = 0, 
		   const int& theWidth = 5);

  //! Applies color and size (PointSize and LineWidth) of primitives in preselection mode
  void
  SetPreselectionProp(const double& theRed = 0, 
		      const double& theGreen = 1,
		      const double& theBlue = 1, 
		      const int& theWidth = 5);

  //! Setup requested tollerance for the picking
  void
  SetSelectionTolerance(const double& theTolNodes = 0.025, 
			const double& theTolCell = 0.001);

  //----------------------------------------------------------------------------
  //! Adjust all intenal actors (trihedron and graduated rules) to the scene
  void
  AdjustActors();

  //! Set size of the trihedron in percents from bounding box of the scene
  void
  SetTrihedronSize(int theSize, const bool theRelative = true);
 
  //! Get size of the trihedron in percents from bounding box of the scene
  int  
  GetTrihedronSize() const;

  //! Shows if the size of the trihedron is relative
  bool  
  IsTrihedronRelative() const;

  //----------------------------------------------------------------------------
  //! Get trihedron control
  VTKViewer_Trihedron* 
  GetTrihedron();

  //! Is trihedron displayed
  bool 
  IsTrihedronDisplayed();

  //! Toggle trihedron visibility
  void 
  OnViewTrihedron(); 

  //! Adjust size of the trihedron to the bounding box of the scene
  void 
  OnAdjustTrihedron();

  //----------------------------------------------------------------------------
  //! Get graduated rules control
  SVTK_CubeAxesActor2D* 
  GetCubeAxes();

  //! Is graduated rules displayed
  bool 
  IsCubeAxesDisplayed();

  //! Toggle graduated rules visibility
  void 
  OnViewCubeAxes();

  //! Adjust size of the graduated rules to the bounding box of the scene
  void 
  OnAdjustCubeAxes();

  //----------------------------------------------------------------------------
  //! Fit all presentation in the scene into the window
  void OnFitAll(); 
  
  //! Set camera into predefined state
  void OnResetView(); 

  //! Reset camera clipping range to adjust the range to the bounding box of the scene
  void OnResetClippingRange(); 

  //! To reset direction of the camera to front view
  void OnFrontView(); 

  //! To reset direction of the camera to back view
  void OnBackView(); 

  //! To reset direction of the camera to top view
  void OnTopView();

  //! To reset direction of the camera to bottom view
  void OnBottomView();

  //! To reset direction of the camera to right view
  void OnRightView(); 

  //! To reset direction of the camera to left view
  void OnLeftView();     

 protected:
  SVTK_Renderer();
  ~SVTK_Renderer();

  virtual
  bool
  OnAdjustActors();

  //----------------------------------------------------------------------------
  // Priority at which events are processed
  vtkFloatingPointType myPriority;

  // Used to process events
  vtkSmartPointer<vtkCallbackCommand> myEventCallbackCommand;

  // Description:
  // Main process event method
  static void ProcessEvents(vtkObject* object, 
                            unsigned long event,
                            void* clientdata, 
                            void* calldata);
  
  vtkSmartPointer<vtkRenderer> myDevice;
  vtkRenderWindowInteractor* myInteractor;
  vtkSmartPointer<SVTK_Selector> mySelector;

  //----------------------------------------------------------------------------
  vtkSmartPointer<VTKViewer_Transform> myTransform;

  //----------------------------------------------------------------------------
  // Highlight/ Prehighlight devices
  vtkSmartPointer<vtkPointPicker> myPointPicker;
  vtkSmartPointer<vtkCellPicker> myCellPicker;

  vtkSmartPointer<SVTK_RectPicker> myPointRectPicker;
  vtkSmartPointer<SVTK_RectPicker> myCellRectPicker;

  vtkSmartPointer<vtkProperty> myPreHighlightProperty;
  vtkSmartPointer<vtkProperty> myHighlightProperty;

  //----------------------------------------------------------------------------
  vtkSmartPointer<SVTK_CubeAxesActor2D> myCubeAxes;
  vtkSmartPointer<VTKViewer_Trihedron> myTrihedron;  
  int  myTrihedronSize;
  bool myIsTrihedronRelative;
  vtkFloatingPointType myBndBox[6];
};

#endif
