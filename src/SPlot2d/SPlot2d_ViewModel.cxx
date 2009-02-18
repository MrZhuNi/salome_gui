//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
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
//  File   : SPlot2d_Viewer.cxx
//  Author : Sergey RUIN
//  Module : SUIT
//
#include "SPlot2d_ViewModel.h"

#include "SPlot2d_ViewWindow.h"

#include "SPlot2d_Prs.h"
#include "SUIT_Session.h"
#include "SUIT_Application.h"
#include "SUIT_ViewManager.h"

#include "SALOME_ListIO.hxx"

#include <QApplication>
#include <QToolBar>
#include <QToolButton>
#include <QCursor>
#include <QColorDialog>

//#include <qwt_math>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <stdlib.h>

using namespace std;

//ASL: Temporary commented in order to avoir dependency on SALOMEDS

//#include "SALOMEDSClient.hxx"
//#include "SALOMEDS_StudyManager.hxx"

// in order NOT TO link with SalomeApp, here the code returns SALOMEDS_Study.
// SalomeApp_Study::studyDS() does it as well, but -- here it is retrieved from
// SALOMEDS::StudyManager - no linkage with SalomeApp.  

/*static _PTR(Study) getStudyDS()
{
  SALOMEDSClient_Study* aStudy = NULL;
  _PTR(StudyManager) aMgr( new SALOMEDS_StudyManager() );

  // get id of SUIT_Study, if it's a SalomeApp_Study, it will return
  //    id of its underlying SALOMEDS::Study
  SUIT_Application* app = SUIT_Session::session()->activeApplication();
  if ( !app )  return _PTR(Study)(aStudy);
  SUIT_Study* stud = app->activeStudy();
  if ( !stud ) return _PTR(Study)(aStudy);
  const int id = stud->id(); // virtual method, must return SALOMEDS_Study id
  // get SALOMEDS_Study with this id from StudyMgr
  return aMgr->GetStudyByID( id );
} */               

/*!
  Constructor
*/
SPlot2d_Viewer::SPlot2d_Viewer(  bool theAutoDel )
: Plot2d_Viewer( theAutoDel )    
{
}

/*!
  Destructor
*/
SPlot2d_Viewer::~SPlot2d_Viewer()
{
}

/*!
  Renames curve if it is found
*/
void SPlot2d_Viewer::rename( const Handle(SALOME_InteractiveObject)& IObject,
			     const QString& newName, Plot2d_ViewFrame* fr ) 
{
  Plot2d_ViewFrame* aViewFrame = fr ? fr : getActiveViewFrame();
  if( !aViewFrame )
    return;

  CurveDict aCurves = aViewFrame->getCurves();
  CurveDict::Iterator it = aCurves.begin();
  for( ; it != aCurves.end(); ++it )
  {
    SPlot2d_Curve* aCurve = dynamic_cast<SPlot2d_Curve*>( it.value() );
    if( aCurve && aCurve->hasIO() && aCurve->getIO()->isSame( IObject ) )
    {
      aCurve->setVerTitle( newName );
      it.key()->setTitle( newName );
    }

    if( aCurve && aCurve->hasTableIO() && aCurve->getTableIO()->isSame( IObject ) )
      aCurve->getTableIO()->setName( newName.toLatin1() );
  }
  aViewFrame->updateTitles();
}

/*!
  Renames all copies of object in all view windows
  \param IObj - object to be renamed
  \param name - new name
*/
void SPlot2d_Viewer::renameAll( const Handle(SALOME_InteractiveObject)& IObj, const QString& name )
{
  SUIT_ViewManager* vm = getViewManager();
  if ( vm )
  {
    const QVector<SUIT_ViewWindow*>& wnds = vm->getViews();

    for ( uint i = 0; i < wnds.size(); i++ )
    {
      Plot2d_ViewWindow* pwnd = dynamic_cast<Plot2d_ViewWindow*>( wnds.at( i ) );
      rename( IObj, name, pwnd->getViewFrame() );
    }
  }
}

/*!
  Returns true if interactive object is presented in the viewer
*/
bool SPlot2d_Viewer::isInViewer( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame == NULL) return 0;

  if( getCurveByIO( IObject ) != NULL )
    return 1;
  else{
    if(!IObject.IsNull()){
      CurveDict aCurves = aViewFrame->getCurves();
      CurveDict::Iterator it = aCurves.begin();
      for( ; it != aCurves.end(); ++it ) {
        SPlot2d_Curve* aCurve = dynamic_cast<SPlot2d_Curve*>( it.value() );
	if(aCurve && aCurve->hasIO() && aCurve->getTableIO()->isSame(IObject))
	  return 1;
      }
    }
  }
  return 0;
}


/*!
  Actually this method just re-displays curves which refers to the <IObject>
*/
void SPlot2d_Viewer::Display( const Handle(SALOME_InteractiveObject)& IObject, bool update )
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame == NULL) return;

  SPlot2d_Curve* curve = getCurveByIO( IObject );
  if ( curve )
    aViewFrame->updateCurve( curve, update );
}

/*!
  Actually this method just erases all curves which don't refer to <IOBject> 
  and re-displays curve which is of <IObject>
*/
void SPlot2d_Viewer::DisplayOnly( const Handle(SALOME_InteractiveObject)& IObject )
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame == NULL) return;

  Plot2d_Curve* curve = getCurveByIO( IObject );
  CurveDict aCurves = aViewFrame->getCurves();
  CurveDict::Iterator it = aCurves.begin();
  for( ; it != aCurves.end(); ++it ) {
    if(it.value() != curve)
      aViewFrame->eraseCurve( curve );
    else
      aViewFrame->updateCurve( curve, false );
  }

  aViewFrame->Repaint();
}

/*!
  Removes from the viewer the curves which refer to <IObject>
*/
void SPlot2d_Viewer::Erase( const Handle(SALOME_InteractiveObject)& IObject, bool update )
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame == NULL) return;

  SPlot2d_Curve* curve = getCurveByIO( IObject );
  if ( curve )
    aViewFrame->eraseCurve( curve, update );

  // it can be table or container object selected
  //ASL: Temporary commented in order to avoid dependency on SALOMEDS
/*  _PTR(Study) aStudy = getStudyDS();
  _PTR(SObject) aSO = aStudy->FindObjectID(IObject->getEntry());
  if ( aSO ) {
    _PTR(ChildIterator) aIter = aStudy->NewChildIterator( aSO );
    for ( ; aIter->More(); aIter->Next() ) {
      _PTR(SObject) aChildSO = aIter->Value();
      _PTR(SObject) refSO;
      if ( aChildSO->ReferencedObject( refSO ) && refSO )
	aChildSO = refSO;
      curve = getCurveByIO( new SALOME_InteractiveObject( aChildSO->GetID().c_str(), "") );
      if ( curve )
	aViewFrame->eraseCurve( curve, update );
    }
  }
*/

}

/*!
   Removes all curves from the view
*/
void SPlot2d_Viewer::EraseAll(const bool /*forced*/) 
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame) aViewFrame->EraseAll();
}

/*!
  Redraws Viewer contents
*/
void SPlot2d_Viewer::Repaint()
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame) aViewFrame->Repaint();
}

/*!
  Display presentation
*/
void SPlot2d_Viewer::Display( const SALOME_Prs2d* prs )
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  Plot2d_Prs* aPrs = dynamic_cast<Plot2d_Prs*>(const_cast<SALOME_Prs2d*>(prs));
  if(aViewFrame && aPrs) aViewFrame->Display(aPrs);
}

/*!
  Erase presentation
*/
void SPlot2d_Viewer::Erase( const SALOME_Prs2d* prs, const bool )
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  Plot2d_Prs* aPrs = dynamic_cast<Plot2d_Prs*>(const_cast<SALOME_Prs2d*>(prs));
  if(aViewFrame && aPrs) aViewFrame->Erase(aPrs);
}
  
/*!
  Create presentation by entry
*/
SALOME_Prs* SPlot2d_Viewer::CreatePrs( const char* entry )
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame)
  {
    Plot2d_Prs* prs = aViewFrame->CreatePrs(entry);
    if( prs )
      return new SPlot2d_Prs( prs );
  }

  return NULL;
}

/*!
  Axiluary method called before displaying of objects
*/
void  SPlot2d_Viewer::BeforeDisplay( SALOME_Displayer* d )
{
  d->BeforeDisplay( this, SALOME_Plot2dViewType() );
}

/*!
  Axiluary method called after displaying of objects
*/
void  SPlot2d_Viewer::AfterDisplay( SALOME_Displayer* d )
{
  d->AfterDisplay( this, SALOME_Plot2dViewType() );
}

/*!
  Returns true if interactive object is presented in the viewer and displayed
*/
bool SPlot2d_Viewer::isVisible( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame == NULL) return false;

  SPlot2d_Curve* curve = getCurveByIO( IObject );
  return aViewFrame->isVisible( curve );
}

/*!
  Return interactive obeject if is presented in the viewer
*/
Handle(SALOME_InteractiveObject) SPlot2d_Viewer::FindIObject( const char* Entry )
{
  Handle(SALOME_InteractiveObject) anIO;
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame == NULL) return anIO;

  CurveDict aCurves = aViewFrame->getCurves();
  CurveDict::Iterator it = aCurves.begin();
  for( ; it != aCurves.end(); ++it ) {
    SPlot2d_Curve* aCurve = dynamic_cast<SPlot2d_Curve*>(it.value()); 
    if ( aCurve && aCurve->hasIO() && !strcmp( aCurve->getIO()->getEntry(), Entry ) ) {
      anIO = aCurve->getIO();
      break;
    }
  }
  return anIO;
}

/*!
  Returns an active Plot2d ViewFrame or NULL
*/
Plot2d_ViewFrame* SPlot2d_Viewer::getActiveViewFrame()
{
  SUIT_ViewManager* aViewMgr = getViewManager();
  if(aViewMgr) {
    Plot2d_ViewWindow* aViewWnd = dynamic_cast<Plot2d_ViewWindow*>(aViewMgr->getActiveView());
    if(aViewWnd)
      return aViewWnd->getViewFrame();
  }

  return NULL;
}

/*!
  \return curve by object and viewframe
  \param theIObject - object
  \param fr - viewframe
*/
SPlot2d_Curve* SPlot2d_Viewer::getCurveByIO( const Handle(SALOME_InteractiveObject)& theIObject,
					     Plot2d_ViewFrame* fr )
{
  if ( !theIObject.IsNull() ) {
    Plot2d_ViewFrame* aViewFrame = fr ? fr : getActiveViewFrame();
    if(aViewFrame) {
      CurveDict aCurves = aViewFrame->getCurves();
      CurveDict::Iterator it = aCurves.begin();
      for( ; it != aCurves.end(); ++it ) {
        SPlot2d_Curve* aCurve = dynamic_cast<SPlot2d_Curve*>( it.value() );
	if(aCurve) {
	  if ( aCurve->hasIO() && aCurve->getIO()->isSame( theIObject ) )
	    return aCurve;
	}
      }
    }
  }
  return NULL;
}

/*!
  create SPlot2d_ViewWindow
*/
SUIT_ViewWindow* SPlot2d_Viewer::createView( SUIT_Desktop* theDesktop )
{
  SPlot2d_ViewWindow* aPlot2dView = new SPlot2d_ViewWindow(theDesktop, this);
  if (getPrs())
    aPlot2dView->getViewFrame()->Display(getPrs());
  return aPlot2dView;
}

/*!
  SLOT: called when action "Legend Clicked" is activated.
  override "onLegendClicked" method from Plot2d_ViewModel.
*/
void SPlot2d_Viewer::onLegendClicked( QwtPlotItem* plotItem )
{
  Plot2d_ViewFrame* aViewFrame = getActiveViewFrame();
  if(aViewFrame == NULL) return;

  CurveDict aCurves = aViewFrame->getCurves();
  SPlot2d_Curve* aSCurve;
  CurveDict::Iterator it = aCurves.begin();
  for( ; it != aCurves.end(); ++it )
  {
    if ( it.key() == plotItem ) {
      aSCurve = dynamic_cast<SPlot2d_Curve*>( it.value() );
      break;
    }
  }
  // Highlight curve in Object Browser
  if(aSCurve && aSCurve->hasIO()) {
    QString anEntry = aSCurve->getIO()->getEntry();
    emit legendSelected( anEntry );
  }
}