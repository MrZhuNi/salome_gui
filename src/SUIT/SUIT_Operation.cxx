/**
*  SALOME SalomeApp
*
*  Copyright (C) 2005  CEA/DEN, EDF R&D
*
*
*
*  File   : SUIT_Operation.h
*  Author : Unknown
*  Module : SALOME
*/

#include "SUIT_Operation.h"

#include "SUIT_Study.h"
#include "SUIT_Application.h"
#include "SUIT_MessageBox.h"
#include "SUIT_Desktop.h"

//=======================================================================
// name    : SUIT_Operation
// Purpose : Constructor
//=======================================================================
SUIT_Operation::SUIT_Operation( SUIT_Application* app )
: QObject(),
myApp( app ),
myStudy( 0 ),
myState( Waiting )
{
}

SUIT_Operation::~SUIT_Operation()
{
}

//=======================================================================
// name    : study
// Purpose : Gets operation study (i.e. study which starts this operation )
//=======================================================================
SUIT_Study* SUIT_Operation::study() const
{
  return myStudy;
}

//=======================================================================
// name    : setStudy
// Purpose : Sets operation study (i.e. study which starts this operation )
//=======================================================================
void SUIT_Operation::setStudy( SUIT_Study* theStudy )
{
  myStudy = theStudy;
}

//=======================================================================
// name    : application
// Purpose : Returns application
//=======================================================================
SUIT_Application* SUIT_Operation::application() const
{
  return myApp;
}

//=======================================================================
// name    : setApplication
// Purpose : Sets application
//=======================================================================
void SUIT_Operation::setApplication( SUIT_Application* theApp )
{
  myApp = theApp;
}

//=======================================================================
// name    : state
// Purpose : Returns state of operation (see OperationState enumeration)
//=======================================================================
SUIT_Operation::OperationState SUIT_Operation::state() const
{
  return myState;
}

//=======================================================================
// name    : setState
// Purpose : Sets state of operation (see OperationState enumeration)
//=======================================================================
void SUIT_Operation::setState( const SUIT_Operation::OperationState theState )
{
  myState = theState;
}

//=======================================================================
// name    : start
// Purpose : Verifies whether operation can be started and starts operation. This slot
//           is not virtual and cannot be redefined. Redefine startOperation method
//           to change behaviour of operation instead
//=======================================================================
void SUIT_Operation::start()
{
  if ( study() )
    study()->start( this );
  else
  {
    startOperation();
    emit started( this );
  }
}

//=======================================================================
// name    : abort
// Purpose : Aborts operation. This slot is not virtual and cannot be
//           redefined. Redefine abortOperation method to change behaviour
//           of operation instead
//=======================================================================
void SUIT_Operation::abort()
{
  if ( study() )
    study()->abort( this );
  else
  {
    abortOperation();
    myState = Waiting;
    emit aborted( this );
    emit stopped( this );
  }
}

//=======================================================================
// name    : commit
// Purpose : Commits operation. This slot is not virtual and cannot be
//           redefined. Redefine commitOperation method to change behaviour
//           of operation instead
//=======================================================================
void SUIT_Operation::commit()
{
  if ( study() )
    study()->commit( this );
  else
  {
    commitOperation();
    myState = Waiting;
    emit committed( this );
    emit stopped( this );
  }
}

//=======================================================================
// name    : resume
// Purpose : Resume operation. This slot is called when operation is
//           resumed after previous suspending. This slot is not virtual
//           and cannot be redefined. Redefine resumeOperation method
//           to change behaviour of operation instead 
//=======================================================================
void SUIT_Operation::resume()
{
  if ( study() )
    study()->resume( this );
  else
  {
    resumeOperation();
    myState = Running;
    emit resumed( this );
  }
}

//=======================================================================
// name    : suspend
// Purpose : Suspend operation. This slot is called when operation is
//           suspended (for starting other one, for example). This slot is not
//           virtual and cannot be redefined. Redefine suspendOperation
//           method to change behaviour of operation instead
//=======================================================================
void SUIT_Operation::suspend()
{
  if ( study() )
    study()->suspend( this );
  else
  {
    suspendOperation();
    myState = Suspended;
    emit suspended( this );
  }
}

//=======================================================================
// name    : isReadyToStart
// Purpose : Verify whether operator is ready to start. Default implementation
//           returns true. Redefine this method to add own verifications 
//=======================================================================
bool SUIT_Operation::isReadyToStart() const
{
  return true;
}

//=======================================================================
// name    : startOperation
// Purpose : Virtual method called when operation started (see start()
//           method for more description)
//=======================================================================
void SUIT_Operation::startOperation()
{
  emit callSlot();
  commit();
}

//=======================================================================
// name    : abortOperation
// Purpose : Virtual method called when operation aborted (see abort()
//           method for more description)
//=======================================================================
void SUIT_Operation::abortOperation()
{
}

//=======================================================================
// name    : resumeOperation
// Purpose : Virtual method called when operation resumed (see resume()
//           method for more description)
//=======================================================================
void SUIT_Operation::resumeOperation()
{
}

//=======================================================================
// name    : suspendOperation
// Purpose : Virtual method called when operation suspended (see suspend()
//           method for more description)
//=======================================================================
void SUIT_Operation::suspendOperation()
{
}

//=======================================================================
// name    : commitOperation
// Purpose : Virtual method called when operation committed (see commit()
//           method for more description)
//=======================================================================
void SUIT_Operation::commitOperation()
{
}

//=======================================================================
// name    : setSlot
// Purpose : Sets slot which is called when operation is started. There is no point in
//           using this method. It would be better to inherit own operator from base
//           one and redefine startOperation method.
//=======================================================================
bool SUIT_Operation::setSlot( const QObject* theReceiver, const char* theSlot )
{
  return connect( this, SIGNAL( callSlot() ), theReceiver, theSlot );
}

//=======================================================================
// name    : isValid
// Purpose : Returns TRUE if the given operator is valid for the current one
//           (can be started "above")
//=======================================================================
bool SUIT_Operation::isValid( SUIT_Operation* ) const
{
  return false;
}

//=======================================================================
// name    : isGranted
// Purpose : Returns TRUE if current operation must not be checked for
//           ActiveOperation->IsValid(this).  Default implementation returns FALSE,
//           so it is being checked for IsValid, but some operations may overload IsGranted()
//           In this case they will always start, no matter what operation is running
//=======================================================================
bool SUIT_Operation::isGranted() const
{
  return false;
}

//=======================================================================
// name    : isActive
// Purpose : Verify whether operation is an active one
//=======================================================================
bool SUIT_Operation::isActive() const
{
  return state()==Running;
}

//=======================================================================
// name    : start
// Purpose : Start operator above this one
//=======================================================================
void SUIT_Operation::start( SUIT_Operation* op )
{
  if ( !op )
    return;
    
  if ( study() )
    study()->start( op, false );
  else
  {
    connect( this, SIGNAL( stopped( SUIT_Operation* ) ), op, SLOT( abort() ) );
    op->start();
  }
}

//=======================================================================
// name    : setExecStatus
// Purpose : Sets myExecStatus to the given value
//=======================================================================
void SUIT_Operation::setExecStatus( const int theVal )
{
  myExecStatus = (ExecStatus)theVal;
}

//=======================================================================
// name    : execStatus
// Purpose : Gets execution status
//=======================================================================
int SUIT_Operation::execStatus() const
{
  return myExecStatus;
}








