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
  SALOMEDS_Study* aStudy = dynamic_cast<SALOMEDS_Study*>( theStudy->studyDS().operator->() );
  if( aStudy )
    myNotebook = aStudy->GetStudy()->GetNotebook();
}

SalomeApp_Notebook::~SalomeApp_Notebook()
{
}

bool SalomeApp_Notebook::isParameter( const QString& theName ) const
{
  SALOME::Parameter_ptr aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  return !CORBA::is_nil( aParam );
}

void SalomeApp_Notebook::set( const QString& theName, const QVariant& theValue )
{
  SALOME::Parameter_ptr aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  bool isNew = CORBA::is_nil( aParam );

  switch( theValue.type() )
  {
  case QVariant::Int:
  case QVariant::UInt:
    if( isNew )
      myNotebook->AddInteger( theName.toLatin1().constData(), theValue.toInt() );
    else
      aParam->SetInteger( theValue.toInt() );
    break;

  case QVariant::Double:
    if( isNew )
      myNotebook->AddReal( theName.toLatin1().constData(), theValue.toDouble() );
    else
      aParam->SetReal( theValue.toDouble() );
    break;

  case QVariant::Bool:
    if( isNew )
      myNotebook->AddBoolean( theName.toLatin1().constData(), theValue.toBool() );
    else
      aParam->SetBoolean( theValue.toBool() );
    break;

  case QVariant::String:
    {
      if( isNew )
        myNotebook->AddNamedExpression( theName.toLatin1().constData(),
                                        theValue.toString().toLatin1().constData() );
      else
        aParam->SetExpression( theValue.toString().toLatin1().constData() );
      break;
    }
  }
}

QVariant SalomeApp_Notebook::get( const QString& theName ) const
{
  return convert( myNotebook->GetParameter( theName.toLatin1().constData() ) );
}

QVariant SalomeApp_Notebook::calculate( const QString& theExpr )
{
  if( CORBA::is_nil( myTmp ) )
  {
    static const char TMP_NAME[] = "__notebook__tmp__";
    myTmp = myNotebook->GetParameter( TMP_NAME );
    if( CORBA::is_nil( myTmp ) )
    {
      myNotebook->AddReal( TMP_NAME, 0 );
      myTmp = myNotebook->GetParameter( TMP_NAME );
    }
  }
  myTmp->SetExpression( theExpr.toLatin1().constData() );
  myTmp->Update( myNotebook._retn() );
  return convert( myTmp );
}

QVariant SalomeApp_Notebook::convert( SALOME::Parameter_ptr theParam ) const
{
  QVariant aRes;
  if( !CORBA::is_nil( theParam ) )
    switch( theParam->GetType() )
    {
    case SALOME::TBoolean:
      aRes = theParam->AsBoolean();
      break;
    case SALOME::TInteger:
      aRes = (int)theParam->AsInteger();
      break;
    case SALOME::TReal:
      aRes = theParam->AsReal();
      break;
    case SALOME::TString:
      aRes = theParam->AsString();
      break;
    }
  return aRes;
}

void SalomeApp_Notebook::update( bool theOnlyParameters )
{
  myNotebook->Update( theOnlyParameters );
}

void SalomeApp_Notebook::remove( const QString& theParamName )
{
  myNotebook->Remove( theParamName.toLatin1().constData() );
}

void SalomeApp_Notebook::rename( const QString& theOldName, const QString& theNewName )
{
  myNotebook->Rename( theOldName.toLatin1().constData(), theNewName.toLatin1().constData() );
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

QStringList SalomeApp_Notebook::absentParameters( const QString& theExpr ) const
{
  return convert( myNotebook->AbsentParameters( theExpr.toLatin1().constData() ) );
}

void SalomeApp_Notebook::setParameters( SALOME::ParameterizedObject_ptr theObject, int theCount, QAbstractSpinBox* theFirstSpin, ... )
{
  SALOME::StringArray_var aParams = new SALOME::StringArray();
  aParams->length( theCount );

  QAbstractSpinBox** aSpinArray = &theFirstSpin;
  for( int i=0; i<theCount; i++, aSpinArray++ )
  {
    QAbstractSpinBox* aSpin = *aSpinArray;
    QString aText = aSpin->text();

    bool anIsValue = false;
    if( dynamic_cast<SalomeApp_DoubleSpinBox*>( aSpin ) )
      aText.toDouble( &anIsValue );
    else if( dynamic_cast<SalomeApp_IntSpinBox*>( aSpin ) )
      aText.toInt( &anIsValue );

    if( anIsValue )
      aText = "";

    aParams[i] = CORBA::string_dup( aText.toLatin1().constData() );
  }

  theObject->SetParameters( myNotebook._retn(), aParams );
}

char* SalomeApp_Notebook::dump()
{
  return myNotebook->Dump();
}
