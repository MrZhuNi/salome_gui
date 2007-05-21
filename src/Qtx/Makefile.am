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
#  Module : suit
#  $Header$

include $(top_srcdir)/adm_local/unix/make_common_starter.am

# header files

#VSR: already migrated to Qt4 files
salomeinclude_HEADERS=		\
	Qtx.h			\
	QtxAction.h		\
	QtxActionMenuMgr.h	\
	QtxActionMgr.h		\
	QtxActionSet.h		\
	QtxActionToolMgr.h	\
	QtxColorScale.h		\
	QtxComboBox.h		\
	QtxDialog.h		\
	QtxDockAction.h		\
	QtxDockWidget.h		\
	QtxLogoMgr.h		\
	QtxMainWindow.h 	\
	QtxMap.h		\
	QtxMRUAction.h		\
	QtxResourceMgr.h	\
	QtxToolBar.h		\
	QtxWorkspace.h		\
	QtxWorkstack.h

#VSR: not yet migrated to Qt4 files
#				\
	QtxDblSpinBox.h		\
	QtxDblValidator.h	\
	QtxDirListEditor.h	\
	QtxGroupBox.h		\
	QtxIntSpinBox.h		\
	QtxListAction.h		\
	QtxListBox.h		\
	QtxListOfOperations.h	\
	QtxListResourceEdit.h	\
	QtxListView.h		\
	QtxMenuButton.h		\
	QtxOperations.h		\
	QtxParser.h		\
	QtxPathDialog.h		\
	QtxPopupMenu.h		\
	QtxPopupMgr.h		\
	QtxResourceEdit.h	\
	QtxSplash.h		\
	QtxStdOperations.h	\
	QtxTable.h		\
	QtxToolTip.h		\
	QtxWorkspaceAction.h	\
	QtxWorkstackAction.h

# Libraries targets

lib_LTLIBRARIES= libqtx.la

#VSR: already migrated to Qt4 files
dist_libqtx_la_SOURCES=		\
	Qtx.cxx			\
	QtxAction.cxx		\
	QtxActionMenuMgr.cxx	\
	QtxActionMgr.cxx	\
	QtxActionSet.cxx	\
	QtxActionToolMgr.cxx	\
	QtxColorScale.cxx	\
	QtxComboBox.cxx		\
	QtxDialog.cxx		\
	QtxDockAction.cxx	\
	QtxDockWidget.cxx	\
	QtxLogoMgr.cxx		\
	QtxMainWindow.cxx	\
	QtxMRUAction.cxx	\
	QtxResourceMgr.cxx	\
	QtxToolBar.cxx		\
	QtxWorkspace.cxx	\
	QtxWorkstack.cxx

#VSR: not yet migrated to Qt4 files
#				\
	QtxDblSpinBox.cxx	\
	QtxDblValidator.cxx	\
	QtxDirListEditor.cxx	\
	QtxGroupBox.cxx		\
	QtxIntSpinBox.cxx	\
	QtxListAction.cxx	\
	QtxListBox.cxx		\
	QtxListOfOperations.cxx	\
	QtxListResourceEdit.cxx	\
	QtxListView.cxx		\
	QtxMenuButton.cxx	\
	QtxOperations.cxx	\
	QtxParser.cxx		\
	QtxPathDialog.cxx	\
	QtxPopupMenu.cxx	\
	QtxPopupMgr.cxx		\
	QtxResourceEdit.cxx	\
	QtxSplash.cxx		\
	QtxStdOperations.cxx	\
	QtxTable.cxx		\
	QtxToolTip.cxx		\
	QtxWorkspaceAction.cxx	\
	QtxWorkstackAction.cxx

#VSR: already migrated to Qt4 files
MOC_FILES=				\
	QtxAction_moc.cxx		\
	QtxActionMenuMgr_moc.cxx	\
	QtxActionMgr_moc.cxx		\
	QtxActionSet_moc.cxx		\
	QtxActionToolMgr_moc.cxx	\
	QtxColorScale_moc.cxx		\
	QtxComboBox_moc.cxx		\
	QtxDialog_moc.cxx		\
	QtxDockAction_moc.cxx		\
	QtxDockWidget_moc.cxx		\
	QtxLogoMgr_moc.cxx		\
	QtxMainWindow_moc.cxx		\
	QtxMRUAction_moc.cxx		\
	QtxToolBar_moc.cxx		\
	QtxWorkspace_moc.cxx		\
	QtxWorkstack_moc.cxx

#VSR: not yet migrated to Qt4 files
#					\
	QtxDblSpinBox_moc.cxx		\
	QtxDblValidator_moc.cxx		\
	QtxDirListEditor_moc.cxx	\
	QtxGroupBox_moc.cxx		\
	QtxIntSpinBox_moc.cxx		\
	QtxListAction_moc.cxx		\
	QtxListBox_moc.cxx		\
	QtxListResourceEdit_moc.cxx	\
	QtxListView_moc.cxx		\
	QtxMenuButton_moc.cxx		\
	QtxPathDialog_moc.cxx		\
	QtxPopupMenu_moc.cxx		\
	QtxPopupMgr_moc.cxx		\
	QtxSplash_moc.cxx		\
	QtxTable_moc.cxx		\
	QtxToolTip_moc.cxx		\
	QtxWorkspaceAction_moc.cxx	\
	QtxWorkstackAction_moc.cxx

nodist_libqtx_la_SOURCES= $(MOC_FILES)

libqtx_la_CPPFLAGS=$(QT_INCLUDES)

libqtx_la_LDFLAGS=$(QT_MT_LIBS)
