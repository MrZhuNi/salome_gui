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
    QPushButton* but = anIt.data().btn;
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
    obj.edit->setShown( shown );
    obj.btn->setShown( shown );
    obj.label->setShown( shown );
    if( !shown )
      obj.btn->setOn( false );
  }
}

bool SalomeApp_Dialog::isObjectShown( const int id ) const
{
  return myObjects.contains( id ) && myObjects[ id ].edit->isShown();
}

void SalomeApp_Dialog::selectObject( const QString& name, const int type, const QString& entry )
{
  ObjectMap::iterator anIt = myObjects.begin(),
                      aLast = myObjects.end();
  for( ; anIt!=aLast; anIt++ )
    if( anIt.data().btn->isOn() )
    {
      if( isCorrectType( anIt.key(), type ) )
      {
        anIt.data().edit->setText( name );
        anIt.data().type = type;
        anIt.data().entry = entry;
      }
      else
      {
        anIt.data().edit->setText( QString::null );
        anIt.data().type = -1;        
        anIt.data().entry = QString::null;
      }
      emit selectionChanged( anIt.key() ); 
    }
}

bool SalomeApp_Dialog::hasSelection( const int id ) const
{
  return myObjects.contains( id ) && !myObjects[ id ].entry.isEmpty();
}

void SalomeApp_Dialog::clearSelection( const int id )
{
  if( myObjects.contains( id ) )
  {
    myObjects[ id ].entry = QString::null;
    myObjects[ id ].type = -1;
    myObjects[ id ].edit->setText( QString::null );
    emit selectionChanged( id );
  }
}

QString SalomeApp_Dialog::selectedObject( const int id ) const
{
  if( myObjects.contains( id ) )
    return myObjects[ id ].entry;
  else
    return QString::null;
}

void SalomeApp_Dialog::objectSelection( SelectedObjects& objs ) const
{
  //objs.clear();
  ObjectMap::const_iterator anIt = myObjects.begin(),
                            aLast = myObjects.end();
  for( ; anIt!=aLast; anIt++ )
  {
    QString entry = selectedObject( anIt.key() );
    if( !entry.isEmpty() )
      objs.insert( anIt.key(), entry );
  }
}

int SalomeApp_Dialog::createObject( const QString& label, QWidget* parent, const int id )
{
  static int _id = -1;

  int nid = id>=0 ? id : --_id;
  if( !myObjects.contains( nid ) )
  {
    QLabel* lab = new QLabel( label, parent );
    myObjects[ nid ].label = lab;
    
    QPushButton* but = new QPushButton( parent );
    but->setIconSet( QIconSet( myPixmap ) );
    but->setToggleButton( true );
    connect( but, SIGNAL( toggled( bool ) ), this, SLOT( onToggled( bool ) ) );
    myObjects[ nid ].btn = but;

    QLineEdit* ne = new QLineEdit( parent );
    ne->setReadOnly( true );
    myObjects[ nid ].edit = ne;
  }
  return nid;
}

void SalomeApp_Dialog::renameObject( const int id, const QString& label )
{
  if( myObjects.contains( id ) )
    myObjects[ id ].label->setText( label );
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

  TypesList& internal = myObjects[ id ].types;
    
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

  TypesList& internal = myObjects[ id ].types;

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
    return myObjects[ id ].types.contains( type );
  else
    return false;
}

void SalomeApp_Dialog::objectTypes( const int id, TypesList& list ) const
{
  if( myObjects.contains( id ) )
  {
    TypesList::const_iterator anIt = myObjects[ id ].types.begin(),
                              aLast = myObjects[ id ].types.end();
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
    if( anIt.data().btn==but )
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

void SalomeApp_Dialog::updateObject( const int id )
{
  if( hasSelection( id ) && !isCorrectType( id, myObjects[ id ].type ) )
    clearSelection( id );
}

bool SalomeApp_Dialog::isCorrectType( const int id, const int type ) const
{
  if( myObjects.contains( id ) )
    return hasObjectType( id, type ) || myObjects[ id ].types.isEmpty();
  else
    return false;
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
    anIt.data().btn->setIconSet( p );
}

void SalomeApp_Dialog::setObjectPixmap( const QString& section, const QString& file )
{
  SUIT_ResourceMgr* mgr = resMgr();
  if( mgr )
    setObjectPixmap( mgr->loadPixmap( section, file ) );
}
