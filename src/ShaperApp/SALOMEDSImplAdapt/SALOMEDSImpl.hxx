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

#ifndef SALOMEDSIMPL_HXX__
#define SALOMEDSIMPL_HXX__

#include "SALOMEDSImpl_AttributeComment.hxx"
#include "SALOMEDSImpl_AttributeDrawable.hxx"
#include "SALOMEDSImpl_AttributeExpandable.hxx"
#include "SALOMEDSImpl_AttributeExternalFileDef.hxx"
#include "SALOMEDSImpl_AttributeFileType.hxx"
#include "SALOMEDSImpl_AttributeFlags.hxx"
#include "SALOMEDSImpl_AttributeGraphic.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx"
#include "SALOMEDSImpl_AttributeInteger.hxx"
#include "SALOMEDSImpl_AttributeLocalID.hxx"
#include "SALOMEDSImpl_AttributeName.hxx"
#include "SALOMEDSImpl_AttributeOpened.hxx"
#include "SALOMEDSImpl_AttributePersistentRef.hxx"
#include "SALOMEDSImpl_AttributePixMap.hxx"
#include "SALOMEDSImpl_AttributePythonObject.hxx"
#include "SALOMEDSImpl_AttributeReal.hxx"
#include "SALOMEDSImpl_AttributeSelectable.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfInteger.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfReal.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_AttributeTableOfInteger.hxx"
#include "SALOMEDSImpl_AttributeTableOfReal.hxx"
#include "SALOMEDSImpl_AttributeTableOfString.hxx"
#include "SALOMEDSImpl_AttributeTarget.hxx"
#include "SALOMEDSImpl_AttributeTextColor.hxx"
#include "SALOMEDSImpl_AttributeTextHighlightColor.hxx"
#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include "SALOMEDSImpl_AttributeUserID.hxx"
#include "SALOMEDSImpl_AttributeParameter.hxx"
#include "SALOMEDSImpl_AttributeString.hxx"
#include "SALOMEDSImpl_ChildIterator.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_GenericVariable.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_SComponentIterator.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_UseCaseIterator.hxx"
#include "SALOMEDSImpl_IParameters.hxx"

#endif
