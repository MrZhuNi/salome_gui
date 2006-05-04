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
// See http://www.salome-platform.org/
//
#ifndef LIGHTAPP_SELECTIONMGR_H
#define LIGHTAPP_SELECTIONMGR_H

#include "LightApp.h"

#include <SUIT_SelectionMgr.h>

#ifndef DISABLE_SALOMEOBJECT
  #include <SALOME_InteractiveObject.hxx>
  #include <qmap.h>

  class SALOME_ListIO;
  class TColStd_IndexedMapOfInteger;
  class TColStd_MapOfInteger;
#else
#include <qstringlist.h>
#endif

class LightApp_Application;

/*!
  Custom selection manager, allowing to work with object selection
  (additionally to data owners) and to access to sub-selection of objects
*/
class LIGHTAPP_EXPORT LightApp_SelectionMgr : public SUIT_SelectionMgr
{
  Q_OBJECT

public:
  LightApp_SelectionMgr( LightApp_Application*, const bool = true );
  virtual ~LightApp_SelectionMgr();

  LightApp_Application* application() const;

#ifndef DISABLE_SALOMEOBJECT
  typedef QMap< Handle(SALOME_InteractiveObject), TColStd_IndexedMapOfInteger > MapIOOfMapOfInteger;
  typedef QMap< QString, TColStd_IndexedMapOfInteger > MapEntryOfMapOfInteger;

  void                   selectedObjects( SALOME_ListIO&, const QString& = QString::null, const bool = true ) const;
  void                   setSelectedObjects( const SALOME_ListIO&, const bool = false );

  void                   GetIndexes( const Handle(SALOME_InteractiveObject)& IObject, 
				     TColStd_IndexedMapOfInteger& theIndex );
  void                   GetIndexes( const QString& theEntry, 
				     TColStd_IndexedMapOfInteger& theIndex );

  bool                   AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, 
					   const TColStd_MapOfInteger& theIndices, 
					   bool modeShift );

  void                   selectObjects( const Handle(SALOME_InteractiveObject)& IObject, 
					TColStd_IndexedMapOfInteger theIndex, bool append );
  void                   selectObjects( MapIOOfMapOfInteger theMapIO, bool append );

  void                   selectedSubOwners( MapEntryOfMapOfInteger& theMap );
#else
  void                   selectedObjects( QStringList&, const QString& = QString::null, const bool = true ) const;
#endif

signals:
  void                   currentSelectionChanged();

private:
  virtual void           selectionChanged( SUIT_Selector* );

private:
  LightApp_Application* myApp;
};

#endif
