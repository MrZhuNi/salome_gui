#include "SALOME_Actor.h"

#include <qapplication.h>
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

#include "VTKViewer_Utilities.h"
#include "VTKViewer_CellRectPicker.h"

//#include "SVTK_View.h"
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
::SVTK_ViewWindow( SUIT_Desktop* theDesktop, 
		   SVTK_ViewModelBase* theModel)
  : SUIT_ViewWindow( theDesktop )
{
  QWidget* aCentralWidget = new QWidget( this );
  setCentralWidget( aCentralWidget );
  QBoxLayout* aLayout = new QVBoxLayout( aCentralWidget );
  aLayout->setAutoAdd( true );
  QSplitter* aSplitter = new QSplitter( Qt::Vertical, aCentralWidget );

  // Create SVTK_View (successor of RenderWindowInteractor).
  myView = new SVTK_View( aSplitter, "SVTK_View" );
  myView->Initialize();
  myView->setFocusPolicy( StrongFocus );
  myView->setFocus();

  /*
  SVTK_View* bottomView = new SVTK_View( aSplitter, "BottomView" );
  bottomView->getInteractor()->SetInteractorStyle( myInteractorStyle ); 
  bottomView->Initialize();
  */

  connect( myView, SIGNAL(KeyPressed( QKeyEvent* )),
           this,   SLOT(onKeyPressed( QKeyEvent* )) );
  connect( myView, SIGNAL(KeyReleased( QKeyEvent* )),
           this,   SLOT(onKeyReleased( QKeyEvent* )) );
  connect( myView, SIGNAL(MouseButtonPressed( QMouseEvent* )),
           this,   SLOT(onMousePressed( QMouseEvent* )) );
  connect( myView, SIGNAL(MouseButtonReleased( QMouseEvent* )),
           this,   SLOT(onMouseReleased( QMouseEvent* )) );
  connect( myView, SIGNAL(MouseDoubleClicked( QMouseEvent* )),
           this,   SLOT(onMouseDoubleClicked( QMouseEvent* )) );
  connect( myView, SIGNAL(MouseMove( QMouseEvent* )),
           this,   SLOT(onMouseMoving( QMouseEvent* )) );
  connect( myView, SIGNAL(contextMenuRequested( QContextMenuEvent * )),
           this,   SIGNAL(contextMenuRequested( QContextMenuEvent * )) );
  connect( myView, SIGNAL(selectionChanged()),
	   theModel,SLOT(onSelectionChanged()));

  onResetView();
  setFocusProxy( myView ); // send focus events to myView (fixes a bug
                           // when on module activation myView received focusOutEvent 
}

//----------------------------------------------------------------------------
SVTK_ViewWindow
::~SVTK_ViewWindow()
{}


//----------------------------------------------------------------------------
SVTK_InteractorStyle* 
SVTK_ViewWindow
::getInteractorStyle()
{
  return myView->getInteractorStyle();
}

//----------------------------------------------------------------------------
vtkRenderer*
SVTK_ViewWindow
::getRenderer()
{
  return myView->getRenderer();
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

SVTK_View* 
SVTK_ViewWindow
::getRenderWindow() 
{ 
  return myView; 
}

SVTK_View* 
SVTK_ViewWindow
::getRWInteractor() 
{ 
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
  return myView->SelectionMode();
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
  vtkActorCollection* anActors = myView->getRenderer()->GetActors();
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
SVTK_ViewWindow
::SetSelectionTolerance(const double& theTolNodes, 
			const double& theTolItems)
{
  vtkActorCollection* anActors = myView->getRenderer()->GetActors();
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
