#ifndef LIGHTAPP_DATAOBJECT_H
#define LIGHTAPP_DATAOBJECT_H

#include "LightApp.h"

#include "CAM_DataObject.h"
#include "CAM_RootObject.h"

class LightApp_Study;

class LIGHTAPP_EXPORT LightApp_DataObject : public virtual CAM_DataObject
{
  class Key;

public:
  enum { CT_Value, CT_Entry, CT_IOR, CT_RefEntry };

public:
  LightApp_DataObject( SUIT_DataObject* = 0 );
  virtual ~LightApp_DataObject();

  virtual SUIT_DataObjectKey*     key() const;
  virtual QString                 entry() const;

  SUIT_DataObject*                componentObject() const;
  /*! GEOM, SMESH, VISU, etc.*/
  QString                         componentDataType() const;

};

#endif
