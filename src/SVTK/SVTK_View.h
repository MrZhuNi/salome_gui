#ifndef SVTK_VIEW_H
#define SVTK_VIEW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"
#include "SALOME_InteractiveObject.hxx"

#include <qobject.h>

class vtkActorCollection;
class vtkRenderer;

class SVTK_MainWindow;
class SVTK_Renderer;

class SALOME_Actor;

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
  
  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  void
  SetSelectionProp(const double& theRed = 1, 
		   const double& theGreen = 1,
		   const double& theBlue = 0, 
		   const int& theWidth = 5);

  void
  SetPreselectionProp(const double& theRed = 0, 
		      const double& theGreen = 1,
		      const double& theBlue = 1, 
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
