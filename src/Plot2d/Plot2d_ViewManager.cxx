//  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "Plot2d_ViewManager.h"
#include "Plot2d_ViewModel.h"
#include "Plot2d_ViewWindow.h"
#include "Plot2d_ViewFrame.h"

/*!
  Constructor
*/
Plot2d_ViewManager::Plot2d_ViewManager( SUIT_Study* study, SUIT_Desktop* desk ) 
: SUIT_ViewManager( study, desk, new Plot2d_Viewer() )
{
  setTitle( tr( "PLOT2D_VIEW_TITLE" ) );
}

/*!
  Destructor
*/
Plot2d_ViewManager::~Plot2d_ViewManager()
{
}

/*!
  \return corresponding viewer
*/
Plot2d_Viewer* Plot2d_ViewManager::getPlot2dModel() const
{
  return (Plot2d_Viewer*)myViewModel;
}

/*!
  Adds new view
  \param theView - view to be added
*/
bool Plot2d_ViewManager::insertView( SUIT_ViewWindow* theView )
{
  bool res = SUIT_ViewManager::insertView( theView );
  if ( res )
  {
    Plot2d_ViewWindow* view = (Plot2d_ViewWindow*)theView;
    connect( view, SIGNAL( cloneView() ), this, SLOT( onCloneView() ) );

    Plot2d_ViewFrame* aViewFrame = view->getViewFrame();
    Plot2d_Viewer* aViewer = getPlot2dModel();
    connect( aViewFrame, SIGNAL( legendClicked( QwtPlotItem* ) ), 
             aViewer, SLOT( onLegendClicked( QwtPlotItem* ) ) );
  }
  return res;
}

/*!
  Creates new view
*/
void Plot2d_ViewManager::createView()
{
  createViewWindow();
}

/*!
  SLOT: called if action "Clone view" is activated, emits signal cloneView()
*/
void Plot2d_ViewManager::onCloneView()
{
  if( sender() && sender()->inherits( "Plot2d_ViewWindow" ) )
  {
    Plot2d_ViewWindow* srcWnd = ( Plot2d_ViewWindow* )sender();
    cloneView( srcWnd );
  }
}

/*!
  \brief Creates clone of source window
  \param srcWnd source window
  \return Pointer on the new window
  \sa onCloneView()
*/
Plot2d_ViewWindow* Plot2d_ViewManager::cloneView( Plot2d_ViewWindow* srcWnd )
{
  SUIT_ViewWindow* vw = createViewWindow();

  Plot2d_ViewWindow* newWnd = 0;
  if( vw && vw->inherits( "Plot2d_ViewWindow" ) )
    newWnd = ( Plot2d_ViewWindow* )vw;
  
  if( newWnd && srcWnd )
    emit cloneView( srcWnd->getViewFrame(), newWnd->getViewFrame() );

  return newWnd;
}
