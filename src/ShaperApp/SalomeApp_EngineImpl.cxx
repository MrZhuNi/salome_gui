// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, CSGROUP
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

#include "SalomeApp_EngineImpl.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"
#include "SUIT_Session.h"
#include "CAM_Module.h"
#include "LightApp_DataModel.h"

#include <SALOMEDS_Tool.hxx>
#include <Utils_SALOME_Exception.hxx>
#include <utilities.h>
#include "SALOMEDSImpl_SComponent.hxx"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QUuid>

#include <iostream>

/*!
  Constructor
*/
SalomeApp_EngineImpl::SalomeApp_EngineImpl( const char* theComponentName )
  : _myKeepFiles( false ), _myComponentName( theComponentName )
{
  MESSAGE("SalomeApp_EngineImpl::SalomeApp_EngineImpl(): myComponentName = " <<
          qPrintable( _myComponentName.c_str() ) << ", this = " << this);
}

/*!
  Destructor
*/
SalomeApp_EngineImpl::~SalomeApp_EngineImpl()
{
  SalomeApp_EngineImplFactory::instance()->removeEngine(_myComponentName);

  MESSAGE("SalomeApp_EngineImpl::~SalomeApp_EngineImpl(): _myComponentName = " <<
          qPrintable( _myComponentName.c_str() ) << ", this = " << this);
}

std::map<std::string, std::string> SalomeApp_EngineImpl::_myComponentIORS =
{{"SHAPER", "1efa48dd-ce57-4e7b-a22d-e562b0563ddf"}};

SalomeApp_EngineImplFactory::~SalomeApp_EngineImplFactory()
{
  for(auto aEnginePair: _myEnginesMap)
  {
    if(aEnginePair.second){
      delete aEnginePair.second;
      aEnginePair.second = nullptr;
    }
  }
}

SALOMEDSImpl_Driver* SalomeApp_EngineImplFactory::GetDriverByType(const std::string& theComponentType)
{
  auto theFatory = instance();
  auto anIt = theFatory->_myEnginesMap.find(theComponentType);
  if(anIt !=  theFatory->_myEnginesMap.end())
  {
    if(anIt->second)
      return dynamic_cast<SALOMEDSImpl_Driver*>(anIt->second);
  }
  return nullptr;
}

void SalomeApp_EngineImplFactory::removeEngine(const std::string& theComponentName)
{
  _myEnginesMap.erase(theComponentName);
}

SALOMEDSImpl_Driver* SalomeApp_EngineImplFactory::GetDriverByIOR(const std::string& theIOR)
{
  for(auto aCompIOR : SalomeApp_EngineImpl::getComponentIORS()){
    if(aCompIOR.second == theIOR){
      return GetDriverByType(aCompIOR.first);
    }
  }
  return  nullptr;
}

SalomeApp_EngineImplFactory* SalomeApp_EngineImplFactory::instance()
{
 static std::unique_ptr<SalomeApp_EngineImplFactory> myFactory(new SalomeApp_EngineImplFactory());
 return myFactory.get();
}

SalomeApp_EngineImpl* SalomeApp_EngineImplFactory::makeEngine(const char* theComponentName, bool toCreate)
{
  SalomeApp_EngineImpl* anEngine = nullptr;
  auto theFactory = SalomeApp_EngineImplFactory::instance();
  auto myItEngine = theFactory->_myEnginesMap.find(theComponentName);
  if(myItEngine != theFactory->_myEnginesMap.end())
    anEngine = myItEngine->second;
  // Activating a new engine
  if ( toCreate && !anEngine ){
    anEngine =  new SalomeApp_EngineImpl(theComponentName);
    theFactory->_myEnginesMap[theComponentName] = anEngine;
  }
  return anEngine;
}


std::string SalomeApp_EngineImpl::GetIOR()
{
  return _myComponentIORS[_myComponentName];
}

SALOMEDSImpl_TMPFile *SalomeApp_EngineImpl::Save(const SALOMEDSImpl_SComponent& theComponent,  const std::string& theURL,
                                                 long& theStreamLength,  bool isMultiFile )
{
  SALOMEDSImpl_Tool::TMPFile *aStreamFile = nullptr;

  if (theComponent.IsNull())
    return nullptr;
  
  // Component type
  std::string componentName (const_cast<SALOMEDSImpl_SComponent*>(&theComponent)->ComponentDataType());

  // Error somewhere outside - Save() called with wrong SComponent instance
  if ( _myComponentName != componentName )
    return nullptr;

  // Get a temporary directory to store a file
  //std::string aTmpDir = isMultiFile ? theURL : SALOMEDS_Tool::GetTmpDir();

  bool manuallySaved = false;

  if ( GetListOfFiles(0).empty() ) // 0 means persistence file
  {

    // Save was probably called from outside GUI, so SetListOfFiles was not called!
    // Try to get list of files from directly from data model

    MESSAGE("SalomeApp_EngineImpl::Save(): _myComponentName = " <<
            qPrintable( _myComponentName.c_str() ) <<
            "it seems Save() was called from outside GUI" );

    // - Get app
    SalomeApp_Application* app =
        dynamic_cast<SalomeApp_Application*>(SUIT_Session::session()->activeApplication());
    if ( !app )
      return nullptr;

    // - Get study
    SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( app->activeStudy() );

    if ( !study )
      return nullptr;
    QString url = QString::fromStdString(study->studyDS()->URL());

    // - Get module
    CAM_Module* module = app->module( SalomeApp_Application::moduleTitle( componentName.c_str() ) );
    if ( !module ) // load module???
      return nullptr;
    // - Get data model
    LightApp_DataModel* dataModel = dynamic_cast<LightApp_DataModel*>( module->dataModel() );
    if ( !dataModel )
      return nullptr;
    // - Save data files
    QStringList dataFiles;
    // we use 'url' instead of 'theURL' as latter normally contains path to the tmp dir,
    // but not actual study's URL
    dataModel->saveAs( url, study, dataFiles );
    std::vector<std::string> names;
    foreach ( QString name, dataFiles ) {
      if ( !name.isEmpty() )
        names.push_back(name.toUtf8().data());
    }
    SetListOfFiles( 0, names ); // 0 means persistence file
    manuallySaved = true;
  }

  // Get a temporary directory to store a file
  //std::string aTmpDir = isMultiFile ? theURL : SALOMEDS_Tool::GetTmpDir();

  // listOfFiles must contain temporary directory name in its first item
  // and names of files (relatively the temporary directory) in the others
  ListOfFiles listOfFiles = GetListOfFiles( 0 ); // 0 means persistence file
  const int n = (int)listOfFiles.size() - 1; //!< TODO: conversion from size_t to int
  
  if (n > 0) { // there are some files, containing persistent data of the component
    std::string aTmpDir = listOfFiles[0];
    
    // Create a list to store names of created files
    ListOfFiles aSeq;
    aSeq.reserve(n);
    for (int i = 0; i < n; i++)
      aSeq.push_back(listOfFiles[i + 1]);
    
    // Convert a file to the byte stream
    aStreamFile = SALOMEDS_Tool::PutFilesToStreamImpl(aTmpDir.c_str(), aSeq, isMultiFile);
    
    // Remove the files and tmp directory, created by the component storage procedure
    SalomeApp_Application* app =
        dynamic_cast<SalomeApp_Application*>( SUIT_Session::session()->activeApplication() );
    SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( app->activeStudy() );
    study->RemoveTemporaryFiles( _myComponentName.c_str(), isMultiFile );
  }
  
  if ( manuallySaved )
    SetListOfFiles(0, ListOfFiles()); // 0 means persistence file

  return dynamic_cast<SALOMEDSImpl_TMPFile*>(aStreamFile);
}

bool SalomeApp_EngineImpl::Load(const SALOMEDSImpl_SComponent & theComponent,  const unsigned char* theStream,
                                const long theStreamLength, const std::string& theURL, bool isMultiFile )
{
  std::cout << "SalomeApp_EngineImpl::Load() isMultiFile = " << isMultiFile << std::endl;
  if (theComponent.IsNull())
    return false;

  // Error somewhere outside - Load() called with
  // wrong SComponent instance
  std::string componentName (const_cast<SALOMEDSImpl_SComponent*>(&theComponent)->ComponentDataType());
  if ( _myComponentName != componentName )
    return false;

  // Create a temporary directory for the component's data files
  std::string aTmpDir = isMultiFile ? theURL : SALOMEDS_Tool::GetTmpDir();

  // Convert the byte stream theStream to a files and place them in the tmp directory.
  // The files and temporary directory must be deleted by the component loading procedure.
  SALOMEDSImpl_Tool::TMPFile theFile(theStreamLength, const_cast<unsigned char *>(theStream), 1);
  ListOfFiles aSeq =
      SALOMEDS_Tool::PutStreamToFilesImpl(theFile, aTmpDir.c_str(), isMultiFile);

  // Store list of file names to be used by the component loading procedure
  const int n = (int)aSeq.size() + 1; //!< TODO: conversion from size_t to int
  ListOfFiles listOfFiles (n);
  listOfFiles[0] = aTmpDir;
  for (int i = 1; i < n; i++)
    listOfFiles[i] = std::string(aSeq[i - 1]);

  SetListOfFiles(0, listOfFiles); // 0 means persistence file
  keepFiles( true );

  return true;
}

SalomeApp_EngineImpl::ListOfFiles SalomeApp_EngineImpl::GetListOfFiles(int type)
{
  return _myListOfFiles.count(type) ? _myListOfFiles[type] : ListOfFiles();
}

void SalomeApp_EngineImpl::SetListOfFiles (int type, const ListOfFiles& theListOfFiles)
{
  _myListOfFiles[type] = theListOfFiles;
}

/*! 
 *  DumpPython implementation for light modules
 */
SALOMEDSImpl_TMPFile*  SalomeApp_EngineImpl::DumpPython(bool isPublished, bool isMultiFile,
                                                        bool& isValidScript, long& theStreamLength)
{
  MESSAGE("SalomeApp_EngineImpl::DumpPython(): _myComponentName = "<<
          qPrintable( _myComponentName.c_str() ) << ", this = " << this);
  
  // Temporary solution: returning a non-empty sequence
  // even if there's nothing to dump, to avoid crashes in SALOMEDS
  // TODO: Improve SALOMEDSImpl_Study::DumpStudy() by skipping the components
  // with isValidScript == false, and initialize isValidScript by false below.
  SALOMEDSImpl_TMPFile* aStreamFile = nullptr;
  SALOMEDSImpl_TMPFile* aStreamFileDefault = new SALOMEDSImpl_Tool::TMPFile(1, (unsigned char *)("\0"), 1);
  isValidScript = true;

  ListOfFiles listOfFiles = GetListOfFiles( 1 ); // 1  means dump file

  // listOfFiles must contain temporary directory name in its first item
  // and names of files (relatively the temporary directory) in the others
  if ( listOfFiles.size() < 2 )
    return aStreamFileDefault;

  // there are some files, containing persistent data of the component
  QString aTmpPath( listOfFiles.front().c_str() );
  QDir aTmpDir( aTmpPath );
  if ( !aTmpDir.exists() )
    return aStreamFileDefault;

  // Calculate file sizes
  QStringList aFilePaths;
  QList<qint64> aFileSizes;
  qint64 aBuffSize = 0;
  ListOfFiles::const_iterator aFIt  = listOfFiles.begin();
  ListOfFiles::const_iterator aFEnd = listOfFiles.end();
  aFIt++;
  for (; aFIt != aFEnd; aFIt++){
    QString aFileName( (*aFIt).c_str() );
    if ( !aTmpDir.exists( aFileName ) ){
      continue;
    }

    QFile aFile( aTmpDir.filePath( aFileName ) );
    if ( !aFile.open( QIODevice::ReadOnly ) ){
      continue;
    }

    aFilePaths.push_back( aTmpDir.filePath( aFileName ) );
    aFileSizes.push_back( aFile.size() );
    aBuffSize += aFileSizes.back();

    aFile.close();
  }

  if ( !aFilePaths.size() || !aBuffSize )
    return aStreamFileDefault;

  char* aBuffer = new char[aBuffSize + 1];
  if ( !aBuffer )
    return aStreamFileDefault;

  // Convert the file(s) to the byte stream, multiple files are simply
  // concatenated
  // TODO: imporve multi-script support if necessary...
  qint64 aCurrPos = 0;
  QStringList::const_iterator aFileIt  = aFilePaths.begin();
  QStringList::const_iterator aFileEnd = aFilePaths.end();
  QList<qint64>::const_iterator   aSIt = aFileSizes.begin();
  for ( ; aFileIt != aFileEnd; aFileIt++, aSIt++ ){
    QFile aFile( aTmpDir.filePath( *aFileIt ) );
    if ( !aFile.open( QIODevice::ReadOnly ) ){
      continue;
    }

    // Incorrect size of file
    // Do not remove the bad file to have some diagnostic means
    if ( aFile.read( aBuffer + aCurrPos, *aSIt ) != *aSIt ){
      aFile.close();
      return aStreamFileDefault;
    }

    aCurrPos += (*aSIt);
    aFile.remove();
  }

  // Here we should end up with empty aTmpDir
  // TODO: Handle QDir::rmdir() error status somehow...
  aTmpDir.rmdir( aTmpPath );

  aBuffer[aBuffSize] = '\0';
  aStreamFile = new SALOMEDSImpl_Tool::TMPFile(aBuffSize + 1, (unsigned char*)(aBuffer), 1);

  return aStreamFile;
}

/*!
  \return Component data type string for this instance of the engine
*/
std::string SalomeApp_EngineImpl::ComponentDataType()
{
  return  _myComponentName;
}

/*!
  \return Component version
*/
std::string SalomeApp_EngineImpl::Version()
{
  SalomeApp_Application::ModuleShortInfoList versions = SalomeApp_Application::getVersionInfo();
  QString version;
  SalomeApp_Application::ModuleShortInfo version_info;
  foreach ( version_info, versions ) {
    if ( SalomeApp_Application::moduleName( version_info.name ) == QString(_myComponentName.c_str()) ) {
      version = version_info.version;
      break;
    }
  }
  
  return std::string(version.toLatin1().constData());
}


/*!
  Internal method, creates an engine for a light SALOME module
  with the given "component data type" string,
  activates it and registers in SALOME naming service with
  /SalomeAppEngine/comp_data_type path. If the engine is already in the
  naming service, simply returns and object reference to it.
  \param theComponentName - synthetic "component data type" used to identify a given light module
  \return Object reference to the engine
*/
SalomeApp_EngineImpl *SalomeApp_EngineImpl::EngineForComponent( const char* theComponentName,
                                                                bool toCreate )
{
  if ( !theComponentName || !strlen( theComponentName ) )
    return nullptr;

  if ( SalomeApp_Application::moduleTitle( theComponentName ).isEmpty() )
    return nullptr;

  return SalomeApp_EngineImplFactory::makeEngine(theComponentName, toCreate);
}

/*!
  \param theComponentName - synthetic "component data type" used to identify a given light module
  \return IOR string for the engine for a light SALOME module
  with the given "component data type" string
  \sa GetInstance( const char* theComponentName )
*/
std::string SalomeApp_EngineImpl::EngineIORForComponent( const char* theComponentName,
                                                         bool toCreate )
{
  std::string anIOR( "" );
  SalomeApp_EngineImpl * anEngine = EngineForComponent( theComponentName, toCreate );
  if (anEngine)
  {
    anIOR = anEngine->GetIOR();
  }
  return anIOR;
}

/*!
  \param theComponentName - synthetic "component data type" used to identify a given light module
  \return A pointer to corresponding C++ engine instance, null means some internal problems.
  \sa EngineIORForComponent( const char* theComponentName )
*/
SalomeApp_EngineImpl* SalomeApp_EngineImpl::GetInstance( const char* theComponentName,
                                                         bool toCreate )
{
  SalomeApp_EngineImpl* anEngine = EngineForComponent( theComponentName, toCreate );
  MESSAGE("SalomeApp_EngineImpl::GetInstance(): theComponentName = " <<
          theComponentName);
  return anEngine;
}
