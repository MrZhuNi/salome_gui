// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
#include "SalomeApp_VisualParameters.h"
#include <utilities.h>

using namespace std;

#define PT_INTEGER   0
#define PT_REAL      1
#define PT_BOOLEAN   2
#define PT_STRING    3
#define PT_REALARRAY 4
#define PT_INTARRAY  5
#define PT_STRARRAY  6

#define ONE_ENTRY_DBL_ARGS_LENGTH   12
#define IDX_COLOR_R        0
#define IDX_COLOR_G        1
#define IDX_COLOR_B        2
#define IDX_TRANSPARENCY   3
#define IDX_LINE_WIDTH     4
#define IDX_CLIPPING       5 //[5:11] 
#define CLIPPING_LENGTH    6

#define ONE_ENTRY_INT_ARGS_LENGTH   4
#define IDX_DISPLAY_MODE   0
#define IDX_DISPLAYED      1
#define IDX_TYPE           2

#define AP_ENTRIES         0 //StrArray
#define AP_DOUBLE_ARGS     1 //RealArray
#define AP_INT_ARGS        1 //IntArray
#define AP_MODULE_NAME     1 //String
/*!
  Constructor
*/
SalomeApp_VisualParameters::SalomeApp_VisualParameters(const string& moduleName, const int savePoint)
{
  SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );
  if( !study ) return;
  _PTR(AttributeParameter) main_ap = study->getStateParameters(savePoint);
  _PTR(SObject) main_so = main_ap->GetSObject();
  

  _PTR(ChildIterator) it = study->studyDS()->NewChildIterator(main_so);
  for(; it->More(); it->Next()) {
    _PTR(SObject) so(it->Value());
    _PTR(GenericAttribute) ga;
    if(so->FindAttribute(ga, "AttributeParameter")) {
      _PTR(AttributeParameter) par(ga);
      if(!par->IsSet(AP_MODULE_NAME, PT_STRING)) continue;
      if(par->GetString(AP_MODULE_NAME) == moduleName) {
	_ap = par;
	return;
      }
    }
  }

  _PTR(StudyBuilder) builder = study->studyDS()->NewBuilder();
  _PTR(SObject) so = builder->NewObject(main_so);
  _ap = builder->FindOrCreateAttribute(so, "AttributeParameter");
  setModuleName(moduleName);
} 

/*!
  sets a name of the module for which the parameters are stored
*/ 
void SalomeApp_VisualParameters::setModuleName(const string& moduleName)
{
  if(!_ap) return;
  _ap->SetString(AP_MODULE_NAME, moduleName);
}

/*!
  returns a name of the module for which the parameters are stored
*/
string SalomeApp_VisualParameters::getModuleName()
{
  if(!_ap) return "";
  if(!_ap->IsSet(AP_MODULE_NAME, PT_STRING)) return "";
  return _ap->GetString(AP_MODULE_NAME);
}

/*!
  sets a color (RGB values) for the obect with id \a entry
*/  
void SalomeApp_VisualParameters::setColor(const string& entry, const vector<double>& color)
{
  int entryID = getEntryID(entry);
  if(entryID < 0 || color.size() < 3) return;
  vector<double> vr;
  if(!_ap->IsSet(AP_DOUBLE_ARGS, PT_REALARRAY)) return;
  vr = _ap->GetRealArray(AP_DOUBLE_ARGS);
  int idx = entryID*ONE_ENTRY_DBL_ARGS_LENGTH;

  vr[idx+IDX_COLOR_R] = color[0];
  vr[idx+IDX_COLOR_G] = color[1];
  vr[idx+IDX_COLOR_B] = color[2];
  _ap->SetRealArray(AP_DOUBLE_ARGS, vr);
}

/*!
  returns a color (RGB values) for the obect with id \a entry
*/
vector<double> SalomeApp_VisualParameters::getColor(const string& entry)
{
  vector<double> color, vr;
  int entryID = getEntryID(entry);
  if(entryID < 0) return color;
  if(!_ap->IsSet(AP_DOUBLE_ARGS, PT_REALARRAY)) return color;
  vr = _ap->GetRealArray(AP_DOUBLE_ARGS);
  int idx = entryID*ONE_ENTRY_DBL_ARGS_LENGTH;

  color.push_back(vr[idx+IDX_COLOR_R]);
  color.push_back(vr[idx+IDX_COLOR_G]);
  color.push_back(vr[idx+IDX_COLOR_B]); 

  return color;
}

/*!
  sets a transparency for the obect with id \a entry
*/
void SalomeApp_VisualParameters::setTransparency(const string& entry, const double& value)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return;
  if(!_ap->IsSet(AP_DOUBLE_ARGS, PT_REALARRAY)) return;
  vector<double> vr = _ap->GetRealArray(AP_DOUBLE_ARGS);
  int idx = entryID*ONE_ENTRY_DBL_ARGS_LENGTH;

  vr[idx+IDX_TRANSPARENCY] = value;
  _ap->SetRealArray(AP_DOUBLE_ARGS, vr);
}

/*!
  returns a transparency for the obect with id \a entry
*/
double SalomeApp_VisualParameters::getTransparency(const string& entry)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return -1.0;
  if(!_ap->IsSet(AP_DOUBLE_ARGS, PT_REALARRAY)) return -1.0;
  vector<double> vr = _ap->GetRealArray(AP_DOUBLE_ARGS);
  int idx = entryID*ONE_ENTRY_DBL_ARGS_LENGTH;

  return vr[idx+IDX_TRANSPARENCY];
} 

/*!
  sets a line width for the obect with id \a entry
*/
void SalomeApp_VisualParameters::setLineWidth(const string& entry, const double& value)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return;
  if(!_ap->IsSet(AP_DOUBLE_ARGS, PT_REALARRAY)) return;
  vector<double> vr = _ap->GetRealArray(AP_DOUBLE_ARGS);
  int idx = entryID*ONE_ENTRY_DBL_ARGS_LENGTH;

  vr[idx+IDX_LINE_WIDTH] = value;
  _ap->SetRealArray(AP_DOUBLE_ARGS, vr);
}

/*!
  returns a line width for the obect with id \a entry
*/
double SalomeApp_VisualParameters::getLineWidth(const string& entry)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return -1.0;
  if(!_ap->IsSet(AP_DOUBLE_ARGS, PT_REALARRAY)) return -1.0;
  vector<double> vr = _ap->GetRealArray(AP_DOUBLE_ARGS);
  int idx = entryID*ONE_ENTRY_DBL_ARGS_LENGTH;

  return vr[idx+IDX_LINE_WIDTH];
}

/*!
  sets a represenatation mode of the obect with id \a entry
*/
void SalomeApp_VisualParameters::setPresentationMode(const string& entry, const int mode)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return;
  if(!_ap->IsSet(AP_INT_ARGS, PT_INTARRAY)) return;
  vector<int> vr = _ap->GetIntArray(AP_INT_ARGS);
  int idx = entryID*ONE_ENTRY_INT_ARGS_LENGTH;

  vr[idx+IDX_DISPLAY_MODE] = mode;
  _ap->SetIntArray(AP_INT_ARGS, vr);
}

/*!
  returns a represenatation mode of the obect with id \a entry
*/
int SalomeApp_VisualParameters::getPresentationMode(const string& entry)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return -1;
  if(!_ap->IsSet(AP_INT_ARGS, PT_INTARRAY)) return -1;
  vector<int> vr = _ap->GetIntArray(AP_INT_ARGS);
  int idx = entryID*ONE_ENTRY_INT_ARGS_LENGTH;

  return vr[idx+IDX_DISPLAY_MODE];
}

/*!
  marks the object with id \a entry as displayed
*/
void SalomeApp_VisualParameters::setDisplayed(const string& entry)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return;
  if(!_ap->IsSet(AP_INT_ARGS, PT_INTARRAY)) return;
  vector<int> vr = _ap->GetIntArray(AP_INT_ARGS);
  int idx = entryID*ONE_ENTRY_INT_ARGS_LENGTH;

  vr[idx+IDX_DISPLAYED] = 1;
  _ap->SetIntArray(AP_INT_ARGS, vr);
}

/*!
  returns true if the object with id \a entry is displayed
*/
bool SalomeApp_VisualParameters::isDisplayed(const string& entry)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return -1;
  if(!_ap->IsSet(AP_INT_ARGS, PT_INTARRAY)) return -1;
  vector<int> vr = _ap->GetIntArray(AP_INT_ARGS);
  int idx = entryID*ONE_ENTRY_INT_ARGS_LENGTH;

  return (vr[idx+IDX_DISPLAYED] == 1);
}

/*!
  sets a type of the displayed object with id \a entry
*/
void SalomeApp_VisualParameters::setTypeOfDisplayed(const string& entry, const int type)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return;
  if(!_ap->IsSet(AP_INT_ARGS, PT_INTARRAY)) return;
  vector<int> vr = _ap->GetIntArray(AP_INT_ARGS);
  int idx = entryID*ONE_ENTRY_INT_ARGS_LENGTH;

  vr[idx+IDX_TYPE] = type;
  _ap->SetIntArray(AP_INT_ARGS, vr);
}

/*!
  gets a type of displayed object with id \a entry
*/
int SalomeApp_VisualParameters::getTypeOfDisplayed(const string& entry)
{
  int entryID = getEntryID(entry);
  if(entryID < 0) return -1;
  if(!_ap->IsSet(AP_INT_ARGS, PT_INTARRAY)) return -1;
  vector<int> vr = _ap->GetIntArray(AP_INT_ARGS);
  int idx = entryID*ONE_ENTRY_INT_ARGS_LENGTH;

  return vr[idx+IDX_TYPE];
}

/*!
  sets clipping plane definitions for the object with id \a entry
*/
void SalomeApp_VisualParameters::setClipping(const string& entry, const vector<double>& color)
{
}
 
/*!
  returns clipping plane definitions for the object with id \a entry
*/
vector<double> SalomeApp_VisualParameters::getClipping(const string& entry)
{
  vector<double> v;
  return v;
}

/*!
  returns a list of stored entries
*/
vector<string> SalomeApp_VisualParameters::getEntries()
{
  vector<string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(AP_ENTRIES, PT_STRARRAY)) return v;
  return _ap->GetStrArray(AP_ENTRIES);
}

/*!
  returns an id of the \a entry 
*/
int SalomeApp_VisualParameters::getEntryID(const std::string& entry)
{
  if(!_ap) return -1;
  vector<string> v = getEntries();
  int idx = 0;
  for(int i = 0; i<v.size(); i++) {
    if(v[i] == entry) return i;
  }
 
  idx =  addEntry(entry);
  return idx;
}

int SalomeApp_VisualParameters::addEntry(const std::string& entry)
{
  if(!_ap) return -1;
  vector<string> v = getEntries();
  v.push_back(entry);
  _ap->SetStrArray(AP_ENTRIES, v);
  
  //Add double parameters of the entry
  vector<double> vr, new_vr;
  if(_ap->IsSet(AP_DOUBLE_ARGS, PT_REALARRAY)) vr = _ap->GetRealArray(AP_DOUBLE_ARGS);
  int length = vr.size(), new_length = vr.size();
  new_length += ONE_ENTRY_DBL_ARGS_LENGTH;
  new_vr.resize(new_length);
  for(int i = 0; i<new_length; i++) {
    if(i<length) new_vr[i] = vr[i];
    else new_vr[i] = 0.0;
  }
  _ap->SetRealArray(AP_DOUBLE_ARGS, new_vr);


  //Add int parameters of the entry
  vector<int> vi, new_vi;
  if(_ap->IsSet(AP_INT_ARGS, PT_INTARRAY)) vi = _ap->GetIntArray(AP_INT_ARGS);
  length = vi.size(); 
  new_length = vi.size();
  new_length += ONE_ENTRY_INT_ARGS_LENGTH;
  new_vi.resize(new_length);
  for(int i = 0; i<new_length; i++) {
    if(i<length) new_vi[i] = vi[i];
    else new_vi[i] = 0;
  }
  _ap->SetIntArray(AP_INT_ARGS, new_vi);

  return (v.size()-1);
}



/*###############################################################################################*/

#define AP_ID_OF_VIEWERS   1 //Int array
#define AP_ID_OF_VIEWS     2 //Int array
#define AP_MODULES         1 //String array
#define AP_ACTIVE_VIEW     1 //INT
#define AP_ACTIVE_MODULE   1 //STRING
#define AP_SAVE_POINT_NAME 2 //STRING

#define START_VIEWER_ID  100
#define START_VIEW_ID    200

ViewerContainer::ViewerContainer(int savePoint)
{
  SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );
  if( !study ) return;
  _ap = study->getStateParameters(savePoint);
}

int ViewerContainer::getNbViewers()
{
  if(!_ap) return -1;
  if(!_ap->IsSet(AP_ID_OF_VIEWERS, PT_INTARRAY)) return 0;
  return _ap->GetIntArray(AP_ID_OF_VIEWERS).size();
}

void ViewerContainer::setActiveViewID(int viewerID)
{
  if(!_ap) return;
  _ap->SetInt(AP_ACTIVE_VIEW, viewerID);
}

int ViewerContainer::getActiveViewID()
{
  if(!_ap) return -1;
  if(!_ap->IsSet(AP_ACTIVE_VIEW, PT_INTEGER)) return -1;
  return _ap->GetInt(AP_ACTIVE_VIEW);
}

int ViewerContainer::getViewerID(int viewerNumber)
{
  if(!_ap) return -1;
  vector<int> v;
  if(_ap->IsSet(AP_ID_OF_VIEWERS, PT_INTARRAY)) v = _ap->GetIntArray(AP_ID_OF_VIEWERS);
  if(v.size() < viewerNumber) return -1;
  return v[viewerNumber-1];
}

string ViewerContainer::getViewerType(int viewerID)
{
  if(!_ap) return "";
  if(!_ap->IsSet(viewerID, PT_STRING)) return "";
  return _ap->GetString(viewerID);
}

int ViewerContainer::addViewer(const QString& type)
{
  if(!_ap) return -1;
  vector<int> v;
  if(_ap->IsSet(AP_ID_OF_VIEWERS, PT_INTARRAY)) v = _ap->GetIntArray(AP_ID_OF_VIEWERS);
  int viewerID = _currentViewerID;
  v.push_back(viewerID);
  _ap->SetIntArray(AP_ID_OF_VIEWERS, v); //Added a new viewer to the list of viewers
  _ap->SetString(viewerID, type.latin1()); //Viewer type

  //Compute the next viewer ID
  _currentViewerID++;

  return viewerID;
}

int ViewerContainer::getNbViews(int viewerID)
{
  if(!_ap) return -1;
  if(!_ap->IsSet(viewerID, PT_INTARRAY)) return 0;
  return _ap->GetIntArray(viewerID).size();
}

int ViewerContainer::getViewID(int viewerID, int viewNumber)
{
  if(!_ap) return -1;
  //Get a list of view ID's associated with the given viewer
  vector<int> v;
  if(_ap->IsSet(viewerID, PT_INTARRAY)) v = _ap->GetIntArray(viewerID);
  if(v.size() < viewNumber) return -1;
  return v[viewNumber-1];  
}

int ViewerContainer::addView(int viewerID, const QString& caption, const QString& parameters)
{
  if(!_ap) return -1;
  //Get a list of view ID's associated with the given viewer
  vector<int> v;
  if(_ap->IsSet(viewerID, PT_INTARRAY)) v = _ap->GetIntArray(viewerID);
  int viewID = _currentViewID;

  v.push_back(viewID);
  _ap->SetIntArray(viewerID, v); //Add a view to the list of viewer's views

  vector<string> vs;
  vs.push_back(caption.latin1());
  vs.push_back(parameters.latin1());
  _ap->SetStrArray(viewID, vs); //Store view's caption and parameters

  

  //Compute the next view ID
  _currentViewID++;

  return viewID;
}

QString ViewerContainer::getViewCaption(int viewID)
{
  if(!_ap) return "";
  vector<string> vs;
  if(_ap->IsSet(viewID, PT_STRARRAY)) vs = _ap->GetStrArray(viewID);
  if(vs.size() < 2) return "";
  return vs[0];
}

QString ViewerContainer::getViewParameters(int viewID)
{
  if(!_ap) return "";
  vector<string> vs;
  if(_ap->IsSet(viewID, PT_STRARRAY)) vs = _ap->GetStrArray(viewID);
  if(vs.size() < 2) return "";
  return vs[1];
}

void ViewerContainer::addModule(const QString& name)
{
  vector<string> v;
  if(!_ap) return;
  if(_ap->IsSet(AP_MODULES, PT_STRARRAY)) v = _ap->GetStrArray(AP_MODULES);
  v.push_back(name.latin1());
  _ap->SetStrArray(AP_MODULES, v);
}

void ViewerContainer::setActiveModule(const QString& name)
{
  if(!_ap) return;
  _ap->SetString(AP_ACTIVE_MODULE, name.latin1());
}

QString ViewerContainer::getActiveModule()
{
  if(!_ap) return "";
  if(!_ap->IsSet(AP_ACTIVE_MODULE, PT_STRING)) return "";
  return _ap->GetString(AP_ACTIVE_MODULE);
}

vector<string> ViewerContainer::getModules()
{
  vector<string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(AP_MODULES, PT_STRARRAY)) return v;
  return _ap->GetStrArray(AP_MODULES);
}

void ViewerContainer::setSavePointName(const QString& name)
{
  if(!_ap) return;
  _ap->SetString(AP_SAVE_POINT_NAME, name.latin1());
}

QString ViewerContainer::getSavePointName()
{
  if(!_ap) return "";
  if(!_ap->IsSet(AP_SAVE_POINT_NAME, PT_STRING)) return "";
  return _ap->GetString(AP_SAVE_POINT_NAME);
}


void ViewerContainer::init()
{
  _ap->Clear();
  _currentViewerID = START_VIEWER_ID;
  _currentViewID =  START_VIEW_ID; 
}
