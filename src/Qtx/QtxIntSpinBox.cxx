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
// File:      QtxIntSpinBox.cxx
// Author:    Sergey TELKOV

#include "QtxIntSpinBox.h"

#include <QLineEdit>
//#include <QApplication>

/*!
  Constructor
*/
QtxIntSpinBox::QtxIntSpinBox( QWidget* parent )
: QSpinBox( parent ),
myCleared( false )
{
  connect( lineEdit(), SIGNAL( textChanged( const QString& ) ), this, SLOT( onTextChanged( const QString& ) ) );
}

/*!
  Constructor
*/
QtxIntSpinBox::QtxIntSpinBox( int min, int max, int step, QWidget* parent )
: QSpinBox( parent ),
myCleared( false )
{
  setMinimum( min );
  setMaximum( max );
  setSingleStep( step );
  connect( lineEdit(), SIGNAL( textChanged( const QString& ) ), this, SLOT( onTextChanged( const QString& ) ) );
}

/*!
  Destructor
*/
QtxIntSpinBox::~QtxIntSpinBox()
{
}

/*!
  \return true if spin box is cleared
*/
bool QtxIntSpinBox::isCleared() const
{
  return myCleared;
}

/*!
  Changes cleared status of spin box
  \param on - new status
*/
void QtxIntSpinBox::setCleared( const bool on )
{
  if ( myCleared == on )
    return;
    
  myCleared = on;
  setSpecialValueText( specialValueText() );
}

QString QtxIntSpinBox::textFromValue( int val ) const
{
  return myCleared ? QString() : QSpinBox::textFromValue( val );
}

void QtxIntSpinBox::stepBy( int steps )
{
  myCleared = false;

  QSpinBox::stepBy( steps );
}

/*!
  SLOT: called if text is changed
*/
void QtxIntSpinBox::onTextChanged( const QString& )
{
  myCleared = false;
}
