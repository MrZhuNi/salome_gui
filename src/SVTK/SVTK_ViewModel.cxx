#include <qpopupmenu.h>
#include <qcolordialog.h>

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkActorCollection.h>

#include "SUIT_Session.h"

#include "SVTK_Selection.h"
#include "SVTK_ViewModel.h"
#include "SVTK_ViewWindow.h"
#include "SVTK_View.h"
#include "SVTK_MainWindow.h"
#include "SVTK_Prs.h"

#include "VTKViewer_ViewModel.h"

#include "ToolsGUI.h"

#include "SALOME_Actor.h"
#include "SALOME_InteractiveObject.hxx"

#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOME_ModuleCatalog_impl.hxx"
#include "SALOME_NamingService.hxx"

#include "SALOMEDSClient.hxx"
#include "SALOMEDS_StudyManager.hxx"

// in order NOT TO link with SalomeApp, here the code returns SALOMEDS_Study.
// SalomeApp_Study::studyDS() does it as well, but -- here it is retrieved from 
// SALOMEDS::StudyManager - no linkage with SalomeApp. 

static _PTR(Study) getStudyDS() 
{
  SALOMEDSClient_Study* aStudy = NULL;
  _PTR(StudyManager) aMgr( new SALOMEDS_StudyManager() );

  // get id of SUIT_Study, if it's a SalomeApp_Study, it will return
  //    id of its underlying SALOMEDS::Study
  SUIT_Application* app = SUIT_Session::session()->activeApplication();
  if ( !app )  return _PTR(Study)(aStudy); 
  SUIT_Study* stud = app->activeStudy();
  if ( !stud ) return _PTR(Study)(aStudy);  
  const int id = stud->id(); // virtual method, must return SALOMEDS_Study id
  // get SALOMEDS_Study with this id from StudyMgr
  return aMgr->GetStudyByID( id );
}

//==========================================================
SVTK_Viewer
::SVTK_Viewer()
{
  myTrihedronSize = 105;
}

//==========================================================
SVTK_Viewer
::~SVTK_Viewer() 
{
}

QColor
SVTK_Viewer
::backgroundColor() const
{
  return myBgColor;
}

void
SVTK_Viewer
::setBackgroundColor( const QColor& theColor )
{
  if ( !theColor.isValid() )
    return;

  QPtrVector<SUIT_ViewWindow> aViews = myViewManager->getViews();
  for(int i = 0, iEnd = aViews.size(); i < iEnd; i++){
    if(SUIT_ViewWindow* aViewWindow = aViews.at(i)){
      if(TViewWindow* aView = dynamic_cast<TViewWindow*>(aViewWindow)){
	aView->getMainWindow()->SetBackgroundColor(theColor);
      }
    }
  }

  myBgColor = theColor;
}

//==========================================================
SUIT_ViewWindow* 
SVTK_Viewer::
createView( SUIT_Desktop* theDesktop )
{
  SVTK_ViewWindow* aViewWindow = new SVTK_ViewWindow(theDesktop);
  aViewWindow->Initialize(this);
    
  aViewWindow->setBackgroundColor( backgroundColor() );
  aViewWindow->SetTrihedronSize( trihedronSize() );

  return aViewWindow;
}

int SVTK_Viewer::trihedronSize() const
{
  return myTrihedronSize;
}

void SVTK_Viewer::setTrihedronSize( const int sz )
{
  myTrihedronSize = sz;

  SUIT_ViewManager* vm = getViewManager();
  if ( !vm )
    return;

  QPtrVector<SUIT_ViewWindow> vec = vm->getViews();
  for ( int i = 0; i < vec.count(); i++ )
  {
    SUIT_ViewWindow* win = vec.at( i );
    if ( !win || !win->inherits( "SVTK_ViewWindow" ) )
      continue;

    SVTK_ViewWindow* vw = (SVTK_ViewWindow*)win;
    vw->SetTrihedronSize( sz );
  }
}

//==========================================================
void SVTK_Viewer::setViewManager(SUIT_ViewManager* theViewManager)
{
  SUIT_ViewModel::setViewManager(theViewManager);

  if ( !theViewManager )
    return;

  connect(theViewManager, SIGNAL(mousePress(SUIT_ViewWindow*, QMouseEvent*)), 
          this, SLOT(onMousePress(SUIT_ViewWindow*, QMouseEvent*)));
  
  connect(theViewManager, SIGNAL(mouseMove(SUIT_ViewWindow*, QMouseEvent*)), 
          this, SLOT(onMouseMove(SUIT_ViewWindow*, QMouseEvent*)));
  
  connect(theViewManager, SIGNAL(mouseRelease(SUIT_ViewWindow*, QMouseEvent*)), 
          this, SLOT(onMouseRelease(SUIT_ViewWindow*, QMouseEvent*)));
}

//==========================================================
void
SVTK_Viewer
::contextMenuPopup( QPopupMenu* thePopup )
{
  thePopup->insertItem( VTKViewer_Viewer::tr( "MEN_DUMP_VIEW" ), this, SLOT( onDumpView() ) );
  thePopup->insertItem( VTKViewer_Viewer::tr( "MEN_CHANGE_BACKGROUD" ), this, SLOT( onChangeBgColor() ) );

  thePopup->insertSeparator();

  if(TViewWindow* aView = dynamic_cast<TViewWindow*>(myViewManager->getActiveView())){
    if ( !aView->getView()->GetMainWindow()->getToolBar()->isVisible() ){
      thePopup->insertItem( VTKViewer_Viewer::tr( "MEN_SHOW_TOOLBAR" ), this, SLOT( onShowToolbar() ) );
    }
  }
}

//==========================================================
void 
SVTK_Viewer
::onMousePress(SUIT_ViewWindow* vw, QMouseEvent* event)
{}

//==========================================================
void 
SVTK_Viewer
::onMouseMove(SUIT_ViewWindow* vw, QMouseEvent* event)
{}

//==========================================================
void 
SVTK_Viewer
::onMouseRelease(SUIT_ViewWindow* vw, QMouseEvent* event)
{}

//==========================================================
void 
SVTK_Viewer
::enableSelection(bool isEnabled)
{
  mySelectionEnabled = isEnabled;
  //!! To be done for view windows
}

//==========================================================
void
SVTK_Viewer
::enableMultiselection(bool isEnable)
{
  myMultiSelectionEnabled = isEnable;
  //!! To be done for view windows
}

void
SVTK_Viewer
::onDumpView()
{
  if(SUIT_ViewWindow* aView = myViewManager->getActiveView())
    aView->onDumpView();
}

//==========================================================
void
SVTK_Viewer
::onChangeBgColor()
{
  if(SUIT_ViewWindow* aView = myViewManager->getActiveView()){
    QColor aColor = QColorDialog::getColor( backgroundColor(), aView);
    setBackgroundColor(aColor);
  }
}

//==========================================================
void
SVTK_Viewer
::onShowToolbar() 
{
  QPtrVector<SUIT_ViewWindow> aViews = myViewManager->getViews();
  for(int i = 0, iEnd = aViews.size(); i < iEnd; i++){
    if(TViewWindow* aView = dynamic_cast<TViewWindow*>(aViews.at(i))){
      aView->getView()->GetMainWindow()->getToolBar()->show();
    }
  }
}

//==========================================================
void
SVTK_Viewer
::Display( const SALOME_VTKPrs* prs )
{
  // try do downcast object
  if(const SVTK_Prs* aPrs = dynamic_cast<const SVTK_Prs*>( prs )){
    if(aPrs->IsNull())
      return;
    if(vtkActorCollection* anActorCollection = aPrs->GetObjects()){
      // get SALOMEDS Study
      _PTR(Study) aStudy(getStudyDS());
      anActorCollection->InitTraversal();
      while(vtkActor* anActor = anActorCollection->GetNextActor()){
	if(SALOME_Actor* anAct = SALOME_Actor::SafeDownCast(anActor)){
	  // Set visibility flag
	  Handle(SALOME_InteractiveObject) anObj = anAct->getIO();
	  if(!anObj.IsNull() && anObj->hasEntry() && aStudy){
	    ToolsGUI::SetVisibility(aStudy,anObj->getEntry(),true,this);
	  }
	  // just display the object
	  QPtrVector<SUIT_ViewWindow> aViews = myViewManager->getViews();
	  for(int i = 0, iEnd = aViews.size(); i < iEnd; i++){
	    if(SVTK_ViewWindow* aViewWindow = dynamic_cast<SVTK_ViewWindow*>(aViews.at(i))){
	      if(SVTK_View* aView = aViewWindow->getView()){
		aView->Display(anAct,false);
		if(anAct->IsSetCamera()){
		  vtkRenderer* aRenderer = aView->getRenderer();
		  anAct->SetCamera( aRenderer->GetActiveCamera() );
		}
	      }
	    }
	  }
	}
      }
    }
  }
}

//==========================================================
void
SVTK_Viewer
::Erase( const SALOME_VTKPrs* prs, const bool forced )
{
  // try do downcast object
  if(const SVTK_Prs* aPrs = dynamic_cast<const SVTK_Prs*>( prs )){
    if(aPrs->IsNull())
      return;
    if(vtkActorCollection* anActorCollection = aPrs->GetObjects()){
      // get SALOMEDS Study
      _PTR(Study) aStudy(getStudyDS());
      anActorCollection->InitTraversal();
      while(vtkActor* anActor = anActorCollection->GetNextActor())
	if(SALOME_Actor* anAct = SALOME_Actor::SafeDownCast(anActor)){
	  // Set visibility flag
	  Handle(SALOME_InteractiveObject) anObj = anAct->getIO();
	  if(!anObj.IsNull() && anObj->hasEntry() && aStudy){
	    ToolsGUI::SetVisibility(aStudy,anObj->getEntry(),false,this);
	  }
	  // just display the object
	  QPtrVector<SUIT_ViewWindow> aViews = myViewManager->getViews();
	  for(int i = 0, iEnd = aViews.size(); i < iEnd; i++){
	    if(SVTK_ViewWindow* aViewWindow = dynamic_cast<SVTK_ViewWindow*>(aViews.at(i)))
	      if(SVTK_View* aView = aViewWindow->getView())
		if ( forced )
		  aView->Remove(anAct,false);
		else
		  aView->Erase(anAct,forced);
	  }
	}
    }
  }
}
  
//==========================================================
void
SVTK_Viewer
::EraseAll( const bool forced )
{
  _PTR(Study) aStudy(getStudyDS());
  QPtrVector<SUIT_ViewWindow> aViews = myViewManager->getViews();
  for(int i = 0, iEnd = aViews.size(); i < iEnd; i++){
    if(SVTK_ViewWindow* aViewWindow = dynamic_cast<SVTK_ViewWindow*>(aViews.at(i)))
      if(SVTK_View* aView = aViewWindow->getView()){
	vtkRenderer* aRenderer =  aView->getRenderer();
	vtkActorCollection* anActorCollection = aRenderer->GetActors();
	anActorCollection->InitTraversal();
	while(vtkActor* anActor = anActorCollection->GetNextActor()){
	  if(SALOME_Actor* anAct = SALOME_Actor::SafeDownCast(anActor)){
	    // Set visibility flag
	    Handle(SALOME_InteractiveObject) anObj = anAct->getIO();
	    if(!anObj.IsNull() && anObj->hasEntry() && aStudy)
	      ToolsGUI::SetVisibility(aStudy,anObj->getEntry(),false,this);
	    if(forced)
	      aRenderer->RemoveActor(anAct);
	    else{
	      // just erase actor
	      anAct->SetVisibility( false );
	      // erase dependent actors
	      vtkActorCollection* aCollection = vtkActorCollection::New();
	      anAct->GetChildActors( aCollection );
	      aCollection->InitTraversal();
	      while(vtkActor* aSubAct = aCollection->GetNextActor())
		aSubAct->SetVisibility( false );
	      aCollection->Delete();
	    }
	  }
	}
      }
  }
  Repaint();
}

//==========================================================
SALOME_Prs* 
SVTK_Viewer
::CreatePrs( const char* entry )
{
  SVTK_Prs* prs = new SVTK_Prs();
  if ( entry ) {
    if(SVTK_ViewWindow* aViewWindow = dynamic_cast<SVTK_ViewWindow*>(getViewManager()->getActiveView()))
      if(SVTK_View* aView = aViewWindow->getView()){
	vtkRenderer* aRenderer =  aView->getRenderer();
	vtkActorCollection* theActors = aRenderer->GetActors();
	theActors->InitTraversal();
	vtkActor* ac;
	while( ( ac = theActors->GetNextActor() ) ) {
	  SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	  if ( anActor && anActor->hasIO() && !strcmp( anActor->getIO()->getEntry(), entry ) ) {
	    prs->AddObject( ac );
	  }
	}
      }
  }
  return prs;
}

//==========================================================
void
SVTK_Viewer
::BeforeDisplay( SALOME_Displayer* d )
{
  d->BeforeDisplay( this, SALOME_VTKViewType() );
}

//==========================================================
void
SVTK_Viewer::AfterDisplay( SALOME_Displayer* d )
{
  d->AfterDisplay( this, SALOME_VTKViewType() );
}

//==========================================================
bool
SVTK_Viewer
::isVisible( const Handle(SALOME_InteractiveObject)& io )
{
  QPtrVector<SUIT_ViewWindow> aViews = myViewManager->getViews();
  for(int i = 0, iEnd = aViews.size(); i < iEnd; i++)
    if(SVTK_ViewWindow* aViewWindow = dynamic_cast<SVTK_ViewWindow*>(aViews.at(i)))
      if(SVTK_View* aView = aViewWindow->getView())
	if(!aView->isVisible( io ))
	  return false;

  return true;
}

//==========================================================
void 
SVTK_Viewer
::Repaint()
{
//  if (theUpdateTrihedron) onAdjustTrihedron();
  QPtrVector<SUIT_ViewWindow> aViews = myViewManager->getViews();
  for(int i = 0, iEnd = aViews.size(); i < iEnd; i++)
    if(SVTK_ViewWindow* aViewWindow = dynamic_cast<SVTK_ViewWindow*>(aViews.at(i)))
      if(SVTK_View* aView = aViewWindow->getView())
	aView->Repaint();
}
