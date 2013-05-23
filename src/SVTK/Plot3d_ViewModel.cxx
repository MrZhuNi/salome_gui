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
#include "Plot3d_ViewModel.h"

#include "SVTK_ViewWindow.h"

/*!
  Constructor
*/
Plot3d_Viewer::Plot3d_Viewer()
{
}

/*!
  Destructor
*/
Plot3d_Viewer::~Plot3d_Viewer() 
{
}

/*!Create new instance of view window on desktop \a theDesktop.
 *\retval SUIT_ViewWindow* - created view window pointer.
 */
SUIT_ViewWindow* Plot3d_Viewer::createView( SUIT_Desktop* theDesktop )
{
  SUIT_ViewWindow* aViewWindow = SVTK_Viewer::createView( theDesktop );
  if( SVTK_ViewWindow* aSVTKViewWindow = dynamic_cast<SVTK_ViewWindow*>( aViewWindow ) )
    aSVTKViewWindow->SetMode2DEnabled( true );
  return aViewWindow;
}
