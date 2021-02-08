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



#include "SALOMEDSImplAdapt_UseCaseBuilder.hxx"
#include "SALOMEDSImplAdapt_UseCaseIterator.hxx"
#include "SALOMEDSImplAdapt_SObject.hxx"


// ----------------------------------------------------------------------------------------
SALOMEDSImplAdapt_UseCaseBuilder::SALOMEDSImplAdapt_UseCaseBuilder(SALOMEDSImpl_UseCaseBuilder* aCopy):
  SALOMEDSImpl_UseCaseBuilder(aCopy){}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_UseCaseBuilder::HasChildren(const _PTR(SObject) & theObject){
  if(theObject)
    return SALOMEDSImpl_UseCaseBuilder::HasChildren(
          *std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(theObject));
  return false;
}

// ----------------------------------------------------------------------------------------
bool SALOMEDSImplAdapt_UseCaseBuilder::IsUseCaseNode(const _PTR(SObject) & theObject){
  if(theObject)
    return SALOMEDSImpl_UseCaseBuilder::IsUseCaseNode(
          *std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(theObject));
  return false;
}

// ----------------------------------------------------------------------------------------
_PTR(UseCaseIterator) SALOMEDSImplAdapt_UseCaseBuilder::GetUseCaseIterator(const _PTR(SObject) & theObject){
  if(theObject){
    return _PTR(UseCaseIterator)(_CLASS(UseCaseIterator)(
                                   std::unique_ptr<SALOMEDSImpl_UseCaseIterator>(
                                     SALOMEDSImpl_UseCaseBuilder::GetUseCaseIterator(
                                       *std::dynamic_pointer_cast<SALOMEDSImpl_SObject>(theObject)
                                       ).GetPersistentCopy())
                                   ).GetPersistentCopy());
  }
  return nullptr;
}
