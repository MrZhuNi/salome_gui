// Copyright (C) 2007-2013  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
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
// Author : Adrien Bruneton (CEA/DEN)
// Created on: 3 avr. 2013

#include "PyConsole_Request.h"

#include "PyInterp_Event.h"
#include "PyConsole_Event.h"
#include "PyInterp_Interp.h"
#include "PyConsole_Editor.h"

#include <QCoreApplication>

ExecCommand::ExecCommand( PyInterp_Interp*        theInterp,
               const QString&          theCommand,
               PyConsole_Editor*       theListener,
               bool                    sync )
    : PyInterp_LockRequest( theInterp, theListener, sync ),
      myCommand( theCommand ), myState( PyInterp_Event::ES_OK )
  {}

void ExecCommand::execute()
{
  if ( myCommand != "" )
    {
      int ret = getInterp()->run( myCommand.toUtf8().data() );
      if ( ret < 0 )
        myState = PyInterp_Event::ES_ERROR;
      else if ( ret > 0 )
        myState = PyInterp_Event::ES_INCOMPLETE;
    }
}

QEvent* ExecCommand::createEvent() const
{
  if ( IsSync() )
    QCoreApplication::sendPostedEvents( listener(), PrintEvent::EVENT_ID );
  return new PyInterp_Event( myState, (PyInterp_Request*)this );
}
