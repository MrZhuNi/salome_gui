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
#include "SalomeApp_Notebook.h"

#include <SUIT_Session.h>

#include <QKeyEvent>
#include <QLineEdit>

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
  myIsRangeSet( false ),
  myMinimum( 0.0 ),
  myMaximum( 99.99 )
{
  connectSignalsAndSlots();
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
  myIsRangeSet( false ),
  myMinimum( min ),
  myMaximum( max )
{
  connectSignalsAndSlots();
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
  myIsRangeSet( false ),
  myMinimum( min ),
  myMaximum( max )
{
  connectSignalsAndSlots();
}

/*!
  \brief Destructor.
*/
SalomeApp_DoubleSpinBox::~SalomeApp_DoubleSpinBox()
{
}

/*!
  \brief Connect signals and slots.
*/
void SalomeApp_DoubleSpinBox::connectSignalsAndSlots()
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
void SalomeApp_DoubleSpinBox::onEditingFinished()
{
  if( myTextValue.isNull() )
    myTextValue = text();

  setText( myTextValue );
}

/*!
  \brief This function is called when value is changed.
*/
void SalomeApp_DoubleSpinBox::onTextChanged( const QString& text )
{
  myTextValue = text;

  double value = 0;
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
double SalomeApp_DoubleSpinBox::valueFromText( const QString& text ) const
{
  double value = 0;
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
bool SalomeApp_DoubleSpinBox::isValid( QString& msg, QStringList& absentParams, bool toCorrect )
{
  double value;
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
void SalomeApp_DoubleSpinBox::setDefaultValue( const double value )
{
  myDefaultValue = value;
}

/*!
  \brief This function is used to set minimum and maximum values for this spinbox.
  \param min minimum value
  \param max maximum value
*/
void SalomeApp_DoubleSpinBox::setRange( const double min, const double max )
{
  QtxDoubleSpinBox::setRange( min, max );

  myIsRangeSet = true;
  myMinimum = min;
  myMaximum = max;
}

/*!
  \brief This function is used to set a current value for this spinbox.
  \param value current value
*/
void SalomeApp_DoubleSpinBox::setValue( const double value )
{
  QtxDoubleSpinBox::setValue( value );

  myCorrectValue = QtxDoubleSpinBox::textFromValue( value );
  myTextValue = myCorrectValue;
}

/*!
  \brief This function is used to set a text for this spinbox.
  \param value current value
*/
void SalomeApp_DoubleSpinBox::setText( const QString& value )
{
  lineEdit()->setText(value);
}

/*!
  \brief This function is used to determine whether input is valid.
  \return validating operation result
*/
SalomeApp_DoubleSpinBox::State SalomeApp_DoubleSpinBox::isValid( const QString& text, double& value, QStringList& absent ) const
{
  SearchState aSearchState = findVariable( text, value, absent );
  if( aSearchState == NotFound )
  {
    bool ok = false;
    value = QLocale().toDouble( text, &ok );
    if ( !ok )
      return NoVariable;
  }
  else if( aSearchState == IncorrectType )
    return Incompatible;
  else if( aSearchState == IncorrectExpression )
    return Invalid;

  if( !checkRange( value ) )
    return Invalid;

  return Acceptable;
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
bool SalomeApp_DoubleSpinBox::checkRange( const double value ) const
{
  if( !myIsRangeSet )
    return true;

  return value >= myMinimum && value <= myMaximum;
}

/*!
  \brief This function is used to determine whether input is a variable name and to get its value.
  \return status of search operation
*/
SalomeApp_DoubleSpinBox::SearchState SalomeApp_DoubleSpinBox::findVariable( const QString& name, double& value, QStringList& absent ) const
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
      if( aType == QVariant::Double || aType == QVariant::Int || aType == QVariant::String ) 
      {
        bool ok = false;
        value = aVariant.toDouble( &ok );
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
void SalomeApp_DoubleSpinBox::keyPressEvent( QKeyEvent* e )
{
  if ( e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter )
    QWidget::keyPressEvent( e );
  else
    QtxDoubleSpinBox::keyPressEvent( e );
}

/*!
  \brief This function is called when the spinbox recieves show event.
*/
void SalomeApp_DoubleSpinBox::showEvent( QShowEvent* )
{
  setText( myTextValue );
}
