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
#include "SalomeApp_Study.h"

#include "SalomeApp_Module.h"
#include "SalomeApp_DataModel.h"
#include "SalomeApp_DataObject.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Engine_i.hxx"

#include "LightApp_RootObject.h"

#include <OB_Browser.h>

#include <SUIT_ResourceMgr.h>

#include <qptrlist.h>

#include "utilities.h"
#include <iostream.h>
#include <string>
#include <vector>

#include <SUIT_Session.h>

#include "SALOMEDS_Tool.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Exception)

using namespace std;

/*!
 * \brief internal class
*/
class SALOMEAPP_EXPORT ViewerContainer
{
 public:
  ViewerContainer(int savePoint);
  /*! returns a number of viewers*/
  int getNbViewers();
  /*! sets an active view ID*/
  void setActiveViewID(int viewID);
  /*! returns an active view ID*/
  int getActiveViewID();
  /*! returns an ID of the viewer with given number [1:nbViewers]*/
  int getViewerID(int viewerNumber);
  /*! returns a type of the viewer with given ID*/
  std::string getViewerType(int viewerID);
  /*! returns the added viewer ID */
  int addViewer(const QString& type);
  /*! returns a number of views of the viewer with given ID*/
  int getNbViews(int viewerID);
  /*! returns an ID of the view with given number [1:nbViews]*/
  int getViewID(int viewerID, int viewNumber);
  /*! adds a view of the viewer */
  int addView(int viewerID, const QString& caption, const QString& parameters);
  /*! return a caption of the view with given ID*/
  QString getViewCaption(int viewID);
  /*! return parameters of the view with given ID*/
  QString getViewParameters(int viewID);
  

  void addModule(const QString& name);
  void setActiveModule(const QString& name);
  QString getActiveModule();
  std::vector<std::string> getModules();

  void setSavePointName(const QString& name);
  QString getSavePointName();

  void init();

 protected:
  _PTR(AttributeParameter) _ap;
  int _currentViewerID;
  int _currentViewID;
};


/*!
  Constructor.
*/
SalomeApp_Study::SalomeApp_Study( SUIT_Application* app )
: LightApp_Study( app )
{
}  

/*!
  Destructor.
*/
SalomeApp_Study::~SalomeApp_Study()
{
}

/*!
  Gets study id.
*/
int SalomeApp_Study::id() const
{
  int id = -1;
  if ( myStudyDS )
    id = studyDS()->StudyId();
  return id;
}

/*!
  Gets studyDS pointer.
*/
_PTR(Study) SalomeApp_Study::studyDS() const
{
  return myStudyDS;
}

/*!
  Create document.
*/
void SalomeApp_Study::createDocument()
{
  MESSAGE( "openDocument" );

  // initialize myStudyDS, read HDF file
  QString aName = newStudyName();
  _PTR(Study) study ( SalomeApp_Application::studyMgr()->NewStudy( aName.latin1() ) );
  if ( !study )
    return;

  setStudyDS( study );
  setStudyName( aName );

  // create myRoot
  setRoot( new LightApp_RootObject( this ) );

  CAM_Study::createDocument();
  emit created( this );
}

//=======================================================================
// name    : openDocument
/*! Purpose : Open document*/
//=======================================================================
bool SalomeApp_Study::openDocument( const QString& theFileName )
{
  MESSAGE( "openDocument" );

  // initialize myStudyDS, read HDF file
  _PTR(Study) study ( SalomeApp_Application::studyMgr()->Open( (char*) theFileName.latin1() ) );
  if ( !study )
    return false;

  setStudyDS( study );

  setRoot( new LightApp_RootObject( this ) ); // create myRoot

  // update loaded data models: call open() and update() on them.
  ModelList dm_s;
  dataModels( dm_s );
  for ( ModelListIterator it( dm_s ); it.current(); ++it )
    openDataModel( studyName(), it.current() );

  // this will build a SUIT_DataObject-s tree under myRoot member field
  // passing "false" in order NOT to rebuild existing data models' trees - it was done in previous step
  // but tree that corresponds to not-loaded data models will be updated any way. 
  ((SalomeApp_Application*)application())->updateObjectBrowser( false ); 

  bool res = CAM_Study::openDocument( theFileName );
  
  emit opened( this );
  study->IsSaved(true);

  restoreViewers(1);//############### VISUAL PARAMETERS

  return res;
}

//=======================================================================
// name    : loadDocument
/*! Purpose : Connects GUI study to SALOMEDS one already loaded into StudyManager*/
//=======================================================================
bool SalomeApp_Study::loadDocument( const QString& theStudyName )
{
  MESSAGE( "loadDocument" );

  // obtain myStudyDS from StudyManager
  _PTR(Study) study ( SalomeApp_Application::studyMgr()->GetStudyByName( (char*) theStudyName.latin1() ) );
  if ( !study )
    return false;

  setStudyDS( study );

  setRoot( new LightApp_RootObject( this ) ); // create myRoot

  //SRN: BugID IPAL9021, put there the same code as in a method openDocument

  // update loaded data models: call open() and update() on them.
  ModelList dm_s;
  dataModels( dm_s );

  for ( ModelListIterator it( dm_s ); it.current(); ++it )
    openDataModel( studyName(), it.current() );

  // this will build a SUIT_DataObject-s tree under myRoot member field
  // passing "false" in order NOT to rebuild existing data models' trees - it was done in previous step
  // but tree that corresponds to not-loaded data models will be updated any way. 
  ((SalomeApp_Application*)application())->updateObjectBrowser( false ); 

  bool res = CAM_Study::openDocument( theStudyName );
  emit opened( this );

  //SRN: BugID IPAL9021: End

  return res;
}

//=======================================================================
// name    : saveDocumentAs
/*! Purpose : Save document*/
//=======================================================================
bool SalomeApp_Study::saveDocumentAs( const QString& theFileName )
{
  storeViewers(1);//############### VISUAL PARAMETERS

  ModelList list; dataModels( list );

  SalomeApp_DataModel* aModel = (SalomeApp_DataModel*)list.first();
  QStringList listOfFiles;
  for ( ; aModel; aModel = (SalomeApp_DataModel*)list.next() ) {
    listOfFiles.clear();
    aModel->saveAs( theFileName, this, listOfFiles );
    if ( !listOfFiles.isEmpty() )
      saveModuleData(aModel->module()->name(), listOfFiles);
  }

  // save SALOMEDS document
  SUIT_ResourceMgr* resMgr = application()->resourceMgr();
  if( !resMgr )
    return false;

  bool isMultiFile = resMgr->booleanValue( "Study", "multi_file", false ),
       isAscii = resMgr->booleanValue( "Study", "ascii_file", false ),
       res = isAscii ? 
	 SalomeApp_Application::studyMgr()->SaveAsASCII( theFileName.latin1(), studyDS(), isMultiFile ) :
	 SalomeApp_Application::studyMgr()->SaveAs     ( theFileName.latin1(), studyDS(), isMultiFile ) &&
    CAM_Study::saveDocumentAs( theFileName ) && saveStudyData(theFileName);

  if ( res )
    emit saved( this );

  return res;
}

//=======================================================================
// name    : saveDocument
/*! Purpose : Save document*/
//=======================================================================
bool SalomeApp_Study::saveDocument()
{
  storeViewers(1); //############### VISUAL PARAMETERS

  ModelList list; dataModels( list );

  SalomeApp_DataModel* aModel = (SalomeApp_DataModel*)list.first();
  QStringList listOfFiles;
  for ( ; aModel; aModel = (SalomeApp_DataModel*)list.next() ) {
    listOfFiles.clear();
    aModel->save(listOfFiles);
    if ( !listOfFiles.isEmpty() ) 
      saveModuleData(aModel->module()->name(), listOfFiles);
  }

  // save SALOMEDS document
  SUIT_ResourceMgr* resMgr = application()->resourceMgr();
  if( !resMgr )
    return false;

  bool isMultiFile = resMgr->booleanValue( "Study", "multi_file", false ),
       isAscii = resMgr->booleanValue( "Study", "ascii_file", false ),
       res = isAscii ? 
	 SalomeApp_Application::studyMgr()->SaveASCII( studyDS(), isMultiFile ) :
         SalomeApp_Application::studyMgr()->Save     ( studyDS(), isMultiFile ) && CAM_Study::saveDocument();

  res = res && saveStudyData(studyName());
  if ( res )
    emit saved( this );  

  return res;
}

//================================================================
// Function : closeDocument
/*! Purpose  : Close document*/
//================================================================
void SalomeApp_Study::closeDocument(bool permanently)
{
  LightApp_Study::closeDocument(permanently);

  // close SALOMEDS document
  _PTR(Study) studyPtr = studyDS();
  if ( studyPtr )
  {
    if(permanently) SalomeApp_Application::studyMgr()->Close( studyPtr );
    SALOMEDSClient_Study* aStudy = 0;
    setStudyDS( _PTR(Study)(aStudy) );
  }
}

//================================================================
// Function : isModified
// Purpose  : 
//================================================================
bool SalomeApp_Study::isModified() const
{
  bool isAnyChanged = studyDS() && studyDS()->IsModified();
  if (!isAnyChanged)
    isAnyChanged = LightApp_Study::isModified();

  return isAnyChanged; 
}

//================================================================
// Function : isSaved
/*! Purpose  : Check: data model is saved?*/
//================================================================
bool SalomeApp_Study::isSaved() const
{
  bool isAllSaved = studyDS() && studyDS()->GetPersistentReference().size();
  if (!isAllSaved)
    isAllSaved = LightApp_Study::isModified();

  return isAllSaved; 
}

//=======================================================================
// name    : saveModuleData
/*! Purpose : save list file for module 'theModuleName' */
//=======================================================================
void SalomeApp_Study::saveModuleData( QString theModuleName, QStringList theListOfFiles )
{
  int aNb = theListOfFiles.count();
  if ( aNb == 0 )
    return;

  std::vector<std::string> aListOfFiles ( aNb );
  int anIndex = 0;
  for ( QStringList::Iterator it = theListOfFiles.begin(); it != theListOfFiles.end(); ++it ) {
    if ( (*it).isEmpty() )
      continue;
    aListOfFiles[anIndex] = (*it).latin1();
    anIndex++;
  }
  SetListOfFiles(theModuleName, aListOfFiles);
}

//=======================================================================
// name    : openModuleData
/*! Purpose : gets list of file for module 'theModuleNam' */
//=======================================================================
void SalomeApp_Study::openModuleData( QString theModuleName, QStringList& theListOfFiles )
{
  std::vector<std::string> aListOfFiles =  GetListOfFiles( theModuleName );

  int i, aLength = aListOfFiles.size() - 1;
  if ( aLength < 0 )
    return;

  //Get a temporary directory for saved a file
  theListOfFiles.append(aListOfFiles[0].c_str());

  for(i = 0; i < aLength; i++)
    theListOfFiles.append(aListOfFiles[i+1].c_str());
}

//=======================================================================
// name    : saveStudyData
/*! Purpose : save data from study */
//=======================================================================
bool SalomeApp_Study::saveStudyData( const QString& theFileName )
{
  ModelList list; dataModels( list );
  SalomeApp_DataModel* aModel = (SalomeApp_DataModel*)list.first();
  std::vector<std::string> listOfFiles(0);
  for ( ; aModel; aModel = (SalomeApp_DataModel*)list.next() )
    SetListOfFiles(aModel->module()->name(), listOfFiles);
  return true;
}

//=======================================================================
// name    : openStudyData
/*! Purpose : open data for study */
//=======================================================================
bool SalomeApp_Study::openStudyData( const QString& theFileName )
{
 return true;
}

/*!
  Set studyDS.
*/
void SalomeApp_Study::setStudyDS( const _PTR(Study)& s )
{
  myStudyDS = s;
}

/*!
  Insert data model.
*/
void SalomeApp_Study::dataModelInserted (const CAM_DataModel* dm)
{
  MESSAGE("SalomeApp_Study::dataModelInserted() : module name() = " << dm->module()->name());

  CAM_Study::dataModelInserted(dm);

  //  addComponent(dm);
}

/*!
 Create SComponent for module, using default engine (CORBAless)
*/
void SalomeApp_Study::addComponent(const CAM_DataModel* dm)
{
  SalomeApp_Module* aModule = dynamic_cast<SalomeApp_Module*>( dm->module() );
  // 1. aModule == 0 means that this is a light module (no CORBA enigine)
  if (!aModule) {
    // Check SComponent existance
    _PTR(Study) aStudy = studyDS();
    if (!aStudy) 
      return;
    _PTR(SComponent) aComp = aStudy->FindComponent(dm->module()->name());
    if (!aComp) {
      // Create SComponent
      _PTR(StudyBuilder) aBuilder = aStudy->NewBuilder();
      aComp = aBuilder->NewComponent(dm->module()->name());
      aBuilder->SetName(aComp, dm->module()->moduleName().latin1());
      QString anIconName = dm->module()->iconName();
      if (!anIconName.isEmpty()) {
        _PTR(AttributePixMap) anAttr = aBuilder->FindOrCreateAttribute(aComp, "AttributePixMap");
        if (anAttr)
          anAttr->SetPixMap(anIconName.latin1());
      }
      // Set default engine IOR
      aBuilder->DefineComponentInstance(aComp, SalomeApp_Application::defaultEngineIOR().latin1());
      SalomeApp_DataModel::BuildTree( aComp, root(), this, /*skipExisitng=*/true );
    }
  }
}

/*!
  Open data model
*/
bool SalomeApp_Study::openDataModel( const QString& studyName, CAM_DataModel* dm )
{
  if (!dm)
    return false;

  //  SalomeApp_DataModel* aDM = (SalomeApp_DataModel*)(dm);
  SalomeApp_Module* aModule = dynamic_cast<SalomeApp_Module*>( dm->module() );
  _PTR(Study)       aStudy = studyDS(); // shared_ptr cannot be used here
  _PTR(SComponent)  aSComp;
  QString anEngine;
  // 1. aModule == 0 means that this is a light module (no CORBA enigine)
  if (!aModule) {
    anEngine = SalomeApp_Application::defaultEngineIOR();
    aSComp = aStudy->FindComponent(dm->module()->name());
  }
  else {
    SalomeApp_DataModel* aDM = dynamic_cast<SalomeApp_DataModel*>( dm );
    if ( aDM ) {
      QString anId = aDM->getRootEntry( this );
      if ( anId.isEmpty() )
        return true; // Probably nothing to load
      anEngine = aDM->getModule()->engineIOR();
      if ( anEngine.isEmpty() )
        return false;
      aSComp = aStudy->FindComponentID( std::string( anId.latin1() ) );
    }
  }
  if ( aSComp ) {
    _PTR(StudyBuilder) aBuilder( aStudy->NewBuilder() );
    if ( aBuilder ) {
      try {
        aBuilder->LoadWith( aSComp, std::string( anEngine.latin1() ) );
      }
      catch( const SALOME::SALOME_Exception& ) {
        // Oops, something went wrong while loading -> return an error
        return false;
      }
      // Something has been read -> create data model tree
      //SalomeApp_DataModel* aDM = dynamic_cast<SalomeApp_DataModel*>( dm );
      // aDM->buildTree( aSComp, 0, this );
    }
  } else {
    // Don't return false here, for there might be no data
    // for a given component in the study yet
  }
  QStringList listOfFiles;
  openModuleData(dm->module()->name(), listOfFiles);
  if (dm && dm->open(studyName, this, listOfFiles)) {
    // Remove the files and temporary directory, created
    // for this module by LightApp_Engine_i::Load()
    bool isMultiFile = false; // TODO: decide, how to access this parameter
    RemoveTemporaryFiles( dm->module()->name(), isMultiFile );

    // Something has been read -> create data model tree
    LightApp_DataModel* aDM = dynamic_cast<LightApp_DataModel*>( dm );
    if ( aDM )
      aDM->update(NULL, this);
    return true;
  }
  return false;
}

/*!
  Create new study name.
*/
QString SalomeApp_Study::newStudyName() const
{
  std::vector<std::string> studies = SalomeApp_Application::studyMgr()->GetOpenStudies();
  QString prefix( "Study%1" ), newName, curName;
  int i = 1, j, n = studies.size();
  while ( newName.isEmpty() ){
    curName = prefix.arg( i );
    for ( j = 0 ; j < n; j++ ){
      if ( !strcmp( studies[j].c_str(), curName.latin1() ) )
	break;
    }
    if ( j == n )
      newName = curName;
    else
      i++;
  }
  return newName;
}

//================================================================
// Function : GetListOfFiles
/*! Purpose  : to be used by CORBAless modules*/
//================================================================
std::vector<std::string> SalomeApp_Study::GetListOfFiles( const char* theModuleName  ) const
{
  SalomeApp_Engine_i* aDefaultEngine = SalomeApp_Engine_i::GetInstance();
  if (aDefaultEngine)
    return aDefaultEngine->GetListOfFiles(id(), theModuleName);

  std::vector<std::string> aListOfFiles;
  return aListOfFiles;
}

//================================================================
// Function : SetListOfFiles
/*! Purpose  : to be used by CORBAless modules*/
//================================================================
void SalomeApp_Study::SetListOfFiles ( const char* theModuleName,
                                       const std::vector<std::string> theListOfFiles )
{
  SalomeApp_Engine_i* aDefaultEngine = SalomeApp_Engine_i::GetInstance();
  if (aDefaultEngine)
    aDefaultEngine->SetListOfFiles(theListOfFiles, id(), theModuleName);
}

//================================================================
// Function : GetTmpDir
/*! Purpose  : to be used by CORBAless modules*/
//================================================================
std::string SalomeApp_Study::GetTmpDir ( const char* theURL, const bool  isMultiFile )
{
  std::string anURLDir = SALOMEDS_Tool::GetDirFromPath(theURL);
  std::string aTmpDir = isMultiFile ? anURLDir : SALOMEDS_Tool::GetTmpDir();
  return aTmpDir;
}

//================================================================
// Function : RemoveTemporaryFiles
/*! Purpose  : to be used by CORBAless modules*/
//================================================================
void SalomeApp_Study::RemoveTemporaryFiles ( const char* theModuleName, const bool isMultiFile ) const
{
  if (isMultiFile)
    return;

  std::vector<std::string> aListOfFiles = GetListOfFiles( theModuleName );
  if (aListOfFiles.size() > 0) {
    std::string aTmpDir = aListOfFiles[0];

    const int n = aListOfFiles.size() - 1;
    SALOMEDS::ListOfFileNames_var aSeq = new SALOMEDS::ListOfFileNames;
    aSeq->length(n);
    for (int i = 0; i < n; i++)
      aSeq[i] = CORBA::string_dup(aListOfFiles[i + 1].c_str());

    SALOMEDS_Tool::RemoveTemporaryFiles(aTmpDir.c_str(), aSeq.in(), true);
  }
}

// END: methods to be used by CORBAless modules

void SalomeApp_Study::deleteReferencesTo( _PTR( SObject ) obj )
{
  _PTR(StudyBuilder) sb = studyDS()->NewBuilder();
  std::vector<_PTR(SObject)> aRefs = studyDS()->FindDependances( obj );
  for( int i=0, n=aRefs.size(); i<n; i++ )
  {
    _PTR( SObject ) o = aRefs[i];
    if( o->GetFatherComponent()->ComponentDataType()==obj->GetFatherComponent()->ComponentDataType() )
    {
      sb->RemoveReference( o );
      sb->RemoveObjectWithChildren( o );
    }
  }
}

//================================================================
// Function : referencedToEntry
/*! Purpose  : Return referenced entry from entry*/
//================================================================
QString SalomeApp_Study::referencedToEntry( const QString& entry ) const
{
  _PTR(SObject) obj = studyDS()->FindObjectID( entry.latin1() );
  _PTR(SObject) refobj;

  if( obj && obj->ReferencedObject( refobj ) )
    return refobj->GetID().c_str();
  return LightApp_Study::referencedToEntry( entry );
}

//================================================================
// Function : componentDataType
/*! Purpose  : Return component data type from entry*/
//================================================================
QString SalomeApp_Study::componentDataType( const QString& entry ) const
{
  _PTR(SObject) obj( studyDS()->FindObjectID( entry.latin1() ) );
  if ( !obj )
    return LightApp_Study::componentDataType( entry );
  return obj->GetFatherComponent()->ComponentDataType().c_str();
}

//================================================================
// Function : componentDataType
/*! Purpose  : Return component data type from entry*/
//================================================================
bool SalomeApp_Study::isComponent( const QString& entry ) const
{
  _PTR(SObject) obj( studyDS()->FindObjectID( entry.latin1() ) );
  return obj && QString( obj->GetID().c_str() ) == obj->GetFatherComponent()->GetID().c_str();
}

//================================================================
// Function : children
/*! Purpose : Return entries of children of object*/
//================================================================
void SalomeApp_Study::children( const QString& entry, QStringList& child_entries ) const
{
  _PTR(SObject) SO = studyDS()->FindObjectID( entry.latin1() );
  _PTR(ChildIterator) anIter ( studyDS()->NewChildIterator( SO ) );
  anIter->InitEx( true );
  while( anIter->More() )
  {
    _PTR(SObject) val( anIter->Value() );
    child_entries.append( val->GetID().c_str() );
    anIter->Next();
  }
}

void SalomeApp_Study::components( QStringList& comps ) const
{
  for( _PTR(SComponentIterator) it ( studyDS()->NewComponentIterator() ); it->More(); it->Next() ) 
  {
    _PTR(SComponent) aComponent ( it->Value() );
    if( aComponent && aComponent->ComponentDataType() == "Interface Applicative" )
      continue; // skip the magic "Interface Applicative" component
    comps.append( aComponent->ComponentDataType().c_str() );
  }
}

//================================================================
// Function : getNbSavePoints
/*! Purpose : returns a number of saved points
*/
//================================================================
int SalomeApp_Study::getNbSavePoints()
{
  return 1;
}

//================================================================
// Function : getNameOfSavePoint
/*! Purpose : returns a name of save point
*/
//================================================================
QString SalomeApp_Study::getNameOfSavePoint(int savePoint)
{
  ViewerContainer container(savePoint); 
  return container.getSavePointName();
}

//================================================================
// Function : setNameOfSavePoint
/*! Purpose : sets a name of save point
*/
//================================================================
void SalomeApp_Study::setNameOfSavePoint(int savePoint, const QString& nameOfSavePoint)
{
  ViewerContainer container(savePoint); 
  container.setSavePointName(nameOfSavePoint);
}

//================================================================
// Function : storeViewers
/*! Purpose : store the visual parameters of the viewers
*/
//================================================================
void SalomeApp_Study::storeViewers(int savePoint)
{
  SUIT_ViewWindow* activeWindow = application()->desktop()->activeWindow();

  //Remove the previous content of the attribute
  ViewerContainer container(savePoint);  
  container.init();

  SUIT_ViewManager* vm = 0;
  ViewManagerList lst;
  ((SalomeApp_Application*)application())->viewManagers(lst);
  for(QPtrListIterator<SUIT_ViewManager> it(lst); it.current(); ++it) {
    vm = it.current();
    if(vm) {
      int view_count = vm->getViewsCount();
      if(!view_count) continue; //No views is opened in the viewer
      
      int viewerID = container.addViewer(vm->getType());

      QPtrVector<SUIT_ViewWindow> views = vm->getViews();
      for(int i = 0; i<view_count; i++) {
	SUIT_ViewWindow* vw = views[i];
	
	QString visualParameters = vw->getVisualParameters();
	int viewID = container.addView(viewerID, vw->caption(), vw->getVisualParameters());
	
	if(vw == activeWindow) container.setActiveViewID(viewID); //Store the active view ID
      }
    }
  }

  //Sava a name of the active module
  CAM_Module* activeModule = ((SalomeApp_Application*)application())->activeModule();
  QString moduleName = "";
  if(activeModule) {
    moduleName = activeModule->moduleName();
    container.setActiveModule(moduleName);
  }

  //Store visual parameters of the modules
  QPtrList<CAM_Module> list; 
  ((SalomeApp_Application*)application())->modules( list );
  for(SalomeApp_Module* module = (SalomeApp_Module*)list.first(); module; module = (SalomeApp_Module*)list.next()) {
    container.addModule(module->moduleName());
    module->storeVisualParameters(savePoint); 
  }
}

//================================================================
// Function : restoreViewers
/*! Purpose : restore the visual parameters of the viewers
*/
//================================================================
void SalomeApp_Study::restoreViewers(int savePoint)
{
  ViewerContainer container(savePoint);

  //Remove all already existent veiwers and their views
  ViewManagerList lst;
  ((SalomeApp_Application*)application())->viewManagers(lst);
  for(QPtrListIterator<SUIT_ViewManager> it(lst); it.current(); ++it) {
    SUIT_ViewManager* vm = it.current();
    if(vm) ((SalomeApp_Application*)application())->removeViewManager(vm);
  }

  //Restore the viewers
  int nbViewers = container.getNbViewers();
  int activeViewID = container.getActiveViewID();
  SUIT_ViewWindow *viewWin = 0, *activeView = 0;

  for(int i = 1; i <= nbViewers; i++) {
    int viewerID = container.getViewerID(i);
    std::string type = container.getViewerType(viewerID);
    SUIT_ViewManager* vm = ((SalomeApp_Application*)application())->createViewManager(type.c_str());
    if(!vm) continue; //Unknown viewer
    
    
    int nbViews = container.getNbViews(viewerID);
    //Create nbViews-1 view (-1 because 1 view is created by createViewManager)
    for(int i = 1; i< nbViews; i++)  vm->createViewWindow();
    
    int viewCount = vm->getViewsCount();
    if(viewCount != nbViews) {
      cout << "Unknow error, Can't create a view!" << endl;
      continue;
    }

     //Resize the views, set their captions and apply visual parameters.
    QPtrVector<SUIT_ViewWindow> views = vm->getViews();  
    for(int i = 1; i<=viewCount; i++) {
      viewWin = views[i-1];
      if(!viewWin) continue;
      if(application()->desktop()) 
	viewWin->resize( (int)( application()->desktop()->width() * 0.6 ), (int)( application()->desktop()->height() * 0.6 ) );
      int viewID = container.getViewID(viewerID, i);
      viewWin->setCaption(container.getViewCaption(viewID));
      viewWin->setVisualParameters(container.getViewParameters(viewID));
      if(!activeView && viewID == activeViewID) activeView = viewWin;
    }
  }

  //Set focus to an active view window
  if(activeView) {
    activeView->show();
    activeView->raise();
    activeView->setActiveWindow();
    activeView->setFocus();
  }

  vector<string> v = container.getModules();
  for(int i = 0; i<v.size(); i++) {
    ((SalomeApp_Application*)application())->activateModule(v[i].c_str());
    SalomeApp_Module* module = (SalomeApp_Module*)(((SalomeApp_Application*)application())->activeModule());
    module->restoreVisualParameters(savePoint);
  }

  QString activeModuleName = container.getActiveModule();
  if(activeModuleName != "") ((SalomeApp_Application*)application())->activateModule(activeModuleName);
  
}

//================================================================
// Function : getViewerParameters
/*! Purpose : Return an attribute that stores the viewers'
 *            parameters
*/
//================================================================
_PTR(AttributeParameter) SalomeApp_Study::getViewerParameters(int savePoint)
{
  _PTR(StudyBuilder) builder = studyDS()->NewBuilder();
  _PTR(SObject) so = studyDS()->FindComponent("Interface Applicative");
  if(!so) so = builder->NewComponent("Interface Applicative"); 
  return builder->FindOrCreateAttribute(so, "AttributeParameter");
}

/*###############################################################################################*/

#define PT_INTEGER   0
#define PT_REAL      1
#define PT_BOOLEAN   2
#define PT_STRING    3
#define PT_REALARRAY 4
#define PT_INTARRAY  5
#define PT_STRARRAY  6

#define AP_ID_OF_VIEWERS   1 //Int array
#define AP_ID_OF_VIEWS     2 //Int array
#define AP_MODULES         1 //String array
#define AP_ACTIVE_VIEW     1 //INT
#define AP_ACTIVE_MODULE   1 //STRING
#define AP_SAVE_POINT_NAME 2 //STRING

#define START_VIEWER_ID  100
#define START_VIEW_ID    200

ViewerContainer::ViewerContainer(int savePoint)
{
  SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );
  if( !study ) return;
  _ap = study->getViewerParameters(savePoint);
}

int ViewerContainer::getNbViewers()
{
  if(!_ap) return -1;
  if(!_ap->IsSet(AP_ID_OF_VIEWERS, PT_INTARRAY)) return 0;
  return _ap->GetIntArray(AP_ID_OF_VIEWERS).size();
}

void ViewerContainer::setActiveViewID(int viewerID)
{
  if(!_ap) return;
  _ap->SetInt(AP_ACTIVE_VIEW, viewerID);
}

int ViewerContainer::getActiveViewID()
{
  if(!_ap) return -1;
  if(!_ap->IsSet(AP_ACTIVE_VIEW, PT_INTEGER)) return -1;
  return _ap->GetInt(AP_ACTIVE_VIEW);
}

int ViewerContainer::getViewerID(int viewerNumber)
{
  if(!_ap) return -1;
  vector<int> v;
  if(_ap->IsSet(AP_ID_OF_VIEWERS, PT_INTARRAY)) v = _ap->GetIntArray(AP_ID_OF_VIEWERS);
  if(v.size() < viewerNumber) return -1;
  return v[viewerNumber-1];
}

string ViewerContainer::getViewerType(int viewerID)
{
  if(!_ap) return "";
  if(!_ap->IsSet(viewerID, PT_STRING)) return "";
  return _ap->GetString(viewerID);
}

int ViewerContainer::addViewer(const QString& type)
{
  if(!_ap) return -1;
  vector<int> v;
  if(_ap->IsSet(AP_ID_OF_VIEWERS, PT_INTARRAY)) v = _ap->GetIntArray(AP_ID_OF_VIEWERS);
  int viewerID = _currentViewerID;
  v.push_back(viewerID);
  _ap->SetIntArray(AP_ID_OF_VIEWERS, v); //Added a new viewer to the list of viewers
  _ap->SetString(viewerID, type.latin1()); //Viewer type

  //Compute the next viewer ID
  _currentViewerID++;

  return viewerID;
}

int ViewerContainer::getNbViews(int viewerID)
{
  if(!_ap) return -1;
  if(!_ap->IsSet(viewerID, PT_INTARRAY)) return 0;
  return _ap->GetIntArray(viewerID).size();
}

int ViewerContainer::getViewID(int viewerID, int viewNumber)
{
  if(!_ap) return -1;
  //Get a list of view ID's associated with the given viewer
  vector<int> v;
  if(_ap->IsSet(viewerID, PT_INTARRAY)) v = _ap->GetIntArray(viewerID);
  if(v.size() < viewNumber) return -1;
  return v[viewNumber-1];  
}

int ViewerContainer::addView(int viewerID, const QString& caption, const QString& parameters)
{
  if(!_ap) return -1;
  //Get a list of view ID's associated with the given viewer
  vector<int> v;
  if(_ap->IsSet(viewerID, PT_INTARRAY)) v = _ap->GetIntArray(viewerID);
  int viewID = _currentViewID;

  v.push_back(viewID);
  _ap->SetIntArray(viewerID, v); //Add a view to the list of viewer's views

  vector<string> vs;
  vs.push_back(caption.latin1());
  vs.push_back(parameters.latin1());
  _ap->SetStrArray(viewID, vs); //Store view's caption and parameters

  

  //Compute the next view ID
  _currentViewID++;

  return viewID;
}

QString ViewerContainer::getViewCaption(int viewID)
{
  if(!_ap) return "";
  vector<string> vs;
  if(_ap->IsSet(viewID, PT_STRARRAY)) vs = _ap->GetStrArray(viewID);
  if(vs.size() < 2) return "";
  return vs[0];
}

QString ViewerContainer::getViewParameters(int viewID)
{
  if(!_ap) return "";
  vector<string> vs;
  if(_ap->IsSet(viewID, PT_STRARRAY)) vs = _ap->GetStrArray(viewID);
  if(vs.size() < 2) return "";
  return vs[1];
}

void ViewerContainer::addModule(const QString& name)
{
  vector<string> v;
  if(!_ap) return;
  if(_ap->IsSet(AP_MODULES, PT_STRARRAY)) v = _ap->GetStrArray(AP_MODULES);
  v.push_back(name.latin1());
  _ap->SetStrArray(AP_MODULES, v);
}

void ViewerContainer::setActiveModule(const QString& name)
{
  if(!_ap) return;
  _ap->SetString(AP_ACTIVE_MODULE, name.latin1());
}

QString ViewerContainer::getActiveModule()
{
  if(!_ap) return "";
  if(!_ap->IsSet(AP_ACTIVE_MODULE, PT_STRING)) return "";
  return _ap->GetString(AP_ACTIVE_MODULE);
}

vector<string> ViewerContainer::getModules()
{
  vector<string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(AP_MODULES, PT_STRARRAY)) return v;
  return _ap->GetStrArray(AP_MODULES);
}

void ViewerContainer::setSavePointName(const QString& name)
{
  if(!_ap) return;
  _ap->SetString(AP_SAVE_POINT_NAME, name.latin1());
}

QString ViewerContainer::getSavePointName()
{
  if(!_ap) return "";
  if(!_ap->IsSet(AP_SAVE_POINT_NAME, PT_STRING)) return "";
  return _ap->GetString(AP_SAVE_POINT_NAME);
}


void ViewerContainer::init()
{
  _ap->Clear();
  _currentViewerID = START_VIEWER_ID;
  _currentViewID =  START_VIEW_ID; 
}
