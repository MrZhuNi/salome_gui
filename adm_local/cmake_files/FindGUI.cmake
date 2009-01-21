#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

SET(GUI_CXXFLAGS -I${GUI_ROOT_DIR}/include/salome)

FIND_LIBRARY(caf caf ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(CAM CAM ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(CASCatch CASCatch ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(DDS DDS ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(Event Event ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(GLViewer GLViewer ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(LightApp LightApp ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(LogWindow LogWindow ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(ObjBrowser ObjBrowser ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(OCCViewer OCCViewer ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(Plot2d Plot2d ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(PyConsole PyConsole ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(PyInterp PyInterp ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(QDS QDS ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(qtx qtx ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(QxScene QxScene ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeApp SalomeApp ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeIDLGUI SalomeIDLGUI ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeObject SalomeObject ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomePrs SalomePrs ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeSession SalomeSession ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeStyle SalomeStyle ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SOCC SOCC ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SPlot2d SPlot2d ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(std std ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SUITApp SUITApp ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(suit suit ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SUPERVGraph SUPERVGraph ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SVTK SVTK ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(ToolsGUI ToolsGUI ${GUI_ROOT_DIR}/lib/salome)
FIND_LIBRARY(VTKViewer VTKViewer ${GUI_ROOT_DIR}/lib/salome)