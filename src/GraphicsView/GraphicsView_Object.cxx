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

#include "GraphicsView_Object.h"

#include "GraphicsView_Scene.h"
#include "GraphicsView_ViewPort.h"

//=======================================================================
// Name    : GraphicsView_Object
// Purpose : Constructor
//=======================================================================
GraphicsView_Object::GraphicsView_Object( QGraphicsItem* theParent )
: QGraphicsItemGroup( theParent ),
  myPriority( 0 ),
  myIsOnTop( false ),
  myIsHighlighted( false ),
  myIsSelected( false ),
  myIsMoving( false )
{
  myHighlightCursor = new QCursor( Qt::OpenHandCursor );
}

//=======================================================================
// Name    : GraphicsView_Object
// Purpose : Destructor
//=======================================================================
GraphicsView_Object::~GraphicsView_Object()
{
  if( myHighlightCursor )
  {
    delete myHighlightCursor;
    myHighlightCursor = 0;
  }
}

//================================================================
// Function : addTo
// Purpose  : 
//================================================================
void GraphicsView_Object::addTo( GraphicsView_ViewPort* theViewPort )
{
  if( QGraphicsScene* aScene = theViewPort->scene() )
    aScene->addItem( this );
}

//================================================================
// Function : removeFrom
// Purpose  : 
//================================================================
void GraphicsView_Object::removeFrom( GraphicsView_ViewPort* theViewPort )
{
  if( QGraphicsScene* aScene = theViewPort->scene() )
    aScene->removeItem( this );
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
bool GraphicsView_Object::checkHighlight( double theX, double theY, QCursor& theCursor ) const
{
  return !getRect().isNull() && getRect().contains( theX, theY );
}

//================================================================
// Function : highlight
// Purpose  : 
//================================================================
bool GraphicsView_Object::highlight( double theX, double theY )
{
  QCursor aCursor;
  if( myIsHighlighted = isVisible() )
    myIsHighlighted = checkHighlight( theX, theY, aCursor );
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
  QCursor aCursor;
  if( myIsSelected = isVisible() )
  {
    if( !theRect.isNull() )
      myIsSelected = theRect.contains( getRect() );
    else
      myIsSelected = checkHighlight( theX, theY, aCursor );
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
  if( theIsAtOnce )
  {
    finishMove();
    return;
  }

  myIsMoving = true;
  moveBy( theDX, theDY );
}

//================================================================
// Function : finishMove
// Purpose  : 
//================================================================
bool GraphicsView_Object::finishMove()
{
  myIsMoving = false;
  if( GraphicsView_Scene* aScene = dynamic_cast<GraphicsView_Scene*>( scene() ) )
    aScene->processRectChanged();
  return true;
}

//================================================================
// Function : setViewTransform
// Purpose  : 
//================================================================
void GraphicsView_Object::setViewTransform( const QTransform& theTransform )
{
  myViewTransform = theTransform;
}
