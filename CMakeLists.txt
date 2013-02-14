# Copyright (C) 2012  CEA/DEN, EDF R&D, OPEN CASCADE
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
PROJECT(SALOME_GUI)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)

IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

IF(APPLE)
  SET(CMAKE_C_COMPILER gcc)
  SET(CMAKE_CXX_COMPILER g++)
  # because default is clang(llvm) with mountain lion at least
ENDIF(APPLE)

# Temporary definitions
SET(DOXYGEN_IS_OK 0)
IF(WINDOWS)
 SET(CPPUNIT_IS_OK 0)
ENDIF(WINDOWS)

#Defining all options first
option(SALOME_USE_MPI "Use MPI containers" OFF)
option(SALOME_USE_CORBA "Use distribute application with components" ON)
option(SALOME_USE_DOC "Generate documentation" ON)
option(SALOME_USE_TEST "Generate test" OFF)
OPTION(SALOME_USE_VTKVIEWER "Enable VTK visualization (Mandatory in classic configurations)" ON)
OPTION(SALOME_USE_SALOMEOBJECT "Enable Salome Object (Mandatory in classic configurations)" ON)
OPTION(SALOME_USE_OCCVIEWER "Enable OCC visualization (Mandatory in classic configurations)" ON)
OPTION(SALOME_USE_GLVIEWER "Enable OpenGL visualization (Mandatory in classic configurations)" ON)
OPTION(SALOME_USE_PLOT2DVIEWER "Enable Plot2D visualization (Mandatory in classic configurations)" ON)
OPTION(SALOME_USE_PYCONSOLE "Enable Python GUI interface (Mandatory in classic configurations)" ON)
OPTION(SALOME_USE_SUPERVGRAPHVIEWER "Enable Supervision visualization (Mandatory in classic configurations)" ON)
OPTION(SALOME_USE_QXGRAPHVIEWER "Enable QX graph visualization (Mandatory in classic configurations)" ON)

ENABLE_TESTING()
SET(VERSION "6.6.0")
SET(VERSION_DEV "1")

SET(BUILD_SHARED_LIBS true)

SET(KERNEL_ROOT_DIR $ENV{KERNEL_ROOT_DIR} CACHE PATH "Path to the Salome KERNEL")
IF(EXISTS ${KERNEL_ROOT_DIR})
  INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindKERNEL.cmake)
ELSE(EXISTS ${KERNEL_ROOT_DIR})
  MESSAGE(FATAL_ERROR "We absolutely need a Salome KERNEL, please define KERNEL_ROOT_DIR")
ENDIF(EXISTS ${KERNEL_ROOT_DIR})

STRING(REPLACE "\\" "/" KERNEL_ROOT_DIR ${KERNEL_ROOT_DIR})
INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindPLATFORM.cmake)


# Threads
SET(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)


# Python
set(PYTHON_ROOT_DIR $ENV{PYTHON_ROOT_DIR} CACHE PATH "Path to Python directory")
if(EXISTS ${PYTHON_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${PYTHON_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${PYTHON_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${PYTHON_ROOT_DIR}/bin)
endif(EXISTS ${PYTHON_ROOT_DIR})
find_package(PythonInterp REQUIRED)
# Set PythonLibs_FIND_VERSION To avoid problems when several versions are in the system
SET(PythonLibs_FIND_VERSION ${PYTHON_VERSION_STRING})
find_package(PythonLibs REQUIRED)
set(PYLOGLEVEL WARNING)

# Swig
set(SWIG_ROOT_DIR $ENV{SWIG_ROOT_DIR} CACHE PATH "Path to Swig directory")
if(EXISTS ${SWIG_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${SWIG_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${SWIG_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${SWIG_ROOT_DIR}/bin)
endif(EXISTS ${SWIG_ROOT_DIR})
find_package(SWIG REQUIRED)

# LIBXML2
set(LIBXML2_ROOT_DIR $ENV{LIBXML2_ROOT_DIR} CACHE PATH "Path to LibXml2 directory")
if(EXISTS ${LIBXML2_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${LIBXML2_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${LIBXML2_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${LIBXML2_ROOT_DIR}/bin)
endif(EXISTS ${LIBXML2_ROOT_DIR})
find_package(LibXml2 REQUIRED)
if(${LIBXML2_VERSION_STRING} VERSION_LESS "2.7.6")
  message(FATAL_ERROR "LIbXml2 required version is 2.7.6, version is ${LIBXML2_VERSION_STRING} , please defined LIBXML2_ROOT_DIR as environment or cmake variable")
endif(${LIBXML2_VERSION_STRING} VERSION_LESS "2.7.6")


# HDF5
set(HDF5_ROOT_DIR $ENV{HDF5_ROOT_DIR} CACHE PATH "Path to Hdf5 directory")
if(EXISTS ${HDF5_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${HDF5_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${HDF5_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${HDF5_ROOT_DIR}/bin)
endif(EXISTS ${HDF5_ROOT_DIR})
find_package(HDF5 COMPONENTS C REQUIRED)
if (HDF5_FOUND)
  set(HDF5_DEFINITIONS "-DH5_USE_16_API ${HDF5_DEFINITIONS}" )
  if(WINDOWS)
    set(HDF5_DEFINITIONS "-D_HDF5USEDLL_ ${HDF5_DEFINITIONS}" )
  endif(WINDOWS)
endif(HDF5_FOUND)

# MPI
if(SALOME_USE_MPI OR HDF5_IS_PARALLEL)
  set(MPI_ROOT_DIR $ENV{MPI_ROOT_DIR} CACHE PATH "Path to Mpi directory")
  if(EXISTS ${MPI_ROOT_DIR})
    set(CMAKE_INCLUDE_PATH ${MPI_ROOT_DIR}/include)
    set(CMAKE_LIBRARY_PATH ${MPI_ROOT_DIR}/lib)
    set(CMAKE_PROGRAM_PATH ${MPI_ROOT_DIR}/bin)
  endif(EXISTS ${MPI_ROOT_DIR})
  find_package(MPI REQUIRED)
  if(HDF5_IS_PARALLEL)
    set(HDF5_INCLUDE_DIRS ${HDF5_INCLUDE_DIRS} ${MPI_C_INCLUDE_PATH} ${MPI_CXX_INCLUDE_PATH} )
    set(HDF5_LIBRARIES ${HDF5_LIBRARIES} ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES})
  endif(HDF5_IS_PARALLEL)
endif(SALOME_USE_MPI OR HDF5_IS_PARALLEL)

# Omniorb
if(SALOME_USE_CORBA)
  INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindOMNIORB.cmake)
endif(SALOME_USE_CORBA)

# Boost
set(Boost_ROOT_DIR $ENV{Boost_ROOT_DIR} CACHE PATH "Path to Boost directory")
if(EXISTS ${Boost_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${Boost_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${Boost_ROOT_DIR}/lib)
endif(EXISTS ${Boost_ROOT_DIR})
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
#find_package( Boost 1.49 REQUIRED  )#COMPONENTS thread date_time signals filesystem system)
find_package( Boost REQUIRED  )#COMPONENTS thread date_time signals filesystem system)
if(WINDOWS)
  set(BOOST_DEFINITIONS -DBOOST_DISABLE_ASSERTS)
endif(WINDOWS)

# CPPUNIT
if(SALOME_USE_TEST)
  set(CPPUNIT_ROOT_DIR $ENV{CPPUNIT_ROOT_DIR} CACHE PATH "Path to Cppunit directory")
  if(EXISTS ${CPPUNIT_ROOT_DIR})
    set(CMAKE_INCLUDE_PATH ${CPPUNIT_ROOT_DIR}/include)
    set(CMAKE_LIBRARY_PATH ${CPPUNIT_ROOT_DIR}/lib)
    set(CMAKE_PROGRAM_PATH ${CPPUNIT_ROOT_DIR}/bin)
  endif(EXISTS ${CPPUNIT_ROOT_DIR})
  INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindCPPUNIT.cmake)
endif(SALOME_USE_TEST)

# Doxygen and graphviz
if(SALOME_USE_DOC)
  set(DOXYGEN_ROOT_DIR $ENV{DOXYGEN_ROOT_DIR} CACHE PATH "Path to Doxygen directory")
  set(GRAPHVIZ_ROOT_DIR $ENV{GRAPHVIZ_ROOT_DIR} CACHE PATH "Path to Graphviz directory")
  if(EXISTS ${DOXYGEN_ROOT_DIR})
    set(CMAKE_PROGRAM_PATH ${DOXYGEN_ROOT_DIR}/bin)
  endif(EXISTS ${DOXYGEN_ROOT_DIR})
  if(EXISTS ${GRAPHVIZ_ROOT_DIR})
    set(CMAKE_PROGRAM_PATH ${GRAPHVIZ_ROOT_DIR}/bin)
    if(EXISTS ${DOXYGEN_ROOT_DIR})
      set(CMAKE_PROGRAM_PATH ${GRAPHVIZ_ROOT_DIR}/bin ${DOXYGEN_ROOT_DIR}/bin)
    endif(EXISTS ${DOXYGEN_ROOT_DIR})
  endif(EXISTS ${GRAPHVIZ_ROOT_DIR})
  find_package(Doxygen)
endif(SALOME_USE_DOC)

# Sphinx
if(SALOME_USE_DOC)
  set(SPHINX_ROOT_DIR $ENV{SPHINX_ROOT_DIR} CACHE PATH "Path to Sphinx directory")
  if(EXISTS ${SPHINX_ROOT_DIR})
    set(CMAKE_PROGRAM_PATH ${SPHINX_ROOT_DIR}/bin)
  endif(EXISTS ${SPHINX_ROOT_DIR})
  INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindSPHINX.cmake)
endif(SALOME_USE_DOC)

# Qt 4
#set(QT_MIN_VERSION 4.8.0)
set(QT_ROOT_DIR $ENV{QT_ROOT_DIR} CACHE PATH "Path to Qt directory")
if(EXISTS ${QT_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${QT_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${QT_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${QT_ROOT_DIR}/bin)
endif(EXISTS ${QT_ROOT_DIR})
find_package(Qt4 REQUIRED QtCore QtGui QtXml QtWebKit QtOpenGL)
IF(NOT QT4_FOUND)
  MESSAGE(FATAL_ERROR "Qt4 not found, please set QT_ROOT_DIR environment or cmake variable")
ELSE(NOT QT4_FOUND})
  INCLUDE(${QT_USE_FILE})
ENDIF(NOT QT4_FOUND)

# OpenCascade
if(SALOME_USE_OCCVIEWER)
  set(CAS_ROOT_DIR $ENV{CAS_ROOT_DIR} CACHE PATH "Path to OpenCascade directory")
  if(EXISTS ${CAS_ROOT_DIR})
    set(CMAKE_INCLUDE_PATH ${CAS_ROOT_DIR}/inc)
    set(CMAKE_PROGRAM_PATH ${CAS_ROOT_DIR}/bin)
    if(WINDOWS)
      if(CMAKE_BUILD_TYPE STREQUAL Debug)
        set(CMAKE_LIBRARY_PATH ${CAS_ROOT}/win32/libd)
      else(CMAKE_BUILD_TYPE STREQUAL Debug)
        set(CMAKE_LIBRARY_DIR ${CAS_ROOT}/win32/lib)
      endif(CMAKE_BUILD_TYPE STREQUAL Debug)
    else(WINDOWS)
      set(CMAKE_LIBRARY_PATH ${CAS_ROOT_DIR}/lib)
    endif(WINDOWS)
  endif(EXISTS ${CAS_ROOT_DIR})
  INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindCAS.cmake)
endif(SALOME_USE_OCCVIEWER)

# OpenGL
if(SALOME_USE_GLVIEWER)
  find_package(OpenGL REQUIRED)
endif(SALOME_USE_GLVIEWER)

# VTK
if(SALOME_USE_VTKVIEWER)
  set(PARAVIEW_ROOT_DIR $ENV{PARAVIEW_ROOT_DIR} CACHE PATH "Path to ParaView directory")
  set(PARAVIEW_VERSION $ENV{PARAVIEW_VERSION} CACHE STRING "Version of Paraview")
  if(EXISTS ${PARAVIEW_ROOT_DIR})
    set(VTK_DIR ${PARAVIEW_ROOT_DIR}/lib/paraview-${PARAVIEW_VERSION} CACHE PATH "Path to directory including VTKConfig.cmake")
  endif(EXISTS ${PARAVIEW_ROOT_DIR})
  find_package(VTK REQUIRED)
  if (VTK_FOUND)
    include(${VTK_USE_FILE})
    message(STATUS "VTK version is ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}")
    message(STATUS "Kit VTK availables : ${VTK_KITS}")
    message(STATUS "VTK Library dir is : ${VTK_LIBRARY_DIRS}")
    set(CMAKE_LIBRARY_PATH ${VTK_LIBRARY_DIRS})
    set(VTK_LIBSWENEED vtkCommon vtkGraphics vtkImaging vtkFiltering vtkIO vtkRendering vtkHybrid vtkParallel vtkWidgets vtkInfovis vtkVolumeRendering)
    unset(VTK_LIBRARIES)
    foreach(VTK_LIB_NEEDED ${VTK_LIBSWENEED})
      unset(VTK_LIB CACHE)
      find_library(VTK_LIB ${VTK_LIB_NEEDED})
      set(VTK_LIBRARIES ${VTK_LIBRARIES} ${VTK_LIB})
    endforeach(VTK_LIB_NEEDED)
    find_library(vtkCommonPythonD vtkCommonPythonD)
    find_library(vtkGraphicsPythonD vtkGraphicsPythonD)
    find_library(vtkImagingPythonD vtkImagingPythonD)
    find_library(vtkPythonCore vtkPythonCore)
  endif(VTK_FOUND)
endif(SALOME_USE_VTKVIEWER)

# QWT
if(SALOME_USE_PLOT2DVIEWER)
  set(QWT_ROOT_DIR $ENV{QWT_ROOT_DIR} CACHE PATH "Path to Qwt directory")
  if(EXISTS ${QWT_ROOT_DIR})
    set(CMAKE_INCLUDE_PATH ${QWT_ROOT_DIR}/include)
    set(CMAKE_LIBRARY_PATH ${QWT_ROOT_DIR}/lib)
    set(CMAKE_PROGRAM_PATH ${QWT_ROOT_DIR}/bin)
  endif(EXISTS ${QWT_ROOT_DIR})
  INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindQWT.cmake)
endif(SALOME_USE_PLOT2DVIEWER)

# Sip
set(SIP_ROOT_DIR $ENV{SIP_ROOT_DIR} CACHE PATH "Path to Sip directory")
if(EXISTS ${SIP_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${SIP_ROOT_DIR}/include/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} ${SIP_ROOT_DIR}/include)
  set(CMAKE_PROGRAM_PATH ${SIP_ROOT_DIR}/bin)
endif(EXISTS ${SIP_ROOT_DIR})
find_program(SIP_EXECUTABLE sip REQUIRED)
find_path(SIP_INCLUDE_DIR sip.h REQUIRED HINTS /usr/include/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})

# PyQt4
set(PYQT_ROOT_DIR $ENV{PYQT_ROOT_DIR} CACHE PATH "Path to PyQt4 directory")
if(EXISTS ${PYQT_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${PYQT_ROOT_DIR}/share/sip ${PYQT_ROOT_DIR}/sip)
  set(CMAKE_PROGRAM_PATH ${PYQT_ROOT_DIR}/bin)
endif(EXISTS ${PYQT_ROOT_DIR})
FIND_PROGRAM(PYQT_PYUIC_EXECUTABLE NAMES pyuic4 pyuic4.bat)
FIND_PATH(PYQT_SIPS_DIR NAMES QtCore QtGui QtXml QtOpenGL QtAssistant QtDesigner QtNetwork QtSql QtSvg QtTest HINTS /usr/share/sip/PyQt4)
SET(PYQT_PYTHONPATH "")
if(EXISTS ${PYQT_ROOT_DIR})
  set(PYQT_PYTHONPATH ${PYQT_ROOT_DIR}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages)
endif(EXISTS ${PYQT_ROOT_DIR})
EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; sys.path[:0] = ['${PYQT_PYTHONPATH}']; from PyQt4 import pyqtconfig; sys.stdout.write(pyqtconfig.Configuration().pyqt_sip_flags)"
  OUTPUT_VARIABLE PYQT_SIPFLAGS)
SEPARATE_ARGUMENTS(PYQT_SIPFLAGS)
SET(PYQT_SIPFLAGS ${PYQT_SIPFLAGS} -s .cc -c .
  -I ${PYQT_SIPS_DIR}
  -I ${PYQT_SIPS_DIR}/QtCore -I ${PYQT_SIPS_DIR}/QtGui  -I ${PYQT_SIPS_DIR}/QtXml -I ${PYQT_SIPS_DIR}/QtOpenGL 
  -I ${PYQT_SIPS_DIR}/QtAssistant  -I ${PYQT_SIPS_DIR}/QtDesigner -I ${PYQT_SIPS_DIR}/QtNetwork -I ${PYQT_SIPS_DIR}/QtSql 
  -I ${PYQT_SIPS_DIR}/QtSvg -I ${PYQT_SIPS_DIR}/QtTest )

EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; t=sys.argv[-1].split(\".\") ; t[:]=(int(elt) for elt in t) ; sys.stdout.write(\"0x%02x%02x%02x\"%tuple(t))" ${VERSION}
  OUTPUT_VARIABLE XVERSION)

SET(GUI_salomebin_BINS bin/salome)

SET(GUI_salomelib_LIBS lib/salome)
SET(GUI_paraviewlib_LIBS lib/paraview)

SET(GUI_salomeinclude_HEADERS include/salome)

SET(GUI_salomeres_DATA share/salome/resources/gui)

SET(GUI_salomeres_SCRIPTS share/salome/resources/gui)

SET(GUI_salomepluginsdir share/salome/plugins/gui)

SET(GUI_salomescript_SCRIPTS bin/salome)

SET(GUI_salomescript_DATA bin/salome)

SET(GUI_salomescript_PYTHON bin/salome)
SET(KERNEL_salomescript_PYTHON ${GUI_salomescript_PYTHON})

SET(GUI_appliskel_SCRIPTS bin/salome/appliskel)

SET(GUI_appliskel_PYTHON bin/salome/appliskel)

SET(GUI_pythondir lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages)

SET(GUI_salomepythondir ${GUI_pythondir}/salome)
SET(KERNEL_salomepythondir ${GUI_salomepythondir})

SET(GUI_sharedpkgpython_PYTHON ${GUI_salomepythondir}/shared_modules)

SET(GUI_admlocalcmake adm_local/cmake_files)

SET(IDLDIR)                        
IF(SALOME_USE_CORBA)
  SET(IDLDIR idl)
ENDIF(SALOME_USE_CORBA)

SET(SUBDIRS
  ${IDLDIR}
  adm_local
  resources
  src
  doc
  bin
  tools
)
FOREACH(dir ${SUBDIRS})
  ADD_SUBDIRECTORY(${dir})
ENDFOREACH(dir ${SUBDIRS})

SET(input ${CMAKE_CURRENT_SOURCE_DIR}/GUI_version.h.in)
SET(output ${CMAKE_CURRENT_BINARY_DIR}/GUI_version.h)
MESSAGE(STATUS "Creation of ${output}")
CONFIGURE_FILE(${input} ${output} @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/GUI_version.h DESTINATION ${GUI_salomeinclude_HEADERS})
