// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// File   : LightApp_ModuleAction.cxx
// Author : Sergey TELKOV, Vadim SANDLER
//

#include "LightApp_ModuleAction.h"

#include <QtxComboBox.h>

/*!
  \class LightApp_ModuleAction
  \brief An action, representing the list of modules to be inserted to the
  toolbar.

  This action is represented in the toolbar as combo box and a set of buttons 
  for each module. In addition, the combo box contains an item, corresponding
  to the "neutral point" of the application (where there is no active module).
  
  The action can be constructed with up to two parameters, defining the text
  and icon to be displayed for the "neutral point".

  Only one module can be active at the moment. It can be set programmatically 
  with setActiveModule() function. Use this method with empty string to turn
  to the "neutral point". To get active module, use activeModule() function.

  When user activates/deactivates any module, the signal moduleActivated() 
  is emitted.
*/

/*!
  \brief Constructor

  Creates an module action with "neutral point" item described by \a text.

  \param text "neutral point" item's text
  \param parent parent object
*/
LightApp_ModuleAction::LightApp_ModuleAction( const QString& text, QObject* parent )
: QtxActionSet( parent )
{
  setText( text );
  init();
}

/*!
  \brief Constructor

  Creates an module action with "neutral point" item described by \a text and \a ico.

  \param text "neutral point" item's text
  \param ico "neutral point" item's icon
  \param parent parent object
*/
LightApp_ModuleAction::LightApp_ModuleAction( const QString& text, const QIcon& ico, QObject* parent )
: QtxActionSet( parent )
{
  setText( text );
  setIcon( ico );
  init();
}

/*!
  \brief Destructor
*/
LightApp_ModuleAction::~LightApp_ModuleAction()
{
}

/*!
  \brief Get list of modules.
  \return modules names list
*/
QStringList LightApp_ModuleAction::modules() const
{
  QStringList lst;
  QList<QAction*> alist = actions();
  for ( QList<QAction*>::const_iterator it = alist.begin(); it != alist.end(); ++it )
    lst.append( (*it)->text() );
  return lst;
}

/*!
  \brief Get module icon
  \param name module name
  \return module icon
*/
QIcon LightApp_ModuleAction::moduleIcon( const QString& name ) const
{
  QAction* a = moduleAction( name );
  return a ? a->icon() : QIcon();
}

/*!
  \brief Set module icon
  \param name module name
  \param ico new module icon
*/
void LightApp_ModuleAction::setModuleIcon( const QString& name, const QIcon& ico )
{
  QAction* a = moduleAction( name );
  if ( !a )
    return;

  a->setIcon( ico );
  update();
}

/*!
  \brief Add module into the list.
  \param name module name
  \param ico module icon
  \param idx position in the module list (if -1, the module is added to the end of list)
*/
void LightApp_ModuleAction::insertModule( const QString& name, const QIcon& ico,
                                          const int idx )
{
  QtxAction* a = new QtxAction( name, ico, name, 0, this, true );
  a->setStatusTip( tr( "Activate/deactivate %1 module" ).arg( name ) );

  int id = insertAction( a, -1, idx );
  update();
}

/*!
  \brief Remove module from the list.
  \param name module name
*/
void LightApp_ModuleAction::removeModule( const QString& name )
{
  int id = moduleId( name );
  if ( id == -1 )
    return;

  removeAction( id );
  update();
}

/*!
  \brief Get active module.

  If there is no active module ("neutral point"), then the null string 
  is returned.

  \return active module name
*/
QString LightApp_ModuleAction::activeModule() const
{
  QAction* a = active();
  return a ? a->text() : QString();
}

/*!
  \brief Set active module.

  To turn to the "neutral point" (no active module), pass empty string.

  \param name new active module name
*/
void LightApp_ModuleAction::setActiveModule( const QString& name )
{
  int id = moduleId( name );
  if ( name.isEmpty() || id != -1 )
    activate( id, false );
}

/*!
  \brief Called when module button is triggered.
  \internal
  \param id module identifier
*/
void LightApp_ModuleAction::onTriggered( int id )
{
  activate( id );
}

/*!
  \brief Activate a module item.
  \internal
  \param id module identifier
  \param fromAction \c true if function is called from the module button
*/
void LightApp_ModuleAction::activate( int id, bool fromAction )
{
  bool checked = false;

  QList<QAction*> alist = actions();
  for ( QList<QAction*>::const_iterator it = alist.begin(); it != alist.end(); ++it )
  {
    if ( actionId( *it ) != id ) {
      (*it)->setChecked( false );
    }
    else {
      if ( !fromAction )
        (*it)->setChecked( true );
      checked = (*it)->isChecked();
    }
  }
  
  QList<QWidget*> widgets = createdWidgets();
  for ( QList<QWidget*>::const_iterator wit = widgets.begin(); wit != widgets.end(); ++wit )
  {
    QtxComboBox* cb = ::qobject_cast<QtxComboBox*>( *wit );
    if ( cb ) {
      bool blocked = cb->signalsBlocked();
      cb->blockSignals( true );
      cb->setCurrentId( checked ? id : -1 );
      cb->blockSignals( blocked );
    }
  }

  emit moduleActivated( activeModule() );
}

/*!
  \brief Called when combo box item is activated.
  \param id module identifier
*/
void LightApp_ModuleAction::onComboActivated( int id )
{
  QList<QAction*> alist = actions();
  for ( QList<QAction*>::const_iterator it = alist.begin(); it != alist.end(); ++it )
  {
    (*it)->setChecked( actionId( *it ) == id );
  }
  
  emit moduleActivated( activeModule() );
}

/*!
  \brief Create combo box widget by request from tool bar.
  \param parent parent widget (should be QToolBar or its successor)
  \return new combo box widget
*/
QWidget* LightApp_ModuleAction::createWidget( QWidget* parent )
{
  if ( !parent->inherits( "QToolBar" ) )
    return 0;

  QtxComboBox* cb = new QtxComboBox( parent );
  cb->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
  cb->setFocusPolicy( Qt::NoFocus );

  update( cb );
  cb->blockSignals( true );
  cb->setCurrentId( actionId( active() ) );
  cb->blockSignals( false );

  connect( cb, SIGNAL( activatedId( int ) ), this, SLOT( onComboActivated( int ) ) );

  return cb;
}

/*!
  \brief Return \c false if the action contains custom widgets.
  \return \c false (action provides combo box as custom widget)
*/
bool LightApp_ModuleAction::isEmptyAction() const
{
  return false;
}

/*!
  \brief Get an action corresponding to the active module.
  \internal
  \return active module action or 0 if there is no active module
*/
QAction* LightApp_ModuleAction::active() const
{
  QAction* a = 0;
  QList<QAction*> alist = actions();
  for ( QList<QAction*>::const_iterator it = alist.begin(); it != alist.end() && !a; ++it )
  {
    if ( (*it)->isChecked() )
      a = *it;
  }
  return a;
}

/*!
  \brief Update an action.
  \internal
*/
void LightApp_ModuleAction::update()
{
  QList<QWidget*> lst = createdWidgets();
  for ( QList<QWidget*>::const_iterator itr = lst.begin(); itr != lst.end(); ++itr )
    update( ::qobject_cast<QtxComboBox*>( *itr ) );
}

/*!
  \brief Update combo box.
  \internal
  \param cb combo box
*/
void LightApp_ModuleAction::update( QtxComboBox* cb )
{
  if ( !cb )
    return;

  int curId = cb->currentId();

  QList<QAction*> alist = actions();
  cb->clear();

  cb->addItem( icon(), text() );
  cb->setId( 0, -1 );

  for ( QList<QAction*>::const_iterator it = alist.begin(); it != alist.end(); ++it )
  {
    QAction* a = *it;
    int id = actionId( a );
    cb->addItem( a->icon(), a->text() );
    cb->setId( cb->count() - 1, id );
  }

  cb->setCurrentId( curId );
}

/*!
  \brief Initialize an action,
  \internal
*/
void LightApp_ModuleAction::init()
{
  setVisible( true );
  connect( this, SIGNAL( triggered( int ) ), this, SLOT( onTriggered( int ) ) );
}

/*!
  \brief Get action corresponding to the specified module.
  \internal
  \param name module name
  \return module action or 0 if \a name is invalid
*/
QAction* LightApp_ModuleAction::moduleAction( const QString& name ) const
{
  QAction* a = 0;
  QList<QAction*> alist = actions();
  for ( QList<QAction*>::const_iterator it = alist.begin(); it != alist.end() && !a; ++it )
  {
    if ( (*it)->text() == name )
      a = *it;
  }
  return a;
}

/*!
  \brief Get module action identifier.
  \internal
  \param name module name
  \return module action ID or 0 if \a name is invalid
*/
int LightApp_ModuleAction::moduleId( const QString& name ) const
{
  int id = -1;
  QList<QAction*> alist = actions();
  for ( QList<QAction*>::const_iterator it = alist.begin(); it != alist.end() && id == -1; ++it )
  {
    if ( (*it)->text() == name )
      id = actionId( *it );
  }
  return id;
}

/*!
  \fn void LightApp_ModuleAction::moduleActivated( const QString& name );
  \brief Emitted when the module is activated
  \param name module name (empty string for neutral point)
*/
