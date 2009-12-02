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

QString SalomeApp_Notebook::expression( const QString& theName ) const
{
  SALOME::Parameter_var aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  return CORBA::is_nil( aParam ) ? QString::null : QString( aParam->GetExpression( true ) );
}

QVariant SalomeApp_Notebook::calculate( const QString& theExpr )
{
  if( CORBA::is_nil( myTmp ) )
  {
    static const char TMP_NAME[] = "__tmp__";
    myTmp = myNotebook->GetParameter( TMP_NAME );
    if( CORBA::is_nil( myTmp ) )
    {
      myNotebook->AddReal( TMP_NAME, 0 );
      myTmp = myNotebook->GetParameter( TMP_NAME );
    }
  }
  myTmp->SetExpression( theExpr.toLatin1().constData() );
  myTmp->Update( myNotebook.in() );
  return convert( myTmp );
}

bool SalomeApp_Notebook::isValid( const QString& theName ) const
{
  SALOME::Parameter_var aParam = myNotebook->GetParameter( theName.toLatin1().constData() );
  return CORBA::is_nil( aParam ) ? false : aParam->IsValid();
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
  QList<QAbstractSpinBox*> aSpinList;
  QAbstractSpinBox** aSpinArray = &theFirstSpin;
  for( int i = 0; i < theCount; i++, aSpinArray++ )
    aSpinList << *aSpinArray;
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

    aParams[i] = CORBA::string_dup( aParameter.toLatin1().constData() );
    i++;
  }

  theObject->SetParameters( myNotebook.in(), aParams );
}

QString SalomeApp_Notebook::getParameters( const QString& theComponent, const QString& theEntry )
{
  return QString( myNotebook->GetParameters( theComponent.toLatin1().constData(),
                                             theEntry.toLatin1().constData() ) );
}

char* SalomeApp_Notebook::dump()
{
  return myNotebook->Dump();
}
