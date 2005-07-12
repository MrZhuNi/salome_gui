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



#ifndef SalomeApp_SwitchOp_H
#define SalomeApp_SwitchOp_H

#include <qobject.h>

class SalomeApp_Module;
class SalomeApp_Operation;
class QEvent;
class SUIT_Study;

/*!
 * \brief This class is intended for controling switching between operation
 *
 * Several operation may be launched simultaneously. This class is intended for
 * controlling switching between such operations. This class works with operations having
 * dialogs (activation of other operations is performed by SUIT_Study). When several
 * operations is launched simultaneously corresponding dialogs are shown on the screen.
 * Only one operation from the launched ones can be active (active operation). Other
 * operations are suspended. As result only one dialog from shown ones can be active too.
 * Other dialogs are disabled. This class installs event filter on application. When mouse
 * cursor is moved above disabled dialog corresponding event is catched by this class.
 * It finds corresponding operation and verify whether operation can be resumed (see
 * SUIT_Study::canActivate( SUIT_Operation* ) method). If yes then current active
 * operation is suspended and new operation activated. Module contains this class as a
 * field. Then module is created instance of this class created too.
 */
class SalomeApp_SwitchOp : public QObject
{
  Q_OBJECT

public:

  SalomeApp_SwitchOp( SalomeApp_Module* );
  virtual ~SalomeApp_SwitchOp();

  void                      connect( SalomeApp_Operation* );

  // Redefined from base class
  bool                      eventFilter( QObject*, QEvent* );

private slots:

  void                      onOperation();

private:

  SalomeApp_Module*         module() const;
  SalomeApp_Operation*      operation( QWidget* ) const;
  SUIT_Study*               study() const;

private:

  
};

#endif






