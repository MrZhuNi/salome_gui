// Copyright (C) 2013-2015  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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
// File   : PyEditor_ViewManager.h
// Author : Maxim GLIBIN, Open CASCADE S.A.S. (maxim.glibin@opencascade.com)
//

#ifndef PYEDITOR_VIEWMANAGER_H
#define PYEDITOR_VIEWMANAGER_H

#include "PyEditor.h"

#include <SUIT_ViewManager.h>

class PYEDITOR_EXPORT PyEditor_ViewManager : public SUIT_ViewManager
{
  Q_OBJECT

public:
  PyEditor_ViewManager( SUIT_Study* theStudy,
                        SUIT_Desktop* theDesktop );
  virtual ~PyEditor_ViewManager();
};

#endif // PYEDITOR_VIEWMANAGER_H
