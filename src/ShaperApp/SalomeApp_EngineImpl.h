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

#ifndef _SALOMEAPP_ENGINE_IMPL_HXX_
#define _SALOMEAPP_ENGINE_IMPL_HXX_

#include "SalomeApp.h"
#include "SALOMEDS_Tool.hxx"
#include "SALOMEDSImpl_Driver.hxx"
#include <QString>
#include <vector> 
#include <map> 
#include <memory>

class SALOMEDSImpl_SComponent;
class SALOMEDSImpl_SObject;
class SalomeApp_EngineImplFactory;

class SALOMEAPP_EXPORT SalomeApp_EngineImpl: public SALOMEDSImpl_Driver
{
public:

  ~SalomeApp_EngineImpl();

  virtual SALOMEDSImpl_TMPFile* Save(const SALOMEDSImpl_SComponent& theComponent,  const std::string& theURL,
                                     long& theStreamLength,  bool isMultiFile ) override;

  virtual bool Load(const SALOMEDSImpl_SComponent & theComponent,  const unsigned char* theStream,
                    const long theStreamLength, const std::string& theURL, bool isMultiFile ) override;

  virtual SALOMEDSImpl_TMPFile* DumpPython(bool isPublished, bool isMultiFile,
                                           bool& isValidScript, long& theStreamLength) override;

  typedef std::vector<std::string> ListOfFiles;
  typedef std::map<int, ListOfFiles> MapOfFiles;

  ListOfFiles             GetListOfFiles (int);
  void                    SetListOfFiles (int, const ListOfFiles& theListOfFiles);

  static std::string         EngineIORForComponent( const char* theComponentName,
                                                    bool toCreate );
  static SalomeApp_EngineImpl* GetInstance ( const char* theComponentName, bool toCreate );

  static SalomeApp_EngineImpl* EngineForComponent( const char* theComponentName,
                                                   bool toCreate );
  virtual std::string GetIOR() override;
  bool keepFiles() const { return _myKeepFiles; }
  void keepFiles( bool keep ) { _myKeepFiles = keep; }
  static const std::map<std::string, std::string> & getComponentIORS()
  {return _myComponentIORS;}

private:

  friend class SalomeApp_EngineImplFactory; // builder
  explicit SalomeApp_EngineImpl( const char* theComponentName );
  SalomeApp_EngineImpl() = default;

  // methods from SALOMEDS::Driver without implementation.  Must be redefined because
  // there is no default implementation of SALOMEDSImpl_Driver interface
  virtual SALOMEDSImpl_TMPFile* SaveASCII( const SALOMEDSImpl_SComponent &, const std::string& ,
                                           long&, bool ) override
  { return nullptr; }
  virtual bool LoadASCII( const SALOMEDSImpl_SComponent &, const unsigned char* , const long, const std::string& , bool ) override
  {return 0;}
  virtual void Close( const SALOMEDSImpl_SComponent & ) override
  {}
  virtual std::string ComponentDataType();
  virtual std::string IORToLocalPersistentID( const SALOMEDSImpl_SObject &, const std::string &, bool, bool) override
  {return "";}
  virtual std::string LocalPersistentIDToIOR( const SALOMEDSImpl_SObject &, const std::string&, bool, bool ) override
  {return "";}
  virtual bool CanCopy( const SALOMEDSImpl_SObject& ) override
  {return 0;}
  virtual SALOMEDSImpl_TMPFile* CopyFrom(const SALOMEDSImpl_SObject& , int& , long& ) override
  {}
  virtual bool CanPaste(const std::string& , int ) override
  {return 0;}
  virtual std::string PasteInto( const unsigned char* , long, int, const SALOMEDSImpl_SObject& ) override
  {return "";}
  virtual std::string Version() override;

  MapOfFiles                         _myListOfFiles;
  bool                               _myKeepFiles;
  std::string                            _myComponentName;
  static std::map<std::string, std::string> _myComponentIORS;
};

SALOMEAPP_EXPORT class SalomeApp_EngineImplFactory:
    public SALOMEDSImpl_DriverFactory
{
public:

  virtual SALOMEDSImpl_Driver* GetDriverByType(const std::string& theComponentType) override;
  virtual SALOMEDSImpl_Driver* GetDriverByIOR(const std::string& theIOR) override;
  static SalomeApp_EngineImpl* makeEngine(const char* theComponentName, bool toCreate);
  static SalomeApp_EngineImplFactory* instance();
  void removeEngine(const std::string& theComponentName);
  ~SalomeApp_EngineImplFactory();
private:
  std::map<std::string, SalomeApp_EngineImpl*> _myEnginesMap;
  SalomeApp_EngineImplFactory() = default;  
};



#endif

