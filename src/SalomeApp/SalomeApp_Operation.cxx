//  SALOME SalomeApp
//
//  Copyright (C) 2005  CEA/DEN, EDF R&D
//
//
//
//  File   : SalomeApp_Operation.h
//  Author : Sergey LITONIN
//  Module : SALOME

#include <SalomeApp_Operation.h>
#include <SalomeApp_Module.h>
#include <SalomeApp_Application.h>
#include <SalomeApp_Operation.h>
#include <SalomeApp_SelectionMgr.h>
#include <SalomeApp_Dialog.h>

#include <SUIT_Desktop.h>

#include <qapplication.h>

/*
  Class       : SalomeApp_Operation
  Description : Base class for all operations
*/

//=======================================================================
// name    : SalomeApp_Operation
// Purpose : Constructor
//=======================================================================
SalomeApp_Operation::SalomeApp_Operation()
: SUIT_Operation( 0 ),
  myModule( 0 ),
  myIsAutoResumed( false )
{
  
}

SalomeApp_Operation::~SalomeApp_Operation()
{
  
}

//=======================================================================
// name    : module
// Purpose : Get module
//=======================================================================
SalomeApp_Module* SalomeApp_Operation::module() const
{
  return myModule;
}

//=======================================================================
// name    : setModule
// Purpose : 
//=======================================================================
void SalomeApp_Operation::setModule( SalomeApp_Module* theModule )
{
  myModule = theModule;
  setApplication( myModule ? myModule->application() : 0 );
  setStudy( application() ? application()->activeStudy() : 0 );
}

//=======================================================================
// name    : desktop
// Purpose : Get desktop
//=======================================================================
SUIT_Desktop* SalomeApp_Operation::desktop() const
{
  return application() != 0 ? application()->desktop() : 0;
}

//=======================================================================
// name    : resumeOperation
// Purpose : Enable dialog if it was desabled (in suspend method)
//           and activate selection
//=======================================================================
void SalomeApp_Operation::resumeOperation()
{
  setDialogActive( true );
}

//=======================================================================
// name    : startOperation
// Purpose : Connect signal of selection manager to onSelectionDone() slot
//=======================================================================
void SalomeApp_Operation::startOperation()
{
  if( selectionMgr() )
    connect( selectionMgr(), SIGNAL( selectionChanged() ), SLOT( onSelectionDone() ) );
}

//=======================================================================
// name    : suspendOperation
// Purpose : Disable dialog for mouse and key events
//=======================================================================
void SalomeApp_Operation::suspendOperation()
{
  setDialogActive( false );
}

//=======================================================================
// name    : abortOperation
// Purpose : Hide dialog box (if it is exists)
//=======================================================================
void SalomeApp_Operation::abortOperation()
{
  setDialogActive( true );
  if ( dlg() )
    dlg()->hide();

  if( selectionMgr() )
    disconnect( selectionMgr(), SIGNAL( selectionChanged() ), this, SLOT( onSelectionDone() ) );
}

//=======================================================================
// name    : commitOperation
// Purpose : Hide dialog box (if it is exists)
//=======================================================================
void SalomeApp_Operation::commitOperation()
{
  setDialogActive( true );
  if ( dlg() )
    dlg()->hide();

  if( selectionMgr() )
    disconnect( selectionMgr(), SIGNAL( selectionChanged() ), this, SLOT( onSelectionDone() ) );
}

//=======================================================================
// name    : dlg
// Purpose : Get dialog. This method should be redefined in derived classes
//           if they use dialogs. If this function returns pointer to dialog
//           then dialog will be correctly
//           1. deactivated in suspendOperation method
//           2. activated in resumeOperation method
//           3. hidden in abortOperation and commitOperation methods
//=======================================================================
SalomeApp_Dialog* SalomeApp_Operation::dlg() const
{
  return 0;
}

//=======================================================================
// name    : activateSelection
// Purpose : Activate selection. This method should be redefined in derived
//           classes if they use own selection modes (different from default)
//=======================================================================
void SalomeApp_Operation::activateSelection()
{
}

//=======================================================================
// name    : selectionDone
// Purpose : This method called when selection is changed
//=======================================================================
void SalomeApp_Operation::selectionDone()
{
}

//=======================================================================
// name    : activeOperation
// Purpose : Get active operation
//=======================================================================
SUIT_Operation* SalomeApp_Operation::activeOperation() const
{
  return study() != 0 ? study()->activeOperation() : 0;
}
//=======================================================================
// name    : selectionMgr
// Purpose : Get selection manager
//=======================================================================
SalomeApp_SelectionMgr* SalomeApp_Operation::selectionMgr() const
{
  SUIT_Application* app = application();
  if ( app != 0 && app->inherits( "SalomeApp_Application" ) )
    return ( (SalomeApp_Application*)app )->selectionMgr();
  else
    return 0;
}

//=======================================================================
// name    : onSelectionDone
// Purpose : Call selectionDone() method if operator is an active one
//=======================================================================
void SalomeApp_Operation::onSelectionDone()
{
  if ( isActive() )
    selectionDone();
}

//=======================================================================
// name    : eventFilter
// Purpose : Block mouse and key events if operator is not active one
//=======================================================================
bool SalomeApp_Operation::eventFilter( QObject* obj, QEvent* e )
{
  if( e )
  {
//    if( isAutoResumed() &&
//        ( e->type()==QEvent::Enter  ||
//          e->type()==QEvent::WindowActivate ||
//          e->type()==QEvent::MouseButtonPress ||
//          e->type()==QEvent::MouseButtonDblClick ) )
//      resume();
      
    if( e->type()==QEvent::MouseButtonRelease ||
        e->type()==QEvent::MouseButtonDblClick ||
        e->type()==QEvent::MouseMove ||
        e->type()==QEvent::KeyPress ||
        e->type()==QEvent::KeyRelease  )
      return true;
  }
      
  return SUIT_Operation::eventFilter( obj, e );
}

//=======================================================================
// name    : update
// Purpose : 
//=======================================================================
void SalomeApp_Operation::update( const int flags )
{
  if ( myModule != 0 )
    myModule->update( flags );
}

//=======================================================================
// name    : update
// Purpose :
//=======================================================================
void SalomeApp_Operation::setDialogActive( const bool active )
{
  if( dlg() )
  {
    if( active )
    {
      dlg()->removeEventFilter( this );
      activateSelection();
      dlg()->setActiveWindow();
    }
    else
      dlg()->installEventFilter( this );
  }
}

//=======================================================================
// name    : update
// Purpose :
//=======================================================================
bool SalomeApp_Operation::isAutoResumed() const
{
  return myIsAutoResumed;
}

//=======================================================================
// name    : update
// Purpose :
//=======================================================================
void SalomeApp_Operation::setAutoResumed( const bool on )
{
  myIsAutoResumed = on;
}
