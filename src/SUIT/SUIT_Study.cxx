#include "SUIT_Study.h"

#include "SUIT_Desktop.h"
#include "SUIT_Operation.h"
#include "SUIT_DataObject.h"
#include "SUIT_MessageBox.h"
#include "SUIT_Application.h"

SUIT_Study::SUIT_Study( SUIT_Application* app )
: QObject(),
myApp( app ),
myIsSaved( false ),
myIsModified( false ),
myName( "" ),
myBlockChangeState( false )
{
  static int _id = 0;

  myId = ++_id;

  myRoot = new SUIT_DataObject();
  myOperations.setAutoDelete( false );

  connect( this, SIGNAL( changeOperationState( bool ) ), this, SLOT( onChangeOperationState( bool ) ) );

  myOperations.setAutoDelete( false );
}

SUIT_Study::~SUIT_Study()
{
  delete myRoot;
  myRoot = 0;
}

int SUIT_Study::id() const
{
  return myId;
}

SUIT_DataObject* SUIT_Study::root() const
{
  return myRoot;
}

SUIT_Application* SUIT_Study::application() const
{
  return myApp;
}

QString SUIT_Study::studyName() const
{
  return myName;
}

SUIT_Operation* SUIT_Study::activeOperation() const
{
  Operations::const_iterator anIt = myOperations.begin(),
                             aLast = myOperations.end();
  for( ; anIt!=aLast; anIt++ )
    if( (*anIt)->isActive() )
      return *anIt;
  
  return 0;
}

bool SUIT_Study::isSaved() const
{
  return myIsSaved;
}

bool SUIT_Study::isModified() const
{
  return myIsModified;
}

void SUIT_Study::closeDocument()
{
}

void SUIT_Study::createDocument()
{
  // Custom document initialization to be performed
  // within onNewDoc() handler can be put here
}

bool SUIT_Study::openDocument( const QString& fileName )
{
  myName = fileName;
  myIsSaved = true;
  myIsModified = false;

  return true;
}

bool SUIT_Study::saveDocumentAs( const QString& fileName )
{
  myName = fileName;
  myIsSaved = true;
  myIsModified = false;

  return true;
}

bool SUIT_Study::saveDocument()
{
  return saveDocumentAs( myName );
}

void SUIT_Study::abortAllOperations()
{
  myBlockChangeState = true;
  for( SUIT_Operation* op = myOperations.first(); op; op = myOperations.next() )
    op->abort();
  myBlockChangeState = false;
  myOperations.clear();
}

void SUIT_Study::update()
{
}

void SUIT_Study::sendChangesNotification()
{
  emit studyModified( this );
}

void SUIT_Study::setIsSaved( const bool on )
{
  myIsSaved = on;
}

void SUIT_Study::setIsModified( const bool on )
{
  myIsModified = on;
}

void SUIT_Study::setRoot( SUIT_DataObject* obj )
{
  if ( myRoot == obj )
    return;

  delete myRoot;
  myRoot = obj;
}

void SUIT_Study::setStudyName( const QString& name )
{
  myName = name;
}

bool SUIT_Study::canActivate( SUIT_Operation* op, SUIT_Operation** refusingOp ) const
{
  if( !op )
  {
    if( refusingOp )
      *refusingOp = 0;      
    return false;
  }

  if( op->isGranted() )
    return true;

  Operations::const_iterator anIt = myOperations.begin(),
                             aLast = myOperations.end();
  for( ; anIt!=aLast; anIt++ )
    if( !(*anIt)->isValid( op ) )
    {
      if( refusingOp )
        *refusingOp = *anIt;
      return false;
    }
    
  return true;
}

void SUIT_Study::connectOperation( SUIT_Operation* op, const bool conn ) const
{
  if( !op )
    return;
    
  if( conn )
  {
    connect( op, SIGNAL( started( SUIT_Operation* ) ), this, SLOT( onAddOperation( SUIT_Operation* ) ) );
    connect( op, SIGNAL( stoped( SUIT_Operation* ) ), this, SLOT( onRemoveOperation( SUIT_Operation* ) ) );
    connect( op, SIGNAL( resumed( SUIT_Operation* ) ), this, SLOT( onOperationResume( SUIT_Operation* ) ) );
  }
  else
  {
    disconnect( op, SIGNAL( started( SUIT_Operation* ) ), this, SLOT( onAddOperation( SUIT_Operation* ) ) );
    disconnect( op, SIGNAL( stoped( SUIT_Operation* ) ), this, SLOT( onRemoveOperation( SUIT_Operation* ) ) );
    disconnect( op, SIGNAL( resumed( SUIT_Operation* ) ), this, SLOT( onOperationResume( SUIT_Operation* ) ) );
  }
}

void SUIT_Study::onAddOperation( SUIT_Operation* op )
{
  myOperations.append( op );
}

void SUIT_Study::onRemoveOperation( SUIT_Operation* op )
{
  myOperations.remove( op );
}

void SUIT_Study::onOperationResume( SUIT_Operation* op )
{
  Operations::const_iterator anIt = myOperations.begin(),
                             aLast = myOperations.end();
  for( ; anIt!=aLast; anIt++ )
    if( *anIt!=op )
      (*anIt)->suspend();
}

//=======================================================================
// name    : start
// Purpose : Starts operation.
//=======================================================================
void SUIT_Study::start( SUIT_Operation* op, const bool check )
{
  if ( !op || myOperations.find( op ) >= 0 )
    return;
    
  op->setExecStatus( SUIT_Operation::Rejected );
  op->setStudy( this );

  if ( !op->isReadyToStart() )
    return;

  if ( check )
  {
    SUIT_Operation* refusingOperation = 0;
    while( canActivate( op, &refusingOperation ) && refusingOperation )
    {
      int anAnsw = SUIT_MessageBox::warn2( application()->desktop(), tr( "Operation launch" ),
                                           tr( "Previous operation is not finished and will be aborted." ),
                                           tr( "Continue" ), tr( "Cancel" ), 0, 1, 1 );
      if( anAnsw == 1 )
        return;  // user refuse to start this operation
      else
        refusingOperation->abort();
    }
  }

  if ( activeOperation() )
    activeOperation()->suspendOperation();

  op->startOperation();
  myOperations.append( op );
  op->setState( SUIT_Operation::Running );
  emit op->started( op );
}

//=======================================================================
// name    : abort
// Purpose : Aborts operation.
//=======================================================================  
void SUIT_Study::abort( SUIT_Operation* op )
{
  if ( !op || myOperations.find( op ) == -1 )
    return;
    
  op->abortOperation();
  emit op->aborted( op );
  stop( op );
}

//=======================================================================
// name    : commit
// Purpose : Commits operation
//=======================================================================
void SUIT_Study::commit( SUIT_Operation* op )
{
  if ( !op || myOperations.find( op ) == -1 )
    return;

  op->commitOperation();
  emit op->commited( op );
  stop( op );
  emit studyModified( this );
}

//=======================================================================
// name    : suspend
// Purpose : Suspends operation
//=======================================================================  
void SUIT_Study::suspend( SUIT_Operation* op )
{
  if ( !op || myOperations.find( op ) == -1 || op->state() == SUIT_Operation::Suspended )
    return;

  op->setState( SUIT_Operation::Suspended );
  op->suspendOperation();
  emit op->suspended( op );
}


//=======================================================================
// name    : resume
// Purpose : Resume operation
//=======================================================================  
void SUIT_Study::resume( SUIT_Operation* op )
{
  if ( !op || myOperations.find( op ) == -1 || op->state() == SUIT_Operation::Running )
    return;

  op->setState( SUIT_Operation::Running );
  op->resumeOperation();
  emit op->resumed( op );
}

//=======================================================================
// name    : stop
// Purpose : Stop operation. This method is called when operation is
//           aborted or commited
//=======================================================================  
void SUIT_Study::stop( SUIT_Operation* op )
{
  op->setState( SUIT_Operation::Waiting );
  myOperations.remove( op );
  if ( myOperations.count() > 0 && myOperations.getLast() )
    myOperations.getLast()->resumeOperation();
    
  emit op->stopped( op );
}










  