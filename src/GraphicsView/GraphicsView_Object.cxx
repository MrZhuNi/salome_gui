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
// File:   GraphicsView_Object.h
// Author: Oleg UVAROV, Open CASCADE S.A.S. (oleg.uvarov@opencascade.com)
//

#include "GraphicsView_Object.h"

#include "GraphicsView_Scene.h"

//=======================================================================
// Name    : GraphicsView_Object
// Purpose : Constructor
//=======================================================================
GraphicsView_Object::GraphicsView_Object( QGraphicsItem* theParent )
: QGraphicsItemGroup( theParent ),
  myIsHighlighted( false ),
  myIsSelected( false )
{
}

//=======================================================================
// Name    : GraphicsView_Object
// Purpose : Destructor
//=======================================================================
GraphicsView_Object::~GraphicsView_Object()
{
}

//================================================================
// Function : setName
// Purpose  : 
//================================================================
void GraphicsView_Object::setName( const QString& theName )
{
  myName = theName;
}

//================================================================
// Function : getRect
// Purpose  : 
//================================================================
QRectF GraphicsView_Object::getRect() const
{
  return sceneBoundingRect();
}

//================================================================
// Function : checkHighlight
// Purpose  : 
//================================================================
bool GraphicsView_Object::checkHighlight( double theX, double theY ) const
{
  return getRect().contains( theX, theY );
}

//================================================================
// Function : highlight
// Purpose  : 
//================================================================
bool GraphicsView_Object::highlight( double theX, double theY )
{
  if( myIsHighlighted = isVisible() )
    myIsHighlighted = checkHighlight( theX, theY );
  return myIsHighlighted;
}

//================================================================
// Function : unhighlight
// Purpose  : 
//================================================================
void GraphicsView_Object::unhighlight()
{
  myIsHighlighted = false;
}

//================================================================
// Function : select
// Purpose  : 
//================================================================
bool GraphicsView_Object::select( double theX, double theY, const QRectF& theRect )
{
  if( myIsSelected = isVisible() )
  {
    if( !theRect.isNull() )
      myIsSelected = theRect.contains( getRect() );
    else
      myIsSelected = checkHighlight( theX, theY );
  }
  return myIsSelected;
}

//================================================================
// Function : unselect
// Purpose  : 
//================================================================
void GraphicsView_Object::unselect()
{
  myIsSelected = false;
}

//================================================================
// Function : move
// Purpose  : 
//================================================================
void GraphicsView_Object::move( double theDX, double theDY, bool theIsAtOnce )
{
  moveBy( theDX, theDY );
  if( theIsAtOnce )
    finishMove();
}

//================================================================
// Function : finishMove
// Purpose  : 
//================================================================
bool GraphicsView_Object::finishMove()
{
  if( GraphicsView_Scene* aScene = dynamic_cast<GraphicsView_Scene*>( scene() ) )
    aScene->processRectChanged();
  return true;
}
