/**
*  SALOME SalomeApp
*
*  Copyright (C) 2005  CEA/DEN, EDF R&D
*
*
*
*  File   : SalomeApp_SwitchOp.h
*  Author : Sergey LITONIN
*  Module : SALOME
*/

#include "SalomeApp_SwitchOp.h"
#include "SalomeApp_Module.h"
#include "SalomeApp_Operation.h"
#include "SalomeApp_Dialog.h"
#include <CAM_Application.h>
#include <SUIT_Operation.h>
#include <SUIT_Study.h>
#include <qevent.h>
#include <qwidget.h>
#include <qptrlist.h>
#include <qapplication.h>

/*!
 * \brief Constructor
  * \param theParent - parent of object
*
* Creates instance of the object. Connects signals and slots. Install eveny filter
* on application
*/
SalomeApp_SwitchOp::SalomeApp_SwitchOp( SalomeApp_Module* theModule )
: QObject( theModule )
{
  qApp->installEventFilter( this );
}

/*!
 * \brief Destructor
*/
SalomeApp_SwitchOp::~SalomeApp_SwitchOp()
{
  
}

/*!
 * \brief SLOT. Called when number of operations changed
*
*
*/
void SalomeApp_SwitchOp::onOperation()
{
}

/*!
 * \brief Get module
*
* Get module. Module is a parent of this class
*/
SalomeApp_Module* SalomeApp_SwitchOp::module() const
{
  return ( SalomeApp_Module* )parent();
}

/*!
 * \brief Get study
 * \return Active study of application (in current realisation)
*
* Get study
*/
SUIT_Study* SalomeApp_SwitchOp::study() const
{
  return module()->application()->activeStudy();
}

/*!
 * \brief Get operation by widget
  * \param theWg - key widget to find operation
  * \return Pointer to the operations if it is found or zero 
*
* Find operation containing dialog with given widget
*/
SalomeApp_Operation* SalomeApp_SwitchOp::operation( QWidget* theWg ) const
{
  // get dialog from widget
  SalomeApp_Dialog* aDlg = 0;
  QWidget* aParent = theWg;
  while( aParent && !aParent->inherits( "SalomeApp_Dialog" ) )
    aParent = aParent->parentWidget();

  if ( aParent && aParent->inherits( "SalomeApp_Dialog" ) )
    aDlg = (SalomeApp_Dialog*)aParent;

  // try to find operation corresponding to the dialog
  if ( aDlg != 0 && study() != 0 )
  {
    QPtrListIterator<SUIT_Operation> anIter( study()->operations() );
    while( SUIT_Operation* anOp = anIter.current() )
    {
      if ( anOp->inherits( "SalomeApp_Operation" ) &&
           ((SalomeApp_Operation*)anOp)->dlg() == aDlg )
        return ((SalomeApp_Operation*)anOp);
      ++anIter;
   }
  }

  return 0;
}

/*!
 * \brief Connect signals of operation on the slots of object
  * \param theOp - operation for connection
*
* Connect signals of operation on the slots of object. This method is called by module
* when it creates operation
*/
void SalomeApp_SwitchOp::connect( SalomeApp_Operation* theOp )
{
// to do: ???
//  void              started( SUIT_Operation* );
//  void              aborted( SUIT_Operation* );
//  void              resumed( SUIT_Operation* );
//  void              committed( SUIT_Operation* );
//  void              suspended( SUIT_Operation* );
//  void              stopped( SUIT_Operation* );
}

/*!
 * \brief Event filter
  * \param theObj - object
  * \param theEv - event
*
* Event filter. Catched signals off application. If event concerns to dialog then
* corresponding operation is found and activated.
*/
bool SalomeApp_SwitchOp::eventFilter( QObject* theObj, QEvent* theEv )
{
  if ( theObj->inherits( "QWidget" ) && ( theEv->type() == QEvent::Enter ) )
  {
    SalomeApp_Operation* anOp = operation( (QWidget*)theObj );
    if ( anOp && !anOp->isActive() && anOp->isAutoResumed() )
    {
      if ( study() )
      {
        if ( study()->canActivate( anOp ) )
          study()->resume( anOp );
      }
    }
  }

  return QObject::eventFilter( theObj, theEv );
}



















