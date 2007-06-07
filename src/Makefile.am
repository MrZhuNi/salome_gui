#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#
#
#  File   : Makefile.in
#  Author : Patrick GOLDBRONN (CEA)
#  Module : SALOME
# $Header$

# VSR: this is the original packages list
#SUBDIRS = Qtx Style DDS QDS SUIT STD CAF SUITApp LogWindow ObjBrowser Prs
# VSR: already migrated to Qt4 packages
SUBDIRS = Qtx DDS QDS SUIT SUITApp STD CAF CAM LogWindow Prs

if ENABLE_SALOMEOBJECT
  SUBDIRS += OBJECT
endif

if ENABLE_GLVIEWER
# VSR: this is the original packages list
#  SUBDIRS += GLViewer
# VSR: already migrated to Qt4 packages
  SUBDIRS += GLViewer
endif

if ENABLE_VTKVIEWER
# VSR: this is the original packages list
#  SUBDIRS += VTKViewer
# VSR: already migrated to Qt4 packages
  SUBDIRS += VTKViewer
if ENABLE_SALOMEOBJECT
# VSR: this is the original packages list
#    SUBDIRS += SVTK
# VSR: already migrated to Qt4 packages
    SUBDIRS += SVTK
endif
endif
if ENABLE_OCCVIEWER
# VSR: this is the original packages list
#  SUBDIRS += OCCViewer
# VSR: already migrated to Qt4 packages
  SUBDIRS += OCCViewer
if ENABLE_SALOMEOBJECT
# VSR: this is the original packages list
#    SUBDIRS += SOCC
# VSR: already migrated to Qt4 packages
    SUBDIRS += SOCC
endif
endif
if ENABLE_PYCONSOLE
# VSR: this is the original packages list
#  SUBDIRS += PyInterp PythonConsole
# VSR: already migrated to Qt4 packages
  SUBDIRS += PyInterp PyConsole
endif
if ENABLE_PLOT2DVIEWER
# VSR: this is the original packages list
#  SUBDIRS += Plot2d
# VSR: already migrated to Qt4 packages
  SUBDIRS += 
if ENABLE_SALOMEOBJECT
# VSR: this is the original packages list
#    SUBDIRS += SPlot2d
# VSR: already migrated to Qt4 packages
    SUBDIRS += 
endif
endif
if ENABLE_SUPERVGRAPHVIEWER
# VSR: this is the original packages list
#  SUBDIRS += SUPERVGraph
# VSR: already migrated to Qt4 packages
  SUBDIRS += 
endif
# VSR: this is the original packages list
#SUBDIRS += Event LightApp ResExporter
# VSR: already migrated to Qt4 packages
SUBDIRS += Event ResExporter

if GUI_ENABLE_CORBA
# VSR: this is the original packages list
#  SUBDIRS += RegistryDisplay TOOLSGUI \
#          Session SalomeApp SALOME_SWIG SALOME_PY SALOME_PYQT
# VSR: already migrated to Qt4 packages
  SUBDIRS += RegistryDisplay
endif
