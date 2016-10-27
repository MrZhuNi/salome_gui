# -*- coding: iso-8859-1 -*-
# Copyright (C) 2010-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
# Author : Gilles DAVID (EDF)

try:
    from PyQt4 import uic
except ImportError:
    from PyQt5 import uic

try:
    from qtsalome import Qt, QDialog, QTableWidgetItem, QMessageBox
except ImportError:
    from PyQt4.Qt import QDialog, QTableWidgetItem, QMessageBox
    from PyQt4.QtCore import Qt

import os
import salome_pluginsmanager  # @UnresolvedImport


__dialog = None


class SalomePluginManagerDialog(QDialog):
    '''
    GUI to manage the python plugins from all the modules.
    The list of plugins must be updated when a module is loaded for the
    first time.
    A double click on a row enables or disables the selected plugin.
    '''

    def __init__(self, name, context):
        QDialog.__init__(self)
        self.setWindowFlags(Qt.WindowStaysOnTopHint)
        self.name = name
        self.context = context
        self.directory = os.path.dirname(os.path.abspath(__file__))
        uic.loadUi(os.path.join(self.directory,
                                "salome_pluginsmanager_dialog.ui"),
                   self)
        self.updatePluginsList()

    def getPluginsManager(self, module=None):
        if module is not None:
            pluginslist = salome_pluginsmanager.plugins[module]
            if isinstance(pluginslist, list):
                return pluginslist[0]
            elif isinstance(pluginslist, dict):
                return pluginslist.values()[0]
        return salome_pluginsmanager.current_plugins_manager

    def updatePluginsList(self, row=None):
        self.plugins_list.clear()
        self.plugins_list.setRowCount(0)
        self.plugins_list.setHorizontalHeaderLabels(['Module', 'Plugin',
                                                     'Activated', 'Description'])
        current_plugins_manager = self.getPluginsManager(self.name)
        if current_plugins_manager is None:
            QMessageBox.warning(None, "Exception occured",
                                'current_plugins_manager is None')
            return

        for modulename in sorted(salome_pluginsmanager.plugins):
            value = salome_pluginsmanager.plugins[modulename]
            if isinstance(value, list):
                # Not a module plugin: pure salome one
                self.addrows(modulename, value)
            elif isinstance(value, dict):
                # Module plugin
                print(value.values())
                self.addrows(modulename, value.values())
        if row is not None:
            self.plugins_list.setCurrentIndex(self.plugins_list.model()
                                              .index(row, 0))

    def addrows(self, module, pluginmanagerlist):
        for pluginmanager in pluginmanagerlist:
            for pluginname, pluginval in pluginmanager.registry.items():
                if pluginname == "Python Plugins Manager":
                    continue
                _, description, isActivated = pluginval
                module_item = QTableWidgetItem(module)
                plugin_item = QTableWidgetItem(pluginname)
                activated_item = QTableWidgetItem(str(isActivated))
                description_item = QTableWidgetItem(description)
                row = self.plugins_list.rowCount()
                self.plugins_list.insertRow(self.plugins_list.rowCount())
                self.plugins_list.setItem(row, 0, module_item)
                self.plugins_list.setItem(row, 1, plugin_item)
                self.plugins_list.setItem(row, 2, activated_item)
                self.plugins_list.setItem(row, 3, description_item)

    def createPlugin(self):
        pass

    def importPlugin(self):
        pass

    def togglePluginStatus(self):
        currentRow = self.plugins_list.currentRow()
        if currentRow < 0:
            return
        currentModule = str(self.plugins_list.item(currentRow, 0).text())
        currentPlugin = str(self.plugins_list.item(currentRow, 1).text())
        current_plugins_manager = self.getPluginsManager(currentModule)
        current_plugins_manager.toggleStatus(currentPlugin)
        self.updatePluginsList(currentRow)


def getDialog(name, context):
    """
    This function returns a singleton instance of the plugin dialog.
    It is mandatory in order to call show without a parent ...
    """
    global __dialog
    if __dialog is None:
        __dialog = SalomePluginManagerDialog(name, context)
    else:
        __dialog.name = name
        __dialog.context = context
        __dialog.updatePluginsList()
    return __dialog
