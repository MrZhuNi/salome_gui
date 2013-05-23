// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#ifndef PLOT3D_VIEWMODEL_H
#define PLOT3D_VIEWMODEL_H

#include "SVTK.h"
#include "SVTK_ViewModel.h"

//! Extends interface #SVTK_Viewer
class SVTK_EXPORT Plot3d_Viewer : public SVTK_Viewer
{
  Q_OBJECT;

public:
  //! Define string representation of the viewer type
  static QString Type() { return "Plot3d"; }

  Plot3d_Viewer();
  virtual ~Plot3d_Viewer();

  //! See #SUIT_ViewModel::createView
  virtual SUIT_ViewWindow* createView( SUIT_Desktop* );

  //! See #SUIT_ViewModel::getType
  virtual QString getType() const { return Type(); }
};

#endif
