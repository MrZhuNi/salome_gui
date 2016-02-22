// Copyright (C) 2007-2015  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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

// File   : PyConsole_Console.cxx
// Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//
/*!
  \class PyConsole_Console
  \brief Python console widget.
*/  

#include "PyConsole_Interp.h"   /// !!! WARNING !!! THIS INCLUDE MUST BE VERY FIRST !!!
#include "PyConsole_Console.h"
#include "PyConsole_EnhEditor.h"
#include "PyConsole_EnhInterp.h"

#include <Qtx.h>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QEvent>
#include <QMenu>
#include <QVBoxLayout>

/*!
  \brief Constructor.

  Creates new python console widget.
  \param parent parent widget
  \param interp python interpreter
*/
PyConsole_Console::PyConsole_Console( QWidget* parent, PyConsole_Interp* interp )
  : PyConsole_ConsoleBase( parent, interp )
{
}

/**
 * Protected constructor.
 */
PyConsole_Console::PyConsole_Console( QWidget* parent, PyConsole_Interp* i,  PyConsole_Editor* e )
  : PyConsole_ConsoleBase(parent,i,e)
{}

/*!
  \brief Destructor.

  Does nothing for the moment.
*/
PyConsole_Console::~PyConsole_Console()
{
}

/*!
  \brief Create the context popup menu.

  Fill in the popup menu with the commands.

  \param menu context popup menu
*/
void PyConsole_Console::contextMenuPopup( QMenu* menu )
{
  if ( myEditor->isReadOnly() )
    return;

  menu->addAction( myActions[CopyId] );
  menu->addAction( myActions[PasteId] );
  menu->addAction( myActions[ClearId] );
  menu->addSeparator();
  menu->addAction( myActions[SelectAllId] );
  menu->addSeparator();
  menu->addAction( myActions[DumpCommandsId] );
  if ( !myEditor->isLogging() )
    menu->addAction( myActions[StartLogId] );
  else
    menu->addAction( myActions[StopLogId] );

  Qtx::simplifySeparators( menu );

  updateActions();
}

/*!
  \brief Event handler.

  Handles context menu request event.

  \param o object
  \param e event
  \return True if the event is processed and further processing should be stopped
*/
bool PyConsole_Console::eventFilter( QObject* o, QEvent* e )
{
  if ( o == myEditor->viewport() && e->type() == QEvent::ContextMenu )
  {
    contextMenuRequest( (QContextMenuEvent*)e );
    return true;
  }
  return QWidget::eventFilter( o, e );
}

/**
 * Similar to constructor of the base class but using enhanced objects.
 * TODO: this should really be done in a factory to avoid code duplication.
 * @param parent
 * @param interp
 */
PyConsole_EnhConsole::PyConsole_EnhConsole( QWidget* parent, PyConsole_Interp* interp )
  : PyConsole_Console( parent, interp, 0 )
{
  PyConsole_Interp* anInterp = interp ? interp : new PyConsole_EnhInterp();

  // initialize Python interpretator
  anInterp->initialize();

  // create editor console
  QVBoxLayout* lay = new QVBoxLayout( this );
  lay->setMargin( 0 );
  myEditor = new PyConsole_EnhEditor( anInterp, this );
  char* synchronous = getenv("PYTHON_CONSOLE_SYNC");
  if (synchronous && atoi(synchronous))
  {
      myEditor->setIsSync(true);
  }
  myEditor->viewport()->installEventFilter( this );
  lay->addWidget( myEditor );

  createActions();
}
