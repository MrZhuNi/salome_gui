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
//  File   : 
//  Author : 
//  Module : SALOME
//  $Header$

#include "SALOME_Actor.h"

#include "SVTK_View.h"
#include "SVTK_Renderer.h"
#include "SVTK_MainWindow.h"
#include "SVTK_RenderWindowInteractor.h"
#include "SALOME_ListIteratorOfListIO.hxx"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"

#include <vtkActorCollection.h>
#include <vtkRenderer.h>

#include "utilities.h"

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


//----------------------------------------------------------------------------
SVTK_SignalHandler
::SVTK_SignalHandler(SVTK_MainWindow* theMainWindow):
  QObject(theMainWindow),
  myMainWindow(theMainWindow)
{
  if(MYDEBUG) INFOS("SVTK_SignalHandler() - "<<this);

  SVTK_RenderWindowInteractor* anInteractor = theMainWindow->GetInteractor();

  connect(anInteractor,SIGNAL(KeyPressed(QKeyEvent*)),
	  this,SIGNAL(KeyPressed(QKeyEvent*)) );
  connect(anInteractor,SIGNAL(KeyReleased(QKeyEvent*)),
	  this,SIGNAL(KeyReleased(QKeyEvent*)));
  connect(anInteractor,SIGNAL(MouseButtonPressed(QMouseEvent*)),
	  this,SIGNAL(MouseButtonPressed(QMouseEvent*)));
  connect(anInteractor,SIGNAL(MouseButtonReleased(QMouseEvent*)),
	  this,SIGNAL(MouseButtonReleased(QMouseEvent*)));
  connect(anInteractor,SIGNAL(MouseDoubleClicked(QMouseEvent*)),
	  this,SIGNAL(MouseDoubleClicked(QMouseEvent*)));
  connect(anInteractor,SIGNAL(MouseMove(QMouseEvent*)),
	  this,SIGNAL(MouseMove(QMouseEvent*)));
  connect(anInteractor,SIGNAL(contextMenuRequested(QContextMenuEvent*)),
	  this,SIGNAL(contextMenuRequested(QContextMenuEvent*)));
  connect(anInteractor,SIGNAL(selectionChanged()),
	  this,SIGNAL(selectionChanged()));
}

SVTK_SignalHandler
::~SVTK_SignalHandler()
{
  if(MYDEBUG) INFOS("~SVTK_SignalHandler() - "<<this);
}

SVTK_MainWindow*
SVTK_SignalHandler
::GetMainWindow()
{
  return myMainWindow;
}


//----------------------------------------------------------------
void
SVTK_SignalHandler
::Repaint()
{
  myMainWindow->Repaint();
}

void
SVTK_SignalHandler
::Repaint(bool theUpdateTrihedron)
{
  myMainWindow->Repaint(theUpdateTrihedron);
}

//----------------------------------------------------------------------------
SVTK_Renderer* 
SVTK_SignalHandler
::GetRenderer()
{
  return myMainWindow->GetRenderer();
}

vtkRenderer* 
SVTK_SignalHandler
::getRenderer()
{
  return myMainWindow->getRenderer();
}

//----------------------------------------------------------------
struct THighlightAction
{
  bool myIsHighlight;
  Selection_Mode mySelectionMode;
  THighlightAction( bool theIsHighlight, 
		    Selection_Mode theSelectionMode = ActorSelection ):
    myIsHighlight( theIsHighlight ),
    mySelectionMode( theSelectionMode ) 
  {}

  void
  operator()( SALOME_Actor* theActor) 
  {
    if(theActor->GetMapper() && theActor->hasIO()){
      theActor->highlight( myIsHighlight, mySelectionMode );
    }
  }
};

struct TSelectionAction
{
  bool myIsHighlight;
  SVTK_Selector* mySelector;
  TSelectionAction( bool theIsHighlight, 
		    SVTK_Selector* theSelector ):
    myIsHighlight( theIsHighlight ),
    mySelector( theSelector ) 
  {}

  void
  operator()( SALOME_Actor* theActor) 
  {
    if(theActor->GetMapper()){
      theActor->highlight( myIsHighlight, mySelector );
    }
  }
};

void
SVTK_SignalHandler
::onSelectionChanged()
{
  vtkActorCollection* anActors = myMainWindow->getRenderer()->GetActors();

  using namespace VTK;
  ForEach<SALOME_Actor>(anActors,
			THighlightAction( false ));
  SVTK_Selector* aSelector = myMainWindow->GetSelector();
  const SALOME_ListIO& aListIO = aSelector->StoredIObjects();
  SALOME_ListIteratorOfListIO anIter(aListIO);
  for(; anIter.More(); anIter.Next()){
    ForEachIf<SALOME_Actor>(anActors,
			    TIsSameIObject<SALOME_Actor>(anIter.Value()),
			    TSelectionAction(true,aSelector));
  }

  myMainWindow->Repaint(false);
}


//----------------------------------------------------------------------------
SVTK_View
::SVTK_View(SVTK_MainWindow* theMainWindow) :
  SVTK_SignalHandler(theMainWindow)
{
  if(MYDEBUG) INFOS("SVTK_View() - "<<this);
}

//----------------------------------------------------------------------------
SVTK_View
::~SVTK_View()
{
  if(MYDEBUG) INFOS("~SVTK_View() - "<<this);
}

//----------------------------------------------------------------
void 
SVTK_View
::unHighlightAll() 
{
  if(SVTK_Selector* aSelector = myMainWindow->GetSelector()){
    using namespace VTK;
    ForEach<SALOME_Actor>(getRenderer()->GetActors(),
			  THighlightAction( false ));
    Repaint();
  }
}

//----------------------------------------------------------------
void
SVTK_View
::highlight( const Handle(SALOME_InteractiveObject)& theIO, 
	     bool theIsHighlight, 
	     bool theIsUpdate ) 
{
  if(SVTK_Selector* aSelector = myMainWindow->GetSelector()){
    using namespace VTK;
    ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			    TIsSameIObject<SALOME_Actor>( theIO ),
			    THighlightAction( theIsHighlight, aSelector->SelectionMode() ));
    Repaint();
  }
}

//----------------------------------------------------------------------------
void
SVTK_View
::SetSelectionProp(const double& theRed, 
		   const double& theGreen, 
		   const double& theBlue, 
		   const int& theWidth) 
{
  vtkActorCollection* anActors = getRenderer()->GetActors();
  anActors->InitTraversal();
  while(vtkActor* anActor = anActors->GetNextActor()){
    if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(anActor))
      aSActor->SetSelectionProp(theRed,theGreen,theBlue,theWidth);
  }
}

//----------------------------------------------------------------------------
void
SVTK_View
::SetPreselectionProp(const double& theRed, 
		      const double& theGreen, 
		      const double& theBlue, 
		      const int& theWidth) 
{
  vtkActorCollection* anActors = getRenderer()->GetActors();
  anActors->InitTraversal();
  while(vtkActor* anActor = anActors->GetNextActor()){
    if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(anActor))
      aSActor->SetPreselectionProp(theRed,theGreen,theBlue,theWidth);
  }
}

//----------------------------------------------------------------------------
void
SVTK_View
::SetSelectionTolerance(const double& theTolNodes, 
			const double& theTolCell)
{
  vtkActorCollection* anActors = getRenderer()->GetActors();
  anActors->InitTraversal();
  while(vtkActor* anActor = anActors->GetNextActor()){
    if(SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(anActor))
      aSActor->SetSelectionTolerance(theTolNodes,theTolCell);
  }
}

//----------------------------------------------------------------------------
bool
SVTK_View
::isInViewer(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(getRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  return anActor != NULL;
}

//----------------------------------------------------------------------------
bool
SVTK_View
::isVisible(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(getRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  return anActor != NULL && anActor->GetVisibility();
}

//----------------------------------------------------------------------------
void
SVTK_View
::rename(const Handle(SALOME_InteractiveObject)& theIObject, 
	 const QString& theName)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,const char*,QString>
			  (&SALOME_Actor::setName,theName.latin1()));
}

//----------------------------------------------------------------------------
int
SVTK_View
::GetDisplayMode() 
{
  return myDisplayMode; 
}

void
SVTK_View
::SetDisplayMode(int theMode)
{
  if(theMode == 0) 
    ChangeRepresentationToWireframe();
  else 
    ChangeRepresentationToSurface();
  myDisplayMode = theMode;
}

void
SVTK_View
::SetDisplayMode(const Handle(SALOME_InteractiveObject)& theIObject, 
		 int theMode)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,int>
			  (&SALOME_Actor::setDisplayMode,theMode));
}

//----------------------------------------------------------------------------
void
SVTK_View
::ChangeRepresentationToWireframe()
{
  ChangeRepresentationToWireframe(getRenderer()->GetActors());
}

void
SVTK_View
::ChangeRepresentationToSurface()
{
  ChangeRepresentationToSurface(getRenderer()->GetActors());
}


void
SVTK_View
::ChangeRepresentationToWireframe(vtkActorCollection* theCollection)
{
  using namespace VTK;
  ForEach<SALOME_Actor>(theCollection,
			TSetFunction<SALOME_Actor,int>
			(&SALOME_Actor::setDisplayMode,0));
  Repaint();
}

void
SVTK_View
::ChangeRepresentationToSurface(vtkActorCollection* theCollection)
{
  using namespace VTK;
  ForEach<SALOME_Actor>(theCollection,
			TSetFunction<SALOME_Actor,int>
			(&SALOME_Actor::setDisplayMode,1));
  Repaint();
}

//----------------------------------------------------------------------------
struct TErase
{
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
SVTK_View
::EraseAll()
{   
  using namespace VTK;
  ForEach<SALOME_Actor>(getRenderer()->GetActors(),
			TErase());
  Repaint();
}

void
SVTK_View
::DisplayAll()
{ 
  using namespace VTK;
  ForEach<SALOME_Actor>(getRenderer()->GetActors(),
			TSetVisibility<SALOME_Actor>(true));
  Repaint();
}


void
SVTK_View
::Erase(SALOME_Actor* theActor, 
	bool theIsUpdate)
{
  TErase()(theActor);

  if(theIsUpdate)
    Repaint();
}


void
SVTK_View
::Erase(const Handle(SALOME_InteractiveObject)& theIObject, 
	bool theIsUpdate)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TErase());
  if(theIsUpdate)
    Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_View
::Display(SALOME_Actor* theActor, 
	  bool theIsUpdate)
{
  GetRenderer()->AddActor(theActor);
  theActor->SetVisibility(true);

  if(theIsUpdate)
    Repaint();
}

void
SVTK_View
::Display(const Handle(SALOME_InteractiveObject)& theIObject, 
	  bool theIsUpdate)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetVisibility<SALOME_Actor>(true));

  if(theIsUpdate)
    Repaint();
}

void
SVTK_View
::DisplayOnly(const Handle(SALOME_InteractiveObject)& theIObject)
{
  EraseAll();
  Display(theIObject);
}


//----------------------------------------------------------------------------
struct TRemoveAction
{
  SVTK_Renderer* myRenderer;
  TRemoveAction(SVTK_Renderer* theRenderer): 
    myRenderer(theRenderer)
  {}
  void
  operator()(SALOME_Actor* theActor)
  {
    myRenderer->RemoveActor(theActor);
  }
};

void
SVTK_View
::Remove(const Handle(SALOME_InteractiveObject)& theIObject, 
	 bool theIsUpdate)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TRemoveAction(GetRenderer()));
  if(theIsUpdate)
    Repaint();
}

void
SVTK_View
::Remove(SALOME_Actor* theActor, 
	 bool theIsUpdate)
{
  GetRenderer()->RemoveActor(theActor);
  if(theIsUpdate)
    Repaint();
}

void
SVTK_View
::RemoveAll(bool theIsUpdate)
{
  vtkRenderer* aRenderer = getRenderer();
  if(vtkActorCollection* anActors = aRenderer->GetActors()){
    anActors->InitTraversal();
    while(vtkActor *anAct = anActors->GetNextActor()){
      if(SALOME_Actor* aSAct = SALOME_Actor::SafeDownCast(anAct)){
        if(aSAct->hasIO() && aSAct->getIO()->hasEntry())
          aRenderer->RemoveActor( anAct );
      }
    }

    if(theIsUpdate)
      Repaint();
  }
}

//----------------------------------------------------------------------------
float
SVTK_View
::GetTransparency(const Handle(SALOME_InteractiveObject)& theIObject) 
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(getRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor)
    return 1.0 - anActor->GetOpacity();
  return -1.0;
}


void
SVTK_View
::SetTransparency(const Handle(SALOME_InteractiveObject)& theIObject, 
		  float theTrans)
{
  float anOpacity = 1.0 - theTrans;
  using namespace VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,float>
			  (&SALOME_Actor::SetOpacity,anOpacity));
}

//----------------------------------------------------------------------------
void
SVTK_View
::SetColor(const Handle(SALOME_InteractiveObject)& theIObject,
	   const QColor& theColor) 
{
  float aColor[3] = {theColor.red()/255., theColor.green()/255., theColor.blue()/255.};

  using namespace VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,const float*>
			  (&SALOME_Actor::SetColor,aColor));
}


QColor
SVTK_View
::GetColor(const Handle(SALOME_InteractiveObject)& theIObject) 
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(getRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor){
    float r,g,b;
    anActor->GetColor(r,g,b);
    return QColor(int(r*255),int(g*255),int(b*255));
  }

  return QColor(0,0,0);
}

