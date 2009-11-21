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
// File:      SalomeApp_Notebook.cxx
// Created:   11/21/2009 10:08:37 AM
// Author:    Alexandre SOLOVYOV
//

#include "SalomeApp_Notebook.h"
#include "SalomeApp_Study.h"
#include "SalomeApp_DoubleSpinBox.h"
#include "SalomeApp_IntSpinBox.h"
#include <SALOMEDS_Study.hxx>
#include <QVariant>
#include <QStringList>

SalomeApp_Notebook::SalomeApp_Notebook( SalomeApp_Study* theStudy )
{
  SALOME::Notebook_var aRes;
  SALOMEDS_Study* aStudy = dynamic_cast<SALOMEDS_Study*>( theStudy->studyDS().operator->() );
  if( aStudy )
    aRes = aStudy->GetStudy()->GetNotebook();
  myNotebook = aRes._retn();
}

SalomeApp_Notebook::~SalomeApp_Notebook()
{
}

void SalomeApp_Notebook::set( const QString& theName, const QVariant& theValue )
{
  const char* aName = theName.toLatin1().constData();
  SALOME::Parameter_ptr aParam = myNotebook->GetParameter( aName );
  bool isNew = CORBA::is_nil( aParam );

  switch( theValue.type() )
  {
  case QVariant::Int:
  case QVariant::UInt:
    if( isNew )
      myNotebook->AddInteger( aName, theValue.toInt() );
    else
      aParam->SetInteger( theValue.toInt() );
    break;

  case QVariant::Double:
    if( isNew )
      myNotebook->AddReal( aName, theValue.toDouble() );
    else
      aParam->SetReal( theValue.toDouble() );
    break;

  case QVariant::Bool:
    if( isNew )
      myNotebook->AddBoolean( aName, theValue.toBool() );
    else
      aParam->SetBoolean( theValue.toBool() );
    break;

  case QVariant::String:
    {
      const char* aData = theValue.toString().toLatin1().constData();
      if( isNew )
        myNotebook->AddNamedExpression( aName, aData );
      else
        aParam->SetExpression( aData );
      break;
    }
  }
}

QVariant SalomeApp_Notebook::get( const QString& theName ) const
{
  QVariant aRes;
  SALOME::Parameter_ptr aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  if( !CORBA::is_nil( aParam ) )
    switch( aParam->GetType() )
    {
    case SALOME::TBoolean:
      aRes = aParam->AsBoolean();
      break;
    case SALOME::TInteger:
      aRes = (int)aParam->AsInteger();
      break;
    case SALOME::TReal:
      aRes = aParam->AsReal();
      break;
    case SALOME::TString:
      aRes = aParam->AsString();
      break;
    }
  return aRes;
}

void SalomeApp_Notebook::update()
{
  myNotebook->Update();
}

QStringList SalomeApp_Notebook::convert( SALOME::StringArray* theList ) const
{
  QStringList aRes;

  int n = theList->length();
  for( int i = 0; i < n; i++ )
    aRes.append( CORBA::string_dup( theList->operator[]( i ) ) );

  return aRes;
}

QStringList SalomeApp_Notebook::parameters() const
{
  return convert( myNotebook->Parameters() );
}

QStringList SalomeApp_Notebook::absentParameters() const
{
  return convert( myNotebook->AbsentParameters() );
}

void SalomeApp_Notebook::setParameters( SALOME::ParameterizedObject_ptr theObject, int theCount, QAbstractSpinBox* theFirstSpin, ... )
{
  SALOME::StringArray aParams;
  aParams.length( theCount );
  QAbstractSpinBox** aSpin = &theFirstSpin;
  for( int i=0; i<theCount; i++, aSpin++ )
  {
    SalomeApp_DoubleSpinBox* aDbl = dynamic_cast<SalomeApp_DoubleSpinBox*>( *aSpin );
    if( aDbl )
    {
      continue;
    }

    SalomeApp_IntSpinBox* anInt = dynamic_cast<SalomeApp_IntSpinBox*>( *aSpin );
    if( anInt )
    {
      continue;
    }

    aParams[i] = CORBA::string_dup( "" );
  }

  theObject->SetParameters( myNotebook, aParams );
}
