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


#include "SALOMEDSImplAdapt_StudyBuilder.hxx"
#include "SALOMEDSImplAdapt_SObject.hxx"
#include "SALOMEDSImplAdapt_SComponent.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImplAdaptCopyAttribute.hxx"


// ----------------------------------------------------------------------------------------
SALOMEDSImplAdapt_StudyBuilder::SALOMEDSImplAdapt_StudyBuilder(SALOMEDSImpl_StudyBuilder* theBuilder):
  SALOMEDSImpl_StudyBuilder(theBuilder->GetOwner()){}

// ----------------------------------------------------------------------------------------
void SALOMEDSImplAdapt_StudyBuilder::RemoveReference(_PTR(SObject) & aSObject){
  if(aSObject)
    SALOMEDSImpl_StudyBuilder::RemoveReference(*std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(aSObject));
}

// ----------------------------------------------------------------------------------------
_PTR(SComponent) SALOMEDSImplAdapt_StudyBuilder::NewComponent (const QString& ComponentDataType){
  return _PTR(SComponent)(_CLASS(SComponent)(
                            SALOMEDSImpl_StudyBuilder::NewComponent(ComponentDataType.toUtf8().data())));
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_StudyBuilder::SetName(const _PTR(SComponent)& theSO, const std::string& theValue){
  if(theSO){
    return SALOMEDSImpl_StudyBuilder::SetName(*std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(theSO), theValue);
  }
  return false;
}

// ----------------------------------------------------------------------------------------
_PTR(GenericAttribute) SALOMEDSImplAdapt_StudyBuilder::FindOrCreateAttribute(const _PTR(SComponent)& anObject,
                                                                             const std::string& aTypeOfAttribute){
  if(anObject)
  {
    auto aResult = SALOMEDSImpl_StudyBuilder::FindOrCreateAttribute(
          *std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(anObject), aTypeOfAttribute);
    if(aResult){
      return copyAttribute(_CAST(GenericAttribute, aResult));
    }
  }
  return nullptr;
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_StudyBuilder::DefineComponentInstance(const _PTR(SComponent)& aComponent, const std::string& IOR){
  if(aComponent){
    return SALOMEDSImpl_StudyBuilder::DefineComponentInstance(
          *std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(aComponent), IOR);
  }
  return false;
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_StudyBuilder::RemoveObjectWithChildren(const _PTR(SObject) &object){
  if(object)
    return SALOMEDSImpl_StudyBuilder::RemoveObjectWithChildren(
          *std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(object));
  return false;
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_StudyBuilder::RemoveObjectWithChildren(const SALOMEDSImpl_SObject& anObject){
  return SALOMEDSImpl_StudyBuilder::RemoveObjectWithChildren(anObject);
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_StudyBuilder::FindAttribute(const _PTR(SObject)& anObject,
                                                   _PTR(GenericAttribute)& anAttribute,
                                                   const std::string& aTypeOfAttribute)
{
  DF_Attribute* anAttr = nullptr;
  if(anObject){
    bool aResult = SALOMEDSImpl_StudyBuilder::FindAttribute(*std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(anObject),
                                                            anAttr, aTypeOfAttribute);
    anAttribute = _PTR(GenericAttribute)(_CAST(GenericAttribute, anAttr));
    return aResult;
  }
  return false;
}

