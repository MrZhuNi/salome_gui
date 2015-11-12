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

# --- options ---
IF (NOT SALOME_BUILD_WITH_QT5)
  SET(_QT_INCLUDES ${QT_INCLUDES})
  SET(_QT_DEFINITIONS ${QT_DEFINITIONS})
  SET(_QT_LIBRARIES ${QT_LIBRARIES})
ELSE()
  SET(_QT_INCLUDES ${Qt5Widgets_INCLUDES})
  SET(_QT_DEFINITIONS ${Qt5Widgets_DEFINITIONS})
  SET(_QT_LIBRARIES ${Qt5Widgets_LIBRARIES})
ENDIF()

# additional include directories
INCLUDE_DIRECTORIES(
  ${OMNIORB_INCLUDE_DIR}
  ${_QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src/GuiHelpers
  ${PROJECT_SOURCE_DIR}/src/TreeData
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${_QT_DEFINITIONS})

# libraries to link to
SET(_link_LIBRARIES
  ${CORBA_LIBS}
  ${_QT_LIBRARIES}
  SalomeTreeData SalomeGuiHelpers
  ${KERNEL_SalomeLifeCycleCORBA} ${KERNEL_SalomeKernelHelpers}
)

# --- resources ---

# resource files / to be processed by uic
SET(_uic_files mainwindow.ui)

# --- headers ---

# header files / to be processed by moc
SET(_moc_HEADERS mainwindow.hxx)

IF (NOT SALOME_BUILD_WITH_QT5)
  # header files / uic wrappings
  QT4_WRAP_UI(_uic_HEADERS ${_uic_files})
  # sources / moc wrappings
  QT4_WRAP_CPP(_moc_SOURCES ${_moc_HEADERS})
ELSE()
  # header files / uic wrappings
  QT5_WRAP_UI(_uic_HEADERS ${_uic_files})
  # sources / moc wrappings
  QT5_WRAP_CPP(_moc_SOURCES ${_moc_HEADERS})
ENDIF()

# --- sources ---

# sources / to compile
SET(TreeData_guitester_SOURCES MyDataModel.cxx guitester.cxx mainwindow.cxx testhelper.cxx ${_moc_SOURCES} ${_uic_HEADERS})
SET(TreeData_tester_SOURCES MyDataModel.cxx tester.cxx)

# --- rules ---

ADD_EXECUTABLE(TreeData_guitester ${TreeData_guitester_SOURCES})
TARGET_LINK_LIBRARIES(TreeData_guitester ${_link_LIBRARIES})

ADD_EXECUTABLE(TreeData_tester ${TreeData_tester_SOURCES})
TARGET_LINK_LIBRARIES(TreeData_tester ${_link_LIBRARIES})

INSTALL(TARGETS TreeData_guitester TreeData_tester DESTINATION ${SALOME_INSTALL_BINS})
