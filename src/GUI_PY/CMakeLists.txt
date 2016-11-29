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

# --- resources ---

# uic files / to be processed by pyuic
SET(_pyuic_files
  SelectVarsDialog.ui
  genericdialog.ui
  mytestdialog.ui
)

# --- scripts ---

# scripts / pyuic wrappings

IF(SALOME_USE_PYSIDE)
  PYSIDE_WRAP_UIC(_pyuic_SCRIPTS ${_pyuic_files})
ELSE(SALOME_USE_PYSIDE)
  PYQT4_WRAP_UIC(_pyuic_SCRIPTS ${_pyuic_files})
ENDIF(SALOME_USE_PYSIDE)



# scripts / static
SET(_other_SCRIPTS 
  __init__.py
  genericdialog.py
  helper.py
  mytestdialog.py
  selectvars.py
)

# scritps / to install

SET(_all_SCRIPTS ${_other_SCRIPTS} ${_pyuic_SCRIPTS})

# --- rules ---

SALOME_INSTALL_SCRIPTS("${_all_SCRIPTS}" ${SALOME_INSTALL_PYTHON}/salome/gui)
