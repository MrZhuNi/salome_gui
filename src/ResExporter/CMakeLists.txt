# Copyright (C) 2012-2015  CEA/DEN, EDF R&D, OPEN CASCADE
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
IF (NOT SALOME_BUILD_WITH_QT5)
  SET(_QT_INCLUDES ${QT_INCLUDES})
  SET(_QT_DEFINITIONS ${QT_DEFINITIONS})
  SET(_QT_LIBRARIES ${QT_LIBRARIES})
ELSE()
  SET(_QT_INCLUDES ${Qt5Widgets_INCLUDES})
  SET(_QT_DEFINITIONS ${Qt5Widgets_DEFINITIONS})
  SET(_QT_LIBRARIES ${Qt5Widgets_LIBRARIES})
ENDIF()

# additional include directories
INCLUDE_DIRECTORIES(
  ${_QT_INCLUDES}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src/Qtx
  ${PROJECT_SOURCE_DIR}/src/SUIT
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(${_QT_DEFINITIONS})

# libraries to link to
SET(_link_LIBRARIES ${_QT_LIBRARIES} qtx suit)

# --- rules ---

ADD_EXECUTABLE(ResourceExporter ResourceExporter.cxx)
TARGET_LINK_LIBRARIES(ResourceExporter SUITApp ${_link_LIBRARIES})
INSTALL(TARGETS ResourceExporter EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_BINS})
