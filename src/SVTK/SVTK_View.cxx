#include "SALOME_Actor.h"

#include <qapplication.h>

#include <vtkCallbackCommand.h>
#include <vtkTextProperty.h>
#include <vtkActorCollection.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkGenericRenderWindowInteractor.h>

#include "QtxAction.h"

#include "SUIT_Session.h"
#include "SUIT_ToolButton.h"
#include "SUIT_MessageBox.h"

#include "SUIT_Tools.h"
#include "SUIT_ResourceMgr.h"

#include "VTKViewer_Utilities.h"
#include "VTKViewer_CellRectPicker.h"

#include "SVTK_View.h"
#include "SVTK_ViewModel.h"
#include "SVTK_InteractorStyle.h"

#include "SALOME_ListIteratorOfListIO.hxx"

#include "SVTK_SelectorDef.h"

#include "VTKViewer_Algorithm.h"
#include "SVTK_Functor.h"

//----------------------------------------------------------------------------
SVTK_View
::SVTK_View( QWidget* parent, const char* name ) :
  SVTK_RenderWindowInteractor( parent, name )
{
  myEventCallbackCommand = vtkCallbackCommand::New();
  myEventCallbackCommand->Delete();

  myEventCallbackCommand->SetClientData(this); 
  myPriority = 0.0;

  myEventCallbackCommand->SetCallback(SVTK_View::ProcessEvents);

  SetSelector(SVTK_Selector::New());
  GetSelector()->Delete();

  pushInteractorStyle( SVTK_InteractorStyle::New() );
}

//----------------------------------------------------------------------------
SVTK_View
::~SVTK_View()
{
}

//----------------------------------------------------------------------------
SVTK_Selector* 
SVTK_View
::GetSelector() 
{ 
  return mySelector.GetPointer(); 
}

void
SVTK_View
::SetSelector( SVTK_Selector* theSelector )
{ 
  if(mySelector.GetPointer())
    mySelector->RemoveObserver(myEventCallbackCommand.GetPointer());

  mySelector = theSelector; 

  if(mySelector.GetPointer())
    mySelector->AddObserver(vtkCommand::EndPickEvent, 
			    myEventCallbackCommand.GetPointer(), 
			    myPriority);
}

//----------------------------------------------------------------------------
void 
SVTK_View
::ProcessEvents(vtkObject* vtkNotUsed(theObject), 
		unsigned long theEvent,
		void* theClientData, 
		void* vtkNotUsed(theCallData))
{
  SVTK_View* self = reinterpret_cast<SVTK_View*>(theClientData);

  switch(theEvent){
  case vtkCommand::EndPickEvent:
    self->onEmitSelectionChanged();
    break;
  }
}

//----------------------------------------------------------------------------
void
SVTK_View
::activateZoom()
{
  getInteractorStyle()->startZoom();
}

//----------------------------------------------------------------------------
void
SVTK_View
::activatePanning()
{
  getInteractorStyle()->startPan();
}

//----------------------------------------------------------------------------
void
SVTK_View
::activateRotation()
{
  getInteractorStyle()->startRotate();
}

//----------------------------------------------------------------------------
void
SVTK_View
::activateGlobalPanning()
{
  getInteractorStyle()->startGlobalPan();
}

//----------------------------------------------------------------------------
void
SVTK_View
::activateWindowFit()
{
  getInteractorStyle()->startFitArea();
}

//----------------------------------------------------------------
struct THighlightAction{
  bool myIsHighlight;
  Selection_Mode myMode;
  THighlightAction( bool theIsHighlight, Selection_Mode theMode = ActorSelection )
    : myIsHighlight( theIsHighlight ),
      myMode( theMode ) {}
  void operator()( SALOME_Actor* theActor) {
    if(theActor->GetMapper()){
      theActor->highlight( myIsHighlight, myMode );
    }
  }
};

//----------------------------------------------------------------
void
SVTK_View
::onSelectionChanged()
{
  unHighlightAll();

  const SALOME_ListIO& aListIO = mySelector->StoredIObjects();
  SALOME_ListIteratorOfListIO anIter(aListIO);
  for(; anIter.More(); anIter.Next()){
    highlight(anIter.Value(),true,!anIter.More());
  }
}

//----------------------------------------------------------------
void
SVTK_View
::onEmitSelectionChanged()
{
  emit selectionChanged();
}

//----------------------------------------------------------------
void
SVTK_View
::SetSelectionMode(Selection_Mode theMode)
{
  mySelector->SetSelectionMode(theMode);
}

//----------------------------------------------------------------
Selection_Mode
SVTK_View
::SelectionMode() const
{
  return mySelector->SelectionMode();
}

//----------------------------------------------------------------
void 
SVTK_View
::unHighlightAll() 
{
  //cout << "SVTK_View::unHighlightAll" << endl;

  using namespace VTK;
  ForEach<SALOME_Actor>( getRenderer()->GetActors(),
			 THighlightAction( false ) );

  update();
}

//----------------------------------------------------------------
void
SVTK_View
::highlight( const Handle(SALOME_InteractiveObject)& theIO, 
	     bool theIsHighlight, 
	     bool theIsUpdate ) 
{
  using namespace VTK;
  ForEachIf<SALOME_Actor>( getRenderer()->GetActors(),
			   TIsSameIObject<SALOME_Actor>( theIO ),
			   THighlightAction( theIsHighlight, mySelector->SelectionMode() ) );

  update();
}

//----------------------------------------------------------------------------
void
SVTK_View
::InsertActor( VTKViewer_Actor* theActor, bool theMoveInternalActors )
{
  theActor->AddToRender( getRenderer() );

  //theActor->SetTransform(myTransform);
  //if(theMoveInternalActors) 
  //  myRWInteractor->MoveInternalActors();
}

//----------------------------------------------------------------------------
void
SVTK_View
::AddActor( VTKViewer_Actor* theActor, bool theUpdate )
{
  InsertActor(theActor);
  if(theUpdate) 
    Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_View
::RemoveActor( VTKViewer_Actor* theActor, bool theUpdate )
{
  theActor->RemoveFromRender( getRenderer() );

  if(theUpdate) 
    Repaint();
}

//----------------------------------------------------------------------------
void
SVTK_View
::MoveActor( VTKViewer_Actor* theActor)
{
  RemoveActor(theActor);
  InsertActor(theActor,true);
}

//----------------------------------------------------------------------------
void
SVTK_View
::SetSelectionProp(const double& theRed, 
		   const double& theGreen, 
		   const double& theBlue, 
		   const int& theWidth) 
{
  vtkActorCollection* anActors = getRenderer()->GetActors();
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
SVTK_View
::SetSelectionTolerance(const double& theTolNodes, 
			const double& theTolItems)
{
  vtkActorCollection* anActors = getRenderer()->GetActors();
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

//----------------------------------------------------------------------------
void
SVTK_View
::initInteractorStyle( SVTK_InteractorStyle* interactorStyle )
{
  getInteractor()->SetInteractorStyle( interactorStyle ); 
  interactorStyle->FindPokedRenderer( 0, 0 );

  interactorStyle->SetSelector( GetSelector() );
  interactorStyle->SetRenderWidget( this );
}

//----------------------------------------------------------------------------
void
SVTK_View
::pushInteractorStyle( SVTK_InteractorStyle* interactorStyle )
{
  myInteractorStyles.push( interactorStyle );
  initInteractorStyle( interactorStyle );
}

//----------------------------------------------------------------------------
void
SVTK_View
::popInteractorStyle()
{
  if ( getInteractorStyle() )
    myInteractorStyles.pop();
  
  if ( getInteractorStyle() ) 
    initInteractorStyle( getInteractorStyle() );
}

//----------------------------------------------------------------------------
SVTK_InteractorStyle* 
SVTK_View
::getInteractorStyle()
{
  return myInteractorStyles.empty() ? 0 : myInteractorStyles.top();
}
