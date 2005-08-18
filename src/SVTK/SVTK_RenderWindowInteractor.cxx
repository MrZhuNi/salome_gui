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

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"
#include "SALOME_Actor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// VTK Includes
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkCommand.h>
#include <vtkPicker.h>
#include <vtkCamera.h>

// QT Includes
#include <qcolordialog.h>

#include "utilities.h"

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


//----------------------------------------------------------------------------
SVTK_RenderWindowInteractor
::SVTK_RenderWindowInteractor( QWidget* parent, const char* name ) :
  SVTK_RenderWindow( parent, name )
{
  myInteractor = vtkGenericRenderWindowInteractor::New();

  myInteractor->SetRenderWindow( getRenderWindow() );
  myDisplayMode = 0;

  mTimer = new QTimer( this ) ;
  connect(mTimer, SIGNAL(timeout()), this, SLOT(TimerFunc())) ;
}


SVTK_RenderWindowInteractor
::~SVTK_RenderWindowInteractor() 
{
  if(MYDEBUG) INFOS("SVTK_RenderWindowInteractor::~SVTK_RenderWindowInteractor()");

  delete mTimer ;

  myInteractor->Delete();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::Initialize() 
{
  myInteractor->Initialize();
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::UpdateSize(int w, int h) 
{
  myInteractor->UpdateSize(w,h);
}


//----------------------------------------------------------------------------
int
SVTK_RenderWindowInteractor
::CreateTimer(int vtkNotUsed(timertype)) 
{
  //
  // Start a one-shot timer for 10ms. 
  //
  mTimer->start(10, TRUE) ;
  return 1 ;
}

int
SVTK_RenderWindowInteractor
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

void
SVTK_RenderWindowInteractor
::TimerFunc() 
{
  if( ! myInteractor->GetEnabled() ) {
    return ;
  }

  vtkInteractorStyle* aStyle = vtkInteractorStyle::SafeDownCast( myInteractor->GetInteractorStyle() );
  aStyle->OnTimer();

  update();
}


//----------------------------------------------------------------------------
int
SVTK_RenderWindowInteractor
::GetDisplayMode() 
{
  return myDisplayMode;
}


//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::SetDisplayMode(int theMode)
{
  if(theMode == 0) 
    ChangeRepresentationToWireframe();
  else 
    ChangeRepresentationToSurface();
  myDisplayMode = theMode;
}


void
SVTK_RenderWindowInteractor
::SetDisplayMode(const Handle(SALOME_InteractiveObject)& theIObject, 
		 int theMode)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,int>
			  (&SALOME_Actor::setDisplayMode,theMode));
}


void
SVTK_RenderWindowInteractor
::ChangeRepresentationToWireframe()
{
  ChangeRepresentationToWireframe(GetRenderer()->GetActors());
}

void
SVTK_RenderWindowInteractor
::ChangeRepresentationToSurface()
{
  ChangeRepresentationToSurface(GetRenderer()->GetActors());
}


void
SVTK_RenderWindowInteractor
::ChangeRepresentationToWireframe(vtkActorCollection* theCollection)
{
  using namespace VTK;
  ForEach<SALOME_Actor>(theCollection,
			TSetFunction<SALOME_Actor,int>
			(&SALOME_Actor::setDisplayMode,0));
  update();
}

void
SVTK_RenderWindowInteractor
::ChangeRepresentationToSurface(vtkActorCollection* theCollection)
{
  using namespace VTK;
  ForEach<SALOME_Actor>(theCollection,
			TSetFunction<SALOME_Actor,int>
			(&SALOME_Actor::setDisplayMode,1));
  update();
}


//----------------------------------------------------------------------------
vtkRenderer* 
SVTK_RenderWindowInteractor
::GetRenderer()
{
  vtkRendererCollection * theRenderers =  getRenderWindow()->GetRenderers();
  theRenderers->InitTraversal();
  return theRenderers->GetNextItem();
}


struct TErase{
  VTK::TSetFunction<vtkActor,int> mySetFunction;
  TErase():
    mySetFunction(&vtkActor::SetVisibility,false)
  {}
  void operator()(SALOME_Actor* theActor){
    theActor->SetVisibility(false);
    // Erase dependent actors
    vtkActorCollection* aCollection = vtkActorCollection::New(); 
    theActor->GetChildActors(aCollection);
    VTK::ForEach<vtkActor>(aCollection,mySetFunction);
    aCollection->Delete();
  }
};

void
SVTK_RenderWindowInteractor
::EraseAll()
{   
  using namespace VTK;
  ForEach<SALOME_Actor>(GetRenderer()->GetActors(),
			TErase());

  update();
}

void
SVTK_RenderWindowInteractor
::DisplayAll()
{ 
  vtkActorCollection* aCollection = GetRenderer()->GetActors();
  using namespace VTK;
  ForEach<SALOME_Actor>(aCollection,TSetVisibility<SALOME_Actor>(true));

  update();
}


void
SVTK_RenderWindowInteractor
::Erase(SALOME_Actor* theActor, bool updateViewer)
{
  TErase()(theActor);

  if(updateViewer)
    update();
}


void
SVTK_RenderWindowInteractor
::Erase(const Handle(SALOME_InteractiveObject)& theIObject, 
	bool updateViewer)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TErase());

  if(updateViewer)
    update();
}


struct TRemoveAction{
  vtkRenderer* myRen;
  TRemoveAction(vtkRenderer* theRen): myRen(theRen){}
  void operator()(SALOME_Actor* theActor){
    myRen->RemoveActor(theActor);
  }
};

void
SVTK_RenderWindowInteractor
::Remove(const Handle(SALOME_InteractiveObject)& theIObject, 
	 bool updateViewer)
{
  vtkRenderer* aRen = GetRenderer();

  using namespace VTK;
  ForEachIf<SALOME_Actor>(aRen->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TRemoveAction(aRen));
}

void
SVTK_RenderWindowInteractor
::Remove( SALOME_Actor* SActor, bool updateViewer )
{
  if ( SActor != 0 )
  {
    GetRenderer()->RemoveProp( SActor );
    if ( updateViewer )
      update();
  }
}

void
SVTK_RenderWindowInteractor
::RemoveAll( const bool updateViewer )
{
  vtkRenderer* aRenderer = GetRenderer();
  vtkActorCollection* anActors = aRenderer->GetActors();
  if ( anActors )
  {
    anActors->InitTraversal();
    while ( vtkActor *anAct = anActors->GetNextActor() )
    {
      if ( anAct->IsA( "SALOME_Actor" ) )
      {
        SALOME_Actor* aSAct = (SALOME_Actor*)anAct;
        if ( aSAct->hasIO() && aSAct->getIO()->hasEntry() )
          aRenderer->RemoveActor( anAct );
      }
    }

    if ( updateViewer )
      update();
  }
}


float
SVTK_RenderWindowInteractor
::GetTransparency(const Handle(SALOME_InteractiveObject)& theIObject) 
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor)
    return 1.0 - anActor->GetOpacity();
  return -1.0;
}


void
SVTK_RenderWindowInteractor
::SetTransparency(const Handle(SALOME_InteractiveObject)& theIObject, 
		  float theTrans)
{
  float anOpacity = 1.0 - theTrans;
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,float>
			  (&SALOME_Actor::SetOpacity,anOpacity));
}


void
SVTK_RenderWindowInteractor
::Display(SALOME_Actor* theActor, bool updateViewer)
{
  GetRenderer()->AddActor(theActor);
  theActor->SetVisibility(true);

  if(updateViewer)
    update();
}


void
SVTK_RenderWindowInteractor
::Display(const Handle(SALOME_InteractiveObject)& theIObject, bool updateViewer)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetVisibility<SALOME_Actor>(true));

  if(updateViewer)
    update();
}

struct TUpdateAction{
  void operator()(vtkActor* theActor){
    theActor->ApplyProperties();
  }
};

void
SVTK_RenderWindowInteractor
::Update() 
{
  vtkRenderer* aRen = GetRenderer();

  using namespace VTK;
  ForEach<vtkActor>(aRen->GetActors(),TUpdateAction());

  aRen->ResetCamera();

  update();
}

//-----------------
// Color methods
//-----------------

void
SVTK_RenderWindowInteractor
::SetColor(const Handle(SALOME_InteractiveObject)& theIObject,QColor theColor) 
{
  float aColor[3] = {theColor.red()/255., theColor.green()/255., theColor.blue()/255.};
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,const float*>
			  (&SALOME_Actor::SetColor,aColor));
}


QColor
SVTK_RenderWindowInteractor
::GetColor(const Handle(SALOME_InteractiveObject)& theIObject) 
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor){
    float r,g,b;
    anActor->GetColor(r,g,b);
    return QColor(int(r*255),int(g*255),int(b*255));
  }
  return QColor(0,0,0);
}


bool
SVTK_RenderWindowInteractor
::isInViewer(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  return anActor != NULL;
}


bool
SVTK_RenderWindowInteractor
::isVisible(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  return anActor != NULL && anActor->GetVisibility();
}


void
SVTK_RenderWindowInteractor
::rename(const Handle(SALOME_InteractiveObject)& theIObject, QString theName)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,const char*,QString>
			  (&SALOME_Actor::setName,theName.latin1()));
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::mouseMoveEvent( QMouseEvent* event ) 
{
  //cout << "SVTK_RenderWindowInteractor::mouseMoveEvent" << endl;

  myInteractor->SetEventInformation( event->x(), event->y(),
				     ( event->state() & ControlButton ),
				     ( event->state() & ShiftButton ) );

  myInteractor->MouseMoveEvent();

  //emit MouseMove( event ) ;
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::mousePressEvent( QMouseEvent* event ) 
{
  //cout << "SVTK_RenderWindowInteractor::mousePressEvent" << endl;

  myInteractor->SetEventInformation( event->x(), event->y(),
				     ( event->state() & ControlButton ),
				     ( event->state() & ShiftButton ) );

  if( event->button() & LeftButton )
    myInteractor->LeftButtonPressEvent();
  else if( event->button() & MidButton )
    myInteractor->MiddleButtonPressEvent();
  else if( event->button() & RightButton )
    myInteractor->RightButtonPressEvent();

  //emit MouseButtonPressed( event );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::mouseReleaseEvent( QMouseEvent *event )
{
  //cout << "SVTK_RenderWindowInteractor::mouseReleaseEvent" << endl;

  myInteractor->SetEventInformation( event->x(), event->y(),
				     ( event->state() & ControlButton ),
				     ( event->state() & ShiftButton ) );

  if( event->button() & LeftButton )
    myInteractor->LeftButtonReleaseEvent();
  else if( event->button() & MidButton )
    myInteractor->MiddleButtonReleaseEvent();
  else if( event->button() & RightButton )
    myInteractor->RightButtonReleaseEvent();

  //emit MouseButtonReleased( event );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::mouseDoubleClickEvent( QMouseEvent* event )
{
  //cout << "SVTK_RenderWindowInteractor::mouseDoubleClickEvent" << endl;

  //emit MouseDoubleClicked( event );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::keyPressEvent( QKeyEvent* event ) 
{
  //cout << "SVTK_RenderWindowInteractor::keyPressEvent" << endl;

  myInteractor->SetKeyEventInformation( ( event->state() & ControlButton ),
					( event->state() & ShiftButton ),
					0 );

  myInteractor->KeyPressEvent();

  //emit KeyPressed(event) ;
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::keyReleaseEvent( QKeyEvent * event ) 
{
  //cout << "SVTK_RenderWindowInteractor::keyReleaseEvent" << endl;

  myInteractor->SetKeyEventInformation( ( event->state() & ControlButton ),
					( event->state() & ShiftButton ),
					0 );

  myInteractor->KeyReleaseEvent();

  //emit KeyReleased(event) ;
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::wheelEvent( QWheelEvent* event )
{
  //cout << "SVTK_RenderWindowInteractor::wheelEvent" << endl;

  //emit WheelMoved(event) ;
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::contextMenuEvent( QContextMenuEvent* event )
{
  //cout << "SVTK_RenderWindowInteractor::contextMenuEvent" << endl;

  if( !( event->state() & KeyButtonMask ) )
    emit contextMenuRequested( event );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::paintEvent( QPaintEvent* theEvent ) 
{
  getRenderWindow()->Render();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindowInteractor
::resizeEvent( QResizeEvent* theEvent )
{
  int aWidth = getRenderWindow()->GetSize()[0], aHeight = getRenderWindow()->GetSize()[1];

  myInteractor->UpdateSize( width(), height() );

  if( aWidth != width() || aHeight != height() )
  {
    vtkRendererCollection * aRenderers = getRenderWindow()->GetRenderers();
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
SVTK_RenderWindowInteractor
::onChangeBackgroundColor()
{
  float backint[3];

  vtkRendererCollection* theRenderers = getRenderWindow()->GetRenderers();
  theRenderers->InitTraversal();
  vtkRenderer* theRenderer = theRenderers->GetNextItem();
  theRenderer->GetBackground(backint);

  QColor selColor = QColorDialog::getColor ( QColor(int(backint[0]*255), int(backint[1]*255), int(backint[2]*255)), NULL );	
  if ( selColor.isValid() )
    theRenderer->SetBackground( selColor.red()/255., selColor.green()/255., selColor.blue()/255. ); 
}
