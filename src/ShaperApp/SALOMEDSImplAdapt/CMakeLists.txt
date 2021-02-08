# Copyright (C) 2012-2020  CEA/DEN, EDF R&D, CSGROUP
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#


SET(SALOMEDSImplAdapt_SOURCES
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt/SALOMEDSImplAdapt_ChildIterator.cxx
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt/SALOMEDSImplAdapt_SComponent.cxx
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt/SALOMEDSImplAdapt_SComponentIterator.cxx
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt/SALOMEDSImplAdapt_SObject.cxx
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt/SALOMEDSImplAdapt_StudyBuilder.cxx
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt/SALOMEDSImplAdapt_Study.cxx
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt/SALOMEDSImplAdapt_UseCaseBuilder.cxx
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt/SALOMEDSImplAdapt_UseCaseIterator.cxx
 )

# export for Shaper application
SET(SALOMEDSImplAdapt_SOURCES ${SALOMEDSImplAdapt_SOURCES} PARENT_SCOPE) 

