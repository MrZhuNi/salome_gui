# Copyright (C) 2015-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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
# Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
#

INCLUDE(UseQtExt)
INCLUDE(UsePyQt)

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  $(QT_INCLUDES)
  ${PYTHON_INCLUDE_DIRS}
  ${SIP_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(${QT_DEFINITIONS})

# libraries to link to
SET(_link_LIBRARIES ${QT_LIBRARIES} ${PYTHON_LIBRARIES} PyConsole)

# --- sources ---

# sip files / to be processed by sip
SET(_sip_files PyConsolePy.sip)

# sources / sip wrappings
PYQT_WRAP_SIP(_sip_SOURCES ${_sip_files})

# sources / to compile
SET(PyConsolePy_SOURCES ${_sip_SOURCES})

# --- rules ---

ADD_LIBRARY(PyConsolePy ${PyConsolePy_SOURCES})
IF(WIN32)
  SET_TARGET_PROPERTIES(PyConsolePy PROPERTIES SUFFIX ".pyd" DEBUG_OUTPUT_NAME PyConsolePy_d RELEASE_OUTPUT_NAME PyConsolePy)
ELSE()
  SET_TARGET_PROPERTIES(PyConsolePy PROPERTIES PREFIX "")
ENDIF()
TARGET_LINK_LIBRARIES(PyConsolePy ${_link_LIBRARIES})
INSTALL(TARGETS PyConsolePy EXPORT ${PYCONSOLE_EXPORT_NAME}TargetGroup DESTINATION ${PYCONSOLE_INSTALL_PYTHON})
