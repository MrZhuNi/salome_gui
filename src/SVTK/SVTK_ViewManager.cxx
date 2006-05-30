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
#include "SVTK_ViewManager.h"
#include "SVTK_ViewModel.h"

int SVTK_ViewManager::_SVTKViewMgr_Id = 0;

/*!
  Constructor
*/
SVTK_ViewManager::SVTK_ViewManager( SUIT_Study* study, 
		    SUIT_Desktop* theDesktop ) 
: SUIT_ViewManager( study, theDesktop )
{
  myId = ++_SVTKViewMgr_Id;
  setViewModel( new SVTK_Viewer() );
}

/*!
  Destructor
*/
SVTK_ViewManager::~SVTK_ViewManager()
{
}

/*!
  \return corresponding main window
*/
SUIT_Desktop* SVTK_ViewManager::getDesktop()
{
  return myDesktop;
}

/*!
  Sets default name of view
*/
void SVTK_ViewManager::setViewName(SUIT_ViewWindow* theView)
{
  int aPos = myViews.find(theView);
  theView->setCaption( tr( "VTK_VIEW_TITLE" ).arg( myId ).arg( aPos + 1 ) );
}
