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

#ifndef SUIT_OPERATION_H
#define SUIT_OPERATION_H

#include <qobject.h>

#include "SUIT.h"

class SUIT_Study;
class SUIT_Application;

//==============================================================================
//! Base class for all operations
/*!
*  Base class for all operations. If you perform an action it is reasonable to create
*  operation intended for this. This is a base class for all operations which provides
*  mechanism for correct starting operations, starting operations above alredy strated
*  ones, commiting operations and so on. To create own operation it is reasonable to
*  inherit it from this class and reddefines virtual methods to provide own behaviour
*  Main virtual methods are
*  - virtual bool      isReadyToStart();
*  - virtual void      startOperation();
*  - virtual void      abortOperation();
*  - virtual void      commitOperation();
*  - virtual void      resumeOperation();
*  - virtual void      suspendOperation();
*/
//==============================================================================
class SUIT_EXPORT SUIT_Operation : public QObject  
{
  Q_OBJECT

public:

  /*! Enum describes state of operation */
  enum OperationState
  {
    Waiting,  /*!< Operation is not used (it is not runned or suspended) */
    Running,  /*!< Operation is started */
    Suspended /*!< Operation is started but suspended (other operation is performed above it) */
  };

  /*! Enum describes execution status of operation */
  enum ExecStatus
  {
    Rejected, /*! Operation has not parformed any action (modificetion of data model for example) */
    Accepted  /*! Operation has performed an actions and must be stopped */
  };

public:

  SUIT_Operation( SUIT_Application* );
  //!< Constructor
  
  virtual ~SUIT_Operation();
  //!< Destructor

  bool              isActive() const;
  //!< Verifies whether operation is an active one (state()==Running)
  
  OperationState    state() const;
  //!< Returns state of operation (see OperationState enumeration)
  void              setState( const OperationState );
  //!< Sets state of operation (see OperationState enumeration)
  
  SUIT_Study*       study() const;
  //!< Returns operation study (i.e. study which starts this operation )
  virtual void      setStudy( SUIT_Study* );
  //!< Sets operation study (i.e. study which starts this operation )
  
  SUIT_Application* application() const;
  //!< Returns application
  virtual void      setApplication( SUIT_Application* );
  //!< Sets application
  
  bool              setSlot( const QObject* theReceiver, const char* theSlot );
  //!< Sets slot which is called when operation is started
  /*! Sets slot which is called when operation is started. There is no point in
      using this method. It would be better to inherit own operator from base
      one and redefine startOperation method. */

  virtual bool      isValid( SUIT_Operation* ) const;
  //! Returns TRUE if the given operator is valid for (can be started "above") the current one

  virtual bool      isGranted() const;
  //!< Returns TRUE if current operation must not be checked for ActiveOperation->IsValid(this)
  /*!< Returns TRUE if current operation must not be checked for
    ActiveOperation->IsValid(this).  Default implementation returns FALSE,
    so it is being checked for IsValid, but some operations may overload IsGranted()
    In this case they will always start, no matter what operation is running. */

public slots:

  void              start();
  //!< Starts operation.
  /*!< Verifies whether operation can be started and starts operation. This slot
       is not virtual and cannot be redefined. Redefine startOperation method
       to change behaviour of operation instead */
       
  void              abort();
  //!< Aborts operation.
  /*!< This slot is not virtual and cannot be redefined. Redefine abortOperation
       method to change behaviour of operation instead */
  
  void              commit();
  //!< Commits operation.
  /*!< This slot is not virtual and cannot be redefined. Redefine commitOperation
       method to change behaviour of operation instead */

  void              suspend();
  //!< Suspend operation.
  /*!< This slot is called when operation is suspended (for starting other one, for example)
       This slot is not virtual and cannot be redefined. Redefine suspendOperation
       method to change behaviour of operation instead */

  void              resume();
  //!< Resume operation.
  /*!< This slot is called when operation is resumed after previous suspending.
       This slot is not virtual and cannot be redefined. Redefine resumeOperation
       method to change behaviour of operation instead */
  
signals:

  void              started( SUIT_Operation* );
  //!< Signal emited from start method when operation started
  void              aborted( SUIT_Operation* );
  //!< Signal emited from abort method when operation aborted
  void              resumed( SUIT_Operation* );
  //!< Signal emited from resume method when operation resumed
  void              commited( SUIT_Operation* );
  //!< Signal emited from commite method when operation commited
  void              suspended( SUIT_Operation* );
  //!< Signal emited from suspend method when operation suspended
  void              stopped( SUIT_Operation* );
  //!< Signal emited from stop method when operation stopped
  
  void              callSlot();
  //!< Signal emited from default implementation of startOperation method
  /*!< See setSlot for more description */

protected:

  virtual bool      isReadyToStart();
  //!< Verify whether operator is ready to start.
  /*!< Default implementation returns true. Redefine this method to add own verifications */
  
  virtual void      startOperation();
  //!< Virtual method called when operation started (see start() method for more description)
  virtual void      abortOperation();
  //!< Virtual method called when operation aborted (see abort() method for more description)
  virtual void      commitOperation();
  //!< Virtual method called when operation commited (see commit() method for more description)
  virtual void      suspendOperation();
  //!< Virtual method called when operation suspended (see suspend() method for more description)
  virtual void      resumeOperation();
  //!< Virtual method called when operation resumed (see resume() method for more description)
  
  void              setExecStatus( const int );
  //!< Sets myExecStatus to the given value
  int               execStatus();
  //!< Gets execution status

  void              start( SUIT_Operation* );
  //!< Start operator above this one

private:

  SUIT_Application* myApp;
  SUIT_Study*       myStudy;
  OperationState    myState;
  ExecStatus        myExecStatus;

  friend class SUIT_Study;
};

#endif
