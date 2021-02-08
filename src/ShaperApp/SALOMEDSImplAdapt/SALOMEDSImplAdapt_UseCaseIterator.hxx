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
// file: SALOMEDSImplAdapt_UseCaseIterator.hxx
// purpose: adaper for class SALOMEDSImpl_UseCaseIterator in Salome GUI without CORBA
//=======================================================================================

#ifndef SALOMEDSIMPL_ADAPT_UseCaseIterator_HXX
#define SALOMEDSIMPL_ADAPT_UseCaseIterator_HXX

#include "SALOMEDSImpl_UseCaseIterator.hxx"

#include <memory>

class SALOMEDSImplAdapt_SObject;

class SALOMEDSIMPL_EXPORT SALOMEDSImplAdapt_UseCaseIterator: public SALOMEDSImpl_UseCaseIterator
{
public:
  SALOMEDSImplAdapt_UseCaseIterator() = default;
  explicit SALOMEDSImplAdapt_UseCaseIterator(const std::unique_ptr<SALOMEDSImpl_UseCaseIterator> refIterator);
  virtual ~SALOMEDSImplAdapt_UseCaseIterator() = default;

  SALOMEDSImplAdapt_UseCaseIterator* GetPersistentCopy() const;
  SALOMEDSImplAdapt_SObject GetValue();
};

#endif
