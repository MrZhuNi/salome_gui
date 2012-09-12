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
// File:      QtxPathEdit.h
// Author:    Sergey TELKOV

#ifndef QTXPATHEDIT_H
#define QTXPATHEDIT_H

#include "Qtx.h"

#include <QFrame>

class QLineEdit;
class QValidator;

class QTX_EXPORT QtxPathEdit : public QFrame
{
  Q_OBJECT

public:
  QtxPathEdit( const Qtx::PathType, QWidget* = 0 );
  QtxPathEdit( QWidget* = 0 );
  virtual ~QtxPathEdit();

  QString           path() const;
  void              setPath( const QString& );

  Qtx::PathType     pathType() const;
  void              setPathType( const Qtx::PathType );

  QString           pathFilter() const;
  void              setPathFilter( const QString& );

  QLineEdit*        lineEdit() const;

  const QValidator* validator() const;
  void              setValidator( QValidator* );

  void              setBasePath( const QString& theBasePath );
  QString           basePath() const;

signals:
  void              returnPressed();
  void              pathChanged( const QString& );
  void              pathSelected( const QString& );
  void              beforeBrowse();

private slots:
  void              onBrowse( bool = false );

private:
  void              initialize();
  void              updateState();

private:
  QLineEdit*        myPath;
  Qtx::PathType     myType;
  QString           myFilter;
  QString           myBasePath;
};

#endif
