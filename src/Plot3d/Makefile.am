#  Copyright (C) 2005  CEA/DEN, EDF R&D, OPEN CASCADE, PRINCIPIA R&D
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
#  File   : Makefile.in
#  Author : Oleg Uvarov (OCN)
#  Module : Plot3d
#  $Header: 

include $(top_srcdir)/adm_local/unix/make_common_starter.am

lib_LTLIBRARIES = libPlot3d.la

salomeinclude_HEADERS= \
	Plot3d.h \
	Plot3d_Actor.h \
	Plot3d_ColorDic.h \
	Plot3d_FitDataDlg.h \
	Plot3d_SetupColorScaleDlg.h \
	Plot3d_SetupSpecificColorScaleDlg.h \
	Plot3d_SetupSurfacesDlg.h \
	Plot3d_ViewManager.h \
	Plot3d_ViewModel.h \
	Plot3d_ViewWindow.h \
	SALOME_ExtractGeometry.h

dist_libPlot3d_la_SOURCES= \
	Plot3d_Actor.cxx \
	Plot3d_ColorDic.cxx \
	Plot3d_FitDataDlg.cxx \
	Plot3d_SetupColorScaleDlg.cxx \
	Plot3d_SetupSpecificColorScaleDlg.cxx \
	Plot3d_SetupSurfacesDlg.cxx \
	Plot3d_ViewManager.cxx \
	Plot3d_ViewModel.cxx \
	Plot3d_ViewWindow.cxx \
	SALOME_ExtractGeometry.cxx

MOC_FILES= \
	Plot3d_FitDataDlg_moc.cxx \
	Plot3d_SetupColorScaleDlg_moc.cxx \
	Plot3d_SetupSpecificColorScaleDlg_moc.cxx \
	Plot3d_SetupSurfacesDlg_moc.cxx \
	Plot3d_ViewManager_moc.cxx \
	Plot3d_ViewModel_moc.cxx \
	Plot3d_ViewWindow_moc.cxx
nodist_libPlot3d_la_SOURCES= $(MOC_FILES)

dist_salomeres_DATA=\
	resources/plot3d_merge_scalar_bars.png \
	resources/plot3d_minus.png \
	resources/plot3d_mode_2d.png \
	resources/plot3d_surfaces_settings.png

nodist_salomeres_DATA =	\
	Plot3d_msg_en.qm	\
	Plot3d_images.qm

libPlot3d_la_CPPFLAGS =		\
	$(QT_INCLUDES)		\
	$(CAS_CPPFLAGS)		\
	$(VTK_INCLUDES)		\
	$(BOOST_CPPFLAGS)		\
	-I$(srcdir)/../Qtx	\
	-I$(srcdir)/../SUIT	\
	-I$(srcdir)/../OBJECT	\
	-I$(srcdir)/../Prs 	\
	-I$(srcdir)/../VTKViewer \
	-I$(srcdir)/../SVTK

libPlot3d_la_LDFLAGS =			\
	$(QT_MT_LIBS)			\
	$(CAS_KERNEL) $(CAS_VIEWER) -lTKCDF \
	$(VTK_LIBS) $(OGL_LIBS)

libPlot3d_la_LIBADD = ../Qtx/libqtx.la ../SUIT/libsuit.la ../OBJECT/libSalomeObject.la \
		    ../Prs/libSalomePrs.la ../VTKViewer/libVTKViewer.la ../SVTK/libSVTK.la
