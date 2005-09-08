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
//  Module :
//  $Header$

#include "SVTK_RenderWindow.h"

#include "SVTK_Renderer.h"
#include "SVTK_Selector.h"

#include <vtkObjectFactory.h>

//----------------------------------------------------------------------------
vtkStandardNewMacro(SVTK_RenderWindow);


//----------------------------------------------------------------------------
SVTK_RenderWindow
::SVTK_RenderWindow():
  myRenderWidget(NULL)
{
}

SVTK_RenderWindow
::~SVTK_RenderWindow() 
{}


//----------------------------------------------------------------------------
SVTK_Renderer* 
SVTK_RenderWindow
::GetRenderer()
{
  return myRenderer.GetPointer();
}

void
SVTK_RenderWindow
::SetRenderer(SVTK_Renderer* theRenderer)
{
  this->RemoveRenderer(GetRenderer());
  myRenderer = theRenderer;
  this->AddRenderer(GetRenderer());
}


//----------------------------------------------------------------------------
SVTK_Selector* 
SVTK_RenderWindow
::GetSelector()
{
  return mySelector.GetPointer();
}

void
SVTK_RenderWindow
::SetSelector(SVTK_Selector* theSelector)
{
  mySelector = theSelector;
}


//----------------------------------------------------------------------------
SVTK_RenderWindow::TRenderWidget* 
SVTK_RenderWindow
::GetRenderWidget()
{
  return myRenderWidget;
}

void
SVTK_RenderWindow
::GetRenderWidget(TRenderWidget* theWidget)
{
  myRenderWidget = theWidget;
}
