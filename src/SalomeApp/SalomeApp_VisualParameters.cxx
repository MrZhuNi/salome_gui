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

/*!
  Constructor
*/
SalomeApp_VisualParameters::SalomeApp_VisualParameters(_PTR(SComponent), const int savePoint)
{
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
