
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


#include "SALOMEDSImplAdapt_UseCaseIterator.hxx"
#include "SALOMEDSImplAdapt_SObject.hxx"

// ----------------------------------------------------------------------------------------
SALOMEDSImplAdapt_UseCaseIterator::SALOMEDSImplAdapt_UseCaseIterator(const std::unique_ptr<SALOMEDSImpl_UseCaseIterator> refIterator):
  SALOMEDSImpl_UseCaseIterator(refIterator.get()){}

SALOMEDSImplAdapt_UseCaseIterator* SALOMEDSImplAdapt_UseCaseIterator::GetPersistentCopy() const
{
  SALOMEDSImplAdapt_UseCaseIterator* itr = new SALOMEDSImplAdapt_UseCaseIterator();
  itr->_it = _it;
  itr->_node = _node;
  itr->_guid = _guid;
  itr->_levels = _levels;
  return itr;
}

SALOMEDSImplAdapt_SObject SALOMEDSImplAdapt_UseCaseIterator::GetValue() {
  return SALOMEDSImplAdapt_SObject(SALOMEDSImpl_UseCaseIterator::Value());
}
// ----------------------------------------------------------------------------------------
