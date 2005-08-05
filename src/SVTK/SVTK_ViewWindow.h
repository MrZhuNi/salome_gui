#ifndef SVTK_VIEWWINDOW_H
#define SVTK_VIEWWINDOW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"
#include "SUIT_ViewWindow.h"

#include "SVTK_Selection.h"
#include "SALOME_InteractiveObject.hxx"

class vtkRenderer;

class QtxAction;
class SUIT_Desktop;

class VTKViewer_Trihedron;
class VTKViewer_Transform;

class VTKViewer_Actor;

class SVTK_Viewer;
class SVTK_Selector;
class SVTK_CubeAxesActor2D;

class SVTK_RenderWindow;
class SVTK_InteractorStyle;
class SVTK_RenderWindowInteractor;
class SVTK_InteractorStyle;

class SVTK_EXPORT SVTK_ViewWindow : public SUIT_ViewWindow
{
  Q_OBJECT;

public:
  SVTK_ViewWindow( SUIT_Desktop*, SVTK_Viewer* );
  virtual ~SVTK_ViewWindow();
  
  QToolBar* getToolBar() { return myToolBar; }
  
  void setBackgroundColor( const QColor& );
  QColor backgroundColor() const;

  vtkRenderer* getRenderer() {return myRenderer;}
  SVTK_Selector* GetSelector() {return mySelector;}
  SVTK_RenderWindow* getRenderWindow() {return myRenderWindow;}
  SVTK_RenderWindowInteractor* getRWInteractor() {return myRWInteractor;}
  SVTK_InteractorStyle* getInteractorStyle(){ return myInteractorStyle;}
  Selection_Mode SelectionMode() const;
  void SetSelectionMode(Selection_Mode theMode);

  bool isTrihedronDisplayed();
  bool isCubeAxesDisplayed();
 
  /*  interactive object management */
  void highlight( const Handle(SALOME_InteractiveObject)& IObject, 
                  bool highlight, bool immediatly = true );
  void unHighlightAll();
  bool isInViewer( const Handle(SALOME_InteractiveObject)& IObject );
  bool isVisible( const Handle(SALOME_InteractiveObject)& IObject );

  /* selection */
  Handle(SALOME_InteractiveObject) FindIObject(const char* Entry);
  
  /* display */		
  void Display( const Handle(SALOME_InteractiveObject)& IObject,
                bool immediatly = true );
  void DisplayOnly( const Handle(SALOME_InteractiveObject)& IObject );
  void Erase( const Handle(SALOME_InteractiveObject)& IObject,
              bool immediatly = true );
  void DisplayAll();
  void EraseAll();
  void Repaint( bool theUpdateTrihedron );
  void Repaint() { Repaint(true); }

  //apply existing transformation on adding SALOME_Actor
  void SetScale( double theScale[3] );
  void GetScale( double theScale[3] );
  void AddActor( VTKViewer_Actor*, bool update = false );
  void RemoveActor(VTKViewer_Actor*, bool update = false);

  void AdjustTrihedrons( const bool forced );
  //merge with V2_2_0_VISU_improvements:bool ComputeTrihedronSize( double& theNewSize,
  //merge with V2_2_0_VISU_improvements:                          double& theOldSize );

  int  GetTrihedronSize() const;
  void SetTrihedronSize( const int );

  VTKViewer_Trihedron*  GetTrihedron() {return this->myTrihedron;};
  SVTK_CubeAxesActor2D* GetCubeAxes() {return this->myCubeAxes;};

  void SetSelectionProp(const double& theRed = 1, 
			const double& theGreen = 1,
			const double& theBlue = 0, 
			const int& theWidth = 5);
  void SetSelectionTolerance(const double& theTolNodes = 0.025, 
			     const double& theTolCell = 0.001);

public slots:
  void onSelectionChanged();

signals:
 void selectionChanged();

public slots:
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
 
  void onPanLeft();
  void onPanRight();
  void onPanUp();
  void onPanDown();
  void onZoomIn();
  void onZoomOut();
  void onRotateLeft();
  void onRotateRight();
  void onRotateUp();
  void onRotateDown();

  void activateZoom();
  void activateWindowFit();
  void activateRotation();
  void activatePanning(); 
  void activateGlobalPanning(); 

protected:
  QImage dumpView();

protected slots:
  void onKeyPressed(QKeyEvent* event);
  void onKeyReleased(QKeyEvent* event);
  void onMousePressed(QMouseEvent* event);
  void onMouseDoubleClicked(QMouseEvent* event);
  void onMouseReleased(QMouseEvent* event);
  void onMouseMoving(QMouseEvent* event);

private:
  void InitialSetup();
  void InsertActor( VTKViewer_Actor* theActor,
                    bool theMoveInternalActors = false );
  void MoveActor( VTKViewer_Actor* theActor );

private:  
  enum { DumpId, FitAllId, FitRectId, ZoomId, PanId, GlobalPanId, RotationId,
         FrontId, BackId, TopId, BottomId, LeftId, RightId, ResetId, ViewTrihedronId };
  typedef QMap<int, QtxAction*> ActionsMap;
  
  void createActions();
  void createToolBar();
  
  vtkRenderer* myRenderer;

  SVTK_Viewer* myModel;
  SVTK_Selector* mySelector;

  SVTK_RenderWindow* myRenderWindow;
  SVTK_RenderWindowInteractor* myRWInteractor;
  SVTK_InteractorStyle* myInteractorStyle;

  VTKViewer_Transform*  myTransform;
  VTKViewer_Trihedron*  myTrihedron;  
  int                   myTrihedronSize;
  SVTK_CubeAxesActor2D* myCubeAxes;
  
  QToolBar* myToolBar;
  ActionsMap myActionsMap;  
  
  double myCurScale;

  friend class SVTK_RenderWindowInteractor;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
