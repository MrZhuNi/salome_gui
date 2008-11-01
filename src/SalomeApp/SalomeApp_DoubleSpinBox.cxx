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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// File:      SalomeApp_DoubleSpinBox.cxx
// Author:    Oleg UVAROV

#include "SalomeApp_DoubleSpinBox.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"

#include <SUIT_Session.h>

#include "SALOMEDSClient_ClientFactory.hxx" 
#include CORBA_SERVER_HEADER(SALOMEDS)

/*!
  \class SalomeApp_DoubleSpinBox
*/

/*!
  \brief Constructor.

  Constructs a spin box with 0.0 as minimum value and 99.99 as maximum value,
  a step value of 1.0 and a precision of 2 decimal places. 
  The value is initially set to 0.00.

  \param parent parent object
*/
SalomeApp_DoubleSpinBox::SalomeApp_DoubleSpinBox( QWidget* parent )
: QtxDoubleSpinBox( parent ),
  myDefaultValue( 0.0 ),
  myMinimum( 0.0 ),
  myMaximum( 99.99 )
{
}

/*!
  \brief Constructor.

  Constructs a spin box with specified minimum, maximum and step value.
  The precision is set to 2 decimal places. 
  The value is initially set to the minimum value.

  \param min spin box minimum possible value
  \param max spin box maximum possible value
  \param step spin box increment/decrement value
  \param parent parent object
*/
SalomeApp_DoubleSpinBox::SalomeApp_DoubleSpinBox( double min, double max, double step, QWidget* parent )
: QtxDoubleSpinBox( min, max, step, parent ),
  myDefaultValue( 0.0 ),
  myMinimum( min ),
  myMaximum( max )
{
}

/*!
  \brief Constructor.

  Constructs a spin box with specified minimum, maximum and step value.
  The precision is set to 2 decimal places. 
  The value is initially set to the minimum value.

  \param min spin box minimum possible value
  \param max spin box maximum possible value
  \param step spin box increment/decrement value
  \param parent parent object
*/
SalomeApp_DoubleSpinBox::SalomeApp_DoubleSpinBox( double min, double max, double step, int prec, int dec, QWidget* parent )
: QtxDoubleSpinBox( min, max, step, prec, dec, parent ),
  myDefaultValue( 0.0 ),
  myMinimum( min ),
  myMaximum( max )
{
}

/*!
  \brief Destructor.
*/
SalomeApp_DoubleSpinBox::~SalomeApp_DoubleSpinBox()
{
}

/*!
  \brief Interpret text entered by the user as a value.
  \param text text entered by the user
  \return mapped value
  \sa textFromValue()
*/
double SalomeApp_DoubleSpinBox::valueFromText( const QString& text ) const
{
  QString str = text;

  double value = 0;
  if( findVariable( str, value ) )
    str = QString::number( value );

  int pos = 0;
  if( checkRange( str ) && QtxDoubleSpinBox::validate( str, pos ) == QValidator::Acceptable )
    value = QtxDoubleSpinBox::valueFromText( str );
  else
    value = defaultValue();

  return value;
}

/*!
  \brief This function is used by the spin box whenever it needs to display
  the given value.

  \param val spin box value
  \return text representation of the value
  \sa valueFromText()
*/
QString SalomeApp_DoubleSpinBox::textFromValue( double val ) const
{
  return QtxDoubleSpinBox::textFromValue( val );
}

/*!
  \brief This function is used to determine whether input is valid.
  \param str currently entered value
  \param pos cursor position in the string
  \return validating operation result
*/
QValidator::State SalomeApp_DoubleSpinBox::validate( QString& str, int& pos ) const
{
  return QValidator::Acceptable;
}

/*!
  \brief This function is used to determine whether input is valid.
  \return validating operation result
*/
bool SalomeApp_DoubleSpinBox::isValid() const
{
  QString str = text();

  double value = 0;
  if( findVariable( str, value ) )
    str = QString::number( value );

  int pos = 0;
  return checkRange( str ) && QtxDoubleSpinBox::validate( str, pos ) == QValidator::Acceptable;
}

/*!
  \brief This function is used to set minimum and maximum values for this spinbox.
  \param value default value
*/
void SalomeApp_DoubleSpinBox::setRange( const double min, const double max )
{
  QtxDoubleSpinBox::setRange( min, max );

  myMinimum = min;
  myMaximum = max;
}

/*!
  \brief This function is used to set a default value for this spinbox.
  \param value default value
*/
void SalomeApp_DoubleSpinBox::setDefaultValue( const double value )
{
  myDefaultValue = value;
}

/*!
  \brief This function return a default acceptable value (commonly, 0.0).
  \return default acceptable value
*/
double SalomeApp_DoubleSpinBox::defaultValue() const
{
  if( myMinimum > myDefaultValue || myMaximum < myDefaultValue )
    return myMinimum;

  return myDefaultValue;
}

/*!
  \brief This function is used to check that string value lies within predefined range.
  \return check status
*/
bool SalomeApp_DoubleSpinBox::checkRange( const QString& str ) const
{
  bool ok = false;
  double value = str.toDouble( &ok );
  return ok && value >= myMinimum && value <= myMaximum;
}

/*!
  \brief This function is used to determine whether input is a variable name and to get its value.
  \return true if the input is a variable name
*/
bool SalomeApp_DoubleSpinBox::findVariable( const QString& name, double& value ) const
{
  if( SalomeApp_Application* app = dynamic_cast<SalomeApp_Application*>( SUIT_Session::session()->activeApplication() ) )
  {
    if( SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( app->activeStudy() ) )
    {
      _PTR(Study) studyDS = study->studyDS();
      std::vector<std::string> aVariableNames = studyDS->GetVariableNames();
      for( unsigned int i = 0, n = aVariableNames.size(); i < n; i++ )
      {
	std::string aName = aVariableNames[ i ];
	if( studyDS->IsReal( aName ) || studyDS->IsInteger( aName ) )
	{
	  double aValue = studyDS->GetReal( aName );
	  if( aName == name.toStdString() )
	  {
	    value = aValue;
	    return true;
	  }
	}
      }
    }
  }
  return false;
}
