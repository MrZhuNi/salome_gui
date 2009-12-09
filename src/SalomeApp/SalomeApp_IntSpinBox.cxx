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
#include "SalomeApp_Notebook.h"

#include <SUIT_Session.h>

#include <QKeyEvent>
#include <QLineEdit>

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
: QtxIntSpinBox( parent ),
  myDefaultValue( 0 )
{
  connectSignalsAndSlots();
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
: QtxIntSpinBox( min, max, step, parent ),
  myDefaultValue( 0 )
{
  connectSignalsAndSlots();
}

/*!
  \brief Destructor.
*/
SalomeApp_IntSpinBox::~SalomeApp_IntSpinBox()
{
}

/*!
  \brief Connect signals and slots.
*/
void SalomeApp_IntSpinBox::connectSignalsAndSlots()
{
  connect( this, SIGNAL( editingFinished() ),
	   this, SLOT( onEditingFinished() ) );

  connect( this, SIGNAL( valueChanged( const QString& ) ),
	   this, SLOT( onTextChanged( const QString& ) ) );

  connect( lineEdit(), SIGNAL( textChanged( const QString& ) ),
	   this, SLOT( onTextChanged( const QString& ) ) );

  connect( lineEdit(), SIGNAL( textChanged( const QString& )),
 	   this, SIGNAL( textChanged( const QString& ) ) );
}

/*!
  \brief This function is called when editing is finished.
*/
void SalomeApp_IntSpinBox::onEditingFinished()
{
  if( myTextValue.isNull() )
    myTextValue = text();

  setText( myTextValue );
}

/*!
  \brief This function is called when value is changed.
*/
void SalomeApp_IntSpinBox::onTextChanged( const QString& text )
{
  myTextValue = text;

  int value = 0;
  QStringList absent;
  if( isValid( text, value, absent ) == Acceptable )
    myCorrectValue = text;
}

/*!
  \brief Interpret text entered by the user as a value.
  \param text text entered by the user
  \return mapped value
  \sa textFromValue()
*/
int SalomeApp_IntSpinBox::valueFromText( const QString& text ) const
{
  int value = 0;
  QStringList absent;
  if( isValid( text, value, absent ) == Acceptable )
    return value;

  return defaultValue();
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
bool SalomeApp_IntSpinBox::isValid( QString& msg, QStringList& absentParams, bool toCorrect )
{
  int value;
  QStringList absent;
  State aState = isValid( text(), value, absent );

  if( aState != Acceptable )
  {
    if( toCorrect )
    {
      if( aState == Incompatible )
        msg += tr( "ERR_INCOMPATIBLE_TYPE" ).arg( text() ) + "\n";
      else if( aState == NoVariable && !absent.isEmpty() )
      {
        // this kind of error should be processed by the external module
        absentParams << absent;
        return false;
      }
      else if( aState == Invalid )
        msg += tr( "ERR_INVALID_VALUE" ) + "\n";

      setText( myCorrectValue );
    }
    return false;
  }

  // workaround (to update the spin-box value after automatic variable definition)
  // in this case the spin-box should behave as its value has been changed but
  // at the same time its text should not be changed
  bool isBlocked = blockSignals( true );
  QString aText = text();
  setValue( value );
  setText( aText );
  blockSignals( isBlocked );

  return true;
}

/*!
  \brief This function is used to set a default value for this spinbox.
  \param value default value
*/
void SalomeApp_IntSpinBox::setDefaultValue( const int value )
{
  myDefaultValue = value;
}

/*!
  \brief This function is used to set a current value for this spinbox.
  \param value current value
*/
void SalomeApp_IntSpinBox::setValue( const int value )
{
  QtxIntSpinBox::setValue( value );

  myCorrectValue = QString::number( value );
  myTextValue = myCorrectValue;
}

/*!
  \brief This function is used to set a text for this spinbox.
  \param value current value
*/
void SalomeApp_IntSpinBox::setText( const QString& value )
{
  lineEdit()->setText(value);
}

/*!
  \brief This function is used to determine whether input is valid.
  \return validating operation result
*/
SalomeApp_IntSpinBox::State SalomeApp_IntSpinBox::isValid( const QString& text, int& value, QStringList& absent ) const
{
  bool isNumber = false;
  value = text.toInt( &isNumber );
  if( !isNumber )
  {
    text.toDouble( &isNumber );
    if( isNumber )
      return Invalid;

    SearchState aSearchState = findVariable( text, value, absent );
    if( aSearchState == NotFound )
      return NoVariable;
    else if( aSearchState == IncorrectType )
      return Incompatible;
    else if( aSearchState == IncorrectExpression )
      return Invalid;
  }

  if( !checkRange( value ) )
    return Invalid;

  return Acceptable;
}

/*!
  \brief This function return a default acceptable value (commonly, 0).
  \return default acceptable value
*/
int SalomeApp_IntSpinBox::defaultValue() const
{
  if( minimum() > myDefaultValue || maximum() < myDefaultValue )
    return minimum();

  return myDefaultValue;
}

/*!
  \brief This function is used to check that string value lies within predefined range.
  \return check status
*/
bool SalomeApp_IntSpinBox::checkRange( const int value ) const
{
  return value >= minimum() && value <= maximum();
}

/*!
  \brief This function is used to determine whether input is a variable name and to get its value.
  \return status of search operation
*/
SalomeApp_IntSpinBox::SearchState SalomeApp_IntSpinBox::findVariable( const QString& name, int& value, QStringList& absent ) const
{
  value = 0;
  if( SalomeApp_Application* app = dynamic_cast<SalomeApp_Application*>( SUIT_Session::session()->activeApplication() ) )
  {
    if( SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( app->activeStudy() ) )
    {
      SalomeApp_Notebook aNotebook( study );
      QStringList anAbsentParameters = aNotebook.absentParameters( name );
      if( !anAbsentParameters.isEmpty() )
      {
        absent << anAbsentParameters;
        return NotFound;
      }

      QVariant aVariant;
      if( aNotebook.isParameter( name ) )
	aVariant = aNotebook.get( name );
      else
      {
        try {
          aVariant = aNotebook.calculate( name );
        } catch ( const SALOME::TypeError& ex ) {
          return IncorrectType;
        } catch ( const SALOME::ExpressionError& ex ) {
          return IncorrectExpression;
        }
      }

      QVariant::Type aType = aVariant.type();
      if( aType == QVariant::Int || aType == QVariant::String ) 
      {
        bool ok = false;
        value = aVariant.toInt( &ok );
        if( ok )
          return Found;
      }
      return IncorrectType;
    }
  }
  absent << name;
  return NotFound;
}

/*!
  \brief This function is called when the spinbox recieves key press event.
*/
void SalomeApp_IntSpinBox::keyPressEvent( QKeyEvent* e )
{
  if ( e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter )
    QWidget::keyPressEvent( e );
  else
    QtxIntSpinBox::keyPressEvent( e );
}

/*!
  \brief This function is called when the spinbox recieves show event.
*/
void SalomeApp_IntSpinBox::showEvent( QShowEvent* )
{
  setText( myTextValue );
}
