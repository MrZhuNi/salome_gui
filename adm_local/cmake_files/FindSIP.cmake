# - Find SIP
# Sets the following variables:
#   SIP_EXECUTABLE      - path to the SIP executable
#   SIP_INCLUDE_DIR     - path to the SIP headers
#   SIP_PYTHONPATH      - path to the SIP Python packages
#
#  The header sip.h is looked for.
#  The binary 'sip' is looked for.
#

#########################################################################
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

IF(NOT SIP_FIND_QUIETLY)
  MESSAGE(STATUS "Looking for SIP ...")
ENDIF()

FIND_PROGRAM(SIP_EXECUTABLE sip)
FIND_PATH(SIP_INCLUDE_DIR sip.h PATH_SUFFIXES python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})

IF(SIP_INCLUDE_DIR)
  GET_FILENAME_COMPONENT(SIP_PYTHONPATH "${SIP_INCLUDE_DIR}" PATH)
  GET_FILENAME_COMPONENT(SIP_PYTHONPATH "${SIP_PYTHONPATH}" PATH)
  SET(SIP_PYTHONPATH "${SIP_PYTHONPATH}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages")
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SIP REQUIRED_VARS SIP_INCLUDE_DIR SIP_EXECUTABLE SIP_PYTHONPATH)


