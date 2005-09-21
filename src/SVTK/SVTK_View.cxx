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

#include <qapplication.h>
#include <qpushbutton.h>

#include <vtkGenericRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkTextProperty.h>
#include <vtkActorCollection.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkRenderWindow.h>

#include "QtxAction.h"

#include "SUIT_Session.h"
#include "SUIT_ToolButton.h"
#include "SUIT_MessageBox.h"

#include "SUIT_Tools.h"
#include "SUIT_ResourceMgr.h"

#include "VTKViewer_Utilities.h"
#include "VTKViewer_CellRectPicker.h"

#include "SVTK_View.h"
#include "SVTK_Event.h"
#include "SVTK_Renderer.h"
#include "SVTK_ViewModel.h"
#include "SVTK_InteractorStyle.h"
#include "SVTK_RenderWindowInteractor.h"

#include "SALOME_ListIteratorOfListIO.hxx"

#include "SVTK_SelectorDef.h"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"

#include "utilities.h"

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


//----------------------------------------------------------------------------
SVTK_MainWindow
::SVTK_MainWindow(QWidget* theParent, 
		  const char* theName,
		  SUIT_ResourceMgr* theResourceMgr) :
  QMainWindow(theParent,theName,0)
{
  if(MYDEBUG) INFOS("SVTK_MainWindow() - "<<this);

  myToolBar = new QToolBar(this);
  myToolBar->setCloseMode(QDockWindow::Undocked);
  myToolBar->setLabel(tr("LBL_TOOLBAR_LABEL"));

  createActions(theResourceMgr);
  createToolBar();
}

void
SVTK_MainWindow
::Initialize(SVTK_RenderWindowInteractor* theInteractor)
{
  myInteractor = theInteractor;
  SetEventDispatcher(myInteractor->GetDevice());

  setCentralWidget(myInteractor);

  myInteractor->setFocusPolicy(StrongFocus);
  myInteractor->setFocus();
  setFocusProxy(myInteractor);
}


//----------------------------------------------------------------------------
SVTK_MainWindow
::~SVTK_MainWindow()
{
  if(MYDEBUG) INFOS("~SVTK_MainWindow() - "<<this);
}


//----------------------------------------------------------------------------
SVTK_RenderWindowInteractor*
SVTK_MainWindow
::GetInteractor()
{
  return myInteractor;
}

vtkRenderWindowInteractor*
SVTK_MainWindow
::getInteractor()
{
  return GetInteractor()->GetDevice();
}

vtkRenderWindow*
SVTK_MainWindow
::getRenderWindow()
{
  return GetInteractor()->getRenderWindow();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::Repaint() 
{ 
  Repaint( true ); 
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::Repaint(bool theUpdateTrihedron)
{
  if(theUpdateTrihedron) 
    GetRenderer()->OnAdjustTrihedron();

  GetInteractor()->update();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::InvokeEvent(unsigned long theEvent, void* theCallData)
{
  GetInteractor()->InvokeEvent(theEvent,theCallData);
}

//----------------------------------------------------------------------------
vtkInteractorStyle*
SVTK_MainWindow
::GetInteractorStyle()
{
  return GetInteractor()->GetInteractorStyle();
}

void
SVTK_MainWindow
::PushInteractorStyle(vtkInteractorStyle* theStyle)
{
  GetInteractor()->PushInteractorStyle(theStyle);
}

void
SVTK_MainWindow
::PopInteractorStyle()
{
  GetInteractor()->PopInteractorStyle();
}

//----------------------------------------------------------------------------
SVTK_Selector*
SVTK_MainWindow
::GetSelector()
{
  return GetInteractor()->GetSelector();
}

Selection_Mode
SVTK_MainWindow
::SelectionMode()
{
  return GetSelector()->SelectionMode();
}

void
SVTK_MainWindow
::SetSelectionMode(Selection_Mode theMode)
{
  GetSelector()->SetSelectionMode(theMode);
}


//----------------------------------------------------------------------------
SVTK_Renderer* 
SVTK_MainWindow
::GetRenderer()
{
  return GetInteractor()->GetRenderer();
}

vtkRenderer* 
SVTK_MainWindow
::getRenderer()
{
  return GetInteractor()->getRenderer();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::SetBackgroundColor(const QColor& theColor)
{
  getRenderer()->SetBackground(theColor.red()/255.0, 
			       theColor.green()/255.0,
			       theColor.blue()/255.0);
}

QColor
SVTK_MainWindow
::BackgroundColor()
{
  float aBackgroundColor[3];
  getRenderer()->GetBackground(aBackgroundColor);
  return QColor(int(aBackgroundColor[0]*255), 
		int(aBackgroundColor[1]*255), 
		int(aBackgroundColor[2]*255));
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::GetScale( double theScale[3] ) 
{
  GetRenderer()->GetScale( theScale );
}

void
SVTK_MainWindow
::SetScale( double theScale[3] ) 
{
  GetRenderer()->SetScale( theScale );
  Repaint();
}


//----------------------------------------------------------------------------
void
SVTK_MainWindow
::AddActor(VTKViewer_Actor* theActor, 
	   bool theIsUpdate)
{
  GetRenderer()->AddActor(theActor);
  if(theIsUpdate) 
    Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::RemoveActor(VTKViewer_Actor* theActor, 
	      bool theIsUpdate)
{
  GetRenderer()->RemoveActor(theActor);
  if(theIsUpdate) 
    Repaint();
}


//----------------------------------------------------------------------------
int
SVTK_MainWindow
::GetTrihedronSize()
{
  return GetRenderer()->GetTrihedronSize();
}

//----------------------------------------------------------------------------
void 
SVTK_MainWindow
::SetTrihedronSize( const int theSize )
{
  GetRenderer()->SetTrihedronSize(theSize);
  Repaint();
}


//----------------------------------------------------------------------------
/*! If parameter theIsForcedUpdate is true, recalculate parameters for
 *  trihedron and cube axes, even if trihedron and cube axes is invisible.
 */
void
SVTK_MainWindow
::AdjustActors()
{
  GetRenderer()->AdjustActors();
  Repaint();
}

//----------------------------------------------------------------------------
bool
SVTK_MainWindow
::IsTrihedronDisplayed()
{
  return GetRenderer()->IsTrihedronDisplayed();
}

//----------------------------------------------------------------------------
bool
SVTK_MainWindow
::IsCubeAxesDisplayed()
{
  return GetRenderer()->IsCubeAxesDisplayed();
}

//----------------------------------------------------------------------------
VTKViewer_Trihedron*  
SVTK_MainWindow
::GetTrihedron() 
{ 
  return GetRenderer()->GetTrihedron(); 
}

//----------------------------------------------------------------------------
SVTK_CubeAxesActor2D* 
SVTK_MainWindow
::GetCubeAxes() 
{ 
  return GetRenderer()->GetCubeAxes(); 
}


//----------------------------------------------------------------------------
QToolBar* 
SVTK_MainWindow
::getToolBar()
{
  return myToolBar;
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::SetEventDispatcher(vtkObject* theDispatcher)
{
  myEventDispatcher = theDispatcher;
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::createActions(SUIT_ResourceMgr* theResourceMgr)
{
  if(!myActionsMap.isEmpty()) 
    return;
  
  QtxAction* aAction;

  // Dump view
  aAction = new QtxAction(tr("MNU_DUMP_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_DUMP" ) ),
			  tr( "MNU_DUMP_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_DUMP_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onDumpView()));
  myActionsMap[ DumpId ] = aAction;

  // FitAll
  aAction = new QtxAction(tr("MNU_FITALL"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FITALL" ) ),
			  tr( "MNU_FITALL" ), 0, this);
  aAction->setStatusTip(tr("DSC_FITALL"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onFitAll()));
  myActionsMap[ FitAllId ] = aAction;

  // FitRect
  aAction = new QtxAction(tr("MNU_FITRECT"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FITAREA" ) ),
			  tr( "MNU_FITRECT" ), 0, this);
  aAction->setStatusTip(tr("DSC_FITRECT"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateWindowFit()));
  myActionsMap[ FitRectId ] = aAction;

  // Zoom
  aAction = new QtxAction(tr("MNU_ZOOM_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_ZOOM" ) ),
			  tr( "MNU_ZOOM_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_ZOOM_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateZoom()));
  myActionsMap[ ZoomId ] = aAction;

  // Panning
  aAction = new QtxAction(tr("MNU_PAN_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_PAN" ) ),
			  tr( "MNU_PAN_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_PAN_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activatePanning()));
  myActionsMap[ PanId ] = aAction;

  // Global Panning
  aAction = new QtxAction(tr("MNU_GLOBALPAN_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_GLOBALPAN" ) ),
			  tr( "MNU_GLOBALPAN_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_GLOBALPAN_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateGlobalPanning()));
  myActionsMap[ GlobalPanId ] = aAction;

  // Rotation
  aAction = new QtxAction(tr("MNU_ROTATE_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_ROTATE" ) ),
			  tr( "MNU_ROTATE_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_ROTATE_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateRotation()));
  myActionsMap[ RotationId ] = aAction;

  // Projections
  aAction = new QtxAction(tr("MNU_FRONT_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FRONT" ) ),
			  tr( "MNU_FRONT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_FRONT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onFrontView()));
  myActionsMap[ FrontId ] = aAction;

  aAction = new QtxAction(tr("MNU_BACK_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_BACK" ) ),
			  tr( "MNU_BACK_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_BACK_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onBackView()));
  myActionsMap[ BackId ] = aAction;

  aAction = new QtxAction(tr("MNU_TOP_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_TOP" ) ),
			  tr( "MNU_TOP_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_TOP_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onTopView()));
  myActionsMap[ TopId ] = aAction;

  aAction = new QtxAction(tr("MNU_BOTTOM_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_BOTTOM" ) ),
			  tr( "MNU_BOTTOM_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_BOTTOM_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onBottomView()));
  myActionsMap[ BottomId ] = aAction;

  aAction = new QtxAction(tr("MNU_LEFT_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_LEFT" ) ),
			  tr( "MNU_LEFT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_LEFT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onLeftView()));
  myActionsMap[ LeftId ] = aAction;

  aAction = new QtxAction(tr("MNU_RIGHT_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_RIGHT" ) ),
			  tr( "MNU_RIGHT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_RIGHT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onRightView()));
  myActionsMap[ RightId ] = aAction;

  // Reset
  aAction = new QtxAction(tr("MNU_RESET_VIEW"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_RESET" ) ),
			  tr( "MNU_RESET_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_RESET_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onResetView()));
  myActionsMap[ ResetId ] = aAction;

  // onViewTrihedron: Shows - Hides Trihedron
  aAction = new QtxAction(tr("MNU_SHOW_TRIHEDRON"), 
			  theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_TRIHEDRON" ) ),
			  tr( "MNU_SHOW_TRIHEDRON" ), 0, this);
  aAction->setStatusTip(tr("DSC_SHOW_TRIHEDRON"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onViewTrihedron()));
  myActionsMap[ ViewTrihedronId ] = aAction;
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::createToolBar()
{
  myActionsMap[DumpId]->addTo(myToolBar);
  myActionsMap[ViewTrihedronId]->addTo(myToolBar);

  SUIT_ToolButton* aScaleBtn = new SUIT_ToolButton(myToolBar);
  aScaleBtn->AddAction(myActionsMap[FitAllId]);
  aScaleBtn->AddAction(myActionsMap[FitRectId]);
  aScaleBtn->AddAction(myActionsMap[ZoomId]);

  SUIT_ToolButton* aPanningBtn = new SUIT_ToolButton(myToolBar);
  aPanningBtn->AddAction(myActionsMap[PanId]);
  aPanningBtn->AddAction(myActionsMap[GlobalPanId]);

  myActionsMap[RotationId]->addTo(myToolBar);

  SUIT_ToolButton* aViewsBtn = new SUIT_ToolButton(myToolBar);
  aViewsBtn->AddAction(myActionsMap[FrontId]);
  aViewsBtn->AddAction(myActionsMap[BackId]);
  aViewsBtn->AddAction(myActionsMap[TopId]);
  aViewsBtn->AddAction(myActionsMap[BottomId]);
  aViewsBtn->AddAction(myActionsMap[LeftId]);
  aViewsBtn->AddAction(myActionsMap[RightId]);

  myActionsMap[ResetId]->addTo(myToolBar);
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::activateZoom()
{
  myEventDispatcher->InvokeEvent(SVTK::StartZoom,0);
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::activatePanning()
{
  myEventDispatcher->InvokeEvent(SVTK::StartPan,0);
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::activateRotation()
{
  myEventDispatcher->InvokeEvent(SVTK::StartRotate,0);
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::activateGlobalPanning()
{
  myEventDispatcher->InvokeEvent(SVTK::StartGlobalPan,0);
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::activateWindowFit()
{
  myEventDispatcher->InvokeEvent(SVTK::StartFitArea,0);
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onFrontView()
{
  GetRenderer()->OnFrontView();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onBackView()
{
  GetRenderer()->OnBackView();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onTopView()
{
  GetRenderer()->OnTopView();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onBottomView()
{
  GetRenderer()->OnBottomView();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onLeftView()
{
  GetRenderer()->OnLeftView();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onRightView()
{
  GetRenderer()->OnRightView();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onResetView()
{
  GetRenderer()->OnResetView();
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onFitAll()
{
  GetRenderer()->OnFitAll();
  Repaint();
}

//----------------------------------------------------------------------------
void 
SVTK_MainWindow
::onViewTrihedron()
{
  GetRenderer()->OnViewTrihedron();
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onViewCubeAxes()
{
  GetRenderer()->OnViewCubeAxes();
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onAdjustTrihedron()
{   
  GetRenderer()->OnAdjustTrihedron();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onAdjustCubeAxes()
{   
  GetRenderer()->OnAdjustCubeAxes();
}

//----------------------------------------------------------------------------
void
SVTK_MainWindow
::onDumpView()
{}

//----------------------------------------------------------------------------
QImage
SVTK_MainWindow
::dumpView()
{
  QPixmap px = QPixmap::grabWindow( GetInteractor()->winId() );
  return px.convertToImage();
}


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
    if(theActor->GetMapper()){
      theActor->highlight( myIsHighlight, mySelectionMode );
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
  Selection_Mode aSelectionMode = aSelector->SelectionMode();
  for(; anIter.More(); anIter.Next()){
    ForEachIf<SALOME_Actor>(anActors,
			    TIsSameIObject<SALOME_Actor>(anIter.Value()),
			    THighlightAction(true,aSelectionMode));
  }

  myMainWindow->Repaint();
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
  using namespace VTK;
  ForEach<SALOME_Actor>(getRenderer()->GetActors(),
			THighlightAction( false ));

  Repaint();
}

//----------------------------------------------------------------
void
SVTK_View
::highlight( const Handle(SALOME_InteractiveObject)& theIO, 
	     bool theIsHighlight, 
	     bool theIsUpdate ) 
{
  using namespace VTK;
  SVTK_Selector* aSelector = myMainWindow->GetSelector();
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>( theIO ),
			  THighlightAction( theIsHighlight, aSelector->SelectionMode() ));
  
  Repaint();
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

