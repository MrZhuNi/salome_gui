# Copyright (C) 2012-2020  CEA/DEN, EDF R&D, CSGROUP
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
  ${OpenCASCADE_INCLUDE_DIR}
  ${QT_INCLUDES}
  ${QWT_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIR}
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
  ${PROJECT_SOURCE_DIR}/src/SalomeApp
  ${PROJECT_SOURCE_DIR}/src/ShaperApp
  SALOMEDSImplAdapt
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
   ${QWT_DEFINITIONS}
  ${OpenCASCADE_DEFINITIONS}
  ${BOOST_DEFINITIONS}  
  -DDISABLE_ORB
  -DSHAPER_STANDALONE
  #-DDISABLE_PYCONSOLE
   )

IF(SALOME_USE_PYCONSOLE)
  ADD_DEFINITIONS(${PYTHON_DEFINITIONS})
ENDIF()

# ---- add static ToolsGUI in SALOMEDSImpl mode ------------------
# header files / to be processed by moc
SET(ToolsGUI_moc_HEADERS
  ${PROJECT_SOURCE_DIR}/src/TOOLSGUI/ToolsGUI_CatalogGeneratorDlg.h
 )

# header files / to install
SET(ToolsGUI_HEADERS ${ToolsGUI_moc_HEADERS} ${ToolsGUI_other_HEADERS})
# --- resources ---
# resource files / to be processed by lrelease
SET(ToolsGUI_ts_RESOURCES
  ${PROJECT_SOURCE_DIR}/src/TOOLSGUI/resources/ToolsGUI_msg_en.ts
  ${PROJECT_SOURCE_DIR}/src/TOOLSGUI/resources/ToolsGUI_msg_fr.ts
  ${PROJECT_SOURCE_DIR}/src/TOOLSGUI/resources/ToolsGUI_msg_ja.ts
)
# sources / static
SET(ToolsGUI_other_SOURCES  
  ${PROJECT_SOURCE_DIR}/src/TOOLSGUI/ToolsGUI_CatalogGeneratorDlg.cxx 
)
# sources / to compile
SET(ToolsGUI_SOURCES ${ToolsGUI_other_SOURCES} ${ToolsGUI_moc_SOURCES})

# ---- add static TOOLSDS in SALOMEDSImpl mode ------------------
# sources / static
SET(TOOLSDS_SOURCES
  ${PROJECT_SOURCE_DIR}/../KERNEL/src/TOOLSDS/SALOMEDS_Tool.cxx
  ${PROJECT_SOURCE_DIR}/../KERNEL/src/Utils/Utils_SALOME_Exception.cxx
  ${PROJECT_SOURCE_DIR}/../KERNEL/src/Utils/duplicate.cxx
 )

# ------------------------------------------------------------------

# libraries to link to
SET(_link_LIBRARIES  
  ${QT_LIBRARIES}  
  ${KERNEL_SalomeDSImpl} 
  LightApp
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
ENDIF()

# --- headers ---

# header files / to be processed by moc

SET(_moc_HEADERS ${SalomeCommon_moc_HEADERS})

IF(SALOME_USE_PYCONSOLE)
  LIST(APPEND _moc_HEADERS ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_NoteBook.h)
ENDIF()

# header files / no moc processing

SET(_other_HEADERS ${SalomeCommon_other_HEADERS}
  SalomeApp_EngineImpl.h)

IF(SALOME_USE_PYCONSOLE)
  LIST(APPEND _other_HEADERS ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_PyInterp.h)
ENDIF()

# header files / to install
SET(ShaperApp_HEADERS ${_moc_HEADERS} ${_other_HEADERS})

# --- resources ---

# resource files / to be processed by lrelease
SET(_ts_RESOURCES
  resources/ShaperApp_images.ts
  resources/ShaperApp_msg_en.ts
  resources/ShaperApp_msg_fr.ts
  resources/ShaperApp_msg_ja.ts
  resources/LightShaperApp_images.ts
  resources/LightShaperApp_msg_en.ts
  resources/LightShaperApp_msg_fr.ts
  resources/LightShaperApp_msg_ja.ts
)

# resource files / static
SET(_other_RESOURCES
  resources/ShaperApp.xml
  resources/LightShaperApp.xml
  resources/shaper.png
)
SET(SALOME_RESOURCES_FILES
  shaper.png 
)
# --- sources ---

# sources / moc wrappings
QT_WRAP_MOC(_moc_SOURCES ${_moc_HEADERS} ${ToolsGUI_moc_HEADERS})

# sources / static

SET(_other_SOURCES ${SalomeCommon_other_SOURCES}
  SalomeApp_EngineImpl.cxx)

IF(SALOME_USE_PYCONSOLE)
  LIST(APPEND _other_SOURCES ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_PyInterp.cxx
 ${PROJECT_SOURCE_DIR}/src/SalomeApp/SalomeApp_NoteBook.cxx)
ENDIF()

# adapter sources for GUI without CORBA
ADD_SUBDIRECTORY(SALOMEDSImplAdapt)

# sources / to compile
SET(ShaperApp_SOURCES ${_other_SOURCES} ${_moc_SOURCES} ${ToolsGUI_SOURCES} ${TOOLSDS_SOURCES} ${SALOMEDSImplAdapt_SOURCES})

# --- scripts ---

# scripts / bin
SET(_bin_SCRIPTS
   ${PROJECT_SOURCE_DIR}/src/SalomeApp/addvars2notebook.py
)

# scripts / python
SET(_py_SCRIPTS
   ${PROJECT_SOURCE_DIR}/src/SalomeApp/salome_pluginsmanager.py
)

# --- rules ---

ADD_LIBRARY(ShaperApp ${ShaperApp_SOURCES})
TARGET_LINK_LIBRARIES(ShaperApp ${_link_LIBRARIES})
INSTALL(TARGETS ShaperApp EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${ShaperApp_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
QT_INSTALL_TS_RESOURCES("${_ts_RESOURCES}" "${SALOME_GUI_INSTALL_RES_DATA}")

INSTALL(FILES ${_other_RESOURCES} DESTINATION ${SALOME_GUI_INSTALL_RES_DATA})

SALOME_INSTALL_SCRIPTS("${_py_SCRIPTS}"  ${SALOME_INSTALL_PYTHON})
SALOME_INSTALL_SCRIPTS("${_bin_SCRIPTS}" ${SALOME_INSTALL_SCRIPT_PYTHON})



