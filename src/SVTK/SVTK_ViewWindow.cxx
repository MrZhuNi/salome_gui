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

#include "SVTK_SelectorDef.h"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"

//----------------------------------------------------------------------------
SVTK_ViewWindow
::SVTK_ViewWindow( SUIT_Desktop* theDesktop, 
		   SVTK_ViewModelBase* theModel)
  : SUIT_ViewWindow( theDesktop )
{
  mySelector = new SVTK_SelectorDef();
  connect(this,SIGNAL(selectionChanged()),theModel,SLOT(onSelectionChanged()));

  QWidget* aCentralWidget = new QWidget( this );
  setCentralWidget( aCentralWidget );
  QBoxLayout* aLayout = new QVBoxLayout( aCentralWidget );
  aLayout->setAutoAdd( true );
  QSplitter* aSplitter = new QSplitter( Qt::Vertical, aCentralWidget );

  // Create an interactor.
  myView = new SVTK_View( aSplitter, "SVTK_View" );
  /*
  SVTK_View* bottomView = new SVTK_View( aSplitter, "BottomView" );
  bottomView->getInteractor()->SetInteractorStyle( myInteractorStyle ); 
  bottomView->Initialize();
  */
  myInteractorStyle = SVTK_InteractorStyle::New();
  myInteractorStyle->setGUIWindow( myView );
  myInteractorStyle->setViewWindow( this );

  myView->SetInteractorStyle( myInteractorStyle );
  myView->SetSelector( mySelector );
  myView->Initialize();

  //merge with V2_2_0_VISU_improvements:myInteractorStyle->setTriedron( myTrihedron );
  myInteractorStyle->FindPokedRenderer( 0, 0 );

  SetSelectionMode( ActorSelection );

  myView->setFocusPolicy( StrongFocus );
  myView->setFocus();

  /*
  connect( myRenderWindow, SIGNAL(KeyPressed( QKeyEvent* )),
           this,           SLOT(onKeyPressed( QKeyEvent* )) );
  connect( myRenderWindow, SIGNAL(KeyReleased( QKeyEvent* )),
           this,           SLOT(onKeyReleased( QKeyEvent* )) );
  connect( myRenderWindow, SIGNAL(MouseButtonPressed( QMouseEvent* )),
           this,           SLOT(onMousePressed( QMouseEvent* )) );
  connect( myRenderWindow, SIGNAL(MouseButtonReleased( QMouseEvent* )),
           this,           SLOT(onMouseReleased( QMouseEvent* )) );
  connect( myRenderWindow, SIGNAL(MouseDoubleClicked( QMouseEvent* )),
           this,           SLOT(onMouseDoubleClicked( QMouseEvent* )) );
  connect( myRenderWindow, SIGNAL(MouseMove( QMouseEvent* )),
           this,           SLOT(onMouseMoving( QMouseEvent* )) );
  */

  connect( myInteractorStyle, SIGNAL(RenderWindowModified()),
           myView, SLOT(update()) );
  connect( myView, SIGNAL(contextMenuRequested( QContextMenuEvent * )),
           this, SIGNAL(contextMenuRequested( QContextMenuEvent * )) );

  onResetView();
}

//----------------------------------------------------------------------------
SVTK_ViewWindow
::~SVTK_ViewWindow()
{
  // In order to ensure that the interactor unregisters
  // this RenderWindow, we assign a NULL RenderWindow to 
  // it before deleting it.
  //myView->SetRenderWindow( NULL );
  //delete myView;
}

//----------------------------------------------------------------------------
vtkRenderer*
SVTK_ViewWindow
::getRenderer()
{
  return myView->getRenderer();
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
  //cout << "SVTK_ViewWindow::onSelectionChanged" << endl;
  myView->onSelectionChanged();
  emit selectionChanged();
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
/*bool
SVTK_ViewWindow
::ComputeTrihedronSize( double& theNewSize, double& theSize )
{
  // calculating diagonal of visible props of the renderer
  float aBndBox[ 6 ];
  myTrihedron->VisibilityOff();

  if ( ::ComputeVisiblePropBounds( myRenderer, aBndBox ) == 0 ) {
    aBndBox[ 1 ] = aBndBox[ 3 ] = aBndBox[ 5 ] = 100;
    aBndBox[ 0 ] = aBndBox[ 2 ] = aBndBox[ 4 ] = 0;
  }

  myTrihedron->VisibilityOn();
  float aLength = 0;
  static bool aCalcByDiag = false;
  if ( aCalcByDiag ) {
    aLength = sqrt( ( aBndBox[1]-aBndBox[0])*(aBndBox[1]-aBndBox[0] )+
                    ( aBndBox[3]-aBndBox[2])*(aBndBox[3]-aBndBox[2] )+
                    ( aBndBox[5]-aBndBox[4])*(aBndBox[5]-aBndBox[4] ) );
  } else {
    aLength = aBndBox[ 1 ]-aBndBox[ 0 ];
    aLength = max( ( aBndBox[ 3 ] - aBndBox[ 2 ] ),aLength );
    aLength = max( ( aBndBox[ 5 ] - aBndBox[ 4 ] ),aLength );
  }

  float aSizeInPercents = myTrihedronSize;

  static float EPS_SIZE = 5.0E-3;
  theSize = myTrihedron->GetSize();
  theNewSize = aLength * aSizeInPercents / 100.0;

  // if the new trihedron size have sufficient difference, then apply the value
  return fabs( theNewSize - theSize) > theSize * EPS_SIZE ||
         fabs( theNewSize-theSize ) > theNewSize * EPS_SIZE;
}*/

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

#define INCREMENT_FOR_OP 10

//=======================================================================
// name    : onPanLeft
// Purpose : Performs incremental panning to the left
//=======================================================================
void
SVTK_ViewWindow
::onPanLeft()
{
  myInteractorStyle->IncrementalPan( -INCREMENT_FOR_OP, 0 );
}

//=======================================================================
// name    : onPanRight
// Purpose : Performs incremental panning to the right
//=======================================================================
void
SVTK_ViewWindow
::onPanRight()
{
  myInteractorStyle->IncrementalPan( INCREMENT_FOR_OP, 0 );
}

//=======================================================================
// name    : onPanUp
// Purpose : Performs incremental panning to the top
//=======================================================================
void
SVTK_ViewWindow
::onPanUp()
{
  myInteractorStyle->IncrementalPan( 0, INCREMENT_FOR_OP );
}

//=======================================================================
// name    : onPanDown
// Purpose : Performs incremental panning to the bottom
//=======================================================================
void
SVTK_ViewWindow
::onPanDown()
{
  myInteractorStyle->IncrementalPan( 0, -INCREMENT_FOR_OP );
}

//=======================================================================
// name    : onZoomIn
// Purpose : Performs incremental zooming in
//=======================================================================
void
SVTK_ViewWindow
::onZoomIn()
{
  myInteractorStyle->IncrementalZoom( INCREMENT_FOR_OP );
}

//=======================================================================
// name    : onZoomOut
// Purpose : Performs incremental zooming out
//=======================================================================
void
SVTK_ViewWindow
::onZoomOut()
{
  myInteractorStyle->IncrementalZoom( -INCREMENT_FOR_OP );
}

//=======================================================================
// name    : onRotateLeft
// Purpose : Performs incremental rotating to the left
//=======================================================================
void
SVTK_ViewWindow
::onRotateLeft()
{
  myInteractorStyle->IncrementalRotate( -INCREMENT_FOR_OP, 0 );
}

//=======================================================================
// name    : onRotateRight
// Purpose : Performs incremental rotating to the right
//=======================================================================
void
SVTK_ViewWindow
::onRotateRight()
{
  myInteractorStyle->IncrementalRotate( INCREMENT_FOR_OP, 0 );
}

//=======================================================================
// name    : onRotateUp
// Purpose : Performs incremental rotating to the top
//=======================================================================
void
SVTK_ViewWindow
::onRotateUp()
{
  myInteractorStyle->IncrementalRotate( 0, -INCREMENT_FOR_OP );
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

//=======================================================================
// name    : onRotateDown
// Purpose : Performs incremental rotating to the bottom
//=======================================================================
void
SVTK_ViewWindow
::onRotateDown()
{
  myInteractorStyle->IncrementalRotate( 0, INCREMENT_FOR_OP );
}

//----------------------------------------------------------------------------
//void
//SVTK_ViewWindow
//::InsertActor( VTKViewer_Actor* theActor, bool theMoveInternalActors )
//{
//  myView->InsertActor( theActor, theMoveInternalActors );
//}

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
//void
//SVTK_ViewWindow
//::MoveActor( VTKViewer_Actor* theActor)
//{
//  myView->MoveActor( theActor );
//}

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
