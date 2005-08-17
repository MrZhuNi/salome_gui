#include "SVTK_RenderWindow.h"
#include "SVTK_Trihedron.h"
#include "SVTK_CubeAxesActor2D.h"
#include "SALOME_Actor.h"

#include "VTKViewer_Transform.h"
#include "VTKViewer_Utilities.h"

#include "QtxAction.h"

#include "SUIT_Session.h"
#include "SUIT_ToolButton.h"
#include "SUIT_MessageBox.h"

#include "SUIT_Tools.h"
#include "SUIT_ResourceMgr.h"

#include <stdlib.h>
#include <math.h>

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkTextProperty.h>
#include <vtkCamera.h>
#ifndef WNT
#include <vtkXOpenGLRenderWindow.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <qgl.h>
#endif

#if QT_VERSION > 300
#include <qcursor.h>
#endif

//==========================================================
SVTK_RenderWindow
::SVTK_RenderWindow(QWidget* parent, const char* name) :
  QMainWindow(parent, name, 
	  Qt::WStyle_NoBorder | Qt::WDestructiveClose | 
	  Qt::WResizeNoErase | Qt::WRepaintNoErase),
  myTrihedronSize( 100 )
{
  myRenderWindow = vtkRenderWindow::New();
#ifndef WNT
  myRenderWindow->SetDisplayId((void*)x11Display());
#endif
  myRenderWindow->SetWindowId((void*)winId());
  myRenderWindow->DoubleBufferOn();
  //myRenderWindow->DebugOn();
  setMouseTracking(true);

  myRenderer  = vtkRenderer::New();
  myTransform = VTKViewer_Transform::New();
  myTrihedron = SVTK_Trihedron::New();
  myCubeAxes  = SVTK_CubeAxesActor2D::New();

  myTrihedron->AddToRender( myRenderer );
  myRenderer->AddProp(myCubeAxes);

  myRenderWindow->AddRenderer( myRenderer );
  myRenderer->GetActiveCamera()->ParallelProjectionOn();
  myRenderer->LightFollowCameraOn();
  myRenderer->TwoSidedLightingOn();

  // Set BackgroundColor
  QString BgrColorRed   = "0";//SUIT_CONFIG->getSetting("VTKViewer:BackgroundColorRed");
  QString BgrColorGreen = "0";//SUIT_CONFIG->getSetting("VTKViewer:BackgroundColorGreen");
  QString BgrColorBlue  = "0";//SUIT_CONFIG->getSetting("VTKViewer:BackgroundColorBlue");

  if( !BgrColorRed.isEmpty() && !BgrColorGreen.isEmpty() && !BgrColorBlue.isEmpty() ) 
    myRenderer->SetBackground( BgrColorRed.toInt()/255., BgrColorGreen.toInt()/255., BgrColorBlue.toInt()/255. );
  else
    myRenderer->SetBackground( 0, 0, 0 );

  vtkTextProperty* tprop = vtkTextProperty::New();
  tprop->SetColor(1, 1, 1);
  tprop->ShadowOn();
  
  float bnd[6];
  bnd[0] = bnd[2] = bnd[4] = 0;
  bnd[1] = bnd[3] = bnd[5] = myTrihedron->GetSize();
  myCubeAxes->SetLabelFormat("%6.4g");
  myCubeAxes->SetBounds(bnd);
  myCubeAxes->SetCamera(myRenderer->GetActiveCamera());
  myCubeAxes->SetFlyModeToOuterEdges(); // ENK remarks: it must bee
  myCubeAxes->SetFontFactor(0.8);
  myCubeAxes->SetAxisTitleTextProperty(tprop);
  myCubeAxes->SetAxisLabelTextProperty(tprop);
  myCubeAxes->SetCornerOffset(0);
  myCubeAxes->SetScaling(0);
  myCubeAxes->SetNumberOfLabels(5);
  myCubeAxes->VisibilityOff();
  myCubeAxes->SetTransform(myTransform);
  tprop->Delete();

  myToolBar = new QToolBar(this);
  myToolBar->setCloseMode(QDockWindow::Undocked);
  myToolBar->setLabel(tr("LBL_TOOLBAR_LABEL"));

  createActions();
  createToolBar();
}

//==========================================================
SVTK_RenderWindow
::~SVTK_RenderWindow() 
{
  myRenderWindow->Delete();

  myTransform->Delete();

  //m_RW->Delete() ;
  myRenderer->RemoveAllProps();
  //m_Renderer->Delete();
  myTrihedron->Delete();
  myCubeAxes->Delete();
}

//==========================================================
void
SVTK_RenderWindow
::createActions()
{
  if (!myActionsMap.isEmpty()) return;
  
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  
  QtxAction* aAction;

  // Dump view
  aAction = new QtxAction(tr("MNU_DUMP_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_DUMP" ) ),
                           tr( "MNU_DUMP_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_DUMP_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onDumpView()));
  myActionsMap[ DumpId ] = aAction;

  // FitAll
  aAction = new QtxAction(tr("MNU_FITALL"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FITALL" ) ),
                           tr( "MNU_FITALL" ), 0, this);
  aAction->setStatusTip(tr("DSC_FITALL"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onFitAll()));
  myActionsMap[ FitAllId ] = aAction;

  // FitRect
  aAction = new QtxAction(tr("MNU_FITRECT"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FITAREA" ) ),
                           tr( "MNU_FITRECT" ), 0, this);
  aAction->setStatusTip(tr("DSC_FITRECT"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateWindowFit()));
  myActionsMap[ FitRectId ] = aAction;

  // Zoom
  aAction = new QtxAction(tr("MNU_ZOOM_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_ZOOM" ) ),
                           tr( "MNU_ZOOM_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_ZOOM_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateZoom()));
  myActionsMap[ ZoomId ] = aAction;

  // Panning
  aAction = new QtxAction(tr("MNU_PAN_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_PAN" ) ),
                           tr( "MNU_PAN_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_PAN_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activatePanning()));
  myActionsMap[ PanId ] = aAction;

  // Global Panning
  aAction = new QtxAction(tr("MNU_GLOBALPAN_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_GLOBALPAN" ) ),
                           tr( "MNU_GLOBALPAN_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_GLOBALPAN_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateGlobalPanning()));
  myActionsMap[ GlobalPanId ] = aAction;

  // Rotation
  aAction = new QtxAction(tr("MNU_ROTATE_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_ROTATE" ) ),
                           tr( "MNU_ROTATE_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_ROTATE_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateRotation()));
  myActionsMap[ RotationId ] = aAction;

  // Projections
  aAction = new QtxAction(tr("MNU_FRONT_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_FRONT" ) ),
                           tr( "MNU_FRONT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_FRONT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onFrontView()));
  myActionsMap[ FrontId ] = aAction;

  aAction = new QtxAction(tr("MNU_BACK_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_BACK" ) ),
                           tr( "MNU_BACK_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_BACK_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onBackView()));
  myActionsMap[ BackId ] = aAction;

  aAction = new QtxAction(tr("MNU_TOP_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_TOP" ) ),
                           tr( "MNU_TOP_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_TOP_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onTopView()));
  myActionsMap[ TopId ] = aAction;

  aAction = new QtxAction(tr("MNU_BOTTOM_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_BOTTOM" ) ),
                           tr( "MNU_BOTTOM_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_BOTTOM_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onBottomView()));
  myActionsMap[ BottomId ] = aAction;

  aAction = new QtxAction(tr("MNU_LEFT_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_LEFT" ) ),
                           tr( "MNU_LEFT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_LEFT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onLeftView()));
  myActionsMap[ LeftId ] = aAction;

  aAction = new QtxAction(tr("MNU_RIGHT_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_RIGHT" ) ),
                           tr( "MNU_RIGHT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_RIGHT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onRightView()));
  myActionsMap[ RightId ] = aAction;

  // Reset
  aAction = new QtxAction(tr("MNU_RESET_VIEW"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_RESET" ) ),
                           tr( "MNU_RESET_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_RESET_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onResetView()));
  myActionsMap[ ResetId ] = aAction;

  // onViewTrihedron: Shows - Hides Trihedron
  aAction = new QtxAction(tr("MNU_SHOW_TRIHEDRON"), aResMgr->loadPixmap( "VTKViewer", tr( "ICON_VTKVIEWER_VIEW_TRIHEDRON" ) ),
                           tr( "MNU_SHOW_TRIHEDRON" ), 0, this);
  aAction->setStatusTip(tr("DSC_SHOW_TRIHEDRON"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onViewTrihedron()));
  myActionsMap[ ViewTrihedronId ] = aAction;
}

//==========================================================
void
SVTK_RenderWindow
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
SVTK_RenderWindow
::Repaint( bool theUpdateTrihedron )
{
  if( theUpdateTrihedron ) 
    onAdjustTrihedron();

  update();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::setBackgroundColor( const QColor& color )
{
  if( myRenderer )
    myRenderer->SetBackground( color.red()/255., color.green()/255., color.blue()/255. );
}

//----------------------------------------------------------------------------
QColor
SVTK_RenderWindow
::backgroundColor() const
{
  float backint[3];
  if( myRenderer )
  {
    myRenderer->GetBackground( backint );
    return QColor(int(backint[0]*255), int(backint[1]*255), int(backint[2]*255));
  }
  return QColor();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::GetScale( double theScale[3] ) 
{
  myTransform->GetMatrixScale( theScale );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::SetScale( double theScale[3] ) 
{
  myTransform->SetMatrixScale( theScale[0], theScale[1], theScale[2] );
  //myRWInteractor->Render();
  Repaint();
}

//----------------------------------------------------------------------------
bool
SVTK_RenderWindow
::isTrihedronDisplayed()
{
  return myTrihedron->GetVisibility() == VTKViewer_Trihedron::eOn;
}

//----------------------------------------------------------------------------
bool
SVTK_RenderWindow
::isCubeAxesDisplayed()
{
  return myCubeAxes->GetVisibility() == 1;
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activateZoom()
{
  //myInteractorStyle->startZoom();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activatePanning()
{
  //myInteractorStyle->startPan();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activateRotation()
{
  //myInteractorStyle->startRotate();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activateGlobalPanning()
{
  //myInteractorStyle->startGlobalPan();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activateWindowFit()
{
  //myInteractorStyle->startFitArea();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onFrontView()
{
  vtkCamera* camera = myRenderer->GetActiveCamera();
  camera->SetPosition(1,0,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onBackView()
{
  vtkCamera* camera = myRenderer->GetActiveCamera();
  camera->SetPosition(-1,0,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onTopView()
{
  vtkCamera* camera = myRenderer->GetActiveCamera();
  camera->SetPosition(0,0,1);
  camera->SetViewUp(0,1,0);
  camera->SetFocalPoint(0,0,0);
  onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onBottomView()
{
  vtkCamera* camera = myRenderer->GetActiveCamera();
  camera->SetPosition(0,0,-1);
  camera->SetViewUp(0,1,0);
  camera->SetFocalPoint(0,0,0);
  onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onLeftView()
{
  vtkCamera* camera = myRenderer->GetActiveCamera(); 
  camera->SetPosition(0,-1,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onRightView()
{
  vtkCamera* camera = myRenderer->GetActiveCamera();
  camera->SetPosition(0,1,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onFitAll();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onResetView()
{
  int aTrihedronIsVisible = isTrihedronDisplayed();
  int aCubeAxesIsVisible  = isCubeAxesDisplayed();

  myTrihedron->SetVisibility( VTKViewer_Trihedron::eOnlyLineOn );
  myCubeAxes->SetVisibility(0);

  ::ResetCamera(myRenderer,true);  
  vtkCamera* aCamera = myRenderer->GetActiveCamera();
  aCamera->SetPosition(1,-1,1);
  aCamera->SetViewUp(0,0,1);
  ::ResetCamera(myRenderer,true);  

  if (aTrihedronIsVisible) myTrihedron->VisibilityOn();
  else myTrihedron->VisibilityOff();

  if (aCubeAxesIsVisible) myCubeAxes->VisibilityOn();
  else myCubeAxes->VisibilityOff();

  static float aCoeff = 3.0;
  aCamera->SetParallelScale(aCoeff*aCamera->GetParallelScale());
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onFitAll()
{
  int aTrihedronWasVisible = false;
  int aCubeAxesWasVisible = false;
  if (myTrihedron) {
    aTrihedronWasVisible = isTrihedronDisplayed();
    if (aTrihedronWasVisible)
      myTrihedron->VisibilityOff();
  }

  if (myCubeAxes) {
    aCubeAxesWasVisible = isCubeAxesDisplayed();
    if (aCubeAxesWasVisible)
      myCubeAxes->VisibilityOff();
  }

  if (myTrihedron->GetVisibleActorCount(myRenderer)) {
    myTrihedron->VisibilityOff();
    myCubeAxes->VisibilityOff();
    ::ResetCamera(myRenderer);
  } else {
    myTrihedron->SetVisibility(VTKViewer_Trihedron::eOnlyLineOn);
    myCubeAxes->SetVisibility(2);
    ::ResetCamera(myRenderer,true);
  }

  if (aTrihedronWasVisible)
    myTrihedron->VisibilityOn();
  else
    myTrihedron->VisibilityOff();

  if (aCubeAxesWasVisible)
    myCubeAxes->VisibilityOn();
  else
    myCubeAxes->VisibilityOff();

  ::ResetCameraClippingRange(myRenderer);

  Repaint();
}

//----------------------------------------------------------------------------
void 
SVTK_RenderWindow
::onViewTrihedron()
{
  if(!myTrihedron) 
    return;

  if(isTrihedronDisplayed())
    myTrihedron->VisibilityOff();
  else
    myTrihedron->VisibilityOn();

  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onViewCubeAxes()
{
  if(!myCubeAxes)
    return;

  if(isCubeAxesDisplayed())
    myCubeAxes->VisibilityOff();
  else
    myCubeAxes->VisibilityOn();

  Repaint();
}
//----------------------------------------------------------------------------
int SVTK_RenderWindow::GetTrihedronSize() const
{
  return myTrihedronSize;
}

//----------------------------------------------------------------------------
void SVTK_RenderWindow::SetTrihedronSize( const int sz )
{
  if ( myTrihedronSize == sz )
    return;

  myTrihedronSize = sz;
  AdjustTrihedrons( true );
}

/*! If parameter theIsForcedUpdate is true, recalculate parameters for
 *  trihedron and cube axes, even if trihedron and cube axes is invisible.
 */
void
SVTK_RenderWindow
::AdjustTrihedrons(const bool theIsForcedUpdate)
{
  if ((!isCubeAxesDisplayed() && !isTrihedronDisplayed()) && !theIsForcedUpdate)
    return;

  float bnd[ 6 ];
  float newbnd[6];
  newbnd[ 0 ] = newbnd[ 2 ] = newbnd[ 4 ] = VTK_LARGE_FLOAT;
  newbnd[ 1 ] = newbnd[ 3 ] = newbnd[ 5 ] = -VTK_LARGE_FLOAT;

  myCubeAxes->GetBounds(bnd);

  int aVisibleNum = myTrihedron->GetVisibleActorCount( myRenderer );
  //if (aVisibleNum || theIsForcedUpdate) {
  if (aVisibleNum) {
    // if the new trihedron size have sufficient difference, then apply the value
    double aNewSize = 100, anOldSize=myTrihedron->GetSize();
    bool aTDisplayed = isTrihedronDisplayed();
    bool aCDisplayed = isCubeAxesDisplayed();
    if(aTDisplayed) myTrihedron->VisibilityOff();
    if(aCDisplayed) myCubeAxes->VisibilityOff();

    SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
    static float aSizeInPercents = aResMgr->doubleValue("VTKViewer","trihedron_size", 105);

    //bool isComputeTrihedronSize =
      ::ComputeTrihedronSize(myRenderer, aNewSize, anOldSize, aSizeInPercents);

    myTrihedron->SetSize( aNewSize );

    // iterate through displayed objects and set size if necessary
    vtkActorCollection* anActors = getRenderer()->GetActors();
    anActors->InitTraversal();
    while (vtkActor* anActor = anActors->GetNextActor())
    {
      if (SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast( anActor ))
      {
	if (aSActor->IsResizable())
	  aSActor->SetSize( 0.5 * aNewSize );
        if (aSActor->GetVisibility() && !aSActor->IsInfinitive()) {
	  float *abounds = aSActor->GetBounds();
          if (abounds[0] > -VTK_LARGE_FLOAT && abounds[1] < VTK_LARGE_FLOAT &&
              abounds[2] > -VTK_LARGE_FLOAT && abounds[3] < VTK_LARGE_FLOAT &&
              abounds[4] > -VTK_LARGE_FLOAT && abounds[5] < VTK_LARGE_FLOAT)
	    for (int i = 0; i < 5; i = i + 2) {
	      if (abounds[i] < newbnd[i]) newbnd[i] = abounds[i];
	      if (abounds[i+1] > newbnd[i+1]) newbnd[i+1] = abounds[i+1];
	    }
        }
      }
    }
    if (aTDisplayed) myTrihedron->VisibilityOn();
    if (aCDisplayed) myCubeAxes->VisibilityOn();
    
  } else {
     double aSize = myTrihedron->GetSize();
     newbnd[0] = newbnd[2] = newbnd[4] = 0;
     newbnd[1] = newbnd[3] = newbnd[5] = aSize;
  }
  
  if (newbnd[0] < VTK_LARGE_FLOAT && newbnd[2] < VTK_LARGE_FLOAT && newbnd[4] < VTK_LARGE_FLOAT &&
      newbnd[1] >-VTK_LARGE_FLOAT && newbnd[3] >-VTK_LARGE_FLOAT && newbnd[5] >-VTK_LARGE_FLOAT) {
    for(int i=0;i<6;i++) bnd[i] = newbnd[i];
    myCubeAxes->SetBounds(bnd);
  }
  
  myCubeAxes->SetBounds(bnd);

  ::ResetCameraClippingRange(myRenderer);
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onAdjustTrihedron()
{   
  AdjustTrihedrons( false );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onAdjustCubeAxes()
{   
  AdjustTrihedrons(false);
}

//----------------------------------------------------------------------------
QImage
SVTK_RenderWindow
::dumpView()
{
  QPixmap px = QPixmap::grabWindow( winId() );
  return px.convertToImage();
}
