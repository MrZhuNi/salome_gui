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
#  Author : Vladimir Klyachin (OCN)
#  Module : caf
#  $Header$

include $(top_srcdir)/adm_local/unix/make_common_starter.am

lib_LTLIBRARIES = libGraphicsView.la

salomeinclude_HEADERS =		\
GraphicsView_Defs.h	\
GraphicsView.h	\
GraphicsView_Object.h	\
GraphicsView_Scene.h	\
GraphicsView_Selector.h	\
GraphicsView_Viewer.h	\
GraphicsView_ViewFrame.h	\
GraphicsView_ViewManager.h	\
GraphicsView_ViewPort.h	\
GraphicsView_ViewTransformer.h \
GraphicsView_PrsImage.h \
GraphicsView_PrsImageFrame.h \
GraphicsView_PrsPropDlg.h

dist_libGraphicsView_la_SOURCES =		\
GraphicsView_Object.cxx	\
GraphicsView_Scene.cxx	\
GraphicsView_Selector.cxx	\
GraphicsView_Viewer.cxx	\
GraphicsView_ViewFrame.cxx	\
GraphicsView_ViewManager.cxx	\
GraphicsView_ViewPort.cxx	\
GraphicsView_ViewTransformer.cxx \
GraphicsView_PrsImage.cxx \
GraphicsView_PrsImageFrame.cxx \
GraphicsView_PrsPropDlg.cxx

MOC_FILES =				\
GraphicsView_ViewManager_moc.cxx \
GraphicsView_Viewer_moc.cxx \
GraphicsView_ViewFrame_moc.cxx \
GraphicsView_Selector_moc.cxx \
GraphicsView_ViewPort_moc.cxx \
GraphicsView_Scene_moc.cxx \
GraphicsView_PrsPropDlg_moc.cxx

nodist_libGraphicsView_la_SOURCES = $(MOC_FILES)

dist_salomeres_DATA =			\
resources/graphics_view_cursor_zoom.png	\
resources/graphics_view_dump.png	\
resources/graphics_view_fitall.png	\
resources/graphics_view_fitarea.png	\
resources/graphics_view_fitselect.png	\
resources/graphics_view_glpan.png	\
resources/graphics_view_pan.png	\
resources/graphics_view_reset.png	\
resources/graphics_view_rotate.png	\
resources/graphics_view_zoom.png

nodist_salomeres_DATA =		\
	GraphicsView_images.qm	\
	GraphicsView_msg_en.qm	\
	GraphicsView_msg_fr.qm

libGraphicsView_la_CPPFLAGS = $(QT_INCLUDES) -I$(srcdir)/../SUIT -I$(srcdir)/../Qtx
libGraphicsView_la_LDFLAGS  = $(QT_MT_LIBS)
libGraphicsView_la_LIBADD   = ../SUIT/libsuit.la
