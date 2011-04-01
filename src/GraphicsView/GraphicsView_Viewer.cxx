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
// File:   GraphicsView_Viewer.cxx
// Author: Oleg UVAROV, Open CASCADE S.A.S. (oleg.uvarov@opencascade.com)
//

#include "GraphicsView_Viewer.h"

#include "GraphicsView_Object.h"
#include "GraphicsView_Selector.h"
#include "GraphicsView_Scene.h"
#include "GraphicsView_ViewFrame.h"
#include "GraphicsView_ViewPort.h"
#include "GraphicsView_ViewTransformer.h"

#include <SUIT_ViewManager.h>

#include <QApplication>
#include <QColorDialog>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QMenu>

//=======================================================================
// Name    : GraphicsView_Viewer
// Purpose : Constructor
//=======================================================================
GraphicsView_Viewer::GraphicsView_Viewer( const QString& title )
: SUIT_ViewModel(),
  mySelector( 0 ),
  myTransformer( 0 ),
  myIsInitialized( false ),
  myIsImmediateSelectionEnabled( false )
{
}

//=======================================================================
// Name    : GraphicsView_Viewer
// Purpose : Destructor
//=======================================================================
GraphicsView_Viewer::~GraphicsView_Viewer()
{
  delete mySelector;
}

//================================================================
// Function : createView
// Purpose  : 
//================================================================
SUIT_ViewWindow* GraphicsView_Viewer::createView( SUIT_Desktop* theDesktop )
{
  GraphicsView_ViewFrame* aViewFrame = new GraphicsView_ViewFrame( theDesktop, this );

  connect( aViewFrame, SIGNAL( keyPressed( QKeyEvent* ) ),
           this, SLOT( onKeyEvent( QKeyEvent* ) ) );

  connect( aViewFrame, SIGNAL( keyReleased( QKeyEvent* ) ),
           this, SLOT( onKeyEvent( QKeyEvent* ) ) );

  connect( aViewFrame, SIGNAL( mousePressed( QGraphicsSceneMouseEvent* ) ),
           this, SLOT( onMouseEvent( QGraphicsSceneMouseEvent* ) ) );

  connect( aViewFrame, SIGNAL( mouseMoving( QGraphicsSceneMouseEvent* ) ),
           this, SLOT( onMouseEvent( QGraphicsSceneMouseEvent* ) ) );

  connect( aViewFrame, SIGNAL( mouseReleased( QGraphicsSceneMouseEvent* ) ),
           this, SLOT( onMouseEvent( QGraphicsSceneMouseEvent* ) ) );

  connect( aViewFrame, SIGNAL( wheeling( QGraphicsSceneWheelEvent* ) ),
           this, SLOT( onWheelEvent( QGraphicsSceneWheelEvent* ) ) );

  return aViewFrame;
}

//================================================================
// Function : contextMenuPopup
// Purpose  : 
//================================================================
void GraphicsView_Viewer::contextMenuPopup( QMenu* thePopup )
{
  if( thePopup->actions().count() > 0 )
    thePopup->addSeparator();

  thePopup->addAction( tr( "CHANGE_BGCOLOR" ), this, SLOT( onChangeBgColor() ) );
}

//================================================================
// Function : getSelector
// Purpose  : 
//================================================================
GraphicsView_Selector* GraphicsView_Viewer::getSelector()
{
  if( !mySelector )
  {
    mySelector = new GraphicsView_Selector( this );
    if( mySelector )
    {
      connect( mySelector, SIGNAL( selSelectionDone( GV_SelectionChangeStatus ) ),
               this, SLOT( onSelectionDone( GV_SelectionChangeStatus ) ) );
      connect( mySelector, SIGNAL( selSelectionCancel() ),
               this, SLOT( onSelectionCancel() ) );
    }
  }
  return mySelector;
}

//================================================================
// Function : getActiveView
// Purpose  : 
//================================================================
GraphicsView_ViewFrame* GraphicsView_Viewer::getActiveView() const
{
  if( SUIT_ViewManager* aViewManager = getViewManager() )
    return dynamic_cast<GraphicsView_ViewFrame*>( aViewManager->getActiveView() );
  return NULL;
}

//================================================================
// Function : getActiveViewPort
// Purpose  : 
//================================================================
GraphicsView_ViewPort* GraphicsView_Viewer::getActiveViewPort() const
{
  if( GraphicsView_ViewFrame* aViewFrame = getActiveView() )
    return aViewFrame->getViewPort();
  return NULL;
}

//================================================================
// Function : getActiveScene
// Purpose  : 
//================================================================
GraphicsView_Scene* GraphicsView_Viewer::getActiveScene() const
{
  if( GraphicsView_ViewPort* aViewPort = getActiveViewPort() )
    return dynamic_cast<GraphicsView_Scene*>( aViewPort->scene() );
  return NULL;
}

//================================================================
// Function : activateTransform
// Purpose  : 
//================================================================
void GraphicsView_Viewer::activateTransform( int theType )
{
  if( theType == NoTransform ) // finish current transform
  {
    if ( myTransformer )
    {
      onTransformationFinished();
      delete myTransformer;
      myTransformer = 0;
    }
  }
  else // activate new transform
  {
    activateTransform( NoTransform );
    myTransformer = createTransformer( theType );
    onTransformationStarted();
    myTransformer->exec();
  }
}

//================================================================
// Function : setIsInitialized
// Purpose  : 
//================================================================
void GraphicsView_Viewer::setIsInitialized( bool theFlag )
{
  myIsInitialized = theFlag;
}

//================================================================
// Function : setIsImmediateSelectionEnabled
// Purpose  : 
//================================================================
void GraphicsView_Viewer::setIsImmediateSelectionEnabled( bool theFlag )
{
  myIsImmediateSelectionEnabled = theFlag;
}

//================================================================
// Function : createTransformer
// Purpose  : 
//================================================================
GraphicsView_ViewTransformer* GraphicsView_Viewer::createTransformer( int theType )
{
  return new GraphicsView_ViewTransformer( this, theType );
}

//================================================================
// Function : onTransformationStarted
// Purpose  : 
//================================================================
void GraphicsView_Viewer::onTransformationStarted()
{
  if( GraphicsView_Selector* aSelector = getSelector() )
  {
    aSelector->undetectAll();
    aSelector->lock( true ); // disable selection
  }

  // watch events: any mouse/key event outside the
  // viewport will be considered as the end of transform
  if( myTransformer )
    qApp->installEventFilter( this );
}

//================================================================
// Function : onTransformationFinished
// Purpose  : 
//================================================================
void GraphicsView_Viewer::onTransformationFinished()
{
  if( GraphicsView_Selector* aSelector = getSelector() )
    aSelector->lock( false ); // enable selection

  // stop watching events
  if( myTransformer )
    qApp->removeEventFilter( this );
}

//================================================================
// Function : onKeyEvent
// Purpose  : 
//================================================================
void GraphicsView_Viewer::onKeyEvent( QKeyEvent* e )
{
  switch( e->type() )
  {
    case QEvent::KeyPress:
      handleKeyPress( e );
      break;
    case QEvent::KeyRelease:
      handleKeyRelease( e );
      break;
    default: break;
  }
}

//================================================================
// Function : onMouseEvent
// Purpose  : 
//================================================================
void GraphicsView_Viewer::onMouseEvent( QGraphicsSceneMouseEvent* e )
{
  switch( e->type() )
  {
    case QEvent::GraphicsSceneMousePress:
      handleMousePress( e );
      break;
    case QEvent::GraphicsSceneMouseMove:
      handleMouseMove( e );
      break;
    case QEvent::GraphicsSceneMouseRelease:
      handleMouseRelease( e );
      break;
    default: break;
  }
}

//================================================================
// Function : onWheelEvent
// Purpose  : 
//================================================================
void GraphicsView_Viewer::onWheelEvent( QGraphicsSceneWheelEvent* e )
{
  switch( e->type() )
  {
    case QEvent::GraphicsSceneWheel:
      handleWheel( e );
      break;
    default: break;
  }
}

//================================================================
// Function : handleKeyPress
// Purpose  : 
//================================================================
void GraphicsView_Viewer::handleKeyPress( QKeyEvent* e )
{
}

//================================================================
// Function : handleKeyRelease
// Purpose  : 
//================================================================
void GraphicsView_Viewer::handleKeyRelease( QKeyEvent* e )
{
}

//================================================================
// Function : handleMousePress
// Purpose  : 
//================================================================
void GraphicsView_Viewer::handleMousePress( QGraphicsSceneMouseEvent* e )
{
  // test accel for transforms
  if ( e->modifiers() & GraphicsView_ViewTransformer::accelKey() )
  {
    Qt::MouseButton bs = e->button();
    if ( bs == GraphicsView_ViewTransformer::zoomButton() )
      activateTransform( Zoom );
    else if ( bs == GraphicsView_ViewTransformer::panButton() )
      activateTransform( Pan );
  }
  else // checking for other operations before selection in release event
  {
    if( GraphicsView_ViewPort* aViewPort = getActiveViewPort() )
    {
      if( e->button() == Qt::RightButton &&
          isImmediateSelectionEnabled() &&
          aViewPort->nbSelected() < 1 )
      {
        // If the 'immediate selection' mode is enabled,
        // try to perform selection before invoking context menu
        bool append = bool ( e->modifiers() & GraphicsView_Selector::getAppendKey() );
        getSelector()->select( QRectF(), append );
      }
      else if( e->button() == Qt::LeftButton &&
               !aViewPort->isSelectByRect() && 
               !aViewPort->isDragging() &&
               aViewPort->startPulling( e->scenePos() ) )
      {
        // Try to start pulling if rectangular selection is performed
        aViewPort->finishSelectByRect();
      }
      else if( e->button() == Qt::LeftButton &&
               !( aViewPort->currentBlock() & GraphicsView_ViewPort::BS_Selection ) &&
               !aViewPort->getHighlightedObject() )
      {
        // Start rectangular selection if pulling was not started
        QPoint p = aViewPort->mapFromScene( e->scenePos() );
        aViewPort->startSelectByRect( p.x(), p.y() );
      }
    }
  }
}

//================================================================
// Function : handleMouseMove
// Purpose  : 
//================================================================
void GraphicsView_Viewer::handleMouseMove( QGraphicsSceneMouseEvent* e )
{
  // highlight for selection
  bool dragged = ( e->buttons() & ( Qt::LeftButton | Qt::MidButton | Qt::RightButton ) );
  if ( !dragged )
  {
    if ( getSelector() )
      getSelector()->detect( e->scenePos().x(), e->scenePos().y() );
  }

  // try to activate other operations
  if( GraphicsView_ViewPort* aViewPort = getActiveViewPort() )
  {
    if( aViewPort->isPulling() )
    {
      aViewPort->drawPulling( e->scenePos() );
    }
    else if( e->button() == Qt::LeftButton &&
             !aViewPort->isSelectByRect() &&
             !aViewPort->isDragging() &&
             aViewPort->startPulling( e->scenePos() ) )
    {
      aViewPort->finishSelectByRect();
    }
    else if( !aViewPort->getHighlightedObject() )
    {
      QPoint p = aViewPort->mapFromScene( e->scenePos() );
      aViewPort->drawSelectByRect( p.x(), p.y() );
    }
  }
}

//================================================================
// Function : handleMouseRelease
// Purpose  : 
//================================================================
void GraphicsView_Viewer::handleMouseRelease( QGraphicsSceneMouseEvent* e )
{
  // selection
  if( GraphicsView_ViewPort* aViewPort = getActiveViewPort() )
  {
    if( e->button() == Qt::LeftButton &&
        !( aViewPort->currentBlock() & GraphicsView_ViewPort::BS_Selection ) )
    {
      if ( getSelector() )
      {
        bool append = bool ( e->modifiers() & GraphicsView_Selector::getAppendKey() );
        getSelector()->select( QRectF(), append );
      }
    }
  }

  // try to finish active operations
  if( GraphicsView_ViewPort* aViewPort = getActiveViewPort() )
  {
    if( aViewPort->isPulling() )
    {
      aViewPort->finishPulling();
    }
    else if( !aViewPort->getHighlightedObject() )
    {
      QRect aSelRect = aViewPort->selectionRect();
      aViewPort->finishSelectByRect();
      if ( getSelector() && !aSelRect.isNull() )
      {            
        bool append = bool ( e->modifiers() & GraphicsView_Selector::getAppendKey() );
        QRectF aRect = aViewPort->mapToScene( aSelRect ).boundingRect();
        getSelector()->select( aRect, append );
      }
    }
  }
}

//================================================================
// Function : handleWheel
// Purpose  : 
//================================================================
void GraphicsView_Viewer::handleWheel( QGraphicsSceneWheelEvent* e )
{
  if( GraphicsView_ViewPort* aViewPort = getActiveViewPort() )
  {
    bool anIsScaleUp = e->delta() > 0;
    bool anIsCtrl = e->modifiers() & Qt::ControlModifier;

    bool anIsScaleChanged = false;
    for( aViewPort->initSelected(); aViewPort->moreSelected(); aViewPort->nextSelected() )
      if( GraphicsView_Object* anObject = aViewPort->selectedObject() )
        anIsScaleChanged = anObject->updateScale( anIsScaleUp, anIsCtrl ) || anIsScaleChanged;

    if( anIsScaleChanged )
    {
      emit wheelScaleChanged();
      aViewPort->onBoundingRectChanged();
    }
  }
}

//================================================================
// Function : onSelectionDone
// Purpose  : 
//================================================================
void GraphicsView_Viewer::onSelectionDone( GV_SelectionChangeStatus theStatus )
{
  emit selectionChanged( theStatus );
}

//================================================================
// Function : onChangeBgColor
// Purpose  : 
//================================================================
void GraphicsView_Viewer::onChangeBgColor()
{
  if( GraphicsView_ViewPort* aViewPort = getActiveViewPort() )
  {
    QColor aColor = aViewPort->isForegroundEnabled() ?
      aViewPort->foregroundColor() : aViewPort->backgroundColor();
    aColor = QColorDialog::getColor( aColor, aViewPort );	
    if ( aColor.isValid() )
    {
      if( aViewPort->isForegroundEnabled() )
      {
        aViewPort->setForegroundColor( aColor );
        aViewPort->updateForeground();
      }
      else
        aViewPort->setBackgroundColor( aColor );
    }
  }
}

//================================================================
// Function : onSelectionCancel
// Purpose  : 
//================================================================
void GraphicsView_Viewer::onSelectionCancel()
{
  emit selectionChanged( GVSCS_Invalid );
}
