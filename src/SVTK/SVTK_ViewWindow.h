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
  
  virtual
  void
  SetSelectionMode(Selection_Mode theMode);

  virtual
  void
  setBackgroundColor( const QColor& );

  QColor
  backgroundColor() const;

  bool
  isTrihedronDisplayed();

  bool
  isCubeAxesDisplayed();
 
  /*  interactive object management */
  virtual
  void
  highlight(const Handle(SALOME_InteractiveObject)& theIO, 
	    bool theIsHighlight = true, 
	    bool theIsUpdate = true);
  virtual
  void
  unHighlightAll();

  bool
  isInViewer(const Handle(SALOME_InteractiveObject)& theIObject);

  bool
  isVisible(const Handle(SALOME_InteractiveObject)& theIObject);

  /* selection */
  //----------------------------------------------------------------------------
  Handle(SALOME_InteractiveObject) 
  FindIObject(const char* theEntry);
  
  /* display */		
  //----------------------------------------------------------------------------
  virtual
  void
  Display(const Handle(SALOME_InteractiveObject)& theIObject,
	  bool theImmediatly = true);

  virtual
  void
  DisplayOnly(const Handle(SALOME_InteractiveObject)& theIObject);

  virtual
  void
  Erase(const Handle(SALOME_InteractiveObject)& theIObject,
	bool theImmediatly = true);

  virtual
  void 
  DisplayAll();

  virtual
  void 
  EraseAll();

  virtual
  void
  Repaint(bool theUpdateTrihedron = true);

  //----------------------------------------------------------------------------
  virtual
  void 
  SetScale( double theScale[3] );

  virtual
  void
  GetScale( double theScale[3] );

  virtual
  void
  AddActor(VTKViewer_Actor* theActor,
	   bool theIsUpdate = false);

  virtual
  void
  RemoveActor(VTKViewer_Actor* theActor,
	      bool theIsUpdate = false);

  //----------------------------------------------------------------------------
  virtual
  void
  AdjustTrihedrons(const bool theIsForced);

  VTKViewer_Trihedron*  
  GetTrihedron();

  SVTK_CubeAxesActor2D* 
  GetCubeAxes();

  int  
  GetTrihedronSize() const;

  virtual
  void 
  SetTrihedronSize( const int );

  virtual
  void
  SetSelectionProp(const double& theRed = 1, 
		   const double& theGreen = 1,
		   const double& theBlue = 0, 
		   const int& theWidth = 5);

  virtual
  void
  SetPreselectionProp(const double& theRed = 0, 
		      const double& theGreen = 1,
		      const double& theBlue = 1, 
		      const int& theWidth = 5);
  virtual
  void
  SetSelectionTolerance(const double& theTolNodes = 0.025, 
			const double& theTolCell = 0.001);
  
public slots:
  virtual
  void
  onSelectionChanged();

signals:
 void selectionChanged();

public slots:
  virtual
  void
  onFrontView(); 

  virtual
  void
  onBackView(); 

  virtual
  void
  onTopView();

  virtual
  void
  onBottomView();

  virtual
  void 
  onRightView(); 

  virtual
  void 
  onLeftView();     

  virtual
  void
  onResetView();     

  virtual
  void 
  onFitAll();

  virtual
  void
  onViewTrihedron(); 

  virtual
  void
  onViewCubeAxes();

  virtual
  void
  onAdjustTrihedron();

  virtual
  void 
  onAdjustCubeAxes();

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
