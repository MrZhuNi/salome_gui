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

INCLUDE(UseQtExt)

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  ${QT_INCLUDES}
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(${QT_DEFINITIONS})
IF(ENABLE_TESTRECORDER)
  ADD_DEFINITIONS("-DENABLE_TESTRECORDER ${TESTRECORDER_DEFINITIONS}")
ENDIF()

# libraries to link to
SET(_link_LIBRARIES ${PLATFORM_LIBS} ${QT_LIBRARIES} qtx ObjBrowser)
IF(ENABLE_TESTRECORDER)
  LIST(APPEND _link_LIBRARIES ${TESTRECORDER_LIBS})
ENDIF()

# --- headers ---

# header files / to be processed by moc
SET(_moc_HEADERS
  SUIT_MessageBox.h
)

# header files / no moc processing
SET(_other_HEADERS
  SUIT_OverrideCursor.h
  SUIT_FileValidator.h
  Qtxx.h
  SUIT.h
)

# header files / to install
SET(suitt_HEADERS ${_moc_HEADERS} ${_other_HEADERS})

# --- resources ---

# resource files / to be processed by lrelease

# resource files / static
SET(_other_RESOURCES
  resources/icon_visibility_on.png
  resources/icon_visibility_off.png
  resources/view_sync.png
)

# --- sources ---

# sources / moc wrappings
QT_WRAP_MOC(_moc_SOURCES ${_moc_HEADERS})

# sources / static
SET(_other_SOURCES
  SUIT_OverrideCursor.cxx
  SUIT_MessageBox.cxx
  SUIT_FileValidator.cxx
  Qtxx.cxx
)

# sources / to compile
SET(suitt_SOURCES ${_other_SOURCES} ${_moc_SOURCES})

# --- rules ---

ADD_LIBRARY(suitt ${suitt_SOURCES})
TARGET_LINK_LIBRARIES(suitt ${_link_LIBRARIES})
INSTALL(TARGETS suitt EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${suitt_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})

INSTALL(FILES ${_other_RESOURCES} DESTINATION ${SALOME_GUI_INSTALL_RES_DATA})

