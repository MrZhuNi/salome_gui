//  SALOME SUPERVGraph : build Supervisor viewer into desktop
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
//  File   : SUPERVGraph.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

using namespace std;
#include "SUPERVGraph.h"
#include "SUPERVGraph_ViewFrame.h"

SUIT_ViewWindow* SUPERVGraph::createView(SUIT_Desktop* parent)
{
  return new SUPERVGraph_ViewFrame( parent/*, "vtkView"*/ ); 
}

extern "C"
{
  SUIT_ViewWindow* createView(SUIT_Desktop* parent)
  {
    return SUPERVGraph::createView(parent);
  }
}
