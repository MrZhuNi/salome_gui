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
# Author: Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)

####################################################################
#
# SIP_WRAP_SIP macro
#
# Generate C++ wrappings for *.sip files by processing them with sip.
#
# USAGE: SIP_WRAP_SIP(output_files sip_file [sip_file...] [OPTIONS options])
#
# ARGUMENTS:
#   output_files [out] variable where output file names are listed to
#   sip_file     [in]  input sip file (a sequence can be provided)
#   options      [in]  additional options to be specified to sip
# 
# NOTES:
#   - Input files are considered relative to the current source directory.
#   - Output files are generated in the current build directory.
# 
# WARNING:
#   - Macro requires class(es) definition in the *.sip file(s) to be started
#     on a new line without any preceeding characters.
#   - Macro does not properly processes sip features which are wrapped
#     with sip conditionals.
#   - Macro works correctly only if one single sip module is processed
#     (there's only one %Module directive within all input sip files).
#
# TODO:
#   - Check if dependency of static sources on generated headers works properly:
#     if header is changed, dependant sources should be recompiled.
#   - Process sip conditionals.
#   - Process several sip modules.
# 
####################################################################
MACRO(SIP_WRAP_SIP outfiles)
  SET(_output)
  SET(_src_ext ".cc")
  SET(_options -s ${_src_ext} -c .)
  SET(_sip_files)
  SET(_get_options "0")
  FOREACH(_input ${ARGN})
    IF(${_input} STREQUAL "OPTIONS")
      SET(_get_options "1")
    ELSE()
      IF(${_get_options} STREQUAL "1")
        SET(_options ${_options} ${_input})
      ELSE()
        SET(_sip_files ${_sip_files} ${_input})
      ENDIF()
    ENDIF()
  ENDFOREACH()
  SET(_module_input)
  FOREACH(_input ${_sip_files})
    FILE(STRINGS ${_input} _sip_modules REGEX "%Module")
    FILE(STRINGS ${_input} _sip_classes REGEX "^class ")
    FOREACH(_sip_module ${_sip_modules})
      STRING(REGEX MATCH ".*%Module *\\( *name=.*\\).*" _mod_name "${_sip_module}")
      IF (_mod_name)
	STRING(REGEX REPLACE ".*%Module *\\( *name=(.*).*\\).*" "\\1" _mod_name ${_sip_module})
      ELSE()
	STRING(REGEX REPLACE ".*%Module *(.*)" "\\1" _mod_name ${_sip_module})
      ENDIF()
      SET(_mod_header "sipAPI${_mod_name}.h")
      SET(_mod_source "sip${_mod_name}cmodule${_src_ext}")
      LIST(APPEND _output ${CMAKE_CURRENT_BINARY_DIR}/${_mod_source})
      SET(${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${_mod_source})
      SET(_module_input ${_input})
    ENDFOREACH()
    FOREACH(_sip_class ${_sip_classes})
      STRING(REGEX MATCH ".*class +.* *:" _class_name "${_sip_class}")
      IF (_class_name)
	STRING(REGEX REPLACE ".*class +(.*) *:.*" "\\1" _class_name ${_sip_class})
      ELSE()
	STRING(REGEX REPLACE ".*class *(.*)" "\\1" _class_name ${_sip_class})
      ENDIF()
      STRING(STRIP ${_class_name} _class_name)
      SET(_class_source "sip${_mod_name}${_class_name}${_src_ext}")
      LIST(APPEND _output ${CMAKE_CURRENT_BINARY_DIR}/${_class_source})
      SET(${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${_class_source})
    ENDFOREACH()
  ENDFOREACH()
  ADD_CUSTOM_COMMAND(
    OUTPUT ${_output}
    COMMAND ${SIP_EXECUTABLE} ${_options} ${CMAKE_CURRENT_SOURCE_DIR}/${_module_input}
    MAIN_DEPENDENCY ${_module_input}
    )
ENDMACRO(SIP_WRAP_SIP)
