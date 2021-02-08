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
// file: SALOMEDSImplAdapt_SObject.hxx
// purpose: adaper for class SALOMEDSImpl_SObject in Salome GUI without CORBA
//=======================================================================================

#ifndef SALOMEDSIMPL_ADAPT_SObject_HXX
#define SALOMEDSIMPL_ADAPT_SObject_HXX

#include "SALOMEDSImplAdapt_definitions.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include <string>

class SALOMEDSImpl_GenericAttribute;
typedef SALOMEDSImpl_GenericAttribute SALOMEDSImplAdapt_GenericAttribute;

class SALOMEDSIMPL_EXPORT SALOMEDSImplAdapt_SObject: public virtual SALOMEDSImpl_SObject
{
public:
  SALOMEDSImplAdapt_SObject() = default;
  SALOMEDSImplAdapt_SObject(const SALOMEDSImpl_SObject& theSObject);
  virtual ~SALOMEDSImplAdapt_SObject() = default;

  bool ReferencedObject(_PTR(SObject) & theObject) const;
  bool FindAttribute( _PTR(GenericAttribute) & anAttr, const std::string aTypeOfAttribute);
  SALOMEDSImplAdapt_SObject* GetPersistentCopy() const;
};

#endif
