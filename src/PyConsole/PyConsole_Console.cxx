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
//  File   : PyConsole_Console.cxx
//  Author : Vadim SANDLER
//  Module : SALOME

/*!
  \class PyConsole_Console
  \brief Python console widget.
*/  

#include "PyConsole_Interp.h"   /// !!! WARNING !!! THIS INCLUDE MUST BE VERY FIRST !!!
#include "PyConsole_Console.h"
#include "PyConsole_Editor.h"

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
: QFrame( parent ),
  myEditor( 0 )
{
  // create python interpreter
  myInterp = interp;
  if ( !myInterp )
    myInterp = new PyConsole_Interp();
  
  // initialize Python interpretator
  myInterp->initialize();
  
  // create editor console
  QVBoxLayout* lay = new QVBoxLayout( this );
  lay->setMargin( 5 );
  myEditor = new PyConsole_Editor( myInterp, this );
  myEditor->viewport()->installEventFilter( this );
  lay->addWidget( myEditor );

  createActions();
}

/*!
  \brief Destructor.

  Does nothing for the moment.
*/
PyConsole_Console::~PyConsole_Console()
{
}

/*!
  \brief Execute python command in the interpreter.
  \param command string with command and arguments
*/
void PyConsole_Console::exec( const QString& command )
{
  if ( myEditor )
    myEditor->exec( command );
}

/*!
  \brief Execute python command in the interpreter 
         and wait until it is finished.
  
  Block execution of main application until the python command is executed.
  \param command string with command and arguments
*/
void PyConsole_Console::execAndWait( const QString& command )
{
  if ( myEditor )
    myEditor->execAndWait( command );
}

/*!
  \brief Get synchronous mode flag value.
  
  \sa setIsSync()
  \return True if python console works in synchronous mode
*/
bool PyConsole_Console::isSync() const
{
  return myEditor->isSync();
}

/*!
  \brief Set synchronous mode flag value.

  In synhronous mode the Python commands are executed in the GUI thread
  and the GUI is blocked until the command is finished. In the asynchronous
  mode each Python command is executed in the separate thread that does not
  block the main GUI loop.

  \param on synhronous mode flag
*/
void PyConsole_Console::setIsSync( const bool on )
{
  myEditor->setIsSync( on );
}

/*!
  \brief Change the python console's font.
  \param f new font
*/
void PyConsole_Console::setFont( const QFont& f )
{
  if( myEditor )
    myEditor->setFont( f );
}

/*!
  \brief Get python console font.
  \return current python console's font
*/
QFont PyConsole_Console::font() const
{
  QFont res;
  if( myEditor )
    res = myEditor->font();
  return res;
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
  return QFrame::eventFilter( o, e );
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

  Qtx::simplifySeparators( menu );

  updateActions();
}

/*!
  \brief Set actions to be visible in the context popup menu.
  
  Actions, which IDs are set in \a flags parameter, will be shown in the 
  context popup menu. Other actions will not be shown.

  \param flags ORed together actions flags
*/
void PyConsole_Console::setMenuActions( const int flags )
{
  myActions[CopyId]->setVisible( flags & CopyId );
  myActions[PasteId]->setVisible( flags & PasteId );
  myActions[ClearId]->setVisible( flags & ClearId );
  myActions[SelectAllId]->setVisible( flags & SelectAllId );
}

/*!
  \brief Test menu action.
  \param flags ORed together actions flags
  \return \c true if all specified actions are visible and
          \c false if at least one specified action is not visible
*/
bool PyConsole_Console::testMenuActions( const int flags ) const
{
  bool ret = true;
  ret = ret && ( !(  flags & CopyId )      || myActions[CopyId]->isVisible() );
  ret = ret && ( !(  flags & PasteId )     || myActions[PasteId]->isVisible() );
  ret = ret && ( !(  flags & ClearId )     || myActions[ClearId]->isVisible() );
  ret = ret && ( !(  flags & SelectAllId ) || myActions[SelectAllId]->isVisible() );
  return ret;
}

/*!
  \brief Create menu actions.

  Create context popup menu actions.
*/
void PyConsole_Console::createActions()
{
  QAction* a = new QAction( tr( "EDIT_COPY_CMD" ), this );
  a->setStatusTip( tr( "EDIT_COPY_CMD" ) );
  connect( a, SIGNAL( triggered( bool ) ), myEditor, SLOT( copy() ) );
  myActions.insert( CopyId, a );

  a = new QAction( tr( "EDIT_PASTE_CMD" ), this );
  a->setStatusTip( tr( "EDIT_PASTE_CMD" ) );
  connect( a, SIGNAL( triggered( bool ) ), myEditor, SLOT( paste() ) );
  myActions.insert( PasteId, a );

  a = new QAction( tr( "EDIT_CLEAR_CMD" ), this );
  a->setStatusTip( tr( "EDIT_CLEAR_CMD" ) );
  connect( a, SIGNAL( triggered( bool ) ), myEditor, SLOT( clear() ) );
  myActions.insert( ClearId, a );

  a = new QAction( tr( "EDIT_SELECTALL_CMD" ), this );
  a->setStatusTip( tr( "EDIT_SELECTALL_CMD" ) );
  connect( a, SIGNAL( triggered( bool ) ), myEditor, SLOT( selectAll() ) );
  myActions.insert( SelectAllId, a );
}

/*!
  \brief Update menu actions.

  Update context popup menu action state.
*/
void PyConsole_Console::updateActions()
{
  myActions[CopyId]->setEnabled( myEditor->textCursor().hasSelection() );
  myActions[PasteId]->setEnabled( !myEditor->isReadOnly() && !QApplication::clipboard()->text().isEmpty() );
  myActions[SelectAllId]->setEnabled( !myEditor->document()->isEmpty() );
}
