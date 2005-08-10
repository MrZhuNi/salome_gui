//  SALOME VTKViewer : build VTK viewer into Salome desktop
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : VTKViewer_RenderWindow.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SVTK_RenderWindow_h
#define SVTK_RenderWindow_h

#include <qmainwindow.h>

#include "SVTK.h"

class VTKViewer_Trihedron;
class VTKViewer_Transform;
class SVTK_CubeAxesActor2D;

class QtxAction;

class vtkRenderer;
class vtkRenderWindow;

class SVTK_EXPORT SVTK_RenderWindow : public QMainWindow
{
  Q_OBJECT

 public:
  SVTK_RenderWindow( QWidget*, const char* );
  virtual ~SVTK_RenderWindow() ;

  QToolBar* getToolBar() { return myToolBar; }

  vtkRenderer* getRenderer() { return myRenderer;}
  vtkRenderWindow* getRenderWindow() { return myRenderWindow; }

  VTKViewer_Trihedron*  GetTrihedron() { return myTrihedron; }
  SVTK_CubeAxesActor2D* GetCubeAxes() { return myCubeAxes; }

  int  GetTrihedronSize() const;
  void SetTrihedronSize( const int );
  void AdjustTrihedrons( const bool forced );

  bool isTrihedronDisplayed();
  bool isCubeAxesDisplayed();

  void Repaint( bool theUpdateTrihedron = true );

  void setBackgroundColor( const QColor& );
  QColor backgroundColor() const;

  //apply existing transformation on adding SALOME_Actor
  void SetScale( double theScale[3] );
  void GetScale( double theScale[3] );
  //void AddActor( VTKViewer_Actor*, bool update = false );
  //void RemoveActor(VTKViewer_Actor*, bool update = false);

 public slots:
  virtual void activateZoom();
  virtual void activateWindowFit();
  virtual void activateRotation();
  virtual void activatePanning(); 
  virtual void activateGlobalPanning(); 

  void onFrontView(); 
  void onBackView(); 
  void onTopView();
  void onBottomView();
  void onRightView(); 
  void onLeftView();     

  void onResetView();     
  void onFitAll();

  void onViewTrihedron(); 
  void onViewCubeAxes();

  void onAdjustTrihedron();
  void onAdjustCubeAxes();

 protected:
  enum { DumpId, FitAllId, FitRectId, ZoomId, PanId, GlobalPanId, RotationId,
         FrontId, BackId, TopId, BottomId, LeftId, RightId, ResetId, ViewTrihedronId };
  typedef QMap<int, QtxAction*> ActionsMap;
  
  void createActions();
  void createToolBar();

  vtkRenderer* myRenderer;
  vtkRenderWindow* myRenderWindow;

  VTKViewer_Transform*  myTransform;
  VTKViewer_Trihedron*  myTrihedron;  
  int                   myTrihedronSize;
  SVTK_CubeAxesActor2D* myCubeAxes;

  QToolBar* myToolBar;
  ActionsMap myActionsMap;  
};

#endif
