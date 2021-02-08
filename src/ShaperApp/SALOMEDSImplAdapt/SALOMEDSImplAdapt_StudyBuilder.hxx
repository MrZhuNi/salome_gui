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
// file: SALOMEDSImplAdapt_StudyBuilder.hxx
// purpose: adaper for class SALOMEDSImpl_StudyBuilder in Salome GUI without CORBA
//=======================================================================================

#ifndef SALOMEDSIMPL_ADAPT_StudyBuilder_HXX
#define SALOMEDSIMPL_ADAPT_StudyBuilder_HXX

#include "SALOMEDSImplAdapt_definitions.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"

#include <QString>

class SALOMEDSImpl_SObject;
class SALOMEDSImplAdapt_SObject;
class SALOMEDSImplAdapt_SComponent;
class SALOMEDSImpl_GenericAttribute;
typedef SALOMEDSImpl_GenericAttribute SALOMEDSImplAdapt_GenericAttribute;

class SALOMEDSIMPL_EXPORT SALOMEDSImplAdapt_StudyBuilder: public SALOMEDSImpl_StudyBuilder
{
public:
  SALOMEDSImplAdapt_StudyBuilder() = default;
  explicit SALOMEDSImplAdapt_StudyBuilder(SALOMEDSImpl_StudyBuilder* theBuilder);
  virtual ~SALOMEDSImplAdapt_StudyBuilder() = default;

  void RemoveReference(_PTR(SObject) & aSObject);

  _PTR(SComponent) NewComponent (const QString& ComponentDataType);
  _PTR(GenericAttribute) FindOrCreateAttribute(const _PTR(SComponent)& anObject,
                                               const std::string& aTypeOfAttribute);

  bool SetName(const _PTR(SComponent)& theSO, const std::string& theValue);
  bool DefineComponentInstance(const _PTR(SComponent)& aComponent, const std::string& IOR);
  bool RemoveObjectWithChildren(const _PTR(SObject) &object);
  bool RemoveObjectWithChildren(const SALOMEDSImpl_SObject& anObject) override;

  bool FindAttribute(const _PTR(SObject)& anObject,
                     _PTR(GenericAttribute)& anAttribute,
                     const std::string& aTypeOfAttribute);
};

#endif
