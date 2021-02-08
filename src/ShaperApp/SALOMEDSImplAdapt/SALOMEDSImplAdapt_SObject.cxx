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

#include "SALOMEDSImplAdapt_SObject.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImplAdaptCopyAttribute.hxx"

// ----------------------------------------------------------------------------------------
SALOMEDSImplAdapt_SObject::SALOMEDSImplAdapt_SObject(const SALOMEDSImpl_SObject& theSObject):
  SALOMEDSImpl_SObject(theSObject){}

bool SALOMEDSImplAdapt_SObject::SALOMEDSImplAdapt_SObject::ReferencedObject(_PTR(SObject) & theObject) const
{
  SALOMEDSImpl_SObject anObj;
  bool aResult = SALOMEDSImpl_SObject::ReferencedObject(anObj);
  if(aResult){
    theObject = _PTR(SObject)(new _CLASS(SObject)(anObj));
  }
  return aResult;
}

bool SALOMEDSImplAdapt_SObject::FindAttribute( _PTR(GenericAttribute) & anAttr, const std::string aTypeOfAttribute)
{
  DF_Attribute* aFilledAttr = nullptr;
  bool isFound = SALOMEDSImpl_SObject::FindAttribute(aFilledAttr, aTypeOfAttribute);
  if(isFound){
    anAttr = copyAttribute(_CAST(GenericAttribute, aFilledAttr));
    return true;
  }
  return false;
}

SALOMEDSImplAdapt_SObject* SALOMEDSImplAdapt_SObject::GetPersistentCopy() const
{
  SALOMEDSImplAdapt_SObject* so = new SALOMEDSImplAdapt_SObject;
  so->_lab = _lab;
  so->_name = _name;
  so->_type = _type;
  so->_value = _value;
  return so;
}
// ----------------------------------------------------------------------------------------
