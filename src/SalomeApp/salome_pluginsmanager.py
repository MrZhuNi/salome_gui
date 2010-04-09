"""
This module is imported from C++ SalomeApp_Application
and initialized (call to initialize function with 4 parameters)
module : 0 if it's plugins manager at the application level 1 if it is at the module level
name : the name of the plugins manager. This name is used to build the name of the plugins files
basemenuname : the name of the menu into we want to add the menu of the plugins ("Tools" for example)
menuname : the name of plugins menu

A plugins manager is created at when calling initialize.

The plugins manager creates a submenu <menuname> in the <basemenuname> menu.

The plugins manager searches in $HOME/.salome/Plugins, $HOME/$APPLI/Plugins, $SALOME_PLUGINS_PATH directories
files named <name>_plugins.py and executes them.

These files should contain python code that register functions into the plugins manager.

Example of a plugins manager with name salome. It searches files with name salome_plugins.py (example follows)::

  import salome_pluginsmanager

  def about(context):
    from PyQt4.QtGui import QMessageBox
    QMessageBox.about(None, "About SALOME pluginmanager", "SALOME plugins manager in SALOME virtual application ")

  salome_pluginsmanager.AddFunction('About plugins','About SALOME pluginmanager',about)

First you need to import the python module salome_pluginsmanager
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
if sys.platform == "win32":
  SEP = ";"

# Get SALOME PyQt interface
import SalomePyQt
sgPyQt = SalomePyQt.SalomePyQt()

# Get SALOME Swig interface
import libSALOME_Swig
sg = libSALOME_Swig.SALOMEGUI_Swig()

plugins={}
current_plugins_manager=None

def initialize(module,name,basemenuname,menuname):
  if plugins.has_key(name):return
  plugins[name]=PluginsManager(module,name,basemenuname,menuname)
  plugins[name].activate()

class Context:
    def __init__(self,sg):
        self.sg=sg
        self.studyId=salome.sg.getActiveStudyId()
        self.study= salome.myStudyManager.GetStudyByID(self.studyId)

class PluginsManager:
    def __init__(self,module,name,basemenuname,menuname):
        self.name=name
        self.basemenuname=basemenuname
        self.menuname=menuname
        self.module=module
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

        if self.module:
          self.menu=QtGui.QMenu(self.menuname)
          mid=sgPyQt.createMenu(self.menu.menuAction(),self.basemenuname)
        else:
          self.basemenu=sgPyQt.getPopupMenu(self.basemenuname)
          self.menu=QtGui.QMenu(self.menuname,self.basemenu)
          self.basemenu.addMenu(self.menu)

        self.menu.connect(self.menu,QtCore.SIGNAL("aboutToShow()"),self.importPlugins)

    def AddFunction(self,name,description,script):
        """ Add a plugin function
        """
        self.registry[name]=script,description

        def handler(obj=self,script=script):
          try:
            script(Context(sgPyQt))
          except:
            s=traceback.format_exc()
            QtGui.QMessageBox.warning(None,"Exception occured",s)

        self.handlers[name]=handler

    def importPlugins(self):
        """Execute the salome_plugins file that contains plugins definition """
        studyId=sg.getActiveStudyId()
        if studyId == 0:
          self.menu.clear()
          #self.menu.addAction("No active study : open or load a new one")
          return
        elif self.lasttime ==0:
          salome.salome_init(embedded=1)

        lasttime=0

        plugins_files=[]
        plugins_file_name=self.name+"_plugins.py"
        for directory in self.plugindirs:
          plugins_file = os.path.join(directory,plugins_file_name)
          if os.path.isfile(plugins_file):
            plugins_files.append((directory,plugins_file))
            lasttime=max(lasttime,os.path.getmtime(plugins_file))

        plugins_files.sort()

        if not plugins_files:
          self.registry.clear()
          self.handlers.clear()
          self.lasttime=0
          self.menu.clear()
          return

        if self.plugins_files != plugins_files or lasttime > self.lasttime:
          global current_plugins_manager
          current_plugins_manager=self
          self.registry.clear()
          self.handlers.clear()
          self.lasttime=lasttime
          for directory,plugins_file in plugins_files:
            if directory not in sys.path:
              sys.path.insert(0,directory)
            try:
              execfile(plugins_file,globals(),{})
            except:
              print "Error while loading plugins from file:",plugins_file
              traceback.print_exc()

          self.updateMenu()

    def updateMenu(self):
        """Update the Plugins menu"""
        self.menu.clear()
        for name,handler in self.handlers.items():
          act=self.menu.addAction(name,handler)
          act.setStatusTip(self.registry[name][1])

    def activate(self):
        """Add the Plugins menu to the Tools menu"""

    def deactivate(self):
        """Remove the Plugins menu from Tools menu (not implemented)"""


def AddFunction(name,description,script):
   """ Add a plugin function
       Called by a user to register a function (script)
   """
   return current_plugins_manager.AddFunction(name,description,script)
