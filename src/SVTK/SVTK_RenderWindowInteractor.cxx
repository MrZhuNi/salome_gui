//  SALOME VTKViewer : build VTK viewer into Salome desktop
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : VTKViewer_RenderWindowInteractor.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SVTK_RenderWindowInteractor.h"

#include "SVTK_InteractorStyle.h"
#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"
#include "SALOME_Actor.h"
#include "SVTK_SpaceMouse.h" 
#include "SVTK_Event.h" 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// VTK Includes
#include <vtkObjectFactory.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkPicker.h>
#include <vtkCamera.h>

// QT Includes
#include <qtimer.h>
#include <qcolordialog.h>
#include <qpaintdevice.h>

#include "utilities.h"

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


//----------------------------------------------------------------------------
vtkStandardNewMacro(QtRenderWindowInteractor);

QtRenderWindowInteractor
::QtRenderWindowInteractor()
{
  myTimer = new QTimer( ) ;
  connect(myTimer, SIGNAL(timeout()), this, SLOT(OnTimeOut())) ;
}

QtRenderWindowInteractor
::~QtRenderWindowInteractor()
{
  delete myTimer;
}


//----------------------------------------------------------------------------
void
QtRenderWindowInteractor
::OnTimeOut() 
{
  if( GetEnabled() ) {
    this->InvokeEvent(vtkCommand::TimerEvent,NULL);
  }
}

int
QtRenderWindowInteractor
::CreateTimer(int vtkNotUsed(timertype)) 
{
  //
  // Start a one-shot timer for 10ms. 
  //
  myTimer->start(10, TRUE) ;
  return 1 ;
}

int
QtRenderWindowInteractor
::DestroyTimer(void) 
{
  //
  // :TRICKY: Tue May  2 00:17:32 2000 Pagey
  //
  // QTimer will automatically expire after 10ms. So 
  // we do not need to do anything here. In fact, we 
  // should not even Stop() the QTimer here because doing 
  // this will skip some of the processing that the TimerFunc()
  // does and will result in undesirable effects. For 
  // example, this will result in vtkLODActor to leave
  // the models in low-res mode after the mouse stops
  // moving. 
  //
  return 1 ;
}


//----------------------------------------------------------------------------
QVTK_RenderWindowInteractor
::QVTK_RenderWindowInteractor(QWidget* theParent, 
			      const char* theName):
  QWidget(theParent,theName),
  myInteractor(QtRenderWindowInteractor::New())
{
  myInteractor->Delete();
}


QVTK_RenderWindowInteractor
::~QVTK_RenderWindowInteractor() 
{
  if(SVTK_SpaceMouse* aSpaceMouse = SVTK_SpaceMouse::getInstance())
    if(aSpaceMouse->isSpaceMouseOn())
      aSpaceMouse->close(x11Display());
}

//----------------------------------------------------------------------------
vtkRenderWindowInteractor* 
QVTK_RenderWindowInteractor
::GetDevice()
{
  return myInteractor.GetPointer();
}

//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::SetRenderWindow(vtkRenderWindow *theRenderWindow)
{
  myInteractor->SetRenderWindow(theRenderWindow);
#ifndef WNT
  theRenderWindow->SetDisplayId((void*)x11Display());
#endif
  theRenderWindow->SetWindowId((void*)winId());
  theRenderWindow->DoubleBufferOn();
  setMouseTracking(true);

  // To avoid UnbindContext: Assertion `vb.context == ctx' failed
  // Initialize();
}

vtkRenderWindow*
QVTK_RenderWindowInteractor
::GetRenderWindow()
{
  return myInteractor->GetRenderWindow();
}

//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::Initialize() 
{
  myInteractor->Initialize();
}

//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::InvokeEvent(unsigned long theEvent, void* theCallData)
{
  myInteractor->InvokeEvent(theEvent,theCallData);
}

//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::UpdateSize(int w, int h) 
{
  myInteractor->UpdateSize(w,h);
}

//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::paintEvent( QPaintEvent* theEvent ) 
{
  myInteractor->Render();
}


//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::resizeEvent( QResizeEvent* theEvent )
{
  int* aSize = GetRenderWindow()->GetSize();
  int aWidth = aSize[0];
  int aHeight = aSize[1];

  UpdateSize( width(), height() );

  if( aWidth != width() || aHeight != height() )
  {
    vtkRendererCollection * aRenderers = GetRenderWindow()->GetRenderers();
    aRenderers->InitTraversal();
    double aCoeff = 1.0;
    if(vtkRenderer *aRenderer = aRenderers->GetNextItem())
    {
      vtkCamera *aCamera = aRenderer->GetActiveCamera();
      double aScale = aCamera->GetParallelScale();
      if((aWidth - width())*(aHeight - height()) > 0)
        aCoeff = sqrt(double(aWidth)/double(width())*double(height())/double(aHeight));
      else
        aCoeff = double(aWidth)/double(width());
      aCamera->SetParallelScale(aScale*aCoeff);
    }
  }
}


//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::contextMenuEvent( QContextMenuEvent* event )
{}


//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::mouseMoveEvent( QMouseEvent* event ) 
{
  myInteractor->SetEventInformation( event->x(), event->y(),
				     ( event->state() & ControlButton ),
				     ( event->state() & ShiftButton ) );
  myInteractor->MouseMoveEvent();
}


//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::mousePressEvent( QMouseEvent* event ) 
{
  myInteractor->SetEventInformation( event->x(), event->y(),
				     ( event->state() & ControlButton ),
				     ( event->state() & ShiftButton ) );
  if( event->button() & LeftButton )
    myInteractor->LeftButtonPressEvent();
  else if( event->button() & MidButton )
    myInteractor->MiddleButtonPressEvent();
  else if( event->button() & RightButton )
    myInteractor->RightButtonPressEvent();
}


//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::mouseReleaseEvent( QMouseEvent *event )
{
  myInteractor->SetEventInformation( event->x(), event->y(),
				     ( event->state() & ControlButton ),
				     ( event->state() & ShiftButton ) );

  if( event->button() & LeftButton )
    myInteractor->LeftButtonReleaseEvent();
  else if( event->button() & MidButton )
    myInteractor->MiddleButtonReleaseEvent();
  else if( event->button() & RightButton )
    myInteractor->RightButtonReleaseEvent();
}


//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::mouseDoubleClickEvent( QMouseEvent* event )
{}


//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::wheelEvent( QWheelEvent* event )
{}


//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::keyPressEvent( QKeyEvent* event ) 
{
  myInteractor->SetKeyEventInformation( ( event->state() & ControlButton ),
					( event->state() & ShiftButton ),
					event->key() );
  myInteractor->KeyPressEvent();
}

//----------------------------------------------------------------------------
void
QVTK_RenderWindowInteractor
::keyReleaseEvent( QKeyEvent * event ) 
{
  myInteractor->SetKeyEventInformation( ( event->state() & ControlButton ),
					( event->state() & ShiftButton ),
					event->key() );
  myInteractor->KeyReleaseEvent();
}


//----------------------------------------------------------------------------
void  
QVTK_RenderWindowInteractor
::focusInEvent ( QFocusEvent* event )
{
  QWidget::focusInEvent( event );

  // register set space mouse events receiver
  if(SVTK_SpaceMouse* aSpaceMouse = SVTK_SpaceMouse::getInstance()){
    if(!aSpaceMouse->isSpaceMouseOn()) {// initialize 3D space mouse driver 
      aSpaceMouse->initialize(x11Display(),winId());
    }else{
      aSpaceMouse->setWindow(x11Display(),winId());
    }
  }

  myInteractor->EnterEvent();
}

//----------------------------------------------------------------------------
void  
QVTK_RenderWindowInteractor
::focusOutEvent ( QFocusEvent* event )
{
  QWidget::focusInEvent( event );

  // unregister set space mouse events receiver
  if(SVTK_SpaceMouse* aSpaceMouse = SVTK_SpaceMouse::getInstance()){
    if(aSpaceMouse->isSpaceMouseOn())
      aSpaceMouse->setWindow(x11Display(),0);
  }

  myInteractor->LeaveEvent();
}


//----------------------------------------------------------------------------
bool 
QVTK_RenderWindowInteractor
::x11Event( XEvent *xEvent )
{
  // handle 3d space mouse events
  if(SVTK_SpaceMouse* aSpaceMouse = SVTK_SpaceMouse::getInstance()){
    if(aSpaceMouse->isSpaceMouseOn() && xEvent->type == ClientMessage){
      SVTK_SpaceMouse::MoveEvent anEvent;
      int type = aSpaceMouse->translateEvent( x11Display(), xEvent, &anEvent, 1.0, 1.0 );
      switch( type ){
      case SVTK_SpaceMouse::SpaceMouseMove : 
	myInteractor->InvokeEvent(SVTK::SpaceMouseMoveEvent, anEvent.data );
	break;
      case SVTK_SpaceMouse::SpaceButtonPress :
	myInteractor->InvokeEvent( SVTK::SpaceMouseButtonEvent, &anEvent.button );
	break;
      case SVTK_SpaceMouse::SpaceButtonRelease :
	break;
      }
      return true; // stop handling the event
    }
  }

  return QWidget::x11Event( xEvent );
}


//----------------------------------------------------------------------------
SVTK_RenderWindowInteractor
::SVTK_RenderWindowInteractor(QWidget* theParent, 
			       const char* theName):
  QVTK_RenderWindowInteractor(theParent,theName),
  myEventCallbackCommand(vtkCallbackCommand::New())
{
  myEventCallbackCommand->Delete();

  myEventCallbackCommand->SetClientData(this); 
  myPriority = 0.0;

  myEventCallbackCommand->SetCallback(SVTK_RenderWindowInteractor::ProcessEvents);

  SetSelector(SVTK_Selector::New());
  GetSelector()->Delete();
}

SVTK_RenderWindowInteractor
::~SVTK_RenderWindowInteractor() 
{}


//----------------------------------------------------------------------------
SVTK_Selector* 
SVTK_RenderWindowInteractor
::GetSelector() 
{ 
  return mySelector.GetPointer(); 
}


void
SVTK_RenderWindowInteractor
::SetSelector(SVTK_Selector* theSelector)
{ 
  if(mySelector.GetPointer())
    mySelector->RemoveObserver(myEventCallbackCommand.GetPointer());

  mySelector = theSelector; 

  if(mySelector.GetPointer())
    mySelector->AddObserver(vtkCommand::EndPickEvent, 
			    myEventCallbackCommand.GetPointer(), 
			    myPriority);
}


//----------------------------------------------------------------------------
void 
SVTK_RenderWindowInteractor
::ProcessEvents(vtkObject* vtkNotUsed(theObject), 
		unsigned long theEvent,
		void* theClientData, 
		void* vtkNotUsed(theCallData))
{
  SVTK_RenderWindowInteractor* self = reinterpret_cast<SVTK_RenderWindowInteractor*>(theClientData);

  switch(theEvent){
  case vtkCommand::EndPickEvent:
    self->onEmitSelectionChanged();
    break;
  }
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::InitInteractorStyle(SVTK_InteractorStyle* theStyle)
{
  GetDevice()->SetInteractorStyle(theStyle); 
  theStyle->FindPokedRenderer(0,0);

  theStyle->SetSelector(GetSelector());
  theStyle->SetRenderWidget(this);
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::PushInteractorStyle(SVTK_InteractorStyle* theStyle)
{
  myInteractorStyles.push(PInteractorStyle(theStyle));
  InitInteractorStyle(theStyle);
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::PopInteractorStyle()
{
  if(GetInteractorStyle())
    myInteractorStyles.pop();
  
  if(GetInteractorStyle()) 
    InitInteractorStyle(GetInteractorStyle());
}


//----------------------------------------------------------------------------
SVTK_InteractorStyle* 
SVTK_RenderWindowInteractor
::GetInteractorStyle()
{
  return myInteractorStyles.isEmpty() ? 0 : myInteractorStyles.top().GetPointer();
}


//----------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::SetSelectionMode(int theMode)
{
  mySelector->SetSelectionMode(theMode);
}


//----------------------------------------------------------------
int
SVTK_RenderWindowInteractor
::SelectionMode() const
{
  return mySelector->SelectionMode();
}


//----------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::onEmitSelectionChanged()
{
  return emit selectionChanged();
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::mouseMoveEvent( QMouseEvent* event ) 
{
  QVTK_RenderWindowInteractor::mouseMoveEvent(event);

  emit MouseMove( event );
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::mousePressEvent( QMouseEvent* event ) 
{
  QVTK_RenderWindowInteractor::mousePressEvent(event);

  emit MouseButtonPressed( event );
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::mouseReleaseEvent( QMouseEvent *event )
{
  QVTK_RenderWindowInteractor::mouseReleaseEvent(event);

  emit MouseButtonReleased( event );
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::mouseDoubleClickEvent( QMouseEvent* event )
{
  QVTK_RenderWindowInteractor::mouseDoubleClickEvent(event);

  emit MouseDoubleClicked( event );
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::wheelEvent( QWheelEvent* event )
{
  QVTK_RenderWindowInteractor::wheelEvent(event);

  emit WheelMoved( event );
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::keyPressEvent( QKeyEvent* event ) 
{
  QVTK_RenderWindowInteractor::keyPressEvent(event);

  emit KeyPressed( event );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::keyReleaseEvent( QKeyEvent * event ) 
{
  QVTK_RenderWindowInteractor::keyReleaseEvent(event);

  emit KeyReleased( event );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::contextMenuEvent( QContextMenuEvent* event )
{
  if( !( event->state() & KeyButtonMask ) )
    emit contextMenuRequested( event );
}
