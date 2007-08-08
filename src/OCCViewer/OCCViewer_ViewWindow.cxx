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
// File   : OCCViewer_ViewWindow.cxx
// Author :
//

#include "OCCViewer_ViewWindow.h"
#include "OCCViewer_ViewModel.h"
#include "OCCViewer_ViewPort3d.h"
#include "OCCViewer_CreateRestoreViewDlg.h"
#include "OCCViewer_ClippingDlg.h"
#include "OCCViewer_SetRotationPointDlg.h"

#include <SUIT_Desktop.h>
#include <SUIT_Session.h>
#include <SUIT_ViewManager.h>
#include <SUIT_Tools.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_MessageBox.h>

#include <QtxToolBar.h>
#include <QtxMultiAction.h>

#include <QPainter>
#include <QTime>
#include <QImage>
#include <QMouseEvent>
#include <QRubberBand>

#include <V3d_Plane.hxx>
#include <gp_Dir.hxx>
#include <gp_Pln.hxx>

#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <AIS_Shape.hxx>

#include <BRep_Tool.hxx>
#include <TopoDS.hxx>

#include <BRepBndLib.hxx>
#include <Graphic3d_MapIteratorOfMapOfStructure.hxx>
#include <Visual3d_View.hxx>
#include <Graphic3d_MapOfStructure.hxx>
#include <Graphic3d_Structure.hxx>

const char* imageZoomCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
".#######........................",
"..aaaaaaa.......................",
"................................",
".............#####..............",
"...........##.aaaa##............",
"..........#.aa.....a#...........",
".........#.a.........#..........",
".........#a..........#a.........",
"........#.a...........#.........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
".........#...........#.a........",
".........#a..........#a.........",
".........##.........#.a.........",
"........#####.....##.a..........",
".......###aaa#####.aa...........",
"......###aa...aaaaa.......#.....",
".....###aa................#a....",
"....###aa.................#a....",
"...###aa...............#######..",
"....#aa.................aa#aaaa.",
".....a....................#a....",
"..........................#a....",
"...........................a....",
"................................",
"................................",
"................................",
"................................"};

const char* imageRotateCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
"................................",
"................................",
"........#.......................",
".......#.a......................",
"......#######...................",
".......#aaaaa#####..............",
"........#..##.a#aa##........##..",
".........a#.aa..#..a#.....##.aa.",
".........#.a.....#...#..##.aa...",
".........#a.......#..###.aa.....",
"........#.a.......#a..#aa.......",
"........#a.........#..#a........",
"........#a.........#a.#a........",
"........#a.........#a.#a........",
"........#a.........#a.#a........",
".........#.........#a#.a........",
"........##a........#a#a.........",
"......##.a#.......#.#.a.........",
"....##.aa..##.....##.a..........",
"..##.aa.....a#####.aa...........",
"...aa.........aaa#a.............",
"................#.a.............",
"...............#.a..............",
"..............#.a...............",
"...............a................",
"................................",
"................................",
"................................",
"................................",
"................................"};

const char* imageCrossCursor[] = { 
  "32 32 3 1",
  ". c None",
  "a c #000000",
  "# c #ffffff",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "...............#................",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  ".......#################........",
  "........aaaaaaa#aaaaaaaaa.......",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "...............#a...............",
  "................a...............",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................",
  "................................"};


/*!
  \brief Constructor
  \param theDesktop main window of application
  \param theModel OCC 3D viewer
*/
OCCViewer_ViewWindow::OCCViewer_ViewWindow( SUIT_Desktop*     theDesktop, 
					    OCCViewer_Viewer* theModel )
: SUIT_ViewWindow( theDesktop )
{
  myModel = theModel;
  myRestoreFlag = 0;
  myEnableDrawMode = false;
  updateEnabledDrawMode();
  myClippingDlg = 0;
  mySetRotationPointDlg = 0;
  myRectBand = 0;
}

/*!
  \brief Destructor.
*/
OCCViewer_ViewWindow::~OCCViewer_ViewWindow()
{
  endDrawRect();
}

/*!
  \brief Internal initialization.
*/
void OCCViewer_ViewWindow::initLayout()
{
  myViewPort = new OCCViewer_ViewPort3d( this, myModel->getViewer3d(), V3d_ORTHOGRAPHIC );
  myViewPort->setBackgroundColor(Qt::black);
  myViewPort->installEventFilter(this);
  setCentralWidget(myViewPort);
  myOperation = NOTHING;

  myCurrPointType = GRAVITY;
  myPrevPointType = GRAVITY;
  mySelectedPoint = gp_Pnt(0.,0.,0.);
  myRotationPointSelection = false;

  setTransformRequested ( NOTHING );
  setTransformInProcess ( false );

  myToolBar = new QtxToolBar( true, tr("LBL_TOOLBAR_LABEL"), this );

  createActions();
  createToolBar();
}

/*!
  \brief Detect viewer operation according the the mouse button pressed
  and key modifiers used.
  \param theEvent mouse event
  \return type of the operation
*/
OCCViewer_ViewWindow::OperationType 
OCCViewer_ViewWindow::getButtonState( QMouseEvent* theEvent )
{
  OperationType aOp = NOTHING;
  if( (theEvent->modifiers() == SUIT_ViewModel::myStateMap[SUIT_ViewModel::ZOOM]) &&
      (theEvent->button() == SUIT_ViewModel::myButtonMap[SUIT_ViewModel::ZOOM]) )
    aOp = ZOOMVIEW;
  else if( (theEvent->modifiers() == SUIT_ViewModel::myStateMap[SUIT_ViewModel::PAN]) && 
           (theEvent->button() == SUIT_ViewModel::myButtonMap[SUIT_ViewModel::PAN]) )
    aOp = PANVIEW;
  else if( (theEvent->modifiers()  == SUIT_ViewModel::myStateMap[SUIT_ViewModel::ROTATE]) &&
           (theEvent->button() == SUIT_ViewModel::myButtonMap[SUIT_ViewModel::ROTATE]) )
    aOp = ROTATE;

  return aOp;
}

/*!
  \brief Customize event handling
  \param watched event receiver object
  \param e event
  \return \c true if the event processing should be stopped
*/
bool OCCViewer_ViewWindow::eventFilter( QObject* watched, QEvent* e )
{
  if ( watched == myViewPort ) {
    int aType = e->type();
    switch(aType) {
    case QEvent::MouseButtonPress:
      vpMousePressEvent((QMouseEvent*) e);
      return true;

    case QEvent::MouseButtonRelease:
      vpMouseReleaseEvent((QMouseEvent*) e);
      return true;

    case QEvent::MouseMove:
      vpMouseMoveEvent((QMouseEvent*) e);
      return true;

    case QEvent::MouseButtonDblClick:
      emit mouseDoubleClicked(this, (QMouseEvent*)e);
      return true;

    case QEvent::Wheel:
      {
        QWheelEvent* aEvent = (QWheelEvent*) e;
        double aDelta = aEvent->delta();
        double aScale = (aDelta < 0) ? 100./(-aDelta) : aDelta/100.; 
        myViewPort->getView()->SetZoom(aScale);
      }
      return true;

    case QEvent::ContextMenu:
      {
        QContextMenuEvent * aEvent = (QContextMenuEvent*)e;
        if ( aEvent->reason() != QContextMenuEvent::Mouse )
          emit contextMenuRequested( aEvent );
      }
      return true;

    default:
      break;
    }
  }
  return SUIT_ViewWindow::eventFilter(watched, e);
}

/*!
  \brief Update state of enable draw mode state.
*/
void OCCViewer_ViewWindow::updateEnabledDrawMode()
{
  if ( myModel )
    myEnableDrawMode = myModel->isSelectionEnabled() && myModel->isMultiSelectionEnabled();
}

/*!
  \brief Handle mouse press event
  \param theEvent mouse event
*/
void OCCViewer_ViewWindow::vpMousePressEvent( QMouseEvent* theEvent )
{
  myStartX = theEvent->x();
  myStartY = theEvent->y();
  switch ( myOperation ) {
  case WINDOWFIT:
    if ( theEvent->button() == Qt::LeftButton )
      emit vpTransformationStarted ( WINDOWFIT );
    break;    
   
  case PANGLOBAL:
    if ( theEvent->button() == Qt::LeftButton )
      emit vpTransformationStarted ( PANGLOBAL );
    break;    
    
  case ZOOMVIEW:
    if ( theEvent->button() == Qt::LeftButton )
      emit vpTransformationStarted ( ZOOMVIEW );
    break;
    
  case PANVIEW:
    if ( theEvent->button() == Qt::LeftButton )
      emit vpTransformationStarted ( PANVIEW );
    break;

  case ROTATE:
    if ( theEvent->button() == Qt::LeftButton ) {
	    myViewPort->startRotation(myStartX, myStartY, myCurrPointType, mySelectedPoint);
	    emit vpTransformationStarted ( ROTATE );
	  }
    break;
      
  default:
  /*  Try to activate a transformation */
    switch ( getButtonState(theEvent) ) {
    case ZOOMVIEW:
	    activateZoom();
      break;
    case PANVIEW:
	    activatePanning();
      break;
    case ROTATE:
	    activateRotation();
	    myViewPort->startRotation(myStartX, myStartY, myCurrPointType, mySelectedPoint);
      break;
    default:
      if ( myRotationPointSelection )
      {
	if ( theEvent->button() == Qt::LeftButton )
	{
	  Handle(AIS_InteractiveContext) ic = myModel->getAISContext();
	  ic->Select();
	  for ( ic->InitSelected(); ic->MoreSelected(); ic->NextSelected() )
	  {
	    TopoDS_Shape aShape = ic->SelectedShape();
	    if ( !aShape.IsNull() && aShape.ShapeType() == TopAbs_VERTEX )
	    {
	      gp_Pnt aPnt = BRep_Tool::Pnt( TopoDS::Vertex( ic->SelectedShape() ) ); 
	      if ( mySetRotationPointDlg )
	      {
		myRotationPointSelection = false;
		mySetRotationPointDlg->setCoords(aPnt.X(), aPnt.Y(), aPnt.Z());
	      }
	    }	 
	    else 
	    {
	      myCurrPointType = myPrevPointType;
	      break;
	    }
	  }
	  if ( ic->NbSelected() == 0 ) myCurrPointType = myPrevPointType;
	  if ( mySetRotationPointDlg ) mySetRotationPointDlg->toggleChange();
	  ic->CloseAllContexts();
	  myOperation = NOTHING; 
	  myViewPort->setCursor( myCursor );
	  myCursorIsHand = false;
	  myRotationPointSelection = false;
	}
      }
      else
	emit mousePressed(this, theEvent);
      break;
    }
    /* notify that we start a transformation */
    if ( transformRequested() ) 
	    emit vpTransformationStarted ( myOperation );
  }
  if ( transformRequested() ) 
    setTransformInProcess( true );		 
}


/*!
  \brief Start zooming operation.

  Sets the corresponding cursor for the widget.
*/
void OCCViewer_ViewWindow::activateZoom()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        /* save old cursor */
  
  if ( myOperation != ZOOMVIEW ) {
    QPixmap zoomPixmap (imageZoomCursor);
    QCursor zoomCursor (zoomPixmap);
    setTransformRequested ( ZOOMVIEW );		
    myViewPort->setCursor( zoomCursor );
  }
}


/*!
  \brief Start panning operation.
  
  Sets the corresponding cursor for the widget.
*/
void OCCViewer_ViewWindow::activatePanning()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        // save old cursor 
  
  if ( myOperation != PANVIEW ) {
    QCursor panCursor (Qt::SizeAllCursor);
    setTransformRequested ( PANVIEW );
    myViewPort->setCursor( panCursor );
  }
}

/*!
  \brief Start rotation operation

  Sets the corresponding cursor for the widget.
*/
void OCCViewer_ViewWindow::activateRotation()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        // save old cursor 
  
  if ( myOperation != ROTATE ) {
    QPixmap rotatePixmap (imageRotateCursor);
    QCursor rotCursor (rotatePixmap);
    setTransformRequested ( ROTATE );
    myViewPort->setCursor( rotCursor );	
  }
}

/*!
  \brief Compute the gravity center.
  \param theX used to return X coordinate of the gravity center
  \param theY used to return Y coordinate of the gravity center
  \param theZ used to return Z coordinate of the gravity center
  \return \c true if the gravity center is computed
*/
bool OCCViewer_ViewWindow::computeGravityCenter( double& theX, double& theY, double& theZ )
{
  Handle(Visual3d_View) aView = myViewPort->getView()->View();

  Standard_Real Xmin,Ymin,Zmin,Xmax,Ymax,Zmax,U,V,W ;
  Standard_Real Umin,Vmin,Umax,Vmax ;
  Standard_Integer Nstruct,Npoint ;
  Graphic3d_MapOfStructure MySetOfStructures;
  
  aView->DisplayedStructures (MySetOfStructures);
  Nstruct = MySetOfStructures.Extent() ;
  
  Graphic3d_MapIteratorOfMapOfStructure MyIterator(MySetOfStructures) ;
  aView->ViewMapping().WindowLimit(Umin,Vmin,Umax,Vmax) ;
  Npoint = 0 ; theX = theY = theZ = 0. ;
  for( ; MyIterator.More(); MyIterator.Next()) {
    if (!(MyIterator.Key())->IsEmpty()) {
      (MyIterator.Key())->MinMaxValues(Xmin,Ymin,Zmin,
                                         Xmax,Ymax,Zmax) ;
    
      Standard_Real LIM = ShortRealLast() -1.;
      if (!    (fabs(Xmin) > LIM || fabs(Ymin) > LIM || fabs(Zmin) > LIM 
                ||  fabs(Xmax) > LIM || fabs(Ymax) > LIM || fabs(Zmax) > LIM )) {
        
        aView->Projects(Xmin,Ymin,Zmin,U,V,W) ;
        if( U >= Umin && U <= Umax && V >= Vmin && V <= Vmax ) {
          Npoint++ ; theX += Xmin ; theY += Ymin ; theZ += Zmin ;
        }
        aView->Projects(Xmax,Ymin,Zmin,U,V,W) ;
        if( U >= Umin && U <= Umax && V >= Vmin && V <= Vmax ) {
          Npoint++ ; theX += Xmax ; theY += Ymin ; theZ += Zmin ;
        }
        aView->Projects(Xmin,Ymax,Zmin,U,V,W) ;
        if( U >= Umin && U <= Umax && V >= Vmin && V <= Vmax ) {
          Npoint++ ; theX += Xmin ; theY += Ymax ; theZ += Zmin ;
        }
        aView->Projects(Xmax,Ymax,Zmin,U,V,W) ;
        if( U >= Umin && U <= Umax && V >= Vmin && V <= Vmax ) {
          Npoint++ ; theX += Xmax ; theY += Ymax ; theZ += Zmin ;
        }
        aView->Projects(Xmin,Ymin,Zmax,U,V,W) ;
        if( U >= Umin && U <= Umax && V >= Vmin && V <= Vmax ) {
          Npoint++ ; theX += Xmin ; theY += Ymin ; theZ += Zmax ;
        }
        aView->Projects(Xmax,Ymin,Zmax,U,V,W) ;
        if( U >= Umin && U <= Umax && V >= Vmin && V <= Vmax ) {
          Npoint++ ; theX += Xmax ; theY += Ymin ; theZ += Zmax ;
        }
        aView->Projects(Xmin,Ymax,Zmax,U,V,W) ;
        if( U >= Umin && U <= Umax && V >= Vmin && V <= Vmax ) {
          Npoint++ ; theX += Xmin ; theY += Ymax ; theZ += Zmax ;
        }
        aView->Projects(Xmax,Ymax,Zmax,U,V,W) ;
        if( U >= Umin && U <= Umax && V >= Vmin && V <= Vmax ) {
          Npoint++ ; theX += Xmax ; theY += Ymax ; theZ += Zmax ;
        }
      }
    }
  }
  if( Npoint > 0 ) {
    theX /= Npoint ; theY /= Npoint ; theZ /= Npoint ;
  }
  return true;
}

/*!
  \brief Set the gravity center as a rotation point.
*/
void OCCViewer_ViewWindow::activateSetRotationGravity()
{
  if ( myRotationPointSelection )
  {
    Handle(AIS_InteractiveContext) ic = myModel->getAISContext();
    ic->CloseAllContexts();
    myOperation = NOTHING; 
    myViewPort->setCursor( myCursor );
    myCursorIsHand = false;
    myRotationPointSelection = false;
  }

  myPrevPointType = myCurrPointType;
  myCurrPointType = GRAVITY;

  Standard_Real Xcenter, Ycenter, Zcenter;
  if ( computeGravityCenter( Xcenter, Ycenter, Zcenter ) )
    mySetRotationPointDlg->setCoords( Xcenter, Ycenter, Zcenter );
}

/*!
  \brief Update gravity center in the "Set Rotation Point" dialog box.
  \sa OCCViewer_SetRotationPointDlg class
*/
void OCCViewer_ViewWindow::updateGravityCoords()
{
  if ( mySetRotationPointDlg && mySetRotationPointDlg->isVisible() && myCurrPointType == GRAVITY )
  {
    Standard_Real Xcenter, Ycenter, Zcenter;
    if ( computeGravityCenter( Xcenter, Ycenter, Zcenter ) )
      mySetRotationPointDlg->setCoords( Xcenter, Ycenter, Zcenter );
  }
}

/*!
  \brief Set the point selected by the user as a rotation point.
  \param theX X coordinate of the rotation point
  \param theY Y coordinate of the rotation point
  \param theZ Z coordinate of the rotation point
*/
void OCCViewer_ViewWindow::activateSetRotationSelected( double theX, double theY, double theZ )
{
  if ( myRotationPointSelection )
  {
    Handle(AIS_InteractiveContext) ic = myModel->getAISContext();
    ic->CloseAllContexts();
    myOperation = NOTHING; 
    myViewPort->setCursor( myCursor );
    myCursorIsHand = false;
    myRotationPointSelection = false;
  }

  myPrevPointType = myCurrPointType;
  myCurrPointType = SELECTED;
  mySelectedPoint.SetCoord(theX,theY,theZ);
}

/*!
  \brief Start the point selection process.
*/
void OCCViewer_ViewWindow::activateStartPointSelection()
{
  myPrevPointType = myCurrPointType;
  myCurrPointType = SELECTED;

  // activate selection ------>
  Handle(AIS_InteractiveContext) ic = myModel->getAISContext();

  ic->OpenLocalContext();

  AIS_ListOfInteractive aList;
  ic->DisplayedObjects( aList );
  for ( AIS_ListIteratorOfListOfInteractive it( aList ); it.More(); it.Next() ) 
  {
    Handle(AIS_InteractiveObject) anObj = it.Value();
    if ( !anObj.IsNull() && anObj->HasPresentation() &&
         anObj->IsKind( STANDARD_TYPE(AIS_Shape) ) )
    {
      ic->Load(anObj,-1);
      ic->Activate(anObj,AIS_Shape::SelectionMode(TopAbs_VERTEX));
     }
  }  
  // activate selection <------

  if ( !myCursorIsHand )
  {
    QCursor handCursor (Qt::PointingHandCursor);
    myCursorIsHand = true;		
    myCursor = cursor();
    myViewPort->setCursor( handCursor );
  }
  myRotationPointSelection = true;
}

/*!
  \brief Start global panning operation

  Sets the corresponding cursor for the widget.
*/
void OCCViewer_ViewWindow::activateGlobalPanning()
{
  Handle(V3d_View) aView3d = myViewPort->getView();
  if ( !aView3d.IsNull() ) {
    QPixmap globalPanPixmap (imageCrossCursor);
    QCursor glPanCursor (globalPanPixmap);
    myCurScale = aView3d->Scale();
    aView3d->FitAll(0.01, false);
    myCursor = cursor();	        // save old cursor 
    myViewPort->fitAll(); // fits view before selecting a new scene center 
    setTransformRequested( PANGLOBAL );
    myViewPort->setCursor( glPanCursor );
  }
}

/*!
  \brief Starts fit operation.

  Sets the corresponding cursor for the widget.
*/
void OCCViewer_ViewWindow::activateWindowFit()
{
  if ( !transformRequested() && !myCursorIsHand )
    myCursor = cursor();	        /* save old cursor */

  if ( myOperation != WINDOWFIT ) {
    QCursor handCursor (Qt::PointingHandCursor);
    setTransformRequested ( WINDOWFIT );		
    myViewPort->setCursor ( handCursor );
    myCursorIsHand = true;
  }
}

/*!
  \brief Start delayed viewer operation.
*/
void OCCViewer_ViewWindow::setTransformRequested( OperationType op )
{    
  myOperation = op;
  myViewPort->setMouseTracking( myOperation == NOTHING );
}


/*!
  \brief Handle mouse move event.
  \param theEvent mouse event
*/
void OCCViewer_ViewWindow::vpMouseMoveEvent( QMouseEvent* theEvent )
{
  myCurrX = theEvent->x();
  myCurrY = theEvent->y();
  switch (myOperation) {
  case ROTATE:
    myViewPort->rotate(myCurrX, myCurrY, myCurrPointType, mySelectedPoint);
    break;
    
  case ZOOMVIEW:
    myViewPort->zoom(myStartX, myStartY, myCurrX, myCurrY);
    myStartX = myCurrX;
    myStartY = myCurrY;
    break;
    
  case PANVIEW:
    myViewPort->pan(myCurrX - myStartX, myStartY - myCurrY);
    myStartX = myCurrX;
    myStartY = myCurrY;
    break;
    
/*    case WINDOWFIT:
    myDrawRect = true;
    repaint();
    break;
*/      
  case PANGLOBAL:
    break;
    
  default:
    if ( myRotationPointSelection )
      emit mouseMoving( this, theEvent ); 
    else
    {
      int aState = theEvent->modifiers();
      int aButton = theEvent->buttons();
      if ( aButton == Qt::LeftButton ||
	   ( aButton == Qt::LeftButton && aState == Qt::ShiftModifier ) )	{
	myDrawRect = myEnableDrawMode;
	if ( myDrawRect ) {
	  drawRect();
	  if ( !myCursorIsHand )	{   // we are going to sketch a rectangle
	    QCursor handCursor (Qt::PointingHandCursor);
	    myCursorIsHand = true;		
	    myCursor = cursor();
	    myViewPort->setCursor( handCursor );
	  }
	}
      } 
      else
	emit mouseMoving( this, theEvent ); 
    }	
  }
}

/*!
  \brief Handle mouse release event.
  \param theEvent mouse event
*/
void OCCViewer_ViewWindow::vpMouseReleaseEvent(QMouseEvent* theEvent)
{
  switch ( myOperation ) {
  case NOTHING:
    {
      emit mouseReleased(this, theEvent);
      if(theEvent->button() == Qt::RightButton)
      {
        QContextMenuEvent aEvent( QContextMenuEvent::Mouse,
                                  theEvent->pos(), theEvent->globalPos() );
        emit contextMenuRequested( &aEvent );
      }
    }
    break;
  case ROTATE:
    myViewPort->endRotation();
    resetState();
    break;
    
  case PANVIEW:
  case ZOOMVIEW:
    resetState();
    break;
    
  case PANGLOBAL:
    if ( theEvent->button() == Qt::LeftButton ) {
      myViewPort->setCenter( theEvent->x(), theEvent->y() );
      myViewPort->getView()->SetScale(myCurScale);
      resetState();
    }
    break;
      
  case WINDOWFIT:
    if ( theEvent->button() == Qt::LeftButton ) {
      myCurrX = theEvent->x();
      myCurrY = theEvent->y();
      drawRect();
      QRect rect = SUIT_Tools::makeRect(myStartX, myStartY, myCurrX, myCurrY);
      if ( !rect.isEmpty() ) myViewPort->fitRect(rect);
      endDrawRect();
      resetState();
    }
    break;
  }
  
  // NOTE: viewer 3D detects a rectangle of selection using this event
  // so we must emit it BEFORE resetting the selection rectangle
  
  if ( theEvent->button() == Qt::LeftButton && myDrawRect ) {
    drawRect();
    endDrawRect();
    resetState(); 
    myViewPort->update();
  }
}

/*!
  \brief Reset the viewport to its initial state
  ( no transformations in process etc. )
*/
void OCCViewer_ViewWindow::resetState()
{
  myDrawRect = false;
  
  if ( myRotationPointSelection )
  {
    QCursor handCursor (Qt::PointingHandCursor);
    myViewPort->setCursor( handCursor );
  }
  else
  { 
    if ( transformRequested() || myCursorIsHand ) 
      myViewPort->setCursor( myCursor );
    myCursorIsHand = false;
  }
  
  if ( transformRequested() ) 
    emit vpTransformationFinished (myOperation);
  
  setTransformInProcess( false );		
  setTransformRequested( NOTHING );	
}


/*!
  \brief Draw rubber band rectangle.
*/
void OCCViewer_ViewWindow::drawRect()
{
  if ( !myRectBand ) {
    myRectBand = new QRubberBand( QRubberBand::Rectangle, myViewPort );
    QPalette palette;
    palette.setColor(myRectBand->foregroundRole(), Qt::white);
    myRectBand->setPalette(palette);
  }
  myRectBand->hide();

  QRect aRect = SUIT_Tools::makeRect(myStartX, myStartY, myCurrX, myCurrY);
  myRectBand->setGeometry( aRect );
  myRectBand->setVisible( aRect.isValid() );
}

/*!
  \brief Clear rubber band rectangle on the end on the dragging operation.
*/
void OCCViewer_ViewWindow::endDrawRect()
{
  delete myRectBand;
  myRectBand = 0;
}

/*!
  \brief Create actions.
*/
void OCCViewer_ViewWindow::createActions()
{
  if (!myActionsMap.isEmpty()) return;
  
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  
  QtxAction* aAction;

  // Dump view
  aAction = new QtxAction(tr("MNU_DUMP_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_DUMP" ) ),
                           tr( "MNU_DUMP_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_DUMP_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onDumpView()));
	myActionsMap[ DumpId ] = aAction;

  // FitAll
  aAction = new QtxAction(tr("MNU_FITALL"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_FITALL" ) ),
                           tr( "MNU_FITALL" ), 0, this);
  aAction->setStatusTip(tr("DSC_FITALL"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onFitAll()));
	myActionsMap[ FitAllId ] = aAction;

  // FitRect
  aAction = new QtxAction(tr("MNU_FITRECT"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_FITAREA" ) ),
                           tr( "MNU_FITRECT" ), 0, this);
  aAction->setStatusTip(tr("DSC_FITRECT"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateWindowFit()));
	myActionsMap[ FitRectId ] = aAction;

  // Zoom
  aAction = new QtxAction(tr("MNU_ZOOM_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_ZOOM" ) ),
                           tr( "MNU_ZOOM_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_ZOOM_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateZoom()));
	myActionsMap[ ZoomId ] = aAction;

  // Panning
  aAction = new QtxAction(tr("MNU_PAN_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_PAN" ) ),
                           tr( "MNU_PAN_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_PAN_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activatePanning()));
	myActionsMap[ PanId ] = aAction;

  // Global Panning
  aAction = new QtxAction(tr("MNU_GLOBALPAN_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_GLOBALPAN" ) ),
                           tr( "MNU_GLOBALPAN_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_GLOBALPAN_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateGlobalPanning()));
  myActionsMap[ GlobalPanId ] = aAction;

  // Rotation Point
  mySetRotationPointAction = new QtxAction(tr("MNU_CHANGINGROTATIONPOINT_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_ROTATION_POINT" ) ),
                           tr( "MNU_CHANGINGROTATIONPOINT_VIEW" ), 0, this);
  mySetRotationPointAction->setStatusTip(tr("DSC_CHANGINGROTATIONPOINT_VIEW"));
  mySetRotationPointAction->setCheckable( true );
  connect(mySetRotationPointAction, SIGNAL(toggled( bool )), this, SLOT(onSetRotationPoint( bool )));
  myActionsMap[ ChangeRotationPointId ] = mySetRotationPointAction;

  // Rotation
  aAction = new QtxAction(tr("MNU_ROTATE_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_ROTATE" ) ),
                           tr( "MNU_ROTATE_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_ROTATE_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(activateRotation()));
	myActionsMap[ RotationId ] = aAction;

  // Projections
  aAction = new QtxAction(tr("MNU_FRONT_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_FRONT" ) ),
                           tr( "MNU_FRONT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_FRONT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onFrontView()));
	myActionsMap[ FrontId ] = aAction;

  aAction = new QtxAction(tr("MNU_BACK_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_BACK" ) ),
                           tr( "MNU_BACK_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_BACK_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onBackView()));
	myActionsMap[ BackId ] = aAction;

  aAction = new QtxAction(tr("MNU_TOP_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_TOP" ) ),
                           tr( "MNU_TOP_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_TOP_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onTopView()));
	myActionsMap[ TopId ] = aAction;

  aAction = new QtxAction(tr("MNU_BOTTOM_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_BOTTOM" ) ),
                           tr( "MNU_BOTTOM_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_BOTTOM_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onBottomView()));
	myActionsMap[ BottomId ] = aAction;

  aAction = new QtxAction(tr("MNU_LEFT_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_LEFT" ) ),
                           tr( "MNU_LEFT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_LEFT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onLeftView()));
	myActionsMap[ LeftId ] = aAction;

  aAction = new QtxAction(tr("MNU_RIGHT_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_RIGHT" ) ),
                           tr( "MNU_RIGHT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_RIGHT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onRightView()));
	myActionsMap[ RightId ] = aAction;

  // Reset
  aAction = new QtxAction(tr("MNU_RESET_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_RESET" ) ),
                           tr( "MNU_RESET_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_RESET_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onResetView()));
	myActionsMap[ ResetId ] = aAction;

  // Reset
  aAction = new QtxAction(tr("MNU_CLONE_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_CLONE_VIEW" ) ),
                           tr( "MNU_CLONE_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_CLONE_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onCloneView()));
	myActionsMap[ CloneId ] = aAction;

  myClippingAction = new QtxAction(tr("MNU_CLIPPING"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_CLIPPING" ) ),
                           tr( "MNU_CLIPPING" ), 0, this);
  myClippingAction->setStatusTip(tr("DSC_CLIPPING"));
  myClippingAction->setCheckable( true );
  connect(myClippingAction, SIGNAL(toggled( bool )), this, SLOT(onClipping( bool )));
  myActionsMap[ ClippingId ] = myClippingAction;

  aAction = new QtxAction(tr("MNU_SHOOT_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_SHOOT_VIEW" ) ),
                           tr( "MNU_SHOOT_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_SHOOT_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onMemorizeView()));
	myActionsMap[ MemId ] = aAction;

  aAction = new QtxAction(tr("MNU_PRESETS_VIEW"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_PRESETS_VIEW" ) ),
                           tr( "MNU_PRESETS_VIEW" ), 0, this);
  aAction->setStatusTip(tr("DSC_PRESETS_VIEW"));
  connect(aAction, SIGNAL(activated()), this, SLOT(onRestoreView()));
	myActionsMap[ RestoreId ] = aAction;

  if (myModel->trihedronActivated()) {
    aAction = new QtxAction(tr("MNU_SHOW_TRIHEDRE"), aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_VIEW_TRIHEDRON" ) ),
                             tr( "MNU_SHOW_TRIHEDRE" ), 0, this);
    aAction->setStatusTip(tr("DSC_SHOW_TRIHEDRE"));
    connect(aAction, SIGNAL(activated()), this, SLOT(onTrihedronShow()));
	  myActionsMap[ TrihedronShowId ] = aAction;
  }
}

/*!
  \brief Create toolbar.
*/
void OCCViewer_ViewWindow::createToolBar()
{
  myToolBar->addAction( myActionsMap[DumpId] );
  if ( myModel->trihedronActivated() ) 
    myToolBar->addAction( myActionsMap[TrihedronShowId] );

  QtxMultiAction* aScaleAction = new QtxMultiAction( this );
  aScaleAction->insertAction( myActionsMap[FitAllId] );
  aScaleAction->insertAction( myActionsMap[FitRectId] );
  aScaleAction->insertAction( myActionsMap[ZoomId] );
  myToolBar->addAction( aScaleAction );

  QtxMultiAction* aPanningAction = new QtxMultiAction( this );
  aPanningAction->insertAction( myActionsMap[PanId] );
  aPanningAction->insertAction( myActionsMap[GlobalPanId] );
  myToolBar->addAction( aPanningAction );

  myToolBar->addAction( myActionsMap[ChangeRotationPointId] );

  myToolBar->addAction( myActionsMap[RotationId] );

  QtxMultiAction* aViewsAction = new QtxMultiAction( this );
  aViewsAction->insertAction( myActionsMap[FrontId] );
  aViewsAction->insertAction( myActionsMap[BackId] );
  aViewsAction->insertAction( myActionsMap[TopId] );
  aViewsAction->insertAction( myActionsMap[BottomId] );
  aViewsAction->insertAction( myActionsMap[LeftId] );
  aViewsAction->insertAction( myActionsMap[RightId] );
  myToolBar->addAction( aViewsAction );

  myToolBar->addAction( myActionsMap[ResetId] );

  QtxMultiAction* aMemAction = new QtxMultiAction( this );
  aMemAction->insertAction( myActionsMap[MemId] );
  aMemAction->insertAction( myActionsMap[RestoreId] );
  myToolBar->addAction( aMemAction );

  myToolBar->addSeparator();
  myToolBar->addAction( myActionsMap[CloneId] );
  
  myToolBar->addSeparator();
  myToolBar->addAction( myActionsMap[ClippingId] );
}

/*!
  \brief Perform 'fit all' operation.
*/
void OCCViewer_ViewWindow::onViewFitAll()
{
  myViewPort->fitAll();
}

/*!
  \brief Perform "front view" transformation.
*/
void OCCViewer_ViewWindow::onFrontView()
{
  emit vpTransformationStarted ( FRONTVIEW );
  Handle(V3d_View) aView3d = myViewPort->getView();
  if ( !aView3d.IsNull() ) aView3d->SetProj (V3d_Xpos);
  onViewFitAll();
}

/*!
  \brief Perform "back view" transformation.
*/
void OCCViewer_ViewWindow::onBackView()
{
  emit vpTransformationStarted ( BACKVIEW );
  Handle(V3d_View) aView3d = myViewPort->getView();
  if ( !aView3d.IsNull() ) aView3d->SetProj (V3d_Xneg);
  onViewFitAll();
}

/*!
  \brief Perform "top view" transformation.
*/
void OCCViewer_ViewWindow::onTopView()
{
  emit vpTransformationStarted ( TOPVIEW );
  Handle(V3d_View) aView3d = myViewPort->getView();
  if ( !aView3d.IsNull() ) aView3d->SetProj (V3d_Zpos);
  onViewFitAll();
}

/*!
  \brief Perform "bottom view" transformation.
*/
void OCCViewer_ViewWindow::onBottomView()
{
  emit vpTransformationStarted ( BOTTOMVIEW );
  Handle(V3d_View) aView3d = myViewPort->getView();
  if ( !aView3d.IsNull() ) aView3d->SetProj (V3d_Zneg);
  onViewFitAll();
}

/*!
  \brief Perform "left view" transformation.
*/
void OCCViewer_ViewWindow::onLeftView()
{
  emit vpTransformationStarted ( LEFTVIEW );
  Handle(V3d_View) aView3d = myViewPort->getView();
  if ( !aView3d.IsNull() ) aView3d->SetProj (V3d_Yneg);
  onViewFitAll();
}

/*!
  \brief Perform "right view" transformation.
*/
void OCCViewer_ViewWindow::onRightView()
{
  emit vpTransformationStarted ( RIGHTVIEW );
  Handle(V3d_View) aView3d = myViewPort->getView();
  if ( !aView3d.IsNull() ) aView3d->SetProj (V3d_Ypos);
  onViewFitAll();
}

/*!
  \brief Perform "reset view" transformation.

  Sets default orientation of the viewport camera.
*/
void OCCViewer_ViewWindow::onResetView()
{
  emit vpTransformationStarted( RESETVIEW );
  bool upd = myViewPort->getView()->SetImmediateUpdate( false );
  myViewPort->getView()->Reset( false );
  myViewPort->fitAll( false, true, false );
  myViewPort->getView()->SetImmediateUpdate( upd );
  myViewPort->getView()->Update();
}

/*!
  \brief Perform "fit all" transformation.
*/
void OCCViewer_ViewWindow::onFitAll()
{
  emit vpTransformationStarted( FITALLVIEW );
  myViewPort->fitAll();
}

/*!
  \brief Called if 'change rotation point' operation is activated.
  \param on action state
*/
void OCCViewer_ViewWindow::onSetRotationPoint( bool on )
{
  if ( on )
    {
      if ( !mySetRotationPointDlg )
	{
	  mySetRotationPointDlg = new OCCViewer_SetRotationPointDlg( this, myDesktop );
	  mySetRotationPointDlg->SetAction( mySetRotationPointAction );
	}

      if ( !mySetRotationPointDlg->isVisible() )
      {
	if ( mySetRotationPointDlg->IsFirstShown() )
	{ 
	  Standard_Real Xcenter, Ycenter, Zcenter;
	  if ( computeGravityCenter( Xcenter, Ycenter, Zcenter ) )
	    mySetRotationPointDlg->setCoords( Xcenter, Ycenter, Zcenter );
	}
	mySetRotationPointDlg->show();
      }
    }
  else
    {
      if ( mySetRotationPointDlg->isVisible() )
	mySetRotationPointDlg->hide();
    }
}

/*!
   \brief Create one more window with same content.
*/
void OCCViewer_ViewWindow::onCloneView()
{
  SUIT_ViewWindow* vw = myManager->createViewWindow();
  vw->show();
}

/*!
  \brief called if clipping operation is activated.

  Enables/disables clipping plane displaying.

  \parma on action state
*/
void OCCViewer_ViewWindow::onClipping( bool on )
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  if ( on )
    myActionsMap[ ClippingId ]->setIcon(aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_CLIPPING_PRESSED" )));
  else
    myActionsMap[ ClippingId ]->setIcon(aResMgr->loadPixmap( "OCCViewer", tr( "ICON_OCCVIEWER_CLIPPING" )));
  
  if ( on )
    {
      if ( !myClippingDlg )
	{
	  myClippingDlg = new OCCViewer_ClippingDlg( this, myDesktop );
	  myClippingDlg->SetAction( myClippingAction );
	}

      if ( !myClippingDlg->isVisible() )
	myClippingDlg->show();
    }
  else
    {
      if ( myClippingDlg->isVisible() )
	myClippingDlg->hide();
      setCuttingPlane(false);
    }
}

/*!
  \brief Store view parameters.
*/
void OCCViewer_ViewWindow::onMemorizeView()
{
  myModel->appendViewAspect( getViewParams() );
}

/*!
  \brief Restore view parameters.
*/
void OCCViewer_ViewWindow::onRestoreView()
{
  OCCViewer_CreateRestoreViewDlg* aDlg = new OCCViewer_CreateRestoreViewDlg( centralWidget(), myModel );
  connect( aDlg, SIGNAL( dlgOk() ), this, SLOT( setRestoreFlag() ) );
  aDlg->exec();
  myModel->updateViewAspects( aDlg->parameters() );
  if( myRestoreFlag && aDlg->parameters().count() )
    performRestoring( aDlg->currentItem() );
}

/*!
  \brief Restore view parameters.
  \param anItem view parameters
*/
void OCCViewer_ViewWindow::performRestoring( const viewAspect& anItem )
{
  Handle(V3d_View) aView3d = myViewPort->getView();
	
  Standard_Boolean prev = aView3d->SetImmediateUpdate( Standard_False );
  aView3d->SetScale( anItem.scale );
  aView3d->SetCenter( anItem.centerX, anItem.centerY );
  aView3d->SetTwist( anItem.twist );
  aView3d->SetAt( anItem.atX, anItem.atY, anItem.atZ );
  aView3d->SetImmediateUpdate( prev );
  aView3d->SetEye( anItem.eyeX, anItem.eyeY, anItem.eyeZ );
  aView3d->SetProj( anItem.projX, anItem.projY, anItem.projZ );
  
  myRestoreFlag = 0;
}

/*!
  \brief Set restore flag.
*/
void OCCViewer_ViewWindow::setRestoreFlag()
{
  myRestoreFlag = 1;
}

/*!
  \brief Called when action "show/hide trihedron" is activated.
*/
void OCCViewer_ViewWindow::onTrihedronShow()
{
  myModel->toggleTrihedron();
}

/*!
  \brief Dump view window contents to the pixmap.
  \return pixmap containing all scene rendered in the window
*/
QImage OCCViewer_ViewWindow::dumpView()
{
  QPixmap px = QPixmap::grabWindow( myViewPort->winId() );
  return px.toImage();
}

/*!
  \brief Set parameters of the cutting plane
  \param on if \c true, cutting plane is enabled
  \param x X position of plane point 
  \param y Y position of plane point 
  \param z Z position of plane point 
  \param dx X coordinate of plane normal
  \param dy Y coordinate of plane normal
  \param dz Z coordinate of plane normal
*/
void OCCViewer_ViewWindow::setCuttingPlane( bool on, const double x,  const double y,  const double z,
					    const double dx, const double dy, const double dz )
{
  Handle(V3d_View) view = myViewPort->getView();
  if ( view.IsNull() )
    return;

  if ( on ) {
    Handle(V3d_Viewer) viewer = myViewPort->getViewer();
    
    // try to use already existing plane or create a new one
    Handle(V3d_Plane) clipPlane;
    view->InitActivePlanes();
    if ( view->MoreActivePlanes() )
      clipPlane = view->ActivePlane();
    else
      clipPlane = new V3d_Plane( viewer );
    
    // set new a,b,c,d values for the plane
    gp_Pln pln( gp_Pnt( x, y, z ), gp_Dir( dx, dy, dz ) );
    double a, b, c, d;
    pln.Coefficients( a, b, c, d );
    clipPlane->SetPlane( a, b, c, d );
    
    view->SetPlaneOn( clipPlane );
  } 
  else
    view->SetPlaneOff();
  
  view->Update();
  view->Redraw();
}

/*!
  \brief Check if any cutting plane is enabled
  \return \c true if at least one cutting plane is enabled
*/
bool OCCViewer_ViewWindow::isCuttingPlane()
{
  Handle(V3d_View) view = myViewPort->getView();
  view->InitActivePlanes();
  return (view->MoreActivePlanes());
}

/*!
  \brief Get the visual parameters of the view window.
  \return visual parameters of view window
*/
viewAspect OCCViewer_ViewWindow::getViewParams() const
{
  double centerX, centerY, projX, projY, projZ, twist;
  double atX, atY, atZ, eyeX, eyeY, eyeZ;

  Handle(V3d_View) aView3d = myViewPort->getView();

  aView3d->Center( centerX, centerY );
  aView3d->Proj( projX, projY, projZ );
  aView3d->At( atX, atY, atZ );
  aView3d->Eye( eyeX, eyeY, eyeZ );
  twist = aView3d->Twist();

  QString aName = QTime::currentTime().toString() + QString::fromLatin1( " h:m:s" );

  viewAspect params;
  params.scale    = aView3d->Scale();
  params.centerX  = centerX;
  params.centerY  = centerY;
  params.projX    = projX;
  params.projY    = projY;
  params.projZ    = projZ;
  params.twist    = twist;
  params.atX      = atX;
  params.atY      = atY;
  params.atZ      = atZ;
  params.eyeX     = eyeX;
  params.eyeY     = eyeY;
  params.eyeZ     = eyeZ;
  params.name	  = aName;

  return params;
}


/*!
  \brief Get visual parameters of this view window.
  \return visual parameters of view window
*/
QString OCCViewer_ViewWindow::getVisualParameters()
{
  viewAspect params = getViewParams();
  QString retStr;
  retStr.sprintf( "%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e*%.12e", params.scale,
		  params.centerX, params.centerY, params.projX, params.projY, params.projZ, params.twist,
		  params.atX, params.atY, params.atZ, params.eyeX, params.eyeY, params.eyeZ );
  return retStr;
}

/*!
  \brief Restore visual parameters of the view window.
  \param parameters visual parameters of view window
*/
void OCCViewer_ViewWindow::setVisualParameters( const QString& parameters )
{
  QStringList paramsLst = parameters.split( '*' );
  if ( paramsLst.size() == 13 ) {
    viewAspect params;
    params.scale    = paramsLst[0].toDouble();
    params.centerX  = paramsLst[1].toDouble();
    params.centerY  = paramsLst[2].toDouble();
    params.projX    = paramsLst[3].toDouble();
    params.projY    = paramsLst[4].toDouble();
    params.projZ    = paramsLst[5].toDouble();
    params.twist    = paramsLst[6].toDouble();
    params.atX      = paramsLst[7].toDouble();
    params.atY      = paramsLst[8].toDouble();
    params.atZ      = paramsLst[9].toDouble();
    params.eyeX     = paramsLst[10].toDouble();
    params.eyeY     = paramsLst[11].toDouble();
    params.eyeZ     = paramsLst[12].toDouble();

    performRestoring( params );
  }
}

/*!
  \brief Handle show event.
 
  Emits Show() signal.
  
  \param theEvent show event
*/
void OCCViewer_ViewWindow::showEvent( QShowEvent* theEvent ) 
{
  emit Show( theEvent );
}

/*!
  \brief Handle hide event.
 
  Emits Hide() signal.
  
  \param theEvent hide event
*/
void OCCViewer_ViewWindow::hideEvent( QHideEvent* theEvent ) 
{
  emit Hide( theEvent );
}

