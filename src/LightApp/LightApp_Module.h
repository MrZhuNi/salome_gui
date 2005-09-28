// File:      LightApp_Module.h
// Created:   6/20/2005 16:25:06 AM
// Author:    OCC team
// Copyright (C) CEA 2005

#ifndef LIGHTAPP_MODULE_H
#define LIGHTAPP_MODULE_H

#include "LightApp.h"
#include <CAM_Module.h>

class LightApp_Application;
class LightApp_Preferences;
class LightApp_Selection;
class LightApp_Operation;
class LightApp_SwitchOp;

class SUIT_Study;
class SUIT_DataObject;
class SUIT_Operation;
class CAM_Application;

class QtxPopupMgr;

class QString;
class QVariant;

/*
  Class : LightApp_Module
  Description : Base class for all light modules
*/

class LIGHTAPP_EXPORT LightApp_Module : public CAM_Module
{
  Q_OBJECT

public:
  LightApp_Module( const QString& );
  virtual ~LightApp_Module();

  virtual void                        initialize( CAM_Application* );
  virtual void                        windows( QMap<int, int>& ) const;
  virtual void                        viewManagers( QStringList& ) const;

  virtual void                        contextMenuPopup( const QString&, QPopupMenu*, QString& );

  virtual void                        createPreferences();

  LightApp_Application*               getApp() const;

  virtual void                        update( const int );

  virtual void                        updateObjBrowser( bool = true, SUIT_DataObject* = 0 );

  virtual void                        selectionChanged();
  virtual void                        preferencesChanged( const QString&, const QString& );

  virtual void                        studyActivated() {};

public slots:
  virtual bool                        activateModule( SUIT_Study* );
  virtual bool                        deactivateModule( SUIT_Study* );

  void                                MenuItem();

protected slots:
  virtual void                        onModelSaved();
  virtual void                        onModelOpened();
  virtual void                        onModelClosed();

  virtual void                        onOperationStopped( SUIT_Operation* );
  virtual void                        onOperationDestroyed();

protected:
  QtxPopupMgr*                        popupMgr();
  LightApp_Preferences*               preferences() const;

  virtual CAM_DataModel*              createDataModel();
  virtual LightApp_Selection*         createSelection() const;

  int                                 addPreference( const QString& label );
  int                                 addPreference( const QString& label, const int pId, const int = -1,
				                     const QString& section = QString::null,
				                     const QString& param = QString::null );
  QVariant                            preferenceProperty( const int, const QString& ) const;
  void                                setPreferenceProperty( const int, const QString&, const QVariant& );

  void                                startOperation( const int );
  virtual LightApp_Operation*         createOperation( const int ) const;

  virtual void                        updateControls();

private:
  typedef QMap<int,LightApp_Operation*> MapOfOperation;

private:
  QtxPopupMgr*          myPopupMgr;
  MapOfOperation        myOperations;
  LightApp_SwitchOp*    mySwitchOp;
};

#endif
