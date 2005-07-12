#include "SUIT_Study.h"

#include "SUIT_Desktop.h"
#include "SUIT_Operation.h"
#include "SUIT_DataObject.h"
#include "SUIT_MessageBox.h"
#include "SUIT_Application.h"

#include <qvaluevector.h>

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

//=======================================================================
// name    : canActivate
// Purpose : Verify whether operation can be activated (abort other operations
//           if necessary)
//=======================================================================
bool SUIT_Study::canActivate( SUIT_Operation* op ) const
{
  if( !op )
    return false;

  if( op->isGranted() )
    return true;

  Operations tmpOps( myOperations );
  SUIT_Operation* anOp = 0;
  for ( anOp = tmpOps.last(); anOp; anOp = tmpOps.prev() )
  {
    if ( anOp != 0 && anOp!= op && !anOp->isValid( op ) )
      return false;
  }

  return true;
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

  if ( check && !canActivate( op ) )
  {
    while( activeOperation() )
    {
      int anAnsw = SUIT_MessageBox::warn2( application()->desktop(),
         tr( "OPERATION_LAUNCH" ), tr( "PREVIOUS_NOT_FINISHED" ),
         tr( "CONTINUE" ), tr( "CANCEL" ), 0, 1, 1 );

      if( anAnsw == 1 )
        return;
      else
        activeOperation()->abort();
    }
  }
  
  SUIT_Operation* anOp = activeOperation();
  if ( anOp )
  {
    activeOperation()->suspendOperation();
    anOp->setState( SUIT_Operation::Suspended );
  }

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
  emit op->committed( op );
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
  if ( !op || myOperations.find( op ) == -1 ||
       op->state() == SUIT_Operation::Running ||
       !canActivate( op ) )
    return;

  if ( myOperations.count() > 0 )
    suspend( myOperations.last() );

  op->setState( SUIT_Operation::Running );
  op->resumeOperation();
  
  // Move operation at the end of list in order to sort it in the order of activation.
  // As result active operation is a last operation of list, operation which was active
  // before currently active operation is located before it and so on
  myOperations.remove( op );
  myOperations.append( op );
  
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

  // get last operation which can be resumed
  SUIT_Operation* anOp, *aResultOp = 0;
  for( anOp = myOperations.last(); anOp; anOp = myOperations.prev() )
    if ( anOp && anOp != op && canActivate( anOp ) )
    {
      aResultOp = anOp;
      break;
    }

  emit op->stopped( op );
  if ( aResultOp )
    resume( aResultOp );
}

/*!
 * \brief Get all started operations
  * \return List of all started operations
*/
const QPtrList<SUIT_Operation>& SUIT_Study::operations() const
{
  return myOperations;
}










