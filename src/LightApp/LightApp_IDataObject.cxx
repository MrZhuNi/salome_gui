//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "LightApp_IDataObject.h"

#define ENTRY "entry"
#define NAME "name"
#define OBJECT_NODE "objectNode"
#define ROOT_NODE "rootNode"

/*!
  \class LightApp_IDataObject
  \brief Base data object class to build the object tree in Object Browser.
*/

/*!
  \brief Constructor. 
  \param parent parent data object
*/
LightApp_IDataObject::LightApp_IDataObject( SUIT_DataObject* parent )
: CAM_DataObject( parent ),   
  LightApp_DataObject( parent ),
  myName( "" ),
  myEntry( "" )
{
}

/*!
  \brief Destructor.
*/
LightApp_IDataObject::~LightApp_IDataObject()
{
}

/*!
  \brief Set the data object name.
*/
void  LightApp_IDataObject::setName( const QString& theName )
{
  myName = theName; 
}

/*!
  \brief Set the data object ID.
*/
void  LightApp_IDataObject::setEntry( const QString& theEntry )
{
  myEntry = theEntry;
}

/*!
  \brief Return the data object name.
*/
QString LightApp_IDataObject::name() const
{
  return myName;
}
  
/*!
  \brief Return the data object name.
*/
QString LightApp_IDataObject::entry() const
{
  return myEntry;
}

QString LightApp_IDataObject::GetTreeDoc( CAM_DataObject* theRoot )
{

  QDomDocument aDoc;
  QDomElement treeRoot = aDoc.createElement( ROOT_NODE );
  if ( dumpObject( theRoot, treeRoot ) )
  {
    aDoc.appendChild( treeRoot );
    dumpTree( theRoot, treeRoot );
  }
  return aDoc.toString();
}

bool LightApp_IDataObject::dumpObject( SUIT_DataObject* objNode, QDomElement& treeNode ) 
{
  if( objNode != 0 ) {
    LightApp_DataObject* obj = dynamic_cast<LightApp_DataObject*>( objNode );
    treeNode.setAttribute( ENTRY, obj->entry() );
    treeNode.setAttribute( NAME, obj->name() );
    return true;
  }
  return false;
}

void LightApp_IDataObject::dumpTree( SUIT_DataObject* currentNode, QDomElement& currentRoot ) 
{
    for( int i=0; i < currentNode->childCount(); i++ ) {
      QDomElement treeChild = currentRoot.ownerDocument().createElement( OBJECT_NODE );
      SUIT_DataObject* obj = currentNode->childObject( i );
      dumpObject( obj, treeChild );
      currentRoot.appendChild( treeChild );
      dumpTree( obj, treeChild );
    } 
}  

void LightApp_IDataObject::BuildITree( SUIT_DataObject* theRoot, const  QString& theTree )
{
  QDomDocument aDoc;
  if( aDoc.setContent( theTree ) ) {
     QDomElement aRoot = aDoc.documentElement();
     LightApp_IDataObject* obj = new LightApp_IDataObject();
     BuildNode( aRoot, *obj );
     theRoot->appendChild( obj );
     BuildIModuleTree( aRoot, *obj ); 
  }
}

void LightApp_IDataObject::BuildNode( const QDomElement& treeNode, LightApp_IDataObject& objNode ) 
{
  QString aName = treeNode.attribute( NAME );
  objNode.setName( aName );
  QString aEntry = treeNode.attribute( ENTRY );
  objNode.setEntry( aEntry );
}

void LightApp_IDataObject::BuildIModuleTree( const QDomElement& currentRoot, LightApp_IDataObject& currentNode ) 
{
  for( QDomElement treeChild = currentRoot.firstChildElement(); !treeChild.isNull();
       treeChild = treeChild.nextSiblingElement() ) {
    LightApp_IDataObject* obj = new LightApp_IDataObject();
    BuildNode( treeChild, *obj );
    currentNode.appendChild( obj );
    BuildIModuleTree( treeChild, *obj );
  } 
}
