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
#include "LightApp_Study.h"

#include "CAM_DataModel.h"
#include "CAM_Module.h"
#include "LightApp_Application.h"
#include "LightApp_DataModel.h"
#include "LightApp_DataObject.h"
#include "LightApp_HDFDriver.h"
#include "LightApp_Module.h"

#include "SUIT_ResourceMgr.h"
#include "SUIT_DataObjectIterator.h"

#include <set>
#include <QString>
#include <QDir>

/*!
  Constructor.
*/
LightApp_Study::LightApp_Study( SUIT_Application* app )
: CAM_Study( app )
{
  // HDF persistence
  myDriver = new LightApp_HDFDriver();
  //myDriver = new LightApp_Driver();
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
bool LightApp_Study::createDocument( const QString& theStr )
{
  setStudyName( QString( "Study%1" ).arg( LightApp_Application::studyId() ) );

  // create myRoot
  setRoot( new LightApp_RootObject( this ) );

  bool aRet = CAM_Study::createDocument( theStr );

  emit created( this );

  return aRet;
}

/*!
  Opens document
*/
bool LightApp_Study::openDocument( const QString& theFileName )
{
  myDriver->ClearDriverContents();
  // create files for models from theFileName

  if ( myRestFolder.isEmpty() )
  {
    if( !openStudyData(theFileName))
      return false;
  }
  else 
  {
    // Case when study is restored from backup
    openBackupData();
  }

  setRoot( new LightApp_RootObject( this ) ); // create myRoot

  // update loaded data models: call open() and update() on them.
  ModelList dm_s;
  dataModels( dm_s );
  QListIterator<CAM_DataModel*> it( dm_s );
  while ( it.hasNext() )
    openDataModel( studyName(), it.next() );
  // this will build a SUIT_DataObject-s tree under myRoot member field
  // passing "false" in order NOT to rebuild existing data models' trees - it was done in previous step
  // but tree that corresponds to not-loaded data models will be updated any way. 
  ((LightApp_Application*)application())->updateObjectBrowser( false ); 

  bool res = CAM_Study::openDocument( theFileName );

  emit opened( this );

  if ( !myRestFolder.isEmpty() )
  {
    setIsSaved( false );
    //myRestFolder = "";
  }

  return res;
}

/*!
  Loads document
*/
bool LightApp_Study::loadDocument( const QString& theStudyName )
{
  myDriver->ClearDriverContents();
  if( !openStudyData(theStudyName))
    return false;

  setRoot( new LightApp_RootObject( this ) ); // create myRoot

  //SRN: BugID IPAL9021, put there the same code as in a method openDocument

  // update loaded data models: call open() and update() on them.
  ModelList dm_s;
  dataModels( dm_s );

  QListIterator<CAM_DataModel*> it( dm_s );
  while ( it.hasNext() )
    openDataModel( studyName(), it.next() );

  // this will build a SUIT_DataObject-s tree under myRoot member field
  // passing "false" in order NOT to rebuild existing data models' trees - it was done in previous step
  // but tree that corresponds to not-loaded data models will be updated any way. 
  ((LightApp_Application*)application())->updateObjectBrowser( false ); 

  bool res = CAM_Study::openDocument( theStudyName );
  emit opened( this );
  //SRN: BugID IPAL9021: End
  return res;
}

/*!
  Saves document
*/
bool LightApp_Study::saveDocumentAs( const QString& theFileName )
{
  SUIT_ResourceMgr* resMgr = application()->resourceMgr();
  if( !resMgr )
    return false;

  ModelList list; 
  dataModels( list );

  QStringList listOfFiles;
  bool isMultiFile = resMgr->booleanValue( "Study", "multi_file", false );
  QListIterator<CAM_DataModel*> itList( list );
  while ( itList.hasNext() )
  {
    LightApp_DataModel* aModel = (LightApp_DataModel*)itList.next();
    if ( !aModel ) continue;

    std::vector<std::string> anOldList = myDriver->GetListOfFiles( aModel->module()->name().toLatin1().constData() );
    listOfFiles.clear();
    aModel->saveAs( theFileName, this, listOfFiles );
    if ( !listOfFiles.isEmpty() )
      saveModuleData(aModel->module()->name(), listOfFiles);

    // Remove files if necessary. File is removed if it was in the list of files before
    // saving and it is not contained in the list after saving. This provides correct 
    // removing previous temporary files. These files are not removed before saving
    // because they may be required for it.

    std::vector<std::string> aNewList = myDriver->GetListOfFiles( aModel->module()->name().toLatin1().constData() );
    
    std::set<std::string> aNewNames;
    std::set<std::string> toRemove;
    int i, n;
    for( i = 0, n = aNewList.size(); i < n; i++ )
      aNewNames.insert( aNewList[ i ] );
    for( i = 0, n = anOldList.size(); i < n; i++ )
    {
      if ( i == 0 ) // directory is always inserted in list
        toRemove.insert( anOldList[ i ] );
      else if ( aNewNames.find( anOldList[ i ] ) == aNewNames.end() )
        toRemove.insert( anOldList[ i ] );
    }
        
    std::vector<std::string> toRemoveList( toRemove.size() );
    std::set<std::string>::iterator anIter;
    for( anIter = toRemove.begin(), i = 0; anIter != toRemove.end(); ++anIter, ++i )
      toRemoveList[ i ] = *anIter;

    
    myDriver->RemoveFiles( toRemoveList, isMultiFile );
  }

  bool res = saveStudyData(theFileName);
  res = res && CAM_Study::saveDocumentAs( theFileName );
  //SRN: BugID IPAL9377, removed usage of uninitialized variable <res>
  if ( res )
    emit saved( this );

  return res;
}

/*!
  Saves document
*/
bool LightApp_Study::saveDocument()
{
  ModelList list; dataModels( list );

  myDriver->ClearDriverContents();
  QStringList listOfFiles;
  QListIterator<CAM_DataModel*> itList( list );
  while ( itList.hasNext() ) {
    LightApp_DataModel* aModel = (LightApp_DataModel*)itList.next();
    if ( !aModel ) continue;

    listOfFiles.clear();
    aModel->save( listOfFiles );
    saveModuleData(aModel->module()->name(), listOfFiles);
  }

  bool res = saveStudyData(studyName());
  res = res && CAM_Study::saveDocument();
  if (res)
    emit saved( this );

  return res;
}

/*!
  Closes document
*/
void LightApp_Study::closeDocument(bool permanently)
{
  // Inform everybody that this study is going to close when it's most safe to,
  // i.e. in the very beginning
  emit closed( this );

  CAM_Study::closeDocument(permanently);
  
  // Remove temporary files
  myDriver->ClearDriverContents();
}

/*!
  \return real entry by entry of reference
  \param entry - entry of reference object
*/
QString LightApp_Study::referencedToEntry( const QString& entry ) const
{
  return entry;
}

/*!
  \return entries of object children
*/
void LightApp_Study::children( const QString&, QStringList& ) const
{
}

/*!
  \return true if entry corresponds to component
*/
bool LightApp_Study::isComponent( const QString& entry ) const
{
  if( !root() )
    return false;

  DataObjectList ch;
  root()->children( ch );
  DataObjectList::const_iterator anIt = ch.begin(), aLast = ch.end();
  for( ; anIt!=aLast; anIt++ )
  {
    LightApp_DataObject* obj = dynamic_cast<LightApp_DataObject*>( *anIt );
    if( obj && obj->entry()==entry )
      return true;
  }
  return false;
}

/*!
  \return component data type for entry
*/
QString LightApp_Study::componentDataType( const QString& entry ) const
{
  LightApp_DataObject* aCurObj;
  for ( SUIT_DataObjectIterator it( root(), SUIT_DataObjectIterator::DepthLeft ); it.current(); ++it ) {
    aCurObj = dynamic_cast<LightApp_DataObject*>( it.current() );
    if ( aCurObj && aCurObj->entry() == entry ) {
      return aCurObj->componentDataType();
    }
  }
  return "";
}

/*!
  \return true if study is modified
*/
bool LightApp_Study::isModified() const
{
  bool isAnyChanged = CAM_Study::isModified();
  ModelList list; dataModels( list );

  LightApp_DataModel* aModel = 0;
  QListIterator<CAM_DataModel*> it( list );
  while ( it.hasNext() && !isAnyChanged ) {
    aModel = dynamic_cast<LightApp_DataModel*>( it.next() );
    if ( aModel )
      isAnyChanged = aModel->isModified();
  }
  return isAnyChanged; 
}

/*!
  \return true if data model is saved
*/
bool LightApp_Study::isSaved() const
{
  return CAM_Study::isSaved();
}

/*!
  Creates SComponent for module, necessary for SalomeApp study
*/
void LightApp_Study::addComponent(const CAM_DataModel* dm)
{
}

/*!
  Saves list file for module 'theModuleName'
*/
void LightApp_Study::saveModuleData(QString theModuleName, QStringList theListOfFiles)
{
  int aNb = theListOfFiles.count();
  if ( aNb == 0 )
    return;

  std::vector<std::string> aListOfFiles ( aNb );
  int anIndex = 0;
  for ( QStringList::Iterator it = theListOfFiles.begin(); it != theListOfFiles.end(); ++it ) {
    if ( (*it).isEmpty() )
      continue;
    aListOfFiles[anIndex] = (*it).toLatin1().constData();
    anIndex++;
  }
  myDriver->SetListOfFiles(theModuleName.toLatin1().constData(), aListOfFiles);
}

/*!
  Gets list of file for module 'theModuleNam'
*/
void LightApp_Study::openModuleData(QString theModuleName, QStringList& theListOfFiles)
{
  std::vector<std::string> aListOfFiles =  myDriver->GetListOfFiles(theModuleName.toLatin1().constData());
  int i, aLength = aListOfFiles.size() - 1;
  if (aLength < 0)
    return;

  //Get a temporary directory for saved a file
  theListOfFiles.append(aListOfFiles[0].c_str());
  for(i = 0; i < aLength; i++)
    theListOfFiles.append(aListOfFiles[i+1].c_str());
}

/*!
  Saves data from study
*/
bool LightApp_Study::saveStudyData( const QString& theFileName )
{
  ModelList list; dataModels( list );
  SUIT_ResourceMgr* resMgr = application()->resourceMgr();
  if( !resMgr )
    return false;
  bool isMultiFile = resMgr->booleanValue( "Study", "multi_file", false );

  bool aRes = myDriver->SaveDatasInFile(theFileName.toLatin1(), isMultiFile);
  return aRes;
}

/*!
  Opens data for study
*/
bool LightApp_Study::openStudyData( const QString& theFileName )
{
  SUIT_ResourceMgr* resMgr = application()->resourceMgr();
  if( !resMgr )
    return false;
  bool isMultiFile = resMgr->booleanValue( "Study", "multi_file", false );

  bool aRes = myDriver->ReadDatasFromFile(theFileName.toLatin1(), isMultiFile);
  return aRes;
}

/*!
  Opens data model
*/
bool LightApp_Study::openDataModel( const QString& studyName, CAM_DataModel* dm )
{
  if (!dm)
    return false;

  QStringList listOfFiles;
  openModuleData(dm->module()->name(), listOfFiles);
  if (dm && dm->open(studyName, this, listOfFiles)) {
    // Something has been read -> create data model tree
    LightApp_DataModel* aDM = dynamic_cast<LightApp_DataModel*>( dm );
    if ( aDM )
      aDM->update(NULL, this);
    return true;
  }
  return false;
}

/*!
  \return temporary directory for saving files of modules
*/
std::string LightApp_Study::GetTmpDir (const char* theURL,
                                       const bool  isMultiFile)
{
  return myDriver->GetTmpDir(theURL, isMultiFile);
}

/*!
  \return list of files necessary for module
  \param theModuleName - name of module
*/
std::vector<std::string> LightApp_Study::GetListOfFiles(const char* theModuleName) const
{
  std::vector<std::string> aListOfFiles;
  aListOfFiles = myDriver->GetListOfFiles(theModuleName);
  return aListOfFiles;
}

/*!
  Sets list of files necessary for module
  \param theModuleName - name of module
  \param theListOfFiles - list of files
*/
void LightApp_Study::SetListOfFiles (const char* theModuleName, const std::vector<std::string> theListOfFiles)
{
  myDriver->SetListOfFiles(theModuleName, theListOfFiles);
}

/*!
  Removes temporary files
*/
void LightApp_Study::RemoveTemporaryFiles (const char* theModuleName, const bool isMultiFile) const
{
  if (isMultiFile)
    return;
  bool isDirDeleted = true;
  myDriver->RemoveTemporaryFiles(theModuleName, isDirDeleted);
}

/*!
  Fills list with components names
  \param comp - list to be filled
*/
void LightApp_Study::components( QStringList& comp ) const
{
  DataObjectList children = root()->children();
  DataObjectList::const_iterator anIt = children.begin(), aLast = children.end();
  for( ; anIt!=aLast; anIt++ )
  {
    LightApp_DataObject* obj = dynamic_cast<LightApp_DataObject*>( *anIt );
    if( obj && obj->entry()!="Interface Applicative" )
      comp.append( obj->entry() );
  }
}

/*!
 * Keeps folder to be used for restoring.
 */
void LightApp_Study::setRestoreFolder( const QString& folder )
{
  myRestFolder = folder;
}

/*!
  * - Call LightApp_DataModel::backup() for each data model
  * - Creates "data" file in backup folder; this file contains name of opened 
  *    HDF-file if study was stored or opened, name of temporary folder used 
  *    by LightApp_Driver for study opening, list of backup files created by each model
 */
void LightApp_Study::backup( const QString& fName )
{
  QList<CAM_DataModel*> list; 
  dataModels( list );
  if ( list.isEmpty() )
    return;

  QString dataName = Qtx::addSlash( fName ) + "data";
  FILE* f = fopen( dataName.toLatin1().constData(), "w" );
  if ( f )
  {
    bool header = false;

    QListIterator<CAM_DataModel*> itList( list );
    while ( itList.hasNext() ) 
    {
      LightApp_DataModel* model = (LightApp_DataModel*)itList.next();
      if ( model && model->getModule() ) 
      {
        if ( !header )
        {
          // writes location of hdf-file
          QString hdfName = QString( "HDF:" ) + studyName() + "\n";

          //QString tmpName = QString( " ) + GetTmpDir( "", false ).c_str() + "\n";
          QString tmpName( "TMP:" ); 
          
          LightApp_Driver::ListOfFiles files = 
            myDriver->GetListOfFiles( model->getModule()->name().toLatin1().constData() );
          if ( files.size() > 0 )
            tmpName += files.front().c_str();
          else 
          {
            // New (not saved study). Use backup folder as default
            tmpName += fName;
          }
          tmpName += '\n';

          fputs( hdfName.toLatin1().constData(), f );
          fputs( tmpName.toLatin1().constData(), f );

          header = true;
        }

        // backup model
        QStringList backupFiles;
        model->backup( fName, backupFiles );

        // module data string
        CAM_Module* mod = model->module();
        {
          QString modStr = mod->name();
          QStringList::iterator it;
          for ( it = backupFiles.begin(); it != backupFiles.end(); ++it )
          {
            const QString& curr = * it;
            modStr += QString( "*" ) + curr;
          }
          modStr += '\n';

          fputs( modStr.toLatin1().constData(), f );
        }
      }
    }

    fclose( f );
    if ( !header )
      remove( dataName.toLatin1().constData() ); // Remove empty created file
  }
}

/*!
 * Prepare files to be used by data models for restoring; these files are 
 * created from backup files and files of previously opened study.
 */
bool LightApp_Study::openBackupData()
{
  // Case when study is restored from backup
  // Here:
  // tmpFolder - folder where temporary files of previously opened study was saved (ex. D:\temp\84623)
  // hdfName � name of HDF-file of previous study, if exists.
  // backupFolder � folder with backup files. 

  // Main idea: 
  // - Parse �data� file and gets file names, Fill driver with files of results tmpFolder.
  // - Move all files from backup to tmp.
  // - Open study using this files.
  // - Redirect name of opened study to previous hdfName.

  // Parse �data�

  // data-file - something like this
  // HDF:D:/sln/CATHARE/Data/1.hdf
  // TMP:d:\temp\84623\
  // CATHAREGUI*1.cbf
  QString dataName = Qtx::addSlash( myRestFolder ) + "data";
  FILE* f = fopen( dataName.toLatin1().constData(), "r" );
  if ( !f )
    return false;

  char buff[ 1024 ];

  // hdf-name
  memset( buff, 0, 1024 );
  fgets( buff, 1024, f );
  QString hdfName( buff );
  hdfName.remove( '\n' );
  hdfName.remove( "HDF:" );

  // name of temporary file
  memset( buff, 0, 1024 );
  fgets( buff, 1024, f );
  QString tmpFolder( buff );
  tmpFolder.remove( '\n' );
  tmpFolder.remove( "TMP:" );

  // list of files 
  QString modName;
  LightApp_Driver::ListOfFiles modFiles;
  while( !feof( f ) )
  {
    modName = "";
    modFiles.clear();

    memset( buff, 0, 1024 );
    fgets( buff, 1024, f );
    QString modStr( buff );
    modStr.remove( '\n' );
    if ( modStr.isEmpty() )
      continue;

    QStringList lst = modStr.split( "*" );
    QStringList::iterator it = lst.begin();
    for ( int i = 0; it != lst.end(); ++it, ++i )
    {
      const QString& curr = *it;
      if ( i == 0 )
      {
        modName = curr;
        modFiles.push_back( Qtx::addSlash( tmpFolder ).toLatin1().constData() );
        continue;
      }
      else 
      modFiles.push_back( curr.toLatin1().constData() );
    }

    // fill driver
    myDriver->SetListOfFiles( modName.toLatin1().constData(), modFiles );
  }

  fclose( f );

  // Move all files from backup to tmp

  if ( !QFileInfo( tmpFolder ).exists() )
    QDir().mkdir( tmpFolder );

  if ( !QFileInfo( tmpFolder ).exists() )
    return false;

  QDir restDir( myRestFolder );
  QStringList restList = restDir.entryList ( QDir::AllEntries );
  QStringList::iterator it;
  for ( it = restList.begin(); it != restList.end(); ++it )
  {
    const QString& locName = *it;
    if ( locName == "." || locName == ".." || locName == "data" )
      continue;

    QString oldName = QDir::convertSeparators( Qtx::addSlash( myRestFolder ) + locName );
    QString newName = QDir::convertSeparators( Qtx::addSlash( tmpFolder ) + locName );
    if ( oldName != newName ) // oldName == newName for non-saved study
    {
      QFile::remove( newName );
      QFile::copy( oldName, newName );
    }
  }
  return true;
}

