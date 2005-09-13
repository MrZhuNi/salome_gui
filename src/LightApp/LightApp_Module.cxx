// File:      LightApp_Module.cxx
// Created:   6/20/2005 16:30:56 AM
// Author:    OCC team
// Copyright (C) CEA 2005

#include "LightApp_Module.h"

#include "CAM_Application.h"

#include "LightApp_Application.h"
#include "LightApp_DataModel.h"
#include "LightApp_Study.h"
#include "LightApp_Preferences.h"
#include "LightApp_Selection.h"

#include <SUIT_Study.h>
#include <SUIT_DataObject.h>
#include <SUIT_ResourceMgr.h>

#include <OB_Browser.h>

#include <QtxPopupMgr.h>

#include <qvariant.h>
#include <qstring.h>
#include <qstringlist.h>

/*!Constructor.*/
LightApp_Module::LightApp_Module( const QString& name )
: CAM_Module( name ),
myPopupMgr( 0 )
{
}

/*!Destructor.*/
LightApp_Module::~LightApp_Module()
{
}

/*!Initialize module.*/
void LightApp_Module::initialize( CAM_Application* app )
{
  CAM_Module::initialize( app );

  SUIT_ResourceMgr* resMgr = app ? app->resourceMgr() : 0;
  if ( resMgr )
    resMgr->raiseTranslators( name() );
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::windows( QMap<int, int>& ) const
{
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::viewManagers( QStringList& ) const
{
}

/*!Context menu popup.*/
void LightApp_Module::contextMenuPopup( const QString& client, QPopupMenu* menu, QString& /*title*/ )
{
  LightApp_Selection* sel = createSelection();
  sel->init( client, getApp()->selectionMgr() );
  popupMgr()->updatePopup( menu, sel );
  delete sel;
}

/*!Update object browser.*/
void LightApp_Module::updateObjBrowser( bool updateDataModel, SUIT_DataObject* root )
{
  if( updateDataModel )
    if( CAM_DataModel* aDataModel = dataModel() )
      if( LightApp_DataModel* aModel = dynamic_cast<LightApp_DataModel*>( aDataModel ) )
        aModel->update( 0, dynamic_cast<LightApp_Study*>( getApp()->activeStudy() ) );
  getApp()->objectBrowser()->updateTree( root );
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::selectionChanged()
{
}

/*!Activate module.*/
bool LightApp_Module::activateModule( SUIT_Study* study )
{
  bool res = CAM_Module::activateModule( study );

  if ( res && application() && application()->resourceMgr() )
    application()->resourceMgr()->raiseTranslators( name() );

  return res;
}

/*!Deactivate module.*/
bool LightApp_Module::deactivateModule( SUIT_Study* )
{
  return true;
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::MenuItem()
{
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::createPreferences()
{
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::preferencesChanged( const QString&, const QString& )
{
}

/*!Gets application.*/
LightApp_Application* LightApp_Module::getApp() const
{
  return (LightApp_Application*)application();
}

/*!Create new instance of data model and return it.*/
CAM_DataModel* LightApp_Module::createDataModel()
{
  return new LightApp_DataModel(this);
}

/*!Create and return instance of LightApp_Selection.*/
LightApp_Selection* LightApp_Module::createSelection() const
{
  return new LightApp_Selection();
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::onModelOpened()
{
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::onModelSaved()
{
}

/*!NOT IMPLEMENTED*/
void LightApp_Module::onModelClosed()
{
}

/*!Gets popup manager.(create if not exist)*/
QtxPopupMgr* LightApp_Module::popupMgr()
{
  if ( !myPopupMgr )
    myPopupMgr = new QtxPopupMgr( 0, this );
  return myPopupMgr;
}

/*!Gets preferences.*/
LightApp_Preferences* LightApp_Module::preferences() const
{
  LightApp_Preferences* pref = 0;
  if ( getApp() )
    pref = getApp()->preferences();
  return pref;
}

/*!Add preference to preferences.*/
int LightApp_Module::addPreference( const QString& label )
{
  LightApp_Preferences* pref = preferences();
  if ( !pref )
    return -1;

  int catId = pref->addPreference( moduleName(), -1 );
  if ( catId == -1 )
    return -1;

  return pref->addPreference( label, catId );
}

/*!Add preference to preferences.*/
int LightApp_Module::addPreference( const QString& label, const int pId, const int type, const QString& section, const QString& param )
{
  LightApp_Preferences* pref = preferences();
  if ( !pref )
    return -1;

  return pref->addPreference( moduleName(), label, pId, type, section, param );
}

/*!Gets property of preferences.*/
QVariant LightApp_Module::preferenceProperty( const int id, const QString& prop ) const
{
  QVariant var;
  LightApp_Preferences* pref = preferences();
  if ( pref )
    var = pref->itemProperty( id, prop );
  return var;
}

/*!Set property of preferences.*/
void LightApp_Module::setPreferenceProperty( const int id, const QString& prop, const QVariant& var )
{
  LightApp_Preferences* pref = preferences();
  if ( pref )
    pref->setItemProperty( id, prop, var );
}
