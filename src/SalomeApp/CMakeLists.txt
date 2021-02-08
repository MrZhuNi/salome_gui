# Copyright (C) 2012-2020  CEA/DEN, EDF R&D, OPEN CASCADE
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

ADD_SUBDIRECTORY(pluginsdemo)
IF(SALOME_BUILD_TESTS)
   ADD_SUBDIRECTORY(Test)
ENDIF()

INCLUDE(UseQtExt)

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  ${OpenCASCADE_INCLUDE_DIR}
  ${QT_INCLUDES}
  ${QWT_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/idl
  ${PROJECT_SOURCE_DIR}/src/CAM
  ${PROJECT_SOURCE_DIR}/src/CASCatch
  ${PROJECT_SOURCE_DIR}/src/Event
  ${PROJECT_SOURCE_DIR}/src/LightApp
  ${PROJECT_SOURCE_DIR}/src/OBJECT
  ${PROJECT_SOURCE_DIR}/src/ObjBrowser
  ${PROJECT_SOURCE_DIR}/src/Prs
  ${PROJECT_SOURCE_DIR}/src/Qtx
  ${PROJECT_SOURCE_DIR}/src/STD
  ${PROJECT_SOURCE_DIR}/src/SUIT
  ${PROJECT_SOURCE_DIR}/src/SVTK
  ${PROJECT_SOURCE_DIR}/src/TOOLSGUI
  ${PROJECT_SOURCE_DIR}/src/VTKViewer
)

IF(SALOME_USE_PYCONSOLE)
  INCLUDE_DIRECTORIES(
    ${PYTHON_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/tools/PyConsole/src
    ${PROJECT_SOURCE_DIR}/tools/PyInterp/src
  )
ENDIF()

# additional preprocessor / compiler flags
ADD_DEFINITIONS(
  ${QT_DEFINITIONS}
  ${OMNIORB_DEFINITIONS}
  ${QWT_DEFINITIONS}
  ${OpenCASCADE_DEFINITIONS}
  ${BOOST_DEFINITIONS}
)

IF(SALOME_USE_PYCONSOLE)
  ADD_DEFINITIONS(${PYTHON_DEFINITIONS})
ENDIF()

# libraries to link to
SET(_link_LIBRARIES
  ${OpenCASCADE_FoundationClasses_LIBRARIES}
  ${QT_LIBRARIES}
  ${PYTHON_LIBRARIES}
  ${KERNEL_OpUtil} ${KERNEL_SALOMELocalTrace} ${KERNEL_SalomeDSClient} ${KERNEL_TOOLSDS}
  CASCatch suit std CAM ObjBrowser SalomePrs Event
  LogWindow LightApp SalomePrs ToolsGUI
)

IF(SALOME_USE_VTKVIEWER)
  LIST(APPEND _link_LIBRARIES VTKViewer SVTK)
ENDIF()

IF(SALOME_USE_PYCONSOLE)
  LIST(APPEND _link_LIBRARIES PyInterp PyConsole)
ENDIF()

IF(SALOME_USE_GLVIEWER)
  LIST(APPEND _link_LIBRARIES GLViewer)
ENDIF()

IF(SALOME_USE_PLOT2DVIEWER)
  LIST(APPEND _link_LIBRARIES Plot2d)
  IF(SALOME_USE_SALOMEOBJECT)
    LIST(APPEND _link_LIBRARIES SPlot2d)
  ENDIF(SALOME_USE_SALOMEOBJECT)
ENDIF(SALOME_USE_PLOT2DVIEWER)

IF(SALOME_USE_OCCVIEWER)
  LIST(APPEND _link_LIBRARIES OCCViewer)
  IF(SALOME_USE_SALOMEOBJECT)
    LIST(APPEND _link_LIBRARIES SOCC)
  ENDIF(SALOME_USE_SALOMEOBJECT)
ENDIF()

IF(SALOME_USE_PVVIEWER)
  LIST(APPEND _link_LIBRARIES PVViewer)
ENDIF()

IF(SALOME_USE_SALOMEOBJECT)
  LIST(APPEND _link_LIBRARIES SalomeObject)
ENDIF()

IF(NOT SALOME_LIGHT_ONLY)
  LIST(APPEND _link_LIBRARIES SalomeIDLGUI)
  LIST(APPEND _link_LIBRARIES ${KERNEL_SalomeContainer})
  LIST(APPEND _link_LIBRARIES ${KERNEL_SalomeLifeCycleCORBA})
ENDIF()

# --- headers ---

# header files / to be processed by moc
SET(SalomeCommon_moc_HEADERS   
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Application.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_CheckFileDlg.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_DataModel.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_DoubleSpinBox.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_ExitDlg.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_IntSpinBox.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_ListView.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_LoadStudiesDlg.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Module.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Study.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_StudyPropertiesDlg.h
)

SET(_moc_HEADERS ${SalomeCommon_moc_HEADERS})

IF(SALOME_USE_PYCONSOLE)
  LIST(APPEND _moc_HEADERS SalomeApp_NoteBook.h)
ENDIF()

# header files / no moc processing
SET(SalomeCommon_other_HEADERS
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_DataObject.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_ExceptionHandler.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Filter.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_ImportOperation.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Tools.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_TypeFilter.h
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_VisualState.h
)

SET(_other_HEADERS ${SalomeCommon_other_HEADERS}
  SalomeApp_Engine_i.h)

IF(SALOME_USE_PYCONSOLE)
  LIST(APPEND _other_HEADERS SalomeApp_PyInterp.h)
ENDIF()

# header files / to install
SET(SalomeApp_HEADERS ${_moc_HEADERS} ${_other_HEADERS})

# --- resources ---

# resource files / to be processed by lrelease
SET(_ts_RESOURCES
  resources/SalomeApp_images.ts
  resources/SalomeApp_msg_en.ts
  resources/SalomeApp_msg_fr.ts
  resources/SalomeApp_msg_ja.ts
)

# resource files / static
SET(_other_RESOURCES
  resources/SalomeApp.xml
)

# --- sources ---

# sources / moc wrappings
QT_WRAP_MOC(_moc_SOURCES ${_moc_HEADERS})

# sources / static
SET(SalomeCommon_other_SOURCES
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Application.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_CheckFileDlg.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_DataModel.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_DataObject.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_DoubleSpinBox.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_ExceptionHandler.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_ExitDlg.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Filter.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_ImportOperation.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_IntSpinBox.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_ListView.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_LoadStudiesDlg.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Module.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Study.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_StudyPropertiesDlg.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_Tools.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_TypeFilter.cxx
  ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_VisualState.cxx
)

SET(_other_SOURCES ${SalomeCommon_other_SOURCES}
 SalomeApp_Engine_i.cxx)

# export for Shaper application
SET(SalomeCommon_moc_HEADERS ${SalomeCommon_moc_HEADERS} PARENT_SCOPE) 
SET(SalomeCommon_other_SOURCES ${SalomeCommon_other_SOURCES} PARENT_SCOPE)
SET(SalomeCommon_other_HEADERS ${SalomeCommon_other_HEADERS} PARENT_SCOPE)

IF(SALOME_USE_PYCONSOLE)
  LIST(APPEND _other_SOURCES SalomeApp_PyInterp.cxx SalomeApp_NoteBook.cxx)
ENDIF()

# sources / to compile
SET(SalomeApp_SOURCES ${_other_SOURCES} ${_moc_SOURCES})

# --- scripts ---

# scripts / bin
SET(_bin_SCRIPTS
  addvars2notebook.py
)

# scripts / python
SET(_py_SCRIPTS
  salome_pluginsmanager.py
)

# --- rules ---

ADD_LIBRARY(SalomeApp ${SalomeApp_SOURCES})
TARGET_LINK_LIBRARIES(SalomeApp ${_link_LIBRARIES})
INSTALL(TARGETS SalomeApp EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${SalomeApp_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
QT_INSTALL_TS_RESOURCES("${_ts_RESOURCES}" "${SALOME_GUI_INSTALL_RES_DATA}")

INSTALL(FILES ${_other_RESOURCES} DESTINATION ${SALOME_GUI_INSTALL_RES_DATA})

SALOME_INSTALL_SCRIPTS("${_py_SCRIPTS}"  ${SALOME_INSTALL_PYTHON})
SALOME_INSTALL_SCRIPTS("${_bin_SCRIPTS}" ${SALOME_INSTALL_SCRIPT_PYTHON})
