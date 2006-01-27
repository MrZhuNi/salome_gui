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
#ifndef SALOMEAPP_VisualParameters_H
#define SALOMEAPP_VisualParameters_H

#include "SalomeApp.h"

#include <string>
#include <vector>
#include <qstring.h>

#include "SUIT_Session.h"
#include "SalomeApp_Study.h"

#include <SALOMEDSClient.hxx>

/*! 
  Class which an interface to store the visual parameters of the objects
*/
class SALOMEAPP_EXPORT SalomeApp_VisualParameters
{
public:
  SalomeApp_VisualParameters(const _PTR(SComponent)& sco, const int savePoint); 
  
  void setColor(const std::string& entry, std::vector<double> color);
  std::vector<double> getColor(const std::string& entry);

  void setTransparency(const std::string& entry, const double& value);
  double getTransparency(const std::string& entry); 

  void setLineWidth(const std::string& entry, const double& value);
  double getLineWidth(const std::string& entry); 

  void setPresentationMode(const std::string& entry, const int mode);
  int getPresentationMode(const std::string& entry);

  void setDisplayed(const std::string& entry);
  bool isDisplayed(const std::string& entry);

  void setTypeOfDisplayed(const std::string& entry, const int type);
  int getTypeOfDisplayed(const std::string& entry);

  void setClipping(const std::string& entry, std::vector<double> color);
  std::vector<double> getClipping(const std::string& entry);

  _PTR(AttributeParameter) getContainer();
  
  
 private:
  _PTR(AttributeParameter) _ap;

};

/*!
 * \brief internal class
*/
class SALOMEAPP_EXPORT ViewerContainer
{
 public:
  ViewerContainer(int savePoint);
  /*! returns a number of viewers*/
  int getNbViewers();
  /*! sets an active view ID*/
  void setActiveViewID(int viewID);
  /*! returns an active view ID*/
  int getActiveViewID();
  /*! returns an ID of the viewer with given number [1:nbViewers]*/
  int getViewerID(int viewerNumber);
  /*! returns a type of the viewer with given ID*/
  std::string getViewerType(int viewerID);
  /*! returns the added viewer ID */
  int addViewer(const QString& type);
  /*! returns a number of views of the viewer with given ID*/
  int getNbViews(int viewerID);
  /*! returns an ID of the view with given number [1:nbViews]*/
  int getViewID(int viewerID, int viewNumber);
  /*! adds a view of the viewer */
  int addView(int viewerID, const QString& caption, const QString& parameters);
  /*! return a caption of the view with given ID*/
  QString getViewCaption(int viewID);
  /*! return parameters of the view with given ID*/
  QString getViewParameters(int viewID);
  

  void addModule(const QString& name);
  void setActiveModule(const QString& name);
  QString getActiveModule();
  std::vector<std::string> getModules();

  void setSavePointName(const QString& name);
  QString getSavePointName();

  void init();

 protected:
  _PTR(AttributeParameter) _ap;
  int _currentViewerID;
  int _currentViewID;
};

#endif
