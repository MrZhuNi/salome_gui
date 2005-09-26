#ifndef SVTK_MAINWINDOW_H
#define SVTK_MAINWINDOW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"
#include "SVTK_Selection.h"

#include <vtkSmartPointer.h>

#include <qmainwindow.h>

class QtxAction;

class vtkObject;
class vtkRenderer;
class vtkRenderWindow;
class vtkInteractorStyle;
class vtkRenderWindowInteractor;

class SUIT_ResourceMgr;

class SVTK_RenderWindowInteractor;
class SVTK_NonIsometricDlg;
class SVTK_CubeAxesActor2D;

class VTKViewer_Trihedron;
class VTKViewer_Transform;
class VTKViewer_Actor;

class SVTK_Renderer;
class SVTK_Selector;


//----------------------------------------------------------------------------
class SVTK_EXPORT SVTK_MainWindow: public QMainWindow
{
  Q_OBJECT;

public:
  SVTK_MainWindow(QWidget* theParent, 
		  const char* theName,
		  SUIT_ResourceMgr* theResourceMgr);

  virtual
  void
  Initialize(SVTK_RenderWindowInteractor* theInteractor);

  virtual
  ~SVTK_MainWindow();

  //----------------------------------------------------------------------------
  void
  SetInteractor(SVTK_RenderWindowInteractor* theInteractor);

  SVTK_RenderWindowInteractor*
  GetInteractor();

  vtkRenderWindowInteractor*
  getInteractor();

  vtkRenderWindow*
  getRenderWindow();

  void
  Repaint();

  void
  Repaint(bool theUpdateTrihedron);

  void
  InvokeEvent(unsigned long theEvent, void* theCallData);

  //----------------------------------------------------------------------------
  vtkInteractorStyle* 
  GetInteractorStyle();

  void
  PushInteractorStyle(vtkInteractorStyle* theStyle);

  void
  PopInteractorStyle();

  //----------------------------------------------------------------------------
  SVTK_Selector* 
  GetSelector();

  Selection_Mode
  SelectionMode();

  void 
  SetSelectionMode(Selection_Mode theMode);

  //----------------------------------------------------------------------------
  SVTK_Renderer* 
  GetRenderer();

  vtkRenderer* 
  getRenderer();

  void
  SetBackgroundColor(const QColor& theColor);

  QColor 
  BackgroundColor();

  void
  SetScale(double theScale[3]);

  void
  GetScale(double theScale[3]);

  void 
  AddActor(VTKViewer_Actor* theActor, 
	   bool theIsUpdate = false);

  void 
  RemoveActor(VTKViewer_Actor* theActor, 
	      bool theIsUpdate = false);

  int  
  GetTrihedronSize();

  void
  SetTrihedronSize(const int theSize);

  void 
  AdjustActors();

  bool
  IsTrihedronDisplayed();
 
  bool
  IsCubeAxesDisplayed();

  VTKViewer_Trihedron* 
  GetTrihedron();

  SVTK_CubeAxesActor2D*
  GetCubeAxes();

  //----------------------------------------------------------------------------
  QToolBar* getToolBar();

 public slots:
  void activateZoom();
  void activateWindowFit();
  void activateRotation();
  void activatePanning(); 
  void activateGlobalPanning(); 

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

  void onNonIsometric(bool theIsActivate);

  void onAdjustTrihedron();
  void onAdjustCubeAxes();

  void onDumpView();

 public:
  QImage dumpView();

 protected:  
  void
  createActions(SUIT_ResourceMgr* theResourceMgr);

  void
  createToolBar();

  void
  SetEventDispatcher(vtkObject* theDispatcher);

  enum { DumpId, FitAllId, FitRectId, ZoomId, PanId, GlobalPanId, RotationId,
         FrontId, BackId, TopId, BottomId, LeftId, RightId, ResetId, 
	 ViewTrihedronId, NonIsometric};
  typedef QMap<int, QtxAction*> TActionsMap;

  SVTK_NonIsometricDlg* myNonIsometricDlg;
  vtkSmartPointer<vtkObject> myEventDispatcher;
  TActionsMap myActionsMap;  
  QToolBar* myToolBar;

  SVTK_RenderWindowInteractor* myInteractor;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
