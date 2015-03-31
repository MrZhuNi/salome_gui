// Copyright (C) 2010-2015  CEA/DEN, EDF R&D
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
// File   : PyEditor_ViewWindow.h
// Author : Maxim GLIBIN, Open CASCADE S.A.S. (maxim.glibin@opencascade.com)
//

#ifndef PYEDITOR_VIEWWINDOW_H
#define PYEDITOR_VIEWWINDOW_H

#include "PyEditor.h"

#include <SUIT_ViewWindow.h>

class PyEditor_EditorWindow;
class PyEditor_Viewer;
class SUIT_Desktop;

class PYEDITOR_EXPORT PyEditor_ViewWindow : public SUIT_ViewWindow
{
  Q_OBJECT

public:
  PyEditor_ViewWindow( SUIT_Desktop* theDesktop, PyEditor_Viewer* theModel );
  virtual ~PyEditor_ViewWindow();

  virtual void initLayout();

  PyEditor_EditorWindow* getEditorView();

private:
  PyEditor_Viewer*       myModel;
  PyEditor_EditorWindow* myEditorWindow;
};

#endif // PYEDITOR_VIEWWINDOW_H
