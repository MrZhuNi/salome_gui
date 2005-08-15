#ifndef SVTK_VIEW_H
#define SVTK_VIEW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"
#include "SVTK_Selection.h"
#include "SVTK_RenderWindowInteractor.h"

#include "SALOME_InteractiveObject.hxx"

class VTKViewer_Actor;

class SVTK_Viewer;
class SVTK_Selector;

class SVTK_RenderWindow;
class SVTK_InteractorStyle;

class SVTK_EXPORT SVTK_View : public SVTK_RenderWindowInteractor
{
  Q_OBJECT

public:
  SVTK_View( QWidget*, const char* );
  virtual ~SVTK_View();
  
  SVTK_Selector* GetSelector() { return mySelector; }
  void SetSelector( SVTK_Selector* theSelector ) { mySelector = theSelector; }

  SVTK_InteractorStyle* GetInteractorStyle() { return myInteractorStyle; }
  void SetInteractorStyle( SVTK_InteractorStyle* );

  Selection_Mode SelectionMode() const;
  void SetSelectionMode( Selection_Mode );

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

  //apply existing transformation on adding SALOME_Actor
  void AddActor( VTKViewer_Actor*, bool update = false );
  void RemoveActor(VTKViewer_Actor*, bool update = false);

  void AdjustTrihedrons( const bool forced );

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

  virtual void activateZoom();
  virtual void activateWindowFit();
  virtual void activateRotation();
  virtual void activatePanning(); 
  virtual void activateGlobalPanning(); 

protected:
  //QImage dumpView();

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
  SVTK_Viewer* myModel;
  SVTK_Selector* mySelector;

  SVTK_InteractorStyle* myInteractorStyle;

  double myCurScale;

  friend class SVTK_RenderWindowInteractor;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
