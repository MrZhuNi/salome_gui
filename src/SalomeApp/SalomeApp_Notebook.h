//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// File:      SalomeApp_Notebook.h
// Created:   11/21/2009 10:05:25 AM
// Author:    Alexandre SOLOVYOV
//
#ifndef SALOMEAPP_NOTEBOOK_H
#define SALOMEAPP_NOTEBOOK_H

#include "SalomeApp.h"
#include CORBA_CLIENT_HEADER( SALOME_Notebook )

class SalomeApp_Study;
class QString;
class QVariant;
class QStringList;
class QAbstractSpinBox;

/*!
  \class SalomeApp_Notebook
  \brief Wrapper of notebook functionality for the GUI
*/

class SALOMEAPP_EXPORT SalomeApp_Notebook
{
public:
  SalomeApp_Notebook( SalomeApp_Study* theStudy );
  virtual ~SalomeApp_Notebook();

  bool isParameter( const QString& theName ) const;
  void set( const QString& theName, const QVariant& theValue );
  QVariant get( const QString& theName ) const;

  void update();

  QStringList parameters() const;
  QStringList absentParameters() const;

  void setParameters( SALOME::ParameterizedObject_ptr theObject, int theCount, QAbstractSpinBox* theFirstSpin, ... );

protected:
  QStringList convert( SALOME::StringArray* theArray ) const;

private:
  SALOME::Notebook_ptr myNotebook;
};

#endif
