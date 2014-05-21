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
#include "Plot3d_ViewWindow.h"

#include "Plot3d_Actor.h"
#include "Plot3d_FitDataDlg.h"
#include "Plot3d_SetupColorScaleDlg.h"
#include "Plot3d_SetupSurfacesDlg.h"

#include <QtxAction.h>
#include <QtxActionToolMgr.h>
#include <QtxMultiAction.h>

#include <SUIT_ResourceMgr.h>

#include <SVTK_ComboAction.h>
#include <SVTK_CubeAxesActor2D.h>
#include <SVTK_CubeAxesDlg.h>
#include <SVTK_InteractorStyle.h>
#include <SVTK_KeyFreeInteractorStyle.h>
#include <SVTK_Renderer.h>

#include <VTKViewer_Algorithm.h>

#include <QMenu>

#include <vtkAxisActor2D.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkScalarBarActor.h>

/*!
  Constructor
*/
Plot3d_ViewWindow::Plot3d_ViewWindow( SUIT_Desktop* theDesktop ):
  SVTK_ViewWindow( theDesktop ),
  myMode2D( false ),
  myMode2DNormalAxis( AxisZ )
{
}

/*!
  Destructor
*/
Plot3d_ViewWindow::~Plot3d_ViewWindow()
{
}

/*!
  To initialize #Plot3d_ViewWindow instance
*/
void Plot3d_ViewWindow::Initialize( SVTK_ViewModelBase* theModel )
{
  myPlot3dToolBar = toolMgr()->createToolBar( tr( "PLOT3D" ), -1, this );

  SVTK_ViewWindow::Initialize( theModel );
}

/*!
  Fill the context menu
  \param thePopup context menu
*/
void Plot3d_ViewWindow::contextMenuPopup( QMenu* thePopup )
{
  thePopup->addAction( tr( "MNU_PLOT3D_SURFACES_SETTINGS" ), this, SLOT( onSurfacesSettings() ) );

  if( myMode2D )
    thePopup->addAction( tr( "FIT_RANGE" ), this, SLOT( onFitData() ) );
}

/*!
  Creates all actions of Plot3d view window
*/
void Plot3d_ViewWindow::createActions( SUIT_ResourceMgr* theResourceMgr )
{
  SVTK_ViewWindow::createActions( theResourceMgr );

  QtxAction* anAction;
  QtxActionToolMgr* mgr = toolMgr();

  anAction = new QtxAction( tr("MNU_PLOT3D_MODE_2D"),
			    theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_PLOT3D_MODE_2D" ) ),
			    tr( "MNU_PLOT3D_MODE_2D" ), 0, this );
  anAction->setStatusTip( tr("DSC_PLOT3D_MODE_2D") );
  anAction->setCheckable( true );
  connect( anAction, SIGNAL( toggled( bool ) ), this, SLOT( onMode2D( bool ) ) );
  mgr->registerAction( anAction, Mode2DId );

  anAction = new QtxAction( tr( "MNU_PLOT3D_SURFACES_SETTINGS" ),
			    theResourceMgr->loadPixmap( "VTKViewer", tr( "ICON_PLOT3D_SURFACES_SETTINGS" ) ),
			    tr( "MNU_PLOT3D_SURFACES_SETTINGS" ), 0, this );
  anAction->setStatusTip( tr( "DSC_PLOT3D_SURFACES_SETTINGS" ) );
  connect( anAction, SIGNAL( activated() ), this, SLOT( onSurfacesSettings() ) );
  mgr->registerAction( anAction, SurfacesSettingsId );
}

/*!
  Creates toolbar of Plot3d view window
*/
void Plot3d_ViewWindow::createToolBar()
{
  SVTK_ViewWindow::createToolBar();

  QtxActionToolMgr* mgr = toolMgr();
  
  mgr->append( Mode2DId, myPlot3dToolBar );
  mgr->append( SurfacesSettingsId, myPlot3dToolBar );
}

/*!
  Set the normal axis for the 2D mode
*/
void Plot3d_ViewWindow::setMode2DNormalAxis( const int theAxis )
{
  myMode2DNormalAxis = theAxis;
}

/*!
  Check that 2D mode is active
  \return true if 2D mode is active
*/
bool Plot3d_ViewWindow::isMode2D() const
{
  return myMode2D;
}

/*!
  Toggle 2D mode on/off
*/
void Plot3d_ViewWindow::onMode2D( bool theOn )
{
  bool anIsModeChanged = theOn != myMode2D;
  myMode2D = theOn;

  if( getAction( ViewTrihedronId ) )
    getAction( ViewTrihedronId )->setVisible( !theOn );
  if( getAction( ViewTrihedronId ) )
    getAction( ViewTrihedronId )->setVisible( !theOn );
  if( getAction( ChangeRotationPointId ) )
    getAction( ChangeRotationPointId )->setVisible( !theOn );
  if( myViewsAction )
    myViewsAction->setVisible( !theOn );
  if( getAction( ViewTrihedronId ) )
    getAction( ViewTrihedronId )->setVisible( !theOn );

  SVTK_ComboAction* a = ::qobject_cast<SVTK_ComboAction*>( toolMgr()->action( ProjectionModeId ) );
  if( a )
    a->setVisible( !theOn );

  if( theOn )
  {
    myCubeAxesDlg->SetDimensionXEnabled( myMode2DNormalAxis != AxisX );
    myCubeAxesDlg->SetDimensionYEnabled( myMode2DNormalAxis != AxisY );
    myCubeAxesDlg->SetDimensionZEnabled( myMode2DNormalAxis != AxisZ );
    if( SVTK_CubeAxesActor2D* aCubeAxes = GetRenderer()->GetCubeAxes() )
    {
      aCubeAxes->SetIsInvertedGrid( true );
      if( vtkAxisActor2D* aXAxis = aCubeAxes->GetXAxisActor2D() )
      {
        aXAxis->SetTitleVisibility( myMode2DNormalAxis != AxisX );
        aXAxis->SetLabelVisibility( myMode2DNormalAxis != AxisX );
        aXAxis->SetTickVisibility( myMode2DNormalAxis != AxisX );
      }
      if( vtkAxisActor2D* aYAxis = aCubeAxes->GetYAxisActor2D() )
      {
        aYAxis->SetTitleVisibility( myMode2DNormalAxis != AxisY );
        aYAxis->SetLabelVisibility( myMode2DNormalAxis != AxisY );
        aYAxis->SetTickVisibility( myMode2DNormalAxis != AxisY );
      }
      if( vtkAxisActor2D* aZAxis = aCubeAxes->GetZAxisActor2D() )
      {
        aZAxis->SetTitleVisibility( myMode2DNormalAxis != AxisZ );
        aZAxis->SetLabelVisibility( myMode2DNormalAxis != AxisZ );
        aZAxis->SetTickVisibility( myMode2DNormalAxis != AxisZ );
      }
    }

    if( anIsModeChanged )
      storeViewState( myStored3DViewState );
    if( !restoreViewState( myStored2DViewState ) )
    {
      // first time the action is toggled
      GetRenderer()->SetTrihedronDisplayed( false );
      switch( myMode2DNormalAxis )
      {
        case AxisX: onFrontView(); break;
        case AxisY: onLeftView(); break;
        case AxisZ: onTopView(); break;
      }
      onFitAll();
    }

    myStandardInteractorStyle->SetIsRotationEnabled( false );
    myKeyFreeInteractorStyle->SetIsRotationEnabled( false );
  }
  else
  {
    myCubeAxesDlg->SetDimensionXEnabled( true );
    myCubeAxesDlg->SetDimensionYEnabled( true );
    myCubeAxesDlg->SetDimensionZEnabled( true );
    if( SVTK_CubeAxesActor2D* aCubeAxes = GetRenderer()->GetCubeAxes() )
    {
      aCubeAxes->SetIsInvertedGrid( false );
      if( vtkAxisActor2D* aXAxis = aCubeAxes->GetXAxisActor2D() )
      {
        aXAxis->SetTitleVisibility( 1 );
        aXAxis->SetLabelVisibility( 1 );
        aXAxis->SetTickVisibility( 1 );
      }
      if( vtkAxisActor2D* aYAxis = aCubeAxes->GetYAxisActor2D() )
      {
        aYAxis->SetTitleVisibility( 1 );
        aYAxis->SetLabelVisibility( 1 );
        aYAxis->SetTickVisibility( 1 );
      }
      if( vtkAxisActor2D* aZAxis = aCubeAxes->GetZAxisActor2D() )
      {
        aZAxis->SetTitleVisibility( 1 );
        aZAxis->SetLabelVisibility( 1 );
        aZAxis->SetTickVisibility( 1 );
      }
    }

    storeViewState( myStored2DViewState );
    restoreViewState( myStored3DViewState );

    myStandardInteractorStyle->SetIsRotationEnabled( true );
    myKeyFreeInteractorStyle->SetIsRotationEnabled( true );
  }
}

/*!
  Change the surfaces settings
*/
void Plot3d_ViewWindow::onSurfacesSettings()
{
  vtkRenderer* aRenderer = getRenderer();
  if( !aRenderer )
    return;

  QList< Plot3d_Actor* > aSurfaces;
  QStringList aTexts;
  ColorDicDataList aColorDicDataList;
  vtkActor* anActor = 0;

  VTK::ActorCollectionCopy aCopy( aRenderer->GetActors() );
  vtkActorCollection* aCollection = aCopy.GetActors();
  aCollection->InitTraversal();
  while( anActor = aCollection->GetNextActor() )
  {
    if( Plot3d_Actor* aSurface = dynamic_cast<Plot3d_Actor*>( anActor ) )
    {
      aSurfaces << aSurface;

      vtkScalarBarActor* aScalarBar = aSurface->GetScalarBarActor().GetPointer();
      Plot3d_ColorDic* aColorDic = aSurface->GetColorDic();

      if( aScalarBar && aColorDic )
      {
        aTexts << aScalarBar->GetTitle();

        ColorDicData aColorDicData;
        aColorDicData.Num = aColorDic->GetNumber();
        aColorDicData.Min = aColorDic->GetMin();
        aColorDicData.Max = aColorDic->GetMax();

        aColorDic->GetHSVRange( aColorDicData.HueMin, aColorDicData.HueMax,
                                aColorDicData.SaturationMin, aColorDicData.SaturationMax,
                                aColorDicData.ValueMin, aColorDicData.ValueMax );
        aColorDicData.ScaleMode = aColorDic->GetScaleMode();
        aColorDicData.MainSpecificColorScale = aColorDic->GetMainSpecificScale();
        aColorDicData.SpecificColorScale = aColorDic->GetSpecificScale();
        aColorDicData.TimeStep = aColorDic->GetTimeStep();
        aColorDicData.Quantity = aColorDic->GetQuantity();
        aColorDicData.ColorMode = aColorDic->GetColorMode();
        aColorDic->GetCustomColors( aColorDicData.CustomColors[0], aColorDicData.CustomColors[1] );

        aColorDicDataList << aColorDicData;
      }
    }
  }

  Plot3d_SetupSurfacesDlg aDlg( this );
  aDlg.SetParameters( aTexts, aColorDicDataList );

  if ( aDlg.exec() != QDialog::Accepted ) 
    return;

  aDlg.GetParameters( aTexts, aColorDicDataList );

  // Note: Indexes retrieved from dialog do not correspond to the real indexes of 
  // plot 3d surfaces. They correspond to the user actions. For example, if user removes 
  // first surface in dialog's table two times. Then contents of list of indexes is 
  // equal (1, 1) although first and surfaces curves must be removed.
  const QList< int >& toRemove = aDlg.GetRemovedIndexes();
  QList< int >::const_iterator aRemIter;
  for ( aRemIter = toRemove.begin(); aRemIter != toRemove.end(); ++aRemIter )
  {
    int anIndex = *aRemIter;
    if ( anIndex >= 0 && anIndex < (int)aSurfaces.count() )  
    {
      Plot3d_Actor* aSurface = aSurfaces[ anIndex ];
      aSurfaces.removeAt( anIndex );
      aSurface->RemoveFromRender( aRenderer );
      aSurface->Delete();

      aColorDicDataList.removeAt( anIndex );
    }
  }

  QMap< int, Plot3d_Actor* > anIndexToSurface;
  QList< Plot3d_Actor* >::iterator aSurfIter;
  int i;
  for ( i = 0, aSurfIter = aSurfaces.begin(); aSurfIter != aSurfaces.end(); ++aSurfIter, ++i )
    anIndexToSurface[ i ] = *aSurfIter;

  int n;
  for ( i = 0, n = aTexts.size(); i < n; i++ )
  {
    const QString& aText = aTexts[ i ];
    const ColorDicData& aColorDicData = aColorDicDataList[ i ];

    Plot3d_Actor* aSurface = anIndexToSurface[ i ];

    vtkScalarBarActor* aScalarBar = aSurface->GetScalarBarActor().GetPointer();
    Plot3d_ColorDic* aColorDic = aSurface->GetColorDic();

    if( aScalarBar && aColorDic )
    {
      aScalarBar->SetTitle( aText.toLatin1().constData() );

      aColorDic->SetNumber( aColorDicData.Num );
      aColorDic->SetMin( aColorDicData.Min );
      aColorDic->SetMax( aColorDicData.Max );

      aColorDic->SetHSVRange( aColorDicData.HueMin, aColorDicData.HueMax,
                              aColorDicData.SaturationMin, aColorDicData.SaturationMax,
                              aColorDicData.ValueMin, aColorDicData.ValueMax );
      aColorDic->SetScaleMode( aColorDicData.ScaleMode );
      if( aColorDicData.ScaleMode == Plot3d_ColorDic::Specific )
      {
        aColorDic->SetMainSpecificScale( aColorDicData.MainSpecificColorScale );
        aColorDic->SetSpecificScale( aColorDicData.SpecificColorScale );
      }
      aColorDic->SetTimeStep( aColorDicData.TimeStep );
      aColorDic->SetQuantity( aColorDicData.Quantity );
      aColorDic->SetColorMode( aColorDicData.ColorMode );
      aColorDic->SetCustomColors( aColorDicData.CustomColors[0], aColorDicData.CustomColors[1] );

      aSurface->RecomputeLookupTable();
    }
  }

  vtkFloatingPointType aGlobalBounds[6] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MIN,
                                            VTK_DOUBLE_MAX, VTK_DOUBLE_MIN,
                                            VTK_DOUBLE_MAX, VTK_DOUBLE_MIN };

  for ( i = 0, aSurfIter = aSurfaces.begin(); aSurfIter != aSurfaces.end(); ++aSurfIter, ++i )
  {
    if( Plot3d_Actor* aSurface = *aSurfIter )
    {
      vtkFloatingPointType aBounds[6];
      aSurface->GetBounds( aBounds );
      aGlobalBounds[0] = qMin( aGlobalBounds[0], aBounds[0] );
      aGlobalBounds[1] = qMax( aGlobalBounds[1], aBounds[1] );
      aGlobalBounds[2] = qMin( aGlobalBounds[2], aBounds[2] );
      aGlobalBounds[3] = qMax( aGlobalBounds[3], aBounds[3] );
      aGlobalBounds[4] = qMin( aGlobalBounds[4], aBounds[4] );
      aGlobalBounds[5] = qMax( aGlobalBounds[5], aBounds[5] );
    }
  }

  double aDX = aGlobalBounds[1] - aGlobalBounds[0];
  double aDY = aGlobalBounds[3] - aGlobalBounds[2];
  double aDZ = aGlobalBounds[5] - aGlobalBounds[4];

  double aScale[3];
  GetScale( aScale ); // take into account the current scale
  aDX = fabs( aScale[0] ) > DBL_EPSILON ? aDX / aScale[0] : aDX;
  aDY = fabs( aScale[1] ) > DBL_EPSILON ? aDY / aScale[1] : aDY;
  aDZ = fabs( aScale[2] ) > DBL_EPSILON ? aDZ / aScale[2] : aDZ;

  aScale[0] = fabs( aDX ) > DBL_EPSILON ? 1.0 / aDX : 1.0;
  aScale[1] = fabs( aDY ) > DBL_EPSILON ? 1.0 / aDY : 1.0;
  aScale[2] = fabs( aDZ ) > DBL_EPSILON ? 1.0 / aDZ : 1.0;
  SetScale( aScale );
  onFitAll();
}

/*!
  Fit 2D surfaces to the specified data range
*/
void Plot3d_ViewWindow::onFitData()
{
  // to do
}

/*!
  Store 2D/3D view state
  \param theViewState - view state to be stored
*/
void Plot3d_ViewWindow::storeViewState( ViewState& theViewState )
{
  vtkCamera* aCamera = getRenderer()->GetActiveCamera();

  theViewState.IsInitialized = true;

  aCamera->GetPosition( theViewState.Position );
  aCamera->GetFocalPoint( theViewState.FocalPoint );
  aCamera->GetViewUp( theViewState.ViewUp );
  theViewState.ParallelScale = aCamera->GetParallelScale();

  theViewState.IsTrihedronDisplayed = GetRenderer()->IsTrihedronDisplayed();
}

/*!
  Restore 2D/3D view state
  \param theViewState - view state to be restored
  \return true if the view state is initialized and can be restored
*/
bool Plot3d_ViewWindow::restoreViewState( ViewState theViewState )
{
  vtkCamera* aCamera = getRenderer()->GetActiveCamera();
  if( theViewState.IsInitialized )
  {
    GetRenderer()->SetTrihedronDisplayed( theViewState.IsTrihedronDisplayed );

    aCamera->SetPosition( theViewState.Position );
    aCamera->SetFocalPoint( theViewState.FocalPoint );
    aCamera->SetViewUp( theViewState.ViewUp );
    aCamera->SetParallelScale( theViewState.ParallelScale );
    Repaint();

    return true;
  }
  return false;
}

/*!
  Clear 2D/3D view state
  \param theIs2D flag used to indicate which state has to be cleared
*/
void Plot3d_ViewWindow::clearViewState( const bool theIs2D )
{
  if( theIs2D )
    myStored2DViewState.IsInitialized = false;
  else
    myStored3DViewState.IsInitialized = false;
}
