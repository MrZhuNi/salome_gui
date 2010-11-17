//  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

// File:      SalomeApp_Module.cxx
// Created:   10/25/2004 11:39:56 AM
// Author:    Sergey LITONIN
//
#include "SalomeApp_Module.h"
#include "SalomeApp_DataModel.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"
#include "SalomeApp_DataObject.h"

#include "LightApp_Selection.h"
#include "LightApp_DataObject.h"

#include "CAM_DataModel.h"

// temporary commented
//#include "OB_Browser.h"

#include <SALOME_ListIO.hxx>
#include <SALOME_ListIteratorOfListIO.hxx>
#include <SALOME_InteractiveObject.hxx>

#include <SUIT_Session.h>
#include <SUIT_DataObject.h>
#include <SUIT_DataBrowser.h>

#include <QString>

/*!Constructor.*/
SalomeApp_Module::SalomeApp_Module( const QString& name )
  : LightApp_Module( name ),
    myIsFirstActivate( true )
{
  SUIT_DataObject::connect( SIGNAL( inserted( SUIT_DataObject*, SUIT_DataObject* ) ),
                            this, SLOT( onObjectInserted( SUIT_DataObject* ) ) );
}

/*!Destructor.*/
SalomeApp_Module::~SalomeApp_Module()
{
}

/*!Gets application.*/
SalomeApp_Application* SalomeApp_Module::getApp() const
{
  return (SalomeApp_Application*)application();
}

/*!Create new instance of data model and return it.*/
CAM_DataModel* SalomeApp_Module::createDataModel()
{
  return new SalomeApp_DataModel(this);
}

/*!Create and return instance of LightApp_Selection.*/
LightApp_Selection* SalomeApp_Module::createSelection() const
{
  return LightApp_Module::createSelection();
}

/*!
  Converts objects-containers to list of objects, those are contained
  Other objects must be added without conversion
  \param source - source list of objects
  \param dest - list of converted objects
*/
void SalomeApp_Module::extractContainers( const SALOME_ListIO& source, SALOME_ListIO& dest ) const
{
  SalomeApp_Study* study = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );
  if( !study )
  {
    dest = source;
    return;
  }

  SALOME_ListIteratorOfListIO anIt( source );
  for( ; anIt.More(); anIt.Next() )
  {
    Handle( SALOME_InteractiveObject ) obj = anIt.Value();
    if( obj->hasEntry() )
    {
      _PTR(SObject) SO = study->studyDS()->FindObjectID( obj->getEntry() );
      if( SO && QString( SO->GetID().c_str() ) == SO->GetFatherComponent()->GetID().c_str() )
      { //component is selected
        _PTR(SComponent) SC( SO->GetFatherComponent() );
        _PTR(ChildIterator) anIter ( study->studyDS()->NewChildIterator( SC ) );
        anIter->InitEx( true );
        while( anIter->More() )
        {
          _PTR(SObject) valSO ( anIter->Value() );
          _PTR(SObject) refSO;
          if( !valSO->ReferencedObject( refSO ) )
          {
            QString id = valSO->GetID().c_str(),
                    comp = SC->ComponentDataType().c_str(),
                    val = valSO->GetName().c_str();

            Handle( SALOME_InteractiveObject ) new_obj =
              new SALOME_InteractiveObject( id.toLatin1(), comp.toLatin1(), val.toLatin1() );
            dest.Append( new_obj );
          }
          anIter->Next();
        }
        continue;
      }
    }
    dest.Append( obj );
  }
}

/*!
 * \brief Virtual public
 *
 * This method is called just before the study document is saved, so the module has a possibility
 * to store visual parameters in AttributeParameter attribue(s)
 */
void SalomeApp_Module::storeVisualParameters(int savePoint)
{
}

/*!
 * \brief Virtual public
 *
 * This method is called after the study document is opened, so the module has a possibility to restore
 * visual parameters
 */
void SalomeApp_Module::restoreVisualParameters(int savePoint)
{
}

/*!Returns module  data object module*/
static SalomeApp_Module* objectModule( CAM_Application* theApp,
                                       SalomeApp_DataObject* theObj )
{
  SalomeApp_Module* mod = 0;
  if ( !theApp || !theObj )
    return mod;

  mod = dynamic_cast<SalomeApp_Module*>
    (theApp->module(theApp->moduleTitle( theObj->componentDataType() )));

  return mod;
}

/*! Redefined to reset internal flags valid for study instance */
void SalomeApp_Module::studyClosed( SUIT_Study* theStudy )
{
  LightApp_Module::studyClosed( theStudy );
  myIsFirstActivate = true;
  
  LightApp_Application* app = dynamic_cast<LightApp_Application*>(application());
  if (!app)
    return;
  SUIT_DataBrowser* ob = app->objectBrowser();
  if (ob)
    disconnect( ob, SIGNAL( clicked( SUIT_DataObject*, int ) ),
                this, SLOT( onObjectClicked( SUIT_DataObject*, int ) ) );
}

/*!Activate module.*/
bool SalomeApp_Module::activateModule( SUIT_Study* theStudy )
{
  bool state = LightApp_Module::activateModule( theStudy );
  if (!myIsFirstActivate)
    return state;

  myIsFirstActivate = false;

  // update visibility state of objects
  LightApp_Application* app = dynamic_cast<LightApp_Application*>(application());
  if (!app)
    return state;
  
  SUIT_DataBrowser* ob = app->objectBrowser();
  if (!ob)
    return state;

  // connect to click on item
  connect( ob, SIGNAL( clicked( SUIT_DataObject*, int ) ),
           this, SLOT( onObjectClicked( SUIT_DataObject*, int ) ) );

  SUIT_DataObject* rootObj = ob->root();
  if( !rootObj )
    return state;
  DataObjectList listObj = rootObj->children( true );
  for ( DataObjectList::iterator itr = listObj.begin(); itr != listObj.end(); ++itr )
  {
    SalomeApp_DataObject* obj = dynamic_cast<SalomeApp_DataObject*>(*itr);
    if ( !obj || dynamic_cast<LightApp_ModuleObject*>(obj) || obj->isReference() )
      continue;
    
    SalomeApp_Module* mod = objectModule( app, obj );
    if (mod && mod == this)
      mod->initVisibilityState( obj );
  }
  return state;
}

/*!
 * \brief Virtual public slot
 *
 * This method is called after the object inserted into data view
 * This is default implementation
 */
void SalomeApp_Module::onObjectInserted( SUIT_DataObject* theObject )
{
  SalomeApp_DataObject* anObj = dynamic_cast<SalomeApp_DataObject*>( theObject );
  if (!anObj)
    return;
  
  // no visibility state for module root and reference objects
  if ( dynamic_cast<LightApp_ModuleObject*>(anObj) || anObj->isReference())
  {
    anObj->setVisibilityState( SUIT_DataObject::Unpresentable );
    return;
  }
  
  // update visibility state in native object module
  SalomeApp_Module* mod = objectModule( application(), anObj );
  if ( mod )
    mod->initVisibilityState( anObj );
}

/*!
 * \brief Virtual protected method
 *
 * This method is intended to update their visibility state
 * This is default implementation
 */
void SalomeApp_Module::initVisibilityState( SUIT_DataObject* theObject )
{
  if ( theObject )
    theObject->setVisibilityState( SUIT_DataObject::Unpresentable );
}

/*!
 * \brief Virtual public slot
 *
 * This method is called after the object inserted into data view to update their visibility state
 * This is default implementation
 */
void SalomeApp_Module::onObjectClicked( SUIT_DataObject* theObject, int theColumn )
{
  if (!isActiveModule())
    return;
  // change visibility of object
  if (!theObject || theColumn != SUIT_DataObject::VisibilityId )
    return;
  // detect action index (from LightApp level)
  int id = -1;
  if ( theObject->visibilityState() == SUIT_DataObject::Shown )
    id = myErase;
  else if ( theObject->visibilityState() == SUIT_DataObject::Hidden )
    id = myDisplay;
  if ( id != -1 )
    startOperation( id );
}
