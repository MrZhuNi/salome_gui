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
// File:      QtxValidator.cxx
// Author:    Alexandre SOLOVYOV

#include "QtxValidator.h"

/*!
  \class QtxIntValidator
  Validator for integer numbers with possibility to fix up the invalid value
*/

/*!
  Constructor
*/
QtxIntValidator::QtxIntValidator( QObject* parent )
: QIntValidator( parent )
{
}

/*!
  Constructor
  \param bot - minimal possible value
  \param top - maximal possible value
  \param parent - parent object
*/
QtxIntValidator::QtxIntValidator( const int bot, const int top, QObject* parent )
: QIntValidator( bot, top, parent )
{
}

/*!
  Destructor
*/
QtxIntValidator::~QtxIntValidator()
{
}

/*!
  Corrects string: if it represent integer value less then bottom, it becomes equal to bottom,
  if it is more then top, it becomes equal to top, if it isn't number is becomes '0'
*/
void QtxIntValidator::fixup( QString& str ) const
{
  bool ok = false;
  int i = str.toInt( &ok );
  if ( ok )
  {
    if ( i < bottom() )
      str = QString::number( bottom() );
    else if( i > top() )
      str = QString::number( top() );
  }
  else
    str = QString ( "0" );
}

/*!
  \class QtxDoubleValidator
  Validator for double numbers with possibility to fix up the invalid value
*/

/*!
  Constructor
  \param parent - parent object
*/
QtxDoubleValidator::QtxDoubleValidator( QObject* parent )
: QDoubleValidator( parent )
{
}

/*!
  Constructor
  \param bot - minimal possible value
  \param top - maximal possible value
  \param dec - number of digits
  \param parent - parent object
*/
QtxDoubleValidator::QtxDoubleValidator( const double bot, const double top,
                                  const int dec, QObject* parent )
: QDoubleValidator( bot, top, dec, parent )
{
}

/*!
  Destructor
*/
QtxDoubleValidator::~QtxDoubleValidator()
{
}

/*!
  Corrects string: if it represent double value less then bottom, it becomes equal to bottom,
  if it is more then top, it becomes equal to top, if it isn't number is becomes '0'
*/
void QtxDoubleValidator::fixup( QString& str ) const
{
  bool ok = false;
  double d = str.toDouble( &ok );
  if ( ok )
  {
    if ( d < bottom() )
      str = QString::number( bottom() );
    else if ( d > top() )
      str = QString::number( top() );
  }
  else
    str = QString( "0" );
}
