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
# Author : Maxim GLIBIN, Open CASCADE S.A.S. (maxim.glibin@opencascade.com)
#

INCLUDE(UseQt4Ext)

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  ${QT_INCLUDES}
  ${PROJECT_SOURCE_DIR}/src/Qtx
  ${PROJECT_SOURCE_DIR}/src/SUIT
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(${QT_DEFINITIONS})

# libraries to link to
SET(_link_LIBRARIES ${PLATFORM_LIBS} ${QT_LIBRARIES} qtx suit)

# --- headers ---

# header files / to be processed by moc
SET(_moc_HEADERS
  PyEditor_Editor.h
  PyEditor_EditorWindow.h
  PyEditor_LineNumberArea.h
  PyEditor_PyHighlighter.h
  PyEditor_Settings.h
  PyEditor_SettingsDlg.h
  PyEditor_ViewManager.h
  PyEditor_ViewModel.h
  PyEditor_ViewWindow.h
)

# header files / no moc processing
SET(_other_HEADERS
  PyEditor.h
)

# header files / to install
SET(PyEditor_HEADERS ${_moc_HEADERS} ${_other_HEADERS})

# --- resources ---

# resource files / to be processed by lrelease
SET(RESOURCES_PATH resources)

SET(_ts_RESOURCES
  ${RESOURCES_PATH}/translations/PyEditor_msg_en.ts
  ${RESOURCES_PATH}/translations/PyEditor_msg_fr.ts
  ${RESOURCES_PATH}/translations/PyEditor_msg_ja.ts
)

# resource files / to be processed by rcc
SET(_rcc_RESOURCES ${RESOURCES_PATH}/PyEditor.qrc)

# --- sources ---

# sources / moc wrappings
QT4_WRAP_CPP(_moc_SOURCES ${_moc_HEADERS})

# sources / rcc wrappings
QT4_ADD_RESOURCES(_rcc_SOURCES ${_rcc_RESOURCES})

# sources / static
SET(_other_SOURCES
  PyEditor_Editor.cxx
  PyEditor_EditorWindow.cxx
  PyEditor_LineNumberArea.cxx
  PyEditor_PyHighlighter.cxx
  PyEditor_Settings.cxx
  PyEditor_SettingsDlg.cxx
  PyEditor_ViewManager.cxx
  PyEditor_ViewModel.cxx
  PyEditor_ViewWindow.cxx
)

# sources / to compile
SET(PyEditor_SOURCES ${_other_SOURCES} ${_moc_SOURCES})

# --- rules ---
ADD_LIBRARY(PyEditor ${PyEditor_SOURCES} ${_rcc_SOURCES})
TARGET_LINK_LIBRARIES(PyEditor ${_link_LIBRARIES})
INSTALL(TARGETS PyEditor EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(DummyPyEditor PyEditor.cxx)
SET_TARGET_PROPERTIES(DummyPyEditor PROPERTIES OUTPUT_NAME "PyEditor")
TARGET_LINK_LIBRARIES(DummyPyEditor ${_link_LIBRARIES} PyEditor)
INSTALL(TARGETS DummyPyEditor EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_BINS})

INSTALL(FILES ${PyEditor_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
QT4_INSTALL_TS_RESOURCES("${_ts_RESOURCES}" "${SALOME_GUI_INSTALL_RES_DATA}")
