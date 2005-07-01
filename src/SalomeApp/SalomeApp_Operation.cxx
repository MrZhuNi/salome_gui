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
#include <SalomeApp_DataOwner.h>
#include <SalomeApp_SelectionMgr.h>

#include <SUIT_Desktop.h>


/*
  Class       : SalomeApp_Operation
  Description : Base class for all operations
*/

//=======================================================================
// name    : SalomeApp_Operation
// Purpose : Constructor
//=======================================================================
SalomeApp_Operation::SalomeApp_Operation( SalomeApp_Application* theApp )
: SUIT_Operation( theApp ),
  myModule( 0 )
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

void SalomeApp_Operation::setModule( SalomeApp_Module* theModule )
{
  myModule = theModule;
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
  if ( dlg() )
    dlg()->removeEventFilter( this );
  activateSelection();
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
  if ( dlg() )
    dlg()->installEventFilter( this );
}

//=======================================================================
// name    : abortOperation
// Purpose : Hide dialog box (if it is exists)
//=======================================================================
void SalomeApp_Operation::abortOperation()
{
  if ( dlg() )
  {
    dlg()->removeEventFilter( this );
    dlg()->hide();
  }

  if( selectionMgr() )
    disconnect( selectionMgr(), SIGNAL( selectionChanged() ), this, SLOT( onSelectionDone() ) );
}

//=======================================================================
// name    : commitOperation
// Purpose : Hide dialog box (if it is exists)
//=======================================================================
void SalomeApp_Operation::commitOperation()
{
  if ( dlg() )
  {
    dlg()->removeEventFilter( this );
    dlg()->hide();
  }

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
// name    : isActive
// Purpose : Verify whether operator is active one
//=======================================================================
bool SalomeApp_Operation::isActive() const
{
  return activeOperation() == this;
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
  // to do
  return SUIT_Operation::eventFilter( obj, e );
}

//=======================================================================
// name    : type
// Purpose : Find type by entry
//=======================================================================
int SalomeApp_Operation::type( const QString& ) const
{
  return -1;
}

//=======================================================================
// name    : selected
// Purpose : Get names, types and entries of selected objects
//=======================================================================
void SalomeApp_Operation::selected( QStringList& names, SalomeApp_Dialog::TypesList& types, QStringList& entries ) const
{
  SUIT_DataOwnerPtrList list; selectionMgr()->selected( list );
  SUIT_DataOwnerPtrList::const_iterator anIt = list.begin(),
                                        aLast = list.end();
  for( ; anIt!=aLast; anIt++ )
  {
    SalomeApp_DataOwner* owner = dynamic_cast<SalomeApp_DataOwner*>( (*anIt).operator->() );
    if( owner )
    {
      QString entry = owner->entry();
      entries.append( entry );
      types.append( type( entry ) );
      names.append( owner->IO()->getName() );
    }
  }
}
//=======================================================================
// name    : eventFilter
// Purpose : Block mouse and key events if operator is not active one
//=======================================================================
void SalomeApp_Operation::update( const int flags )
{
  if ( myModule != 0 )
    myModule->update( flags );
}
