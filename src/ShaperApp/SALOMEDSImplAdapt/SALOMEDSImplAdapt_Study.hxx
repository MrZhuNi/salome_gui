// Copyright (C) 2007-2020 CEA/DEN, EDF R&D, CSGROUP
//
// Copyright (C) 2003-2007 OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//=======================================================================================
// file: SALOMEDSImplAdapt_Study.hxx
// purpose: adaper for class SALOMEDSImpl_Study in Salome GUI without CORBA
//=======================================================================================

#ifndef SALOMEDSIMPL_ADAPT_Study_HXX
#define SALOMEDSIMPL_ADAPT_Study_HXX

#include "SALOMEDSImplAdapt_definitions.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImplAdapt_StudyBuilder.hxx"
#include "SALOMEDSImplAdapt_UseCaseBuilder.hxx"

#include <QString>
#include <memory>

class SALOMEDSImplAdapt_ChildIterator;
class SALOMEDSImplAdapt_SComponentIterator;
class SALOMEDSImplAdapt_SObject;
class SALOMEDSImplAdapt_SComponent;

class SALOMEDSIMPL_EXPORT SALOMEDSImplAdapt_Study: public SALOMEDSImpl_Study
{
public:
  SALOMEDSImplAdapt_Study();
  virtual ~SALOMEDSImplAdapt_Study() = default;

  _CLASS(StudyBuilder)* NewBuilder();
  _CLASS(UseCaseBuilder)* GetUseCaseBuilder();

  _PTR(SObject) FindObjectID(const QString& anObjectID);
  _PTR(SComponent) FindComponent(const QString& aComponentName);
  _PTR(SComponent) FindComponentID(const QString& aComponentID);
  _PTR(ChildIterator) NewChildIterator(const _PTR(SObject)& aSO);
  _PTR(SComponentIterator) GetNewComponentIterator();

  std::vector<_PTR(SObject)> FindDependances(const _PTR(SObject) & obj);

  bool CanOpen(const std::string & url);
  bool IsReal(const std::string & aName);
  bool IsInteger(const std::string & aName);
  bool IsString(const std::string & aName);
  bool IsBoolean(const std::string & aName);
  inline bool Modified(){return IsModified();}
  int GetInteger( const std::string & aName);
  bool GetBoolean(const std::string & aName);
  std::string GetString(const std::string & aName);
  double GetReal( const std::string & aName);

private:
  std::unique_ptr<_CLASS(StudyBuilder)> _myNewBuilder;
  std::unique_ptr<_CLASS(UseCaseBuilder)> _myUseCaseBuilder;
};

#endif
