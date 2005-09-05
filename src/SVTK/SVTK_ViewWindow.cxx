#include "SALOME_Actor.h"

#include <qapplication.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qlayout.h>

#include <vtkTextProperty.h>
#include <vtkActorCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>

#include "QtxAction.h"

#include "SUIT_Session.h"
#include "SUIT_ToolButton.h"
#include "SUIT_MessageBox.h"

#include "SUIT_Tools.h"
#include "SUIT_ResourceMgr.h"
#include "SUIT_Accel.h"

#include "VTKViewer_Utilities.h"
#include "VTKViewer_CellRectPicker.h"

#include "SVTK_Event.h"
#include "SVTK_ViewWindow.h"
#include "SVTK_ViewModelBase.h"
#include "SVTK_RenderWindowInteractor.h"
#include "SVTK_InteractorStyle.h"

#include "SALOME_ListIteratorOfListIO.hxx"

#include "SVTK_Selector.h"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"

//----------------------------------------------------------------------------
SVTK_ViewWindow
::SVTK_ViewWindow(SUIT_Desktop* theDesktop):
  SUIT_ViewWindow(theDesktop)
{}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::Initialize(SVTK_ViewModelBase* theModel)
{
  if(SUIT_ResourceMgr* aResourceMgr = SUIT_Session::session()->resourceMgr()){
    myView = new SVTK_View(this,"SVTK_View",aResourceMgr);
    setCentralWidget(myView);
    
    Initialize(myView,theModel);
  }
}


void
SVTK_ViewWindow
::Initialize(SVTK_View* theView,
	     SVTK_ViewModelBase* theModel)
{
  theView->Initialize();

  onResetView();

  connect(theView,SIGNAL(KeyPressed(QKeyEvent*)),
          this,SLOT(onKeyPressed(QKeyEvent*)) );
  connect(theView,SIGNAL(KeyReleased(QKeyEvent*)),
          this,SLOT(onKeyReleased(QKeyEvent*)));
  connect(theView,SIGNAL(MouseButtonPressed(QMouseEvent*)),
          this,SLOT(onMousePressed(QMouseEvent*)));
  connect(theView,SIGNAL(MouseButtonReleased(QMouseEvent*)),
          this,SLOT(onMouseReleased(QMouseEvent*)));
  connect(theView,SIGNAL(MouseDoubleClicked(QMouseEvent*)),
          this,SLOT(onMouseDoubleClicked(QMouseEvent*)));
  connect(theView,SIGNAL(MouseMove(QMouseEvent*)),
          this,SLOT(onMouseMoving(QMouseEvent*)));
  connect(theView,SIGNAL(contextMenuRequested(QContextMenuEvent*)),
          this,SIGNAL(contextMenuRequested(QContextMenuEvent *)));
  connect(theView,SIGNAL(selectionChanged()),
	  theModel,SLOT(onSelectionChanged()));
}

//----------------------------------------------------------------------------
SVTK_ViewWindow
::~SVTK_ViewWindow()
{}


//----------------------------------------------------------------------------
vtkRenderer*
SVTK_ViewWindow
::getRenderer()
{
  return myView->GetRenderer();
}

//----------------------------------------------------------------------------
SVTK_Selector* 
SVTK_ViewWindow
::GetSelector() 
{ 
  return myView->GetSelector(); 
}

//----------------------------------------------------------------------------
SVTK_View* 
SVTK_ViewWindow
::getView() { 
  return myView; 
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onFrontView()
{
  myView->onFrontView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onBackView()
{
  myView->onBackView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onTopView()
{
  myView->onTopView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onBottomView()
{
  myView->onBottomView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onLeftView()
{
  myView->onLeftView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onRightView()
{
  myView->onRightView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onResetView()
{
  myView->onResetView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onFitAll()
{
  myView->onFitAll();
}

//----------------------------------------------------------------
void
SVTK_ViewWindow
::onSelectionChanged()
{
  myView->onSelectionChanged();
}

//----------------------------------------------------------------
void
SVTK_ViewWindow
::SetSelectionMode(Selection_Mode theMode)
{
  myView->SetSelectionMode( theMode );
}

//----------------------------------------------------------------
Selection_Mode
SVTK_ViewWindow
::SelectionMode() const
{
  return Selection_Mode(myView->SelectionMode());
}

//----------------------------------------------------------------
void 
SVTK_ViewWindow
::unHighlightAll() 
{
  myView->unHighlightAll();
}

//----------------------------------------------------------------
void
SVTK_ViewWindow
::highlight( const Handle(SALOME_InteractiveObject)& theIO, 
	     bool theIsHighlight, 
	     bool theIsUpdate ) 
{
  myView->highlight( theIO, theIsHighlight, theIsUpdate );
}

//----------------------------------------------------------------
bool
SVTK_ViewWindow
::isInViewer( const Handle(SALOME_InteractiveObject)& theIO ) 
{
  return myView->isInViewer( theIO );
}

//----------------------------------------------------------------
bool
SVTK_ViewWindow
::isVisible( const Handle(SALOME_InteractiveObject)& theIO ) 
{
  return myView->isVisible( theIO );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::setBackgroundColor( const QColor& color )
{
  myView->setBackgroundColor( color );
}

//----------------------------------------------------------------------------
QColor
SVTK_ViewWindow
::backgroundColor() const
{
  return myView->backgroundColor();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::Repaint(bool theUpdateTrihedron)
{
  myView->Repaint( theUpdateTrihedron );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::GetScale( double theScale[3] ) 
{
  myView->GetScale( theScale );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::SetScale( double theScale[3] ) 
{
  myView->SetScale( theScale );
}

//----------------------------------------------------------------------------
bool
SVTK_ViewWindow
::isTrihedronDisplayed()
{
  return myView->isTrihedronDisplayed();
}

bool
SVTK_ViewWindow
::isCubeAxesDisplayed()
{
  return myView->isCubeAxesDisplayed();
}

//----------------------------------------------------------------------------
void 
SVTK_ViewWindow
::onViewTrihedron()
{
  myView->onViewTrihedron();
}

void
SVTK_ViewWindow
::onViewCubeAxes()
{
  myView->onViewCubeAxes();
}

//----------------------------------------------------------------------------
VTKViewer_Trihedron* SVTK_ViewWindow::GetTrihedron()
{
  return myView->GetTrihedron();
}

SVTK_CubeAxesActor2D* SVTK_ViewWindow::GetCubeAxes()
{
  return myView->GetCubeAxes();
}

int SVTK_ViewWindow::GetTrihedronSize() const
{
  return myView->GetTrihedronSize();
}

void SVTK_ViewWindow::SetTrihedronSize( const int sz )
{
  myView->SetTrihedronSize( sz );
}

/*! If parameter theIsForcedUpdate is true, recalculate parameters for
 *  trihedron and cube axes, even if trihedron and cube axes is invisible.
 */
void
SVTK_ViewWindow
::AdjustTrihedrons(const bool theIsForcedUpdate)
{
  myView->AdjustTrihedrons( theIsForcedUpdate );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onAdjustTrihedron()
{   
  myView->onAdjustTrihedron();
}

void
SVTK_ViewWindow
::onAdjustCubeAxes()
{   
  myView->onAdjustCubeAxes();
}

//=======================================================================
void
SVTK_ViewWindow
::onKeyPressed(QKeyEvent* event)
{
  emit keyPressed( this, event );
}

//=======================================================================
void
SVTK_ViewWindow
::onKeyReleased(QKeyEvent* event)
{
  emit keyReleased( this, event );
}

//=======================================================================
void
SVTK_ViewWindow
::onMousePressed(QMouseEvent* event)
{
  emit mousePressed(this, event);
}

//=======================================================================
void
SVTK_ViewWindow
::onMouseReleased(QMouseEvent* event)
{
  emit mouseReleased( this, event );
}

//=======================================================================
void
SVTK_ViewWindow
::onMouseMoving(QMouseEvent* event)
{
  emit mouseMoving( this, event );
}

//=======================================================================
void
SVTK_ViewWindow
::onMouseDoubleClicked( QMouseEvent* event )
{
  emit mouseDoubleClicked( this, event );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::AddActor( VTKViewer_Actor* theActor, bool theUpdate )
{
  myView->AddActor( theActor, theUpdate );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::RemoveActor( VTKViewer_Actor* theActor, bool theUpdate )
{
  myView->RemoveActor( theActor, theUpdate );
}

//----------------------------------------------------------------------------
QImage
SVTK_ViewWindow
::dumpView()
{
  return myView->dumpView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::SetSelectionProp(const double& theRed, 
		   const double& theGreen, 
		   const double& theBlue, 
		   const int& theWidth) 
{
  myView->SetSelectionProp(theRed,theGreen,theBlue,theWidth);
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::SetSelectionTolerance(const double& theTolNodes, 
			const double& theTolItems)
{
  myView->SetSelectionTolerance(theTolNodes,theTolItems);
}

//----------------------------------------------------------------------------
int convertAction( const int accelAction )
{
  switch ( accelAction ) {
  case SUIT_Accel::PanLeft     : return SVTK::PanLeftEvent;
  case SUIT_Accel::PanRight    : return SVTK::PanRightEvent;
  case SUIT_Accel::PanUp       : return SVTK::PanUpEvent;
  case SUIT_Accel::PanDown     : return SVTK::PanDownEvent;
  case SUIT_Accel::ZoomIn      : return SVTK::ZoomInEvent;
  case SUIT_Accel::ZoomOut     : return SVTK::ZoomOutEvent;
  case SUIT_Accel::RotateLeft  : return SVTK::RotateLeftEvent;
  case SUIT_Accel::RotateRight : return SVTK::RotateRightEvent;
  case SUIT_Accel::RotateUp    : return SVTK::RotateUpEvent;
  case SUIT_Accel::RotateDown  : return SVTK::RotateDownEvent;  
  }
  return accelAction;
}

//----------------------------------------------------------------------------
void 
SVTK_ViewWindow
::action( const int accelAction  )
{
  if ( accelAction == SUIT_Accel::ZoomFit )
    onFitAll();
  else {
    int svtkEvent = convertAction( accelAction );
    myView->InvokeEvent( svtkEvent, 0 );
  }
}
