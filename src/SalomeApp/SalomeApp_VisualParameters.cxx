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

#define AP_ENTRIES         0 //StrArray
#define AP_COLOR           1 //RealArray
#define AP_CLIPPING        2 //RealArray
#define AP_TRANSPARENCY    1 //Double
#define AP_LINE_WIDTH      2 //Double
#define AP_MODE            1 //Int
#define AP_TYPE            2 //Int
#define AP_IS_DISPLAYED    1 //Bool

/*!
  Constructor
*/
SalomeApp_VisualParameters::SalomeApp_VisualParameters(const _PTR(SComponent)& sco, const int savePoint)
{
  if(!sco) return;
  _PTR(Study) study = sco->GetStudy();
  _PTR(StudyBuilder) builder = study->NewBuilder();
  _PTR(SObject) so = builder->NewObjectToTag(sco, savePoint);
  _ap = builder->FindOrCreateAttribute(so, "AttributeParameter");
} 

/*!
  sets a color for the obect with id \a entry
*/  
void SalomeApp_VisualParameters::setColor(const string& entry, vector<double> color)
{
}

/*!
  returns a color for the obect with id \a entry
*/
vector<double> SalomeApp_VisualParameters::getColor(const string& entry)
{
  vector<double> v;
  return v;
}

/*!
  sets a transparency for the obect with id \a entry
*/
void SalomeApp_VisualParameters::setTransparency(const string& entry, const double& value)
{
}

/*!
  returns a transparency for the obect with id \a entry
*/
double SalomeApp_VisualParameters::getTransparency(const string& entry)
{
  return 0.0;
} 

/*!
  sets a line width for the obect with id \a entry
*/
void SalomeApp_VisualParameters::setLineWidth(const string& entry, const double& value)
{
}

/*!
  returns a line width for the obect with id \a entry
*/
double SalomeApp_VisualParameters::getLineWidth(const string& entry)
{
  return 0.0;
}

/*!
  sets a represenatation mode of the obect with id \a entry
*/
void SalomeApp_VisualParameters::setPresentationMode(const string& entry, const int mode)
{
}

/*!
  returns a represenatation mode of the obect with id \a entry
*/
int SalomeApp_VisualParameters::getPresentationMode(const string& entry)
{
  return -1;
}

/*!
  marks the object with id \a entry as displayed
*/
void SalomeApp_VisualParameters::setDisplayed(const string& entry)
{
}

/*!
  returns true if the object with id \a entry is displayed
*/
bool SalomeApp_VisualParameters::isDisplayed(const string& entry)
{
  return false;
}

/*!
  sets a type of the displayed object with id \a entry
*/
void SalomeApp_VisualParameters::setTypeOfDisplayed(const string& entry, const int type)
{
}

/*!
  gets a type of displayed object with id \a entry
*/
int SalomeApp_VisualParameters::getTypeOfDisplayed(const string& entry)
{
  return -1;
}

/*!
  sets clipping plane definitions for the object with id \a entry
*/
void SalomeApp_VisualParameters::setClipping(const string& entry, vector<double> color)
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
  returns an AttributeParameter attribute
*/
_PTR(AttributeParameter) SalomeApp_VisualParameters::getContainer()
{
  return _ap;
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
  _ap = study->getViewerParameters(savePoint);
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
