#include "LightApp_OBSelector.h"

#include "LightApp_DataOwner.h"
#include "LightApp_DataObject.h"

#include <OB_Browser.h>

#include <SUIT_DataObjectIterator.h>

/*!
  Constructor
*/
LightApp_OBSelector::LightApp_OBSelector( OB_Browser* ob, SUIT_SelectionMgr* mgr )
: SUIT_Selector( mgr, ob ),
  myBrowser( ob )
{
  if ( myBrowser ) {
    connect( myBrowser, SIGNAL( selectionChanged() ), this, SLOT( onSelectionChanged() ) );
  }    
}

/*!
  Destructor
*/
LightApp_OBSelector::~LightApp_OBSelector()
{
}

/*!
  Gets browser.
*/
OB_Browser* LightApp_OBSelector::browser() const
{
  return myBrowser;
}

/*!
  Gets selection.
*/
void LightApp_OBSelector::getSelection( SUIT_DataOwnerPtrList& list ) const
{
  if ( !myBrowser )
    return;

  DataObjectList objlist;
  myBrowser->getSelected( objlist );
  for ( DataObjectListIterator it( objlist ); it.current(); ++it )
  {
    LightApp_DataObject* obj = dynamic_cast<LightApp_DataObject*>( it.current() );
    if ( obj )
    {
      Handle( SALOME_InteractiveObject ) aSObj = new SALOME_InteractiveObject
	( obj->entry(), obj->componentDataType(), obj->name() );
      list.append( SUIT_DataOwnerPtr( new LightApp_DataOwner( aSObj  ) ) );
    }
  }
}

/*!Sets selection.*/
void LightApp_OBSelector::setSelection( const SUIT_DataOwnerPtrList& list )
{
  if ( !myBrowser )
    return;

  QMap<QString, LightApp_DataObject*> map;
  fillEntries( map );

  DataObjectList objList;
  for ( SUIT_DataOwnerPtrList::const_iterator it = list.begin(); it != list.end(); ++it )
  {
    const LightApp_DataOwner* owner = dynamic_cast<const LightApp_DataOwner*>( (*it).operator->() );
    if ( owner && map.contains( owner->entry() ) )
      objList.append( map[owner->entry()] );
  }

  myBrowser->setSelected( objList );
}

/*!On selection changed.*/
void LightApp_OBSelector::onSelectionChanged()
{
  selectionChanged();
}

/*!Fill entries.*/
void LightApp_OBSelector::fillEntries( QMap<QString, LightApp_DataObject*>& entires )
{
  entires.clear();

  if ( !myBrowser )
    return;

  for ( SUIT_DataObjectIterator it( myBrowser->getRootObject(),
                                    SUIT_DataObjectIterator::DepthLeft ); it.current(); ++it )
  {
    LightApp_DataObject* obj = dynamic_cast<LightApp_DataObject*>( it.current() );
    if ( obj )
      entires.insert( obj->entry(), obj );
  }
}
