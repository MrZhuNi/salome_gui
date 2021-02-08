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

cmake_policy(SET CMP0060 NEW)

INCLUDE(UseQtExt)

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  ${QT_INCLUDES}
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${OpenCASCADE_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_BINARY_DIR}/idl
  ${PROJECT_SOURCE_DIR}/src/Qtx
  ${PROJECT_SOURCE_DIR}/src/SUIT
  ${PROJECT_SOURCE_DIR}/src/Event
  ${PROJECT_SOURCE_DIR}/src/Style
  ${PROJECT_SOURCE_DIR}/src/SUIT
  ${PROJECT_SOURCE_DIR}/src/STD
  ${PROJECT_SOURCE_DIR}/src/CAM
  ${PROJECT_SOURCE_DIR}/src/OBJECT
  ${PROJECT_SOURCE_DIR}/src/LightApp
  ${PROJECT_SOURCE_DIR}/src/ShaperApp
  ${PROJECT_SOURCE_DIR}/../../../SHAPER/src/SHAPERGUI
  ${PROJECT_SOURCE_DIR}/../../../SHAPER/src/XGUI
  ${PROJECT_SOURCE_DIR}/../../../SHAPER/src/ModuleBase
  ${PROJECT_SOURCE_DIR}/../../../SHAPER/src/Config
  ${PROJECT_SOURCE_DIR}/../../../SHAPER/src/Events
  ${PROJECT_SOURCE_DIR}/src/OCCViewer
  ${PROJECT_SOURCE_DIR}/src/ShaperApp/SALOMEDSImplAdapt
  ${PROJECT_SOURCE_DIR}/src/SalomeApp
 )

# additional preprocessor / compiler flags
ADD_DEFINITIONS(
  ${QT_DEFINITIONS}
  ${BOOST_DEFINITIONS}
  ${PYTHON_DEFINITIONS}
  ${OpenCASCADE_DEFINITIONS} 
  -DHAVE_SALOME
  -DDISABLE_ORB
  #-DDISABLE_PYCONSOLE
)

# libraries to link to
SET(_link_LIBRARIES
  ${QT_LIBRARIES}  
  ShaperApp 
)


ADD_EXECUTABLE(shaper Shaper.cxx)
TARGET_LINK_LIBRARIES(shaper ${_link_LIBRARIES})
INSTALL(TARGETS shaper EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_BINS})

