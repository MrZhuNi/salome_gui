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

#ifndef SALOMEDSIMPL_ADAPT_CopyAttribute_HXX
#define SALOMEDSIMPL_ADAPT_CopyAttribute_HXX

#include "SALOMEDSImplAdapt_definitions.hxx"
#include <cstring>
#include "SALOMEDSImplAdaptTypes.hxx"

namespace{

#define __CreateAttributeClass(className) if (strcmp(aTypeOfAttribute.c_str(), #className) == 0) { \
  anAttr = _PTR(className)( new _CLASS(className)); \
}

#define __CreateGenerictAttributeClass \
  __CreateAttributeClass(AttributeReal) \
  __CreateAttributeClass(AttributeInteger) \
  __CreateAttributeClass(AttributeSequenceOfReal) \
  __CreateAttributeClass(AttributeSequenceOfInteger) \
  __CreateAttributeClass(AttributeName) \
  __CreateAttributeClass(AttributeComment) \
  __CreateAttributeClass(AttributeIOR) \
  __CreateAttributeClass(AttributePixMap) \
  __CreateAttributeClass(AttributeLocalID) \
  __CreateAttributeClass(AttributeTableOfInteger) \
  __CreateAttributeClass(AttributeTableOfReal) \
  __CreateAttributeClass(AttributeTableOfString) \
  __CreateAttributeClass(AttributePythonObject) \
  __CreateAttributeClass(AttributePersistentRef) \
  __CreateAttributeClass(AttributeDrawable) \
  __CreateAttributeClass(AttributeSelectable) \
  __CreateAttributeClass(AttributeExpandable) \
  __CreateAttributeClass(AttributeOpened) \
  __CreateAttributeClass(AttributeTextColor) \
  __CreateAttributeClass(AttributeTextHighlightColor) \
  __CreateAttributeClass(AttributeTarget) \
  __CreateAttributeClass(AttributeStudyProperties) \
  __CreateAttributeClass(AttributeExternalFileDef) \
  __CreateAttributeClass(AttributeFileType) \
  __CreateAttributeClass(AttributeFlags) \
  __CreateAttributeClass(AttributeGraphic) \
  __CreateAttributeClass(AttributeTreeNode) \
  __CreateAttributeClass(AttributeUserID) \
  __CreateAttributeClass(AttributeParameter) \
  __CreateAttributeClass(AttributeString)

  inline _PTR(GenericAttribute) copyAttribute(_CLASS(GenericAttribute) * aFilledAttr)
  {
    _PTR(GenericAttribute) anAttr;
    std::string aTypeOfAttribute = aFilledAttr->GetClassType();
    __CreateGenerictAttributeClass
        // fill using implicit-defined copy assignent operator
        *anAttr = *aFilledAttr;
    return anAttr;
  }
}

#endif
