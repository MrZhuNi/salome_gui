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
#ifndef LightApp_IDATAOBJECT_H
#define  LightApp_IDATAOBJECT_H

#include <LightApp_DataObject.h>
#include <SUIT_DataObject.h>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

class LIGHTAPP_EXPORT LightApp_IDataObject : public LightApp_DataObject
{
  
public:
  LightApp_IDataObject( SUIT_DataObject* = 0 );
  virtual ~LightApp_IDataObject();

  virtual QString                 name() const;
  virtual QString                 entry() const;

  void                            setName( const QString& );
  void                            setEntry( const QString& );
  static QString                  GetTreeDoc( CAM_DataObject* theRoot );
  static void                     BuildITree( SUIT_DataObject* theRoot, const QString& theTree = 0 );

protected:
  static bool                     dumpObject( SUIT_DataObject* objNode, QDomElement& treeNode );
  static void                     dumpTree( SUIT_DataObject* currentNode, QDomElement& currentRoot );
  static void                     BuildNode( const QDomElement& treeNode, LightApp_IDataObject& objNode );
  static void                     BuildIModuleTree( const QDomElement& currentRoot, LightApp_IDataObject& currentNode );


private:
  QString                         myName;
  QString                         myEntry;
};

#endif  // LightApp_IDATAOBJECT_H
