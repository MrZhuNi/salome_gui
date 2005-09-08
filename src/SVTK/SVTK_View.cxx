#include "SALOME_Actor.h"

#include <qapplication.h>
#include <qpushbutton.h>

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
#include "SVTK_RenderWindow.h"
#include "SVTK_InteractorStyle.h"
#include "SVTK_RenderWindowInteractor.h"

#include "SALOME_ListIteratorOfListIO.hxx"

#include "SVTK_SelectorDef.h"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"

//----------------------------------------------------------------------------
SVTK_RendererHolder
::SVTK_RendererHolder(QWidget* theParent, 
		      const char* theName):
  QMainWindow(theParent,theName,0),
  myRenderer(SVTK_Renderer::New())
{
  myRenderer->Delete();
}

SVTK_RendererHolder
::~SVTK_RendererHolder()
{}

//----------------------------------------------------------------------------
vtkRenderer* 
SVTK_RendererHolder
::GetRenderer()
{
  return myRenderer.GetPointer();
}

//----------------------------------------------------------------------------
void
SVTK_RendererHolder
::setBackgroundColor(const QColor& theColor)
{
  myRenderer->SetBackground(theColor.red()/255.0, 
			    theColor.green()/255.0,
			    theColor.blue()/255.0);
}

//----------------------------------------------------------------------------
QColor
SVTK_RendererHolder
::backgroundColor() const
{
  float aBackgroundColor[3];
  myRenderer->GetBackground(aBackgroundColor);
  return QColor(int(aBackgroundColor[0]*255), 
		int(aBackgroundColor[1]*255), 
		int(aBackgroundColor[2]*255));
}

//----------------------------------------------------------------------------
void
SVTK_RendererHolder
::GetScale( double theScale[3] ) 
{
  myRenderer->GetScale( theScale );
}

void
SVTK_RendererHolder
::SetScale( double theScale[3] ) 
{
  myRenderer->SetScale( theScale );
  Repaint();
}


//----------------------------------------------------------------------------
void
SVTK_RendererHolder
::AddActor(VTKViewer_Actor* theActor, bool theIsUpdate)
{
  myRenderer->AddActor(theActor);
  if(theIsUpdate) 
    Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_RendererHolder
::RemoveActor(VTKViewer_Actor* theActor, bool theIsUpdate)
{
  myRenderer->RemoveActor(theActor);
  if(theIsUpdate) 
    Repaint();
}


//----------------------------------------------------------------------------
int
SVTK_RendererHolder
::GetTrihedronSize() const
{
  return myRenderer->GetTrihedronSize();
}

//----------------------------------------------------------------------------
void 
SVTK_RendererHolder
::SetTrihedronSize( const int theSize )
{
  myRenderer->SetTrihedronSize(theSize);
  Repaint();
}


//----------------------------------------------------------------------------
/*! If parameter theIsForcedUpdate is true, recalculate parameters for
 *  trihedron and cube axes, even if trihedron and cube axes is invisible.
 */
void
SVTK_RendererHolder
::AdjustTrihedrons(const bool theIsForced)
{
  myRenderer->AdjustTrihedrons(theIsForced);
  Repaint();
}

//----------------------------------------------------------------------------
bool
SVTK_RendererHolder
::isTrihedronDisplayed()
{
  return myRenderer->isTrihedronDisplayed();
}

//----------------------------------------------------------------------------
bool
SVTK_RendererHolder
::isCubeAxesDisplayed()
{
  return myRenderer->isCubeAxesDisplayed();
}

//----------------------------------------------------------------------------
VTKViewer_Trihedron*  
SVTK_RendererHolder
::GetTrihedron() 
{ 
  return myRenderer->GetTrihedron(); 
}

//----------------------------------------------------------------------------
SVTK_CubeAxesActor2D* 
SVTK_RendererHolder
::GetCubeAxes() 
{ 
  return myRenderer->GetCubeAxes(); 
}


//----------------------------------------------------------------------------
SVTK_ActionHolder
::SVTK_ActionHolder(QWidget* theParent, 
		    const char* theName,
		    SUIT_ResourceMgr* theResourceMgr) :
  SVTK_RendererHolder(theParent,theName)
{
  myToolBar = new QToolBar(this);
  myToolBar->setCloseMode(QDockWindow::Undocked);
  myToolBar->setLabel(tr("LBL_TOOLBAR_LABEL"));

  createActions(theResourceMgr);
  createToolBar();
}

//----------------------------------------------------------------------------
SVTK_ActionHolder
::~SVTK_ActionHolder()
{}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::SetEventDispatcher(vtkObject* theDispatcher)
{
  myEventDispatcher = theDispatcher;
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
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
SVTK_ActionHolder
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
QToolBar* 
SVTK_ActionHolder
::getToolBar()
{
  return myToolBar;
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::activateZoom()
{
  myEventDispatcher->InvokeEvent(SVTK::StartZoom,0);
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::activatePanning()
{
  myEventDispatcher->InvokeEvent(SVTK::StartPan,0);
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::activateRotation()
{
  myEventDispatcher->InvokeEvent(SVTK::StartRotate,0);
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::activateGlobalPanning()
{
  myEventDispatcher->InvokeEvent(SVTK::StartGlobalPan,0);
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::activateWindowFit()
{
  myEventDispatcher->InvokeEvent(SVTK::StartFitArea,0);
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onFrontView()
{
  myRenderer->onFrontView();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onBackView()
{
  myRenderer->onBackView();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onTopView()
{
  myRenderer->onTopView();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onBottomView()
{
  myRenderer->onBottomView();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onLeftView()
{
  myRenderer->onLeftView();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onRightView()
{
  myRenderer->onRightView();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onResetView()
{
  myRenderer->onResetView();
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onFitAll()
{
  myRenderer->onFitAll();
  Repaint();
}

//----------------------------------------------------------------------------
void 
SVTK_ActionHolder
::onViewTrihedron()
{
  myRenderer->onViewTrihedron();
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onViewCubeAxes()
{
  myRenderer->onViewCubeAxes();
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onAdjustTrihedron()
{   
  myRenderer->onAdjustTrihedron();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onAdjustCubeAxes()
{   
  myRenderer->onAdjustCubeAxes();
}

//----------------------------------------------------------------------------
void
SVTK_ActionHolder
::onDumpView()
{}

//----------------------------------------------------------------------------
QImage
SVTK_ActionHolder
::dumpView()
{
  QPixmap px = QPixmap::grabWindow( winId() );
  return px.convertToImage();
}


//----------------------------------------------------------------------------
SVTK_InteractorHolder
::SVTK_InteractorHolder(QWidget* theParent, 
			const char* theName,
			SUIT_ResourceMgr* theResourceMgr) :
  SVTK_ActionHolder(theParent,theName,theResourceMgr),
  myRenderWindow(SVTK_RenderWindow::New())
{
  myRenderWindow->Delete();

  myRenderWindow->AddRenderer(GetRenderer());

  myInteractor = new SVTK_RenderWindowInteractor(this,"SVTK_RenderWindowInteractor");

  SetInteractor(myInteractor);

  myInteractor->PushInteractorStyle(SVTK_InteractorStyle::New());
}

//----------------------------------------------------------------------------
SVTK_InteractorHolder
::~SVTK_InteractorHolder()
{}

//----------------------------------------------------------------------------
void
SVTK_InteractorHolder
::SetInteractor(SVTK_RenderWindowInteractor* theInteractor)
{
  myInteractor = theInteractor;

  if(myInteractor){
    connect(myInteractor,SIGNAL(KeyPressed(QKeyEvent*)),
	    this,SIGNAL(KeyPressed(QKeyEvent*)) );
    connect(myInteractor,SIGNAL(KeyReleased(QKeyEvent*)),
	    this,SIGNAL(KeyReleased(QKeyEvent*)));
    connect(myInteractor,SIGNAL(MouseButtonPressed(QMouseEvent*)),
	    this,SIGNAL(MouseButtonPressed(QMouseEvent*)));
    connect(myInteractor,SIGNAL(MouseButtonReleased(QMouseEvent*)),
	    this,SIGNAL(MouseButtonReleased(QMouseEvent*)));
    connect(myInteractor,SIGNAL(MouseDoubleClicked(QMouseEvent*)),
	    this,SIGNAL(MouseDoubleClicked(QMouseEvent*)));
    connect(myInteractor,SIGNAL(MouseMove(QMouseEvent*)),
	    this,SIGNAL(MouseMove(QMouseEvent*)));
    connect(myInteractor,SIGNAL(contextMenuRequested(QContextMenuEvent*)),
	    this,SIGNAL(contextMenuRequested(QContextMenuEvent*)));
    connect(myInteractor,SIGNAL(selectionChanged()),
	    this,SIGNAL(selectionChanged()));

    SetEventDispatcher(myInteractor->GetDevice());

    setCentralWidget(myInteractor);

    myInteractor->setFocusPolicy(StrongFocus);
    myInteractor->setFocus();
    setFocusProxy(myInteractor);

    myInteractor->SetRenderWindow(myRenderWindow.GetPointer());
  }
}

//----------------------------------------------------------------------------
SVTK_RenderWindowInteractor*
SVTK_InteractorHolder
::GetInteractor()
{
  return myInteractor;
}

//----------------------------------------------------------------------------
vtkRenderWindowInteractor*
SVTK_InteractorHolder
::GetRenderWindowInteractor()
{
  return myInteractor->GetDevice();
}

//----------------------------------------------------------------------------
void
SVTK_InteractorHolder
::Repaint( bool theUpdateTrihedron )
{
  if(theUpdateTrihedron) 
    myRenderer->onAdjustTrihedron();

  myInteractor->update();
}

//----------------------------------------------------------------------------
void
SVTK_InteractorHolder
::Repaint() 
{ 
  Repaint( true ); 
}

//----------------------------------------------------------------------------
void
SVTK_InteractorHolder
::InvokeEvent(unsigned long theEvent, void* theCallData)
{
  myInteractor->InvokeEvent(theEvent,theCallData);
}

//----------------------------------------------------------------------------
void
SVTK_InteractorHolder
::SetRenderWindow(vtkRenderWindow *theRenderWindow)
{
  myRenderWindow = theRenderWindow;

  if(myInteractor)
    myInteractor->SetRenderWindow(myRenderWindow.GetPointer());
}

//----------------------------------------------------------------------------
vtkRenderWindow*
SVTK_InteractorHolder
::GetRenderWindow()
{
  return myRenderWindow.GetPointer();
}

//----------------------------------------------------------------------------
SVTK_Selector* 
SVTK_InteractorHolder
::GetSelector() 
{ 
  return myInteractor->GetSelector(); 
}

void
SVTK_InteractorHolder
::SetSelector(SVTK_Selector* theSelector)
{ 
  myInteractor->SetSelector(theSelector);
}

//----------------------------------------------------------------------------
void
SVTK_InteractorHolder
::PushInteractorStyle(SVTK_InteractorStyle* theStyle)
{
  myInteractor->PushInteractorStyle(theStyle);
}

//----------------------------------------------------------------------------
void
SVTK_InteractorHolder
::PopInteractorStyle()
{
  myInteractor->PopInteractorStyle();
}


//----------------------------------------------------------------------------
SVTK_InteractorStyle* 
SVTK_InteractorHolder
::GetInteractorStyle()
{
  return myInteractor->GetInteractorStyle(); 
}


//----------------------------------------------------------------
void
SVTK_InteractorHolder
::SetSelectionMode(int theMode)
{
  myInteractor->SetSelectionMode(theMode);
}


//----------------------------------------------------------------
int
SVTK_InteractorHolder
::SelectionMode() const
{
  return myInteractor->SelectionMode();
}


//----------------------------------------------------------------
struct THighlightAction
{
  bool myIsHighlight;
  int mySelectionMode;
  THighlightAction( bool theIsHighlight, int theSelectionMode = ActorSelection ):
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

//----------------------------------------------------------------
void
SVTK_InteractorHolder
::onSelectionChanged()
{
  vtkActorCollection* anActors = GetRenderer()->GetActors();

  using namespace VTK;
  ForEach<SALOME_Actor>(anActors,
			THighlightAction( false ));

  const SALOME_ListIO& aListIO = GetSelector()->StoredIObjects();
  SALOME_ListIteratorOfListIO anIter(aListIO);
  int aSelectionMode = SelectionMode();
  for(; anIter.More(); anIter.Next()){
    ForEachIf<SALOME_Actor>(anActors,
			    TIsSameIObject<SALOME_Actor>(anIter.Value()),
			    THighlightAction(true,aSelectionMode));
  }

  Repaint();
}


//----------------------------------------------------------------------------
SVTK_View
::SVTK_View(QWidget* theParent, 
	    const char* theName,
	    SUIT_ResourceMgr* theResourceMgr) :
  SVTK_InteractorHolder(theParent,theName,theResourceMgr)
{}

//----------------------------------------------------------------------------
SVTK_View
::~SVTK_View()
{}

//----------------------------------------------------------------
void 
SVTK_View
::unHighlightAll() 
{
  using namespace VTK;
  ForEach<SALOME_Actor>(GetRenderer()->GetActors(),
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
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>( theIO ),
			  THighlightAction( theIsHighlight, GetSelector()->SelectionMode() ));
  
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
  vtkActorCollection* anActors = GetRenderer()->GetActors();
  anActors->InitTraversal();
  while( vtkActor* anActor = anActors->GetNextActor() )
  {
    if( SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast( anActor ) )
    {
      aSActor->getPointProperty()->SetColor( theRed, theGreen, theBlue );
      aSActor->getPointProperty()->SetLineWidth( theWidth );

      aSActor->getCellProperty()->SetColor( theRed, theGreen, theBlue );
      aSActor->getCellProperty()->SetLineWidth( theWidth );

      aSActor->getEdgeProperty()->SetColor( theRed, theGreen, theBlue );
      aSActor->getEdgeProperty()->SetLineWidth( theWidth );
    }
  }
}

//----------------------------------------------------------------------------
void
SVTK_View
::SetSelectionTolerance(const double& theTolNodes, 
			const double& theTolItems)
{
  vtkActorCollection* anActors = GetRenderer()->GetActors();
  anActors->InitTraversal();
  while( vtkActor* anActor = anActors->GetNextActor() )
  {
    if( SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast( anActor ) )
    {
      aSActor->getPointPicker()->SetTolerance( theTolNodes );
      aSActor->getCellPicker()->SetTolerance( theTolItems );
      aSActor->getCellRectPicker()->SetTolerance( theTolItems );
    }
  }
}

//----------------------------------------------------------------------------
bool
SVTK_View
::isInViewer(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
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
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
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
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
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
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,int>
			  (&SALOME_Actor::setDisplayMode,theMode));
}

//----------------------------------------------------------------------------
void
SVTK_View
::ChangeRepresentationToWireframe()
{
  ChangeRepresentationToWireframe(GetRenderer()->GetActors());
}

void
SVTK_View
::ChangeRepresentationToSurface()
{
  ChangeRepresentationToSurface(GetRenderer()->GetActors());
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
  ForEach<SALOME_Actor>(GetRenderer()->GetActors(),
			TErase());
  Repaint();
}

void
SVTK_View
::DisplayAll()
{ 
  using namespace VTK;
  ForEach<SALOME_Actor>(GetRenderer()->GetActors(),
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
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
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
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetVisibility<SALOME_Actor>(true));

  if(theIsUpdate)
    Repaint();
}

//----------------------------------------------------------------------------
struct TRemoveAction
{
  vtkRenderer* myRen;
  TRemoveAction(vtkRenderer* theRen): myRen(theRen){}
  void operator()(SALOME_Actor* theActor){
    myRen->RemoveActor(theActor);
  }
};

void
SVTK_View
::Remove(const Handle(SALOME_InteractiveObject)& theIObject, 
	 bool theIsUpdate)
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
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
  GetRenderer()->RemoveProp(theActor);
  if(theIsUpdate)
    Repaint();
}

void
SVTK_View
::RemoveAll(bool theIsUpdate)
{
  vtkRenderer* aRenderer = GetRenderer();
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
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
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
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
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
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
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
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor){
    float r,g,b;
    anActor->GetColor(r,g,b);
    return QColor(int(r*255),int(g*255),int(b*255));
  }

  return QColor(0,0,0);
}

