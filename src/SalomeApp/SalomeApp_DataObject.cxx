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
#include "SalomeApp_DataObject.h"

#include "SalomeApp_Study.h"
#include "LightApp_RootObject.h"

#include <CAM_DataObject.h>

#include <SUIT_Application.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_DataObjectKey.h>

#include <qobject.h>

#include <SALOMEDSClient_AttributeReal.hxx>
#include <SALOMEDSClient_AttributeInteger.hxx>
#include <SALOMEDSClient_AttributeComment.hxx>
#include <SALOMEDSClient_AttributeTableOfReal.hxx>
#include <SALOMEDSClient_AttributeTableOfInteger.hxx>

#include <iostream>
#include <sys/time.h>
static long tcount=0;
static long cumul;
#define START_TIMING timeval tv; gettimeofday(&tv,0);long tt0=tv.tv_usec+tv.tv_sec*1000000;
#define END_TIMING(NUMBER) \
  tcount=tcount+1;gettimeofday(&tv,0);cumul=cumul+tv.tv_usec+tv.tv_sec*1000000 -tt0; \
  if(tcount==NUMBER){ std::cerr << __FILE__ << __LINE__ << " temps CPU(mus): " << cumul << std::endl; tcount=0;cumul=0; }
#include <map>
std::map<std::string,QString>  pixmapNameMap;
std::map< std::string , QPixmap >  mypixmapMap;


/*!Constructor. Initialize by \a parent*/
SalomeApp_DataObject::SalomeApp_DataObject( SUIT_DataObject* parent )
: LightApp_DataObject( parent ),
  CAM_DataObject( parent ),
  myEntry( "" ),
  myName( "" )
{
}

/*!Constructor. Initialize by \a parent and SObject*/
SalomeApp_DataObject::SalomeApp_DataObject( const _PTR(SObject)& sobj, SUIT_DataObject* parent )
: LightApp_DataObject( parent ),
  CAM_DataObject( parent ),
  myName( "" )
{
  myObject = sobj;
  myEntry = myObject->GetID().c_str();
}

/*!Destructor. Do nothing.*/
SalomeApp_DataObject::~SalomeApp_DataObject()
{
}

/*!Gets object ID.
 *\retval QString
 */
QString SalomeApp_DataObject::entry() const
{
 if ( myObject )
    return myObject->GetID().c_str();
  return QString::null;
  //return myEntry;
}

/*!Gets name of object.*/
QString SalomeApp_DataObject::name() const
{
  //START_TIMING
  //if ( myName.isEmpty() )
  {
    QString str;
    if ( myObject )
      str = myObject->GetName().c_str();

    if ( str.isEmpty() )
    {
      _PTR(SObject) refObj = referencedObject();
      if ( refObj )
        str = refObj->GetName().c_str();
    }

    if ( isReference() )
      {
        if ( !(QString(referencedObject()->GetName().c_str()).isEmpty()) )
	  str = QString( "* " ) + str;
        else
	  str = QString( "<Invalid Reference>" );
      }
    /*
      */
    SalomeApp_DataObject* that = (SalomeApp_DataObject*)this;
    that->myName = str;
  }
  //END_TIMING(25)
  return myName;
}

/*!Gets icon picture of object.*/
QPixmap SalomeApp_DataObject::icon() const
{
  //START_TIMING
  _PTR(GenericAttribute) anAttr;
  if ( myObject && myObject->FindAttribute( anAttr, "AttributePixMap" ) ){
    _PTR(AttributePixMap) aPixAttr ( anAttr );
    if ( aPixAttr->HasPixMap() ){
      QString componentType = componentDataType();
      QString pixmapID      = aPixAttr->GetPixMap().c_str();
      // select a plugin within a component
      QStringList plugin_pixmap = QStringList::split(QString("::"), pixmapID);
      if ( plugin_pixmap.size() == 2 ) {
        componentType = plugin_pixmap.front();
        pixmapID      = plugin_pixmap.back();
      }
      //CCAR: tr is expansive
      QString pixmapName = QObject::tr( pixmapID );
      LightApp_RootObject* aRoot = dynamic_cast<LightApp_RootObject*>( root() );
      if ( aRoot && aRoot->study() ) {
//CCAR
#if 0
      //std::string key= componentDataType().latin1() ;
      std::string key;

    const SalomeApp_DataObject* compObj = dynamic_cast<SalomeApp_DataObject*>( componentObject() );
    if ( compObj && compObj->object() )
    {
      _PTR(SComponent) aComp( compObj->object() );
      if ( aComp ) {
        key=aComp->ComponentDataType();
      }
    }

      key= key + "/" + name;
      QPixmap p;
      if( mypixmapMap.count(key) !=0 )
        {
          p=mypixmapMap[key];
        }
      else
        {
	  SUIT_ResourceMgr* mgr = aRoot->study()->application()->resourceMgr();
	  p= mgr->loadPixmap( componentDataType(), pixmapName, false ); 
          mypixmapMap[key]=p;
        }
#else
	SUIT_ResourceMgr* mgr = aRoot->study()->application()->resourceMgr();
	return mgr->loadPixmap( componentType, pixmapName, false ); 
//GDD
#endif
     }
    }
  }
  //END_TIMING(50)
  return QPixmap();
}

/*!Gets text value for one of entity:
 *\li Value           (id = SalomeApp_DataObject::CT_Value)
 *\li Entry           (id = SalomeApp_DataObject::CT_Entry)
 *\li IOR             (id = SalomeApp_DataObject::CT_IOR)
 *\li Reference entry (id = SalomeApp_DataObject::CT_RefEntry)
 */
QString SalomeApp_DataObject::text( const int id ) const
{
  //START_TIMING
  QString txt;
  switch ( id )
  {
  case CT_Value:
#ifndef WNT
    if ( componentObject() != this )
#else
    if ( componentObject() != (SUIT_DataObject*)this )
#endif
      txt = value( object() );
      if ( txt.isEmpty() )
        {
//CCAR: no need to calculate if it is the same object
#if 0
          _PTR(SObject) o=referencedObject();
          if(o != object())
	    txt = value( o );
#else
	    txt = value( referencedObject() );
#endif
        }
    break;
  case CT_Entry:
    txt = entry( object() );
    break;
  case CT_IOR:
    txt = ior( referencedObject() );
    break;
  case CT_RefEntry:
    if ( isReference() )
      txt = entry( referencedObject() );
    break;
  }
  //END_TIMING(50)
  return txt;
}

/*!Get color value for one of entity:
 *\li Text color
 *\li Highlight color
 *\li Higlighted text color
 */
QColor SalomeApp_DataObject::color( const ColorRole cr ) const
{
  QColor clr;
  switch ( cr )
  {
  case Text:
    if ( isReference() )
      {
	if ( !(QString(referencedObject()->GetName().c_str()).isEmpty()) )
	  clr = QColor( 255, 0, 0 );
	else
	  clr = QColor( 200, 200, 200 );
      }
    else if ( myObject )
    {
      _PTR(GenericAttribute) anAttr;
      if ( myObject->FindAttribute( anAttr, "AttributeTextColor" ) )
      {
	_PTR(AttributeTextColor) aColAttr = anAttr;
	clr = QColor( (int)aColAttr->TextColor().R, (int)aColAttr->TextColor().G, (int)aColAttr->TextColor().B );
      }
    }
    break;
  case Highlight:
    if ( isReference() )
      {
	if ( !(QString(referencedObject()->GetName().c_str()).isEmpty()) )
	  clr = QColor( 255, 0, 0 );
	else
	  clr = QColor( 200, 200, 200 );
      }
    else if ( myObject )
    {
      _PTR(GenericAttribute) anAttr;
      if( myObject->FindAttribute ( anAttr, "AttributeTextHighlightColor") )
      {
        _PTR(AttributeTextHighlightColor) aHighColAttr = anAttr;
	clr = QColor( (int)(aHighColAttr->TextHighlightColor().R), 
		      (int)(aHighColAttr->TextHighlightColor().G), 
		      (int)(aHighColAttr->TextHighlightColor().B));
      }
    }
    break;
  case HighlightedText:
    if ( isReference() )
      clr = QColor( 255, 255, 255 );
    break;
  }
  return clr;
}

/*!Gets tooltip.*/
QString SalomeApp_DataObject::toolTip() const
{
  //return object()->Name();
  return QString( "Object \'%1\', module \'%2\', ID=%3" ).arg( name() ).arg( componentDataType() ).arg( entry() );
}

/*!Get component type.*/
QString SalomeApp_DataObject::componentDataType() const
{
  //  if ( myCompDataType.isEmpty() ) {
    const SalomeApp_DataObject* compObj = dynamic_cast<SalomeApp_DataObject*>( componentObject() );
    if ( compObj && compObj->object() )
    {
      _PTR(SComponent) aComp( compObj->object() );
      if ( aComp ) {
        SalomeApp_DataObject* that = (SalomeApp_DataObject*)this;
        that->myCompDataType = aComp->ComponentDataType().c_str();
      }
    }
    //  }
  return myCompDataType;
}

/*!Gets object.*/
_PTR(SObject) SalomeApp_DataObject::object() const
{
  return myObject;
}

/*!Checks: Is object reference.*/
bool SalomeApp_DataObject::isReference() const
{
  bool isRef = false;
  if ( myObject )
  {
    _PTR(SObject) refObj;
    isRef = myObject->ReferencedObject( refObj );
  }
  return isRef;
}

/*!Gets reference object.*/
_PTR(SObject) SalomeApp_DataObject::referencedObject() const
{
  _PTR(SObject) refObj;
  _PTR(SObject) obj = myObject;
  while ( obj && obj->ReferencedObject( refObj ) )
    obj = refObj;

  return obj;
}

/*!Gets IOR*/
QString SalomeApp_DataObject::ior( const _PTR(SObject)& obj ) const
{
  //START_TIMING
  QString txt;
  if ( obj )
  {
    _PTR(GenericAttribute) attr;
    if ( obj->FindAttribute( attr, "AttributeIOR" ) )
    {
      _PTR(AttributeIOR) iorAttr = attr;
      if ( iorAttr )
      {
//CCAR
#if 0
	std::string str = "IOR:";
#else
	std::string str = iorAttr->Value();
#endif
//CCAR
#if 0
	txt = QString( str );
#else
	txt = QString( str.c_str() );
#endif
  //END_TIMING(50)
      }
    }
  }
  return txt;
}

/*!Gets Entry*/
QString SalomeApp_DataObject::entry( const _PTR(SObject)& obj ) const
{
  QString txt;
  if ( obj )
  {
    std::string str = obj->GetID();
    txt = QString( str.c_str() );
  }
  return txt;
}

/*!Value*/
QString SalomeApp_DataObject::value( const _PTR(SObject)& obj ) const
{
  //START_TIMING
  if ( !obj )
    return QString::null;

  QString val;
  _PTR(GenericAttribute) attr;

  if ( obj->FindAttribute( attr, "AttributeInteger" ) )
  {
    _PTR(AttributeInteger) intAttr = attr;
    if ( intAttr )
      val = QString::number( intAttr->Value() );
  }
  else if ( obj->FindAttribute( attr, "AttributeReal" ) )
  {
    _PTR(AttributeReal) realAttr = attr;
    if ( realAttr )
      val = QString::number( realAttr->Value() );
  }
  else if ( obj->FindAttribute( attr, "AttributeTableOfInteger" ) )
  {
    _PTR(AttributeTableOfInteger) tableAttr = attr;
    std::string title = tableAttr->GetTitle();
    val = QString( title.c_str() );
    if ( !val.isEmpty() )
      val += QString( " " );
    val += QString( "[%1,%2]" ).arg( tableAttr->GetNbRows() ).arg( tableAttr->GetNbColumns() );
  }
  else if ( obj->FindAttribute( attr, "AttributeTableOfReal" ) )
  {
    _PTR(AttributeTableOfReal) tableAttr = attr;
    std::string title = tableAttr->GetTitle();
    val = QString( title.c_str() );
    if ( !val.isEmpty() )
      val += QString( " " );
    val += QString( "[%1,%2]" ).arg( tableAttr->GetNbRows() ).arg( tableAttr->GetNbColumns() );
  }
  else if ( obj->FindAttribute( attr, "AttributeComment") )
  {
    _PTR(AttributeComment) comm = attr;
    std::string str = comm->Value();
    val = QString( str.c_str() );
  }

  //END_TIMING(25)
  return val;
}





/*!Constructor.Initialize by \a parent.*/
SalomeApp_ModuleObject::SalomeApp_ModuleObject( SUIT_DataObject* parent )
: SalomeApp_DataObject( parent ),
  CAM_RootObject( parent ),
  CAM_DataObject( parent )
{
}

/*!Constructor.Initialize by \a parent and SObject.*/
SalomeApp_ModuleObject::SalomeApp_ModuleObject( const _PTR(SObject)& sobj, SUIT_DataObject* parent )
: SalomeApp_DataObject( sobj, parent ),
  CAM_RootObject( 0, parent ),
  CAM_DataObject( parent )
{
}

/*!Constructor.Initialize by \a parent and CAM_DataModel.*/
SalomeApp_ModuleObject::SalomeApp_ModuleObject( CAM_DataModel* dm, const _PTR(SObject)& sobj, SUIT_DataObject* parent )
: SalomeApp_DataObject( sobj, parent ),
  CAM_RootObject( dm, parent ),
  CAM_DataObject( parent )
{
}

/*!Destructor. Do nothing.*/
SalomeApp_ModuleObject::~SalomeApp_ModuleObject()
{
}

/*!Returns module name */
QString SalomeApp_ModuleObject::name() const
{
  return SalomeApp_DataObject::name();
}




/*!Constructor.Initialize by \a parent.*/
SalomeApp_SavePointObject::SalomeApp_SavePointObject( SUIT_DataObject* _parent, const int id, SalomeApp_Study* study )
  : LightApp_DataObject( _parent ), CAM_DataObject( _parent ), // IMPORTANT TO CALL ALL VIRTUAL CONSTRUCTORS!
    myId( id ),
    myStudy( study )
{
}

/*!Destructor. Do nothing.*/
SalomeApp_SavePointObject::~SalomeApp_SavePointObject()
{
}

/*!Returns save points ID */
int SalomeApp_SavePointObject::getId() const
{
  return myId;
}

/*!Returns "invalid" entry, which does not correspond to any object in data structure
  but indicates that it is a save point object  */
QString SalomeApp_SavePointObject::entry() const
{
  return QObject::tr( "SAVE_POINT_DEF_NAME" ) + QString::number( myId );
}

/*!Returns displayed name of object */
QString SalomeApp_SavePointObject::name() const
{
  return myStudy->getNameOfSavePoint( myId );
}

/*!Gets icon picture of object.*/
QPixmap SalomeApp_SavePointObject::icon() const
{
  return QPixmap();
}

/*!Gets tooltip.*/
QString SalomeApp_SavePointObject::toolTip() const
{
  return QObject::tr( "SAVE_POINT_OBJECT_TOOLTIP" ).arg( name() );
}

