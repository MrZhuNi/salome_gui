//  SALOME SalomeApp
//
//  Copyright (C) 2005  CEA/DEN, EDF R&D
//
//
//
//  File   : SalomeApp_Operation.h
//  Author : Sergey LITONIN
//  Module : SALOME


#ifndef SalomeApp_Operation_H
#define SalomeApp_Operation_H

#include <SUIT_Operation.h>
#include <SalomeApp_Dialog.h>

class SalomeApp_Module;
class SalomeApp_Application;
class SalomeApp_Operation;
class SalomeApp_SelectionMgr;
class SUIT_Desktop;

/*
  Class       : SalomeApp_Operation
  Description : Base class for all operations
*/

class SalomeApp_Operation : public SUIT_Operation
{
  Q_OBJECT

public:

  SalomeApp_Operation( SalomeApp_Application* theApp );
  virtual ~SalomeApp_Operation();

  void                    setModule( SalomeApp_Module* );
  // Set module

protected:

  // Important virtual methods (should be redefined in the derived classes)

  virtual SalomeApp_Dialog* dlg() const;
  // Get dialog. This method should be redefined in derived classes
  // if they use dialogs. If this function returns pointer to dialog
  // then dialog will be correctly
  //  1. deactivated in suspendOperation method
  //  2. activated in resumeOperation method
  //  3. hidden in abortOperation and commitOperation methods

  virtual void              activateSelection();
  // Activate selection. This method should be redefined in derived
  // classes if they use own selection modes (different from default)
  
  virtual void              selectionDone();
  // This method called when selection is changed

  // Methods redefined from base class

  virtual void              startOperation();
  // Connect signal of selection manager to onSelectionDone() slot

  virtual void              suspendOperation();
  // Disable dialog for mouse and key events
  
  virtual void              resumeOperation();
  // Enable dialog if it was deactivated (in suspend method) and activate selection
  
  virtual void              abortOperation();
  // Hide dialog box (if it is exists)
  
  virtual void              commitOperation();
  // Hide dialog box (if it is exists)

  bool                      isActive() const;
  // Verify whether operator is active one
  
  SalomeApp_Module*         module() const;
  // Get module
  
  SUIT_Desktop*             desktop() const;
  // Get desktop
  
  SUIT_Operation*           activeOperation() const;
  // Get active operation
  
  SalomeApp_SelectionMgr*   selectionMgr() const;
  // Get selection manager

  void                      update( const int );
  // Call update() method of module ( see SalomeApp_Module for description )
  

  
 
  virtual void              selected( QStringList&, SalomeApp_Dialog::TypesList&, QStringList& ) const;
  // Get names, types and entries of selected objects

  virtual int               type( const QString& ) const;
  // Find type by entry: default implementation is to return -1
  // Please redefine it if you want function "selected" to work correctly
    
private slots:

  virtual void              onSelectionDone();
  // Call selectionDone() method if operator is an active one

private:

  bool                      eventFilter( QObject*, QEvent* );
  // Block mouse and key events if operator is not active one
 
private:

  SalomeApp_Module*         myModule;
};

#endif






