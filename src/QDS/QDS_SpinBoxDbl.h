// Copyright (C) 2005  CEA/DEN, EDF R&D, OPEN CASCADE, PRINCIPIA R&D
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
#ifndef QDS_PINBOXDBL_H
#define QDS_PINBOXDBL_H

#include "QDS_Datum.h"

class QtxDblSpinBox;

class QDS_EXPORT QDS_SpinBoxDbl : public QDS_Datum
{
  Q_OBJECT

public:
  QDS_SpinBoxDbl( const QString&, QWidget* = 0, const int = All, const QString& = QString::null );
  virtual ~QDS_SpinBoxDbl();

  double           step() const;
  void             setStep( const double );

private slots:
  void             onValueChanged( double );

protected:
  QtxDblSpinBox*   spinBox() const;
  virtual QWidget* createControl( QWidget* );

  virtual QString  getString() const;
  virtual void     setString( const QString& );

  virtual void     unitSystemChanged( const QString& );
};

#endif 
