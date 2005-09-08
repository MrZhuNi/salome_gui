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

#ifndef SVTK_RenderWindow_h
#define SVTK_RenderWindow_h

#include "SVTK.h"

#include <vtkSmartPointer.h>

#ifndef VTK_IMPLEMENT_MESA_CXX
#include <vtkXOpenGLRenderWindow.h>
#define SVTK_RENDER_WINDOW_SUPERCLASS vtkXOpenGLRenderWindow
#else
#include <vtkXMesaRenderWindow.h>
#define SVTK_RENDER_WINDOW_SUPERCLASS vtkXMesaRenderWindow
#endif

class QWidget;
class SVTK_Renderer;
class SVTK_Selector;

class SVTK_EXPORT SVTK_RenderWindow: public SVTK_RENDER_WINDOW_SUPERCLASS
{
 public:
  vtkTypeMacro(SVTK_RenderWindow,SVTK_RENDER_WINDOW_SUPERCLASS);
  static SVTK_RenderWindow* New();

  SVTK_RenderWindow();
  virtual ~SVTK_RenderWindow();

  SVTK_Renderer* GetRenderer();
  void SetRenderer(SVTK_Renderer* theRenderer);

  SVTK_Selector* GetSelector();
  void SetSelector(SVTK_Selector* theSelector);

  typedef QWidget TRenderWidget;
  TRenderWidget* GetRenderWidget();
  void GetRenderWidget(TRenderWidget* theWidget);

 protected:
  vtkSmartPointer<SVTK_Selector> mySelector;
  vtkSmartPointer<SVTK_Renderer> myRenderer;
  TRenderWidget* myRenderWidget;
};


#endif
