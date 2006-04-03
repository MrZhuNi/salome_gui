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
#ifndef SUIT_POPUPCLIENT_H
#define SUIT_POPUPCLIENT_H

#include "SUIT.h"

#include <qobject.h>

class QPopupMenu;
class QContextMenuEvent;

/*!\class SUIT_PopupClient
 * Descr: Base class for instances which creates popup menu on QContextMenuEvent
 */
class SUIT_EXPORT SUIT_PopupClient
{
public:

  class Signal;

  SUIT_PopupClient();
  virtual ~SUIT_PopupClient();

  bool            connectPopupRequest( QObject* reciever, const char* slot );
  bool            disconnectPopupRequest( QObject* reciever, const char* slot );

  virtual QString popupClientType() const = 0;
  virtual void    contextMenuPopup( QPopupMenu* ) {}

protected:
  void    contextMenuRequest( QContextMenuEvent* e );

private:
  Signal* mySignal;
};

/*! Class: SUIT_PopupClient::Signal [internal] \n
 *  Descr: invoke signal which is connected to reciever in SUIT_PopupClient
 */
class SUIT_PopupClient::Signal : public QObject
{
  Q_OBJECT

public:
  Signal();
  virtual ~Signal();

  void    sendSignal( SUIT_PopupClient*, QContextMenuEvent* );

signals:
  void contextMenuRequest( SUIT_PopupClient*, QContextMenuEvent* );
};

#endif
