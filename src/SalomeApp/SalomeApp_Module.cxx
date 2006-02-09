// File:      SalomeApp_Module.cxx
// Created:   10/25/2004 11:39:56 AM
// Author:    Sergey LITONIN
// Copyright (C) CEA 2004

#include "SalomeApp_Module.h"
#include "SalomeApp_DataModel.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"

#include "LightApp_Selection.h"
#include "LightApp_Operation.h"
#include "LightApp_Preferences.h"
#include "LightApp_Displayer.h"

#include "CAM_DataModel.h"

#include "OB_Browser.h"

#include <SALOME_ListIO.hxx>
#include <SALOME_ListIteratorOfListIO.hxx>
#include <SALOME_InteractiveObject.hxx>
#include <SALOME_Actor.h>

#include "SALOMEDS_IParameters.hxx"

#include <SUIT_Session.h>
#include <SUIT_ViewModel.h>

#include <SVTK_ViewWindow.h>
#include <SVTK_ViewModel.h>
#include <SVTK_MainWindow.h>
#include <SVTK_RenderWindowInteractor.h>

#include <qstring.h>
#include <qmap.h>

#include <vtkActorCollection.h>
#include <vtkRenderer.h>

/*!Constructor.*/
SalomeApp_Module::SalomeApp_Module( const QString& name )
: LightApp_Module( name )
{
}

/*!Destructor.*/
SalomeApp_Module::~SalomeApp_Module()
{
}

/*!Gets application.*/
SalomeApp_Application* SalomeApp_Module::getApp() const
{
  return (SalomeApp_Application*)application();
}

/*!Create new instance of data model and return it.*/
CAM_DataModel* SalomeApp_Module::createDataModel()
{
  return new SalomeApp_DataModel(this);
}

/*!Create and return instance of LightApp_Selection.*/
LightApp_Selection* SalomeApp_Module::createSelection() const
{
  return LightApp_Module::createSelection();
}

void SalomeApp_Module::extractContainers( const SALOME_ListIO& source, SALOME_ListIO& dest ) const
{
  SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );
  if( !study )
  {
    dest = source;
    return;
  }

  SALOME_ListIteratorOfListIO anIt( source );
  for( ; anIt.More(); anIt.Next() )
  {
    Handle( SALOME_InteractiveObject ) obj = anIt.Value();
    if( obj->hasEntry() )
    {
      _PTR(SObject) SO = study->studyDS()->FindObjectID( obj->getEntry() );
      if( SO && QString( SO->GetID().c_str() ) == SO->GetFatherComponent()->GetID().c_str() )
      { //component is selected
	_PTR(SComponent) SC( SO->GetFatherComponent() );
	_PTR(ChildIterator) anIter ( study->studyDS()->NewChildIterator( SC ) );
	anIter->InitEx( true );
	while( anIter->More() )
	{
	  _PTR(SObject) valSO ( anIter->Value() );
	  _PTR(SObject) refSO;
	  if( !valSO->ReferencedObject( refSO ) )
	  {
	    QString id = valSO->GetID().c_str(),
	            comp = SC->ComponentDataType().c_str(),
		    val = valSO->GetName().c_str();

	    Handle( SALOME_InteractiveObject ) new_obj =
	      new SALOME_InteractiveObject( id.latin1(), comp.latin1(), val.latin1() );
	    dest.Append( new_obj );
	  }
	  anIter->Next();
	}
	continue;
      }
    }
    dest.Append( obj );
  }
}

const char gSeparator = '_'; // character used to separate parameter names
const int ViewerType = 0;
const int ViewIndex = 1;
const int ParamName = 2;
// visual parameters are stored in strings as follows:
// ViewerType_ViewNumber_ParamName.  '_' is used as separator and should not be used in
// viewer type or parameter names

// return viewer type substring from parameter name
std::string getParam( const std::string& paramName, const int index )
{
  QStringList lst = QStringList::split( gSeparator, QString( paramName.c_str() ) );
  if ( !lst.isEmpty() && index < lst.size() )
    return lst[index];
  return "";
}

/*!
 * \brief Virtual public
 *
 * This method is called just before the study document is saved, so the module has a possibility
 * to store visual parameters in AttributeParameter attribue(s)
 */
void SalomeApp_Module::storeVisualParameters(int savePoint)
{
  SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );
  if( !study )
    return;
  _PTR(AttributeParameter) ap = study->studyDS()->GetModuleParameters("Interface Applicative", moduleName().latin1(), savePoint);
  SALOMEDS_IParameters ip(ap);

  QPtrList<SUIT_ViewManager> lst;
  getApp()->viewManagers( lst );
  int vtkViewers( 0 );
  for ( QPtrListIterator<SUIT_ViewManager> it( lst ); it.current(); ++it ) {
    SUIT_ViewManager* vman = it.current();
    SUIT_ViewModel* vmodel = vman->getViewModel();

    // saving VTK actors' properties
    if ( vmodel && vmodel->inherits( "SVTK_Viewer" ) ) {
      QPtrVector<SUIT_ViewWindow> views = vman->getViews();
      for ( int i = 0, iEnd = vman->getViewsCount(); i < iEnd; i++ ) {
	SVTK_ViewWindow* vtkView = dynamic_cast<SVTK_ViewWindow*>( views[i] );
	if ( vtkView ) {
	  vtkActorCollection* allActors = vtkView->getRenderer()->GetActors();
	  allActors->InitTraversal();
	  while ( vtkActor* actor = allActors->GetNextActor() ) {
	    if ( actor->GetVisibility() ) { // store only visible actors
	      if ( SALOME_Actor* salomeActor = SALOME_Actor::SafeDownCast( actor ) ) {
		if ( salomeActor->hasIO() ) { // actor corresponds to existing obj
		  
		  Handle(SALOME_InteractiveObject) io = salomeActor->getIO();
		  const char* entry = io->getEntry();
		  const char* aName = salomeActor->getName();
		  float opacity = salomeActor->GetOpacity();
		  float r, g, b;
		  salomeActor->GetColor(r, g, b);
		  int repMode = salomeActor->GetRepresentation();
		  int disMode = salomeActor->getDisplayMode();
		  bool isInf = salomeActor->IsInfinitive();
		  bool isShrunk = ( salomeActor->IsShrunkable() && salomeActor->IsShrunk() );
		  
		  //  printf (" -- actor: entry=%s, name=%s, opacity=%f, color=%d,%d,%d, repMode=%d, disMode=%d... -- \n",
		  //	  entry, aName, opacity, r, g, b, repMode, disMode );
		  
		  QString param, vtkParam = vmodel->getType(); vtkParam += gSeparator; 
		  vtkParam += QString::number( vtkViewers );   vtkParam += gSeparator; 
		  param = vtkParam + "Visibility";     
		  ip.setParameter( entry, param.latin1(), "true" );
		  param = vtkParam + "Name";     
		  ip.setParameter( entry, param.latin1(), aName );
		  param = vtkParam + "Opacity";  
		  ip.setParameter( entry, param.latin1(), QString::number( opacity ).latin1() );
		  param = vtkParam + "ColorRed"; 
		  ip.setParameter( entry, param.latin1(), QString::number( r ).latin1() );
		  param = vtkParam + "ColorGreen";
		  ip.setParameter( entry, param.latin1(), QString::number( g ).latin1() );
		  param = vtkParam + "ColorBlue";
		  ip.setParameter( entry, param.latin1(), QString::number( b ).latin1() );
		  param = vtkParam + "RepresentationMode";
		  ip.setParameter( entry, param.latin1(), QString::number( repMode ).latin1() );
		  param = vtkParam + "DisplayMode";
		  ip.setParameter( entry, param.latin1(), QString::number( disMode ).latin1() );
		  param = vtkParam + "IsInfitine";
		  ip.setParameter( entry, param.latin1(), QString::number( isInf ).latin1() );
		  param = vtkParam + "IsShrunk";
		  ip.setParameter( entry, param.latin1(), QString::number( isShrunk ).latin1() );
		} // hasIO
	      } // salome_actor successfull downcast
	    } // isVisible
	  } // end of ..while.. actors traversal
	} // if ( vtkView )
      } // for ( views ) 
      vtkViewers++;   
    } // if ( SVTK view model )
  }
}

// returns SALOME_Actor with IO with given entry
SALOME_Actor* getActor( std::string entry, SVTK_ViewWindow* vtkView )
{
  if ( vtkView && !entry.empty() ) {
    vtkActorCollection* allActors = vtkView->getRenderer()->GetActors();
    allActors->InitTraversal();
    while ( vtkActor* actor = allActors->GetNextActor() ) {
      if ( SALOME_Actor* salomeActor = SALOME_Actor::SafeDownCast( actor ) ) {
	if ( salomeActor->hasIO() ) { // actor corresponds to existing obj
	  Handle(SALOME_InteractiveObject) io = salomeActor->getIO();
	  if ( entry == io->getEntry() )
	    return salomeActor;
	}
      }
    }
  }
  return 0;
}

/*!
 * \brief Virtual public
 *
 * This method is called after the study document is opened, so the module has a possibility to restore
 * visual parameters
 */
void SalomeApp_Module::restoreVisualParameters(int savePoint)
{
  SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );
  if( !study )
    return;
  _PTR(AttributeParameter) ap = study->studyDS()->GetModuleParameters("Interface Applicative", moduleName().latin1(), savePoint);
  SALOMEDS_IParameters ip(ap);

  QMap<QString, SALOME_Actor*> vtkActors; // actors are stored in a map after displaying of them for quicker access in future
  
  std::vector<std::string> entries = ip.getEntries();
  for ( std::vector<std::string>::iterator entIt = entries.begin(); entIt != entries.end(); ++entIt ) {
    std::vector<std::string> paramNames = ip.getAllParameterNames( *entIt );
    std::vector<std::string> paramValues = ip.getAllParameterValues( *entIt );
    std::vector<std::string>::iterator namesIt = paramNames.begin();
    std::vector<std::string>::iterator valuesIt = paramValues.begin();
    for ( ; namesIt != paramNames.end(); ++namesIt, ++valuesIt ) {
      std::string viewerType = ::getParam( *namesIt, ViewerType );
      std::string viewIndexStr = ::getParam( *namesIt, ViewIndex );
      std::string paramName = ::getParam( *namesIt, ParamName );
      if ( viewerType == SVTK_Viewer::Type().latin1() ) {
	cout << "--SVTK parameter: entry = " << *entIt << ", viewIndex = " << viewIndexStr << ", paramName = " << paramName << endl;
	int viewIndex = QString( viewIndexStr.c_str() ).toUInt();
	QPtrList<SUIT_ViewManager> lst;
	getApp()->viewManagers( SVTK_Viewer::Type(), lst );
	if ( viewIndex < lst.count() ) {
	  // for VTK ViewManager always has 1 ViewWindow, so view index is number of view manager.
	  if ( paramName == "Visibility" && displayer() ) {
	    displayer()->Display( (*entIt).c_str() );

	    SVTK_ViewWindow* vtkView = (SVTK_ViewWindow*) lst.at( viewIndex )->getActiveView();
	    vtkView->getRenderer()->ResetCameraClippingRange();
	    vtkView->Repaint();

	    vtkActors[(*entIt).c_str()] = getActor( *entIt, vtkView ); // store displayed actor
	  }
	} // if view index less then VTK view managers size
      } // if SVTK
    } // for names/parameters iterator
  } // for entries iterator
}

