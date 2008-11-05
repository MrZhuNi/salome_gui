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
// File:      SalomeApp_DoubleSpinBox.h
// Author:    Oleg UVAROV

#ifndef SALOMEAPP_DOUBLESPINBOX_H
#define SALOMEAPP_DOUBLESPINBOX_H

#include "SalomeApp.h"

#include <QtxDoubleSpinBox.h>

#include <QValidator>

class SALOMEAPP_EXPORT SalomeApp_DoubleSpinBox : public QtxDoubleSpinBox
{
  Q_OBJECT

public:
  SalomeApp_DoubleSpinBox( QWidget* = 0 );
  SalomeApp_DoubleSpinBox( double, double, double = 1, QWidget* = 0 );
  SalomeApp_DoubleSpinBox( double, double, double, int, int, QWidget* = 0 );
  virtual ~SalomeApp_DoubleSpinBox();

  virtual double            valueFromText( const QString& ) const;
  virtual QString           textFromValue( double ) const;

  virtual QValidator::State validate( QString&, int& ) const;

  virtual bool              isValid() const;

  virtual void              setDefaultValue( const double );
  virtual void              setRange( double, double );

protected:
  double                    defaultValue() const;
  bool                      checkRange( const QString& ) const;
  bool                      findVariable( const QString&, double& ) const;

private:
  double                    myDefaultValue; 

  bool                      myIsRangeSet;
  double                    myMinimum;
  double                    myMaximum;
};

#endif
