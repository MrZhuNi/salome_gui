# Copyright (C) 2012-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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

ADD_SUBDIRECTORY(dlgfactory)

##
# Python-based packages, part 1 (generic)
##

IF(SALOME_USE_PYCONSOLE)
  # Include sub-project PyConsole
  SET(PYCONSOLE_SUBPROJECT YES CACHE INTERNAL "")
  SET(PYINTERP_EXPORT_NAME "${PROJECT_NAME}" CACHE STRING "")
  SET(PYCONSOLE_EXPORT_NAME "${PROJECT_NAME}" CACHE STRING "")
  SET(PYCONSOLE_INSTALL_LIBS "${SALOME_INSTALL_LIBS}" CACHE PATH "")
  SET(PYCONSOLE_INSTALL_PYTHON "${SALOME_INSTALL_PYTHON}" CACHE PATH "")
  SET(PYCONSOLE_INSTALL_HEADERS "${SALOME_INSTALL_HEADERS}" CACHE PATH "") 
  SET(PYCONSOLE_INSTALL_RES "${SALOME_GUI_INSTALL_RES_DATA}" CACHE PATH "")
  SET(PYCONSOLE_INSTALL_CMAKE "${SALOME_INSTALL_CMAKE_LOCAL}" CACHE PATH "")
  ADD_SUBDIRECTORY(PyConsole) # brings in PyInterp automatically
  ADD_SUBDIRECTORY(CurvePlot) # to be refactored (CurvePlot does not directly depend on PyConsole)
ENDIF(SALOME_USE_PYCONSOLE)

IF(SALOME_USE_PYVIEWER)
  # Include sub-project PyEditor
  SET(PYEDITOR_SUBPROJECT YES CACHE INTERNAL "")
  SET(PYEDITOR_EXPORT_NAME "${PROJECT_NAME}" CACHE STRING "")
  SET(PYEDITOR_INSTALL_BINS "${SALOME_INSTALL_BINS}" CACHE PATH "")
  SET(PYEDITOR_INSTALL_LIBS "${SALOME_INSTALL_LIBS}" CACHE PATH "")
  SET(PYEDITOR_INSTALL_PYTHON "${SALOME_INSTALL_PYTHON}" CACHE PATH "")
  SET(PYEDITOR_INSTALL_HEADERS "${SALOME_INSTALL_HEADERS}" CACHE PATH "") 
  SET(PYEDITOR_INSTALL_RES "${SALOME_GUI_INSTALL_RES_DATA}" CACHE PATH "")
  SET(PYEDITOR_INSTALL_CMAKE "${SALOME_INSTALL_CMAKE_LOCAL}" CACHE PATH "")
  ADD_SUBDIRECTORY(PyEditor)
ENDIF(SALOME_USE_PYVIEWER)

SET(REMOTEFILEBROWSER_INSTALL_BINS "${SALOME_INSTALL_BINS}" CACHE PATH "")
SET(REMOTEFILEBROWSER_INSTALL_LIBS "${SALOME_INSTALL_LIBS}" CACHE PATH "")
SET(REMOTEFILEBROWSER_INSTALL_HEADERS "${SALOME_INSTALL_HEADERS}" CACHE PATH "")
ADD_SUBDIRECTORY(RemoteFileBrowser)
