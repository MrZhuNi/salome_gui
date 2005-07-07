// File:      SalomeApp_Dialog.cxx
// Author:    Alexander SOLOVYOV

#include <SalomeApp_Dialog.h>
#include <SUIT_Session.h>

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>

SalomeApp_Dialog::SalomeApp_Dialog( QWidget* parent, const char* name, bool modal,
                                    bool allowResize, const int f, WFlags wf )
: QtxDialog( parent, name, modal, allowResize, f, wf ),
  myIsExclusive( true )
{
  setObjectPixmap( "SalomeApp", tr( "ICON_SELECT" ) );
}

SalomeApp_Dialog::~SalomeApp_Dialog()
{
}

void SalomeApp_Dialog::show()
{
  QtxDialog::show();
}

bool SalomeApp_Dialog::isExclusive() const
{
  return myIsExclusive;
}

void SalomeApp_Dialog::updateButtons( const int _id )
{
  if( !myIsExclusive )
    return;

  int id = _id;

  ObjectMap::const_iterator anIt = myObjects.begin(),
                            aLast = myObjects.end();
  for( ; anIt!=aLast; anIt++ )
  {
    QPushButton* but = anIt.data().myBtn;
    if( but && but->isOn() )
    {
      if( id==-1 )
        id = anIt.key();

      if( anIt.key()!=id )
        but->setOn( false );
    }
  }
}

void SalomeApp_Dialog::setExclusive( const bool ex )
{
  myIsExclusive = ex;
  updateButtons();
}

void SalomeApp_Dialog::showObject( const int id )
{
  setObjectShown( id, true );
}

void SalomeApp_Dialog::hideObject( const int id )
{
  setObjectShown( id, false );
}

void SalomeApp_Dialog::setObjectShown( const int id, const bool shown )
{
  if( myObjects.contains( id ) && isObjectShown( id )!=shown )
  {
    Object& obj = myObjects[ id ];
    obj.myEdit->setShown( shown );
    obj.myBtn->setShown( shown );
    obj.myLabel->setShown( shown );
    if( !shown )
      obj.myBtn->setOn( false );
  }
}

bool SalomeApp_Dialog::isObjectShown( const int id ) const
{
  return myObjects.contains( id ) && myObjects[ id ].myEdit->isShown();
}

void SalomeApp_Dialog::selectObject( const QString& name, const int type, const QString& id )
{
  QStringList names;   names.append( name );
  TypesList types;     types.append( type );
  QStringList ids;     ids.append( id );
  selectObject( names, types, ids );
}

void SalomeApp_Dialog::selectObject( const QStringList& _names,
                                     const TypesList& _types,
                                     const QStringList& _ids )
{
  ObjectMap::iterator anIt = myObjects.begin(),
                      aLast = myObjects.end();
  for( ; anIt!=aLast; anIt++ )
    if( anIt.data().myBtn->isOn() )
    {
      QStringList names = _names, ids = _ids;
      TypesList types = _types;
      
      filterTypes( anIt.key(), names, types, ids );

      anIt.data().myEdit->setText( selectionDescription( names, types, anIt.data().myNI ) );
      anIt.data().myTypes = types;
      anIt.data().myIds = ids;
      anIt.data().myNames = names;
      
      emit selectionChanged( anIt.key() ); 
    }
}

bool SalomeApp_Dialog::hasSelection( const int id ) const
{
  return myObjects.contains( id ) && !myObjects[ id ].myIds.isEmpty();
}

void SalomeApp_Dialog::clearSelection( const int id )
{
  if( id==-1 )
  {
    ObjectMap::const_iterator anIt = myObjects.begin(),
                              aLast = myObjects.end();
    for( ; anIt!=aLast; anIt++ )
      clearSelection( anIt.key() );
  }
  
  else if( myObjects.contains( id ) )
  {
    myObjects[ id ].myIds.clear();
    myObjects[ id ].myTypes.clear();
    myObjects[ id ].myNames.clear();
    
    myObjects[ id ].myEdit->setText( QString::null );
    emit selectionChanged( id );
  }
}

void SalomeApp_Dialog::selectedObject( const int id, QStringList& list ) const
{
  if( myObjects.contains( id ) )
    list = myObjects[ id ].myIds;
  //else
  //  list.clear();
}

void SalomeApp_Dialog::objectSelection( SelectedObjects& objs ) const
{
  //objs.clear();
  ObjectMap::const_iterator anIt = myObjects.begin(),
                            aLast = myObjects.end();
  for( ; anIt!=aLast; anIt++ )
  {
    QStringList ids;
    selectedObject( anIt.key(), ids );
    if( !ids.isEmpty() )
      objs.insert( anIt.key(), ids );
  }
}

int SalomeApp_Dialog::createObject( const QString& label, QWidget* parent, const int id )
{
  static int _id = -1;

  int nid = id>=0 ? id : --_id;
  if( !myObjects.contains( nid ) )
  {
    QLabel* lab = new QLabel( label, parent );
    myObjects[ nid ].myLabel = lab;
    
    QPushButton* but = new QPushButton( parent );
    but->setIconSet( QIconSet( myPixmap ) );
    but->setToggleButton( true );
    connect( but, SIGNAL( toggled( bool ) ), this, SLOT( onToggled( bool ) ) );
    myObjects[ nid ].myBtn = but;

    QLineEdit* ne = new QLineEdit( parent );
    ne->setReadOnly( true );
    myObjects[ nid ].myEdit = ne;

    myObjects[ nid ].myNI = OneNameOrCount;
  }
  return nid;
}

void SalomeApp_Dialog::renameObject( const int id, const QString& label )
{
  if( myObjects.contains( id ) )
    myObjects[ id ].myLabel->setText( label );
}

void SalomeApp_Dialog::setObjectType( const int id, const int type1, ... )
{
  TypesList types;
  
  const int* tt = &type1;
  while( *tt>=0 )
  {
    types.append( *tt );
    tt++;
  }

  setObjectType( id, types );
}

void SalomeApp_Dialog::setObjectType( const int id, const TypesList& list )
{
  if( !myObjects.contains( id ) )
    return;

  TypesList& internal = myObjects[ id ].myPossibleTypes;
    
  QMap<int,int> types;
  TypesList::const_iterator anIt = list.begin(),
                            aLast = list.end();
  for( ; anIt!=aLast; anIt++ )
    types.insert( *anIt, 0 );


  internal.clear();
  QMap<int,int>::const_iterator aMIt = types.begin(),
                                aMLast = types.end();
  for( ; aMIt!=aMLast; aMIt++ )
    internal.append( aMIt.key() );

  updateObject( id );
}

void SalomeApp_Dialog::addObjectType( const int id, const int type1, const int, ... )
{
  TypesList types; objectTypes( id, types );

  const int* tt = &type1;
  while( *tt>=0 )
  {
    types.append( *tt );
    tt++;
  }

  setObjectType( id, types );  
}

void SalomeApp_Dialog::addObjectType( const int id, const TypesList& list )
{
  TypesList types = list; objectTypes( id, types );
  setObjectType( id, types );
}

void SalomeApp_Dialog::addObjectType( const int id, const int type )
{
  TypesList types; objectTypes( id, types );
  types.append( type );
  setObjectType( id, types );
}

void SalomeApp_Dialog::removeObjectType( const int id )
{
  TypesList types;
  setObjectType( id, types );
}

void SalomeApp_Dialog::removeObjectType( const int id, const TypesList& list )
{
  if( !myObjects.contains( id ) )
    return;

  TypesList& internal = myObjects[ id ].myPossibleTypes;

  QMap<int,int> types;
  TypesList::const_iterator anIt = internal.begin(),
                            aLast = internal.end();
  for( ; anIt!=aLast; anIt++ )
    types.insert( *anIt, 0 );
  anIt = list.begin(); aLast = list.end();
  for( ; anIt!=aLast; anIt++ )
    types.remove( *anIt );


  internal.clear();
  QMap<int,int>::const_iterator aMIt = types.begin(),
                                aMLast = types.end();
  for( ; aMIt!=aMLast; aMIt++ )
    internal.append( aMIt.key() );

  updateObject( id );
}

void SalomeApp_Dialog::removeObjectType( const int id, const int type )
{
  TypesList list; list.append( type );
  removeObjectType( id, list );
}

bool SalomeApp_Dialog::hasObjectType( const int id, const int type ) const
{
  if( myObjects.contains( id ) )
    return myObjects[ id ].myPossibleTypes.contains( type );
  else
    return false;
}

void SalomeApp_Dialog::objectTypes( const int id, TypesList& list ) const
{
  if( myObjects.contains( id ) )
  {
    TypesList::const_iterator anIt = myObjects[ id ].myPossibleTypes.begin(),
                              aLast = myObjects[ id ].myPossibleTypes.end();
    for( ; anIt!=aLast; anIt++ )
      list.append( *anIt );
  }  
}

void SalomeApp_Dialog::onToggled( bool on )
{
  QPushButton* but = ( QPushButton* )sender();
  int id = -1;

  if( !but )
    return;
    
  ObjectMap::const_iterator anIt = myObjects.begin(),
                            aLast = myObjects.end();
  for( ; anIt!=aLast && id==-1; anIt++ )
    if( anIt.data().myBtn==but )
      id = anIt.key();

  if( id!=-1 )
    if( on )
    {
      emit objectActivated( id );
      updateButtons( id );
    }
    else
      emit objectDeactivated( id );
}

void SalomeApp_Dialog::updateObject( const int id, bool emit_signal )
{
  if( hasSelection( id ) )
  {
    Object& obj = myObjects[ id ];
    filterTypes( id, obj.myNames, obj.myTypes, obj.myIds );
    obj.myEdit->setText( selectionDescription( obj.myNames, obj.myTypes, obj.myNI ) );
    if( emit_signal )
      emit selectionChanged( id );
  }
}

void SalomeApp_Dialog::filterTypes( const int id, QStringList& names, TypesList& types, QStringList& ids ) const
{
  if( !myObjects.contains( id ) )
    return;

  const Object& obj = myObjects[ id ];
  if( obj.myPossibleTypes.isEmpty() )
    return;

  QStringList new_names, new_ids;
  TypesList new_types;
  
  TypesList::const_iterator anIt1 = types.begin(),
                            aLast = types.end();
  QStringList::const_iterator anIt2 = names.begin(),
                              anIt3 = ids.begin();
  for( ; anIt1!=aLast; anIt1++, anIt2++, anIt3++ )
    if( obj.myPossibleTypes.contains( *anIt1 ) )
    {
      if( new_types.count()==1 && !multipleSelection( id ) )
        break;
        
      new_names.append( *anIt2 );
      new_types.append( *anIt1 );
      new_ids.append( *anIt3 );       
    }
  names = new_names;
  types = new_types;
  ids = new_ids;
}

SUIT_ResourceMgr* SalomeApp_Dialog::resMgr() const
{
  return SUIT_Session::session()->resourceMgr();
}

void SalomeApp_Dialog::setObjectPixmap( const QPixmap& p )
{
  myPixmap = p;
  ObjectMap::const_iterator anIt = myObjects.begin(),
                            aLast = myObjects.end();
  for( ; anIt!=aLast; anIt++ )
    anIt.data().myBtn->setIconSet( p );
}                        

void SalomeApp_Dialog::setObjectPixmap( const QString& section, const QString& file )
{
  SUIT_ResourceMgr* mgr = resMgr();
  if( mgr )
    setObjectPixmap( mgr->loadPixmap( section, file ) );
}

bool SalomeApp_Dialog::multipleSelection( const int id ) const
{
  return nameIndication( id )!=OneName;  
}

SalomeApp_Dialog::NameIndication SalomeApp_Dialog::nameIndication( const int id ) const
{
  if( myObjects.contains( id ) )
    return myObjects[ id ].myNI;
  else
    return OneNameOrCount;
}

void SalomeApp_Dialog::setNameIndication( const int id, const NameIndication ni )
{
  if( id==-1 )
  {
    ObjectMap::iterator anIt = myObjects.begin(),
                        aNext,
                        aLast = myObjects.end();
    for( ; anIt!=aLast; anIt++ )
    {
      anIt.data().myNI = ni;
      aNext = anIt; aNext++;
      updateObject( anIt.key(), aNext==aLast );
    }
  }
  else if( myObjects.contains( id ) )
  {
    myObjects[ id ].myNI = ni;
    updateObject( id, true );
  }
}

QString SalomeApp_Dialog::selectionDescription( const QStringList& names, const TypesList& types, const NameIndication ni ) const
{
  if( names.count()!=types.count() )
    return "SalomeApp_Dialog::selectionDescription: Error!!!";
    
  switch( ni )
  {
    case OneName:
      return names.first();
      break;
      
    case OneNameOrCount:
      if( names.count()==1 )
        return names.first();
      else
        return countOfTypes( types );
      break;
      
    case ListOfNames:
      return names.join( ", " );
      break;
      
    case Count:
      return countOfTypes( types );
      break;
  };
  return QString::null;
}

QString SalomeApp_Dialog::countOfTypes( const TypesList& types ) const
{
  QMap<int, int> typesCount;
  QStringList typeCount;
  
  TypesList::const_iterator anIt = types.begin(),
                            aLast = types.end();
  for( ; anIt!=aLast; anIt++ )
    if( typesCount.contains( *anIt ) )
      typesCount[ *anIt ]++;
    else
      typesCount[ *anIt ] = 1;

  QMap<int,int>::const_iterator aMIt = typesCount.begin(),
                                aMLast = typesCount.end();
  for( ; aMIt!=aMLast; aMIt++ )
    typeCount.append( QString( "%1 %2" ).arg( aMIt.data() ).arg( typeName( aMIt.key() ) ) );

  return typeCount.join( ", " );
}

QString& SalomeApp_Dialog::typeName( const int type )
{
  return myTypeNames[ type ];
}

const QString& SalomeApp_Dialog::typeName( const int type ) const
{
  return myTypeNames[ type ];
}
