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
// File:      SalomeApp_IntSpinBox.cxx
// Author:    Oleg UVAROV

#include "SalomeApp_IntSpinBox.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"

#include <SUIT_Session.h>

#include "SALOMEDSClient_ClientFactory.hxx" 
#include CORBA_SERVER_HEADER(SALOMEDS)

/*!
  \class SalomeApp_IntSpinBox
*/

/*!
  \brief Constructor.

  Constructs a spin box with 0 as minimum value and 99 as maximum value,
  a step value of 1. The value is initially set to 0.

  \param parent parent object
*/
SalomeApp_IntSpinBox::SalomeApp_IntSpinBox( QWidget* parent )
: QtxIntSpinBox( parent )
{
}

/*!
  \brief Constructor.

  Constructs a spin box with specified minimum, maximum and step value.
  The value is initially set to the minimum value.

  \param min spin box minimum possible value
  \param max spin box maximum possible value
  \param step spin box increment/decrement value
  \param parent parent object
*/
SalomeApp_IntSpinBox::SalomeApp_IntSpinBox( int min, int max, int step, QWidget* parent )
: QtxIntSpinBox( min, max, step, parent )
{
}

/*!
  \brief Destructor.
*/
SalomeApp_IntSpinBox::~SalomeApp_IntSpinBox()
{
}

/*!
  \brief Interpret text entered by the user as a value.
  \param text text entered by the user
  \return mapped value
  \sa textFromValue()
*/
int SalomeApp_IntSpinBox::valueFromText( const QString& text ) const
{
  QString str = text;

  int value = 0;
  if( findVariable( str, value ) )
    str = QString::number( value );

  int pos = 0;
  if( QtxIntSpinBox::validate( str, pos ) == QValidator::Acceptable )
    value = QtxIntSpinBox::valueFromText( str );
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
QString SalomeApp_IntSpinBox::textFromValue( int val ) const
{
  return QtxIntSpinBox::textFromValue( val );
}

/*!
  \brief This function is used to determine whether input is valid.
  \param str currently entered value
  \param pos cursor position in the string
  \return validating operation result
*/
QValidator::State SalomeApp_IntSpinBox::validate( QString& str, int& pos ) const
{
  return QValidator::Acceptable;
}

/*!
  \brief This function is used to determine whether input is valid.
  \return validating operation result
*/
bool SalomeApp_IntSpinBox::isValid() const
{
  QString str = text();

  int value = 0;
  if( findVariable( str, value ) )
    str = QString::number( value );

  int pos = 0;
  return QtxIntSpinBox::validate( str, pos ) == QValidator::Acceptable;
}

/*!
  \brief This function return a default acceptable value (commonly, 0).
  \return default acceptable value
*/
int SalomeApp_IntSpinBox::defaultValue() const
{
  if( minimum() > 0 || maximum() < 0 )
    return minimum();

  return 0;
}

/*!
  \brief This function is used to determine whether input is a variable name and to get its value.
  \return true if the input is a variable name
*/
bool SalomeApp_IntSpinBox::findVariable( const QString& name, int& value ) const
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
	if( studyDS->IsInteger( aName ) )
	{
	  int aValue = studyDS->GetInteger( aName );

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
