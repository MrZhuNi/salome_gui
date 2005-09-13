#include "LightApp_DataObject.h"

#include "LightApp_Study.h"
#include "LightApp_RootObject.h"

#include <SUIT_Application.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_DataObjectKey.h>

#include <qobject.h>

/*!
	Class: LightApp_DataObject::Key
	Level: Internal
*/
class LightApp_DataObject::Key : public SUIT_DataObjectKey
{
public:
  Key( const QString& );
  virtual ~Key();

  virtual bool isLess( const SUIT_DataObjectKey* ) const;
  virtual bool isEqual( const SUIT_DataObjectKey* ) const;

private:
  QString myEntry;
};

/*!Constructor. Initialize by \a entry.*/
LightApp_DataObject::Key::Key( const QString& entry )
: SUIT_DataObjectKey(),
  myEntry( entry )
{
}

/*!Destructor. Do nothing.*/
LightApp_DataObject::Key::~Key()
{
}

/*!Checks: Is current key less than \a other.*/
bool LightApp_DataObject::Key::isLess( const SUIT_DataObjectKey* other ) const
{
  Key* that = (Key*)other;
  return myEntry < that->myEntry;
}

/*!Checks: Is current key equal with \a other.*/
bool LightApp_DataObject::Key::isEqual( const SUIT_DataObjectKey* other ) const
{
  Key* that = (Key*)other;
  return myEntry == that->myEntry;
}

/*
	Class: LightApp_DataObject
	Level: Public
*/
/*!Constructor. Initialize by \a parent*/
LightApp_DataObject::LightApp_DataObject( SUIT_DataObject* parent )
: CAM_DataObject( parent )
{
}

/*!Destructor. Do nothing.*/
LightApp_DataObject::~LightApp_DataObject()
{
}

/*!Gets object ID.
 *\retval QString
 */
QString LightApp_DataObject::entry() const
{
  return QString::null;
}

/*!Create and return new key object.*/
SUIT_DataObjectKey* LightApp_DataObject::key() const
{
  QString str = entry();
  return new Key( str );
}

/*!Gets component object.
 *\retval SUIT_DataObject.
 */
SUIT_DataObject* LightApp_DataObject::componentObject() const
{
  SUIT_DataObject* compObj = 0; // for root object (invisible SALOME_ROOT_OBJECT) 

  if ( parent() && parent() == root() ) 
    compObj = (SUIT_DataObject*)this; // for component-level objects
  else 
  {
    compObj = parent(); // for lower level objects
    while ( compObj && compObj->parent() != root() )
      compObj = compObj->parent();
  }
  return compObj;
}

/*!Get component type.*/
QString LightApp_DataObject::componentDataType() const
{
  return "";
}

