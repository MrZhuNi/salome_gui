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
// See http://www.salome-platform.org/
//
#include "SALOME_Actor.h"

#include <qapplication.h>
#include <qimage.h>

#include <vtkTextProperty.h>
#include <vtkActorCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkAxisActor2D.h>

#include "QtxAction.h"

#include "SUIT_Session.h"
#include "SUIT_ToolButton.h"
#include "SUIT_MessageBox.h"
#include "SUIT_Accel.h"

#include "SUIT_Tools.h"
#include "SUIT_ResourceMgr.h"
#include "SUIT_Accel.h"

#include "VTKViewer_Utilities.h"

#include "SVTK_View.h"
#include "SVTK_MainWindow.h"
#include "SVTK_Selector.h"

#include "SVTK_Event.h"
#include "SVTK_Renderer.h"
#include "SVTK_ViewWindow.h"
#include "SVTK_ViewModelBase.h"
#include "SVTK_InteractorStyle.h"
#include "SVTK_RenderWindowInteractor.h"
#include "SVTK_GenericRenderWindowInteractor.h"
#include "SVTK_CubeAxesActor2D.h"

#include "SALOME_ListIteratorOfListIO.hxx"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"

//----------------------------------------------------------------------------
SVTK_ViewWindow
::SVTK_ViewWindow(SUIT_Desktop* theDesktop):
  SUIT_ViewWindow(theDesktop),
  myMainWindow(NULL),
  myView(NULL)
{}

void
SVTK_ViewWindow
::Initialize(SVTK_ViewModelBase* theModel)
{
  if(SUIT_ResourceMgr* aResourceMgr = SUIT_Session::session()->resourceMgr()){
    myMainWindow = new SVTK_MainWindow(this,"SVTK_MainWindow",aResourceMgr,this);

    SVTK_RenderWindowInteractor* anIteractor = 
      new SVTK_RenderWindowInteractor(myMainWindow,"SVTK_RenderWindowInteractor");

    SVTK_Selector* aSelector = SVTK_Selector::New();

    SVTK_GenericRenderWindowInteractor* aDevice = 
      SVTK_GenericRenderWindowInteractor::New();
    aDevice->SetRenderWidget(anIteractor);
    aDevice->SetSelector(aSelector);

    SVTK_Renderer* aRenderer = SVTK_Renderer::New();
    aRenderer->Initialize(aDevice,aSelector);

    anIteractor->Initialize(aDevice,aRenderer,aSelector);

    aDevice->Delete();
    aRenderer->Delete();
    aSelector->Delete();

    myMainWindow->Initialize(anIteractor);

    SVTK_InteractorStyle* aStyle = SVTK_InteractorStyle::New();
    anIteractor->PushInteractorStyle(aStyle);
    aStyle->Delete();

    setCentralWidget(myMainWindow);
    
    myView = new SVTK_View(myMainWindow);
    Initialize(myView,theModel);
  }
}

void
SVTK_ViewWindow
::Initialize(SVTK_View* theView,
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

SVTK_ViewWindow
::~SVTK_ViewWindow()
{}


//----------------------------------------------------------------------------
SVTK_View* 
SVTK_ViewWindow
::getView() 
{ 
  return myView; 
}

SVTK_MainWindow* 
SVTK_ViewWindow
::getMainWindow() 
{ 
  return myMainWindow; 
}

vtkRenderWindow*
SVTK_ViewWindow
::getRenderWindow()
{
  return getMainWindow()->getRenderWindow();
}

vtkRenderWindowInteractor*
SVTK_ViewWindow
::getInteractor()
{
  return getMainWindow()->getInteractor();
}

vtkRenderer*
SVTK_ViewWindow
::getRenderer()
{
  return myMainWindow->getRenderer();
}

SVTK_Selector* 
SVTK_ViewWindow
::GetSelector() 
{ 
  return myMainWindow->GetSelector(); 
}


//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onFrontView()
{
  myMainWindow->onFrontView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onBackView()
{
  myMainWindow->onBackView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onTopView()
{
  myMainWindow->onTopView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onBottomView()
{
  myMainWindow->onBottomView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onLeftView()
{
  myMainWindow->onLeftView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onRightView()
{
  myMainWindow->onRightView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onResetView()
{
  myMainWindow->onResetView();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onFitAll()
{
  myMainWindow->onFitAll();
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
  myMainWindow->SetSelectionMode( theMode );
}

//----------------------------------------------------------------
Selection_Mode
SVTK_ViewWindow
::SelectionMode() const
{
  return myMainWindow->SelectionMode();
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
::highlight(const Handle(SALOME_InteractiveObject)& theIO, 
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

//----------------------------------------------------------------
void
SVTK_ViewWindow
::Display(const Handle(SALOME_InteractiveObject)& theIO,
	  bool theImmediatly) 
{
  myView->Display(theIO,theImmediatly);
}

void
SVTK_ViewWindow
::Erase(const Handle(SALOME_InteractiveObject)& theIO,
	  bool theImmediatly) 
{
  myView->Erase(theIO,theImmediatly);
}

void
SVTK_ViewWindow
::DisplayOnly(const Handle(SALOME_InteractiveObject)& theIO) 
{
  myView->DisplayOnly(theIO);
}

void 
SVTK_ViewWindow
::DisplayAll() 
{
  myView->DisplayAll();
}

void 
SVTK_ViewWindow
::EraseAll() 
{
  myView->EraseAll();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::setBackgroundColor( const QColor& color )
{
  myMainWindow->SetBackgroundColor( color );
}

//----------------------------------------------------------------------------
QColor
SVTK_ViewWindow
::backgroundColor() const
{
  return myMainWindow->BackgroundColor();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::Repaint(bool theUpdateTrihedron)
{
  myMainWindow->Repaint( theUpdateTrihedron );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::GetScale( double theScale[3] ) 
{
  myMainWindow->GetScale( theScale );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::SetScale( double theScale[3] ) 
{
  myMainWindow->SetScale( theScale );
}

//----------------------------------------------------------------------------
bool
SVTK_ViewWindow
::isTrihedronDisplayed()
{
  return myMainWindow->IsTrihedronDisplayed();
}

bool
SVTK_ViewWindow
::isCubeAxesDisplayed()
{
  return myMainWindow->IsCubeAxesDisplayed();
}

//----------------------------------------------------------------------------
void 
SVTK_ViewWindow
::onViewTrihedron()
{
  myMainWindow->onViewTrihedron();
}

void
SVTK_ViewWindow
::onViewCubeAxes()
{
  myMainWindow->onViewCubeAxes();
}

//----------------------------------------------------------------------------
VTKViewer_Trihedron* 
SVTK_ViewWindow::
GetTrihedron()
{
  return myMainWindow->GetTrihedron();
}

SVTK_CubeAxesActor2D* 
SVTK_ViewWindow
::GetCubeAxes()
{
  return myMainWindow->GetCubeAxes();
}

int
SVTK_ViewWindow
::GetTrihedronSize() const
{
  return myMainWindow->GetTrihedronSize();
}

void
SVTK_ViewWindow
::SetTrihedronSize(const int theSize, const bool theRelative)
{
  myMainWindow->SetTrihedronSize(theSize, theRelative);
}

/*! If parameter theIsForcedUpdate is true, recalculate parameters for
 *  trihedron and cube axes, even if trihedron and cube axes is invisible.
 */
void
SVTK_ViewWindow
::AdjustTrihedrons(const bool theIsForcedUpdate)
{
  myMainWindow->AdjustActors();
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::onAdjustTrihedron()
{   
  myMainWindow->onAdjustTrihedron();
}

void
SVTK_ViewWindow
::onAdjustCubeAxes()
{   
  myMainWindow->onAdjustCubeAxes();
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
::AddActor( VTKViewer_Actor* theActor, 
	    bool theUpdate )
{
  myMainWindow->AddActor( theActor, theUpdate );
}

//----------------------------------------------------------------------------
void
SVTK_ViewWindow
::RemoveActor( VTKViewer_Actor* theActor, 
	       bool theUpdate )
{
  myMainWindow->RemoveActor( theActor, theUpdate );
}

//----------------------------------------------------------------------------
QImage
SVTK_ViewWindow
::dumpView()
{
  return myMainWindow->dumpView();
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
::SetPreselectionProp(const double& theRed, 
		      const double& theGreen, 
		      const double& theBlue, 
		      const int& theWidth) 
{
  myView->SetPreselectionProp(theRed,theGreen,theBlue,theWidth);
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
    int anEvent = convertAction( accelAction );
    myMainWindow->InvokeEvent( anEvent, 0 );
  }
}

// old visual parameters had 13 values.  New format added additional 
// 76 values for graduated axes, so both numbers are processed.
const int nNormalParams = 13;   // number of view windows parameters excluding graduated axes params
const int nGradAxisParams = 25; // number of parameters of ONE graduated axis (X, Y, or Z)
const int nAllParams = nNormalParams + 3*nGradAxisParams + 1; // number of all visual parameters

/*! The method returns visual parameters of a graduated axis actor (x,y,z axis of graduated axes)
 */
QString getGradAxisVisualParams( vtkAxisActor2D* actor )
{
  QString params;
  if ( !actor )
    return params;

  // Name
  bool isVisible = actor->GetTitleVisibility();
  QString title ( actor->GetTitle() );
  float color[ 3 ];
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
  params.sprintf( "* Graduated Axis: * Name *%u*%s*%.2f*%.2f*%.2f*%u*%u*%u*%u", isVisible, 
		  title.latin1(), color[0], color[1], color[2], font, bold, italic, shadow );

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
  params += QString().sprintf( "* Labels *%u*%u*%u*%.2f*%.2f*%.2f*%u*%u*%u*%u", isVisible, labels, offset,  
			       color[0], color[1], color[2], font, bold, italic, shadow );

  // Tick marks
  isVisible = actor->GetTickVisibility();
  int length = actor->GetTickLength();
  
  params += QString().sprintf( "* Tick marks *%u*%u", isVisible, length );
  
  return params;
}

/*! The method restores visual parameters of a graduated axis actor (x,y,z axis)
 */
void setGradAxisVisualParams( vtkAxisActor2D* actor, const QString& params )
{
  if ( !actor )
    return;

  QStringList paramsLst = QStringList::split( '*', params, true );

  if ( paramsLst.size() == nGradAxisParams ) { // altogether name, lable, ticks parameters make up 25 values

    // retrieve and set name parameters
    bool isVisible = paramsLst[2].toUShort();
    QString title = paramsLst[3];
    float color[3];
    color[0] = paramsLst[4].toDouble();
    color[1] = paramsLst[5].toDouble();
    color[2] = paramsLst[6].toDouble();
    int font = paramsLst[7].toInt();
    int bold = paramsLst[8].toInt();
    int italic = paramsLst[9].toInt();
    int shadow = paramsLst[10].toInt();

    actor->SetTitleVisibility( isVisible );
    actor->SetTitle( title.latin1() );
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
QString
SVTK_ViewWindow
::getVisualParameters()
{
  double pos[3], focalPnt[3], viewUp[3], parScale, scale[3];
  
  // save position, focal point, viewUp, scale
  vtkCamera* camera = getRenderer()->GetActiveCamera();
  camera->GetPosition( pos );
  camera->GetFocalPoint( focalPnt );
  camera->GetViewUp( viewUp );
  parScale = camera->GetParallelScale();
  GetScale( scale );

  // Parameters are given in the following format:view position (3 digits), focal point position (3 digits)
  // view up values (3 digits), parallel scale (1 digit), scale (3 digits, 
  // Graduated axes parameters (X, Y, Z axes parameters)
  QString retStr;
  retStr.sprintf( "%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e", 
		  pos[0], pos[1], pos[2], focalPnt[0], focalPnt[1], focalPnt[2], 
		  viewUp[0], viewUp[1], viewUp[2], parScale, scale[0], scale[1], scale[2] );

  // save graduated axes parameters
  if ( SVTK_CubeAxesActor2D* gradAxesActor = GetCubeAxes() ) {
    retStr += QString( "*%1" ).arg( getMainWindow()->IsCubeAxesDisplayed() );
    retStr += ::getGradAxisVisualParams( gradAxesActor->GetXAxisActor2D() );
    retStr += ::getGradAxisVisualParams( gradAxesActor->GetYAxisActor2D() );
    retStr += ::getGradAxisVisualParams( gradAxesActor->GetZAxisActor2D() );
  }

  return retStr;
}

/* The method restores visual parameters of this view or postpones it untill the view is shown
 */
void
SVTK_ViewWindow
::setVisualParameters( const QString& parameters )
{
  SVTK_RenderWindowInteractor* anInteractor = getMainWindow()->GetInteractor();
  if ( anInteractor->isVisible() ) {
    doSetVisualParameters( parameters ); 
  }
  else {
    myVisualParams = parameters;
    anInteractor->installEventFilter(this);
  }
}

/* The method restores visual parameters of this view from a formated string
 */
void
SVTK_ViewWindow
::doSetVisualParameters( const QString& parameters )
{
  QStringList paramsLst = QStringList::split( '*', parameters, true );
  if ( paramsLst.size() >= nNormalParams ) {
    // 'reading' list of parameters
    double pos[3], focalPnt[3], viewUp[3], parScale, scale[3];
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


//================================================================
// Function : eventFilter
/*! Purpose : delayed setVisualParameters
*/
//================================================================
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
