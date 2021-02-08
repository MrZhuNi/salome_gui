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

#include "SALOMEDSImplAdapt_Study.hxx"
#include "SALOMEDSImplAdapt_ChildIterator.hxx"
#include "SALOMEDSImplAdapt_SComponentIterator.hxx"
#include "SALOMEDSImplAdapt_StudyBuilder.hxx"
#include "SALOMEDSImplAdapt_SObject.hxx"
#include "SALOMEDSImplAdapt_UseCaseBuilder.hxx"
#include "SALOMEDSImplAdapt_SComponent.hxx"
#include "SALOMEDSImpl_GenericVariable.hxx"

typedef SALOMEDSImpl_GenericVariable  SALOMEDSImplAdapt_GenericVariable;

// ----------------------------------------------------------------------------------------
SALOMEDSImplAdapt_Study::SALOMEDSImplAdapt_Study()
  :_myNewBuilder(nullptr), _myUseCaseBuilder(nullptr)
{}

// ----------------------------------------------------------------------------------------
_PTR(SObject) SALOMEDSImplAdapt_Study::FindObjectID(const QString& anObjectID)
{
  auto aFound = SALOMEDSImpl_Study::FindObjectID(anObjectID.toUtf8().data());
  if(!aFound.IsNull())
    return _PTR(SObject) (new _CLASS(SObject)(aFound));
  return nullptr;
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_Study::CanOpen(const std::string & url)
{
  SALOMEDSImpl_Study aTmp;
  return aTmp.Open(url);
}

// ----------------------------------------------------------------------------------------
_CLASS(StudyBuilder)* SALOMEDSImplAdapt_Study::NewBuilder()
{
  _myNewBuilder = std::unique_ptr<_CLASS(StudyBuilder)>
      (new _CLASS(StudyBuilder)(SALOMEDSImpl_Study::NewBuilder()));
  return _myNewBuilder.get();
}

// ----------------------------------------------------------------------------------------
_PTR(SComponent) SALOMEDSImplAdapt_Study::FindComponent (const QString& aComponentName)
{
  return _PTR(SComponent)(_CLASS(SComponent)(
                            SALOMEDSImpl_Study::FindComponent(aComponentName.toUtf8().data())));
}

// ----------------------------------------------------------------------------------------
_PTR(SComponent) SALOMEDSImplAdapt_Study::FindComponentID(const QString& aComponentID){
  return _PTR(SComponent)(_CLASS(SComponent)(
                            SALOMEDSImpl_Study::FindComponentID(aComponentID.toUtf8().data())));
}

// ----------------------------------------------------------------------------------------
std::vector<_PTR(SObject)> SALOMEDSImplAdapt_Study::FindDependances(const _PTR(SObject) & obj)
{
  std::vector<_PTR(SObject)> aResult;
  if(obj){
    auto aRefs = SALOMEDSImpl_Study::FindDependances(
          *std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(obj) );
    for(auto aVal : aRefs)
      aResult.push_back(_PTR(SObject)(new _CLASS(SObject) (aVal)));
  }
  return aResult;
}

// ----------------------------------------------------------------------------------------
_PTR(ChildIterator) SALOMEDSImplAdapt_Study::NewChildIterator(const _PTR(SObject)& aSO)
{
  if(aSO)
    return _PTR(ChildIterator) (_CLASS(ChildIterator)(
                                  *std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(aSO)
                                  ).GetPersistentCopy());
  return nullptr;
}

// ----------------------------------------------------------------------------------------
_CLASS(UseCaseBuilder)* SALOMEDSImplAdapt_Study::GetUseCaseBuilder()
{
  _myUseCaseBuilder = std::unique_ptr<_CLASS(UseCaseBuilder)>(
        new _CLASS(UseCaseBuilder)(SALOMEDSImpl_Study::GetUseCaseBuilder()));
  return _myUseCaseBuilder.get();
}

// ----------------------------------------------------------------------------------------
_PTR(SComponentIterator) SALOMEDSImplAdapt_Study::GetNewComponentIterator()
{
  return _PTR(SComponentIterator)( new SALOMEDSImplAdapt_SComponentIterator(GetDocument()));
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_Study::IsReal(const std::string & aName)
{
  return IsTypeOf(aName, _CLASS(GenericVariable)::REAL_VAR);
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_Study::IsInteger(const std::string & aName)
{
  return IsTypeOf(aName, _CLASS(GenericVariable)::INTEGER_VAR);
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_Study::IsString(const std::string & aName)
{
  return IsTypeOf(aName, _CLASS(GenericVariable)::STRING_VAR);
}

// ----------------------------------------------------------------------------------------
int SALOMEDSImplAdapt_Study::GetInteger( const std::string & aName )
{
  return GetVariableValue(aName);
}

// ----------------------------------------------------------------------------------------
double SALOMEDSImplAdapt_Study::GetReal( const std::string & aName)
{
  return GetVariableValue(aName);
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_Study::IsBoolean(const std::string & aName)
{
  return IsTypeOf(aName, _CLASS(GenericVariable)::BOOLEAN_VAR);
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_Study::GetBoolean(const std::string & aName)
{
  return GetVariableValue(aName);
}

// ----------------------------------------------------------------------------------------
std::string SALOMEDSImplAdapt_Study::GetString(const std::string & aName)
{
  return GetStringVariableValue(aName);
}
