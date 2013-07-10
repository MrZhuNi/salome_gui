// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#include <LightApp_GVSelector.h>

#include <GraphicsView_Object.h>
#include <GraphicsView_Selector.h>
#include <GraphicsView_ViewPort.h>
#include <GraphicsView_Viewer.h>

LightApp_GVDataOwner::LightApp_GVDataOwner( GraphicsView_Object* theObject )
: myObject( theObject )
{
}

LightApp_GVDataOwner::~LightApp_GVDataOwner()
{
}

QString LightApp_GVDataOwner::keyString() const
{
  return myObject->getName();
}

GraphicsView_Object* LightApp_GVDataOwner::object() const
{
  return myObject;
}

LightApp_GVSelector::LightApp_GVSelector( GraphicsView_Viewer* theViewer,
                                          SUIT_SelectionMgr* theSelMgr )
: SUIT_Selector( theSelMgr ), myViewer( theViewer )
{
  connect( theViewer->getSelector(), SIGNAL( selSelectionDone( GV_SelectionChangeStatus ) ),
           this, SLOT( OnSelectionDone( GV_SelectionChangeStatus ) ) );
}

LightApp_GVSelector::~LightApp_GVSelector()
{
}

QString LightApp_GVSelector::type() const
{
  return GraphicsView_Viewer::Type();
}

void LightApp_GVSelector::getSelection( SUIT_DataOwnerPtrList& theList ) const
{
  GraphicsView_ViewPort* aViewport = myViewer->getActiveViewPort();
  for( aViewport->initSelected(); aViewport->moreSelected(); aViewport->nextSelected() )
    theList.append( new LightApp_GVDataOwner( aViewport->selectedObject() ) );
}

void LightApp_GVSelector::setSelection( const SUIT_DataOwnerPtrList& )
{
}

void LightApp_GVSelector::OnSelectionDone( GV_SelectionChangeStatus )
{
  selectionChanged();
}
