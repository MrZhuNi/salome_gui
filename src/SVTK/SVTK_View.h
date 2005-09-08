#ifndef SVTK_VIEW_H
#define SVTK_VIEW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"
#include "SVTK_Selection.h"
#include "SALOME_InteractiveObject.hxx"

#include <vtkSmartPointer.h>

#include <qmainwindow.h>

class QtxAction;

class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;

class vtkObject;
class vtkCallbackCommand;
class vtkActorCollection;

class SUIT_ResourceMgr;
class VTKViewer_Actor;

class SVTK_Viewer;
class SVTK_Selector;

class SVTK_CubeAxesActor2D;
class VTKViewer_Trihedron;
class VTKViewer_Transform;
class VTKViewer_Actor;
class SVTK_Renderer;

class SVTK_RenderWindow;
class SVTK_InteractorStyle;
class SVTK_RenderWindowInteractor;

class SALOME_Actor;

//----------------------------------------------------------------------------
class SVTK_EXPORT SVTK_MainWindow: public QMainWindow
{
  Q_OBJECT;

public:
  SVTK_MainWindow(QWidget* theParent, 
		  const char* theName,
		  SUIT_ResourceMgr* theResourceMgr);

  virtual
  ~SVTK_MainWindow();

  //----------------------------------------------------------------------------
  void
  SetInteractor(SVTK_RenderWindowInteractor* theInteractor);

  SVTK_RenderWindowInteractor*
  GetInteractor();

  vtkRenderWindowInteractor*
  getInteractor();

  void
  Repaint();

  void
  Repaint(bool theUpdateTrihedron);

  void
  InvokeEvent(unsigned long theEvent, void* theCallData);

  //----------------------------------------------------------------------------
  SVTK_InteractorStyle* 
  GetInteractorStyle();

  void
  PushInteractorStyle(SVTK_InteractorStyle* theStyle);

  void
  PopInteractorStyle();

  //----------------------------------------------------------------------------
  SVTK_Selector* 
  GetSelector();

  void
  SetSelector(SVTK_Selector* theSelector);

  Selection_Mode
  SelectionMode();

  void 
  SetSelectionMode(Selection_Mode theMode);

  //----------------------------------------------------------------------------
  void
  SetRenderWindow(SVTK_RenderWindow *theRenderWindow);

  SVTK_RenderWindow*
  GetRenderWindow();

  vtkRenderWindow*
  getRenderWindow();

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
  AdjustTrihedrons(const bool theIsForced);

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
         FrontId, BackId, TopId, BottomId, LeftId, RightId, ResetId, ViewTrihedronId };
  typedef QMap<int, QtxAction*> TActionsMap;

  vtkSmartPointer<vtkObject> myEventDispatcher;
  TActionsMap myActionsMap;  
  QToolBar* myToolBar;

  SVTK_RenderWindowInteractor* myInteractor;
  vtkSmartPointer<SVTK_RenderWindow> myRenderWindow;
};


//----------------------------------------------------------------------------
class SVTK_EXPORT SVTK_SignalHandler : public QObject
{
  Q_OBJECT;

public:
  SVTK_SignalHandler(SVTK_MainWindow* theMainWindow);

  virtual
  ~SVTK_SignalHandler();

  SVTK_MainWindow*
  GetMainWindow();

  //----------------------------------------------------------------------------
  void
  Repaint();

  void
  Repaint(bool theUpdateTrihedron);

  //----------------------------------------------------------------------------
  SVTK_Renderer* 
  GetRenderer();

  vtkRenderer* 
  getRenderer();

  //----------------------------------------------------------------------------
 public slots:
  void onSelectionChanged();

 public:
 signals:
  void MouseMove( QMouseEvent* );
  void MouseButtonPressed( QMouseEvent* );
  void MouseButtonReleased( QMouseEvent* );
  void MouseDoubleClicked( QMouseEvent* );
  void ButtonPressed(const QMouseEvent *event);
  void ButtonReleased(const QMouseEvent *event);
  void WheelMoved( QWheelEvent* );
  void KeyPressed( QKeyEvent* );
  void KeyReleased( QKeyEvent* );
  void contextMenuRequested( QContextMenuEvent *e );
  void selectionChanged();

 protected:
  SVTK_MainWindow* myMainWindow;
};


//----------------------------------------------------------------------------
class SVTK_EXPORT SVTK_View : public SVTK_SignalHandler
{
  Q_OBJECT;

public:
  SVTK_View(SVTK_MainWindow* theMainWindow);

  virtual
  ~SVTK_View();
  
  /*  interactive object management */
  void
  highlight(const Handle(SALOME_InteractiveObject)& IObject, 
	    bool highlight, 
	    bool immediatly = true);
  void
  unHighlightAll();

  /* Selection Management */
  Handle(SALOME_InteractiveObject) 
  FindIObject(const char* Entry);

  bool
  isInViewer(const Handle(SALOME_InteractiveObject)& IObject);

  bool
  isVisible(const Handle(SALOME_InteractiveObject)& IObject);

  void
  rename(const Handle(SALOME_InteractiveObject)& IObject, 
	 const QString& newName);
  
  // Displaymode management
  int 
  GetDisplayMode();

  void
  SetDisplayMode(int);

  // Switch representation wireframe/shading
  void
  SetDisplayMode(const Handle(SALOME_InteractiveObject)& IObject, 
		 int theMode);

  // Change all actors to wireframe or surface
  void 
  ChangeRepresentationToWireframe();

  void
  ChangeRepresentationToSurface();

  // Change to wireframe or surface a list of vtkactor
  void
  ChangeRepresentationToWireframe(vtkActorCollection* theListofActors);
  void
  ChangeRepresentationToSurface(vtkActorCollection* theListofActors);

  // Transparency
  void
  SetTransparency(const Handle(SALOME_InteractiveObject)& theIObject,
		  float trans);
  float 
  GetTransparency(const Handle(SALOME_InteractiveObject)& theIObject);

  // Color
  void
  SetColor(const Handle(SALOME_InteractiveObject)& theIObject,
	   const QColor& theColor);

  QColor
  GetColor(const Handle(SALOME_InteractiveObject)& theIObject);

  // Erase Display functions
  void
  EraseAll();

  void
  DisplayAll();

  void
  RemoveAll( const bool immediatly );

  void
  Erase(const Handle(SALOME_InteractiveObject)& IObject, 
	bool immediatly = true);
  void
  Remove(const Handle(SALOME_InteractiveObject)& IObject, 
	 bool immediatly = true);

  void
  Display(const Handle(SALOME_InteractiveObject)& IObject, 
	  bool immediatly = true);

  void
  DisplayOnly(const Handle(SALOME_InteractiveObject)& IObject);

  void
  Display(SALOME_Actor* SActor, 
	  bool immediatly = true);

  void
  Erase(SALOME_Actor* SActor, 
	bool immediatly = true);

  void
  Remove(SALOME_Actor* SActor, 
	 bool updateViewer = true);

  void
  SetSelectionProp(const double& theRed = 1, 
		   const double& theGreen = 1,
		   const double& theBlue = 0, 
		   const int& theWidth = 5);

  void
  SetSelectionTolerance(const double& theTolNodes = 0.025, 
			const double& theTolCell = 0.001);

 protected:  
  int myDisplayMode;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
