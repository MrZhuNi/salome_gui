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
//  File   : SVTK_RectPicker.h
//  Author : 
//  Module : SALOME

#ifndef __SVTK_RectPicker_h
#define __SVTK_RectPicker_h

#include "SVTK.h"

#include <vtkAbstractPropPicker.h>

class vtkRenderer;

/*! \class vtkAbstractPropPicker
 * \brief For more information see <a href="http://www.vtk.org/">VTK documentation
 */
/*! \class VTKViewer_RectPicker
 * \brief Rectangular picker class.
 */
class SVTK_EXPORT SVTK_RectPicker : public vtkAbstractPropPicker
{
 public:
  static
  SVTK_RectPicker *New();

  vtkTypeMacro(SVTK_RectPicker,vtkAbstractPropPicker);
  
  virtual 
  int
  Pick(float selectionX1, float selectionY1, float selectionZ1, 
       float selectionX2, float selectionY2, float selectionZ2,
       vtkRenderer *renderer);  

  int
  Pick(float selectionPt1[3], float selectionPt2[3], vtkRenderer *ren);

 protected:
  SVTK_RectPicker();
  ~SVTK_RectPicker();

 private:
  virtual 
  int
  Pick(float, float, float, vtkRenderer*);
};

#endif


