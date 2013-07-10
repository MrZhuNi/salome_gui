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

include $(top_srcdir)/adm_local/unix/make_common_starter.am

lib_LTLIBRARIES = libImageComposer.la

salomeinclude_HEADERS =			\
ImageComposer.h				\
ImageComposer_ColorMaskOperator.h	\
ImageComposer_CropOperator.h		\
ImageComposer_CutOperator.h		\
ImageComposer_FuseOperator.h		\
ImageComposer_Image.h			\
ImageComposer_Operator.h		\
ImageComposer_Tools.h

dist_libImageComposer_la_SOURCES =	\
ImageComposer_ColorMaskOperator.cxx	\
ImageComposer_CropOperator.cxx		\
ImageComposer_CutOperator.cxx		\
ImageComposer_FuseOperator.cxx		\
ImageComposer_Image.cxx			\
ImageComposer_Operator.cxx		\
ImageComposer_Tools.cxx

libImageComposer_la_CPPFLAGS = $(QT_INCLUDES)
libImageComposer_la_LDFLAGS  = $(QT_MT_LIBS)
