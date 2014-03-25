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

#include "SVTK_NonIsometricDlg.h"
#include "SVTK_UpdateRateDlg.h"
#include "SVTK_CubeAxesDlg.h"
#include "SVTK_SetRotationPointDlg.h"
#include "SVTK_ViewParameterDlg.h"

#include "Plot3d_Actor.h"
#include "Plot3d_FitDataDlg.h"
#include "Plot3d_SetupSurfacesDlg.h"

#include "SALOME_Actor.h"

#include <QMenu>
#include <QToolBar>
#include <QEvent>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>

#include <vtkTextProperty.h>
#include <vtkActorCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkAxisActor2D.h>
#include <vtkGL2PSExporter.h>
#include <vtkInteractorStyle.h>
#include <vtkScalarBarActor.h>

#include "QtxAction.h"

#include "SUIT_Session.h"
#include "SUIT_MessageBox.h"
#include "SUIT_Accel.h"
#include "SUIT_Tools.h"
#include "SUIT_ResourceMgr.h"
#include "SUIT_Accel.h"
#include "SUIT_OverrideCursor.h"
#include "QtxActionToolMgr.h"
#include "QtxMultiAction.h"

#include "VTKViewer_Trihedron.h"
#include "VTKViewer_Utilities.h"

#include "SVTK_View.h"
//#include "SVTK_MainWindow.h"
#include "SVTK_Selector.h"

#include "SVTK_Event.h"
#include "SVTK_Renderer.h"
#include "SVTK_ViewWindow.h"
#include "SVTK_ViewModelBase.h"
#include "SVTK_InteractorStyle.h"
#include "SVTK_RenderWindowInteractor.h"
#include "SVTK_GenericRenderWindowInteractor.h"
#include "SVTK_CubeAxesActor2D.h"
#include "SVTK_ComboAction.h"
#include "SVTK_KeyFreeInteractorStyle.h"
#include "SVTK_Selector.h"
#include "SVTK_Recorder.h"
#include "SVTK_RecorderDlg.h"
#include "SVTK_FrameBuffer.h"

#include "SALOME_ListIteratorOfListIO.hxx"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"


namespace SVTK
{
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
}





/*!
  Constructor
*/
SVTK_ViewWindow::SVTK_ViewWindow(SUIT_Desktop* theDesktop):
  SUIT_ViewWindow(theDesktop),
  //myMainWindow(0),
  myView(NULL),
  myDumpImage(QImage()),
  myStandardInteractorStyle(SVTK_InteractorStyle::New()),
  myKeyFreeInteractorStyle(SVTK_KeyFreeInteractorStyle::New()),
  myViewsAction( NULL ),
  myMode2D( false ),
  myMode2DNormalAxis( AxisZ )
{
  setWindowFlags( windowFlags() & ~Qt::Window );
  // specific of vtkSmartPointer
  myStandardInteractorStyle->Delete();
  myKeyFreeInteractorStyle->Delete();
}

/*!
  To initialize #SVTK_ViewWindow instance
*/
void SVTK_ViewWindow::Initialize(SVTK_ViewModelBase* theModel)
{
  //  if(SUIT_ResourceMgr* aResourceMgr = SUIT_Session::session()->resourceMgr()){
    //myMainWindow = new SVTK_MainWindow(this,"SVTK_MainWindow",aResourceMgr,this);

    //SVTK_RenderWindowInteractor* anIteractor = 
    //  new SVTK_RenderWindowInteractor(myMainWindow,"SVTK_RenderWindowInteractor");
  myInteractor = new SVTK_RenderWindowInteractor(this,"SVTK_RenderWindowInteractor");
  
  SVTK_Selector* aSelector = SVTK_Selector::New();
  
  SVTK_GenericRenderWindowInteractor* aDevice = SVTK_GenericRenderWindowInteractor::New();
  aDevice->SetRenderWidget(myInteractor);
  aDevice->SetSelector(aSelector);
  
  SVTK_Renderer* aRenderer = SVTK_Renderer::New();
  aRenderer->Initialize(aDevice,aSelector);
  
  myInteractor->Initialize(aDevice,aRenderer,aSelector);
  
  aDevice->Delete();
  aRenderer->Delete();
  aSelector->Delete();
  
  //myMainWindow->Initialize(anIteractor);
  myToolBar = toolMgr()->createToolBar( tr("LBL_TOOLBAR_LABEL"), -1, this );
  myRecordingToolBar = toolMgr()->createToolBar( tr("LBL_TOOLBAR_RECORD_LABEL"), -1, this );
  
  if( SUIT_Session* aSession = SUIT_Session::session() )
  {
    createActions( aSession->activeApplication()->resourceMgr() );
    createToolBar();
  }
  
  SetEventDispatcher(myInteractor->GetDevice());
  myInteractor->setBackgroundRole( QPalette::NoRole );//NoBackground
  myInteractor->setFocusPolicy(Qt::StrongFocus);
  myInteractor->setFocus();
  setFocusProxy(myInteractor);
  
  myUpdateRateDlg = new SVTK_UpdateRateDlg( getAction( UpdateRate ), this, "SVTK_UpdateRateDlg" );
  myNonIsometricDlg = new SVTK_NonIsometricDlg( getAction( NonIsometric ), this, "SVTK_NonIsometricDlg" );
  myCubeAxesDlg = new SVTK_CubeAxesDlg( getAction( GraduatedAxes ), this, "SVTK_CubeAxesDlg" );
  mySetRotationPointDlg = new SVTK_SetRotationPointDlg
    ( getAction( ChangeRotationPointId ), this, "SVTK_SetRotationPointDlg" );
  myViewParameterDlg = new SVTK_ViewParameterDlg
    ( getAction( ViewParametersId ), this, "SVTK_ViewParameterDlg" );
  
  myInteractor->PushInteractorStyle(myStandardInteractorStyle);
  
  myRecorder = SVTK_Recorder::New();
  
  myRecorder->SetNbFPS( 17.3 );
  myRecorder->SetQuality( 100 );
  myRecorder->SetProgressiveMode( true );
  myRecorder->SetUseSkippedFrames( true );
  myRecorder->SetRenderWindow( myInteractor->getRenderWindow() );
  
  //setCentralWidget(myMainWindow);
  setCentralWidget(myInteractor);
  
  //myView = new SVTK_View(myMainWindow);
  myView = new SVTK_View(this);
  if(theModel)
    Initialize(myView,theModel);
  
  myInteractor->getRenderWindow()->Render();
  onResetView();
  //}
}

/*!
  To initialize #SVTK_ViewWindow instance
*/
void SVTK_ViewWindow::Initialize(SVTK_View* theView,
				 SVTK_ViewModelBase* theModel)
{
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

/*!
  Destructor
*/
SVTK_ViewWindow::~SVTK_ViewWindow()
{}

/*!
  Fill the context menu
  \param thePopup context menu
*/
void SVTK_ViewWindow::contextMenuPopup( QMenu* thePopup )
{
  if( myMode2D )
    thePopup->addAction( tr( "TOT_SVTK_FITDATA" ), this, SLOT( onFitData() ) );
}

/*!
  \return corresponding view
*/
SVTK_View* SVTK_ViewWindow::getView() 
{ 
  return myView; 
}

/*!
  \return corresponding vtk main window
*/
// SVTK_MainWindow* SVTK_ViewWindow::getMainWindow() 
// { 
//   return myMainWindow; 
// }

/*!
  \return corresponding vtk render window
*/
vtkRenderWindow* SVTK_ViewWindow::getRenderWindow()
{
  return GetInteractor()->getRenderWindow();
}

/*!
  \return corresponding vtk render window interactor
*/
SVTK_RenderWindowInteractor* SVTK_ViewWindow::GetInteractor() const
{
  return myInteractor;
}

/*!
  \return corresponding vtk render window interactor
*/
vtkRenderWindowInteractor* SVTK_ViewWindow::getInteractor() const
{
  return myInteractor->GetDevice();
}

/*!
  \return corresponding vtk renderer
*/
vtkRenderer* SVTK_ViewWindow::getRenderer() const
{
  return GetInteractor()->getRenderer();
}

/*!
  Redirect the request to SVTK_RenderWindowInteractor::GetRenderer
*/
SVTK_Renderer* SVTK_ViewWindow::GetRenderer() const
{
  return GetInteractor()->GetRenderer();
}

/*!
  \return corresponding vtk selector
*/
SVTK_Selector* SVTK_ViewWindow::GetSelector() const
{ 
  return GetInteractor()->GetSelector(); 
}

/*!
  Processes transformation "front view"
*/
void SVTK_ViewWindow::onFrontView()
{
  //myMainWindow->onFrontView();
  GetRenderer()->OnFrontView();
  Repaint();
}

/*!
  Processes transformation "back view"
*/
void SVTK_ViewWindow::onBackView()
{
  //myMainWindow->onBackView();
  GetRenderer()->OnBackView();
  Repaint();
}

/*!
  Processes transformation "top view"
*/
void SVTK_ViewWindow::onTopView()
{
  //myMainWindow->onTopView();
  GetRenderer()->OnTopView();
  Repaint();
}

/*!
  Processes transformation "bottom view"
*/
void SVTK_ViewWindow::onBottomView()
{
  //myMainWindow->onBottomView();
  GetRenderer()->OnBottomView();
  Repaint();
}

/*!
  Processes transformation "left view"
*/
void SVTK_ViewWindow::onLeftView()
{
  //myMainWindow->onLeftView();
  GetRenderer()->OnLeftView();
  Repaint();
}

/*!
  Processes transformation "right view"
*/
void SVTK_ViewWindow::onRightView()
{
  //myMainWindow->onRightView();
  GetRenderer()->OnRightView();
  Repaint();
}

/*!
  Processes transformation "reset view": sets default orientation of viewport camera
*/
void SVTK_ViewWindow::onResetView()
{
  //myMainWindow->onResetView();
  GetRenderer()->OnResetView();
  Repaint();
}

/*!
  Processes transformation "fit all"
*/
void SVTK_ViewWindow::onFitAll()
{
  //myMainWindow->onFitAll();
  GetRenderer()->OnFitAll();
  Repaint();
}

/*!
  SLOT: called if selection is changed
*/
void SVTK_ViewWindow::onSelectionChanged()
{
  myView->onSelectionChanged();
}

/*!
  Change selection mode
  \param theMode - new selection mode
*/
void SVTK_ViewWindow::SetSelectionMode(Selection_Mode theMode)
{
  //myMainWindow->SetSelectionMode( theMode );
  GetSelector()->SetSelectionMode(theMode);
}

/*!
  \return selection mode
*/
Selection_Mode SVTK_ViewWindow::SelectionMode() const
{
  //return myMainWindow->SelectionMode();
  return GetSelector()->SelectionMode();
}

/*!
  Unhilights all objects in viewer
*/
void SVTK_ViewWindow::unHighlightAll() 
{
  myView->unHighlightAll();
}

/*!
  Hilights/unhilights object in viewer
  \param theIO - object to be updated
  \param theIsHighlight - if it is true, object will be hilighted, otherwise it will be unhilighted
  \param theIsUpdate - update current viewer
*/
void SVTK_ViewWindow::highlight(const Handle(SALOME_InteractiveObject)& theIO, 
				bool theIsHighlight, 
				bool theIsUpdate ) 
{
  myView->highlight( theIO, theIsHighlight, theIsUpdate );
}

/*!
  \return true if object is in viewer or in collector
  \param theIO - object to be checked
*/
bool SVTK_ViewWindow::isInViewer( const Handle(SALOME_InteractiveObject)& theIO ) 
{
  return myView->isInViewer( theIO );
}

/*!
  \return true if object is displayed in viewer
  \param theIO - object to be checked
*/
bool SVTK_ViewWindow::isVisible( const Handle(SALOME_InteractiveObject)& theIO ) 
{
  return myView->isVisible( theIO );
}

/*!
  Display object
  \param theEntry - entry that corresponds to intractive objects
*/
Handle(SALOME_InteractiveObject) SVTK_ViewWindow::FindIObject(const char* theEntry) 
{
  return myView->FindIObject(theEntry);
}

/*!
  Display object
  \param theIO - object
  \param theImmediatly - update viewer
*/
void SVTK_ViewWindow::Display(const Handle(SALOME_InteractiveObject)& theIO,
			      bool theImmediatly) 
{
  myView->Display(theIO,theImmediatly);
}

/*!
  Erase object
  \param theIO - object
  \param theImmediatly - update viewer
*/
void SVTK_ViewWindow::Erase(const Handle(SALOME_InteractiveObject)& theIO,
			    bool theImmediatly) 
{
  myView->Erase(theIO,theImmediatly);
}

/*!
  Display only passed object
  \param theIO - object
*/
void SVTK_ViewWindow::DisplayOnly(const Handle(SALOME_InteractiveObject)& theIO) 
{
  myView->DisplayOnly(theIO);
}

/*!
  Display all objects in view
*/
void SVTK_ViewWindow::DisplayAll() 
{
  myView->DisplayAll();
}

/*!
  Erase all objects in view
*/
void SVTK_ViewWindow::EraseAll() 
{
  myView->EraseAll();
}

/*!
  Sets background color
  \param color - new background color
*/
void SVTK_ViewWindow::setBackgroundColor( const QColor& theColor )
{
  //myMainWindow->SetBackgroundColor( color );
  getRenderer()->SetBackground(theColor.red()/255.0, 
			       theColor.green()/255.0,
			       theColor.blue()/255.0);
}

/*!
  \return background color of viewer
*/
QColor SVTK_ViewWindow::backgroundColor() const
{
  //return myMainWindow->BackgroundColor();
  vtkFloatingPointType aBackgroundColor[3];
  getRenderer()->GetBackground(aBackgroundColor);
  return QColor(int(aBackgroundColor[0]*255), 
		int(aBackgroundColor[1]*255), 
		int(aBackgroundColor[2]*255));
}


/*!
  Redirect the request to SVTK_RenderWindowInteractor::GetInteractorStyle
*/
vtkInteractorStyle* SVTK_ViewWindow::GetInteractorStyle() const
{
  return GetInteractor()->GetInteractorStyle();
}

/*!
  Redirect the request to SVTK_RenderWindowInteractor::PushInteractorStyle
*/
void SVTK_ViewWindow::PushInteractorStyle(vtkInteractorStyle* theStyle)
{
  GetInteractor()->PushInteractorStyle(theStyle);
}

/*!
  Redirect the request to SVTK_RenderWindowInteractor::PopInteractorStyle
*/
void SVTK_ViewWindow::PopInteractorStyle()
{
  GetInteractor()->PopInteractorStyle();
}

/*!
  Updates current viewer
*/
void SVTK_ViewWindow::Repaint(bool theUpdateTrihedron)
{
  //myMainWindow->Repaint( theUpdateTrihedron );
  if(theUpdateTrihedron) 
    GetRenderer()->OnAdjustTrihedron();

  GetInteractor()->update();

  SVTK_InteractorStyle* aStyle = (SVTK_InteractorStyle*)getInteractor()->GetInteractorStyle();
  if ( aStyle )
    aStyle->OnTimer();
}

/*!
  Redirect the request to #SVTK_Renderer::GetScale
*/
void SVTK_ViewWindow::GetScale( double theScale[3] ) 
{
  //myMainWindow->GetScale( theScale );
  GetRenderer()->GetScale( theScale );
}

/*!
  Redirect the request to #SVTK_Renderer::SetScale
*/
void SVTK_ViewWindow::SetScale( double theScale[3] ) 
{
  //myMainWindow->SetScale( theScale );
  GetRenderer()->SetScale( theScale );
  Repaint();
}

/*!
  Redirect the request to #SVTK_Renderer::IsTrihedronDisplayed
*/
bool SVTK_ViewWindow::isTrihedronDisplayed()
{
  return GetRenderer()->IsTrihedronDisplayed();
  //  return myMainWindow->IsTrihedronDisplayed();
}

/*!
  Redirect the request to #SVTK_Renderer::IsCubeAxesDisplayed
*/
bool SVTK_ViewWindow::isCubeAxesDisplayed()
{
  //return myMainWindow->IsCubeAxesDisplayed();
  return GetRenderer()->IsCubeAxesDisplayed();
}

/*!
  Redirect the request to #SVTK_Renderer::OnViewTrihedron
*/
void SVTK_ViewWindow::onViewTrihedron()
{
  //myMainWindow->onViewTrihedron();
  GetRenderer()->OnViewTrihedron();
  Repaint();
}

/*!
  Redirect the request to #SVTK_Renderer::OnViewCubeAxes
*/
void SVTK_ViewWindow::onViewCubeAxes()
{
  //myMainWindow->onViewCubeAxes();
  GetRenderer()->OnViewCubeAxes();
  Repaint();
}

/*!
  Redirect the request to #SVTK_Renderer::GetTrihedron
*/
VTKViewer_Trihedron* SVTK_ViewWindow::GetTrihedron()
{
  //return myMainWindow->GetTrihedron();
  return GetRenderer()->GetTrihedron();
}

/*!
  Redirect the request to #SVTK_Renderer::GetCubeAxes
*/
SVTK_CubeAxesActor2D* SVTK_ViewWindow::GetCubeAxes()
{
  //return myMainWindow->GetCubeAxes();
  return GetRenderer()->GetCubeAxes();
}

/*!
  \return trihedron size
*/
vtkFloatingPointType SVTK_ViewWindow::GetTrihedronSize() const
{
  //return myMainWindow->GetTrihedronSize();
  return GetRenderer()->GetTrihedronSize();
}

/*!
  Sets projection mode
  \param theMode - projection mode ( 0 - orthogonal, 1 - perspective )
*/
void SVTK_ViewWindow::SetProjectionMode(const int theMode)
{
  activateProjectionMode( theMode );
}


/*!
  Set the gravity center as a focal point
*/
void SVTK_ViewWindow::activateSetFocalPointGravity()
{
  myEventDispatcher->InvokeEvent(SVTK::SetFocalPointGravity, 0);
}

/*!
  Set the selected point as a focal point
*/
void SVTK_ViewWindow::activateSetFocalPointSelected()
{
  myEventDispatcher->InvokeEvent(SVTK::SetFocalPointSelected, 0);
}

/*!
  Set the point selected by user as a focal point
*/
void SVTK_ViewWindow::activateStartFocalPointSelection()
{
  myEventDispatcher->InvokeEvent(SVTK::StartFocalPointSelection,0);
}

void SVTK_ViewWindow::activateProjectionMode(int theMode)
{
  SVTK_ComboAction* a = ::qobject_cast<SVTK_ComboAction*>( toolMgr()->action( ProjectionModeId ) );
  if ( a ) a->setCurrentIndex(theMode);
}

/*!
  Sets actual interaction style
  \param theStyle - type of interaction style ( 0 - standard, 1 - keyboard free )
*/
void SVTK_ViewWindow::SetInteractionStyle(const int theStyle)
{
  onSwitchInteractionStyle( theStyle==1 );
}

/*!
  Switches "keyboard free" interaction style on/off
*/
void SVTK_ViewWindow::onSwitchInteractionStyle(bool theOn)
{
  if (theOn) {
    // check if style is already set
    if ( GetInteractorStyle() != myKeyFreeInteractorStyle.GetPointer() )
    {
      // keep the same style extensions
      SVTK_InteractorStyle* aStyle = (SVTK_InteractorStyle*)GetInteractorStyle();
      if ( aStyle ) {
	myKeyFreeInteractorStyle->SetControllerIncrement(aStyle->ControllerIncrement());
	myKeyFreeInteractorStyle->SetControllerOnKeyDown(aStyle->ControllerOnKeyDown());
      }

      PushInteractorStyle(myKeyFreeInteractorStyle.GetPointer());
    }
  }
  else {
    // pop only key free  style
    if ( GetInteractorStyle() == myKeyFreeInteractorStyle.GetPointer() )
      PopInteractorStyle();
  }

  // update action state if method is called outside
  QtxAction* a = getAction( SwitchInteractionStyleId );
  if ( a->isChecked() != theOn ) a->setChecked( theOn );
}

/*!
  Toggle 2D mode on/off
*/
void SVTK_ViewWindow::onMode2D( bool theOn )
{
  bool anIsModeChanged = theOn != myMode2D;
  myMode2D = theOn;

  if( getAction( ViewTrihedronId ) )
    getAction( ViewTrihedronId )->setVisible( !theOn );
  if( getAction( ViewTrihedronId ) )
    getAction( ViewTrihedronId )->setVisible( !theOn );
  if( getAction( ChangeRotationPointId ) )
    getAction( ChangeRotationPointId )->setVisible( !theOn );
  if( myViewsAction )
    myViewsAction->setVisible( !theOn );
  if( getAction( ViewTrihedronId ) )
    getAction( ViewTrihedronId )->setVisible( !theOn );

  SVTK_ComboAction* a = ::qobject_cast<SVTK_ComboAction*>( toolMgr()->action( ProjectionModeId ) );
  if( a )
    a->setVisible( !theOn );

  if( theOn )
  {
    myCubeAxesDlg->SetDimensionXEnabled( myMode2DNormalAxis != AxisX );
    myCubeAxesDlg->SetDimensionYEnabled( myMode2DNormalAxis != AxisY );
    myCubeAxesDlg->SetDimensionZEnabled( myMode2DNormalAxis != AxisZ );
    if( SVTK_CubeAxesActor2D* aCubeAxes = GetRenderer()->GetCubeAxes() )
    {
      aCubeAxes->SetIsInvertedGrid( true );
      if( vtkAxisActor2D* aXAxis = aCubeAxes->GetXAxisActor2D() )
      {
        aXAxis->SetTitleVisibility( myMode2DNormalAxis != AxisX );
        aXAxis->SetLabelVisibility( myMode2DNormalAxis != AxisX );
        aXAxis->SetTickVisibility( myMode2DNormalAxis != AxisX );
      }
      if( vtkAxisActor2D* aYAxis = aCubeAxes->GetYAxisActor2D() )
      {
        aYAxis->SetTitleVisibility( myMode2DNormalAxis != AxisY );
        aYAxis->SetLabelVisibility( myMode2DNormalAxis != AxisY );
        aYAxis->SetTickVisibility( myMode2DNormalAxis != AxisY );
      }
      if( vtkAxisActor2D* aZAxis = aCubeAxes->GetZAxisActor2D() )
      {
        aZAxis->SetTitleVisibility( myMode2DNormalAxis != AxisZ );
        aZAxis->SetLabelVisibility( myMode2DNormalAxis != AxisZ );
        aZAxis->SetTickVisibility( myMode2DNormalAxis != AxisZ );
      }
    }

    if( anIsModeChanged )
      storeViewState( myStored3DViewState );
    if( !restoreViewState( myStored2DViewState ) )
    {
      // first time the action is toggled
      GetRenderer()->SetTrihedronDisplayed( false );
      switch( myMode2DNormalAxis )
      {
        case AxisX: onFrontView(); break;
        case AxisY: onLeftView(); break;
        case AxisZ: onTopView(); break;
      }
      onFitAll();
    }

    myStandardInteractorStyle->SetIsRotationEnabled( false );
    myKeyFreeInteractorStyle->SetIsRotationEnabled( false );
  }
  else
  {
    myCubeAxesDlg->SetDimensionXEnabled( true );
    myCubeAxesDlg->SetDimensionYEnabled( true );
    myCubeAxesDlg->SetDimensionZEnabled( true );
    if( SVTK_CubeAxesActor2D* aCubeAxes = GetRenderer()->GetCubeAxes() )
    {
      aCubeAxes->SetIsInvertedGrid( false );
      if( vtkAxisActor2D* aXAxis = aCubeAxes->GetXAxisActor2D() )
      {
        aXAxis->SetTitleVisibility( 1 );
        aXAxis->SetLabelVisibility( 1 );
        aXAxis->SetTickVisibility( 1 );
      }
      if( vtkAxisActor2D* aYAxis = aCubeAxes->GetYAxisActor2D() )
      {
        aYAxis->SetTitleVisibility( 1 );
        aYAxis->SetLabelVisibility( 1 );
        aYAxis->SetTickVisibility( 1 );
      }
      if( vtkAxisActor2D* aZAxis = aCubeAxes->GetZAxisActor2D() )
      {
        aZAxis->SetTitleVisibility( 1 );
        aZAxis->SetLabelVisibility( 1 );
        aZAxis->SetTickVisibility( 1 );
      }
    }

    storeViewState( myStored2DViewState );
    restoreViewState( myStored3DViewState );

    myStandardInteractorStyle->SetIsRotationEnabled( true );
    myKeyFreeInteractorStyle->SetIsRotationEnabled( true );
  }
}

/*!
  Check that 2D mode is active
  \return true if 2D mode is active
*/
bool SVTK_ViewWindow::isMode2D() const
{
  return myMode2D;
}

/*!
  Store 2D/3D view state
  \param theViewState - view state to be stored
*/
void SVTK_ViewWindow::storeViewState( ViewState& theViewState )
{
  vtkCamera* aCamera = getRenderer()->GetActiveCamera();

  theViewState.IsInitialized = true;

  aCamera->GetPosition( theViewState.Position );
  aCamera->GetFocalPoint( theViewState.FocalPoint );
  aCamera->GetViewUp( theViewState.ViewUp );
  theViewState.ParallelScale = aCamera->GetParallelScale();

  theViewState.IsTrihedronDisplayed = GetRenderer()->IsTrihedronDisplayed();
}

/*!
  Restore 2D/3D view state
  \param theViewState - view state to be restored
  \return true if the view state is initialized and can be restored
*/
bool SVTK_ViewWindow::restoreViewState( ViewState theViewState )
{
  vtkCamera* aCamera = getRenderer()->GetActiveCamera();
  if( theViewState.IsInitialized )
  {
    GetRenderer()->SetTrihedronDisplayed( theViewState.IsTrihedronDisplayed );

    aCamera->SetPosition( theViewState.Position );
    aCamera->SetFocalPoint( theViewState.FocalPoint );
    aCamera->SetViewUp( theViewState.ViewUp );
    aCamera->SetParallelScale( theViewState.ParallelScale );
    Repaint();

    return true;
  }
  return false;
}

/*!
  Clear 2D/3D view state
  \param theIs2D flag used to indicate which state has to be cleared
*/
void SVTK_ViewWindow::clearViewState( const bool theIs2D )
{
  if( theIs2D )
    myStored2DViewState.IsInitialized = false;
  else
    myStored3DViewState.IsInitialized = false;
}

/*!
  Sets incremental speed
  \param theValue - new incremental speed
  \param theMode - modification mode
*/
void SVTK_ViewWindow::SetIncrementalSpeed(const int theValue, const int theMode)
{
  //myMainWindow->SetIncrementalSpeed(theValue, theMode);
  if ( (SVTK_InteractorStyle*)GetInteractorStyle() )
    ((SVTK_InteractorStyle*)GetInteractorStyle())->SetIncrementSpeed(theValue, theMode);
}

/*!
  Sets spacemouse buttons for the functions
  \param theBtn1 - spacemouse button for the "decrease speed increment"
  \param theBtn2 - spacemouse button for the "increase speed increment"
  \param theBtn3 - spacemouse button for the "dominant combined switch"
*/
void SVTK_ViewWindow::SetSpacemouseButtons(const int theBtn1, 
					   const int theBtn2,
					   const int theBtn3)
{
  //myMainWindow->SetSMButtons(theBtn1, theBtn2, theBtn3);
  int val = theBtn1;
  myEventDispatcher->InvokeEvent(SVTK::SetSMDecreaseSpeedEvent, &val);
  val = theBtn2;
  myEventDispatcher->InvokeEvent(SVTK::SetSMIncreaseSpeedEvent, &val);
  val = theBtn3;
  myEventDispatcher->InvokeEvent(SVTK::SetSMDominantCombinedSwitchEvent, &val);
}

/*!
  Sets trihedron size
  \param theSize - new trihedron size
  \param theRelative - trihedron relativeness
*/
void SVTK_ViewWindow::SetTrihedronSize(const vtkFloatingPointType theSize, const bool theRelative)
{
  //myMainWindow->SetTrihedronSize(theSize, theRelative);
  GetRenderer()->SetTrihedronSize(theSize, theRelative);
  Repaint();
}

/*! If parameter theIsForcedUpdate is true, recalculate parameters for
 *  trihedron and cube axes, even if trihedron and cube axes is invisible.
 */
void SVTK_ViewWindow::AdjustTrihedrons(const bool theIsForcedUpdate)
{
  //myMainWindow->AdjustActors();
  GetRenderer()->AdjustActors();
  Repaint();
}

/*!
  Redirect the request to #SVTK_Renderer::OnAdjustTrihedron
*/
void SVTK_ViewWindow::onAdjustTrihedron()
{   
  //myMainWindow->onAdjustTrihedron();
  GetRenderer()->OnAdjustTrihedron();
}

/*!
  Redirect the request to #SVTK_Renderer::OnAdjustCubeAxes
*/
void SVTK_ViewWindow::onAdjustCubeAxes()
{   
  //myMainWindow->onAdjustCubeAxes();
  GetRenderer()->OnAdjustCubeAxes();
}

/*!
  Emits key pressed
*/
void SVTK_ViewWindow::onKeyPressed(QKeyEvent* event)
{
  emit keyPressed( this, event );
}

/*!
  Emits key released
*/
void SVTK_ViewWindow::onKeyReleased(QKeyEvent* event)
{
  emit keyReleased( this, event );
}

/*!
  Emits mouse pressed
*/
void SVTK_ViewWindow::onMousePressed(QMouseEvent* event)
{
  emit mousePressed(this, event);
}

/*!
  Emits mouse released
*/
void SVTK_ViewWindow::onMouseReleased(QMouseEvent* event)
{
  emit mouseReleased( this, event );
}

/*!
  Emits mouse moving
*/
void SVTK_ViewWindow::onMouseMoving(QMouseEvent* event)
{
  emit mouseMoving( this, event );
}

/*!
  Emits mouse double clicked
*/
void SVTK_ViewWindow::onMouseDoubleClicked( QMouseEvent* event )
{
  emit mouseDoubleClicked( this, event );
}

/*!
  Redirect the request to #SVTK_Renderer::AddActor
*/
void SVTK_ViewWindow::AddActor( VTKViewer_Actor* theActor, 
				bool theUpdate )
{
  //myMainWindow->AddActor( theActor, theUpdate );
  GetRenderer()->AddActor(theActor);
  if(theUpdate) 
    Repaint();
  emit actorAdded(theActor);
}

/*!
  Redirect the request to #SVTK_Renderer::RemoveActor
*/
void SVTK_ViewWindow::RemoveActor( VTKViewer_Actor* theActor, 
				   bool theUpdate )
{
  //myMainWindow->RemoveActor( theActor, theUpdate );
  GetRenderer()->RemoveActor(theActor);
  if(theUpdate) 
    Repaint();
  emit actorRemoved(theActor);
}

/*!
  Auxiliary method intended to dump contents of the view to an image
*/
QImage SVTK_ViewWindow::dumpViewContent()
{
  vtkRenderWindow* aWindow = getRenderWindow();
  int* aSize = aWindow->GetSize();
  int aWidth = aSize[0];
  int aHeight = aSize[1];
  
  SVTK_FrameBuffer aFrameBuffer;
  if( aFrameBuffer.init( aWidth, aHeight ) )
  {
    glPushAttrib( GL_VIEWPORT_BIT );
    glViewport( 0, 0, aWidth, aHeight );
    aFrameBuffer.bind();

    // draw scene
    aWindow->Render();

    aFrameBuffer.unbind();
    glPopAttrib();

    QImage anImage( aWidth, aHeight, QImage::Format_RGB32 );

    aFrameBuffer.bind();
    glReadPixels( 0, 0, aWidth, aHeight, GL_RGBA, GL_UNSIGNED_BYTE, anImage.bits() );
    aFrameBuffer.unbind();

    anImage = anImage.rgbSwapped();
    anImage = anImage.mirrored();
    return anImage;
  }

  // if frame buffers are unsupported, use old functionality
  unsigned char *aData = 
    aWindow->GetRGBACharPixelData( 0, 0, aWidth-1, aHeight-1, 0 );
  
  QImage anImage( aData, aWidth, aHeight, QImage::Format_ARGB32 );

  anImage = anImage.rgbSwapped();
  anImage = anImage.mirrored();
  return anImage;
}

/*!
  \return QImage, containing all scene rendering in window
*/
QImage SVTK_ViewWindow::dumpView()
{
  if( myDumpImage.isNull() )
    return dumpViewContent();

  RefreshDumpImage();
  return myDumpImage;
}

QString SVTK_ViewWindow::filter() const
{
  return tr( "SVTK_IMAGE_FILES" );
}

bool SVTK_ViewWindow::dumpViewToFormat( const QImage& img, const QString& fileName, const QString& format )
{
  if ( format != "PS" && format != "EPS" && format != "PDF" )
    return SUIT_ViewWindow::dumpViewToFormat( img, fileName, format );

  SUIT_OverrideCursor wc;

  vtkGL2PSExporter *anExporter = vtkGL2PSExporter::New();
  anExporter->SetRenderWindow(getRenderWindow());

  if ( format == "PS" ) {
    anExporter->SetFileFormatToPS();
    anExporter->CompressOff();
  }

  if ( format == "EPS" ) {
    anExporter->SetFileFormatToEPS();
    anExporter->CompressOff();
  }

  if ( format == "PDF" ) {
    anExporter->SetFileFormatToPDF();
  }

  QString aFilePrefix(fileName);
  QString anExtension(SUIT_Tools::extension(fileName));
  aFilePrefix.truncate(aFilePrefix.length() - 1 - anExtension.length());
  anExporter->SetFilePrefix(aFilePrefix.toLatin1().data());
  anExporter->Write();
  anExporter->Delete();

  return true;
}

/*!
  \refresh QImage, containing all scene rendering in window
*/
void SVTK_ViewWindow::RefreshDumpImage()
{
  myDumpImage = dumpViewContent();
}

/*!
  Redirect the request to #SVTK_Renderer::SetSelectionProp
*/
void SVTK_ViewWindow::SetSelectionProp(const double& theRed, 
				       const double& theGreen, 
				       const double& theBlue, 
				       const int& theWidth) 
{
  myView->SetSelectionProp(theRed,theGreen,theBlue,theWidth);
}

/*!
  Redirect the request to #SVTK_Renderer::SetSelectionProp
*/
void SVTK_ViewWindow::SetPreselectionProp(const double& theRed, 
					  const double& theGreen, 
					  const double& theBlue, 
					  const int& theWidth) 
{
  myView->SetPreselectionProp(theRed,theGreen,theBlue,theWidth);
}

/*!
  Redirect the request to #SVTK_Renderer::SetSelectionTolerance
*/
void SVTK_ViewWindow::SetSelectionTolerance(const double& theTolNodes, 
					    const double& theTolItems,
					    const double& theTolObjects)
{
  myView->SetSelectionTolerance(theTolNodes, theTolItems, theTolObjects);
}

/*!
  Performs action
  \param accelAction - action
*/
bool SVTK_ViewWindow::action( const int accelAction  )
{
  //  if ( !myMainWindow->hasFocus() )
  //    return false;
  if ( accelAction == SUIT_Accel::ZoomFit )
    onFitAll();
  else {
    int anEvent = SVTK::convertAction( accelAction );
    //myMainWindow->InvokeEvent( anEvent, 0 );
    GetInteractor()->InvokeEvent(anEvent, 0);
  }
  return true;
}

/*!
  \return action by it's id
*/
QtxAction* SVTK_ViewWindow::getAction( int id ) const
{
  return dynamic_cast<QtxAction*>( toolMgr()->action( id ) );
}


// old visual parameters had 13 values.  New format added additional 
// 76 values for graduated axes, so both numbers are processed.
const int nNormalParams = 13;   // number of view windows parameters excluding graduated axes params
const int nGradAxisParams = 25; // number of parameters of ONE graduated axis (X, Y, or Z)
const int nAllParams = nNormalParams + 3*nGradAxisParams + 1; // number of all visual parameters

/*! The method returns visual parameters of a graduated axis actor (x,y,z axis of graduated axes)
 */
void getGradAxisVisualParams( QXmlStreamWriter& writer, vtkAxisActor2D* actor, QString theAxis )
{
  //QString params;
  if ( !actor )
    return ;//params;

  // Name
  bool isVisible = actor->GetTitleVisibility();
  QString title ( actor->GetTitle() );
  vtkFloatingPointType color[ 3 ];
  int font = VTK_ARIAL;
  int bold = 0;
  int italic = 0;
  int shadow = 0;

  vtkTextProperty* txtProp = actor->GetTitleTextProperty();
  if ( txtProp )
  {
    txtProp->GetColor( color );
    font = txtProp->GetFontFamily();
    bold = txtProp->GetBold();
    italic = txtProp->GetItalic();
    shadow = txtProp->GetShadow();
  }
  writer.writeStartElement("GraduatedAxis");
  writer.writeAttribute("Axis", theAxis);

  writer.writeStartElement("Title");
  writer.writeAttribute("isVisible", QString("%1").arg(isVisible));
  writer.writeAttribute("Text", title);
  writer.writeAttribute("Font", QString("%1").arg(font));
  writer.writeAttribute("Bold", QString("%1").arg(bold));
  writer.writeAttribute("Italic", QString("%1").arg(italic));
  writer.writeAttribute("Shadow", QString("%1").arg(shadow));

  writer.writeStartElement("Color");
  writer.writeAttribute("R", QString("%1").arg(color[0]));
  writer.writeAttribute("G", QString("%1").arg(color[1]));
  writer.writeAttribute("B", QString("%1").arg(color[2]));
  writer.writeEndElement();
  writer.writeEndElement();

  //params.sprintf( "* Graduated Axis: * Name *%u*%s*%.2f*%.2f*%.2f*%u*%u*%u*%u", isVisible, 
  //		  title.toLatin1().data(), color[0], color[1], color[2], font, bold, italic, shadow );

  // Labels
  isVisible = actor->GetLabelVisibility();
  int labels = actor->GetNumberOfLabels();
  int offset = actor->GetTickOffset();
  font = VTK_ARIAL;
  bold = false;
  italic = false;
  shadow = false;

  txtProp = actor->GetLabelTextProperty();
  if ( txtProp )
  {
    txtProp->GetColor( color );
    font = txtProp->GetFontFamily();
    bold = txtProp->GetBold();
    italic = txtProp->GetItalic();
    shadow = txtProp->GetShadow();
  }

  writer.writeStartElement("Labels");
  writer.writeAttribute("isVisible", QString("%1").arg(isVisible));
  writer.writeAttribute("Number", QString("%1").arg(labels));
  writer.writeAttribute("Offset", QString("%1").arg(offset));
  writer.writeAttribute("Font", QString("%1").arg(font));
  writer.writeAttribute("Bold", QString("%1").arg(bold));
  writer.writeAttribute("Italic", QString("%1").arg(italic));
  writer.writeAttribute("Shadow", QString("%1").arg(shadow));

  writer.writeStartElement("Color");
  writer.writeAttribute("R", QString("%1").arg(color[0]));
  writer.writeAttribute("G", QString("%1").arg(color[1]));
  writer.writeAttribute("B", QString("%1").arg(color[2]));
  writer.writeEndElement();
  writer.writeEndElement();
  //  params += QString().sprintf( "* Labels *%u*%u*%u*%.2f*%.2f*%.2f*%u*%u*%u*%u", isVisible, labels, offset,  
  //			       color[0], color[1], color[2], font, bold, italic, shadow );

  // Tick marks
  isVisible = actor->GetTickVisibility();
  int length = actor->GetTickLength();
  writer.writeStartElement("TickMarks");
  writer.writeAttribute("isVisible", QString("%1").arg(isVisible));
  writer.writeAttribute("Length", QString("%1").arg(length));
  writer.writeEndElement();
  
  //params += QString().sprintf( "* Tick marks *%u*%u", isVisible, length );
  
  writer.writeEndElement();
  //return params;
}

void setGradAxisVisualParams(QXmlStreamReader& reader, vtkAxisActor2D* actor)
{
  if ( !actor )
    return;

  do {
    reader.readNext();
  } while (!reader.isStartElement());

  // Read title params
  QXmlStreamAttributes aAttr = reader.attributes();
  bool isVisible = aAttr.value("isVisible").toString().toUShort();
  QString title = aAttr.value("Text").toString();
  int font = aAttr.value("Font").toString().toInt();
  int bold = aAttr.value("Bold").toString().toInt();
  int italic = aAttr.value("Italic").toString().toInt();
  int shadow = aAttr.value("Shadow").toString().toInt();

  //printf("#### TITLE: %i, %s, %i, %i, %i, %i\n", isVisible, qPrintable(title), font, bold, italic, shadow);

  do {
    reader.readNext();
  } while (!reader.isStartElement());
  
  // Read title color
  aAttr = reader.attributes();

  vtkFloatingPointType color[3];
  color[0] = aAttr.value("R").toString().toDouble();
  color[1] = aAttr.value("G").toString().toDouble();
  color[2] = aAttr.value("B").toString().toDouble();
  //printf("#### Color: %f, %f, %f\n", color[0], color[1], color[2]);

  actor->SetTitleVisibility( isVisible );
  actor->SetTitle( title.toLatin1() );
  vtkTextProperty* txtProp = actor->GetTitleTextProperty();
  if ( txtProp ) {
    txtProp->SetColor( color );
    txtProp->SetFontFamily( font );
    txtProp->SetBold( bold );
    txtProp->SetItalic( italic );
    txtProp->SetShadow( shadow );
  }

  // Labels

  do {
    reader.readNext();
  } while (!reader.isStartElement()); 
  // Read labels
  aAttr = reader.attributes();
  isVisible = aAttr.value("isVisible").toString().toUShort();
  int labels = aAttr.value("Number").toString().toInt();
  int offset = aAttr.value("Offset").toString().toInt();
  font = aAttr.value("Font").toString().toInt();
  bold = aAttr.value("Bold").toString().toInt();
  italic = aAttr.value("Italic").toString().toInt();
  shadow = aAttr.value("Shadow").toString().toInt();

  do {
    reader.readNext();
  } while (!reader.isStartElement()); 
  // Read Color
  aAttr = reader.attributes();

  color[0] = aAttr.value("R").toString().toDouble();
  color[1] = aAttr.value("G").toString().toDouble();
  color[2] = aAttr.value("B").toString().toDouble();

  actor->SetLabelVisibility( isVisible );
  actor->SetNumberOfLabels( labels );
  actor->SetTickOffset( offset );
  txtProp = actor->GetLabelTextProperty();
  if ( txtProp ) {
    txtProp->SetColor( color );
    txtProp->SetFontFamily( font );
    txtProp->SetBold( bold );
    txtProp->SetItalic( italic );
    txtProp->SetShadow( shadow );
  }

  // Tick Marks
  do {
    reader.readNext();
  } while (!reader.isStartElement()); 
  aAttr = reader.attributes();

  // retrieve and set tick marks properties
  isVisible = aAttr.value("isVisible").toString().toUShort();
  int length = aAttr.value("Length").toString().toInt();
  
  actor->SetTickVisibility( isVisible );
  actor->SetTickLength( length );
}

/*! The method restores visual parameters of a graduated axis actor (x,y,z axis)
 */
void setGradAxisVisualParams( vtkAxisActor2D* actor, const QString& params )
{
  if ( !actor )
    return;

  QStringList paramsLst = params.split( '*' );

  if ( paramsLst.size() == nGradAxisParams ) { // altogether name, lable, ticks parameters make up 25 values

    // retrieve and set name parameters
    bool isVisible = paramsLst[2].toUShort();
    QString title = paramsLst[3];
    vtkFloatingPointType color[3];
    color[0] = paramsLst[4].toDouble();
    color[1] = paramsLst[5].toDouble();
    color[2] = paramsLst[6].toDouble();
    int font = paramsLst[7].toInt();
    int bold = paramsLst[8].toInt();
    int italic = paramsLst[9].toInt();
    int shadow = paramsLst[10].toInt();

    actor->SetTitleVisibility( isVisible );
    actor->SetTitle( title.toLatin1() );
    vtkTextProperty* txtProp = actor->GetTitleTextProperty();
    if ( txtProp ) {
      txtProp->SetColor( color );
      txtProp->SetFontFamily( font );
      txtProp->SetBold( bold );
      txtProp->SetItalic( italic );
      txtProp->SetShadow( shadow );
    }

    // retrieve and set lable parameters
    isVisible = paramsLst[12].toUShort();
    int labels = paramsLst[13].toInt();
    int offset = paramsLst[14].toInt();
    color[0] = paramsLst[15].toDouble();
    color[1] = paramsLst[16].toDouble();
    color[2] = paramsLst[17].toDouble();
    font = paramsLst[18].toInt();
    bold = paramsLst[19].toInt();
    italic = paramsLst[20].toInt();
    shadow = paramsLst[21].toInt();

    actor->SetLabelVisibility( isVisible );
    actor->SetNumberOfLabels( labels );
    actor->SetTickOffset( offset );
    txtProp = actor->GetLabelTextProperty();
    if ( txtProp ) {
      txtProp->SetColor( color );
      txtProp->SetFontFamily( font );
      txtProp->SetBold( bold );
      txtProp->SetItalic( italic );
      txtProp->SetShadow( shadow );
    }

    // retrieve and set tick marks properties
    isVisible = paramsLst[23].toUShort();
    int length = paramsLst[24].toInt();

    actor->SetTickVisibility( isVisible );
    actor->SetTickLength( length );
  }
}

/*! The method returns the visual parameters of this view as a formated string
 */
QString SVTK_ViewWindow::getVisualParameters()
{
  double pos[3], focalPnt[3], viewUp[3], parScale, scale[3];
  
  // save position, focal point, viewUp, scale
  vtkCamera* camera = getRenderer()->GetActiveCamera();
  camera->GetPosition( pos );
  camera->GetFocalPoint( focalPnt );
  camera->GetViewUp( viewUp );
  parScale = camera->GetParallelScale();
  GetScale( scale );

  QColor aBgColor = backgroundColor();

  int aThrihedronVisibility = VTKViewer_Trihedron::eOn;
  if( VTKViewer_Trihedron* aTrihedron = GetTrihedron() )
    aThrihedronVisibility = (int)aTrihedron->GetVisibility();

  // Parameters are given in the following format:view position (3 digits), focal point position (3 digits)
  // view up values (3 digits), parallel scale (1 digit), scale (3 digits, 
  // Graduated axes parameters (X, Y, Z axes parameters)
  QString retStr;
  QXmlStreamWriter aWriter(&retStr);
  aWriter.setAutoFormatting(true);

  aWriter.writeStartDocument();
  aWriter.writeStartElement("ViewState");

  aWriter.writeStartElement("Position");
  aWriter.writeAttribute("X", QString("%1").arg(pos[0]));
  aWriter.writeAttribute("Y", QString("%1").arg(pos[1]));
  aWriter.writeAttribute("Z", QString("%1").arg(pos[2]));
  aWriter.writeEndElement();

  aWriter.writeStartElement("FocalPoint");
  aWriter.writeAttribute("X", QString::number(focalPnt[0]));
  aWriter.writeAttribute("Y", QString::number(focalPnt[1]));
  aWriter.writeAttribute("Z", QString::number(focalPnt[2]));
  aWriter.writeEndElement();

  aWriter.writeStartElement("ViewUp");
  aWriter.writeAttribute("X", QString::number(viewUp[0]));
  aWriter.writeAttribute("Y", QString::number(viewUp[1]));
  aWriter.writeAttribute("Z", QString::number(viewUp[2]));
  aWriter.writeEndElement();

  aWriter.writeStartElement("ViewScale");
  aWriter.writeAttribute("Parallel", QString::number(parScale));
  aWriter.writeAttribute("X", QString::number(scale[0]));
  aWriter.writeAttribute("Y", QString::number(scale[1]));
  aWriter.writeAttribute("Z", QString::number(scale[2]));
  aWriter.writeEndElement();

  if ( SVTK_CubeAxesActor2D* gradAxesActor = GetCubeAxes() ) {
    aWriter.writeStartElement("DisplayCubeAxis");
    aWriter.writeAttribute("Show", QString( "%1" ).arg( GetRenderer()->IsCubeAxesDisplayed()));
    aWriter.writeEndElement();

    getGradAxisVisualParams(aWriter, gradAxesActor->GetXAxisActor2D(), "X");
    getGradAxisVisualParams(aWriter, gradAxesActor->GetYAxisActor2D(), "Y");
    getGradAxisVisualParams(aWriter, gradAxesActor->GetZAxisActor2D(), "Z");
  }

  aWriter.writeStartElement("BackgroundColor");
  aWriter.writeAttribute("R", QString::number(aBgColor.red()));
  aWriter.writeAttribute("G", QString::number(aBgColor.green()));
  aWriter.writeAttribute("B", QString::number(aBgColor.blue()));
  aWriter.writeEndElement();

  aWriter.writeStartElement("Trihedron");
  aWriter.writeAttribute("Visibility", QString::number(aThrihedronVisibility));
  aWriter.writeEndElement();

  aWriter.writeEndElement();
  aWriter.writeEndDocument();

//   retStr.sprintf( "%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e", 
// 		  pos[0], pos[1], pos[2], focalPnt[0], focalPnt[1], focalPnt[2], 
// 		  viewUp[0], viewUp[1], viewUp[2], parScale, scale[0], scale[1], scale[2] );

//   // save graduated axes parameters
//   if ( SVTK_CubeAxesActor2D* gradAxesActor = GetCubeAxes() ) {
//     retStr += QString( "*%1" ).arg( GetRenderer()->IsCubeAxesDisplayed() );
//     retStr += ::getGradAxisVisualParams( gradAxesActor->GetXAxisActor2D() );
//     retStr += ::getGradAxisVisualParams( gradAxesActor->GetYAxisActor2D() );
//     retStr += ::getGradAxisVisualParams( gradAxesActor->GetZAxisActor2D() );
//   }

  return retStr;
}

/*!
  The method restores visual parameters of this view or postpones it untill the view is shown
*/ 
void SVTK_ViewWindow::setVisualParameters( const QString& parameters )
{
  //printf("#### %s\n", qPrintable(parameters));
  SVTK_RenderWindowInteractor* anInteractor = GetInteractor();
  if ( anInteractor->isVisible() ) {
    doSetVisualParameters( parameters ); 
  }
  else {
    myVisualParams = parameters;
    anInteractor->installEventFilter(this);
  }
}

/*!
  The method restores visual parameters of this view from a formated string
*/
void SVTK_ViewWindow::doSetVisualParameters( const QString& parameters )
{
  double pos[3], focalPnt[3], viewUp[3], parScale, scale[3];
  QColor aBgColor;

  QXmlStreamReader aReader(parameters);
  SVTK_CubeAxesActor2D* gradAxesActor = GetCubeAxes();

  while(!aReader.atEnd()) {
    aReader.readNext();
    if (aReader.isStartElement()) {
      QXmlStreamAttributes aAttr = aReader.attributes();
      //printf("### Name = %s\n", qPrintable(aReader.name().toString()));
      if (aReader.name() == "Position") {	
	pos[0] = aAttr.value("X").toString().toDouble();
	pos[1] = aAttr.value("Y").toString().toDouble();
	pos[2] = aAttr.value("Z").toString().toDouble();
	//printf("#### Position %f; %f; %f\n", pos[0], pos[1], pos[2]);
      } else if (aReader.name() == "FocalPoint") {
	focalPnt[0] = aAttr.value("X").toString().toDouble();
	focalPnt[1] = aAttr.value("Y").toString().toDouble();
	focalPnt[2] = aAttr.value("Z").toString().toDouble();
	//printf("#### FocalPoint %f; %f; %f\n", focalPnt[0], focalPnt[1], focalPnt[2]);
      } else if (aReader.name() == "ViewUp") {
	viewUp[0] = aAttr.value("X").toString().toDouble();
	viewUp[1] = aAttr.value("Y").toString().toDouble();
	viewUp[2] = aAttr.value("Z").toString().toDouble();
	//printf("#### ViewUp %f; %f; %f\n", viewUp[0], viewUp[1], viewUp[2]);
      } else if (aReader.name() == "ViewScale") {
	parScale = aAttr.value("Parallel").toString().toDouble();
	scale[0] = aAttr.value("X").toString().toDouble();
	scale[1] = aAttr.value("Y").toString().toDouble();
	scale[2] = aAttr.value("Z").toString().toDouble();
	//printf("#### ViewScale %f; %f; %f\n", scale[0], scale[1], scale[2]);
      } else if (aReader.name() == "DisplayCubeAxis") {
	if (aAttr.value("Show") == "0")
	  gradAxesActor->VisibilityOff();
	else
	  gradAxesActor->VisibilityOn();
      } else if (aReader.name() == "GraduatedAxis") {
	if(aAttr.value("Axis") == "X") 
	  setGradAxisVisualParams(aReader, gradAxesActor->GetXAxisActor2D());
	else if(aAttr.value("Axis") == "Y")
	  setGradAxisVisualParams(aReader, gradAxesActor->GetYAxisActor2D());
	else if(aAttr.value("Axis") == "Z")
	  setGradAxisVisualParams(aReader, gradAxesActor->GetZAxisActor2D());
      } else if (aReader.name() == "BackgroundColor") {
        aBgColor.setRed(aAttr.value("R").toString().toInt());
        aBgColor.setGreen(aAttr.value("G").toString().toInt());
        aBgColor.setBlue(aAttr.value("B").toString().toInt());
      } else if (aReader.name() == "Trihedron") {
        int aThrihedronVisibility = aAttr.value("Visibility").toString().toInt();
        if( VTKViewer_Trihedron* aTrihedron = GetTrihedron() )
          aTrihedron->SetVisibility((VTKViewer_Trihedron::TVisibility)aThrihedronVisibility);
      }
    } 
  }
  if (!aReader.hasError()) {
    vtkCamera* camera = getRenderer()->GetActiveCamera();
    camera->SetPosition( pos );
    camera->SetFocalPoint( focalPnt );
    camera->SetViewUp( viewUp );
    camera->SetParallelScale( parScale );
    SetScale( scale );
    setBackgroundColor( aBgColor );
  } else {
    QStringList paramsLst = parameters.split( '*' );
    if ( paramsLst.size() >= nNormalParams ) {
      // 'reading' list of parameters
      pos[0] = paramsLst[0].toDouble();
      pos[1] = paramsLst[1].toDouble();
      pos[2] = paramsLst[2].toDouble();
      focalPnt[0] = paramsLst[3].toDouble();
      focalPnt[1] = paramsLst[4].toDouble();
      focalPnt[2] = paramsLst[5].toDouble();
      viewUp[0] = paramsLst[6].toDouble();
      viewUp[1] = paramsLst[7].toDouble();
      viewUp[2] = paramsLst[8].toDouble();
      parScale = paramsLst[9].toDouble();
      scale[0] = paramsLst[10].toDouble();
      scale[1] = paramsLst[11].toDouble();
      scale[2] = paramsLst[12].toDouble();
      
      // applying parameters
      vtkCamera* camera = getRenderer()->GetActiveCamera();
      camera->SetPosition( pos );
      camera->SetFocalPoint( focalPnt );
      camera->SetViewUp( viewUp );
      camera->SetParallelScale( parScale );
      SetScale( scale );
      
      // apply graduated axes parameters
      SVTK_CubeAxesActor2D* gradAxesActor = GetCubeAxes();
      if ( gradAxesActor && paramsLst.size() == nAllParams ) {
	
	int i = nNormalParams+1, j = i + nGradAxisParams - 1;
	::setGradAxisVisualParams( gradAxesActor->GetXAxisActor2D(), parameters.section( '*', i, j ) ); 
	i = j + 1; j += nGradAxisParams;
	::setGradAxisVisualParams( gradAxesActor->GetYAxisActor2D(), parameters.section( '*', i, j ) ); 
	i = j + 1; j += nGradAxisParams;
	::setGradAxisVisualParams( gradAxesActor->GetZAxisActor2D(), parameters.section( '*', i, j ) ); 
	
	if ( paramsLst[13].toUShort() )
	  gradAxesActor->VisibilityOn();
	else
	  gradAxesActor->VisibilityOff();
      }
    }
  }
}


/*!
  Delayed setVisualParameters
*/
bool SVTK_ViewWindow::eventFilter( QObject* theWatched, QEvent* theEvent )
{
  if ( theEvent->type() == QEvent::Show && theWatched->inherits( "SVTK_RenderWindowInteractor" ) ) {
    SVTK_RenderWindowInteractor* anInteractor = (SVTK_RenderWindowInteractor*)theWatched;
    if ( anInteractor->isVisible() ) {
      doSetVisualParameters( myVisualParams );
      anInteractor->removeEventFilter( this ); // theWatched = RenderWindowInteractor
    }
  }
  return SUIT_ViewWindow::eventFilter( theWatched, theEvent );
}


/*!
  Change rotation point
*/
void SVTK_ViewWindow::onChangeRotationPoint(bool theIsActivate)
{
  if(theIsActivate){
    mySetRotationPointDlg->addObserver();
    if ( mySetRotationPointDlg->IsFirstShown() )
      activateSetRotationGravity();
    mySetRotationPointDlg->show();
  }else
    mySetRotationPointDlg->hide();
}

/*!
  Set the gravity center as a rotation point
*/
void SVTK_ViewWindow::activateSetRotationGravity()
{
  myEventDispatcher->InvokeEvent(SVTK::SetRotateGravity,0);
}

/*!
  Set the selected point as a rotation point
*/
void SVTK_ViewWindow::activateSetRotationSelected(void* theData)
{
  myEventDispatcher->InvokeEvent(SVTK::ChangeRotationPoint,theData);
}

/*!
  Set the point selected by user as a rotation point
*/
void SVTK_ViewWindow::activateStartPointSelection()
{
  myEventDispatcher->InvokeEvent(SVTK::StartPointSelection,0);
}

/*!
  Set the view projection mode: orthogonal or perspective
*/
void SVTK_ViewWindow::onProjectionMode(int mode)
{
  vtkCamera* aCamera = getRenderer()->GetActiveCamera();
  aCamera->SetParallelProjection(mode==0);
  GetInteractor()->GetDevice()->CreateTimer(VTKI_TIMER_FIRST);
}

void SVTK_ViewWindow::SetEventDispatcher(vtkObject* theDispatcher)
{
  myEventDispatcher = theDispatcher;
}

/*!
  Creates all actions of svtk main window
*/
void SVTK_ViewWindow::createActions(SUIT_ResourceMgr* theResourceMgr)
{
  QtxAction* anAction;
  QtxActionToolMgr* mgr = toolMgr();

  // Dump view
  anAction = new QtxAction(tr("MNU_DUMP_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_DUMP" ) ),
			   tr( "MNU_DUMP_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_DUMP_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onDumpView()));
  mgr->registerAction( anAction, DumpId );

  // FitAll
  anAction = new QtxAction(tr("MNU_FITALL"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FITALL" ) ),
			   tr( "MNU_FITALL" ), 0, this);
  anAction->setStatusTip(tr("DSC_FITALL"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onFitAll()));
  mgr->registerAction( anAction, FitAllId );

  // FitRect
  anAction = new QtxAction(tr("MNU_FITRECT"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FITAREA" ) ),
			   tr( "MNU_FITRECT" ), 0, this);
  anAction->setStatusTip(tr("DSC_FITRECT"));
  connect(anAction, SIGNAL(activated()), this, SLOT(activateWindowFit()));
  mgr->registerAction( anAction, FitRectId );

  // Zoom
  anAction = new QtxAction(tr("MNU_ZOOM_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_ZOOM" ) ),
			   tr( "MNU_ZOOM_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_ZOOM_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(activateZoom()));
  mgr->registerAction( anAction, ZoomId );

  // Panning
  anAction = new QtxAction(tr("MNU_PAN_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_PAN" ) ),
			   tr( "MNU_PAN_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_PAN_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(activatePanning()));
  mgr->registerAction( anAction, PanId );

  // Global Panning
  anAction = new QtxAction(tr("MNU_GLOBALPAN_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_GLOBALPAN" ) ),
			   tr( "MNU_GLOBALPAN_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_GLOBALPAN_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(activateGlobalPanning()));
  mgr->registerAction( anAction, GlobalPanId );

  // Change rotation point
  anAction = new QtxAction(tr("MNU_CHANGINGROTATIONPOINT_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_ROTATION_POINT" ) ),
			   tr( "MNU_CHANGINGROTATIONPOINT_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_CHANGINGROTATIONPOINT_VIEW"));
  anAction->setCheckable(true);
  connect(anAction, SIGNAL(toggled(bool)), this, SLOT(onChangeRotationPoint(bool)));
  mgr->registerAction( anAction, ChangeRotationPointId );

  // Rotation
  anAction = new QtxAction(tr("MNU_ROTATE_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_ROTATE" ) ),
			   tr( "MNU_ROTATE_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_ROTATE_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(activateRotation()));
  mgr->registerAction( anAction, RotationId );

  // Projections
  anAction = new QtxAction(tr("MNU_FRONT_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FRONT" ) ),
			   tr( "MNU_FRONT_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_FRONT_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onFrontView()));
  mgr->registerAction( anAction, FrontId );

  anAction = new QtxAction(tr("MNU_BACK_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_BACK" ) ),
			   tr( "MNU_BACK_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_BACK_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onBackView()));
  mgr->registerAction( anAction, BackId );

  anAction = new QtxAction(tr("MNU_TOP_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_TOP" ) ),
			   tr( "MNU_TOP_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_TOP_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onTopView()));
  mgr->registerAction( anAction, TopId );

  anAction = new QtxAction(tr("MNU_BOTTOM_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_BOTTOM" ) ),
			   tr( "MNU_BOTTOM_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_BOTTOM_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onBottomView()));
  mgr->registerAction( anAction, BottomId );

  anAction = new QtxAction(tr("MNU_LEFT_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_LEFT" ) ),
			   tr( "MNU_LEFT_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_LEFT_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onLeftView()));
  mgr->registerAction( anAction, LeftId );

  anAction = new QtxAction(tr("MNU_RIGHT_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_RIGHT" ) ),
			   tr( "MNU_RIGHT_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_RIGHT_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onRightView()));
  mgr->registerAction( anAction, RightId );

  // Reset
  anAction = new QtxAction(tr("MNU_RESET_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_RESET" ) ),
			   tr( "MNU_RESET_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_RESET_VIEW"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onResetView()));
  mgr->registerAction( anAction, ResetId );

  // onViewTrihedron: Shows - Hides Trihedron
  anAction = new QtxAction(tr("MNU_SHOW_TRIHEDRON"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_TRIHEDRON" ) ),
			   tr( "MNU_SHOW_TRIHEDRON" ), 0, this);
  anAction->setStatusTip(tr("DSC_SHOW_TRIHEDRON"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onViewTrihedron()));
  mgr->registerAction( anAction, ViewTrihedronId );

  // onNonIsometric: Manage non-isometric params
  anAction = new QtxAction(tr("MNU_SVTK_SCALING"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_SCALING" ) ),
			   tr( "MNU_SVTK_SCALING" ), 0, this);
  anAction->setStatusTip(tr("DSC_SVTK_SCALING"));
  anAction->setCheckable(true);
  connect(anAction, SIGNAL(toggled(bool)), this, SLOT(onNonIsometric(bool)));
  mgr->registerAction( anAction, NonIsometric );

  // onGraduatedAxes: Manage graduated axes params
  anAction = new QtxAction(tr("MNU_SVTK_GRADUATED_AXES"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_GRADUATED_AXES" ) ),
			   tr( "MNU_SVTK_GRADUATED_AXES" ), 0, this);
  anAction->setStatusTip(tr("DSC_SVTK_GRADUATED_AXES"));
  anAction->setCheckable(true);
  connect(anAction, SIGNAL(toggled(bool)), this, SLOT(onGraduatedAxes(bool)));
  mgr->registerAction( anAction, GraduatedAxes );

  // onGraduatedAxes: Manage graduated axes params
  anAction = new QtxAction(tr("MNU_SVTK_UPDATE_RATE"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_UPDATE_RATE" ) ),
			   tr( "MNU_SVTK_UPDATE_RATE" ), 0, this);
  anAction->setStatusTip(tr("DSC_SVTK_UPDATE_RATE"));
  anAction->setCheckable(true);
  connect(anAction, SIGNAL(toggled(bool)), this, SLOT(onUpdateRate(bool)));
  mgr->registerAction( anAction, UpdateRate );

  // Set projection mode
  SVTK_ComboAction* aModeAction = new SVTK_ComboAction(tr("MNU_SVTK_PROJECTION_MODE"), this);
  aModeAction->setStatusTip(tr("DSC_SVTK_PROJECTION_MODE"));
  aModeAction->insertItem(theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_VIEW_PARALLEL" ) ) );
  aModeAction->insertItem(theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_VIEW_PERSPECTIVE" ) ) );
  connect(aModeAction, SIGNAL(triggered(int)), this, SLOT(onProjectionMode(int)));
  mgr->registerAction( aModeAction, ProjectionModeId );

  // View Parameters
  anAction = new QtxAction(tr("MNU_VIEWPARAMETERS_VIEW"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_VIEW_PARAMETERS" ) ),
			   tr( "MNU_VIEWPARAMETERS_VIEW" ), 0, this);
  anAction->setStatusTip(tr("DSC_VIEWPARAMETERS_VIEW"));
  anAction->setCheckable(true);
  connect(anAction, SIGNAL(toggled(bool)), this, SLOT(onViewParameters(bool)));
  mgr->registerAction( anAction, ViewParametersId );

  // Switch between 3D (default) and 2D modes
  anAction = new QtxAction(tr("MNU_SVTK_MODE_2D"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_MODE_2D" ) ),
			   tr( "MNU_SVTK_MODE_2D" ), 0, this);
  anAction->setStatusTip(tr("DSC_SVTK_MODE_2D"));
  anAction->setCheckable(true);
  connect(anAction, SIGNAL(toggled(bool)), this, SLOT(onMode2D(bool)));
  mgr->registerAction( anAction, Mode2DId );

  // Switch between interaction styles
  anAction = new QtxAction(tr("MNU_SVTK_STYLE_SWITCH"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_STYLE_SWITCH" ) ),
			   tr( "MNU_SVTK_STYLE_SWITCH" ), 0, this);
  anAction->setStatusTip(tr("DSC_SVTK_STYLE_SWITCH"));
  anAction->setCheckable(true);
  connect(anAction, SIGNAL(toggled(bool)), this, SLOT(onSwitchInteractionStyle(bool)));
  mgr->registerAction( anAction, SwitchInteractionStyleId );

  // Surfaces settings
  anAction = new QtxAction(tr("MNU_SVTK_SURFACES_SETTINGS"), 
			   theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_SURFACES_SETTINGS" ) ),
			   tr( "MNU_SVTK_SURFACES_SETTINGS" ), 0, this);
  anAction->setStatusTip(tr("DSC_SVTK_SURFACES_SETTINGS"));
  connect(anAction, SIGNAL(activated()), this, SLOT(onSurfacesSettings()));
  mgr->registerAction( anAction, SurfacesSettingsId );

  // Start recording
  myStartAction = new QtxAction(tr("MNU_SVTK_RECORDING_START"), 
				theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_RECORDING_START" ) ),
				tr( "MNU_SVTK_RECORDING_START" ), 0, this);
  myStartAction->setStatusTip(tr("DSC_SVTK_RECORDING_START"));
  connect( myStartAction, SIGNAL( triggered ( bool ) ), this, SLOT( onStartRecording() ) );
  mgr->registerAction( myStartAction, StartRecordingId );

  // Play recording
  myPlayAction = new QtxAction(tr("MNU_SVTK_RECORDING_PLAY"), 
			       theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_RECORDING_PLAY" ) ),
			       tr( "MNU_SVTK_RECORDING_PLAY" ), 0, this);
  myPlayAction->setStatusTip(tr("DSC_SVTK_RECORDING_PLAY"));
  myPlayAction->setEnabled( false );
  connect( myPlayAction, SIGNAL( triggered ( bool ) ), this, SLOT( onPlayRecording() ) );
  mgr->registerAction( myPlayAction, PlayRecordingId );

  // Pause recording
  myPauseAction = new QtxAction(tr("MNU_SVTK_RECORDING_PAUSE"), 
				theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_RECORDING_PAUSE" ) ),
				tr( "MNU_SVTK_RECORDING_PAUSE" ), 0, this);
  myPauseAction->setStatusTip(tr("DSC_SVTK_RECORDING_PAUSE"));
  myPauseAction->setEnabled( false );
  connect( myPauseAction, SIGNAL( triggered ( bool ) ), this, SLOT( onPauseRecording() ) );
  mgr->registerAction( myPauseAction, PauseRecordingId );

  // Stop recording
  myStopAction = new QtxAction(tr("MNU_SVTK_RECORDING_STOP"), 
			       theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_RECORDING_STOP" ) ),
			       tr( "MNU_SVTK_RECORDING_STOP" ), 0, this);
  myStopAction->setStatusTip(tr("DSC_SVTK_RECORDING_STOP"));
  myStopAction->setEnabled( false );
  connect( myStopAction, SIGNAL( triggered ( bool ) ), this, SLOT( onStopRecording() ) );
  mgr->registerAction( myStopAction, StopRecordingId );
}

/*!
  Creates toolbar of svtk main window
*/
void SVTK_ViewWindow::createToolBar()
{
  QtxActionToolMgr* mgr = toolMgr();
  
  mgr->append( DumpId, myToolBar );
  mgr->append( Mode2DId, myToolBar );
  mgr->append( SwitchInteractionStyleId, myToolBar );
  mgr->append( ViewTrihedronId, myToolBar );

  QtxMultiAction* aScaleAction = new QtxMultiAction( this );
  aScaleAction->insertAction( getAction( FitAllId ) );
  aScaleAction->insertAction( getAction( FitRectId ) );
  aScaleAction->insertAction( getAction( ZoomId ) );
  mgr->append( aScaleAction, myToolBar );

  QtxMultiAction* aPanningAction = new QtxMultiAction( this );
  aPanningAction->insertAction( getAction( PanId ) );
  aPanningAction->insertAction( getAction( GlobalPanId ) );
  mgr->append( aPanningAction, myToolBar );

  mgr->append( ChangeRotationPointId, myToolBar );

  mgr->append( RotationId, myToolBar );

  myViewsAction = new QtxMultiAction( this );
  myViewsAction->insertAction( getAction( FrontId ) );
  myViewsAction->insertAction( getAction( BackId ) );
  myViewsAction->insertAction( getAction( TopId ) );
  myViewsAction->insertAction( getAction( BottomId ) );
  myViewsAction->insertAction( getAction( LeftId ) );
  myViewsAction->insertAction( getAction( RightId ) );
  mgr->append( myViewsAction, myToolBar );

  mgr->append( ResetId, myToolBar );

  mgr->append( UpdateRate, myToolBar );
  mgr->append( NonIsometric, myToolBar );
  mgr->append( GraduatedAxes, myToolBar );

  mgr->append( ViewParametersId, myToolBar );
  mgr->append( SurfacesSettingsId, myToolBar );
  mgr->append( ProjectionModeId, myToolBar );

  mgr->append( StartRecordingId, myRecordingToolBar );
  mgr->append( PlayRecordingId, myRecordingToolBar );
  mgr->append( PauseRecordingId, myRecordingToolBar );
  mgr->append( StopRecordingId, myRecordingToolBar );

  // the following actions are enabled on demand
  getAction( Mode2DId )->setVisible( false );
  getAction( SurfacesSettingsId )->setVisible( false );
}

void SVTK_ViewWindow::onUpdateRate(bool theIsActivate)
{
  if(theIsActivate){
    myUpdateRateDlg->Update();
    myUpdateRateDlg->show();
  }else
    myUpdateRateDlg->hide();
}

void SVTK_ViewWindow::onNonIsometric(bool theIsActivate)
{
  if(theIsActivate){
    myNonIsometricDlg->Update();
    myNonIsometricDlg->show();
  }else
    myNonIsometricDlg->hide();
}

void SVTK_ViewWindow::onGraduatedAxes(bool theIsActivate)
{
  if(theIsActivate){
    myCubeAxesDlg->Update();
    myCubeAxesDlg->show();
  }else
    myCubeAxesDlg->hide();
}

/*!
  Starts rotation transformation
*/
void SVTK_ViewWindow::activateRotation()
{
  myEventDispatcher->InvokeEvent(SVTK::StartRotate,0);
}


/*!
  Starts panning transformation
*/
void SVTK_ViewWindow::activatePanning()
{
  myEventDispatcher->InvokeEvent(SVTK::StartPan,0);
}

/*!
  Starts zoom transformation
*/
void SVTK_ViewWindow::activateZoom()
{
  myEventDispatcher->InvokeEvent(SVTK::StartZoom,0);
}

/*!
  Starts window fit transformation
*/
void SVTK_ViewWindow::activateWindowFit()
{
  myEventDispatcher->InvokeEvent(SVTK::StartFitArea,0);
}

/*!
  Starts global panning transformation
*/
void SVTK_ViewWindow::activateGlobalPanning()
{
  myEventDispatcher->InvokeEvent(SVTK::StartGlobalPan,0);
}

void SVTK_ViewWindow::onStartRecording()
{
  myRecorder->CheckExistAVIMaker();
  if (myRecorder->ErrorStatus()) {
    SUIT_MessageBox::warning(this, tr("ERROR"), tr("MSG_NO_AVI_MAKER") );
  }
  else {
    SVTK_RecorderDlg* aRecorderDlg = new SVTK_RecorderDlg( this, myRecorder );

    if( !aRecorderDlg->exec() )
      return;

    myStartAction->setEnabled( false );
    myPlayAction->setEnabled( false );
    myPauseAction->setEnabled( true );
    myStopAction->setEnabled( true );

    // to prevent resizing the window while recording
    myPreRecordingMinSize = minimumSize();
    myPreRecordingMaxSize = maximumSize();
    setFixedSize( size() );

    myRecorder->Record();
  }
}

void SVTK_ViewWindow::onPlayRecording()
{
  myStartAction->setEnabled( false );
  myPlayAction->setEnabled( false );
  myPauseAction->setEnabled( true );
  myStopAction->setEnabled( true );

  myRecorder->Pause();
}

void SVTK_ViewWindow::onPauseRecording()
{
  myStartAction->setEnabled( false );
  myPlayAction->setEnabled( true );
  myPauseAction->setEnabled( false );
  myStopAction->setEnabled( true );

  myRecorder->Pause();
}

void SVTK_ViewWindow::onStopRecording()
{
  myStartAction->setEnabled( true );
  myPlayAction->setEnabled( false );
  myPauseAction->setEnabled( false );
  myStopAction->setEnabled( false );

  myRecorder->Stop();

  setMinimumSize( myPreRecordingMinSize );
  setMaximumSize( myPreRecordingMaxSize );
}

/*!
  To invoke a VTK event on SVTK_RenderWindowInteractor instance
*/
void SVTK_ViewWindow::InvokeEvent(unsigned long theEvent, void* theCallData)
{
  GetInteractor()->InvokeEvent(theEvent,theCallData);
}

/*!
  Modify view parameters
*/
void SVTK_ViewWindow::onViewParameters(bool theIsActivate)
{
  if(theIsActivate){
    myViewParameterDlg->addObserver();
    myViewParameterDlg->show();
  }else
    myViewParameterDlg->hide();
}

/*!
  Custom show event handler
*/
void SVTK_ViewWindow::showEvent( QShowEvent * theEvent ) 
{
  emit Show( theEvent );
}

/*!
  Custom hide event handler
*/
void SVTK_ViewWindow::hideEvent( QHideEvent * theEvent ) 
{
  emit Hide( theEvent );
}

/*!
  Show/hide the specified action
*/
void SVTK_ViewWindow::SetActionVisible( const int theActionId,
                                        const bool theIsVisible )
{
  if( QtxAction* anAction = getAction( theActionId ) )
    anAction->setVisible( theIsVisible );
}

/*!
  Set the normal axis for the Mode2D
*/
void SVTK_ViewWindow::SetMode2DNormalAxis( const int theAxis )
{
  myMode2DNormalAxis = theAxis;
}

/*!
  Change the surfaces settings
*/
void SVTK_ViewWindow::onSurfacesSettings()
{
  vtkRenderer* aRenderer = getRenderer();
  if( !aRenderer )
    return;

  QList< Plot3d_Actor* > aSurfaces;
  QStringList aNameList;
  vtkActor* anActor = 0;

  VTK::ActorCollectionCopy aCopy( aRenderer->GetActors() );
  vtkActorCollection* aCollection = aCopy.GetActors();
  aCollection->InitTraversal();
  while( anActor = aCollection->GetNextActor() )
  {
    if( Plot3d_Actor* aSurface = dynamic_cast<Plot3d_Actor*>( anActor ) )
    {
      aSurfaces << aSurface;
      if( vtkScalarBarActor* aScalarBar = aSurface->GetScalarBarActor().GetPointer() )
        aNameList << aScalarBar->GetTitle();
    }
  }

  QVector< QString > aTexts = aNameList.toVector();

  Plot3d_SetupSurfacesDlg aDlg;
  aDlg.SetParameters( aTexts );

  if ( aDlg.exec() != QDialog::Accepted ) 
    return;

  // Note: Indexes retrieved from dialog do not correspond to the real indexes of 
  // plot 3d surfaces. They correspond to the user actions. For example, if user removes 
  // first surface in dialog's table two times. Then contents of list of indexes is 
  // equal (1, 1) although first and surfaces curves must be removed.
  const QList< int >& toRemove = aDlg.GetRemovedIndexes();
  QList< int >::const_iterator aRemIter;
  for ( aRemIter = toRemove.begin(); aRemIter != toRemove.end(); ++aRemIter )
  {
    int anIndex = *aRemIter;
    if ( anIndex >= 0 && anIndex < (int)aSurfaces.count() )  
    {
      Plot3d_Actor* aSurface = aSurfaces[ anIndex ];
      aSurfaces.removeAt( anIndex );
      aSurface->RemoveFromRender( aRenderer );
      aSurface->Delete();
    }
  }

  QMap< int, Plot3d_Actor* > anIndexToSurface;
  QList< Plot3d_Actor* >::iterator aSurfIter;
  int i;
  for ( i = 0, aSurfIter = aSurfaces.begin(); aSurfIter != aSurfaces.end(); ++aSurfIter, ++i )
    anIndexToSurface[ i ] = *aSurfIter;

  aDlg.GetParameters( aTexts );

  int n;
  for ( i = 0, n = aTexts.size(); i < n; i++ )
  {
    Plot3d_Actor* aSurface = anIndexToSurface[ i ];
    QString aText = aTexts[ i ];
    if( vtkScalarBarActor* aScalarBar = aSurface->GetScalarBarActor().GetPointer() )
      aScalarBar->SetTitle( aText.toLatin1().constData() );
  }

  vtkFloatingPointType aGlobalBounds[6] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MIN,
                                            VTK_DOUBLE_MAX, VTK_DOUBLE_MIN,
                                            VTK_DOUBLE_MAX, VTK_DOUBLE_MIN };

  for ( i = 0, aSurfIter = aSurfaces.begin(); aSurfIter != aSurfaces.end(); ++aSurfIter, ++i )
  {
    if( Plot3d_Actor* aSurface = *aSurfIter )
    {
      vtkFloatingPointType aBounds[6];
      aSurface->GetBounds( aBounds );
      aGlobalBounds[0] = qMin( aGlobalBounds[0], aBounds[0] );
      aGlobalBounds[1] = qMax( aGlobalBounds[1], aBounds[1] );
      aGlobalBounds[2] = qMin( aGlobalBounds[2], aBounds[2] );
      aGlobalBounds[3] = qMax( aGlobalBounds[3], aBounds[3] );
      aGlobalBounds[4] = qMin( aGlobalBounds[4], aBounds[4] );
      aGlobalBounds[5] = qMax( aGlobalBounds[5], aBounds[5] );
    }
  }

  double aDX = aGlobalBounds[1] - aGlobalBounds[0];
  double aDY = aGlobalBounds[3] - aGlobalBounds[2];
  double aDZ = aGlobalBounds[5] - aGlobalBounds[4];

  double aScale[3];
  GetScale( aScale ); // take into account the current scale
  aDX = fabs( aScale[0] ) > DBL_EPSILON ? aDX / aScale[0] : aDX;
  aDY = fabs( aScale[1] ) > DBL_EPSILON ? aDY / aScale[1] : aDY;
  aDZ = fabs( aScale[2] ) > DBL_EPSILON ? aDZ / aScale[2] : aDZ;

  aScale[0] = fabs( aDX ) > DBL_EPSILON ? 1.0 / aDX : 1.0;
  aScale[1] = fabs( aDY ) > DBL_EPSILON ? 1.0 / aDY : 1.0;
  aScale[2] = fabs( aDZ ) > DBL_EPSILON ? 1.0 / aDZ : 1.0;
  SetScale( aScale );
  onFitAll();
}

/*!
  Fit 2D surfaces to the specified data range
*/
void SVTK_ViewWindow::onFitData()
{
  // to do
}
