# Copyright (C) 2012-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  ${OpenCASCADE_INCLUDE_DIR}
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(${OpenCASCADE_DEFINITIONS})

# libraries to link to
SET(_link_LIBRARIES 
    ${OpenCASCADE_FoundationClasses_LIBRARIES}
    ${OpenCASCADE_Visualization_LIBRARIES})

# --- headers ---

# header files / to install
SET(ViewerData_HEADERS
  ViewerData_AISShape.hxx
)

# --- sources ---

# sources / to compile

SET(ViewerData_SOURCES
  ViewerData_AISShape.cxx
)

# --- rules ---

ADD_LIBRARY(ViewerData ${ViewerData_SOURCES})
TARGET_LINK_LIBRARIES(ViewerData ${_link_LIBRARIES})
INSTALL(TARGETS ViewerData EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${ViewerData_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
