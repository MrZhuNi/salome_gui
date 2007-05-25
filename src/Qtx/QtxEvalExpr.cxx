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

#include "QtxEvalExpr.h"

#include <math.h>

/*!
  \class QtxEvalExpr
  Evaluate string expression.
*/

/*!
  Constructor
*/
QtxEvalExpr::QtxEvalExpr( const QString& expr )
{
  intialize( true, expr );
}

/*!
  Constructor
*/
QtxEvalExpr::QtxEvalExpr( const bool stdSets, const QString& expr )
{
  intialize( stdSets, expr );
}

/*!
  Destructor
*/
QtxEvalExpr::~QtxEvalExpr()
{
  delete myParser;
}

void QtxEvalExpr::intialize( const bool stdSets, const QString& expr )
{
  myParser = new QtxEvalParser();
  if ( stdSets )
  {
    myParser->setAutoDeleteOperationSets( true );
    myParser->insertOperationSet( new QtxEvalSetArithmetic() );
    myParser->insertOperationSet( new QtxEvalSetLogic() );
    myParser->insertOperationSet( new QtxEvalSetString() );
    myParser->insertOperationSet( new QtxEvalSetMath() );
    myParser->insertOperationSet( new QtxEvalSetSets() );
    myParser->insertOperationSet( new QtxEvalSetConst() );
  }
  setExpression( expr );
}

/*!
  Calculates the expression
*/
QVariant QtxEvalExpr::calculate( const QString& expr )
{
  if ( !expr.isEmpty() )
    setExpression( expr );
  return myParser->calculate();
}

/*!
  Returns the expression string
*/
QString QtxEvalExpr::expression() const
{
  return myExpr;
}

/*!
  Sets the expression string
*/
void QtxEvalExpr::setExpression( const QString& expr )
{
  if ( expr == expression() )
    return;

  myExpr = expr;
  myParser->setExpression( myExpr );
}

/*!
  Returns the code of the last error
*/
QtxEvalExpr::Error QtxEvalExpr::error() const
{
  return myParser->error();
}

/*!
  Returns parser
*/
QtxEvalParser* QtxEvalExpr::parser() const
{
  return myParser;
}

QList<QtxEvalSet*> QtxEvalExpr::operationSets() const
{
  return myParser->operationSets();
}

void QtxEvalExpr::insertOperationSet( QtxEvalSet* set, const int idx )
{
  myParser->insertOperationSet( set, idx );
}

void QtxEvalExpr::removeOperationSet( QtxEvalSet* set )
{
  myParser->removeOperationSet( set );
}

QtxEvalSet* QtxEvalExpr::operationSet( const QString& name ) const
{
  return myParser->operationSet( name );
}

bool QtxEvalExpr::autoDeleteOperationSets() const
{
  return myParser->autoDeleteOperationSets();
}

void QtxEvalExpr::setAutoDeleteOperationSets( const bool on )
{
  myParser->setAutoDeleteOperationSets( on );
}

/*!
  \class QtxEvalParser
  Expression parser.
*/

/*!
  Constructor
*/
QtxEvalParser::QtxEvalParser()
: myAutoDel( false )
{
  setError( QtxEvalExpr::OK );
}

/*!
  Destructor
*/
QtxEvalParser::~QtxEvalParser()
{
  if ( autoDeleteOperationSets() )
    qDeleteAll( mySets );
}

QList<QtxEvalSet*> QtxEvalParser::operationSets() const
{
  return mySets;
}

QtxEvalSet* QtxEvalParser::operationSet( const QString& name ) const
{
  QtxEvalSet* set = 0;
  for ( SetList::const_iterator it = mySets.begin(); it != mySets.end() && !set; ++it )
  {
    if ( (*it)->name() == name )
      set = *it;
  }
  return set;
}

void QtxEvalParser::insertOperationSet( QtxEvalSet* set, const int idx )
{
  if ( mySets.contains( set ) )
    return;

  int index = idx < 0 ? mySets.count() - 1 : idx;
  index = qMin( index, mySets.count() - 1 );
  mySets.insert( index, set );
}

void QtxEvalParser::removeOperationSet( QtxEvalSet* set )
{
  mySets.removeAll( set );
}

bool QtxEvalParser::autoDeleteOperationSets() const
{
  return myAutoDel;
}

void QtxEvalParser::setAutoDeleteOperationSets( const bool on )
{
  myAutoDel = on;
}

/*!
  Search elements of list as substrings starting on 'offset'
  \returns the least position of substrings inside string
  \param list - list of substrings
  \param str - string where search
  \param offset - starting index for search
  \param matchLen - the length of appropriate substring
  \param listind - list index of appropriate substring
*/
int QtxEvalParser::search( const QStringList& list, const QString& str,
                           int offset, int& matchLen, int& listind )
{
  int min = -1;
  QStringList::const_iterator anIt = list.begin(), aLast = list.end();
  for ( int ind = 0; anIt != aLast; anIt++, ind++ )
  {
    int pos = str.indexOf( *anIt, offset );
    if ( pos >= 0 && ( min < 0 || min > pos ||
                       ( min == pos && matchLen < (int)(*anIt).length() ) ) )
    {
      min = pos;
      listind = ind;
      matchLen = (*anIt).length();
    }
  }
  if ( min < 0 )
    matchLen = 0;
  return min;
}

/*!
  \return substring
  \param str - string
  \param pos - start position of substring
  \param len - length of substring
*/
QString QtxEvalParser::note( const QString& str, int pos, int len )
{
    return str.mid( pos, len ).trimmed();
}

/*!
  First step of parsing: finding tokens, determining its types and creating of unsorted pseudo-postfix (with brackets)
  \param expr - string expression
  \param post - postfix to be created
*/
bool QtxEvalParser::prepare( const QString& expr, Postfix& post )
{
  int pos = 0;
  int len = expr.length();
  QStack<int> aBracketStack;
  QStringList anOpers, anOpenBr, aCloseBr;
  if ( !checkOperations() )
    return false;

  bracketsList( anOpenBr, true );
  bracketsList( aCloseBr, false );
  operationList( anOpers );

  while ( pos < len && error() == QtxEvalExpr::OK )
  {
    PostfixItem item;
    while ( expr[pos].isSpace() && pos < len )
      pos++;
    if ( pos >= len )
      break;

    int mBrLen = 0, mLen = 0, br_ind = -1, op_ind = -1;
    int oPos = search( anOpenBr, expr, pos, mBrLen, br_ind );
    int cPos = oPos == pos ? -1 : search( aCloseBr, expr, pos, mBrLen, br_ind );
    int opPos = search( anOpers, expr, pos, mLen, op_ind );

    if ( expr[pos] == QChar( '\'' ) )
    {
      int vpos = pos + 1;
      while ( vpos < (int)expr.length() && expr[vpos] != QChar( '\'' ) )
        vpos++;

      mLen = vpos - pos + 1;

      int res = createValue( note( expr, pos, mLen ), item.myValue );
      item.myType = res ? Value : Param;
      post.append( item );
      pos = vpos + 1;
      continue;
    }

    if ( oPos == pos )
    {
      aBracketStack.push( br_ind );
      item.myValue = note( expr, pos, mBrLen );
      item.myType = Open;
      post.append( item );
    }
    else if ( cPos == pos )
    {
      if ( aBracketStack.count() == 0 )
      {
        setError( QtxEvalExpr::ExcessClose );
        break;
      }
      if ( br_ind != aBracketStack.top() )
      {
        setError( QtxEvalExpr::BracketsNotMatch );
        break;
      }
      else
      {
        aBracketStack.pop();
        item.myValue = note( expr, pos, mBrLen );
        item.myType = Close;
        post.append( item );
      }
    }
    else
      mBrLen = 0;

    if ( opPos == pos )
    {
      mBrLen = 0;
      item.myValue = note( expr, pos, mLen );
      item.myType = Binary;

      if ( oPos == pos )
        post.insert( post.count() - 1, item );
      else
        post.append( item );
    }
    else
    {
      mLen = 0;
      if ( oPos != pos && cPos != pos )
      {
	      int i; 
        for ( i = pos + 1; i < (int)expr.length(); i++ )
        {
          if ( expr[i].isSpace() )
            break;
        }

        int vpos = i;
        if ( oPos >= 0 && oPos < vpos )
          vpos = oPos;
        if ( cPos >= 0 && cPos < vpos )
          vpos = cPos;
        if ( opPos >= 0 && opPos < vpos )
          vpos = opPos;

        while( vpos < (int)expr.length() && 
               ( expr[vpos].isLetter() || expr[vpos].isDigit() || expr[vpos]=='_' ) )
          vpos++;

        mLen = vpos - pos;
        bool res = createValue( note( expr, pos, mLen ), item.myValue );
        item.myType = res ? Value : Param;
        post.append( item );
      }
    }

    pos += mBrLen + mLen;
  }

  //Bracket checking
  int brValue = 0;
  for ( Postfix::iterator anIt = post.begin(); anIt != post.end(); ++anIt )
  {
    if ( (*anIt).myType == Open )
      brValue++;
    else if ( (*anIt).myType == Close )
    {
      if ( brValue > 0 )
        brValue--;
      else
      {
        setError( QtxEvalExpr::ExcessClose );
        break;
      }
    }
  }

  if ( brValue > 0 )
    setError( QtxEvalExpr::CloseExpected );

  return error() == QtxEvalExpr::OK;
}

/*!
  Second step of parsing: determining types of operations
  \param post - unsorted postfix
*/
bool QtxEvalParser::setOperationTypes( Postfix& post )
{
  if ( !checkOperations() )
    return false;

  QStringList anOpen, aClose;
  bracketsList( anOpen, true );
  bracketsList( aClose, false );

  Postfix::iterator aPrev, aNext;
  for ( Postfix::iterator anIt = post.begin(); anIt != post.end(); ++anIt )
  {
    aPrev = anIt;
    aPrev--;
    aNext = anIt;
    aNext++;
    if ( (*anIt).myType != Binary )
      continue;

    if ( ( anIt == post.begin() || (*aPrev).myType == Open ||
           (*aPrev).myType == Pre || (*aPrev).myType == Binary ) &&  aNext != post.end() &&
         ( (*aNext).myType == Value || (*aNext).myType == Param ||
           (*aNext).myType == Open  || (*aNext).myType == Binary ) )
      (*anIt).myType = Pre;
    else if ( anIt != post.begin() && ( (*aPrev).myType == Close || (*aPrev).myType == Param ||
                                        (*aPrev).myType == Value || (*aPrev).myType == Pre ||
                                        (*aPrev).myType == Post || (*aPrev).myType == Binary ) &&
              ( aNext == post.end() || (*aNext).myType == Close ) )
      (*anIt).myType = Post;

    if ( anOpen.contains( (*anIt).myValue.toString() ) )
      (*anIt).myType = Pre;
    else if ( aClose.contains( (*anIt).myValue.toString() ) )
      (*anIt).myType = Post;
  }

  return error() == QtxEvalExpr::OK;
}

/*!
  \return how many global brackets there is (for example '((2+3))' has 2 global brackets)
  \param post - postfix to be checked
  \param f - start index to search
  \param l - last index to search
*/
int QtxEvalParser::globalBrackets( const QtxEvalParser::Postfix& post, int f, int l )
{
    int i,
        start_br = 0,
        fin_br = 0,
        br = 0,
        br_num = 0,
        min_br_num = (l-f+1)*5;
    
    for( i=f; i<=l; i++ )
        if( post[ i ].myType==QtxEvalParser::Open )
            start_br++;
        else
            break;
    for( i=l; i>=f; i-- )
        if( post[ i ].myType==QtxEvalParser::Close )
            fin_br++;
        else
            break;

    br = start_br<fin_br ? start_br : fin_br;
    for( i=f+br; i<=l-br; i++ )
    {
        if( post[i].myType==QtxEvalParser::Open )
            br_num++;
        else if( post[i].myType==QtxEvalParser::Close )
            br_num--;
        if( br_num<min_br_num )
            min_br_num = br_num;
    }

    return br+min_br_num;
}

/*!
  Third step of parsing: sorting of postfix in order to convert it to real postfix
  \param post - source postfix
  \param res - destination postfix
  \param anOpen - list of open brackets
  \param aClose - list of close brackets
  \param f - start index of postfix to sorting
  \param l - last index of postfix to sorting
*/
bool QtxEvalParser::sort( const Postfix& post, Postfix& res, const QStringList& anOpen,
                          const QStringList& aClose, int f, int l )
{
  if ( l < f )
    return true;

  if ( f < 0 )
    f = 0;

  if ( l < 0 )
    l = post.count() - 1;

  int br = globalBrackets( post, f, l );
  f += br;
  l -= br;

  if ( f == l && f >= 0 )
    res.append( post[f] );

  if ( l <= f )
    return true;

  if ( !checkOperations() )
    return false;

  int min = -1;
  QIntList argmin;
  QList<PostfixItemType> min_types;

  for ( int i = 0, j = f; j <= l; i++, j++ )
  {
    const PostfixItem& item = post[j];
    PostfixItemType tt = item.myType;
    if ( tt == Binary || tt == Pre || tt == Post )
    {
      int cur_pr = priority( item.myValue.toString(), tt == Binary );
      if ( cur_pr > 0 )
      {
        if ( min < 0 || min >= cur_pr )
        {
          if ( min == cur_pr )
          {
            argmin.append( f + i );
            min_types.append( tt );
          }
          else
          {
            min = cur_pr;
            argmin.clear();
            argmin.append( f + i );
            min_types.clear(); min_types.append( tt );
          }
        }
      }
      else
      {
        setError( QtxEvalExpr::InvalidOperation );
        break;
      }
    }
    else if ( tt == Open )
    {
      QString opBr = item.myValue.toString();
      int ind = anOpen.indexOf( opBr ), brValue = 0;
      while ( j <= l )
      {
        const PostfixItem& anItem = post[j];
        if ( anItem.myType == Open )
          brValue++;

        if ( anItem.myType == Close )
        {
          brValue--;
          QString clBr = anItem.myValue.toString();
          if ( aClose.indexOf( clBr ) == ind && brValue == 0 )
            break;
        }
        i++; j++;
      }

      if ( brValue > 0 )
      {
        setError( QtxEvalExpr::CloseExpected );
          break;
      }
    }
  }

  if ( error() == QtxEvalExpr::OK )
  {
    if ( min >= 0 )
    {
      Postfix one;
      QList<Postfix> parts;
      QIntList::const_iterator anIt = argmin.begin(), aLast = argmin.end();
      bool ok = sort( post, one, anOpen, aClose, f, *anIt - 1 );
      parts.append( one );
      one.clear();
      for ( ; anIt != aLast && ok; anIt++ )
      {
        QIntList::const_iterator aNext = anIt; aNext++;
        ok = sort( post, one, anOpen, aClose, *anIt + 1, aNext == aLast ? l : *aNext - 1 );
        parts.append( one );
        one.clear();
      }

      if ( !ok )
        return false;

      QStack<PostfixItem> aStack;
      QList<Postfix>::const_iterator aPIt = parts.begin();
      QList<PostfixItemType>::const_iterator aTIt = min_types.begin();
      res += (*aPIt);
      aPIt++;
      anIt = argmin.begin();
      for ( ; anIt != aLast; anIt++, aPIt++, aTIt++ )
      {
        if ( *aTIt == Pre )
        {
          if ( anOpen.contains( post[*anIt].myValue.toString() ) == 0 )
          {
            res += (*aPIt);
            aStack.push( post[ *anIt ] );
          }
          else
          {
            res.append( post[*anIt] );
            res += *aPIt;
          }
        }
        else
        {
          res += *aPIt;
          while ( !aStack.isEmpty() )
          {
            res.append( aStack.top() );
            aStack.pop();
          }
          res.append( post[*anIt] );
        }
      }
      while ( !aStack.isEmpty() )
      {
        res.append( aStack.top() );
        aStack.pop();
      }
    }
    else
    { //there are no operations
      for ( int k = f; k <= l; k++ )
      {
        if ( post.at( k ).myType==Value || post.at( k ).myType == Param )
          res.append( post.at( k ) );
      }
    }
  }

  return error() == QtxEvalExpr::OK;
}

/*!
  Build posfix by expression
  \param expr - string expression
*/
bool QtxEvalParser::parse( const QString& expr )
{
  myPostfix.clear();

  if ( !checkOperations() )
    return false;

  Postfix p;
  QStringList opens, closes;

  setError( QtxEvalExpr::OK );
  bracketsList( opens, true );
  bracketsList( closes, false );

  return prepare( expr, p ) && setOperationTypes( p ) && sort( p, myPostfix, opens, closes );
}

/*!
  Calculate operation
  \param op - operation name
  \param v1 - first argument (it is not valid for unary prefix operations and it is used to store result)
  \param v2 - second argument (it is not valid for unary postfix operations)
*/
bool QtxEvalParser::calculate( const QString& op, QVariant& v1, QVariant& v2 )
{
  QtxEvalExpr::Error err = isValid( op, v1.type(), v2.type() );
  if ( err == QtxEvalExpr::OK )
    setError( calculation( op, v1, v2 ) );
  else
    setError( err );

  return error() == QtxEvalExpr::OK;
}

/*!
  Calculates expression without postfix rebuilding
  \return QVariant as result (it is invalid if there were errors during calculation)
*/
QVariant QtxEvalParser::calculate()
{
  if ( !checkOperations() )
    return QVariant();

  setError( QtxEvalExpr::OK );

  QStringList anOpen, aClose;
  bracketsList( anOpen, true );
  bracketsList( aClose, false );

  QStack<QVariant> aStack;
  Postfix::iterator anIt = myPostfix.begin(), aLast = myPostfix.end();
  for ( ; anIt != aLast && error() == QtxEvalExpr::OK; anIt++ )
  {
    QString nn = (*anIt).myValue.toString();
    if ( (*anIt).myType == Param )
    {
      if ( hasParameter( nn ) )
      {
        QVariant& v = myParams[nn];
        if ( v.isValid() )
          aStack.push( v );
        else
          setError( QtxEvalExpr::InvalidToken );
      }
      else
        setError( QtxEvalExpr::InvalidToken );
    }
    else if ( (*anIt).myType == Value )
      aStack.push( (*anIt).myValue );
    else if ( (*anIt).myType == Pre || (*anIt).myType == Post )
    {
      if ( anOpen.contains( nn ) )
      {
        QVariant inv;
        if ( calculate( nn, inv, inv ) )
          aStack.push( QVariant() );
      }
      else if ( aClose.contains( nn ) )
      {
        QList<QVariant> set;
        while ( true )
        {
          if ( aStack.isEmpty() )
          {
            setError( QtxEvalExpr::StackUnderflow );
            break;
          }
          if ( aStack.top().isValid() )
          {
            set.append( aStack.top() );
            aStack.pop();
          }
          else
          {
            aStack.pop();
            break;
          }
        }

        QVariant qSet = set, inv;
        if ( calculate( nn, qSet, inv ) )
          aStack.push( set );
      }
      else if ( aStack.count() >= 1 )
      {
        QVariant inv;
        QVariant* v1 = &aStack.top(), *v2 = &inv; //"post-" case
        if ( (*anIt).myType == Pre )
        {
          v2 = &aStack.top();
          v1 = &inv;
        }
        calculate( nn, *v1, *v2 );
      }
      else
        setError( QtxEvalExpr::StackUnderflow );
    }
    else if ( (*anIt).myType == Binary )
    {
      if ( aStack.count() >= 2 )
      {
        QVariant v2 = aStack.top();
        aStack.pop();
        calculate( nn, aStack.top(), v2 );
      }
      else
        setError( QtxEvalExpr::StackUnderflow );
    }
  }

  QVariant res;
  if ( error() == QtxEvalExpr::OK )
  {
    int count = aStack.count();
    if ( count == 0 )
      setError( QtxEvalExpr::StackUnderflow );
    else if( count == 1 )
      res = aStack.top();
    else
      setError( QtxEvalExpr::ExcessData );
  }
  return res;
}

/*!
  Change expression, rebuild postfix and calculate it
  \return QVariant as result (it is invalid if there were errors during calculation)
*/
QVariant QtxEvalParser::calculate( const QString& expr )
{
  setExpression( expr );
  return calculate();
}

/*!
  Change expression and rebuild postfix
*/
bool QtxEvalParser::setExpression( const QString& expr )
{
  return parse( expr );
}

/*!
  \return true, if parser contain parameter
  \param name - name of parameter
*/
bool QtxEvalParser::hasParameter( const QString& name ) const
{
  return myParams.contains( name.trimmed() );
}

/*!
  Sets parameter value
  \param name - name of parameter
  \param value - value of parameter
*/
void QtxEvalParser::setParameter( const QString& name, const QVariant& value )
{
  myParams.insert( name.trimmed(), value );
}

/*!
  Removes parameter
  \param name - name of parameter
*/
bool QtxEvalParser::removeParameter( const QString& name )
{
  return myParams.remove( name.trimmed() );
}

/*!
  \return value of parameter (result is invalid if there is no such parameter)
  \param name - name of parameter
*/
QVariant QtxEvalParser::parameter( const QString& name ) const
{
  QVariant res;
  if ( myParams.contains( name.trimmed() ) )
    res = myParams[name.trimmed()].toString();
  return res;
}

/*!
  Searches first parameter with assigned invalid QVariant
  \return true if it is found
  \param name - variable to return name of parameter
*/
bool QtxEvalParser::firstInvalid( QString& name ) const
{
  for ( ParamMap::const_iterator anIt = myParams.begin(); anIt != myParams.end(); anIt++ )
  {
    if ( !anIt.value().isValid() )
    {
      name = anIt.key();
      return true;
    }
  }
  return false;
}

/*!
  Removes all parameters with assigned invalid QVariants
*/
void QtxEvalParser::removeInvalids()
{
  QStringList toDelete;
  for ( ParamMap::const_iterator anIt = myParams.begin(); anIt != myParams.end(); anIt++ )
  {
    if ( !anIt.value().isValid() )
      toDelete.append( anIt.key() );
  }

  for ( QStringList::const_iterator aLIt = toDelete.begin(); aLIt != toDelete.end(); aLIt++ )
    myParams.remove( *aLIt );
}

/*!
  \return last error occured during parsing
*/
QtxEvalExpr::Error QtxEvalParser::error() const
{
  return myError;
}

/*!
  Sets last error occured during parsing (for internal using only)
*/
void QtxEvalParser::setError( QtxEvalExpr::Error err )
{
  myError = err;
}

/*!
  \return string dump of internal parser postfix
*/
QString QtxEvalParser::dump() const
{
  return dump( myPostfix );
}

/*!
  \return string dump of postfix
  \param post - postfix to be dumped
*/
QString QtxEvalParser::dump( const Postfix& post ) const
{
  QString res;

  if ( !checkOperations() )
    return res;

  for ( Postfix::const_iterator anIt = post.begin(); anIt != post.end(); anIt++ )
  {
    if ( (*anIt).myType == Value && (*anIt).myValue.type() == QVariant::String )
      res += "'" + (*anIt).myValue.toString() + "'";
    else
      res += (*anIt).myValue.toString();
    
    if ( (*anIt).myType == Pre )
      res += "(pre)";
    else if ( (*anIt).myType == Post )
      res += "(post)";
    else if ( (*anIt).myType == Binary )
      res += "(bin)";

    res += "_";
  }

  return res;
}

/*!
  Fills and returns list with names of parameters
*/
QStringList QtxEvalParser::parameters() const
{
  QStringList lst;
  for ( Postfix::const_iterator anIt = myPostfix.begin(); anIt != myPostfix.end(); anIt++ )
  {
    if ( (*anIt).myType == Param )
    {
      QString name = (*anIt).myValue.toString();
      if ( !lst.contains( name ) )
        lst.append( name );
    }
  }
  return lst;
}

/*!
  Removes all parameters
*/
void QtxEvalParser::clearParameters()
{
  myParams.clear();
}

/*!
  \return string representation for list of QVariants
  \param list - list to be converted
*/
QString QtxEvalParser::toString( const QList<QVariant>& list )
{
  QString res = "set : [ ";
  for ( QList<QVariant>::const_iterator anIt = list.begin(); anIt != list.end(); anIt++ )
    res += (*anIt).toString() + " ";
  res += "]";
  return res;
}

void QtxEvalParser::operationList( QStringList& list ) const
{
  for ( SetList::const_iterator it = mySets.begin(); it != mySets.end(); ++it )
  {
    QStringList custom;
    QtxEvalSet* set = *it;
    set->operationList( custom );
    for ( QStringList::const_iterator sIt = custom.begin(); sIt != custom.end(); ++sIt )
    {
      if ( !list.contains( *sIt ) )
        list.append( *sIt );
    }
  }
}

void QtxEvalParser::bracketsList( QStringList& list, bool open ) const
{
  for ( SetList::const_iterator it = mySets.begin(); it != mySets.end(); ++it )
  {
    QStringList custom;
    QtxEvalSet* set = *it;
    set->bracketsList( custom, open );
    for ( QStringList::const_iterator sIt = custom.begin(); sIt != custom.end(); ++sIt )
    {
      if ( !list.contains( *sIt ) )
        list.append( *sIt );
    }
  }
}

bool QtxEvalParser::createValue( const QString& str, QVariant& val ) const
{
  bool ok = false;
  for ( SetList::const_iterator it = mySets.begin(); it != mySets.end() && !ok; ++it )
    ok = (*it)->createValue( str, val );
  return ok;
}

int QtxEvalParser::priority( const QString& op, bool isBin ) const
{
  int i = 0;
  int priority = 0;
  for ( SetList::const_iterator it = mySets.begin(); it != mySets.end() && priority <= 0; ++it, i++ )
    priority = (*it)->priority( op, isBin );

  return priority > 0 ? priority + i * 10 : 0;
}

QtxEvalExpr::Error QtxEvalParser::isValid( const QString& op,
                                           const QVariant::Type t1, const QVariant::Type t2 ) const
{
  QtxEvalExpr::Error err = QtxEvalExpr::OK;
  for ( SetList::const_iterator it = mySets.begin(); it != mySets.end(); ++it )
  {
    err = (*it)->isValid( op, t1, t2 );
    if ( err == QtxEvalExpr::OK )
      break;
  }
  return err;
}

QtxEvalExpr::Error QtxEvalParser::calculation( const QString& op, QVariant& v1, QVariant& v2 ) const
{
  QVariant nv1, nv2;
  for ( SetList::const_iterator it = mySets.begin(); it != mySets.end(); ++it )
  {
    nv1 = v1;
    nv2 = v2;
    if ( (*it)->isValid( op, v1.type(), v2.type() ) == QtxEvalExpr::OK )
    {
      QtxEvalExpr::Error err = (*it)->calculate( op, nv1, nv2 );
      if ( err == QtxEvalExpr::OK || err == QtxEvalExpr::InvalidResult )
      {
        v1 = nv1;
        v2 = nv2;
        return err;
      }
    }
  }
  return QtxEvalExpr::InvalidOperation;
}

bool QtxEvalParser::checkOperations() const
{
  if ( !mySets.isEmpty() )
    return true;

  QtxEvalParser* that = (QtxEvalParser*)this;
  that->setError( QtxEvalExpr::OperationsNull );
  return false;
}

/*!
  \class QtxEvalSet
  Set of operation for expression.
*/

/*!
  Constructor
*/
QtxEvalSet::QtxEvalSet()
{
}

/*!
  Destructor
*/
QtxEvalSet::~QtxEvalSet()
{
}

/*!
  Creates QVariant by it's string representation
*/
bool QtxEvalSet::createValue( const QString& str, QVariant& v ) const
{
  v = str;
  return false;
}

// ------------------------------- Standard operation sets ------------------------------

/*!
  \class QtxEvalSetBase
  Provides base functionality for standard sets of operations for parser
*/

/*!
   Default constructor
*/
QtxEvalSetBase::QtxEvalSetBase()
{
}

/*!
   Destructor
*/
QtxEvalSetBase::~QtxEvalSetBase()
{
}

/*!
   Fills list of brackets treated as open (when 'open' is 'true') or close ('open' is 'false')
*/
void QtxEvalSetBase::bracketsList( QStringList& list, bool open ) const
{
  list.append( open ? "(" : ")" );
}

/*!
   Fills list with operation names by copying of internal list of operations
*/
void QtxEvalSetBase::operationList( QStringList& list ) const
{
  list += myOpers;
}

/*!
   Add operation names from list to internal list of operations
*/
void QtxEvalSetBase::addOperations( const QStringList& list )
{
  for ( QStringList::const_iterator anIt = list.begin(); anIt != list.end(); ++anIt )
  {
    if ( !myOpers.contains( *anIt ) )
      myOpers.append( *anIt );
  }
}

/*!
   Append operation names from 'list' to internal list of operations
*/
void QtxEvalSetBase::addTypes( const ListOfTypes& list )
{
  for ( ListOfTypes::const_iterator anIt = list.begin(); anIt != list.end(); ++anIt )
  {
    if ( !myTypes.contains( *anIt ) )
      myTypes.append( *anIt );
  }
}

/*!
   \return whether values with passed types are valid for arguments of operation
   \param op - name of operation
   \param t1 - type of first argument
   \param t2 - type of second argument
*/
QtxEvalExpr::Error QtxEvalSetBase::isValid( const QString& op,
                                            const QVariant::Type t1, const QVariant::Type t2 ) const
{
  if ( ( t1 == QVariant::Invalid || myTypes.contains( t1 ) ) &&
       ( t2 == QVariant::Invalid || myTypes.contains( t2 ) ) &&
       ( t1 != QVariant::Invalid || t2 != QVariant::Invalid ) )
  {
    if ( priority( op, t1 != QVariant::Invalid && t2 != QVariant::Invalid ) > 0 )
      return QtxEvalExpr::OK;
    else
      return QtxEvalExpr::InvalidOperation;
  }
  else
    return QtxEvalExpr::OperandsNotMatch;
}

/*!
  \class QtxEvalSetArithmetic
  Provides set of arithmetic operations for parser
*/

/*!
   Default constructor
*/
QtxEvalSetArithmetic::QtxEvalSetArithmetic()
: QtxEvalSetBase()
{
  addOperations( QString( "+;-;*;/;=;<;>;<=;>=;<>;!=" ).split( ";" ) );

  ListOfTypes aTypes;
  aTypes.append( QVariant::Int );
  aTypes.append( QVariant::UInt );
  aTypes.append( QVariant::Double );
  addTypes( aTypes );
}

/*!
   Destructor
*/
QtxEvalSetArithmetic::~QtxEvalSetArithmetic()
{
}

QString QtxEvalSetArithmetic::Name()
{
  return "Arithmetic";
}

QString QtxEvalSetArithmetic::name() const
{
  return Name();
}

/*!
  Creates numbers by it's string representation [redefined virtual]
*/
bool QtxEvalSetArithmetic::createValue( const QString& str, QVariant& v ) const
{
  bool ok = false;
  v = str.toInt( &ok );

  if ( !ok )
  {
    v = str.toDouble( &ok );
    if ( !ok )
      ok = QtxEvalSetBase::createValue( str, v );
  }
  return ok;
}

/*!
  \return priority of arithmetic operation 'op'.
  \param isBin indicate whether the operation is binary
*/
int QtxEvalSetArithmetic::priority( const QString& op, bool isBin ) const
{
  if ( isBin )
  {
    if ( op == "<" || op == ">" || op == "=" ||
         op == "<=" || op == ">=" || op == "<>" || op == "!=" )
      return 1;
    else if ( op == "+" || op == "-" )
      return 2;
    else if( op == "*" || op == "/" )
      return 3;
    else
      return 0;
  }
  else if ( op == "+" || op == "-" )
    return 5;
  else
    return 0;
}
/*
void set( QVariant& v1, bool v2 )
{
  v1 = QVariant( v2 );
}
*/
/*!
  Calculates result of operation
  \return one of error states
  \param op - name of operation
  \param v1 - first operation argument (must be used also to store result)
  \param v2 - second operation argument
*/
QtxEvalExpr::Error QtxEvalSetArithmetic::calculate( const QString& op, QVariant& v1, QVariant& v2 ) const
{
  QtxEvalExpr::Error err = QtxEvalExpr::OK;

  if ( v1.isValid() && v2.isValid() )
  {
    // binary operations
    if ( ( v1.type() == QVariant::Int || v1.type() == QVariant::UInt ) &&
         ( v2.type() == QVariant::Int || v2.type() == QVariant::UInt ) )
    {
      int _v1 = v1.toInt();
      int _v2 = v2.toInt();

      if ( op == "+" )
        v1 = _v1 + _v2;
      else if ( op == "-" )
        v1 = _v1 - _v2;
      else if ( op == "*" )
        v1 = _v1 * _v2;
      else if ( op == "/" )
      {
        if ( _v2 != 0 )
        {
          if ( _v1 % _v2 == 0 )
            v1 = _v1 / _v2;
          else
            v1 = double( _v1 ) / double( _v2 );
        }
        else
          err = QtxEvalExpr::InvalidResult;
      }
      else if ( op == "<" )
        v1 = _v1 < _v2;
      else if ( op == ">" )
        v1 = _v1 > _v2;
      else if ( op == "=" )
        v1 = _v1 == _v2;
      else if ( op == "<=" )
        v1 = _v1 <= _v2;
      else if ( op == ">=" )
        v1 = _v1 >= _v2;
      else if ( op == "<>" || op == "!=" )
        v1 = _v1 != _v2;
    }
    else if ( ( v1.type() == QVariant::Int || v1.type() == QVariant::Double ) &&
              ( v2.type() == QVariant::Int || v2.type() == QVariant::Double ) )
    {
      double _v1 = v1.toDouble();
      double _v2 = v2.toDouble();

      if ( op == "+" )
        v1 = _v1 + _v2;
      else if ( op == "-" )
        v1 = _v1 - _v2;
      else if ( op == "*" )
          v1 = _v1 * _v2;
      else if ( op == "/" )
      {
        if ( _v2 != 0 )
          v1 = _v1 / _v2;
        else
          err = QtxEvalExpr::InvalidResult;
      }
      else if ( op == "<" )
        v1 = _v1 < _v2;
      else if ( op == ">" )
        v1 = _v1 > _v2;
      else if ( op == "=" )
        v1 = _v1 == _v2;
      else if ( op == "<=" )
        v1 = _v1 <= _v2;
      else if ( op == ">=" )
        v1 = _v1 >= _v2;
      else if ( op == "<>" || op == "!=" )
        v1 = _v1 != _v2;
    }
    else // prefix operations
    {
      if ( op == "-" )
      {
        if ( v2.type() == QVariant::Int )
          v2 = -v2.toInt();
        else if ( v2.type() == QVariant::Double )
          v2 = -v2.toDouble();
      }
    }
  }

  return err;
}

/*!
  \class QtxEvalSetLogic
  Provides set of logic operations for parser
*/

/*!
   Default constructor
*/
QtxEvalSetLogic::QtxEvalSetLogic()
: QtxEvalSetBase()
{
  addOperations( QString( "and;&&;or;||;xor;not;!;imp;=" ).split( ";" ) );

  ListOfTypes aTypes;
  aTypes.append( QVariant::Bool );
  aTypes.append( QVariant::Int );
  aTypes.append( QVariant::UInt );
  addTypes( aTypes );
}

/*!
   Destructor
*/
QtxEvalSetLogic::~QtxEvalSetLogic()
{
}

QString QtxEvalSetLogic::Name()
{
  return "Logic";
}

QString QtxEvalSetLogic::name() const
{
  return Name();
}

/*!
  Creates value 'true' or 'false' it's string representation [redefined virtual]
*/
bool QtxEvalSetLogic::createValue( const QString& str, QVariant& v ) const
{
  bool ok = true;
  QString valStr = str.toLower();
  if ( valStr == "true" || valStr == "yes" )
    v = QVariant( true );
  else if ( valStr == "false" || valStr == "no" )
    v = QVariant( false );
  else
    ok = QtxEvalSetBase::createValue( str, v );

  return ok;
}

/*!
  \return priority of arithmetic operation 'op'.
  \param isBin indicate whether the operation is binary
*/
int QtxEvalSetLogic::priority( const QString& op, bool isBin ) const
{
  if ( isBin )
  {
    if ( op == "and" || op == "or" || op == "xor" || op == "&&" || op == "||" || op == "imp" )
      return 1;
    else if ( op == "=" )
      return 2;
    else 
      return 0;
  }
  else if ( op == "not" || op == "!" )
    return 5;
  else
    return 0;
}

/*!
  Calculates result of operation
  \return one of error states
  \param op - name of operation
  \param v1 - first operation argument (must be used also to store result)
  \param v2 - second operation argument
*/
QtxEvalExpr::Error QtxEvalSetLogic::calculate( const QString& op, QVariant& v1, QVariant& v2 ) const
{
  QtxEvalExpr::Error err = QtxEvalExpr::OK;
  bool val1 = booleanValue( v1 );
  bool val2 = booleanValue( v2 );
  if ( v1.isValid() && v2.isValid() )
  {
    if ( op == "and" || op == "&&" )
      v1 = val1 && val2;
    else if ( op == "or" || op == "||" )
      v1 = val1 || val2;
    else if ( op == "xor" )
      v1 = ( !val1 && val2 ) || ( val1 && !val2 );
    else if ( op == "imp" )
      v1 = !val1 || val2;
    else if ( op == "=" )
      v1 = val1 == val2;
  }
  else if ( op == "not" || op == "!" )
    v2 = !val2;

  return err;
}

bool QtxEvalSetLogic::booleanValue( const QVariant& v ) const
{
  bool res = false;
  switch ( v.type() )
  {
  case QVariant::Bool:
    res = v.toBool();
    break;
  case QVariant::Int:
    res = v.toInt() != 0;
    break;
  case QVariant::UInt:
    res = v.toUInt() != 0;
    break;
  default:
    res = false;
    break;
  }
  return res;
}

/*!
  \class QtxEvalSetMath
  Provides set of more complex operations (mathematics functions) for parser (sqrt, sin, cos, etc)
*/

/*!
   Default constructor
*/
QtxEvalSetMath::QtxEvalSetMath()
: QtxEvalSetBase()
{
  addOperations( QString( "sqrt;abs;sin;cos;rad2grad;grad2rad" ).split( ";" ) );

  ListOfTypes aTypes;
  aTypes.append( QVariant::Int );
  aTypes.append( QVariant::Double );
  addTypes( aTypes );
}

/*!
   Destructor
*/
QtxEvalSetMath::~QtxEvalSetMath()
{
}

QString QtxEvalSetMath::Name()
{
  return "Math";
}

QString QtxEvalSetMath::name() const
{
  return Name();
}

/*!
  Creates numbers by it's string representation [redefined virtual]
*/
bool QtxEvalSetMath::createValue( const QString& str, QVariant& v ) const
{
  bool ok = false;
  v = str.toInt( &ok );

  if ( !ok )
  {
    v = str.toDouble( &ok );
    if ( !ok )
      ok = QtxEvalSetBase::createValue( str, v );
  }
  return ok;
}

/*!
  \return priority of arithmetic operation 'op'.
  \param isBin indicate whether the operation is binary
*/
int QtxEvalSetMath::priority( const QString& op, bool isBin ) const
{
  if ( isBin )
    return 0;
  else if ( op == "sqrt" || op == "abs" || op == "sin" ||
            op == "cos" || op == "rad2grad" || op == "grad2rad" )
    return 1;
  else
    return 0;
}

/*!
  Calculates result of operation
  \return one of error states
  \param op - name of operation
  \param v1 - first operation argument (must be used also to store result)
  \param v2 - second operation argument
*/
QtxEvalExpr::Error QtxEvalSetMath::calculate( const QString& op, QVariant& v1, QVariant& v2 ) const
{
  QtxEvalExpr::Error err = QtxEvalExpr::OK;
  double val = v2.toDouble();
  if ( op == "sqrt" )
  {
    if ( val >= 0 )
      v2 = sqrt( val );
    else
      err = QtxEvalExpr::InvalidResult;
  }
  else if ( op == "abs" )
  {
    if ( v2.type() == QVariant::Int )
      v2 = abs( v2.toInt() );
    else 
      v2 = fabs( v2.toDouble() );
  }
  else if ( op == "sin" )
    v2 = sin( val );
  else if ( op == "cos" )
    v2 = cos( val );
  else if ( op == "grad2rad" )
    v2 = val * 3.14159256 / 180.0;
  else if ( op == "rad2grad" )
    v2 = val * 180.0 / 3.14159256;

  return err;
}

/*!
  \class QtxEvalSetString
  Provides set of string operations for parser
*/

/*!
   Default constructor
*/
QtxEvalSetString::QtxEvalSetString()
: QtxEvalSetBase()
{
  addOperations( QString( "+;=;<;>;<=;>=;<>;!=;length;lower;upper" ).split( ";" ) );

  ListOfTypes aTypes;
  aTypes.append( QVariant::Int );
  aTypes.append( QVariant::Double );
  aTypes.append( QVariant::String );
  addTypes( aTypes );
}

/*!
   Destructor
*/
QtxEvalSetString::~QtxEvalSetString()
{
}

QString QtxEvalSetString::Name()
{
  return "String";
}

QString QtxEvalSetString::name() const
{
  return Name();
}

/*!
  Creates string QVariant by it's Qt string representation [redefined virtual]
*/
bool QtxEvalSetString::createValue( const QString& str, QVariant& v ) const
{
  bool ok = false;
  if ( str.length() > 1 && str[0] == '\'' && str[str.length() - 1] == '\'' )
  {
    v = str.mid( 1, str.length() - 2 );
    ok = true;
  }
  else
    ok = QtxEvalSetBase::createValue( str, v );
  return ok;
}

/*!
  \return priority of arithmetic operation 'op'.
  \param isBin indicate whether the operation is binary
*/
int QtxEvalSetString::priority( const QString& op, bool isBin ) const
{
  if ( isBin )
  {
    if ( op == "+" ) 
      return 2;
    else if ( op == "=" || op == "<" || op == ">" ||
              op == "<=" || op == ">=" || op == "<>" || op == "!=" )
      return 1;
    else
      return 0;
  }
  else if ( op == "length" || op == "lower" || op=="upper" )
    return 5;
  else
    return 0;
}

/*!
  Calculates result of operation
  \return one of error states
  \param op - name of operation
  \param v1 - first operation argument (must be used also to store result)
  \param v2 - second operation argument
*/
QtxEvalExpr::Error QtxEvalSetString::calculate( const QString& op, QVariant& v1, QVariant& v2 ) const
{
  QtxEvalExpr::Error err = QtxEvalExpr::OK;
  if ( v1.isValid() && v2.isValid() )
  {
    QString _v1 = v1.toString();
    QString _v2 = v2.toString();
    if ( op == "+" )
      v1 = _v1 + _v2;
    else if ( op == "=" )
      v1 = _v1 ==_v2;
    else if ( op == "<" )
      v1 = _v1 < _v2;
    else if ( op == ">" )
      v1 = _v1 > _v2;
    else if ( op == "<>" || op == "!=" )
      v1 = _v1 != _v2;
    else if ( op == "<=" )
      v1 = _v1 < _v2 || _v1 == _v2;
    else if ( op == ">=" )
      v1 = _v1 > _v2 || _v1 == _v2;
  }
  else if ( !v1.isValid() && v2.isValid() )
  {
    QString val = v2.toString();
    if ( op == "length" )
      v2 = (int)val.length();
    else if ( op == "lower" )
      v2 = val.toLower();
    else if ( op == "upper" )
      v2 = val.toUpper();
  }
  return err;
}

/*!
  \class QtxEvalSetSets
  Provides set of operations with sets for parser
*/

/*!
  Default constructor
*/
QtxEvalSetSets::QtxEvalSetSets()
{
  addOperations( QString( "{;};=;<>;!=;+;-;*;in;count" ).split( ";" ) );

  ListOfTypes aTypes;
  aTypes.append( QVariant::List );
  addTypes( aTypes );
}

/*!
  Destructor
*/
QtxEvalSetSets::~QtxEvalSetSets()
{
}

QString QtxEvalSetSets::Name()
{
  return "Sets";
}

QString QtxEvalSetSets::name() const
{
  return Name();
}

/*!
  Fills list of brackets treated as open (when 'open' is 'true') or close ('open' is 'false')
*/
void QtxEvalSetSets::bracketsList( QStringList& list, bool open ) const
{
  list.append( open ? "{" : "}" );
  QtxEvalSetBase::bracketsList( list, open );
}

/*!
  \return priority of arithmetic operation 'op'.
  \param isBin indicate whether the operation is binary
*/
int QtxEvalSetSets::priority( const QString& op, bool isBin ) const
{
  if ( isBin )
  {
    if ( op == "=" || op == "<>" || op == "!=" )
      return 1;
    else if ( op == "+" || op == "-" || op == "*" )
      return 2;
    else if ( op == "in" )
      return 3;
    else
      return 0;
  }
  else if ( op == "{" || op == "}" )
    return 5;
  else if ( op == "count" )
    return 4;
  else
    return 0;
}

/*!
  \return whether values with passed types are valid for arguments of operation
  \param op - name of operation
  \param t1 - type of first argument
  \param t2 - type of second argument
*/
QtxEvalExpr::Error QtxEvalSetSets::isValid( const QString& op,
                                            const QVariant::Type t1, const QVariant::Type t2 ) const
{
  if ( op == "{" )
    return QtxEvalExpr::OK;

  if ( op != "in" )
    return QtxEvalSetBase::isValid( op, t1, t2 );

  if ( t1 != QVariant::Invalid && t2 == QVariant::List )
    return QtxEvalExpr::OK;
  else
    return QtxEvalExpr::OperandsNotMatch;
}

/*!
  Adds new value 'v' to set 'set' [static]
*/
void QtxEvalSetSets::add( ValueSet& set, const QVariant& v )
{
  if ( v.isValid() && !set.contains( v ) )
    set.append( v );
}

/*!
   Adds values from set 's2' to set 's1'
*/
void QtxEvalSetSets::add( ValueSet& s1, const ValueSet& s2 )
{
  for ( ValueSet::const_iterator anIt = s2.begin(); anIt != s2.end(); ++anIt )
    add( s1, *anIt );
}

/*!
  Removes value 'v' from set 'set'
*/
void QtxEvalSetSets::remove( ValueSet& set, const QVariant& v )
{
  set.removeAll( v );
}

/*!
  Removes values of set 's2' from set 's1'
*/
void QtxEvalSetSets::remove( ValueSet& s1, const ValueSet& s2 )
{
  for ( ValueSet::const_iterator anIt = s2.begin(); anIt != s2.end(); ++anIt )
    s1.removeAll( *anIt );
}

/*!
  Calculates result of operation
  \return one of error states
  \param op - name of operation
  \param v1 - first operation argument (must be used also to store result)
  \param v2 - second operation argument
*/
QtxEvalExpr::Error QtxEvalSetSets::calculate( const QString& op, QVariant& v1, QVariant& v2 ) const
{
  QtxEvalExpr::Error err = QtxEvalExpr::OK;

  if ( op != "{" )
  {
    if ( op == "}" )
    {
      ValueSet aNewList;
      add( aNewList, v1.toList() );
      v1 = aNewList;
    }
    else if ( op == "=" || op == "<>" || op == "!=" || op == "+" || op == "-" || op == "*" )
    {
      ValueSet aNewList;
      add( aNewList, v1.toList() );
      if ( op == "=" || op == "<>" || op == "!=" || op == "-" )
      {
        remove( aNewList, v2.toList() );
        if ( op == "=" )
          v1 = aNewList.isEmpty() && v1.toList().count() == v2.toList().count();
        else if ( op == "<>" || op == "!=" )
          v1 = !aNewList.isEmpty() || v1.toList().count() != v2.toList().count();
        else
          v1 = aNewList;
      }
      else if ( op == "+" )
      {
        add( aNewList, v2.toList() );
        v1 = aNewList;
      }
      else if ( op == "*" )
      {
        ValueSet toDelete;
        add( toDelete, aNewList );
        remove( toDelete, v2.toList() );
        remove( aNewList, toDelete );
        v1 = aNewList;
      }
    }
    else if ( op== "count" )
      v2 = (int)v2.toList().count();
    else if ( op == "in" )
    {
      if ( v1.type() == QVariant::List )
      {
        bool res = true;
        ValueSet lst1 = v1.toList();
        ValueSet lst2 = v2.toList();
        for ( ValueSet::const_iterator anIt = lst1.begin(); anIt != lst1.end() && res; ++anIt )
          res = lst2.contains( *anIt );

        v1 = res;
      }
      else
	      v1 = QVariant( v2.toList().contains( v1 ) );
    }
  }
  return err;
}

/*!
  \class QtxEvalSetConst
  Provides different standard constants
*/
QtxEvalSetConst::QtxEvalSetConst()
{
}

QtxEvalSetConst::~QtxEvalSetConst()
{
}

QString QtxEvalSetConst::Name()
{
  return "Const";
}

QString QtxEvalSetConst::name() const
{
  return Name();
}

bool QtxEvalSetConst::createValue( const QString& str, QVariant& val ) const
{
  bool ok = true;
  if ( str == "pi" ) // PI number
    val = QVariant( 3.141593 );
  else if ( str == "exp" ) // Exponent (e)
    val = QVariant( 2.718282 );
  else if ( str == "g" ) // Free falling acceleration (g)
    val = QVariant( 9.80665 );
  else
    ok = false;

  return ok;
}

void QtxEvalSetConst::operationList( QStringList& ) const
{
}

void QtxEvalSetConst::bracketsList( QStringList&, bool open ) const
{
}

int QtxEvalSetConst::priority( const QString&, bool ) const
{
  return 0;
}

QtxEvalExpr::Error QtxEvalSetConst::isValid( const QString&, const QVariant::Type, const QVariant::Type ) const
{
  return QtxEvalExpr::InvalidOperation;
}

QtxEvalExpr::Error QtxEvalSetConst::calculate( const QString&, QVariant&, QVariant& ) const
{
  return QtxEvalExpr::InvalidOperation;
}
