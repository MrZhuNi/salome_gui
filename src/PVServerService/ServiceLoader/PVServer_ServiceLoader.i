%module PVServer_ServiceLoader

%{
  #include "PVServer_ServiceLoader.h"
%}

%include <std_string.i>

class PVServer_ServiceLoader_Exception
{
public:
  PVServer_ServiceLoader_Exception(const std::string & what);
};

class PVServer_ServiceLoader
{
public:
  PVServer_ServiceLoader() throw(PVServer_ServiceLoader_Exception);
  //! Get the IOR of the CORBA service handling the PVServer
  std::string findOrLoadService(const std::string& containerName);
};
