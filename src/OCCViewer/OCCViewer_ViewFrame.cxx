
#include "OCCViewer_ViewFrame.h"
#include "OCCViewer_ViewWindow.h"
#include "OCCViewer_ViewModel.h"

#include <SUIT_ViewManager.h>

#include <QFrame>
#include <QLayout>
#include <QApplication>

OCCViewer_ViewFrame::OCCViewer_ViewFrame(SUIT_Desktop* theDesktop, OCCViewer_Viewer* theModel)
  : OCCViewer_ViewWindow( theDesktop, theModel ), myPopupRequestedView(0)
{
  QFrame* centralFrame = new QFrame( this );
  setCentralWidget( centralFrame );

   OCCViewer_ViewWindow* view0 = theModel->createSubWindow();
  view0->setParent( centralFrame );
  myViews.append( view0 ); // MAIN_VIEW

  myLayout = new QGridLayout( centralFrame );
  myLayout->setMargin( 0 );
  myLayout->setSpacing( 1 );

  myLayout->addWidget( view0, 1, 1 );
  connectViewSignals(view0);
}

OCCViewer_ViewFrame::~OCCViewer_ViewFrame()
{
}

//**************************************************************************************
OCCViewer_ViewWindow* OCCViewer_ViewFrame::getView( const int i ) const
{
  return ( i < myViews.count() ) ? myViews.at( i ) : 0 ;
}

//**************************************************************************************
void OCCViewer_ViewFrame::setViewManager( SUIT_ViewManager* theMgr )
{
  OCCViewer_ViewWindow::setViewManager(theMgr);
  foreach (OCCViewer_ViewWindow* aView, myViews) {
    aView->setViewManager(theMgr); 
  }
}


//**************************************************************************************
void OCCViewer_ViewFrame::onMaximizedView( OCCViewer_ViewWindow* theView, bool isMaximized)
{
  if (isMaximized) {
    if (myViews.count() <= 1)
      return;

    myLayout->setColumnStretch(0 , 0);
    myLayout->setColumnStretch(1, 0);
   int i = 0;
    OCCViewer_ViewWindow* aView = 0;
    for ( i = BOTTOM_RIGHT; i <= TOP_RIGHT; i++) {
      aView = myViews.at(i);
      if (aView != theView)
        aView->hide();
    }
  } else {

    OCCViewer_Viewer* aModel = dynamic_cast<OCCViewer_Viewer*>(myManager->getViewModel());
    if (!aModel) return;

    myLayout->setColumnStretch(0 , 10);
    myLayout->setColumnStretch(1, 10);

    int i = 0;
    if (myViews.count() == 1) {
      //QColor aColor = myViews.at( MAIN_VIEW )->backgroundColor();
      OCCViewer_ViewWindow* view = 0;
      for ( i = BOTTOM_LEFT; i <= TOP_RIGHT; i++) {
        view = aModel->createSubWindow();
        view->set2dMode( (Mode2dType) i );
        view->setParent( centralWidget() );
        view->setViewManager(myManager); 
        myViews.append( view ); 
        aModel->initView(view);
        view->setMaximized(false, false);
        connectViewSignals(view);
        view->setBackgroundColor(aModel->backgroundColor(i));
     }
      myLayout->addWidget( myViews.at(BOTTOM_LEFT), 1, 0 );
      myLayout->addWidget( myViews.at(TOP_LEFT), 0, 0 );
      myLayout->addWidget( myViews.at(TOP_RIGHT), 0, 1 );
    }
    OCCViewer_ViewWindow* view = 0;
    for ( i = BOTTOM_RIGHT; i <= TOP_RIGHT; i++) {
      view = myViews.at(i);
      view->show();
      QApplication::processEvents();
      if (view != theView)
        view->onViewFitAll();
    }
  }
  myLayout->invalidate();
}

OCCViewer_ViewPort3d* OCCViewer_ViewFrame::getViewPort(int theView) 
{ 
  return getView(theView)? getView(theView)->getViewPort() : 0;
}
  
void OCCViewer_ViewFrame::updateEnabledDrawMode() 
{ 
  foreach (OCCViewer_ViewWindow* aView, myViews) {
    aView->updateEnabledDrawMode(); 
  }
}

void OCCViewer_ViewFrame::setCuttingPlane( bool on, const double x , const double y , const double z,
                                           const double dx, const double dy, const double dz)  
{ 
  foreach (OCCViewer_ViewWindow* aView, myViews) {
    aView->setCuttingPlane(on, x, y, z, dx, dy, dz); 
    aView->update();
  }
}

//**************************************************************************************
void OCCViewer_ViewFrame::setCuttingPlane( bool on, const gp_Pln thePln ) 
{ 
  foreach (OCCViewer_ViewWindow* aView, myViews) {
    aView->setCuttingPlane(on, thePln); 
    aView->update();
  }
}
  
//**************************************************************************************
void OCCViewer_ViewFrame::setInteractionStyle( const int i ) 
{ 
  foreach (OCCViewer_ViewWindow* aView, myViews) {
    aView->setInteractionStyle(i); 
  }
}

  
//**************************************************************************************
void OCCViewer_ViewFrame::connectViewSignals(OCCViewer_ViewWindow* theView)
{
  connect( theView, SIGNAL( maximized( OCCViewer_ViewWindow*, bool ) ), 
           this, SLOT( onMaximizedView( OCCViewer_ViewWindow*, bool ) ) );

  connect( theView, SIGNAL( wheeling(SUIT_ViewWindow*, QWheelEvent*) ), 
           this, SIGNAL( wheeling(SUIT_ViewWindow*, QWheelEvent*) ) );

  connect( theView, SIGNAL( keyReleased(SUIT_ViewWindow*, QKeyEvent*) ), 
           this, SIGNAL( keyReleased(SUIT_ViewWindow*, QKeyEvent*) ) );
  connect( theView, SIGNAL( keyPressed(SUIT_ViewWindow*, QKeyEvent*) ), 
           this, SIGNAL( keyPressed(SUIT_ViewWindow*, QKeyEvent*) ) );

  connect( theView, SIGNAL( mouseDoubleClicked(SUIT_ViewWindow*, QMouseEvent*) ), 
           this, SIGNAL( mouseDoubleClicked(SUIT_ViewWindow*, QMouseEvent*) ) );
  connect( theView, SIGNAL( mousePressed(SUIT_ViewWindow*, QMouseEvent*) ), 
           this, SIGNAL( mousePressed(SUIT_ViewWindow*, QMouseEvent*) ) );
  connect( theView, SIGNAL( mouseReleased(SUIT_ViewWindow*, QMouseEvent*) ), 
           this, SIGNAL( mouseReleased(SUIT_ViewWindow*, QMouseEvent*) ) );
  connect( theView, SIGNAL( mouseMoving(SUIT_ViewWindow*, QMouseEvent*) ), 
           this, SIGNAL( mouseMoving(SUIT_ViewWindow*, QMouseEvent*) ) );

  // The signal is used to process get/set bacgrounf\d color from popup
  connect( theView, SIGNAL( contextMenuRequested(QContextMenuEvent*) ), 
           this, SLOT( onContextMenuRequested(QContextMenuEvent*) ) );

  connect( theView, SIGNAL( contextMenuRequested(QContextMenuEvent*) ), 
           this, SIGNAL( contextMenuRequested(QContextMenuEvent*) ) );

}

void OCCViewer_ViewFrame::setBackgroundColor( const QColor& theColor)
{
  if (myPopupRequestedView)
    myPopupRequestedView->setBackgroundColor(theColor); 
  else
    foreach (OCCViewer_ViewWindow* aView, myViews) {
      if (aView->isVisible())
        aView->setBackgroundColor(theColor); 
    }
}


void OCCViewer_ViewFrame::onViewFitAll()
{
  foreach (OCCViewer_ViewWindow* aView, myViews) {
    aView->onViewFitAll(); 
  }
}

void OCCViewer_ViewFrame::onFitAll()
{
  foreach (OCCViewer_ViewWindow* aView, myViews) {
    aView->onFitAll(); 
  }
}
  
QColor OCCViewer_ViewFrame::backgroundColor() const 
{ 
  if (myPopupRequestedView)
    return myPopupRequestedView->backgroundColor(); 

  foreach (OCCViewer_ViewWindow* aView, myViews) {
    if (aView->isVisible())
      return aView->backgroundColor(); 
  }
  return getView(MAIN_VIEW)->backgroundColor(); 
}

void OCCViewer_ViewFrame::onContextMenuRequested(QContextMenuEvent*)
{
  myPopupRequestedView = dynamic_cast<OCCViewer_ViewWindow*>(sender());
}

