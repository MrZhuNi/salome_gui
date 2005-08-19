#ifndef SVTK_VIEWWINDOW_H
#define SVTK_VIEWWINDOW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"
#include "SVTK_View.h"
#include "SVTK_Selection.h"

#include "SUIT_ViewWindow.h"

#include "SALOME_InteractiveObject.hxx"

#include <vtkSmartPointer.h>

class SUIT_Desktop;

class VTKViewer_Actor;
class VTKViewer_Trihedron;

class SVTK_ViewModelBase;
class SVTK_Selector;

//class SVTK_View;
//class SVTK_RenderWindowInteractor;
class SVTK_InteractorStyle;
class SVTK_CubeAxesActor2D;

class vtkRenderer;

class SVTK_EXPORT SVTK_ViewWindow : public SUIT_ViewWindow
{
  Q_OBJECT

public:
  SVTK_ViewWindow( SUIT_Desktop*, SVTK_ViewModelBase* );
  virtual ~SVTK_ViewWindow();
  
  void setBackgroundColor( const QColor& );
  QColor backgroundColor() const;

  vtkRenderer* getRenderer();
  SVTK_Selector* GetSelector();

  SVTK_View* getView() { return myView; }
  SVTK_View* getRenderWindow() { return myView; }
  SVTK_View* getRWInteractor() { return myView; }
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

protected slots:
  void onKeyPressed(QKeyEvent* event);
  void onKeyReleased(QKeyEvent* event);
  void onMousePressed(QMouseEvent* event);
  void onMouseDoubleClicked(QMouseEvent* event);
  void onMouseReleased(QMouseEvent* event);
  void onMouseMoving(QMouseEvent* event);

protected:
  QImage dumpView();

  SVTK_View* myView;
  SVTK_InteractorStyle* myInteractorStyle;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
