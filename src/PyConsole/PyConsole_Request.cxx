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
#include "PyConsole_EnhInterp.h"
#include "PyConsole_EnhEditor.h"

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

QEvent* ExecCommand::createEvent()
{
  if ( IsSync() )
    QCoreApplication::sendPostedEvents( listener(), PrintEvent::EVENT_ID );
  return new PyInterp_Event( myState, this );
}



CompletionCommand::CompletionCommand( PyConsole_EnhInterp*  theInterp,
               const QString&          input,
               const QString&         startMatch,
               PyConsole_EnhEditor*           theListener,
               bool                    sync)
     : PyInterp_LockRequest( theInterp, theListener, sync ),
       _tabSuccess(false), _dirArg(input), _startMatch(startMatch)
{}

void CompletionCommand::execute()
{
  PyConsole_EnhInterp * interp = static_cast<PyConsole_EnhInterp *>(getInterp());
    int ret = interp->runDirCommand( _dirArg,  _startMatch);
    if (ret == 0)
      _tabSuccess = true;
    else
      _tabSuccess = false;
}

QEvent* CompletionCommand::createEvent()
{
  int typ = _tabSuccess ? PyInterp_Event::ES_TAB_COMPLETE_OK : PyInterp_Event::ES_TAB_COMPLETE_ERR;

  return new PyInterp_Event( typ, this);
}



