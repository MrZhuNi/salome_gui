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

#include "QtxPopupMgr.h"
#include "QtxAction.h"

#include <QMenu>

/*!
  \class QtxPopupMgr::PopupCreator
*/
class QtxPopupMgr::PopupCreator : public QtxActionMgr::Creator
{
public:
  PopupCreator( QtxActionMgr::Reader*, QtxPopupMgr* );
  virtual ~PopupCreator();

  virtual int     append( const QString&, const bool,
                          const ItemAttributes&, const int );

  virtual QString visibleRule( const ItemAttributes& ) const;
  virtual QString toggleRule( const ItemAttributes& ) const;

private:
  QtxPopupMgr*    myMgr;
};

/*!
  Constructor
  \param r - menu reader
  \param mgr - menu manager
*/
QtxPopupMgr::PopupCreator::PopupCreator( QtxActionMgr::Reader* r,
                                         QtxPopupMgr* mgr )
: QtxActionMgr::Creator( r ),
  myMgr( mgr )
{
}

/*!
  Destructor
*/QtxPopupMgr::PopupCreator::~PopupCreator()
{
}


/*!
  Appends new menu items
  \param tag - tag of item
  \param subMenu - it has submenu
  \param attr - list of attributes
  \param pId - id of action corresponding to parent item
*/
int QtxPopupMgr::PopupCreator::append( const QString& tag, const bool subMenu,
                                       const ItemAttributes& attr, const int pId )
{
  if ( !myMgr || !reader() )
    return -1;

  QString label   = reader()->option( "label",     "label"     ),
          id      = reader()->option( "id",        "id"        ),
          pos     = reader()->option( "pos",       "pos"       ),
          group   = reader()->option( "group",     "group"     ),
          tooltip = reader()->option( "tooltip",   "tooltip"   ),
          sep     = reader()->option( "separator", "separator" ),
          accel   = reader()->option( "accel",     "accel"     ),
          icon    = reader()->option( "icon",      "icon"      ),
          toggle  = reader()->option( "toggle",    "toggle"    );

  QtxActionMenuMgr* mgr = myMgr;

  int res = -1, actId = intValue( attr, id, -1 );;
  if ( subMenu )
    res = mgr->insert( strValue( attr, label ), pId, intValue( attr, group, 0 ), intValue( attr, pos, -1 ) );
  else if ( tag == sep )
    res = mgr->insert( separator(), pId, intValue( attr, group, 0 ), intValue( attr, pos, -1 ) );
  else
  {
    QIcon set;
    QPixmap pix;
    QString name = strValue( attr, icon );
    if( !name.isEmpty() )
    {
      if ( loadPixmap( name, pix ) )
        set = QIcon( pix );
    }

    QString actLabel = strValue( attr, label );
    QtxAction* newAct = new QtxAction( strValue( attr, tooltip ), set, actLabel,
                                       QKeySequence( strValue( attr, accel ) ),
                                       myMgr );
    newAct->setToolTip( strValue( attr, tooltip ) );
    QString toggleact = strValue( attr, toggle );
    bool isToggle = !toggleact.isEmpty();
    newAct->setCheckable( isToggle );
    newAct->setChecked( toggleact.toLower() == "true" );
        
    connect( newAct );
    int aid = mgr->registerAction( newAct, actId );
    myMgr->setRule( newAct, visibleRule( attr ), QtxPopupMgr::VisibleRule );
    myMgr->setRule( newAct, isToggle ? toggleRule( attr ) : QString(), QtxPopupMgr::ToggleRule );
    res = mgr->insert( aid, pId, intValue( attr, group, 0 ), intValue( attr, pos, -1 ) );
  }

  return res;
}

/*!
  \return visibility rule by attributes
  Default implementation is empty
*/
QString QtxPopupMgr::PopupCreator::visibleRule( const ItemAttributes& ) const
{
  return QString::null;
}

/*!
  \return toggle rule by attributes
  Default implementation is empty
*/
QString QtxPopupMgr::PopupCreator::toggleRule( const ItemAttributes& ) const
{
  return QString::null;
}

/*!
  \class QPopupMgr
*/

/*!
  Constructor
*/
QtxPopupMgr::QtxPopupMgr( QObject* parent )
: QtxActionMenuMgr( 0, parent ),
  mySelection( 0 )
{
}

/*!
  Constructor
*/
QtxPopupMgr::QtxPopupMgr( QMenu* popup, QObject* parent )
: QtxActionMenuMgr( popup, parent ),
  mySelection( 0 )
{
}

/*!
  Destructor
*/
QtxPopupMgr::~QtxPopupMgr()
{
}

QMenu* QtxPopupMgr::menu() const
{
  return ::qobject_cast<QMenu*>( menuWidget() );
}

void QtxPopupMgr::setMenu( QMenu* menu )
{
  setMenuWidget( menu );
}

QtxPopupSelection* QtxPopupMgr::selection() const
{
  return mySelection;
}

void QtxPopupMgr::setSelection( QtxPopupSelection* sel )
{
  if ( mySelection == sel )
    return;

  mySelection = sel;

  QtxActionMgr::triggerUpdate();
}

/*!
  Additional version of registerAction
  \param act - action to be registered
  \param visible - rule for visibility state
  \param toggle - rule for toggle on state
  \param id - proposed id (if it is less than 0, then id will be generated automatically)
*/
int QtxPopupMgr::registerAction( QAction* act, const int id, const QString& rule, const QtxPopupMgr::RuleType type )
{
  int _id = QtxActionMenuMgr::registerAction( act, id );
  setRule( act, rule, type );
  return _id;
}

/*!
  Removes action from internal map
  \param id - action id
*/
void QtxPopupMgr::unRegisterAction( const int id )
{
  QAction* a = action( id );
  if ( myRules.contains( a ) )
  {
    for ( ExprMap::iterator it = myRules[a].begin(); it != myRules[a].end(); ++it )
      delete it.value();
  }
  myRules.remove( a );

  remove( id );

  QtxActionMenuMgr::unRegisterAction( id );
}

/*!
  \return true if manager has rule for action
  \param act - action
  \param visibility - if it is true, then rule for "visibility" is checked, otherwise - for "toggle"
*/
int QtxPopupMgr::insert( const int id, const int pId, const QString& rule, const RuleType ruleType )
{
  int res = QtxActionMenuMgr::insert( id, pId, -1 );
  setRule( action( id ), rule, ruleType );
  return res;
}

int QtxPopupMgr::insert( QAction* a, const int pId, const QString& rule, const RuleType ruleType )
{
  int res = QtxActionMenuMgr::insert( a, pId, -1 );
  setRule( a, rule, ruleType );
  return res;
}

/*!
  \return Rule of a specified action
  \param a - action
  \param type - type of rule
*/
QString QtxPopupMgr::rule( QAction* a, const RuleType type ) const
{
  QString rule;
  QtxEvalExpr* expr = expression( a, type );
  if ( expr )
    rule = expr->expression();
  return rule;
}

/*!
  \return Rule of action with a specified id
  \param id - action id
  \param type - type of rule
*/
QString QtxPopupMgr::rule( const int id, const RuleType type ) const
{
  return rule( action( id ), type );
}

/*!
  Sets new rule for action
  \param act - action
  \param rule - string expression of rule
  \param type - type of rule
*/
void QtxPopupMgr::setRule( QAction* act, const QString& rule, const RuleType type )
{
  if ( !act )
    return;

  QtxEvalExpr* expr = expression( act, type, true );

  expr->setExpression( rule );
}

/*!
  Sets new rule for action
  \param id - action id
  \param rule - string expression of rule
  \param type - type of rule
*/
void QtxPopupMgr::setRule( const int id, const QString& rule, const RuleType type )
{
  setRule( action( id ), rule, type );
}

/*!
  \return true if parser has finished work without errors
  \param p - parser
*/

bool QtxPopupMgr::result( QtxEvalParser* p ) const
{
  bool res = false;
  if ( p )
  {
    QVariant vv = p->calculate();
    res = p->error() == QtxEvalExpr::OK &&
          ( ( vv.type() == QVariant::Int && vv.toInt() != 0 ) ||
            ( vv.type() == QVariant::Bool && vv.toBool() ) );
  }
  return res;
}

/*!
  Fills parser parameters with help of Selection::globalParam() method
  \param p - parser
  \param specific - list will be filled with names of parameters depending on selection objects (not global)
*/
void QtxPopupMgr::setParameters( QtxEvalParser* p, QStringList& specific ) const
{
  if ( !p || !mySelection )
    return;

  QStringList params = p->parameters();
  for ( QStringList::const_iterator it = params.begin(); it != params.end(); ++it )
  {
    QVariant v = parameter( *it );
    if ( v.isValid() )
	    p->setParameter( *it, v );
    else
      specific.append( *it );
  }
}

/*!
  \return true if 'v1'<'v2'
  This function can work with many types of values
*/
bool operator<( const QVariant& v1, const QVariant& v2 )
{
  QVariant::Type t1 = v1.type(), t2 = v2.type();
  if ( t1 == t2 )
  {
    switch( t1 )
    {
    case QVariant::Int:
      return v1.toInt() < v2.toInt();
      break;      
    case QVariant::Double:
      return v1.toDouble() < v2.toDouble();
      break;      
    case QVariant::String:
      return v1.toString() < v2.toString();
      break;      
    case QVariant::StringList:
    case QVariant::List:
    {
      const QList<QVariant>& aList1 = v1.toList(), aList2 = v2.toList();
      QList<QVariant>::const_iterator anIt1 = aList1.begin(), aLast1 = aList1.end(),
                                           anIt2 = aList2.begin(), aLast2 = aList2.end();
      for ( ; anIt1 != aLast1 && anIt2 != aLast2;  anIt1++, anIt2++ )
      {
	      if ( (*anIt1) != (*anIt2) )
	        return (*anIt1)<(*anIt2);
      }
      return anIt1 == aLast1 && anIt2 != aLast2;
      break;      
    }
    default:
      return v1.toString() < v2.toString();
      break;      
    }
  }
  else
    return t1 < t2;
}

/*!
  \return true if rule of action is satisfied on current selection
  \param act - action
  \param visibility - what rule is checked: for visibility(true) or for toggle(false)
*/
bool QtxPopupMgr::isSatisfied( QAction* act, const RuleType type ) const
{
  if ( !act )
    return false;

  QtxEvalExpr* exp = expression( act, type );
  if ( !exp )
    return true;

  bool res = false;

  QtxEvalParser* p = exp->parser();

  QStringList specific;
  p->clearParameters();
  setParameters( p, specific );

  QMap<QList<QVariant>, int> aCorteges;
  if ( !specific.isEmpty() )
  {
    if ( mySelection )
    {
	    res = false;
      for ( int i = 0; i < mySelection->count() && !res; i++ )
      {
        QList<QVariant> c;
	      for ( QStringList::const_iterator anIt1 = specific.begin(); anIt1 != specific.end(); ++anIt1 )
          c.append( parameter( *anIt1, i ) );
        aCorteges.insert( c, 0 );
	    }
	    for ( QMap<QList<QVariant>, int>::const_iterator anIt = aCorteges.begin(); anIt  != aCorteges.end(); ++anIt )
	    {
	      const QList<QVariant>& aCortege = anIt.key();
	      QStringList::const_iterator anIt1 = specific.begin(), aLast1 = specific.end();
	      QList<QVariant>::const_iterator anIt2 = aCortege.begin();
	      for ( ; anIt1 != aLast1; anIt1++, anIt2++ )
	        p->setParameter( *anIt1, *anIt2 );
	      res = res || result( p );
	    }
    }
    else
	    res = false;
  }
  else
    res = result( p );

  return res;
}

/*!
  \return true if item corresponding to action is visible
  \param actId - action id
  \param place - index of place
*/
bool QtxPopupMgr::isVisible( const int actId, const int place ) const
{
  return QtxActionMenuMgr::isVisible( actId, place ) && isSatisfied( action( actId ) );
}

/*!
  Updates popup according to selection
  \param p - popup menu
  \param sel - selection
*/
void QtxPopupMgr::internalUpdate()
{
  myCache.clear();

  for ( RuleMap::iterator it = myRules.begin(); it != myRules.end(); ++it )
  {
    ExprMap& map = it.value();
    if ( it.key()->isCheckable() && map.contains( ToggleRule ) &&
         !map[ToggleRule]->expression().isEmpty() )
      it.key()->setChecked( isSatisfied( it.key(), ToggleRule ) );
  }

  QtxActionMenuMgr::internalUpdate();
}

/*!
  Updates popup according to selection
  \param p - popup menu
  \param sel - selection
*/
void QtxPopupMgr::updateMenu()
{
  internalUpdate();
}

/*!
  \return reference to eval expression
  \param a - action
  \param type - rule type
  \param create - create eval expression if it doesn't exist
*/
QtxEvalExpr* QtxPopupMgr::expression( QAction* a, const RuleType type, const bool create ) const
{
  QtxEvalExpr* res = 0;

  QtxPopupMgr* that = (QtxPopupMgr*)this;
  RuleMap& ruleMap = that->myRules;
  if ( !ruleMap.contains( a ) && create )
    ruleMap.insert( a, ExprMap() );

  if ( ruleMap.contains( a ) )
  {
    ExprMap& exprMap = ruleMap[a];
    if ( exprMap.contains( type ) )
      res = exprMap[type];
    else if ( create )
      exprMap.insert( type, res = new QtxEvalExpr() );
  }

  return res;
}

/*!
  Loads actions description from file
  \param fname - name of file
  \param r - reader of file
  \return true on success
*/
bool QtxPopupMgr::load( const QString& fname, QtxActionMgr::Reader& r )
{
  PopupCreator cr( &r, this );
  return r.read( fname, cr );
}

QVariant QtxPopupMgr::parameter( const QString& name, const int idx ) const
{
  QVariant val;
  QString cacheName = name + ( idx >= 0 ? QString( "_%1" ).arg( idx ) : QString() );
  if ( myCache.contains( cacheName ) )
    val = myCache[cacheName];
  else
  {
    if ( selection() )
      val = idx < 0 ? selection()->parameter( name ) : 
                      selection()->parameter( idx, name );
    if ( val.isValid() )
    {
      QtxPopupMgr* that = (QtxPopupMgr*)this;
      that->myCache.insert( cacheName, val );
    }
  }
  return val;
}




























































/*!
  \class QtxPopupSelection
*/

QString QtxPopupSelection::option( const QString& optName ) const
{
  QString opt;
  if ( myOptions.contains( optName ) )
    opt = myOptions[optName];
  return opt;
}

void QtxPopupSelection::setOption( const QString& optName, const QString& opt )
{
  myOptions.insert( optName, opt );
}

/*!
  \return value of global parameter (depending on whole selection, but not
  dependending on one object of selection)
  \param str - name of parameter

  By default, it returns count of selected objects ("selcount") and list of parameters ("$<name>")
*/
QVariant QtxPopupSelection::parameter( const QString& str ) const
{
  if ( str == selCountParam() )
    return count();
  else if ( str.startsWith( equalityParam() ) )
  {
    QtxEvalSetSets::ValueSet set;
    QString par = str.mid( equalityParam().length() );
    for ( int i = 0; i < (int)count(); i++ )
    {
      QVariant v = parameter( i, par );
      if ( v.isValid() )
	      QtxEvalSetSets::add( set, v );
      else
	      return QVariant();
    }
    return set;
  }
  else
    return QVariant();
}

/*!
  \return symbole to detect name of parameter list
*/
QString QtxPopupSelection::equalityParam() const
{
  QString str = option( "equality" );
  if ( str.isEmpty() )
    str = "$";
  return str;
}

/*!
  \return name of parameter for count of selected objects
*/
QString QtxPopupSelection::selCountParam() const
{
  QString str = option( "equality" );
  if ( str.isEmpty() )
    str = "selcount";
  return str;
}
