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
// File   : PyEditor_ViewWindow.cxx
// Author : Maxim GLIBIN, Open CASCADE S.A.S. (maxim.glibin@opencascade.com)
//

#include "PyEditor_ViewWindow.h"

#include "PyEditor_EditorWindow.h"

/*!
  \class PyEditor_ViewWindow
  \brief Python editor view window.
*/

/*!
  \brief Constructor.
  \param theDesktop parent desktop window
  \param theModel view model
*/
PyEditor_ViewWindow::PyEditor_ViewWindow( SUIT_Desktop* theDesktop, PyEditor_Viewer* theModel )
  : SUIT_ViewWindow( theDesktop ),
  myModel( theModel ),
  myEditorWindow( NULL )
{
}

/*!
  \brief Destructor.
*/
PyEditor_ViewWindow::~PyEditor_ViewWindow()
{
}

/*!
  \brief Initialization of python editor main window.
*/
void PyEditor_ViewWindow::initLayout()
{
  myEditorWindow = new PyEditor_EditorWindow( toolMgr() );
  if ( myEditorWindow )
  {
    myEditorWindow->setParent( this );
    myEditorWindow->show();
    setCentralWidget( myEditorWindow );
  }
}

/*!
  \return recessed editor window.
*/
PyEditor_EditorWindow* PyEditor_ViewWindow::getEditorView()
{
  return myEditorWindow;
}
