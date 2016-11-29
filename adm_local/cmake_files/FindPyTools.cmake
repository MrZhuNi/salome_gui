# Copyright (C) 2013-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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

# - Find PySide installation
# Sets the following variables:
#    PYSIDE_PYUIC_EXECUTABLE  - path to the pyside-uic executable
#    PYSIDE_PYRCC_EXECUTABLE  - path to the pyside-rcc executable
#    PYSIDE_PYUIC_PATH        - command to launch pyuic with the correct PYTHONPATH
#    PYSIDE_PYRCC_PATH        - command to launch pyrcc with the correct PYTHONPATH
#
#  The executables
#      pyside-uic and pyside-rcc are searched.

IF(NOT PyTools_FIND_QUIETLY)
  MESSAGE(STATUS "Looking for PyTools ...")
ENDIF()

FIND_PROGRAM(PYSIDE_PYUIC_EXECUTABLE NAMES pyside-uic)
FIND_PROGRAM(PYSIDE_PYRCC_EXECUTABLE NAMES pyside-rcc)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PyTools REQUIRED_VARS PYSIDE_PYUIC_EXECUTABLE)

# Wrap the final executables so that they always use the proper environment (PYTHONPATH):
# The results are put to variables:
#   PYSIDE_PYUIC_PATH - command to launch pyuic with the correct PYTHONPATH
#   PYSIDE_PYRCC_PATH - command to launch pyrcc with the correct PYTHONPATH
# TODO: should be done like Sphinx in KERNEL (i.e. generating a shell script)?

IF(WIN32)
  SET(PYSIDE_PYUIC_PATH set PYTHONPATH=${PYSIDE_PYTHONPATH};${SHIBOKEN_PYTHONPATH};%PYTHONPATH% && ${PYSIDE_PYUIC_EXECUTABLE})
  SET(PYSIDE_PYRCC_PATH set PYTHONPATH=${PYSIDE_PYTHONPATH};${SHIBOKEN_PYTHONPATH};%PYTHONPATH% && ${PYSIDE_PYRCC_EXECUTABLE})
ELSE()
  SET(PYSIDE_PYUIC_PATH /usr/bin/env PYTHONPATH="${PYSIDE_PYTHONPATH}:${SHIBOKEN_PYTHONPATH}:$$PYTHONPATH" ${PYSIDE_PYUIC_EXECUTABLE})
  SET(PYSIDE_PYRCC_PATH /usr/bin/env PYTHONPATH="${PYSIDE_PYTHONPATH}:${SHIBOKEN_PYTHONPATH}:$$PYTHONPATH" ${PYSIDE_PYRCC_EXECUTABLE})
ENDIF()
