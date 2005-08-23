#include "SVTK_RenderWindow.h"
#include "SVTK_Trihedron.h"
#include "SVTK_CubeAxesActor2D.h"
#include "SALOME_Actor.h"

#include "VTKViewer_Transform.h"
#include "VTKViewer_Utilities.h"

#include "SVTK_Renderer.h"

#include "QtxAction.h"

#include "SUIT_Session.h"
#include "SUIT_ToolButton.h"
#include "SUIT_MessageBox.h"

#include "SUIT_Tools.h"
#include "SUIT_ResourceMgr.h"

#include <vtkRenderWindow.h>

#if QT_VERSION > 300
#include <qcursor.h>
#endif

//----------------------------------------------------------------------------
SVTK_RenderWindow
::SVTK_RenderWindow(QWidget* parent, const char* name) :
  QMainWindow(parent, name, 
	      Qt::WStyle_NoBorder | 
	      Qt::WDestructiveClose | 
	      Qt::WResizeNoErase | 
	      Qt::WRepaintNoErase),
    myRenderWindow(vtkRenderWindow::New()),
    myRenderer(SVTK_Renderer::New())
{
  myRenderWindow->Delete();
  myRenderer->Delete();

#ifndef WNT
  myRenderWindow->SetDisplayId((void*)x11Display());
#endif
  myRenderWindow->SetWindowId((void*)winId());
  myRenderWindow->DoubleBufferOn();
  setMouseTracking(true);

  myRenderWindow->AddRenderer(getRenderer());

  myToolBar = new QToolBar(this);
  myToolBar->setCloseMode(QDockWindow::Undocked);
  myToolBar->setLabel(tr("LBL_TOOLBAR_LABEL"));

  createActions();
  createToolBar();
}

SVTK_RenderWindow
::~SVTK_RenderWindow() 
{}


//----------------------------------------------------------------------------
QToolBar* 
SVTK_RenderWindow
::getToolBar()
{
  return myToolBar;
}

vtkRenderer* 
SVTK_RenderWindow
::getRenderer() 
{ 
  return myRenderer.GetPointer();
}

vtkRenderWindow* 
SVTK_RenderWindow
::getRenderWindow()
{ 
  return myRenderWindow.GetPointer(); 
}

VTKViewer_Trihedron*  
SVTK_RenderWindow
::GetTrihedron() 
{ 
  return myRenderer->GetTrihedron(); 
}

SVTK_CubeAxesActor2D* 
SVTK_RenderWindow
::GetCubeAxes() 
{ 
  return myRenderer->GetCubeAxes(); 
}

//----------------------------------------------------------------------------
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
  if(theUpdateTrihedron) 
    myRenderer->onAdjustTrihedron();

  update();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::setBackgroundColor(const QColor& theColor)
{
  myRenderer->SetBackground(theColor.red()/255.0, 
			    theColor.green()/255.0,
			    theColor.blue()/255.0);
}

//----------------------------------------------------------------------------
QColor
SVTK_RenderWindow
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
SVTK_RenderWindow
::GetScale( double theScale[3] ) 
{
  myRenderer->GetScale( theScale );
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::SetScale( double theScale[3] ) 
{
  myRenderer->SetScale( theScale );
  Repaint();
}

//----------------------------------------------------------------------------
bool
SVTK_RenderWindow
::isTrihedronDisplayed()
{
  return myRenderer->isTrihedronDisplayed();
}

//----------------------------------------------------------------------------
bool
SVTK_RenderWindow
::isCubeAxesDisplayed()
{
  return myRenderer->isCubeAxesDisplayed();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activateZoom()
{}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activatePanning()
{}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activateRotation()
{}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activateGlobalPanning()
{}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::activateWindowFit()
{}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onFrontView()
{
  myRenderer->onFrontView();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onBackView()
{
  myRenderer->onBackView();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onTopView()
{
  myRenderer->onTopView();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onBottomView()
{
  myRenderer->onBottomView();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onLeftView()
{
  myRenderer->onLeftView();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onRightView()
{
  myRenderer->onRightView();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onResetView()
{
  myRenderer->onResetView();
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onFitAll()
{
  myRenderer->onFitAll();
  Repaint();
}

//----------------------------------------------------------------------------
void 
SVTK_RenderWindow
::onViewTrihedron()
{
  myRenderer->onViewTrihedron();
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onViewCubeAxes()
{
  myRenderer->onViewCubeAxes();
  Repaint();
}

//----------------------------------------------------------------------------
int SVTK_RenderWindow::GetTrihedronSize() const
{
  return myRenderer->GetTrihedronSize();
}

//----------------------------------------------------------------------------
void SVTK_RenderWindow::SetTrihedronSize( const int theSize )
{
  myRenderer->SetTrihedronSize(theSize);
  Repaint();
}

/*! If parameter theIsForcedUpdate is true, recalculate parameters for
 *  trihedron and cube axes, even if trihedron and cube axes is invisible.
 */
void
SVTK_RenderWindow
::AdjustTrihedrons(const bool theIsForced)
{
  myRenderer->AdjustTrihedrons(theIsForced);
  Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onAdjustTrihedron()
{   
  myRenderer->onAdjustTrihedron();
}

//----------------------------------------------------------------------------
void
SVTK_RenderWindow
::onAdjustCubeAxes()
{   
  myRenderer->onAdjustCubeAxes();
}

//----------------------------------------------------------------------------
QImage
SVTK_RenderWindow
::dumpView()
{
  QPixmap px = QPixmap::grabWindow( winId() );
  return px.convertToImage();
}
