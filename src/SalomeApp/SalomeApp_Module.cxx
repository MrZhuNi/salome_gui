// File:      SalomeApp_Module.cxx
// Created:   10/25/2004 11:39:56 AM
// Author:    Sergey LITONIN
// Copyright (C) CEA 2004

#include "SalomeApp_Module.h"
#include "SalomeApp_DataModel.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"

#include "LightApp_Selection.h"
#include "LightApp_Operation.h"
#include "LightApp_Preferences.h"

#include "CAM_DataModel.h"

#include "OB_Browser.h"

#include <SALOME_ListIO.hxx>
#include <SALOME_ListIteratorOfListIO.hxx>
#include <SALOME_InteractiveObject.hxx>

#include <SUIT_Session.h>

#include <qstring.h>
#include <qmap.h>

/*!Constructor.*/
SalomeApp_Module::SalomeApp_Module( const QString& name )
: LightApp_Module( name )
{
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
	      new SALOME_InteractiveObject( id.latin1(), comp.latin1(), val.latin1() );
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
//#include "SalomeApp_VisualParameters.h"
void SalomeApp_Module::storeVisualParameters(int savePoint)
{
  cout << "SalomeApp_Module::storeVisualParameters" << endl;
  /*
  SalomeApp_VisualParameters vp("test", 1);
  _PTR(AttributeParameter) ap = vp.getContainer();

  vector<double> v;
  v.push_back(0);
  v.push_back(1);
  v.push_back(1);
  vp.setColor("0:1", v);
  vp.setLineWidth("0:1", 0.1);
  
  cout << "CHECK 1 ";
  vector<double> vr = ap->GetRealArray(1);
  for(int i = 0; i<vr.size(); i++) 
    cout <<  vr[i] << " ";
  cout << " CHECK 1 done"   << endl;
 
  v[0] = 1;
  v[2] = 0;

  vp.setColor("0:2", v);
  vp.setTransparency("0:2", 0.2);

  
  cout << "CHECK 2 ";
  vr = ap->GetRealArray(1);
  for(int i = 0; i<vr.size(); i++) 
    cout <<  vr[i] << " ";
  cout << " CHECK 2 done"   << endl;

  vp.setPresentationMode("0:1", 11);
  vp.setPresentationMode("0:2", 22);

  cout << "color 1"   << endl;
  v = vp.getColor("0:1");
  for(int i = 0; i<3; i++) cout << "0:1 color " << v[i] << endl;
  cout << "color 2"   << endl;
  v = vp.getColor("0:2");
  for(int i = 0; i<3; i++) cout << "0:2 color " << v[i] << endl;
  cout << "Line width " << vp.getLineWidth("0:1")  << endl;
  cout << "Display mode for 0:1 " << vp.getPresentationMode("0:1")  << endl;
  cout << "Display mode for 0:2 " << vp.getPresentationMode("0:2")  << endl;
  cout << "Transparency " <<  vp.getTransparency("0:2") << endl;
  */
}


/*!
 * \brief Virtual public
 *
 * This method is called after the study document is opened, so the module has a possibility to restore
 * visual parameters
 */
void SalomeApp_Module::restoreVisualParameters(int savePoint)
{

  cout << "SalomeApp_Module::restoreVisualParameters" << endl;
  /* 
  SalomeApp_VisualParameters vp("test", 1);
  _PTR(AttributeParameter) ap = vp.getContainer();

  cout << "CHECK ";
  vector<double> vr = ap->GetRealArray(1);
  for(int i = 0; i<vr.size(); i++) 
    cout <<  vr[i] << " ";
  cout << " CHECK  done"   << endl;

  cout << "color 1"   << endl;
  vector<double> v = vp.getColor("0:1");
  for(int i = 0; i<3; i++) cout << "0:1 color " << v[i] << endl;
  cout << "color 2"   << endl;
  v = vp.getColor("0:2");
  for(int i = 0; i<3; i++) cout << "0:2 color " << v[i] << endl;
  cout << "Line width " << vp.getLineWidth("0:1")  << endl;
  cout << "Display mode for 0:1 " << vp.getPresentationMode("0:1")  << endl;
  cout << "Display mode for 0:2 " << vp.getPresentationMode("0:2")  << endl;
  cout << "Transparency " <<  vp.getTransparency("0:2") << endl;
  */
}

