# Copyright (C) 2012-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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

IF(SALOME_USE_PYSIDE)
  INCLUDE(UsePySide)
ELSE(SALOME_USE_PYSIDE)
  INCLUDE(UsePyQt4)
ENDIF(SALOME_USE_PYSIDE)
IF(SALOME_USE_VTKVIEWER)
  INCLUDE(${VTK_USE_FILE})
ENDIF()

# --- options ---

# --- common include directories ---
SET(SalomePyQt_INCLUDE_DIRS
  ${CAS_INCLUDE_DIRS}
  ${QT_INCLUDES}
  ${QWT_INCLUDE_DIR}
  ${SIP_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/CAM
  ${PROJECT_SOURCE_DIR}/src/Event
  ${PROJECT_SOURCE_DIR}/src/LightApp
  ${PROJECT_SOURCE_DIR}/src/LogWindow
  ${PROJECT_SOURCE_DIR}/src/OBJECT
  ${PROJECT_SOURCE_DIR}/src/OCCViewer
  ${PROJECT_SOURCE_DIR}/src/ObjBrowser
  ${PROJECT_SOURCE_DIR}/src/Plot2d
  ${PROJECT_SOURCE_DIR}/src/PyInterp
  ${PROJECT_SOURCE_DIR}/src/Qtx
  ${PROJECT_SOURCE_DIR}/src/SALOME_PYQT/SALOME_PYQT_GUILight
  ${PROJECT_SOURCE_DIR}/src/STD
  ${PROJECT_SOURCE_DIR}/src/SUIT
)

# additional include directories
IF(SALOME_USE_PYSIDE)
  SET(SalomePyQt_INCLUDE_DIRS
      ${SalomePyQt_INCLUDE_DIRS}
      ${CMAKE_CURRENT_BINARY_DIR}/SalomePyQt
      ${SHIBOKEN_INCLUDE_DIR}
      ${PYSIDE_INCLUDE_DIRS})
ELSE(SALOME_USE_PYSIDE)
  SET(SalomePyQt_INCLUDE_DIRS
      ${SalomePyQt_INCLUDE_DIRS}
      ${SIP_INCLUDE_DIRS})
ENDIF(SALOME_USE_PYSIDE)

INCLUDE_DIRECTORIES(
 ${SalomePyQt_INCLUDE_DIRS}
)

IF(SALOME_LIGHT_ONLY)
  #disable 'ENABLE_CORBA' feature
  SET(PYQT_SIPFLAGS ${PYQT_SIPFLAGS} "-xENABLE_CORBA;")
ELSE(SALOME_LIGHT_ONLY)
  INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/src/SalomeApp
  )
ENDIF(SALOME_LIGHT_ONLY)

# additional preprocessor / compiler flags

# additional preprocessor / compiler flags
IF(SALOME_USE_PYSIDE)
  SET(PACKAGE_DEFINITIONS ${PYSIDE_DEFINITIONS})
ELSE(SALOME_USE_PYSIDE)
  SET(PACKAGE_DEFINITIONS ${PYQT4_DEFINITIONS})
ENDIF(SALOME_USE_PYSIDE)


ADD_DEFINITIONS(
  ${QWT_DEFINITIONS}
  ${QT_DEFINITIONS}
  ${CAS_DEFINITIONS}
  ${BOOST_DEFINITIONS}
  ${PYTHON_DEFINITIONS}
  ${OMNIORB_DEFINITIONS}
  ${PACKAGE_DEFINITIONS})

# libraries to link to
SET(_link_LIBRARIES
  ${PYTHON_LIBRARIES}
  ${VTK_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${QWT_LIBRARY}
  SalomePyQtGUILight
) 

# --- headers ---

# header files / to be processed by moc
SET(_moc_HEADERS SalomePyQt.h)

# --- resources ---

# sip files / to be processed by sip
IF(NOT SALOME_USE_PYSIDE)
  SET(_sip_files SalomePyQt.sip)
ENDIF(NOT SALOME_USE_PYSIDE)

# --- sources ---

# sources / moc wrappings
QT4_WRAP_CPP(_moc_SOURCES ${_moc_HEADERS})

IF(NOT SALOME_USE_PYSIDE)
  # sources / sip wrappings
  PYQT4_WRAP_SIP(_sip_SOURCES ${_sip_files})
ENDIF(NOT SALOME_USE_PYSIDE)

# sources / static
SET(_other_SOURCES SalomePyQt.cxx)

# sources / to compile
SET(SalomePyQt_SOURCES ${_other_SOURCES} ${_moc_SOURCES} ${_sip_SOURCES})

# --- rules ---
IF(SALOME_USE_PYSIDE)

  # --- list of the wrapped sources ---
  SET(SalomePyQt_WRAPPED_SOURCES 
  ${CMAKE_CURRENT_BINARY_DIR}/SalomePyQt/salomepyqt_module_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/SalomePyQt/salome_selection_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/SalomePyQt/qtxaction_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/SalomePyQt/qtxactionset_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/SalomePyQt/qtxactiongroup_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/SalomePyQt/plot2d_curve_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/SalomePyQt/salomepyqt_wrapper.cpp)

  # --- module include directories ---
  SET(SalomePy_MODULE_INCLUDES
  ${PROJECT_SOURCE_DIR}/src/Qtx
  ${PROJECT_SOURCE_DIR}/src/Plot2d)


  # --- create pyside module ---
  MESSAGE("Create pyside module in the ${CAMKE_CURRENT_SOURCE_DIR}")
  CREATE_PYSIDE_MODULE(
    SalomePyQt
    ${CMAKE_CURRENT_SOURCE_DIR}/SalomePyQt_global.h
    SalomePy_MODULE_INCLUDES
    _link_LIBRARIES
    ""
    ""
    SalomePyQt_SOURCES
    SalomePyQt_WRAPPED_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/SalomePyQt_typesystem.xml
  )

ELSE(SALOME_USE_PYSIDE)
  ADD_LIBRARY(SalomePyQt ${SalomePyQt_SOURCES})
  TARGET_LINK_LIBRARIES(SalomePyQt ${_link_LIBRARIES})
ENDIF(SALOME_USE_PYSIDE)

# --- set prefix ---
SET_TARGET_PROPERTIES(SalomePyQt PROPERTIES PREFIX "")

# --- set correct names of the library on win32 platform ---
IF(WIN32)
  SET_TARGET_PROPERTIES(SalomePyQt PROPERTIES SUFFIX ".pyd" DEBUG_OUTPUT_NAME SalomePyQt_d RELEASE_OUTPUT_NAME SalomePyQt)
ENDIF(WIN32)

INSTALL(TARGETS SalomePyQt DESTINATION ${SALOME_INSTALL_LIBS})

FILE(GLOB make_clean_files ${CMAKE_CURRENT_BINARY_DIR}/sip*)
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${make_clean_files}")
