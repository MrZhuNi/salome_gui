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
// File   : PyEditor_ViewModel.cxx
// Author : Maxim GLIBIN, Open CASCADE S.A.S. (maxim.glibin@opencascade.com)
//

#include "PyEditor_ViewModel.h"

#include "PyEditor_ViewWindow.h"

/*!
  \class PyEditor_Viewer
  \brief Python editor view model.
*/

/*!
  \brief Constructor.
*/
PyEditor_Viewer::PyEditor_Viewer() : SUIT_ViewModel()
{
}

/*!
  \brief Destructor.
*/
PyEditor_Viewer::~PyEditor_Viewer()
{
}

/*!
  Create new instance of view window on desktop \a theDesktop.
  \retval SUIT_ViewWindow* - created view window pointer.
*/
SUIT_ViewWindow* PyEditor_Viewer::createView( SUIT_Desktop* theDesktop )
{
  PyEditor_ViewWindow* aPyEditor = new PyEditor_ViewWindow( theDesktop, this );
  initView( aPyEditor );
  return aPyEditor;
}

/*!
  Start initialization of view window
  \param view - view window to be initialized
*/
void PyEditor_Viewer::initView( PyEditor_ViewWindow* theViewModel )
{
  if ( theViewModel )
    theViewModel->initLayout();
}
