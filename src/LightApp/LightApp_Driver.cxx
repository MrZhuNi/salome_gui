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
#include "LightApp_Driver.h"

#ifdef WIN32
#include <time.h>
#else
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#endif

#include <TCollection_AsciiString.hxx> 

#include <OSD_Path.hxx>
#include <OSD_File.hxx>
#include <OSD_Directory.hxx>
#include <OSD_Protection.hxx>
#include <OSD_FileIterator.hxx>

#include <QFileInfo>
#include <QDir>
#include <Qtx.h>
#include <SUIT_Session.h>

/*! Constructor.*/
LightApp_Driver::LightApp_Driver()
: myIsTemp( false ),
  myCurrPos( 0 ),
  myCurrBuff( 0 ),
  myCurrFileIndex( 0 ),
  myCurrIFile( 0 ), 
  mySizeToBeWritten( 0 ),
  myFileSizes( 0 ),
  myFileNameSizes( 0 ),
  myCurrOFile( 0 ),
  myNbFilles( 0 ),
#ifndef WIN32
  myBlocFcntl( 0 ),
#endif
  myBloc( 0 )
{
}
 
/*! Destructor.*/
LightApp_Driver::~LightApp_Driver()
{
  delete myFileSizes;
  delete myFileNameSizes;
}

using namespace std;

/*!
  Save in file 'theFileName' datas from this driver
*/
bool LightApp_Driver::SaveDatasInFile( const char* theFileName, bool isMultiFile )
{
  int aNbModules = 0;
  std::map<std::string, ListOfFiles>::const_iterator it;
  for (it = myMap.begin(); it != myMap.end(); ++it)
    aNbModules++;

  unsigned char** aBuffer = new unsigned char*[aNbModules]; 
  long*           aBufferSize = new long[aNbModules];
  char**          aModuleName = new char*[aNbModules];

  if(aBuffer == NULL || aBufferSize == NULL || aModuleName == NULL)
    return false;

  int aFileBufferSize = 4;  //4 bytes for a number of the modules that will be written to the stream;
  int i = 0;
  for (it = myMap.begin(); it != myMap.end(); ++it) {
    aModuleName[i] = const_cast<char*>(it->first.c_str());//(it->first);
    aFileBufferSize += 4;                                //Add 4 bytes: a length of the module name
    aFileBufferSize += strlen(aModuleName[i])+1;
    std::string aName(aModuleName[i]);
    //PutFilesToStream(aName, aBuffer[i], aBufferSize[i], isMultiFile);
    aFileBufferSize += 8;                                //Add 8 bytes: a length of the buffer
    aFileBufferSize += aBufferSize[i];
    i++;
  }
  int n = i;

  unsigned char* aFileBuffer = new unsigned char[aFileBufferSize];
  if(aFileBuffer == NULL)
    return false;

  myTmpDir = QDir::convertSeparators( QFileInfo( theFileName ).absolutePath() + "/" ).toLatin1().constData() ;

  int aCurrentPos = 0;

  //Initialize 4 bytes of the buffer by 0
  memset(aFileBuffer, 0, 4); 
  //Copy the number of modules that will be written to the stream
  memcpy(aFileBuffer, &aNbModules, ((sizeof(int) > 4) ? 4 : sizeof(int)));
  aCurrentPos += 4;

  int aBufferNameSize = 0;
  for (i = 0; i < n; i++) {
    aBufferNameSize = strlen(aModuleName[i])+1;
    //Initialize 4 bytes of the buffer by 0
    memset((aFileBuffer + aCurrentPos), 0, 4); 
    //Copy the length of the module name to the buffer
    memcpy((aFileBuffer + aCurrentPos), &aBufferNameSize, ((sizeof(int) > 4) ? 4 : sizeof(int))); 
    aCurrentPos += 4;
    //Copy the module name to the buffer
    memcpy((aFileBuffer + aCurrentPos), aModuleName[i], aBufferNameSize);
    aCurrentPos += aBufferNameSize;

    //Initialize 8 bytes of the buffer by 0
    memset((aFileBuffer + aCurrentPos), 0, 8);
    //Copy the length of the module buffer to the buffer
    memcpy((aFileBuffer + aCurrentPos), (aBufferSize + i), ((sizeof(long) > 8) ? 8 : sizeof(long)));
    aCurrentPos += 8;
    //Copy the module buffer to the buffer
    memcpy((aFileBuffer + aCurrentPos), aBuffer[i], aBufferSize[i]);
    aCurrentPos += aBufferSize[i];
  }

#ifdef WIN32
  ofstream aFile(theFileName, ios::out | ios::binary);
#else
  ofstream aFile(theFileName);
#endif
  aFile.write((char*)aFileBuffer, aFileBufferSize); 
  aFile.close();    

  delete[] aBuffer;
  delete[] aBufferSize;
  delete[] aModuleName;
  delete[] aFileBuffer;

  return true;
}

/*!
  Filling current driver from file 'theFileName'
*/
bool LightApp_Driver::ReadDatasFromFile( const char* theFileName, bool isMultiFile )
{
#ifdef WIN32
  ifstream aFile(theFileName, ios::binary);
#else
  ifstream aFile(theFileName);
#endif  

  myTmpDir = QDir::convertSeparators( QFileInfo( theFileName ).absolutePath() + "/" ).toLatin1().constData() ;

  aFile.seekg(0, ios::end);
  int aFileBufferSize = aFile.tellg();
  unsigned char* aFileBuffer = new unsigned char[aFileBufferSize];
  aFile.seekg(0, ios::beg);
  aFile.read((char*)aFileBuffer, aFileBufferSize);
  aFile.close();

  int aNbModules = 0;
  //Copy the number of files in the stream
  memcpy(&aNbModules, aFileBuffer, sizeof(int));
  long aCurrentPos = 4;
  int aModuleNameSize;

  for (int i = 0; i < aNbModules; i++) {
    //Put a length of the module name to aModuleNameSize
    memcpy(&aModuleNameSize, (aFileBuffer + aCurrentPos), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
    aCurrentPos += 4;

    char *aModuleName = new char[aModuleNameSize];
    //Put a module name to aModuleName
    memcpy(aModuleName, (aFileBuffer + aCurrentPos), aModuleNameSize); 
    aCurrentPos += aModuleNameSize;

    //Put a length of the file buffer to aBufferSize
    long aBufferSize;
    memcpy(&aBufferSize, (aFileBuffer + aCurrentPos), ((sizeof(long) > 8) ? 8 : sizeof(long))); 
    aCurrentPos += 8;
    unsigned char *aBuffer = new unsigned char[aBufferSize];
 
    //Put a buffer for current module to aBuffer
    memcpy(aBuffer, (aFileBuffer + aCurrentPos), aBufferSize); 
    aCurrentPos += aBufferSize;

    // Put buffer to aListOfFiles and set to myMap
    //ListOfFiles aListOfFiles = PutStreamToFiles(aBuffer, aBufferSize, isMultiFile);
    ListOfFiles aListOfFiles;
    SetListOfFiles(aModuleName, aListOfFiles);

    delete[] aModuleName;
    delete[] aBuffer;
  }

  delete[] aFileBuffer;
  
  return true;
}

/*!
  \return temp directory for path 'theURL'
*/
std::string LightApp_Driver::GetTmpDir (const char* theURL, const bool  isMultiFile)
{
  std::string anURLDir = GetDirFromPath(theURL);
  std::string aTmpDir = isMultiFile ? anURLDir : GetTmpDir();

  return aTmpDir;
}

/*!
  \return list of files for module with name 'theModuleName'
*/
LightApp_Driver::ListOfFiles LightApp_Driver::GetListOfFiles( const char* theModuleName )
{
  ListOfFiles aListOfFiles;

  std::string aName(theModuleName);
  if (myMap.count(aName))
    aListOfFiles = myMap[aName];

    return aListOfFiles;
}

/*!
  Sets list of files for module with name 'theModuleName'
*/
void LightApp_Driver::SetListOfFiles( const char* theModuleName, const ListOfFiles theListOfFiles )
{
  std::string aName (theModuleName);
  myMap[aName] = theListOfFiles;

  if ( myBloc )
    fclose( myBloc );

  QString fName = Qtx::addSlash( theListOfFiles.front().c_str() ) + "used_by_salome";
  myBloc = fopen( fName.toLatin1().constData(), "w" );

#ifndef WIN32
  fName += ".fcntl";

  if ( myBlocFcntl )
    fclose( myBlocFcntl );
  myBlocFcntl = fopen( fName.toLatin1().constData(), "w" );
  lockFcntl( QString() );
#endif

}

#ifndef WIN32
/*
 * Lock theLF or myBlockFcntl if empty
 * returns 0 on success
 */
int LightApp_Driver::lockFcntl( QString theLF )
{
  if ( theLF.isEmpty() && !myBlocFcntl )
    return -2;

  FILE* aFD;
  if ( theLF.isEmpty() )
    aFD = myBlocFcntl;
  else
    aFD = fopen( theLF.toLatin1().constData(), "w" );
  
  struct flock fLock;
  fLock.l_type = F_WRLCK;
  fLock.l_whence = SEEK_SET;
  fLock.l_len = 0;
  fLock.l_start = 0;
  return fcntl( fileno( aFD ), F_SETLK, &fLock );
}
#endif

/*!
  Remove files. First item in <theFiles> is a directory with slash at the end.
  Other items are names of files. If <IsDirDeleted> is true,
  then the directory is also deleted.
*/
void LightApp_Driver::RemoveFiles( const ListOfFiles& theFiles, const bool IsDirDeleted)
{
  int i, aLength = theFiles.size() - 1;
  if(aLength <= 0) {
    return;
  }
  //Get a temporary directory for saved a file
  TCollection_AsciiString aDirName(const_cast<char*>(theFiles[0].c_str()));

  for(i = 0; i < aLength; i++) {
    TCollection_AsciiString aFile(aDirName);
    aFile += const_cast<char*>(theFiles[i+1].c_str());
    OSD_Path anOSDPath(aFile);
    OSD_File anOSDFile(anOSDPath);
    if(!anOSDFile.Exists()) continue;

    anOSDFile.Remove();
  }

  if(IsDirDeleted) {
    OSD_Path aPath(aDirName);
    OSD_Directory aDir(aPath);
    // san -- Using a special code block below is essential - it ensures that
    // OSD_FileIterator instance is destroyed by the moment when
    // OSD_Directory::Remove() is called.
    // Otherwise, the directory remains locked (at least on Windows)
    // by the iterator and cannot be removed.
    {
      OSD_FileIterator anIterator(aPath, '*');
      if(!aDir.Exists() || anIterator.More())
        return;
    }
    aDir.Remove();
  }
  //if ( IsDirDeleted )
  //{
//    bool ok = Qtx::rmDir( QString( theFiles[0].c_str() ) );
    // std::cout<<"\n DEBUG: rmdir "<<theFiles[0].c_str()<<" ok: "<<ok<<std::endl;
  //}

}

/*!
  Removes files which was created from module theModuleName if 
  <IsDirDeleted> is true tmp directory is also deleted if it is empty
*/
void LightApp_Driver::RemoveTemporaryFiles( const char* theModuleName, const bool IsDirDeleted )
{
  std::string aModuleName(theModuleName);
  ListOfFiles aFiles = myMap[aModuleName];
  // aFiles must contain temporary directory name in its first item
  // and names of files (relatively the temporary directory) in the others

  if ( myBloc )
  {
    fclose( myBloc );
    QString fName = Qtx::addSlash( aFiles.front().c_str() ) + "used_by_salome";
    QFile::remove( fName );
  }
#ifndef WIN32
  if ( myBlocFcntl )
  {
    fclose( myBlocFcntl );
    QString fName = Qtx::addSlash( aFiles.front().c_str() ) + "used_by_salome.fcntl";
    QFile::remove( fName );
  }
#endif

  RemoveFiles( aFiles, IsDirDeleted );

  // if ( IsDirDeleted )
  // {
  //   bool ok = Qtx::rmDir( QString( aFiles[0].c_str() ) );
  //   std::cout<<"\n DEBUG: rmdir "<<aFiles[0].c_str()<<" ok: "<<std::endl;
  // }
}

/*!
  Clears map of list files
*/ 
void LightApp_Driver::ClearDriverContents()
{
  std::map<std::string, ListOfFiles>::iterator it;
  for ( it = myMap.begin(); it != myMap.end(); ++it ) 
  {
    const char* aModuleName = const_cast<char*>(it->first.c_str());
    // If the driver contains temporary files - 
    // remove them along with the temporary directory
    RemoveTemporaryFiles( aModuleName, IsTemporary() );
  }
  myMap.clear();  
  // Reset the "temporary" flag
  SetIsTemporary( false );
}

/*!
  \return a temp directory to store created files like "/tmp/sub_dir/"
*/
std::string LightApp_Driver::GetTmpDir()
{
  if ( myTmpDir.length() != 0 )
    return myTmpDir;

  //Find a temporary directory to store a file
  TCollection_AsciiString aTmpDir;

  char *Tmp_dir = getenv("SALOME_TMP_DIR");
  if ( !Tmp_dir )
    Tmp_dir = getenv ( "TEMP" );
  if ( !Tmp_dir )
    Tmp_dir = getenv ( "TMP" );
  if ( Tmp_dir ) 
  {
    aTmpDir = TCollection_AsciiString(Tmp_dir);
#ifdef WIN32
    if(aTmpDir.Value(aTmpDir.Length()) != '\\') aTmpDir+='\\';
#else
    if(aTmpDir.Value(aTmpDir.Length()) != '/') aTmpDir+='/';
#endif      
  }
  else 
  {
#ifdef WIN32
    aTmpDir = TCollection_AsciiString("C:\\");
#else
    aTmpDir = TCollection_AsciiString("/tmp/");
#endif
  }
  
  // create temporary folder
  QString pref = Qtx::addSlash( aTmpDir.ToCString() ) + 
    SUIT_Session::session()->getSavePrefix();
  int i = 0;
  QString tmpDir;
  do 
  {
    tmpDir = pref + QString( "%1" ).arg( ++i );
  }
  while ( QFileInfo( tmpDir ).exists() );

  aTmpDir = Qtx::addSlash(tmpDir).toLatin1().constData();

  OSD_Path aPath(aTmpDir);
  OSD_Directory aDir(aPath);

#ifdef WIN32
  // Workaround for OSD_Protection bug on Windows
  OSD_Protection aProtection(OSD_RWXD, OSD_RWXD, OSD_RWXD, OSD_RWXD);
#else
  OSD_Protection aProtection(OSD_RX, OSD_RWXD, OSD_RX, OSD_RX);
#endif
  aDir.Build(aProtection);
  myTmpDir = aTmpDir.ToCString();

  if ( myBloc )
    fclose( myBloc );
  QString blocName = Qtx::addSlash(tmpDir) + "used_by_salome";
  myBloc = fopen( blocName.toLatin1().constData(), "w" );
  
#ifndef WIN32
  blocName += ".fcntl";
  if ( myBlocFcntl )
    fclose( myBlocFcntl );
  myBlocFcntl = fopen( blocName.toLatin1().constData(), "w" );
  lockFcntl( QString() );
#endif  
  return aTmpDir.ToCString();
}

/*!
  \return the dir by the path
*/
std::string LightApp_Driver::GetDirFromPath( const std::string& thePath ) {
  if(thePath == "")
    return "";
  OSD_Path aPath = OSD_Path(TCollection_AsciiString(const_cast<char*>(thePath.c_str())));
  TCollection_AsciiString aDirString(aPath.Trek());
  aDirString.ChangeAll('|','/');
  return aDirString.ToCString();
}

//=============================================================================
// Function : PutFilesToFirstStream
// Purpose  : 
//=============================================================================
bool LightApp_Driver::PutFilesToFirstStream( const std::string& theModuleName, unsigned char*& theBuffer,
                                             long& theBufferSize, bool theNamesOnly )
{
  myCurrPos = 0;
  myCurrBuff = 0;
  myCurrFileIndex = 0;
  myCurrIFile = 0;
  mySizeToBeWritten = 0;
  if ( myFileSizes )
  {
    delete[] myFileSizes;
    myFileSizes = 0;
  }
  if ( myFileNameSizes )
  {
    delete[] myFileNameSizes;
    myFileNameSizes = 0;
  }

  ListOfFiles aFiles = myMap[theModuleName];
  // aFiles must contain temporary directory name in its first item
  // and names of files (relatively the temporary directory) in the others

  int i, aLength = aFiles.size() - 1;
  if(aLength <= 0) {
     theBufferSize = 0;
     theBuffer = new unsigned char[theBufferSize];
    return true;
  }
  //Get a temporary directory for saved a file
  TCollection_AsciiString aTmpDir(const_cast<char*>(aFiles[0].c_str()));

  int aNbFiles = 0;
  myFileNameSizes = new size_t[aLength];
  myFileSizes = new size_t[aLength];

  //Determine the required size of the buffer
  TCollection_AsciiString aFileName;
  for (i = 0; i < aLength; i++) 
  {
    char* aFName = const_cast<char*>(aFiles[i+1].c_str());
    aFileName = aFName;
    //Check if the file exists
    if (!theNamesOnly) 
    { 
      TCollection_AsciiString aFullPath = aTmpDir + aFileName;   
      OSD_Path anOSDPath(aFullPath);
      OSD_File anOSDFile(anOSDPath);
      if(!anOSDFile.Exists()) 
        continue;
#ifdef WNT
      ifstream aFile(aFullPath.ToCString(), ios::binary);
#else
      ifstream aFile(aFullPath.ToCString());
#endif
      aFile.seekg(0, ios::end);
      myFileSizes[i] = aFile.tellg();
      mySizeToBeWritten += myFileSizes[i];              //Add a space to store the file
    }
    myFileNameSizes[i] = strlen(aFName) + 1;
    mySizeToBeWritten += myFileNameSizes[i];          //Add a space to store the file name
    mySizeToBeWritten += (theNamesOnly)?4:12;       //Add 4 bytes: a length of the file name,
                                              //    8 bytes: length of the file itself
    aNbFiles++;
  }

  mySizeToBeWritten += 4;      //4 bytes for a number of the files that will be written to the stream;

  size_t aCurrSize = mySizeToBeWritten <= GetMaxBuffSize() ? mySizeToBeWritten : GetMaxBuffSize();
  theBuffer = new unsigned char[ aCurrSize ];
  myCurrBuff = theBuffer;

  //Initialize 4 bytes of the buffer by 0
  memset( theBuffer, 0, 4); 
  //Copy the number of files that will be written to the stream
  memcpy( theBuffer, &aNbFiles, ((sizeof(int) > 4) ? 4 : sizeof(int))); 

  myCurrPos = 4;
  int aCurrnetBuff = 0;

  for ( myCurrFileIndex = 0; myCurrFileIndex < aLength; myCurrFileIndex++ ) 
  {
    if (!theNamesOnly) 
    { // mpv 15.01.2003: we don't open any file if theNamesOnly = true
      TCollection_AsciiString aName(const_cast<char*>(aFiles[myCurrFileIndex+1].c_str()));
      TCollection_AsciiString aFullPath = aTmpDir + aName;
      OSD_Path anOSDPath(aFullPath);
      OSD_File anOSDFile(anOSDPath);
      if(!anOSDFile.Exists()) continue;
#ifdef WNT
      myCurrIFile = new ifstream(aFullPath.ToCString(), ios::binary);
#else
      myCurrIFile = new ifstream(aFullPath.ToCString());
#endif
    }
    
    //Initialize 4 bytes of the buffer by 0
    memset( (theBuffer + myCurrPos), 0, 4); 
        
    //Copy the length of the file name to the buffer
    memcpy((theBuffer + myCurrPos), (myFileNameSizes + myCurrFileIndex), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
    myCurrPos += 4;
    
    //Copy the file name to the buffer
    char* aFName = const_cast<char*>(aFiles[myCurrFileIndex+1].c_str());
    memcpy( myCurrBuff + myCurrPos, aFName, myFileNameSizes[ myCurrFileIndex ] );
    myCurrPos += myFileNameSizes[myCurrFileIndex];
    
    if (!theNamesOnly) // mpv 15.01.2003: we don't copy file content to the buffer if !theNamesOnly
    { 
      //Initialize 8 bytes of the buffer by 0
      memset( myCurrBuff + myCurrPos, 0, 8 ); 

      //Copy the length of the file to the buffer
      memcpy( myCurrBuff + myCurrPos,  
              (unsigned char*)(myFileSizes + myCurrFileIndex), ((sizeof(long) > 8) ? 8 : sizeof(long)));
      myCurrPos += 8;
      
      // old code for small files
      if ( aCurrSize < GetMaxBuffSize() )
      {
        myCurrIFile->seekg(0, ios::beg);
        myCurrIFile->read((char *)(theBuffer + myCurrPos), myFileSizes[myCurrFileIndex]);
        myCurrIFile->close();
        delete(myCurrIFile);
        myCurrIFile = 0;
        myCurrPos += myFileSizes[myCurrFileIndex];
        mySizeToBeWritten = 0;
      }
      else // new code for big files
      {
        myCurrIFile->seekg(0, ios::beg);
        int aPos = myCurrIFile->tellg();
        size_t aSizeToRead = GetMaxBuffSize() - myCurrPos;
        if ( aSizeToRead > myFileSizes[myCurrFileIndex] - myCurrIFile->tellg() )
          aSizeToRead = myFileSizes[myCurrFileIndex] - myCurrIFile->tellg();
        myCurrIFile->read((char *)(theBuffer + myCurrPos), aSizeToRead );
        mySizeToBeWritten -= aSizeToRead - myCurrPos;
        myCurrPos += aSizeToRead;
        if ( myCurrPos == GetMaxBuffSize() )
        {
          myCurrPos = 0;
          break;
        }
        else // file was saved complitelly
        {
          myCurrIFile->close();
          delete(myCurrIFile);
          myCurrIFile = 0;
        }
        if ( myCurrIFile )
          aPos = myCurrIFile->tellg();
      }
    }
  }
  
  if ( aCurrSize < GetMaxBuffSize() )
    theBufferSize = myCurrPos;
  else 
    theBufferSize = GetMaxBuffSize();

  return aCurrSize < GetMaxBuffSize();
}

//=============================================================================
// Function : HasUnsavedData
// Purpose  : 
//=============================================================================
bool LightApp_Driver::HasUnsavedData() const
{
  return myCurrIFile && mySizeToBeWritten;
}

//=============================================================================
// Function : PutFilesToNextStream
// Purpose  : 
//=============================================================================
bool LightApp_Driver::PutFilesToNextStream( const std::string& theModuleName, unsigned char*& theBuffer,
                                            long& theBufferSize, bool theNamesOnly )
{
  if ( theNamesOnly || !HasUnsavedData() )
    return true;

  ListOfFiles aFiles = myMap[ theModuleName ];
  int aLength = aFiles.size() - 1;
  if ( aLength <= 0 )
    return true;

  TCollection_AsciiString aTmpDir(const_cast<char*>(aFiles[0].c_str()));

  size_t aCurrSize = mySizeToBeWritten <= GetMaxBuffSize() ? mySizeToBeWritten : GetMaxBuffSize();

  for ( ; myCurrFileIndex < aLength; myCurrFileIndex++ ) 
  {
    TCollection_AsciiString aName(const_cast<char*>(aFiles[myCurrFileIndex+1].c_str()));
    TCollection_AsciiString aFullPath = aTmpDir + aName;
    OSD_Path anOSDPath(aFullPath);
    OSD_File anOSDFile(anOSDPath);
    if(!anOSDFile.Exists()) 
      continue;

    if ( !myCurrIFile )
    {
#ifdef WNT
      myCurrIFile = new ifstream( aFullPath.ToCString(), ios::binary );
#else
      myCurrIFile = new ifstream( aFullPath.ToCString() );
#endif

      myCurrIFile->seekg(0, ios::beg);

      ////////////
      //Initialize 4 bytes of the buffer by 0
      memset( (myCurrBuff + myCurrPos), 0, 4); 
      //Copy the length of the file name to the buffer
      memcpy((myCurrBuff + myCurrPos), ( myFileNameSizes + myCurrFileIndex ), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
      myCurrPos += 4;

      //Copy the file name to the buffer
      char* aFName = const_cast<char*>(aFiles[myCurrFileIndex+1].c_str());
      memcpy( myCurrBuff + myCurrPos, aFName, myFileNameSizes[ myCurrFileIndex ] );
      myCurrPos += myFileNameSizes[myCurrFileIndex];
      /////////////

      //Initialize 8 bytes of the buffer by 0
      memset( myCurrBuff + myCurrPos, 0, 8 ); 
      
      //Copy the length of the file to the buffer
      memcpy( myCurrBuff + myCurrPos,  
        (unsigned char*)(myFileSizes + myCurrFileIndex), ((sizeof(long) > 8) ? 8 : sizeof(long)));
      myCurrPos += 8;
    }

    // old code for small files
    if ( aCurrSize < GetMaxBuffSize() )
    {
      //myCurrIFile->seekg(0, ios::beg);
      size_t aSizeToRead = myFileSizes[myCurrFileIndex] - myCurrIFile->tellg();
      myCurrIFile->read((char *)(theBuffer + myCurrPos), aSizeToRead );
      myCurrIFile->close();
      delete(myCurrIFile);
      myCurrIFile = 0;
      myCurrPos += aSizeToRead;
      mySizeToBeWritten = 0;
    }
    else // new code for big files
    {
      int aPos = myCurrIFile->tellg();
      size_t aFileSize = myFileSizes[myCurrFileIndex];
      size_t aSizeToRead = GetMaxBuffSize() - myCurrPos;
      if ( aSizeToRead > myFileSizes[myCurrFileIndex] - myCurrIFile->tellg() )
        aSizeToRead = myFileSizes[myCurrFileIndex] - myCurrIFile->tellg();
      myCurrIFile->read((char *)(theBuffer + myCurrPos), aSizeToRead );
      mySizeToBeWritten -= aSizeToRead - myCurrPos;
      myCurrPos += aSizeToRead;
      if ( myCurrPos == GetMaxBuffSize() )
      {
        myCurrPos = 0;
        break;
      }
      else // file was saved complitelly
      {
        myCurrIFile->close();
        delete(myCurrIFile);
        myCurrIFile = 0;
      }
      if ( myCurrIFile )
        aPos = myCurrIFile->tellg();
    }
  }
  
  if ( aCurrSize < GetMaxBuffSize() )
    theBufferSize = myCurrPos - 1;
  else 
    theBufferSize = GetMaxBuffSize();

  return aCurrSize < GetMaxBuffSize();
}

//=============================================================================
// Function : PutFirstStreamToFiles
// Purpose  : 
//=============================================================================
void LightApp_Driver::PutFirstStreamToFiles( ListOfFiles& theListOfFiles, 
                                             const unsigned char* theBuffer,
                                             const long theBufferSize, 
                                             bool theNamesOnly )
{
  theListOfFiles.resize( 0 );
  myCurrPos = 0;
  myCurrOFile = 0;
  mySizeToBeWritten = 0;

  if ( theBufferSize == 0 || theBuffer == 0 )
    return;

  delete[] myFileSizes;
  delete[] myFileNameSizes;

  // Create a temporary directory for the component's data files
  std::string aDir = GetTmpDir();

  // Remember that the files are in a temporary location that should be deleted
  // when a study is closed
  SetIsTemporary( true );

  //Get a temporary directory for saving a file
  TCollection_AsciiString aTmpDir(const_cast<char*>(aDir.c_str()));

  myCurrPos = 4;
  
  //Copy the number of files in the stream
  memcpy(&myNbFilles, theBuffer, sizeof(int)); 

  const int n = myNbFilles + 1;
  theListOfFiles.resize( n );
  myFileSizes = new size_t[ n ];
  myFileNameSizes = new size_t[ n ];
  theListOfFiles[ 0 ] = aDir;
  myFileSizes[ 0 ] = 0;
  myFileNameSizes[ 0 ] = 0;

  for( myCurrFileIndex = 1; myCurrFileIndex <= myNbFilles; myCurrFileIndex++) 
  {
    //Put a length of the file name to myFileNameSizes[ myCurrFileIndex ]
    memcpy(&myFileNameSizes[ myCurrFileIndex ], (theBuffer + myCurrPos), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
    myCurrPos += 4;

    char *aFileName = new char[ myFileNameSizes[ myCurrFileIndex ] ];
    //Put a file name to aFileName
    memcpy(aFileName, (theBuffer + myCurrPos), myFileNameSizes[ myCurrFileIndex ]); 
    myCurrPos += myFileNameSizes[ myCurrFileIndex ];
    
    //Put a length of the file to myFileSizes
    if (!theNamesOnly) 
    {
      memcpy(&myFileSizes[ myCurrFileIndex ], (theBuffer + myCurrPos), ((sizeof(long) > 8) ? 8 : sizeof(long)));
      myCurrPos += 8;    
      
      TCollection_AsciiString aFullPath = aTmpDir + aFileName;
      
#ifdef WNT  
  myCurrOFile  = new ofstream(aFullPath.ToCString(), ios::out | ios::binary);
#else
  myCurrOFile  = new ofstream(aFullPath.ToCString());
#endif
      
      std::string aStrFileName( aFileName );
      theListOfFiles[ myCurrFileIndex ] = aStrFileName;
      delete[] aFileName;

      if ( myFileSizes[ myCurrFileIndex ] <= theBufferSize - myCurrPos )
      {
        // old code 
        myCurrOFile->write((char *)(theBuffer+myCurrPos), myFileSizes[ myCurrFileIndex ] ); 
        myCurrOFile->close();  
        delete myCurrOFile;
        myCurrOFile = 0;
        myCurrPos += myFileSizes[ myCurrFileIndex ];
        myFileSizes[ myCurrFileIndex ] = 0;
      }
      else 
      {
        // old code for big files
        size_t aSize = theBufferSize - myCurrPos;
        myCurrOFile->write((char *)( theBuffer + myCurrPos ), aSize );
        myFileSizes[ myCurrFileIndex ] -= aSize;
        myCurrPos = 0;
        break;
      }
    }
  }
}

//=============================================================================
// Function : PutNextStreamToFiles
// Purpose  : 
//=============================================================================
void LightApp_Driver::PutNextStreamToFiles( ListOfFiles& theListOfFiles, 
                                            const unsigned char* theBuffer,
                                            const long theBufferSize, 
                                            bool theNamesOnly )
{
  if ( theNamesOnly )
    return;

  std::string aDir = GetTmpDir();

  //Get a temporary directory for saving a file
  TCollection_AsciiString aTmpDir(const_cast<char*>(aDir.c_str()));

  if ( !myNbFilles )
    return;

  for( ; myCurrFileIndex <= myNbFilles; myCurrFileIndex++) 
  {
    if ( !myCurrOFile )
    {
      //Put a length of the file name to myFileNameSizes
      //int myFileNameSizes;
      memcpy(&myFileNameSizes[ myCurrFileIndex ], (theBuffer + myCurrPos), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
      myCurrPos += 4;

      char *aFileName = new char[ myFileNameSizes[ myCurrFileIndex ] ];
      //Put a file name to aFileName
      memcpy(aFileName, (theBuffer + myCurrPos), myFileNameSizes[ myCurrFileIndex ]); 
      myCurrPos += myFileNameSizes[ myCurrFileIndex ];

      std::string aStrFileName( aFileName );
      theListOfFiles[ myCurrFileIndex ] = aStrFileName;
      
      //Put a length of the file to myFileSizes
      if ( theNamesOnly )
      {
        delete[] aFileName;
        continue;
      }

      memcpy(&myFileSizes[ myCurrFileIndex ], (theBuffer + myCurrPos), ((sizeof(long) > 8) ? 8 : sizeof(long)));
      myCurrPos += 8;    

      TCollection_AsciiString aFullPath = aTmpDir + aFileName;

#ifdef WNT  
      myCurrOFile  = new ofstream(aFullPath.ToCString(), ios::out | ios::binary);
#else
      myCurrOFile  = new ofstream(aFullPath.ToCString());
#endif

      delete[] aFileName;
      
    } //if ( !myCurrOFile )

    if ( myFileSizes[ myCurrFileIndex ] <= theBufferSize - myCurrPos )
    {
      // old code 
      myCurrOFile->write((char *)(theBuffer+myCurrPos), myFileSizes[ myCurrFileIndex ] ); 
      myCurrOFile->close();  
      delete myCurrOFile;
      myCurrOFile = 0;
      myCurrPos += myFileSizes[ myCurrFileIndex ];
      myFileSizes[ myCurrFileIndex ] = 0;
    }
    else 
    {
      // old code for big files
      size_t aFileSize = myFileSizes[ myCurrFileIndex ];
      size_t aSize = theBufferSize - myCurrPos;
      myCurrOFile->write((char *)( theBuffer + myCurrPos ), aSize );
      myFileSizes[ myCurrFileIndex ] -= aSize;
      myCurrPos = 0;
      break;
    }
  } // for
}
