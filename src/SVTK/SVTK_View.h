#ifndef SVTK_VIEW_H
#define SVTK_VIEW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"

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
class SVTK_EXPORT SVTK_RendererHolder: public QMainWindow
{
  Q_OBJECT;

public:
  SVTK_RendererHolder(QWidget* theParent, 
		      const char* theName);

  virtual
  ~SVTK_RendererHolder();

  vtkRenderer* GetRenderer();

  virtual void Repaint() = 0;

  void setBackgroundColor( const QColor& );
  QColor backgroundColor() const;

  //apply existing transformation on adding SALOME_Actor
  void SetScale( double theScale[3] );
  void GetScale( double theScale[3] );

  void AddActor(VTKViewer_Actor* theActor, bool theIsUpdate = false);
  void RemoveActor(VTKViewer_Actor* theActor, bool theIsUpdate = false);

  int  GetTrihedronSize() const;
  void SetTrihedronSize( const int );
  void AdjustTrihedrons( const bool forced );

  bool isTrihedronDisplayed();
  bool isCubeAxesDisplayed();

  VTKViewer_Trihedron* GetTrihedron();
  SVTK_CubeAxesActor2D* GetCubeAxes();

 protected:  
  vtkSmartPointer<SVTK_Renderer> myRenderer;
};


//----------------------------------------------------------------------------
class SVTK_EXPORT SVTK_ActionHolder: public SVTK_RendererHolder
{
  Q_OBJECT;

public:
  SVTK_ActionHolder(QWidget* theParent, 
		    const char* theName,
		    SUIT_ResourceMgr* theResourceMgr);

  virtual
  ~SVTK_ActionHolder();

  void
  SetEventDispatcher(vtkObject* theDispatcher);

  QToolBar* getToolBar();

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

  void onDumpView();

 public:
  QImage dumpView();

 protected:  
  void
  createActions(SUIT_ResourceMgr* theResourceMgr);

  void
  createToolBar();

  enum { DumpId, FitAllId, FitRectId, ZoomId, PanId, GlobalPanId, RotationId,
         FrontId, BackId, TopId, BottomId, LeftId, RightId, ResetId, ViewTrihedronId };
  typedef QMap<int, QtxAction*> TActionsMap;

  vtkSmartPointer<vtkObject> myEventDispatcher;
  TActionsMap myActionsMap;  
  QToolBar* myToolBar;
};


//----------------------------------------------------------------------------
class SVTK_EXPORT SVTK_InteractorHolder : public SVTK_ActionHolder
{
  Q_OBJECT;

public:
  SVTK_InteractorHolder(QWidget* theParent, 
			const char* theName,
			SUIT_ResourceMgr* theResourceMgr);

  virtual
  ~SVTK_InteractorHolder();

  void
  SetInteractor(SVTK_RenderWindowInteractor* theInteractor);

  SVTK_RenderWindowInteractor*
  GetInteractor();

  vtkRenderWindowInteractor*
  GetRenderWindowInteractor();

  virtual void Repaint();
  void Repaint( bool theUpdateTrihedron );

  void
  InvokeEvent(unsigned long theEvent, void* theCallData);

  void
  SetRenderWindow(vtkRenderWindow *theRenderWindow);

  vtkRenderWindow*
  GetRenderWindow();

  vtkRenderWindow*
  getRenderWindow()
  {
    return GetRenderWindow();
  }

  SVTK_Selector* GetSelector();
  void SetSelector(SVTK_Selector* theSelector);

  SVTK_InteractorStyle* GetInteractorStyle();
  void PushInteractorStyle(SVTK_InteractorStyle* theStyle);
  void PopInteractorStyle();

  int SelectionMode() const;
  void SetSelectionMode(int theMode);

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
  SVTK_RenderWindowInteractor* myInteractor;
  vtkSmartPointer<vtkRenderWindow> myRenderWindow;
};


//----------------------------------------------------------------------------
class SVTK_EXPORT SVTK_View : public SVTK_InteractorHolder
{
  Q_OBJECT;

public:
  SVTK_View(QWidget* theParent, 
	    const char* theName,
	    SUIT_ResourceMgr* theResourceMgr);

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
