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

class VTKViewer_CellRectPicker;

class VTKViewer_Trihedron;
class VTKViewer_Transform;
class SVTK_CubeAxesActor2D;
class VTKViewer_Actor;

class SVTK_EXPORT SVTK_Renderer : public vtkObject
{
 public:
  vtkTypeMacro(SVTK_Renderer,vtkObject);
  static SVTK_Renderer* New();

  //----------------------------------------------------------------------------
  vtkRenderer* 
  GetDevice();

  virtual
  void 
  SetInteractor(vtkRenderWindowInteractor* theInteractor);

  //----------------------------------------------------------------------------
  virtual
  void 
  AddActor(VTKViewer_Actor* theActor);

  virtual
  void 
  RemoveActor(VTKViewer_Actor* theActor);

  VTKViewer_Transform* 
  GetTransform();

  virtual
  void
  SetScale( double theScale[3] );

  void
  GetScale( double theScale[3] );

  //----------------------------------------------------------------------------
  void
  SetSelectionProp(const double& theRed = 1, 
		   const double& theGreen = 1,
		   const double& theBlue = 0, 
		   const int& theWidth = 5);

  void
  SetPreselectionProp(const double& theRed = 0, 
		      const double& theGreen = 1,
		      const double& theBlue = 1, 
		      const int& theWidth = 5);

  void
  SetSelectionTolerance(const double& theTolNodes = 0.025, 
			const double& theTolCell = 0.001);

  //----------------------------------------------------------------------------
  void
  AdjustActors();

  void
  SetTrihedronSize(int theSize);
 
  int  
  GetTrihedronSize() const;

  //----------------------------------------------------------------------------
  VTKViewer_Trihedron* 
  GetTrihedron();

  bool 
  IsTrihedronDisplayed();

  void 
  OnViewTrihedron(); 

  void 
  OnAdjustTrihedron();

  //----------------------------------------------------------------------------
  SVTK_CubeAxesActor2D* 
  GetCubeAxes();

  bool 
  IsCubeAxesDisplayed();

  void 
  OnViewCubeAxes();

  void 
  OnAdjustCubeAxes();

  //----------------------------------------------------------------------------
  void OnFitAll();
  void OnResetView();     
  void OnResetClippingRange();

  void OnFrontView(); 
  void OnBackView(); 
  void OnTopView();
  void OnBottomView();
  void OnRightView(); 
  void OnLeftView();     

 protected:
  SVTK_Renderer();
  ~SVTK_Renderer();

  virtual
  bool
  OnAdjustActors();

  //----------------------------------------------------------------------------
  // Priority at which events are processed
  float myPriority;

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

  //----------------------------------------------------------------------------
  vtkSmartPointer<VTKViewer_Transform> myTransform;

  //----------------------------------------------------------------------------
  // Highlight/ Prehighlight devices
  vtkSmartPointer<vtkPointPicker> myPointPicker;
  vtkSmartPointer<vtkCellPicker> myCellPicker;
  vtkSmartPointer<VTKViewer_CellRectPicker> myCellRectPicker;

  vtkSmartPointer<vtkProperty> myPreHighlightProperty;
  vtkSmartPointer<vtkProperty> myHighlightProperty;

  //----------------------------------------------------------------------------
  vtkSmartPointer<SVTK_CubeAxesActor2D> myCubeAxes;
  vtkSmartPointer<VTKViewer_Trihedron> myTrihedron;  
  int myTrihedronSize;
  float myBndBox[6];
};

#endif
