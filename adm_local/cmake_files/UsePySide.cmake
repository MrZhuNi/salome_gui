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
# Author: Roman NIKOLAEV, (roman.nikolaev@opencascade.com)

INCLUDE(CommonTools)

####################################################################
#
# PYSIDE_WRAP_UIC macro
#
# Create Python modules by processing input *.ui (Qt designer) files with
# PySide pyside-uic tool.
#
# USAGE: PYSIDE_WRAP_UIC(output_files pyuic_files)
#
# ARGUMENTS:
#   output_files [out] variable where output file names are listed to
#   pyuic_files  [in]  list of *.ui files
# 
# NOTES:
#   - Input files are considered relative to the current source directory.
#   - Output files are generated in the current build directory.
#   - Macro automatically adds custom build target to generate output files
# 
####################################################################
MACRO(PYSIDE_WRAP_UIC outfiles)

 IF(NOT WIN32)
  FOREACH(_input ${ARGN})
    GET_FILENAME_COMPONENT(_input_name ${_input} NAME)
    STRING(REPLACE ".ui" "_ui.py" _input_name ${_input_name})
    SET(_output ${CMAKE_CURRENT_BINARY_DIR}/${_input_name})
    ADD_CUSTOM_COMMAND(
      OUTPUT ${_output}
      COMMAND ${PYSIDE_PYUIC_PATH} -o ${_output} ${CMAKE_CURRENT_SOURCE_DIR}/${_input}
      MAIN_DEPENDENCY ${_input}
      )
    SET(${outfiles} ${${outfiles}} ${_output})
  ENDFOREACH()
  _GET_UNIQUE_TARGET_NAME(BUILD_UI_PY_FILES _uniqueTargetName)
  ADD_CUSTOM_TARGET(${_uniqueTargetName} ALL DEPENDS ${${outfiles}})

 ELSEIF(NOT WIN32)
####
# ANA: Workaround for the Microsoft Visual Studio 2010. Seems there is a bug in 
# the Microsoft Visual Studio 2010 or CMake 2.8.10.2: custom target doesn't work 
# for the list of the dependencies. It works only for the first dependency in the 
# list. So generate separate target for the each input file. This problem will be 
#investigated in the future.
####

  SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
  _GET_UNIQUE_TARGET_NAME(BUILD_UI_PY_FILES _uniqueTargetName)
  ADD_CUSTOM_TARGET(${_uniqueTargetName} ALL)
  FOREACH(_input ${ARGN})
    GET_FILENAME_COMPONENT(_input_name ${_input} NAME)
    STRING(REPLACE ".ui" "_ui.py" _input_name ${_input_name})
    SET(_output ${CMAKE_CURRENT_BINARY_DIR}/${_input_name})
    _GET_UNIQUE_TARGET_NAME(BUILD_UI_PY_FILES _TgName)
    ADD_CUSTOM_TARGET(${_TgName} ${PYSIDE_PYUIC_PATH} -o ${_output} ${CMAKE_CURRENT_SOURCE_DIR}/${_input}
      DEPENDS ${_input}
      )
    SET_TARGET_PROPERTIES(${_TgName} PROPERTIES FOLDER PYSIDE_WRAP_UIC_TARGETS)
    ADD_DEPENDENCIES(${_uniqueTargetName} DEPEND ${_TgName})
    SET(${outfiles} ${${outfiles}} ${_output})
  ENDFOREACH()
 ENDIF(NOT WIN32)
ENDMACRO(PYSIDE_WRAP_UIC)

####################################################################
#
# PYSIDE_WRAP_QRC macro
#
# Generate Python wrappings for *.qrc files by processing them with pyrcc4.
#
# USAGE: PYSIDE_WRAP_QRC(output_files qrc_files)
#
# ARGUMENTS:
#   output_files [out] variable where output file names are listed to
#   qrc_files  [in]  list of *.qrc files
# 
# NOTES:
#   - Input files are considered relative to the current source directory.
#   - Output files are generated in the current build directory.
#   - Macro automatically adds custom build target to generate output files
# 
####################################################################

MACRO(PYSIDE_WRAP_QRC outfiles)
  FOREACH(_input ${ARGN})
    GET_FILENAME_COMPONENT(_input_name ${_input} NAME)
    STRING(REPLACE ".qrc" "_qrc.py" _input_name ${_input_name})
    SET(_output ${CMAKE_CURRENT_BINARY_DIR}/${_input_name})
    ADD_CUSTOM_COMMAND(
      OUTPUT ${_output}
      COMMAND ${PYSIDE_PYRCC_PATH} -o ${_output} ${CMAKE_CURRENT_SOURCE_DIR}/${_input}
      MAIN_DEPENDENCY ${_input}
      )
    SET(${outfiles} ${${outfiles}} ${_output})
  ENDFOREACH()
  _GET_UNIQUE_TARGET_NAME(BUILD_QRC_PY_FILES _uniqueTargetName)
  ADD_CUSTOM_TARGET(${_uniqueTargetName} ALL DEPENDS ${${outfiles}})
ENDMACRO(PYSIDE_WRAP_QRC)

####################################################################
#   
# CREATE_PYSIDE_MODULE macro
#
# Create PySide module.
#
# USAGE: CREATE_PYSIDE_MODULE( module_name module_include_dir 
#                              module_libraries module_global_file
#                              module_dependencies module_timesystem_path 
# 			       module_sources module_gen_sources )
#
# ARGUMENTS:
# module_name            [in] name of the PySide module.
# module_global_file     [in] module global header file
# module_include_dir     [in] list of the module include directories.
# module_include_dir     [in] list of the libraries with witch to 
#                             be module linked.
# module_dependencies    [in] list of the module dependencies.
# module_timesystem_path [in] list of the module typesystem paths.
# module_sources         [in] module static sources.
# module_gen_sources     [in] list of the sources to be generated.
#
# NOTES:
#   - Output files are generated in the current build directory.
#   - This version of macro requires class(es) definition in the *_typesystem.xml file.
#
####################################################################
MACRO(CREATE_PYSIDE_MODULE MODULE_NAME MODULE_GLOBAL_FILE MODULE_INCLUDE_DIR MODULE_LIBRARIES MODULE_DEPS MODULE_TYPESYSTEM_PATH MODULE_SOURCES MODULE_GEN_SOURCES)

  IF(${ARGC} GREATER 8)
    SET(TYPESYSTEM_NAME ${ARGV8})
  ELSE()
        set (TYPESYSTEM_NAME "")
  ENDIF()

  IF(${ARGC} GREATER 9)
    string(REPLACE ";" "\\;" DROPPED_ENTRIES "${${ARGV9}}")
  ELSE()
    SET (DROPPED_ENTRIES "")
  ENDIF()
        
  IF(NOT EXISTS ${TYPESYSTEM_NAME})
    SET(TYPESYSTEM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_NAME}_typesystem.xml)
  ELSE()
    SET(TYPESYSTEM_PATH ${TYPESYSTEM_NAME})
  ENDIF()
    
  STRING(REPLACE ";" "${PATH_SEP}" _TMP_QT_INCLUDES "${QT_INCLUDES}")	
  STRING(REPLACE ";" "${PATH_SEP}" _TMP_MODULE_INCLUDE_DIR "${${MODULE_INCLUDE_DIR}}")	

  ADD_CUSTOM_COMMAND(OUTPUT ${${MODULE_GEN_SOURCES}}
                     COMMAND ${SHIBOKEN_BINARY} ${SIHBOKEN_GENERATOR_EXTRA_FLAGS}
                     ${MODULE_GLOBAL_FILE}
                     --include-paths=${PYSIDE_INCLUDE_DIR}${PATH_SEP}${_TMP_QT_INCLUDES}${PATH_SEP}${_TMP_MODULE_INCLUDE_DIR}
                     --typesystem-paths=${PYSIDE_TYPESYSTEMS}${PATH_SEP}${${MODULE_TYPESYSTEM_PATH}}
                     --output-directory=${CMAKE_CURRENT_BINARY_DIR}
                     --avoid-protected-hack
                     ${TYPESYSTEM_PATH}
                     --api-version="${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}"
                     --drop-type-entries="${DROPPED_ENTRIES}"
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      COMMENT "Running shiboken generator for ${MODULE_NAME}..."
                      MAIN_DEPENDENCY ${TYPESYSTEM_PATH})
    
  ADD_LIBRARY(${MODULE_NAME} ${${MODULE_SOURCES}} ${${MODULE_GEN_SOURCES}})
  IF(WIN32)
    SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES SUFFIX ".pyd")
    SET(${MODULE_NAME}_SUFFIX ".pyd")
  ELSE()
    SET(${MODULE_NAME}_SUFFIX ${CMAKE_SHARED_MODULE_SUFFIX})
  ENDIF()
  TARGET_LINK_LIBRARIES(${MODULE_NAME} ${${MODULE_LIBRARIES}})
  IF(${MODULE_DEPS})
    ADD_DEPENDENCIES(${MODULE_NAME} ${${MODULE_DEPS}})
  ENDIF()
ENDMACRO()