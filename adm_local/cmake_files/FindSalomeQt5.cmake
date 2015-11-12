# Copyright (C) 2013-2015  CEA/DEN, EDF R&D, OPEN CASCADE
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
# Author: Adrien Bruneton
#

# Qt5 detection for Salome
#
#  !! Please read the generic detection procedure in SalomeMacros.cmake !!
#

SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5Core    Qt5Core_INCLUDE_DIRS    1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5Gui     Qt5Gui_INCLUDE_DIRS     1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5Widgets Qt5Widgets_INCLUDE_DIRS 1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5Network Qt5Network_INCLUDE_DIRS 1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5Xml     Qt5Xml_INCLUDE_DIRS     1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5OpenGL  Qt5OpenGL_INCLUDE_DIRS  1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5PrintSupport Qt5PrintSupport_INCLUDE_DIRS 1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5WebKit Qt5WebKit_INCLUDE_DIRS 1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5WebKitWidgets Qt5WebKitWidgets_INCLUDE_DIRS 1 ENVVAR QT5_ROOT_DIR)
SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Qt5X11Extras Qt5X11Extras_INCLUDE_DIRS 1 ENVVAR QT5_ROOT_DIR)

get_filename_component(LINGUIST_PATH ${Qt5Core_DIR} DIRECTORY)
get_filename_component(LINGUIST_PATH ${LINGUIST_PATH} DIRECTORY)
get_filename_component(LINGUIST_PATH ${LINGUIST_PATH} DIRECTORY)

set(LINGUIST_PATH ${LINGUIST_PATH}/bin)

FIND_PROGRAM(QT_LUPDATE_EXECUTABLE NAMES lupdate PATHS ${LINGUIST_PATH} NO_DEFAULT_PATH)
if(QT_LUPDATE_EXECUTABLE)
  message(STATUS "Found lupdate: " ${QT_LUPDATE_EXECUTABLE})
else(QT_LUPDATE_EXECUTABLE)
  if(Linguist_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find lupdate")
  endif(Linguist_FIND_REQUIRED)
endif(QT_LUPDATE_EXECUTABLE)

FIND_PROGRAM(QT_LRELEASE_EXECUTABLE NAMES lrelease PATHS ${LINGUIST_PATH} NO_DEFAULT_PATH)
if(QT_LRELEASE_EXECUTABLE)
  message(STATUS "Found lrelease: " ${QT_LRELEASE_EXECUTABLE})
else(QT_LRELEASE_EXECUTABLE)
  if(Linguist_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find lrelease")
  endif(Linguist_FIND_REQUIRED)
endif(QT_LRELEASE_EXECUTABLE)

#MARK_AS_ADVANCED(QT_QMAKE_EXECUTABLE)
#
#IF(QT5_FOUND) 
#  SALOME_ACCUMULATE_HEADERS(QT_INCLUDES)
#  SALOME_ACCUMULATE_ENVIRONMENT(PATH ${QT_QMAKE_EXECUTABLE})
#  SALOME_ACCUMULATE_ENVIRONMENT(LD_LIBRARY_PATH ${QT_QTCORE_LIBRARY})
#ENDIF()
