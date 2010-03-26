"""
This module is imported from C++ SalomeApp_Application
and initialized (call to initialize function)

A plugins manager is created at import (singleton object).

The plugins manager creates a submenu Plugins in the Tools menu.

The plugins manager searches in $HOME/.salome/Plugins, $HOME/$APPLI/Plugins, $SALOME_PLUGINS_PATH directories
files named salome_plugins.py and executes them.

These files should contain python code that register functions into the plugins manager.

salome_plugins.py example::

  import salome_pluginsmanager

  def about(context):
    from PyQt4.QtGui import QMessageBox
    QMessageBox.about(None, "About SALOME pluginmanager", "SALOME plugins manager in SALOME virtual application ")

  salome_pluginsmanager.AddFunction('Mesh Box','Create a box and mesh',mesh)

First import the python module salome_pluginsmanager
Second write a function with one argument context (it's an object with 3 attributes)
Third register the function with a call to AddFunction (entry in menu plugins, tooltip, function)

context attributes:
- sg : the SALOME Swig interface
- studyId : the SALOME studyId that must be used to execute the plugin
- study : the SALOME study object that must be used to execute the plugin
"""

import os,sys,traceback
from PyQt4 import QtGui
from PyQt4 import QtCore

import salome

SEP=":"

# Get SALOME PyQt interface
import SalomePyQt
sgPyQt = SalomePyQt.SalomePyQt()

# Get SALOME Swig interface
import libSALOME_Swig
sg = libSALOME_Swig.SALOMEGUI_Swig()

def initialize():
  plugins.activate()

class Context:
    def __init__(self,sg):
        self.sg=sg
        self.studyId=salome.sg.getActiveStudyId()
        self.study= salome.myStudyManager.GetStudyByID(self.studyId)

class Plugins:
    def __init__(self):
        self.registry={}
        self.handlers={}
        self.lasttime=0
        self.plugindirs=[]
        self.plugins_files=[]

        # USER plugins directory
        user_dir = os.path.expanduser("~/.salome/Plugins")
        self.plugindirs.append(user_dir)

        # APPLI plugins directory
        appli=os.getenv("APPLI")
        if appli:
          appli_dir=os.path.join(os.path.expanduser("~"),appli,"Plugins")
          self.plugindirs.append(appli_dir)

        #SALOME_PLUGINS_PATH environment variable (list of directories separated by ":")
        pluginspath=os.getenv("SALOME_PLUGINS_PATH")
        if pluginspath:
          for directory in pluginspath.split(SEP):
            self.plugindirs.append(directory)

        self.menu=QtGui.QMenu("Plugins",sgPyQt.getPopupMenu(SalomePyQt.Tools))
        self.menu.connect(self.menu,QtCore.SIGNAL("aboutToShow()"),self.importPlugins)

    def AddFunction(self,name,description,script,script_type=None):
        """ Add a plugin function
        """
        self.registry[name]=script,description,script_type

        def handler(obj=self,script=script):
          try:
            script(Context(sgPyQt))
          except:
            s=traceback.format_exc()
            QtGui.QMessageBox.warning(None,"Exception occured",s)

        self.handlers[name]=handler

    def importPlugins(self):
        """Import the salome_plugins module that contains plugins definition """
        if self.lasttime ==0:
          studyId=sg.getActiveStudyId()
          if studyId == 0:
             self.menu.clear()
             self.menu.addAction("No active study : open or load a new one")
             return
          else:
             salome.salome_init(embedded=1)

        lasttime=0

        plugins_files=[]
        for directory in self.plugindirs:
          salome_plugins_file = os.path.join(directory,"salome_plugins.py")
          if os.path.isfile(salome_plugins_file):
            plugins_files.append((directory,salome_plugins_file))
            lasttime=max(lasttime,os.path.getmtime(salome_plugins_file))

        plugins_files.sort()

        if not plugins_files:
          self.registry.clear()
          self.handlers.clear()
          self.lasttime=0
          self.menu.clear()
          return

        if self.plugins_files != plugins_files or lasttime > self.lasttime:
          self.registry.clear()
          self.handlers.clear()
          self.lasttime=lasttime
          for directory,salome_plugins_file in plugins_files:
            if directory not in sys.path:
              sys.path.insert(0,directory)
            execfile(salome_plugins_file,globals(),{})
          self.updateMenu()

    def updateMenu(self):
        """Update the Plugins menu"""
        self.menu.clear()
        for name,handler in self.handlers.items():
            act=self.menu.addAction(name,handler)

    def activate(self):
        """Add the Plugins menu to the Tools menu"""
        tools_menu= sgPyQt.getPopupMenu(SalomePyQt.Tools)
        tools_menu.addMenu(self.menu)

    def deactivate(self):
        """Remove the Plugins menu from Tools menu (not implemented)"""

plugins=Plugins()

def AddFunction(name,description,script,script_type=None):
   """ Add a plugin function
       Called by a user to register a function (script)
   """
   return plugins.AddFunction(name,description,script,script_type)
