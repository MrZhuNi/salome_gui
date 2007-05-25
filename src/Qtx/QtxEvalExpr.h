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
// File:      QtxEvalExpr.h
// Author:    Alexander SOLOVYOV

#ifndef QTX_EVALEXPR_H
#define QTX_EVALEXPR_H

#include "Qtx.h"

#include <QList>
#include <QStack>
#include <QVariant>

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

class QtxEvalSet;
class QtxEvalParser;

class QTX_EXPORT QtxEvalExpr
{
public:
  /*!
    \enum Error
    \brief Errors during parsing
  */
  typedef enum
  {
    OK,               /*! \var All right */
	  OperandsNotMatch, /*! \var Types of arguments are invalid for this operation */
	  InvalidResult,    /*! \var Operation cannot find result (for example, division by zero) */
	  InvalidOperation, /*! \var Name of operation is unknown */
    OperationsNull,   /*! \var Internal operations pointer of parser is null */
	  InvalidToken,     /*! \var It isn't operation, parameter of value  */
	  CloseExpected,    /*! \var Close bracket is expected */
	  ExcessClose,      /*! \var The one of close bracket is excess */
    BracketsNotMatch, /*! \var Last open and this close bracket are different, for example [) */
	  StackUnderflow,   /*! \var There is no arguments in stack for operation */
	  ExcessData        /*! \var The parsing is finished, but there is more then one value in stack */
  } Error;

public:
  QtxEvalExpr( const QString& = QString() );
  QtxEvalExpr( const bool, const QString& = QString() );
  ~QtxEvalExpr();

  QVariant           calculate( const QString& = QString() );

  QString            expression() const;
  void               setExpression( const QString& );

  QtxEvalExpr::Error error() const;
  QtxEvalParser*     parser() const;

  QList<QtxEvalSet*> operationSets() const;
  QtxEvalSet*        operationSet( const QString& ) const;
  void               removeOperationSet( QtxEvalSet* );
  void               insertOperationSet( QtxEvalSet*, const int = -1 );

private:
  void               intialize( const bool, const QString& );

private:
  QString            myExpr;
  QtxEvalParser*     myParser;
};

/*!
  \class QtxEvalParser

  This class allows to calculate values of expressions using different set of operations.
  It is provided some of standard set of operations (arithmetics, logic, strings, etc).
  This parser allows to use parameters with help of methods has(), set(), remove(), value(). It uses
  postfix representation of expressions and uses class QtxOperations in order to make certain operation
  Every instance of parser contains only one postfix, so that if expression has been changed, then postfix
  must be rebuilt. In order to increase performance of frequent calculation for many of expressions it is 
  recommended to use different instances of parser for expressions

*/
class QTX_EXPORT QtxEvalParser
{
public:
  QtxEvalParser();
  virtual ~QtxEvalParser();

  QVariant           calculate();
  QVariant           calculate( const QString& );
  bool               setExpression( const QString& );

  QList<QtxEvalSet*> operationSets() const;
  QtxEvalSet*        operationSet( const QString& ) const;
  void               removeOperationSet( QtxEvalSet* );
  void               insertOperationSet( QtxEvalSet*, const int = -1 );

  bool               autoDeleteOperationSets() const;
  void               setAutoDeleteOperationSets( const bool );

  virtual void       clearParameters();
  virtual bool       removeParameter( const QString& name );
  virtual QVariant   parameter( const QString& name ) const;
  virtual bool       hasParameter( const QString& name ) const;
  virtual void       setParameter( const QString& name, const QVariant& value );
  QStringList        parameters() const;

  QtxEvalExpr::Error error() const;

  bool               firstInvalid( QString& ) const;
  void               removeInvalids();
  QString            dump() const;

  static QString     toString( const QList<QVariant>& );

protected:
  /*!
    \enum PostfixItemType
    \brief Types of postfix representation elements
  */  
  typedef enum
  {
    Value, /*! \var Value (number, string, etc.)*/
    Param, /*! \var Parameter */
    Open,  /*! \var Open bracket */
    Close, /*! \var Close bracket */
    Pre,   /*! \var Unary prefix operation */
    Post,  /*! \var Unary postfix operation */
    Binary /*! \var Binary operation */
  } PostfixItemType;

  /*! \var postfix representation element */
  typedef struct
  {
    QVariant          myValue;
    PostfixItemType   myType;
  } PostfixItem;

  /*! \var postfix representation */
  typedef QList<PostfixItem>      Postfix;
  typedef QList<QtxEvalSet*>      SetList;
  typedef QMap<QString, QVariant> ParamMap;

protected:
  QString            dump( const Postfix& ) const;
  virtual bool       prepare( const QString&, Postfix& );
  virtual bool       setOperationTypes( Postfix& );
  virtual bool       sort( const Postfix&, Postfix&, const QStringList&,
                           const QStringList&, int f = -1, int l = -1 );

  virtual bool       parse( const QString& );
  virtual void       setError( const QtxEvalExpr::Error );

  bool               calculate( const QString&, QVariant&, QVariant& );

  static int         search( const QStringList&, const QString&,
                             int offset, int& matchLen, int& listind );
  static QString     note( const QString& str, int pos, int len );
  static int         globalBrackets( const Postfix&, int, int );

private:
  void               operationList( QStringList& ) const;
  void               bracketsList( QStringList&, bool ) const;
  bool               createValue( const QString&, QVariant& ) const;
  int                priority( const QString&, bool isBin ) const;
  QtxEvalExpr::Error isValid( const QString&,
                              const QVariant::Type, const QVariant::Type ) const;
  QtxEvalExpr::Error calculation( const QString&, QVariant&, QVariant& ) const;

  bool               checkOperations() const;

private:
  SetList            mySets;
  QtxEvalExpr::Error myError;
  ParamMap           myParams;
  Postfix            myPostfix;
  bool               myAutoDel;
};

class QTX_EXPORT QtxEvalSet
{
public:
  QtxEvalSet();
  virtual ~QtxEvalSet();

  virtual QString            name() const = 0;

  virtual void               operationList( QStringList& ) const = 0;
  //list of possible operations

  virtual void               bracketsList( QStringList&, bool open ) const = 0;
  //list of open/close brackets

  virtual bool               createValue( const QString&, QVariant& ) const;
  //by default, the String value will be set, it corresponds to parameter
  //base method returns false (always parameter)
  //successor's method returns true if it has created custom value
  //or call base if it hasn't

  virtual int                priority( const QString&, bool isBin ) const = 0;
  //returns prioritet of operation;
  //if operation is impossible, it must return 0 or less

  virtual QtxEvalExpr::Error isValid( const QString&, const QVariant::Type,
                                      const QVariant::Type ) const = 0;
  //return OK if this parameter types is valid for operation
  //return OperandsNotMatch or InvalidOperation otherwise

  virtual QtxEvalExpr::Error calculate( const QString&, QVariant&, QVariant& ) const = 0;
  //process binary operation with values
  //for unary operation the second QVariant will be passed as invalid
};

// ------------------------------- Standard operation sets ------------------------------

/*!
  \class QtxEvalSetBase
  Provides base functionality for standard sets of operations for parser
*/
class QTX_EXPORT QtxEvalSetBase : public QtxEvalSet
{
public:
  QtxEvalSetBase();
  virtual ~QtxEvalSetBase();

  virtual void               operationList( QStringList& ) const;
  virtual void               bracketsList( QStringList&, bool open ) const;

  virtual QtxEvalExpr::Error isValid( const QString&, const QVariant::Type,
                                      const QVariant::Type ) const;
protected: 
  typedef QList<QVariant::Type> ListOfTypes;

  void                       addTypes( const ListOfTypes& );
  void                       addOperations( const QStringList& );

private:
  QStringList                myOpers;
  ListOfTypes                myTypes;
};

/*!
  \class QtxEvalSetArithmetic
  Provides set of arithmetic operations for parser
*/
class QTX_EXPORT QtxEvalSetArithmetic : public QtxEvalSetBase
{
public:
  QtxEvalSetArithmetic();
  virtual ~QtxEvalSetArithmetic();

  virtual bool               createValue( const QString&, QVariant& ) const;
  virtual int                priority( const QString&, bool isBin ) const;
  virtual QtxEvalExpr::Error calculate( const QString&, QVariant&, QVariant& ) const;

  static QString             Name();
  virtual QString            name() const;
};

/*!
  \class QtxEvalSetLogic
  Provides set of logic operations for parser
*/
class QTX_EXPORT QtxEvalSetLogic : public QtxEvalSetBase
{
public:
  QtxEvalSetLogic();
  virtual ~QtxEvalSetLogic();

  virtual bool               createValue( const QString&, QVariant& ) const;
  virtual int                priority( const QString&, bool isBin ) const;
  virtual QtxEvalExpr::Error calculate( const QString&, QVariant&, QVariant& ) const;

  static QString             Name();
  virtual QString            name() const;

private:
  bool                       booleanValue( const QVariant& v ) const;
};

/*!
  \class QtxEvalSetMath
  Provides set of more complex operations (mathematics functions) for parser (sqrt, sin, cos, etc)
*/
class QTX_EXPORT QtxEvalSetMath : public QtxEvalSetBase
{
public:
  QtxEvalSetMath();
  virtual ~QtxEvalSetMath();

  virtual bool               createValue( const QString&, QVariant& ) const;
  virtual int                priority( const QString&, bool isBin ) const;
  virtual QtxEvalExpr::Error calculate( const QString&, QVariant&, QVariant& ) const;

  static QString             Name();
  virtual QString            name() const;
};

/*!
  \class QtxEvalSetString
  Provides set of string operations for parser
*/
class QTX_EXPORT QtxEvalSetString : public QtxEvalSetBase
{
public:
  QtxEvalSetString();
  virtual ~QtxEvalSetString();

  virtual bool               createValue( const QString&, QVariant& ) const;
  virtual int                priority( const QString&, bool isBin ) const;
  virtual QtxEvalExpr::Error calculate( const QString&, QVariant&, QVariant& ) const;

  static QString             Name();
  virtual QString            name() const;
};

/*!
  \class QtxEvalSetSets
  Provides set of operations with sets for parser
*/
class QTX_EXPORT QtxEvalSetSets : public QtxEvalSetBase
{
public:
  typedef QList<QVariant> ValueSet;

public:
  QtxEvalSetSets();
  virtual ~QtxEvalSetSets();

  virtual void               bracketsList( QStringList&, bool open ) const;
  virtual int                priority( const QString&, bool isBin ) const;
  virtual QtxEvalExpr::Error isValid( const QString&, const QVariant::Type,
                                      const QVariant::Type ) const;
  virtual QtxEvalExpr::Error calculate( const QString&, QVariant&, QVariant& ) const;

  static void add( ValueSet&, const QVariant& );
  static void add( ValueSet&, const ValueSet& );
  static void remove( ValueSet&, const QVariant& );
  static void remove( ValueSet&, const ValueSet& );

  static QString             Name();
  virtual QString            name() const;
};

/*!
  \class QtxEvalSetConst
  Provides different standard constants
*/
class QTX_EXPORT QtxEvalSetConst : public QtxEvalSet
{
public:
  QtxEvalSetConst();
  virtual ~QtxEvalSetConst();

  static QString             Name();
  virtual QString            name() const;

  virtual bool               createValue( const QString&, QVariant& ) const;
};

#endif
