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
//  File   : VTKViewer_RenderWindow.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SVTK_Renderer_h
#define SVTK_Renderer_h

#include "SVTK.h"

#include <vtkSmartPointer.h>

#ifndef VTK_IMPLEMENT_MESA_CXX
#include <vtkOpenGLRenderer.h>
#define RENDERER_SUPERCLASS vtkOpenGLRenderer
#else
#include <vtkMesaRenderer.h>
#define RENDERER_SUPERCLASS vtkMesaRenderer
#endif

class vtkCallbackCommand;

class VTKViewer_Trihedron;
class VTKViewer_Transform;
class SVTK_CubeAxesActor2D;
class VTKViewer_Actor;

class SVTK_EXPORT SVTK_Renderer : public RENDERER_SUPERCLASS
{
 protected:
  SVTK_Renderer();
  ~SVTK_Renderer();

  vtkSmartPointer<VTKViewer_Transform> myTransform;
  vtkSmartPointer<SVTK_CubeAxesActor2D> myCubeAxes;
  vtkSmartPointer<VTKViewer_Trihedron> myTrihedron;  
  int myTrihedronSize;

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
  
 public:
  vtkTypeMacro(SVTK_Renderer,RENDERER_SUPERCLASS);
  static SVTK_Renderer* New();

  VTKViewer_Trihedron* GetTrihedron();
  SVTK_CubeAxesActor2D* GetCubeAxes();

  bool isTrihedronDisplayed();
  bool isCubeAxesDisplayed();

  int  GetTrihedronSize() const;
  void SetTrihedronSize(int theSize);
  void AdjustTrihedrons(bool theIsForced);

  void onViewTrihedron(); 
  void onViewCubeAxes();

  void onAdjustTrihedron();
  void onAdjustCubeAxes();

  VTKViewer_Transform* GetTransform();
  void SetScale( double theScale[3] );
  void GetScale( double theScale[3] );

  void AddActor(VTKViewer_Actor* theActor);
  void RemoveActor(VTKViewer_Actor* theActor);

  void onFitAll();
  void onResetView();     

  void onFrontView(); 
  void onBackView(); 
  void onTopView();
  void onBottomView();
  void onRightView(); 
  void onLeftView();     
};

#endif
