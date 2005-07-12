// File:      SalomeApp_Module.cxx
// Created:   10/25/2004 11:39:56 AM
// Author:    Sergey LITONIN
// Copyright (C) CEA 2004

#include "SalomeApp_Module.h"

#include "SalomeApp_Study.h"
#include "SalomeApp_DataModel.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Preferences.h"
#include "SalomeApp_UpdateFlags.h"
#include "SalomeApp_Operation.h"
#include "SalomeApp_SwitchOp.h"

#include <OB_Browser.h>

#include <CAM_Study.h>

#include <SUIT_MessageBox.h>
#include <SUIT_ResourceMgr.h>

#include <QtxPopupMgr.h>

#include <SVTK_ViewWindow.h>
#include <OCCViewer_ViewWindow.h>
#include <OCCViewer_ViewPort3d.h>
#include <GLViewer_ViewFrame.h>
#include <GLViewer_ViewPort.h>
#include <Plot2d_ViewWindow.h>

SalomeApp_Module::SalomeApp_Module( const QString& name )
: CAM_Module( name ),
myPopupMgr( 0 ),
mySwitchOp( 0 )
{
}

SalomeApp_Module::~SalomeApp_Module()
{
}

void SalomeApp_Module::initialize( CAM_Application* app )
{
  CAM_Module::initialize( app );

  SUIT_ResourceMgr* resMgr = app ? app->resourceMgr() : 0;
  if ( resMgr )
    resMgr->raiseTranslators( name() );
}

bool SalomeApp_Module::activateModule( SUIT_Study* study )
{
  bool res = CAM_Module::activateModule( study );

  if ( res && application() && application()->resourceMgr() )
    application()->resourceMgr()->raiseTranslators( name() );
    
  return res;
}

bool SalomeApp_Module::deactivateModule( SUIT_Study* )
{
  return true;
}

void SalomeApp_Module::selectionChanged()
{
}

void SalomeApp_Module::MenuItem()
{
}

void SalomeApp_Module::windows( QMap<int, int>& ) const
{
}

void SalomeApp_Module::viewManagers( QStringList& ) const
{
}

void SalomeApp_Module::createPreferences()
{
}

void SalomeApp_Module::preferencesChanged( const QString&, const QString& )
{
}

SalomeApp_Application* SalomeApp_Module::getApp() const
{
  return (SalomeApp_Application*)application();
}

void SalomeApp_Module::onModelOpened()
{
}

void SalomeApp_Module::onModelSaved()
{
}

void SalomeApp_Module::onModelClosed()
{
}

QtxPopupMgr* SalomeApp_Module::popupMgr()
{
  if ( !myPopupMgr )
    myPopupMgr = new QtxPopupMgr( 0, this );
  return myPopupMgr;
}

SalomeApp_Preferences* SalomeApp_Module::preferences() const
{
  SalomeApp_Preferences* pref = 0;
  if ( getApp() )
    pref = getApp()->preferences();
  return pref;
}

CAM_DataModel* SalomeApp_Module::createDataModel()
{
  return new SalomeApp_DataModel(this);
}

void SalomeApp_Module::updateObjBrowser( bool updateDataModel, SUIT_DataObject* root )
{
  if( updateDataModel )
    if( CAM_DataModel* aDataModel = dataModel() )
      if( SalomeApp_DataModel* aModel = dynamic_cast<SalomeApp_DataModel*>( aDataModel ) )
        aModel->update( 0, dynamic_cast<SalomeApp_Study*>( getApp()->activeStudy() ) );
  getApp()->objectBrowser()->updateTree( root );
}

void SalomeApp_Module::contextMenuPopup( const QString& client, QPopupMenu* menu, QString& /*title*/ )
{
  SalomeApp_Selection* sel = createSelection();
  sel->init( client, getApp()->selectionMgr() );
  popupMgr()->updatePopup( menu, sel );
  delete sel;
}

SalomeApp_Selection* SalomeApp_Module::createSelection() const
{
  return new SalomeApp_Selection();
}

int SalomeApp_Module::addPreference( const QString& label )
{
  SalomeApp_Preferences* pref = preferences();
  if ( !pref )
    return -1;

  int catId = pref->addPreference( moduleName(), -1 );
  if ( catId == -1 )
    return -1;

  return pref->addPreference( label, catId );
}

int SalomeApp_Module::addPreference( const QString& label, const int pId, const int type,
				     const QString& section, const QString& param )
{
  SalomeApp_Preferences* pref = preferences();
  if ( !pref )
    return -1;

  return pref->addPreference( moduleName(), label, pId, type, section, param );
}

QVariant SalomeApp_Module::preferenceProperty( const int id, const QString& prop ) const
{
  QVariant var;
  SalomeApp_Preferences* pref = preferences();
  if ( pref )
    var = pref->property( id, prop );
  return var;
}

void SalomeApp_Module::setPreferenceProperty( const int id, const QString& prop, const QVariant& var )
{
  SalomeApp_Preferences* pref = preferences();
  if ( pref )
    pref->setProperty( id, prop, var );
}

void SalomeApp_Module::update( const int flags )
{
  if ( flags & UF_Model )
  {
    if( CAM_DataModel* aDataModel = dataModel() )
      if( SalomeApp_DataModel* aModel = dynamic_cast<SalomeApp_DataModel*>( aDataModel ) )
        aModel->update( 0, dynamic_cast<SalomeApp_Study*>( getApp()->activeStudy() ) );
  }
  else if ( flags & UF_ObjBrowser )
    getApp()->objectBrowser()->updateTree( 0 );
  else if ( flags & UF_Controls )
    updateControls();
  else if ( flags & UF_Viewer )
  {
    if ( SUIT_ViewManager* viewMgr = getApp()->activeViewManager() )
      if ( SUIT_ViewWindow* viewWnd = viewMgr->getActiveView() )
      {
        if ( viewWnd->inherits( "SVTK_ViewWindow" ) )
          ( (SVTK_ViewWindow*)viewWnd )->Repaint();
        else if ( viewWnd->inherits( "OCCViewer_ViewWindow" ) )
          ( (OCCViewer_ViewWindow*)viewWnd )->getViewPort()->onUpdate();
        else if ( viewWnd->inherits( "Plot2d_ViewWindow" ) )
          ( (Plot2d_ViewWindow*)viewWnd )->getViewFrame()->Repaint();
        else if ( viewWnd->inherits( "GLViewer_ViewFrame" ) )
          ( (GLViewer_ViewFrame*)viewWnd )->getViewPort()->onUpdate();
      }
  }
  
}

void SalomeApp_Module::updateControls()
{
}

void SalomeApp_Module::startOperation( const int id )
{
  SalomeApp_Operation* op = 0;
  if( myOperations.contains( id ) )
    op = myOperations[ id ];
  else
  {
    op = createOperation( id );
    if( op )
    {
      myOperations.insert( id, op );
      op->setModule( this );
      connect( op, SIGNAL( stopped( SUIT_Operation* ) ), this, SLOT( onOperationStopped( SUIT_Operation* ) ) );
      connect( op, SIGNAL( destroyed() ), this, SLOT( onOperationDestroyed() ) );
      if ( mySwitchOp == 0 )
      {
        mySwitchOp = new SalomeApp_SwitchOp( this );
        printf( "sln: new SalomeApp_SwitchOp\n" );
      }
      mySwitchOp->connect( op );
    }
  }

  if( op )
    op->start();
}

SalomeApp_Operation* SalomeApp_Module::createOperation( const int ) const
{
  return 0;
}

void SalomeApp_Module::onOperationStopped( SUIT_Operation* )
{
}

void SalomeApp_Module::onOperationDestroyed()
{
  const QObject* s = sender();
  if( s && s->inherits( "SalomeApp_Operation" ) )
  {
    const SalomeApp_Operation* op = ( SalomeApp_Operation* )s;
    MapOfOperation::const_iterator anIt = myOperations.begin(),
                                   aLast = myOperations.end();
    for( ; anIt!=aLast; anIt++ )
      if( anIt.data()==op )
      {
        myOperations.remove( anIt.key() );
        break;
      }
  }
}
