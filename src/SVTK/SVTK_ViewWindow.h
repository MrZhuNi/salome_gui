#ifndef SVTK_VIEWWINDOW_H
#define SVTK_VIEWWINDOW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"
#include "SVTK_Selection.h"
#include "SUIT_ViewWindow.h"
#include "SALOME_InteractiveObject.hxx"

class SUIT_Desktop;

class VTKViewer_Actor;
class VTKViewer_Trihedron;

class SVTK_ViewModelBase;
class SVTK_MainWindow;
class SVTK_Selector;
class SVTK_View;

class SVTK_InteractorStyle;
class SVTK_CubeAxesActor2D;

class SVTK_RenderWindow;
class SVTK_RenderWindowInteractor;

class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;

class SVTK_EXPORT SVTK_ViewWindow : public SUIT_ViewWindow
{
  Q_OBJECT;

 public:
  SVTK_ViewWindow(SUIT_Desktop* theDesktop);

  virtual
  ~SVTK_ViewWindow();
  
  virtual
  void
  Initialize(SVTK_ViewModelBase* theModel);

  SVTK_View* 
  getView();

  SVTK_MainWindow* 
  getMainWindow();

  vtkRenderWindow* 
  getRenderWindow();

  vtkRenderWindowInteractor*
  getInteractor();

  vtkRenderer* 
  getRenderer();

  SVTK_Selector* 
  GetSelector();
  
  Selection_Mode
  SelectionMode() const;
  
  void
  SetSelectionMode(Selection_Mode theMode);

  void setBackgroundColor( const QColor& );
  QColor backgroundColor() const;

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

  VTKViewer_Trihedron*  GetTrihedron();
  SVTK_CubeAxesActor2D* GetCubeAxes();

  int  GetTrihedronSize() const;
  void SetTrihedronSize( const int );

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

protected slots:
  void onKeyPressed(QKeyEvent* event);
  void onKeyReleased(QKeyEvent* event);
  void onMousePressed(QMouseEvent* event);
  void onMouseDoubleClicked(QMouseEvent* event);
  void onMouseReleased(QMouseEvent* event);
  void onMouseMoving(QMouseEvent* event);

protected:
  virtual
  void
  Initialize(SVTK_View* theView,
	     SVTK_ViewModelBase* theModel);

  QImage dumpView();
  virtual void action( const int );

  SVTK_View* myView;
  SVTK_MainWindow* myMainWindow;
  SVTK_ViewModelBase* myModel;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
