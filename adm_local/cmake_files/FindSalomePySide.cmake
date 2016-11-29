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
# Author: Roman NIKOLAEV
#

# PySide detection for Salome
#
#  !! Please read the generic detection procedure in SalomeMacros.cmake !!
#

# PySide needs shiboken, call it automatically
FIND_PACKAGE(SalomeShiboken REQUIRED)

# Find the PySide
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(PySide PYSIDE_INCLUDE_DIR 2)

# Find the pyside-uic and pyside-rcc executables
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(PyTools PYSIDE_PYUIC_EXECUTABLE 2)
MARK_AS_ADVANCED(PYSIDE_PYUIC_EXECUTABLE PYSIDE_PYRCC_EXECUTABLE PYSIDE_PYUIC_PATH PYSIDE_PYRCC_PATH)

# TODO : RNV : Check difference between Pakage_FOUND and PACKAGE_FOUND (in upper case and "as is") !!!
IF(PySide_FOUND AND PYTOOLS_FOUND)
  SALOME_ACCUMULATE_ENVIRONMENT(PATH ${PYSIDE_PYUIC_EXECUTABLE})
  SALOME_ACCUMULATE_ENVIRONMENT(LD_LIBRARY_PATH ${PYSIDE_PYTHONPATH})
  SALOME_ACCUMULATE_ENVIRONMENT(PYTHONPATH ${PYSIDE_PYTHONPATH})
  
  # Define shiboken generator options
  SET(SIHBOKEN_GENERATOR_EXTRA_FLAGS --generator-set=shiboken --enable-parent-ctor-heuristic --enable-pyside-extensions --enable-return-value-heuristic --use-isnull-as-nb_nonzero)

  # Define C++ preprocessor options
  SET(PYSIDE_DEFINITIONS -DWITH_PYSIDE -DAVOID_PROTECTED_HACK)

  # Costomize include directories
  SET(PYSIDE_INCLUDE_DIRS ${PYSIDE_INCLUDE_DIR}
      PYSIDE_INCLUDE_DIRS ${PYSIDE_INCLUDE_DIR}/QtCore
      PYSIDE_INCLUDE_DIRS ${PYSIDE_INCLUDE_DIR}/QtGui
      PYSIDE_INCLUDE_DIRS ${PYSIDE_INCLUDE_DIR}/QtXml)
ENDIF()
