//  SALOME VTKViewer : build VTK viewer into Salome desktop
//
//  Copyright (C) 2004  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SVTK_Extension.cxx
//  Author : Sergey ANIKIN
//  Module : SALOME

#include "SVTK_Extension.h"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

void* SVTK::getOpenGLExtension( const char* theExtension )
{
#ifdef WIN32
  return wglGetProcAddress( theExtension );
#else
  void* OpenGLLibrary = dlopen( "libGL.so", RTLD_LAZY );
  return dlsym( OpenGLLibrary, theExtension );
#endif
}
