# Copyright (C) 2012-2022  CEA/DEN, EDF R&D
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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)
INCLUDE(CMakeDependentOption)

PROJECT(SalomeCURVEPLOT C CXX)

# Versioning
# ===========
# Project name, upper case
STRING(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)

SET(${PROJECT_NAME_UC}_MAJOR_VERSION 9)
SET(${PROJECT_NAME_UC}_MINOR_VERSION 2)
SET(${PROJECT_NAME_UC}_PATCH_VERSION 0)
SET(${PROJECT_NAME_UC}_VERSION
  ${${PROJECT_NAME_UC}_MAJOR_VERSION}.${${PROJECT_NAME_UC}_MINOR_VERSION}.${${PROJECT_NAME_UC}_PATCH_VERSION})
SET(${PROJECT_NAME_UC}_VERSION_DEV 0)

# User options
# ============
OPTION(SALOME_CURVEPLOT_STANDALONE "Standalone installation of CURVEPLOT" OFF)
OPTION(SALOME_BUILD_DOC "Generate SALOME CURVEPLOT documentation" ON)
OPTION(SALOME_BUILD_TESTS "Generate SALOME CURVEPLOT tests" ON)

# Common CMake macros
# ===================
SET(CONFIGURATION_ROOT_DIR $ENV{CONFIGURATION_ROOT_DIR} CACHE PATH "Path to the Salome CMake configuration files")
IF(EXISTS ${CONFIGURATION_ROOT_DIR})
  LIST(APPEND CMAKE_MODULE_PATH "${CONFIGURATION_ROOT_DIR}/cmake")
  INCLUDE(SalomeMacros NO_POLICY_SCOPE)
ELSE()
  MESSAGE(FATAL_ERROR "We absolutely need the Salome CMake configuration files, please define CONFIGURATION_ROOT_DIR !")
ENDIF()

IF(NOT SALOME_CURVEPLOT_STANDALONE)
    # Find KERNEL 
    # ==============
    SET(KERNEL_ROOT_DIR $ENV{KERNEL_ROOT_DIR} CACHE PATH "Path to the Salome KERNEL")
    IF( EXISTS ${KERNEL_ROOT_DIR} )
      FIND_PACKAGE(SalomeKERNEL REQUIRED)
      KERNEL_WITH_CORBA() # check whether KERNEL builded with CORBA
      ADD_DEFINITIONS(${KERNEL_DEFINITIONS} -DSALOME_KERNEL)
      INCLUDE_DIRECTORIES(${KERNEL_INCLUDE_DIRS})
    ELSE( EXISTS ${KERNEL_ROOT_DIR} )
      MESSAGE(FATAL_ERROR "We absolutely need a Salome KERNEL, please define KERNEL_ROOT_DIR or turn option SALOME_CURVEPLOT_STANDALONE to ON!")
    ENDIF( EXISTS ${KERNEL_ROOT_DIR} )
    
    # ==============
    SET(GUI_ROOT_DIR $ENV{GUI_ROOT_DIR} CACHE PATH "Path to the Salome GUI")
    # Are we compiled being into GUI/tools directory or have we been extracted somewhere else?
    IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../src/Session/SALOME_Session.hxx")  # we are in GUI
      # Re-define GUI_INCLUDE_DIRS 
      SET(GUI_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../PyInterp/src)
    ELSE()      
      IF(EXISTS ${GUI_ROOT_DIR})
        FIND_PACKAGE(SalomeGUI)
      ELSE()
        MESSAGE(FATAL_ERROR "We absolutely need a Salome GUI, please define GUI_ROOT_DIR or turn option SALOME_CURVEPLOT_STANDALONE to ON!")
      ENDIF()
    ENDIF()
    
ENDIF(NOT SALOME_CURVEPLOT_STANDALONE)

# Platform setup
# ==============
INCLUDE(SalomeSetupPlatform)

# Always build libraries as shared objects:
SET(BUILD_SHARED_LIBS TRUE)
# Local macros:
LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/adm_local/cmake_files")

# Prerequisites
# =============
# Find "big" prerequisites first - they reference themselves many others
#   -> this can helps to find the smaller prerequisites and detect conflicts.
# In our case KERNEL may have already loaded many prerequisites which are also used in OSCAR_POST:

##
## From KERNEL:
##

# Mandatory products
FIND_PACKAGE(SalomePythonInterp    REQUIRED)
FIND_PACKAGE(SalomePythonLibs    REQUIRED)
FIND_PACKAGE(SalomeNumPySciPy    REQUIRED)

# Qt
FIND_PACKAGE(SalomeQt5 REQUIRED)
FIND_PACKAGE(SalomePyQt5 REQUIRED)

# Optional products:
IF(SALOME_BUILD_TESTS)
  ENABLE_TESTING()
ENDIF()

# Detection summary:
SALOME_PACKAGE_REPORT_AND_CHECK()

# Directories
# ===========
IF(SALOME_CURVEPLOT_STANDALONE)
    SET(SALOME_INSTALL_LIBS lib/salome CACHE PATH "Install path: SALOME libs")
    SET(_pydir lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages)
    SET(SALOME_INSTALL_PYTHON ${_pydir}/salome CACHE PATH "Install path: SALOME Python stuff")
    SET(SALOME_INSTALL_SCRIPT_PYTHON bin/salome CACHE PATH 
        "Install path: SALOME Python scripts")
    SET(SALOME_INSTALL_CMAKE_LOCAL adm_local/cmake_files CACHE PATH 
        "Install path: local SALOME CMake files") 
    SET(SALOME_INSTALL_RES share/salome/resources CACHE PATH "Install path: SALOME resources")
    SET(SALOME_INSTALL_DOC share/doc/resources CACHE PATH "Install path: SALOME documentation")
ELSE()
    SET(SALOME_INSTALL_LIBS "${SALOME_INSTALL_LIBS}" CACHE PATH "Install path: SALOME libs")
    SET(SALOME_INSTALL_PYTHON "${SALOME_INSTALL_PYTHON}" CACHE PATH 
        "Install path: SALOME Python scripts")
    SET(SALOME_INSTALL_SCRIPT_PYTHON "${SALOME_INSTALL_SCRIPT_PYTHON}" CACHE PATH 
        "Install path: SALOME Python scripts")
    SET(SALOME_INSTALL_CMAKE_LOCAL ${SALOME_INSTALL_CMAKE_LOCAL} CACHE PATH 
        "Install path: local SALOME CMake files") 
    SET(SALOME_INSTALL_RES "${SALOME_INSTALL_RES}" CACHE PATH "Install path: SALOME resources")
    SET(SALOME_INSTALL_DOC "${SALOME_INSTALL_DOC}" CACHE PATH "Install path: SALOME documentation")
    SET(SALOME_INSTALL_HEADERS ${SALOME_INSTALL_HEADERS} CACHE PATH "Install path: SALOME headers")
ENDIF()

# Specific to CURVEPLOT:
SET(SALOME_CURVEPLOT_INSTALL_RES_DATA "${SALOME_INSTALL_RES}/curveplot" CACHE PATH 
    "Install path: SALOME CURVEPLOT specific data")
# Package installation path (lib/python<x.y>/...)
SET(SALOME_CURVEPLOT_INSTALL_PYTHON ${SALOME_INSTALL_PYTHON}/curveplot CACHE INTERNAL  
    "Install path: SALOME CURVEPLOT Python packages" FORCE)
SET(SALOME_CURVEPLOT_INSTALL_SCRIPT_PYTHON ${SALOME_INSTALL_SCRIPT_PYTHON} CACHE INTERNAL  
    "Install path: SALOME CURVEPLOT Python main entry points" FORCE)

SET(CRVPLOT_TEST_INSTALL ${CMAKE_BINARY_DIR}/local/curveplot)
SET(PYQTSIDE_TEST_INSTALL ${CMAKE_BINARY_DIR}/local/pyqtside)

# Sources 
# ========
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(resources)
ADD_SUBDIRECTORY(cmake_files)
IF(SALOME_BUILD_DOC)
#   ADD_SUBDIRECTORY(doc)
ENDIF()

# Configuration export
# (here only the level 1 prerequisites are exposed)
# ====================
INCLUDE(CMakePackageConfigHelpers)

# List of targets in this project we want to make visible to the rest of the world.
# They all have to be INSTALL'd with the option "EXPORT ${PROJECT_NAME}TargetGroup"
SET(_${PROJECT_NAME}_exposed_targets)
IF(NOT SALOME_CURVEPLOT_STANDALONE)
   SET(_${PROJECT_NAME}_exposed_targets CurvePlot)
ENDIF()

# Add all targets to the build-tree export set
EXPORT(TARGETS ${_${PROJECT_NAME}_exposed_targets}
  FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)

# Create the configuration files:
#   - in the build tree:

#      Ensure the variables are always defined for the configure:
SET(KERNEL_ROOT_DIR "${KERNEL_ROOT_DIR}")
SET(GUI_ROOT_DIR "${GUI_ROOT_DIR}")
SET(QT5_ROOT_DIR "${QT5_ROOT_DIR}")
SET(PYQT5_ROOT_DIR "${PYQT5_ROOT_DIR}")
SET(PYTHON_ROOT_DIR "${PYTHON_ROOT_DIR}")
 
SET(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")

# Build variables that will be expanded when configuring Salome<MODULE>Config.cmake:
SALOME_CONFIGURE_PREPARE(PyQt5 Qt5 Python)

CONFIGURE_PACKAGE_CONFIG_FILE(${PROJECT_NAME}Config.cmake.in 
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION "${SALOME_INSTALL_CMAKE_LOCAL}"
    PATH_VARS CONF_INCLUDE_DIRS SALOME_INSTALL_CMAKE_LOCAL CMAKE_INSTALL_PREFIX
       KERNEL_ROOT_DIR GUI_ROOT_DIR QT5_ROOT_DIR PYQT5_ROOT_DIR PYTHON_ROOT_DIR)

WRITE_BASIC_PACKAGE_VERSION_FILE(${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${${PROJECT_NAME_UC}_VERSION}
    COMPATIBILITY AnyNewerVersion)

# Install the CMake configuration files:
INSTALL(FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${SALOME_INSTALL_CMAKE_LOCAL}")

# Install the export set for use with the install-tree
IF(NOT SALOME_CURVEPLOT_STANDALONE)
    INSTALL(EXPORT ${PROJECT_NAME}TargetGroup DESTINATION "${SALOME_INSTALL_CMAKE_LOCAL}" 
        FILE ${PROJECT_NAME}Targets.cmake)
ENDIF()

