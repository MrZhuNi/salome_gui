# Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

# VTK detection for Salome

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
