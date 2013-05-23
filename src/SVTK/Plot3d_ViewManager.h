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
#ifndef PLOT3D_VIEWMANAGER_H
#define PLOT3D_VIEWMANAGER_H

#include "SVTK.h"
#include "SVTK_ViewManager.h"

class SUIT_Desktop;

//! Extend Plot3d_ViewManager to deal with Plot3d_Viewer
class SVTK_EXPORT Plot3d_ViewManager : public SVTK_ViewManager
{
  Q_OBJECT

public:
  //! Construct the view manager
  Plot3d_ViewManager( SUIT_Study* study, SUIT_Desktop* );

  //! Destroy the view manager
  virtual ~Plot3d_ViewManager();
};

#endif
