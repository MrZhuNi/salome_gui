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
#ifndef PLOT3D_VIEWWINDOW_H
#define PLOT3D_VIEWWINDOW_H

#include "Plot3d.h"

#include <SVTK_ViewWindow.h>

class vtkLookupTable;
class vtkScalarBarActor;
class vtkScalarBarWidget;

class Plot3d_ColorDic;

class PLOT3D_EXPORT Plot3d_ViewWindow : public SVTK_ViewWindow
{
  Q_OBJECT

public:
  enum { FirstId = SVTK_ViewWindow::LastId,
         Mode2DId, SurfacesSettingsId, MergeScalarBarsId,
         LastId };

  enum Axis { AxisX = 0, AxisY, AxisZ };

public:
  Plot3d_ViewWindow( SUIT_Desktop* theDesktop );
  virtual ~Plot3d_ViewWindow();

  virtual void Initialize( SVTK_ViewModelBase* theModel );

  virtual void contextMenuPopup( QMenu* thePopup );

  void setMode2DNormalAxis( const int theAxis );

  bool isMode2D() const;

  void clearViewState( const bool theIs2D );

  vtkSmartPointer<vtkScalarBarActor> GetScalarBarActor() const;
  void UpdateScalarBar( const bool theIsRepaint = true );

public slots:
  void onMode2D( bool theOn );
  void onSurfacesSettings();
  void onMergeScalarBars( bool theOn );
  void onFitData();

protected:
  struct ViewState
  {
    bool IsInitialized;
    double Position[3];
    double FocalPoint[3];
    double ViewUp[3];
    double ParallelScale;
    bool IsTrihedronDisplayed;

    ViewState()
    {
      IsInitialized = false;
      ParallelScale = 0.0;
      IsTrihedronDisplayed = false;
    }
  };
  void storeViewState( ViewState& theViewState );
  bool restoreViewState( ViewState theViewState );

  virtual void createActions( SUIT_ResourceMgr* theResourceMgr );
  virtual void createToolBar();

protected:
  int myPlot3dToolBar;

  bool myMode2D;
  int myMode2DNormalAxis;
  ViewState myStored2DViewState;
  ViewState myStored3DViewState;

  Plot3d_ColorDic* myColorDic;
  vtkSmartPointer<vtkLookupTable> myLookupTable;
  vtkSmartPointer<vtkScalarBarActor> myScalarBarActor;
  vtkSmartPointer<vtkScalarBarWidget> myScalarBarWg;
  bool myToDisplayScalarBar;
};

#endif
