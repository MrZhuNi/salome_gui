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
// File   : LightApp_DataObject.h
// Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
// 

#ifndef LIGHTAPP_DATAOBJECT_H
#define LIGHTAPP_DATAOBJECT_H

#include "LightApp.h"
#include <CAM_DataObject.h>

class CAM_DataModel;
class LightApp_Study;

#ifdef WIN32
#pragma warning ( disable:4251 )
#pragma warning ( disable:4250 )
#endif

class LIGHTAPP_EXPORT LightApp_DataObject : public virtual CAM_DataObject
{
  class Key;

public:
  //! Column index
  enum { 
    EntryIdx = NameIdx + 1    //!< entry column
  };

public:
  LightApp_DataObject( SUIT_DataObject* = 0 );
  virtual ~LightApp_DataObject();

  virtual int                     columnCount() const;
  virtual QString                 columnTitle( const int = NameIdx ) const;
  virtual bool                    appropriate( const int = NameIdx ) const;

  virtual SUIT_DataObjectKey*     key() const;
  virtual QString                 entry() const;

  virtual QString                 text( const int = NameIdx ) const;

  virtual SUIT_DataObject*        componentObject() const;
  virtual QString                 componentDataType() const;

  virtual bool                    customSorting( const int = NameIdx ) const;
  virtual bool                    compare( const QVariant&, const QVariant&,
					   const int = NameIdx ) const;

protected:
  QString                         myCompDataType;
  SUIT_DataObject*                myCompObject;
};

class LIGHTAPP_EXPORT LightApp_ModuleObject
: public virtual LightApp_DataObject, public CAM_ModuleObject
{
public:
  LightApp_ModuleObject( SUIT_DataObject* = 0 );
  LightApp_ModuleObject( CAM_DataModel*, SUIT_DataObject* = 0 );

  virtual ~LightApp_ModuleObject();

  virtual QString        name() const;
  QPixmap                icon( const int = NameIdx ) const;
  QString                toolTip( const int = NameIdx ) const;

  virtual void           insertChild( SUIT_DataObject*, int );
};

class LIGHTAPP_EXPORT LightApp_RootObject : public virtual LightApp_DataObject
{
public:
  LightApp_RootObject( LightApp_Study* );

  virtual ~LightApp_RootObject();
    
  void                   setStudy( LightApp_Study* );
  LightApp_Study*        study() const;
  
private:
  LightApp_Study*        myStudy;
};

#endif  // LIGHTAPP_DATAOBJECT_H
