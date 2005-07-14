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
  return myOperations.count() > 0 ? myOperations.getLast() : 0;
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

/*!
 * \brief Verifies whether operation can be activated above already started ones
  * \param theOp - operation to be checked
  * \return NULL if operation can be activated, pointer to operation which denies
  * starting tested operation
*
* Verifies whether operation can be activated above already started ones. This method
* is called from SUIT_Study::start() and SUIT_Study::resume() methods.
*/
SUIT_Operation* SUIT_Study::blockingOperation( SUIT_Operation* theOp ) const
{
  if( theOp->isGranted() )
    return 0;

  Operations tmpOps( myOperations );
  SUIT_Operation* anOp = 0;
  for ( anOp = tmpOps.last(); anOp; anOp = tmpOps.prev() )
  {
    if ( anOp != 0 && anOp!= theOp && !anOp->isValid( theOp ) )
      return anOp;
  }

  return 0;
}

/*!
 * \brief Starts operation
  * \param theOp - operation to be started
  * \param toCheck - if parameters is equal TRUE then checking performed whether
  * all already started operations allow to start this operation above them (default
  * value is TRUE
  * \return TRUE if operation is started, FALSE otherwise
*
* Verifies whether theOp operation can be started above already started ones (if toCheck
* parameter is equal TRUE) and starts it
*/
bool SUIT_Study::start( SUIT_Operation* theOp, const bool toCheck )
{
  if ( !theOp || myOperations.find( theOp ) >= 0 )
    return false;

  theOp->setExecStatus( SUIT_Operation::Rejected );
  theOp->setStudy( this );

  if ( !theOp->isReadyToStart() )
    return false;

  if ( toCheck )
  {
    while( SUIT_Operation* anOp = blockingOperation( theOp ) )
    {
      int anAnsw = SUIT_MessageBox::warn2( application()->desktop(),
         tr( "OPERATION_LAUNCH" ), tr( "PREVIOUS_NOT_FINISHED" ),
         tr( "CONTINUE" ), tr( "CANCEL" ), 0, 1, 1 );

      if( anAnsw == 1 )
        return false;
      else
        anOp->abort();
    }
  }

  SUIT_Operation* anOp = activeOperation();
  if ( anOp )
  {
    activeOperation()->suspendOperation();
    anOp->setState( SUIT_Operation::Suspended );
  }

  theOp->setState( SUIT_Operation::Running );
  myOperations.append( theOp );
  emit theOp->started( theOp );
  theOp->startOperation();
  
  return true;
}

/*!
 * \brief Aborts operation
  * \param theOp - operation to be aborted
  * \return TRUE if operation is aborted successfully
*
* Verifies whether operation already started and aborts it in this case (sets execution
* status to Rejected and stops operation)
*/
bool SUIT_Study::abort( SUIT_Operation* theOp )
{
  if ( !theOp || myOperations.find( theOp ) == -1 )
    return false;

  theOp->abortOperation();
  theOp->setExecStatus( SUIT_Operation::Rejected );
  emit theOp->aborted( theOp );
  stop( theOp );
  return true;
}

/*!
 * \brief Commits operation
  * \param theOp - operation to be committed
  * \return TRUE if operation is committed successfully
*
* Verifies whether operation already started and commits it in this case (sets execution
* status to Accepted and stops operation)
*/
bool SUIT_Study::commit( SUIT_Operation* theOp )
{
  if ( !theOp || myOperations.find( theOp ) == -1 )
    return false;

  theOp->commitOperation();
  theOp->setExecStatus( SUIT_Operation::Accepted );
  emit theOp->committed( theOp );
  stop( theOp );
  emit studyModified( this );
  return true;
}

/*!
 * \brief Commits operation
  * \param theOp - operation to be committed
  * \return TRUE if operation is suspended successfully
*
* Verifies whether operation already started and suspends it in this case. Operations
* ususlly are suspended to start other one above them.
*/
bool SUIT_Study::suspend( SUIT_Operation* theOp )
{
  if ( !theOp || myOperations.find( theOp ) == -1 || theOp->state() == SUIT_Operation::Suspended )
    return false;

  theOp->setState( SUIT_Operation::Suspended );
  theOp->suspendOperation();
  emit theOp->suspended( theOp );
  return true;
}


/*!
 * \brief Resumes operation
  * \param theOp - operation to be resumed
  * \return TRUE if operation is aborted successfully
*
* Verifies whether operation already started but suspended and resumesit in this case.
*/
bool SUIT_Study::resume( SUIT_Operation* theOp )
{
  if ( !theOp || myOperations.find( theOp ) == -1 ||
       theOp->state() == SUIT_Operation::Running ||
       blockingOperation( theOp ) != 0 )
    return false;

  if ( myOperations.count() > 0 )
    suspend( myOperations.last() );

  theOp->setState( SUIT_Operation::Running );
  theOp->resumeOperation();

  // Move operation at the end of list in order to sort it in the order of activation.
  // As result active operation is a last operation of list, operation which was active
  // before currently active operation is located before it and so on
  myOperations.remove( theOp );
  myOperations.append( theOp );

  emit theOp->resumed( theOp );
  return true;
}

/*!
 * \brief Stops operation
  * \param theOp - operation to be stopped
*
* Stops operation. This private method is called from abort() and commit() ones to perform
* common actions when operation is stopped
*/
void SUIT_Study::stop( SUIT_Operation* theOp )
{
  theOp->setState( SUIT_Operation::Waiting );
  myOperations.remove( theOp );

  // get last operation which can be resumed
  SUIT_Operation* anOp, *aResultOp = 0;
  for( anOp = myOperations.last(); anOp; anOp = myOperations.prev() )
    if ( anOp && anOp != theOp && blockingOperation( anOp ) == 0 )
    {
      aResultOp = anOp;
      break;
    }

  emit theOp->stopped( theOp );
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










