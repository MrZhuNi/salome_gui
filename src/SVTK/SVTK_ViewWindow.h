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

//! Define a container for SALOME VTK view window
class SVTK_EXPORT SVTK_ViewWindow : public SUIT_ViewWindow
{
  Q_OBJECT;

 public:
  //! To construct #SVTK_ViewWindow instance
  SVTK_ViewWindow(SUIT_Desktop* theDesktop);

  virtual
  ~SVTK_ViewWindow();
  
  //! To initialize #SVTK_ViewWindow instance
  virtual
  void
  Initialize(SVTK_ViewModelBase* theModel);

  //! Get #SVTK_View
  SVTK_View* 
  getView();

  //! Get #SVTK_MainWindow
  SVTK_MainWindow* 
  getMainWindow();

  //! Get #vtkRenderWindow (obsolete)
  vtkRenderWindow* 
  getRenderWindow();

  //! Get #vtkRenderWindow (obsolete)
  vtkRenderWindowInteractor*
  getInteractor();

  //! Get #vtkRenderer
  vtkRenderer* 
  getRenderer();

  //! Get #SVTK_Selector
  SVTK_Selector* 
  GetSelector();
  
  //! Redirect the request to #SVTK_Selector::SelectionMode
  Selection_Mode
  SelectionMode() const;
  
  //! Change selection mode
  virtual
  void
  SetSelectionMode(Selection_Mode theMode);

  //! Set background color
  virtual
  void
  setBackgroundColor( const QColor& );

  //! Get current background color
  QColor
  backgroundColor() const;

  //! Redirect the request to #SVTK_Renderer::IsTrihedronDisplayed
  bool
  isTrihedronDisplayed();

  //! Redirect the request to #SVTK_Renderer::IsCubeAxesDisplayed
  bool
  isCubeAxesDisplayed();
 
  /*  interactive object management */
  //! To highlight a VTK presentation with the same #SALOME_InteractiveObject
  virtual
  void
  highlight(const Handle(SALOME_InteractiveObject)& theIO, 
	    bool theIsHighlight = true, 
	    bool theIsUpdate = true);

  //! To unhighlight all VTK presentations
  virtual
  void
  unHighlightAll();

  //! Check, if the viewer contains a presentatin with defined #SALOME_InteractiveObject
  bool
  isInViewer(const Handle(SALOME_InteractiveObject)& theIObject);

  //! Check, if a presentatin with defined #SALOME_InteractiveObject is visible
  bool
  isVisible(const Handle(SALOME_InteractiveObject)& theIObject);

  /* selection */
  //----------------------------------------------------------------------------
  Handle(SALOME_InteractiveObject) 
  FindIObject(const char* theEntry);
  
  /* display */		
  //----------------------------------------------------------------------------
  //! To display VTK presentation with defined #SALOME_InteractiveObject
  virtual
  void
  Display(const Handle(SALOME_InteractiveObject)& theIObject,
	  bool theImmediatly = true);

  //! To display VTK presentation with defined #SALOME_InteractiveObject and erase all anothers
  virtual
  void
  DisplayOnly(const Handle(SALOME_InteractiveObject)& theIObject);

  //! To erase VTK presentation with defined #SALOME_InteractiveObject
  virtual
  void
  Erase(const Handle(SALOME_InteractiveObject)& theIObject,
	bool theImmediatly = true);

  //! To display all existing VTK presentations
  virtual
  void 
  DisplayAll();

  //! To erase all existing VTK presentations
  virtual
  void 
  EraseAll();

  //! To repaint the viewer
  virtual
  void
  Repaint(bool theUpdateTrihedron = true);

  //----------------------------------------------------------------------------
  //! Redirect the request to #SVTK_Renderer::SetScale
  virtual
  void 
  SetScale( double theScale[3] );

  //! Redirect the request to #SVTK_Renderer::GetScale
  virtual
  void
  GetScale( double theScale[3] );

  //! Redirect the request to #SVTK_Renderer::AddActor
  virtual
  void
  AddActor(VTKViewer_Actor* theActor,
	   bool theIsUpdate = false);

  //! Redirect the request to #SVTK_Renderer::RemoveActor
  virtual
  void
  RemoveActor(VTKViewer_Actor* theActor,
	      bool theIsUpdate = false);

  //----------------------------------------------------------------------------
  //! Redirect the request to #SVTK_Renderer::AdjustActors
  virtual
  void
  AdjustTrihedrons(const bool theIsForced);

  //! Redirect the request to #SVTK_Renderer::GetTrihedron
  VTKViewer_Trihedron*  
  GetTrihedron();

  //! Redirect the request to #SVTK_Renderer::GetCubeAxes
  SVTK_CubeAxesActor2D* 
  GetCubeAxes();

  //! Redirect the request to #SVTK_Renderer::GetTrihedronSize
  int  
  GetTrihedronSize() const;

  //! Redirect the request to #SVTK_Renderer::SetTrihedronSize
  virtual
  void 
  SetTrihedronSize( const int );

  //! Redirect the request to #SVTK_Renderer::SetSelectionProp
  virtual
  void
  SetSelectionProp(const double& theRed = 1, 
		   const double& theGreen = 1,
		   const double& theBlue = 0, 
		   const int& theWidth = 5);

  //! Redirect the request to #SVTK_Renderer::SetPreselectionProp
  virtual
  void
  SetPreselectionProp(const double& theRed = 0, 
		      const double& theGreen = 1,
		      const double& theBlue = 1, 
		      const int& theWidth = 5);

  //! Redirect the request to #SVTK_Renderer::SetSelectionTolerance
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
  //! Redirect the request to #SVTK_Renderer::OnFrontView
  virtual
  void
  onFrontView(); 

  //! Redirect the request to #SVTK_Renderer::OnBackView
  virtual
  void
  onBackView(); 

  //! Redirect the request to #SVTK_Renderer::OnTopView
  virtual
  void
  onTopView();

  //! Redirect the request to #SVTK_Renderer::OnBottomView
  virtual
  void
  onBottomView();

  //! Redirect the request to #SVTK_Renderer::OnRightView
  virtual
  void 
  onRightView(); 

  //! Redirect the request to #SVTK_Renderer::OnLeftView
  virtual
  void 
  onLeftView();     

  //! Redirect the request to #SVTK_Renderer::OnResetView
  virtual
  void
  onResetView();     

  //! Redirect the request to #SVTK_Renderer::OnFitAll
  virtual
  void 
  onFitAll();

  //! Redirect the request to #SVTK_Renderer::OnViewTrihedron
  virtual
  void
  onViewTrihedron(); 

  //! Redirect the request to #SVTK_Renderer::OnViewCubeAxes
  virtual
  void
  onViewCubeAxes();

  //! Redirect the request to #SVTK_Renderer::OnAdjustTrihedron
  virtual
  void
  onAdjustTrihedron();

  //! Redirect the request to #SVTK_Renderer::OnAdjustCubeAxes
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
