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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#ifndef QDS_VALIDATOR_H
#define QDS_VALIDATOR_H

#include "QDS.h"

#include <QValidator>

/*!
  class QDS_IntegerValidator
*/
class QDS_EXPORT QDS_IntegerValidator : public QIntValidator
{
public:
  QDS_IntegerValidator( QObject* p = 0 );
  QDS_IntegerValidator( const QString& f, QObject* p = 0 );
  virtual ~QDS_IntegerValidator();

  virtual State validate( QString&, int& ) const;

private:
  QString myFilter;
};

/*!
  class QDS_DoubleValidator
*/
class QDS_DoubleValidator : public QDoubleValidator
{
public:
  QDS_DoubleValidator( QObject* p = 0 );
  QDS_DoubleValidator( const QString& f, QObject* p = 0 );
  virtual ~QDS_DoubleValidator();

  virtual State validate( QString&, int& ) const;

private:
  QString myFilter;
};

/*!
  class QDS_StringValidator
*/
class QDS_EXPORT QDS_StringValidator : public QValidator
{
public:
  QDS_StringValidator( QObject* p = 0 );
  QDS_StringValidator( const QString& f, QObject* p = 0 );
  QDS_StringValidator( const QString& ft, const QString& fg, QObject* p = 0 );
  virtual ~QDS_StringValidator();

  virtual State validate( QString&, int& ) const;

  int           length() const;
  void          setLength( const int );

private:
  int           myLen;
  QString       myFlags;
  QString       myFilter;
};

#endif
