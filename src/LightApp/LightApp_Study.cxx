#include "LightApp_Study.h"

#include "CAM_DataModel.h"
#include "LightApp_Application.h"
#include "LightApp_DataModel.h"
#include "LightApp_RootObject.h"

#include <OB_Browser.h>

#include <qstring.h>

/*!
  Constructor.
*/
LightApp_Study::LightApp_Study( SUIT_Application* app )
: CAM_Study( app )
{
}
 
/*!
  Destructor.
*/
LightApp_Study::~LightApp_Study()
{
}

/*!
  Create document.
*/
void LightApp_Study::createDocument()
{
  // create myRoot
  setRoot( new LightApp_RootObject( this ) );

  CAM_Study::createDocument();

  emit created( this );
}

//=======================================================================
// name    : openDocument
/*! Purpose : Open document*/
//=======================================================================
bool LightApp_Study::openDocument( const QString& theFileName )
{
  setRoot( new LightApp_RootObject( this ) ); // create myRoot

  // update loaded data models: call open() and update() on them.
  ModelList dm_s;
  dataModels( dm_s );
  for ( ModelListIterator it( dm_s ); it.current(); ++it )
    openDataModel( studyName(), it.current() );

  // this will build a SUIT_DataObject-s tree under myRoot member field
  // passing "false" in order NOT to rebuild existing data models' trees - it was done in previous step
  // but tree that corresponds to not-loaded data models will be updated any way. 
  ((LightApp_Application*)application())->updateObjectBrowser( false ); 

  bool res = CAM_Study::openDocument( theFileName );

  emit opened( this );

  return res;
}

//=======================================================================
// name    : saveDocumentAs
/*! Purpose : Save document */
//=======================================================================
bool LightApp_Study::saveDocumentAs( const QString& theFileName )
{
  ModelList list; dataModels( list );

  LightApp_DataModel* aModel = (LightApp_DataModel*)list.first();
  for ( ; aModel; aModel = (LightApp_DataModel*)list.next() )
    aModel->saveAs( theFileName, this );

  bool res = CAM_Study::saveDocumentAs( theFileName );//SRN: BugID IPAL9377, removed usage of uninitialized variable <res>

  if ( res )
    emit saved( this );

  return res;
}

//=======================================================================
// name    : loadDocument
/*! Purpose : Load document */
//=======================================================================
bool LightApp_Study::loadDocument( const QString& theStudyName )
{
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
  ((LightApp_Application*)application())->updateObjectBrowser( false ); 

  bool res = CAM_Study::openDocument( theStudyName );
  emit opened( this );
  //SRN: BugID IPAL9021: End
  return res;
}

//=======================================================================
// name    : saveDocument
/*! Purpose : Save document */
//=======================================================================
void LightApp_Study::saveDocument()
{
  ModelList list; dataModels( list );

  LightApp_DataModel* aModel = (LightApp_DataModel*)list.first();
  for ( ; aModel; aModel = (LightApp_DataModel*)list.next() )
    aModel->save();

  CAM_Study::saveDocument();

  emit saved( this );
}

//================================================================
// Function : closeDocument
/*! Purpose  : Close document */
//================================================================
void LightApp_Study::closeDocument(bool permanently)
{
  // Inform everybody that this study is going to close when it's most safe to,
  // i.e. in the very beginning
  emit closed( this );

  CAM_Study::closeDocument(permanently);
}

//================================================================
// Function : isModified
// Purpose  : 
//================================================================
bool LightApp_Study::isModified() const
{
  bool isAnyChanged = false;
  ModelList list; dataModels( list );

  LightApp_DataModel* aModel = 0;
  for ( QPtrListIterator<CAM_DataModel> it( list ); it.current() && !isAnyChanged; ++it ){
    aModel = dynamic_cast<LightApp_DataModel*>( it.current() );
    if ( aModel )
      isAnyChanged = aModel->isModified();
  }
  return isAnyChanged; 
}

//================================================================
// Function : isSaved
/*! Purpose  : Check: data model is saved?*/
//================================================================
bool LightApp_Study::isSaved() const
{
  bool isAllSaved = false;
  ModelList list; dataModels( list );

  LightApp_DataModel* aModel = 0;
  for ( QPtrListIterator<CAM_DataModel> it( list ); it.current() && isAllSaved; ++it ){
    aModel = dynamic_cast<LightApp_DataModel*>( it.current() );
    if ( aModel )
      isAllSaved = aModel->isSaved();
  }
  return isAllSaved; 
}

//================================================================
// Function : GetListOfFiles
/*! Purpose  : to be used by modules*/
//================================================================
std::vector<std::string> LightApp_Study::GetListOfFiles() const
{
  std::vector<std::string> aListOfFiles;
  return aListOfFiles;
}

//================================================================
// Function : SetListOfFiles
/*! Purpose  : to be used by modules*/
//================================================================
void LightApp_Study::SetListOfFiles (const std::vector<std::string> theListOfFiles)
{
}

//================================================================
// Function : GetTmpDir
/*! Purpose  : to be used by modules*/
//================================================================
std::string LightApp_Study::GetTmpDir (const char* theURL,
                                            const bool  isMultiFile)
{
  std::string aTmpDir = "";
  return aTmpDir;
}

//================================================================
// Function : RemoveTemporaryFiles
/*! Purpose  : to be used by CORBAless modules*/
//================================================================
void LightApp_Study::RemoveTemporaryFiles (const bool isMultiFile) const
{
  if (isMultiFile)
    return;
}
