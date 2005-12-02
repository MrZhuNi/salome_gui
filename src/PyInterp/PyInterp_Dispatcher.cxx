//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2005  CEA/DEN, EDF R&D
//
//
//
//  File   : PyInterp_Dispatcher.cxx
//  Author : Sergey ANIKIN, OCC
//  Module : GUI
//  $Header$


#include <PyInterp_base.h>
#include <PyInterp_Dispatcher.h>
#include <PyInterp_Watcher.h>

#include <qapplication.h>
#include <qobject.h>

//#include <utilities.h>
using namespace std;

PyInterp_Dispatcher* PyInterp_Dispatcher::myInstance = 0;

//////////////////////////////////////////////////////////
// class : PyInterp_Request
//////////////////////////////////////////////////////////

void PyInterp_Request::process()
{
  cerr << "PyInterp_Request::process" << endl;
  safeExecute();
  cerr << "----------PyInterp_Request::process" << endl;

  myMutex.lock();
  if ( !IsSync() && getListener() && getEvent() )
    postEvent();
  myMutex.unlock();
  cerr << "---------------------------PyInterp_Request::process" << endl;
}

void PyInterp_Request::safeExecute()
{
  execute();
}

void PyInterp_Request::Destroy( PyInterp_Request* request )
{
  // Lock and unlock the mutex to avoid errors on its deletion
  request->myMutex.lock();
  request->myMutex.unlock();
  delete request;
}

QEvent* PyInterp_Request::createEvent() const
{
  return new PyInterp_Event( PyInterp_Event::NOTIFY, (PyInterp_Request*)this );
}

QEvent* PyInterp_Request::getEvent()
{
  if ( !myEvent && !IsSync() )
    myEvent = createEvent();
  return myEvent;
}

void PyInterp_Request::postEvent()
{
#if QT_VERSION >= 0x030303
//  MESSAGE("*** PyInterp_Request::postEvent(): for Qt 3.3.3")
  QApplication::postEvent( getListener(), getEvent() );
#else
//  MESSAGE("*** PyInterp_Request::postEvent(): for Qt 3.0.5")
  QThread::postEvent( getListener(), getEvent() );
#endif
}

void PyInterp_Request::setListener( QObject* o )
{
  myMutex.lock();
  myListener = o;
  myMutex.unlock();
}

void PyInterp_LockRequest::safeExecute()
{
  cerr << "PyInterp_Request::safeExecute" << endl;
  if ( getInterp() ){
    PyLockWrapper aLock = getInterp()->GetLockWrapper();
    execute();
  }
  cerr << "---------------------PyInterp_Request::safeExecute" << endl;
}

//////////////////////////////////////////////////////////
// class : PyInterp_Event
//////////////////////////////////////////////////////////

PyInterp_Event::~PyInterp_Event()
{
  PyInterp_Request::Destroy( myRequest );
  myRequest = 0;
}

//////////////////////////////////////////////////////////
// class : PyInterp_Dispatcher
//////////////////////////////////////////////////////////

PyInterp_Dispatcher* PyInterp_Dispatcher::Get()
{
  if ( !myInstance )
    myInstance = new PyInterp_Dispatcher();
  return myInstance;
}

PyInterp_Dispatcher::PyInterp_Dispatcher() 
: QThread()
{
  myWatcher = new PyInterp_Watcher();
}

PyInterp_Dispatcher::~PyInterp_Dispatcher()
{
  // Clear the request queue
  myQueueMutex.lock();

  for ( std::list<PyInterp_Request*>::iterator it = myQueue.begin(); it != myQueue.end(); ++it )
    PyInterp_Request::Destroy( *it );
  myQueue.clear();

  myQueueMutex.unlock();

  // Wait for run() to finish
  wait();

  delete myWatcher;
  myWatcher = 0;
}

bool PyInterp_Dispatcher::IsBusy() const
{
  return running();
}

void PyInterp_Dispatcher::Exec( PyInterp_Request* theRequest )
{
  cerr << "PyInterp_Dispatcher::Exec" << endl;
  if ( !theRequest )
    return;

  //if ( theRequest->IsSync() && !IsBusy() ) // synchronous processing - nothing is done if dispatcher is busy!
  if ( theRequest->IsSync() ) // synchronous processing - nothing is done if dispatcher is busy!
    processRequest( theRequest );
  else { // asynchronous processing
    cerr << "PyInterp_Dispatcher::Exec request delayed" << endl;
    myQueueMutex.lock();
    myQueue.push_back( theRequest );
    if ( theRequest->getListener() )
      QObject::connect( theRequest->getListener(), SIGNAL( destroyed( QObject* ) ), myWatcher, SLOT( onDestroyed( QObject* ) ) );
    myQueueMutex.unlock();  

    if ( !IsBusy() )
      start();
  }
  cerr << "--------------------------PyInterp_Dispatcher::Exec" << endl;
}

void PyInterp_Dispatcher::run()
{
//  MESSAGE("*** PyInterp_Dispatcher::run(): STARTED")
  PyInterp_Request* aRequest;

  // prepare for queue size check
  myQueueMutex.lock();

  while( myQueue.size() ) {
//    MESSAGE("*** PyInterp_Dispatcher::run(): next request taken from the queue")
    aRequest = myQueue.front();

    // let other threads append their requests to the end of the queue
    myQueueMutex.unlock();

    // processRequest() may delete a request, so this pointer must not be used
    // after request is processed!
    processRequest( aRequest );

    // prepare for removal of the first request in the queue
    myQueueMutex.lock();
    // IMPORTANT: the first item could have been removed by objectDestroyed() --> we have to check it
    if ( myQueue.front() == aRequest ) // It's still here --> remove it
      myQueue.pop_front();

//    MESSAGE("*** PyInterp_Dispatcher::run(): request processed")
  }

  myQueueMutex.unlock();
//  MESSAGE("*** PyInterp_Dispatcher::run(): FINISHED")
}

void PyInterp_Dispatcher::processRequest( PyInterp_Request* theRequest )
{
  theRequest->process();
}

void PyInterp_Dispatcher::objectDestroyed( const QObject* o )
{
  // prepare for modification of the queue
  myQueueMutex.lock();

  for ( std::list<RequestPtr>::iterator it = myQueue.begin(); it != myQueue.end(); ++it ){
    if ( o == (*it)->getListener() ){
      (*it)->setListener( 0 ); // to prevent event posting
      it = myQueue.erase( it );
    }
  }

  myQueueMutex.unlock();
}

