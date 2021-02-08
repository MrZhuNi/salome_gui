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
// file: SALOMEDSImplAdapt_SComponent.hxx
// purpose: adaper for class SALOMEDSImpl_SComponent in Salome GUI without CORBA
//=======================================================================================

#ifndef SALOMEDSIMPL_ADAPT_SComponent_HXX
#define SALOMEDSIMPL_ADAPT_SComponent_HXX

#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImplAdapt_SObject.hxx"

class SALOMEDSImplAdapt_SComponent: public virtual SALOMEDSImpl_SComponent,
    public virtual SALOMEDSImplAdapt_SObject
{
public:
  SALOMEDSImplAdapt_SComponent(const SALOMEDSImpl_SComponent& theSCO);
  SALOMEDSImplAdapt_SComponent();
  virtual ~SALOMEDSImplAdapt_SComponent() = default;

  SALOMEDSImplAdapt_SComponent* GetPersistentCopy() const;
};

#endif
