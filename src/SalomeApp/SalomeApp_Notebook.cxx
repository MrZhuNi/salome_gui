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
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"

#include <SALOMEDS_Study.hxx>

#include <QAbstractSpinBox>
#include <QStringList>
#include <QVariant>

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

void SalomeApp_Notebook::set( const QString& theName, const QVariant& theValue, bool theIsNew )
{
  SALOME::Parameter_ptr aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  bool isNew = CORBA::is_nil( aParam ) || theIsNew;

  myRecentValues[ theName ] = false;
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
  myRecentValues[ theName ] = true;
}

QVariant SalomeApp_Notebook::get( const QString& theName ) const
{
  return convert( myNotebook->GetParameter( theName.toLatin1().constData() ) );
}

QString SalomeApp_Notebook::expression( const QString& theName ) const
{
  SALOME::Parameter_var aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  return CORBA::is_nil( aParam ) ? QString::null : QString( aParam->GetExpression( true ) );
}

QVariant SalomeApp_Notebook::calculate( const QString& theExpr )
{
  SALOME::Parameter_var aParam = myNotebook->Calculate( theExpr.toLatin1().constData() );
  return convert( aParam.in() );
}

QVariant::Type SalomeApp_Notebook::getType( const QString& theName )
{
  QVariant::Type aRes = QVariant::Invalid;
  SALOME::Parameter_var aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  if( !CORBA::is_nil( aParam ) )
    switch( aParam->GetType() )
    {
    case SALOME::TBoolean:
      aRes = QVariant::Bool;
      break;
    case SALOME::TInteger:
      aRes = QVariant::Int;
      break;
    case SALOME::TReal:
      aRes = QVariant::Double;
      break;
    case SALOME::TString:
      aRes = QVariant::String;
      break;
    default:
      break;
    }
  return aRes;
}

bool SalomeApp_Notebook::isValid( const QString& theName ) const
{
  QMap<QString, bool>::const_iterator it = myRecentValues.find( theName );
  if( it != myRecentValues.end() )
    if( !it.value() )
      return false;

  SALOME::Parameter_var aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  if( CORBA::is_nil( aParam ) )
    return false;

  if( !aParam->IsValid() )
    return false;

  QStringList aDependencies = getParameters( theName );
  QStringListIterator anIter( aDependencies );
  while( anIter.hasNext() )
    if( !isValid( anIter.next() ) )
      return false;

  return true;
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

void SalomeApp_Notebook::update()
{
  myNotebook->Update();
}

void SalomeApp_Notebook::remove( const QString& theParamName )
{
  myNotebook->Remove( theParamName.toLatin1().constData() );
  QMap<QString, bool>::iterator it = myRecentValues.find( theParamName );
  if( it != myRecentValues.end() )
    myRecentValues.erase( it );
}

void SalomeApp_Notebook::rename( const QString& theOldName, const QString& theNewName )
{
  myNotebook->Rename( theOldName.toLatin1().constData(), theNewName.toLatin1().constData() );
  QMap<QString, bool>::iterator it = myRecentValues.find( theOldName );
  if( it != myRecentValues.end() )
    myRecentValues.insert( theNewName, myRecentValues.take( theOldName ) );
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
  va_list aSpins;
  va_start( aSpins, theFirstSpin );

  int aCounter = 0;
  QList<QAbstractSpinBox*> aSpinList;
  QAbstractSpinBox* aSpin = theFirstSpin;
  while( aSpin && aCounter < theCount )
  {
    aSpinList.append( aSpin );
    aSpin = va_arg( aSpins, QAbstractSpinBox* );
    aCounter++;
  }
  setParameters( theObject, aSpinList );
}

void SalomeApp_Notebook::setParameters( SALOME::ParameterizedObject_ptr theObject, QList<QAbstractSpinBox*> theSpinList )
{
  QStringList aParameters;
  QListIterator<QAbstractSpinBox*> anIter( theSpinList );
  while( anIter.hasNext() )
  {
    QAbstractSpinBox* aSpin = anIter.next();
    QString aText = aSpin->text();
    aParameters << aText;
  }
  setParameters( theObject, aParameters );
}

void SalomeApp_Notebook::setParameters( SALOME::ParameterizedObject_ptr theObject, const QStringList& theParameters )
{
  SALOME::StringArray_var aParams = new SALOME::StringArray();
  aParams->length( theParameters.count() );

  int i = 0;
  QStringListIterator anIter( theParameters );
  while( anIter.hasNext() )
  {
    QString aParameter = anIter.next();

    bool anIsValue = false;
    aParameter.toInt( &anIsValue );
    if( !anIsValue )
      aParameter.toDouble( &anIsValue );

    if( anIsValue )
      aParameter = "";
    else if( !isParameter( aParameter ) )
      myNotebook->AddExpression( aParameter.toLatin1().constData() );

    aParams[i] = CORBA::string_dup( aParameter.toLatin1().constData() );
    i++;
  }

  theObject->SetParameters( myNotebook.in(), aParams );
}

QStringList SalomeApp_Notebook::getObjectParameters( const QString& theComponent, const QString& theEntry ) const
{
  return convert( myNotebook->GetObjectParameters( theComponent.toLatin1().constData(),
                                                   theEntry.toLatin1().constData() ) );
}

QStringList SalomeApp_Notebook::getParameters( const QString& theParamName ) const
{
  return convert( myNotebook->GetParameters( theParamName.toLatin1().constData() ) );
}

void SalomeApp_Notebook::setRecentValues( const QMap<QString, bool>& theRecentValues )
{
  myRecentValues = theRecentValues;
}

char* SalomeApp_Notebook::dump()
{
  return myNotebook->Dump();
}
