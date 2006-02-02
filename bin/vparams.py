import salome
import string
import SALOME
import SALOMEDS
import SALOME_Session_idl

_PT_INTEGER = 0
_PT_REAL = 1
_PT_BOOLEAN = 2
_PT_STRING = 3
_PT_REALARRAY = 4
_PT_INTARRAY = 5
_PT_STRARRAY = 6

_ONE_ENTRY_DBL_ARGS_LENGTH = 12
_IDX_COLOR_R = 0
_IDX_COLOR_G = 1
_IDX_COLOR_B = 2
_IDX_TRANSPARENCY = 3
_IDX_LINE_WIDTH = 4
_IDX_CLIPPING = 5 
_CLIPPING_LENGTH = 6

_ONE_ENTRY_INT_ARGS_LENGTH = 4
_IDX_DISPLAY_MODE = 0
_IDX_DISPLAYED = 1
_IDX_TYPE = 2

_AP_ENTRIES = 0 
_AP_DOUBLE_ARGS = 1 
_AP_INT_ARGS = 1 
_AP_MODULE_NAME = 1

vp_session = None

def getSession():
    global vp_session
    if vp_session is None:
        vp_session = salome.naming_service.Resolve("/Kernel/Session")
        vp_session = vp_session._narrow(SALOME.Session)
        pass
    return vp_session 

def getStateParameters(study, savePoint):
    """Returns AttributeParameters that corresponds the given savePoint"""
    builder = study.NewBuilder();
    so = study.FindComponent("Interface Applicative");
    if so is None:
        so = builder.NewComponent("Interface Applicative")
        pass
    newSO = builder.NewObjectToTag(so, savePoint);
    return builder.FindOrCreateAttribute(newSO, "AttributeParameter");    


class VisualParameters:
    def __init__(self, study, moduleName, savePoint):
        """Initializes the instance"""
        main_ap = getStateParameters(study, savePoint)
        main_so = main_ap.GetSObject()
  
        it = study.NewChildIterator(main_so)
        while it.More():
            so = it.Value()
            isFound, param = so.FindAttribute("AttributeParameter")
            if isFound:
                param = param._narrow(SALOMEDS.AttributeParameter)
                if param.IsSet(_AP_MODULE_NAME, PT_STRING) == 0:
                    continue
            if param.GetString(_AP_MODULE_NAME) == moduleName:
                self._ap = param
                return
            it.Next()
            pass

        builder = study.NewBuilder()
        so = builder.NewObject(main_so)
        self._ap = builder.FindOrCreateAttribute(so, "AttributeParameter")
        self.setModuleName(moduleName)
        pass

    def setModuleName(self, moduleName):
        """Sets a name of a GUI module to mark the save point for the module"""
        if self._ap is None: return
        self._ap.SetString(_AP_MODULE_NAME, moduleName)
        pass
    
    def setColor(self, entry, color):
        """Sets a color of the entry"""
        entryID = self.getEntryID(entry)
        if entryID < 0 and len(color) < 3: return
        if self._ap.IsSet(_AP_DOUBLE_ARGS, _PT_REALARRAY) == 0: return
        vr = self._ap.GetRealArray(_AP_DOUBLE_ARGS)
        idx = entryID*_ONE_ENTRY_DBL_ARGS_LENGTH

        vr[idx+_IDX_COLOR_R] = color[0]
        vr[idx+_IDX_COLOR_G] = color[1]
        vr[idx+_IDX_COLOR_B] = color[2]
        self._ap.SetRealArray(_AP_DOUBLE_ARGS, vr)
        pass
    
    def setTransparency(self, entry, value):
        """Sets a transparency of the entry"""
        entryID = self.getEntryID(entry)
        if entryID < 0: return
        if self._ap.IsSet(_AP_DOUBLE_ARGS, _PT_REALARRAY) == 0: return
        vr = self._ap.GetRealArray(_AP_DOUBLE_ARGS)
        idx = entryID*_ONE_ENTRY_DBL_ARGS_LENGTH

        vr[idx+_IDX_TRANSPARENCY] = value
        self._ap.SetRealArray(_AP_DOUBLE_ARGS, vr)
        pass
    
    def setLineWidth(self, entry, value):
        """Sets a line width of the entry"""
        entryID = self.getEntryID(entry)
        if entryID < 0: return
        if self._ap.IsSet(_AP_DOUBLE_ARGS, _PT_REALARRAY) == 0: return
        vr = self._ap.GetRealArray(_AP_DOUBLE_ARGS)
        idx = entryID*_ONE_ENTRY_DBL_ARGS_LENGTH

        vr[idx+_IDX_LINE_WIDTH] = value
        self._ap.SetRealArray(_AP_DOUBLE_ARGS, vr)
        pass
    
    def setPresentationMode(self, entry, mode):
        """Sets a presentation mode of the displayed object"""
        entryID = self.getEntryID(entry)
        if entryID < 0: return
        if self._ap.IsSet(_AP_INT_ARGS, _PT_INTARRAY) == 0: return
        vr = self._ap.GetIntArray(_AP_INT_ARGS);
        idx = entryID*_ONE_ENTRY_INT_ARGS_LENGTH

        vr[idx+_IDX_DISPLAY_MODE] = mode
        self._ap.SetIntArray(_AP_INT_ARGS, vr)
        pass
    
    def setDisplayed(self, entry):
        """Sets a flag Displayed to True"""
        entryID = self.getEntryID(entry)
        if entryID < 0: return
        if self._ap.IsSet(_AP_INT_ARGS, _PT_INTARRAY) == 0: return
        vr = self._ap.GetIntArray(_AP_INT_ARGS);
        idx = entryID*_ONE_ENTRY_INT_ARGS_LENGTH;

        vr[idx+_IDX_DISPLAYED] = 1
        self._ap.SetIntArray(_AP_INT_ARGS, vr)
        pass
    
    def setTypeOfDisplayed(self, entry, type):
        """Sets a type of the displayed object"""
        entryID = self.getEntryID(entry)
        if entryID < 0: return
        if self._ap.IsSet(_AP_INT_ARGS, _PT_INTARRAY) == 0: return
        vr = self._ap.GetIntArray(_AP_INT_ARGS);
        idx = entryID*_ONE_ENTRY_INT_ARGS_LENGTH

        vr[idx+_IDX_TYPE] = type
        self._ap.SetIntArray(_AP_INT_ARGS, vr)        
        pass
    
    def setClipping(self, entry, clipping):
        """Sets clipping planes for the entry"""
        entryID = self.getEntryID(entry)
        if entryID < 0: return
        if self._ap.IsSet(_AP_DOUBLE_ARGS, _PT_REALARRAY) == 0: return
        vr = self._ap.GetRealArray(_AP_DOUBLE_ARGS)
        idx = entryID*_ONE_ENTRY_DBL_ARGS_LENGTH

        i = 0
        while i<len(clipping) and i<_CLIPPING_LENGTH: 
            vr[idx+_IDX_CLIPPING+i] = clipping[i]
            i+=1
            pass

        self._ap.SetRealArray(_AP_DOUBLE_ARGS, vr)
        pass
    
    def getContainer(self):
        """Returns an underlaying AttributeParameter"""
        return self._ap

    def getEntries(self):
        """Private method: Returns a list of stored entries"""
        v = []
        if self._ap is None: return v
        if self._ap.IsSet(_AP_ENTRIES, _PT_STRARRAY) == 0: return v
        return self._ap.GetStrArray(_AP_ENTRIES);
   
    def getEntryID(self, entry):
        """Private method: Returns an integer ID of the entry"""
        if self._ap is None: return -1
        v = self.getEntries()
        idx = 0
        i = 0
        while i<len(v):
            if v[i] == entry: return i
            i+=1
            pass
 
        idx = self.addEntry(entry)
        return idx
   
    def addEntry(self, entry):
        """Private method: Adda a new entry"""
        if self._ap is None: return -1
        v = self.getEntries()
        v.append(entry)
        self._ap.SetStrArray(_AP_ENTRIES, v);
  
        #Add double parameters of the entry
        vr = []
        new_vr = []
        if self._ap.IsSet(_AP_DOUBLE_ARGS, _PT_REALARRAY) != 0: vr = self._ap.GetRealArray(_AP_DOUBLE_ARGS)
        length = len(vr)
        new_length = length
        new_length += _ONE_ENTRY_DBL_ARGS_LENGTH
        i = 0
        while i<new_length:
            if i<length: new_vr.append(vr[i])
            else: new_vr.append(0.0)
            i+=1
            pass

        self._ap.SetRealArray(_AP_DOUBLE_ARGS, new_vr)


        #Add int parameters of the entry
        vi = []
        new_vi = []
        if self._ap.IsSet(_AP_INT_ARGS, _PT_INTARRAY) != 0: vi = self._ap.GetIntArray(_AP_INT_ARGS)
        length = len(vi) 
        new_length = length
        new_length += _ONE_ENTRY_INT_ARGS_LENGTH;
        i = 0
        while i<new_length:
            if i<length: new_vi.append(vi[i])
            else: new_vi.append(0)
            i+=1
            pass

        self._ap.SetIntArray(_AP_INT_ARGS, new_vi)
        
        return (len(v)-1)        
    
    pass


_AP_ID_OF_VIEWERS = 1 
_AP_ID_OF_VIEWS = 2 
_AP_MODULES = 1 
_AP_ACTIVE_VIEW = 1 
_AP_ACTIVE_MODULE = 1 
_AP_SAVE_POINT_NAME = 2 

START_VIEWER_ID = 100
START_VIEW_ID =200

class  ViewerContainer:

    def __init__(self, study, savePoint):
        """Initializes the instance"""
        self._ap = getStateParameters(study, savePoint)
        pass

    def initialize(self):
        self._currentViewerID = START_VIEWER_ID
        self._currentViewID = START_VIEW_ID        
        
    def setActiveViewID(self, viewID):
        """Marks a view with ID = viewID as an active view"""
        if self._ap is None: return
        self._ap.SetInt(_AP_ACTIVE_VIEW, viewID)
        pass
        
    def addViewer(self, type):
        """Adds a new viewer"""
        if self._ap is None: return -1
        v = []
        if self._ap.IsSet(_AP_ID_OF_VIEWERS, _PT_INTARRAY) != 0: v = self._ap.GetIntArray(_AP_ID_OF_VIEWERS)
        viewerID = self._currentViewerID
        v.append(viewerID)
        self._ap.SetIntArray(_AP_ID_OF_VIEWERS, v) #Added a new viewer to the list of viewers
        self._ap.SetString(viewerID, type) #Viewer type
        
        #Compute the next viewer ID
        self._currentViewerID+=1       
        return viewerID
    
    def addView(self, viewerID, caption, parameters):
        """Adds a new view to the viewer with ID = viewerID"""
        if self._ap is None: return -1
        #Get a list of view ID's associated with the given viewer
        v = []
        if self._ap.IsSet(viewerID, _PT_INTARRAY) != 0: v = self._ap.GetIntArray(viewerID);
        viewID = self._currentViewID

        v.append(viewID)
        self._ap.SetIntArray(viewerID, v) #Add a view to the list of viewer's views

        vs = []
        vs.append(caption)
        vs.append(parameters);
        self._ap.SetStrArray(viewID, vs) #Store view's caption and parameters

        #Compute the next view ID
        self._currentViewID+=1;
        return viewID
        
    def addModule(self, name):
        """Adds a new GUI module"""
        v = []
        if self._ap is None: return
        if self._ap.IsSet(_AP_MODULES, _PT_STRARRAY) != 0: v = self._ap.GetStrArray(_AP_MODULES)
        v.append(name)
        self._ap.SetStrArray(_AP_MODULES, v)
        pass
    
    def setActiveModule(self, name):
        """Marks a GUI module as an active module"""
        if self._ap is None: return
        self._ap.SetString(_AP_ACTIVE_MODULE, name)
        pass
        
    def setSavePointName(self, name):
        """Sets a name of the save point"""
        if self._ap is None: return
        self._ap.SetString(_AP_SAVE_POINT_NAME, name)
        pass
    
    def getContainer(self):
        """Returns an attribute AttributeParameter"""
        return self._ap

    pass
