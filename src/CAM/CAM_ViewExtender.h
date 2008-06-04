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
#ifndef CAM_VIEWEXTENDER_H
#define CAM_VIEWEXTENDER_H

#include "CAM.h"

class QtxActionToolMgr;
class QMenu;
class SUIT_ViewModel;
class SUIT_ViewWindow;
/*! 
  An interface which can be used as a module dedicated extension for Viewer
*/
class CAM_EXPORT CAM_ViewExtender 
{
public:
  //! Creates a New toolbar containing extending actions. Returns New Toolbar Id
  virtual int createToolbar(SUIT_ViewWindow* ) = 0;
  virtual void contextMenuPopup(QMenu* ) = 0;
  virtual void activate(SUIT_ViewModel*) = 0;
  virtual void deactivate(SUIT_ViewModel*) = 0;
};

#endif
