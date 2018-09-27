//  SALOME OBJECT : kernel of SALOME component
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SVTK_AxisActor2D.cxx
//  Module : SALOME
//  $Header$

#ifndef __SVTK_AxisActor2D_h
#define __SVTK_AxisActor2D_h

#include <vtkAxisActor2D.h>

#include "SVTK.h"

#ifdef WIN32
#pragma warning ( disable:4251 )
#endif

#ifndef WIN32
class VTK_HYBRID_EXPORT SVTK_AxisActor2D : public vtkAxisActor2D
#else
class SVTK_EXPORT SVTK_AxisActor2D : public vtkAxisActor2D
#endif
{
public:
  vtkTypeRevisionMacro(SVTK_AxisActor2D,vtkAxisActor2D);

  static SVTK_AxisActor2D *New();

  virtual void BuildAxis(vtkViewport *viewport);

protected:
  SVTK_AxisActor2D();
  ~SVTK_AxisActor2D();

private:
  SVTK_AxisActor2D(const SVTK_AxisActor2D&);  // Not implemented.
  void operator=(const SVTK_AxisActor2D&);  // Not implemented.
};

#ifdef WIN32
//#pragma warning ( default:4251 )
#endif

#endif
