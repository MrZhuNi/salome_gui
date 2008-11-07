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
// File   : SUIT_DataBrowser.h
// Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
// 

#ifndef SUIT_DATABROWSER_H
#define SUIT_DATABROWSER_H

#include "SUIT.h"
#include "SUIT_PopupClient.h"
#include "SUIT_DataObject.h"
#include <OB_Browser.h>

class QShortcut;

class SUIT_EXPORT SUIT_DataBrowser : public OB_Browser, public SUIT_PopupClient
{
  Q_OBJECT

public:
  SUIT_DataBrowser( QWidget* = 0 );
  SUIT_DataBrowser( SUIT_DataObject*, QWidget* = 0 );
  ~SUIT_DataBrowser();

  virtual QString  popupClientType() const;

  SUIT_DataObject* root() const;
  void             setRoot( SUIT_DataObject* );

  bool             autoUpdate() const;
  void             setAutoUpdate( const bool );

  void             updateTree( SUIT_DataObject* = 0, const bool = true );

  int              updateKey() const;
  void             setUpdateKey( const int );

  DataObjectList   getSelected() const;
  void             getSelected( DataObjectList& ) const;

  void             setSelected( const SUIT_DataObject*, const bool = false );
  void             setSelected( const DataObjectList&, const bool = false );

  virtual void     contextMenuPopup( QMenu* );

  void             setAutoSizeFirstColumn( const bool on );
  void             setAutoSizeColumns( const bool on );
  void             setResizeOnExpandItem( const bool on );

  void             ensureItemVisible( const SUIT_DataObject* );

protected:
  virtual void     contextMenuEvent( QContextMenuEvent* );

private:
  void             init( SUIT_DataObject* );

signals:
  void             requestUpdate();
  void             clicked( SUIT_DataObject* );
  void             doubleClicked( SUIT_DataObject* );

private slots:
  void             onModelUpdated();
  void             onClicked( const QModelIndex& );
  void             onDblClicked( const QModelIndex& );
  void             onExpanded( const QModelIndex& );

private:
  QShortcut*       myShortcut;

  bool             myAutoSizeFirstColumn;
  bool             myAutoSizeColumns;
  bool             myResizeOnExpandItem;
};

#endif // SUIT_BROWSER_H
