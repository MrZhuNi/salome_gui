#ifndef SVTK_VIEW_H
#define SVTK_VIEW_H

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include "SVTK.h"

#include "SVTK_Selection.h"
#include "SVTK_RenderWindowInteractor.h"


#include "SALOME_InteractiveObject.hxx"

#include <vtkSmartPointer.h>

#include <qvaluestack.h>

class vtkCallbackCommand;
class VTKViewer_Actor;

class SVTK_Viewer;
class SVTK_Selector;

class SVTK_RenderWindow;
class SVTK_InteractorStyle;

class SVTK_EXPORT SVTK_View : public SVTK_RenderWindowInteractor
{
  Q_OBJECT;

public:
  SVTK_View( QWidget*, const char* );
  virtual ~SVTK_View();
  
  SVTK_Selector* GetSelector();
  void SetSelector( SVTK_Selector* theSelector );

  SVTK_InteractorStyle* getInteractorStyle();
  void pushInteractorStyle( SVTK_InteractorStyle* );
  void popInteractorStyle();

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

  void AdjustTrihedrons( const bool forced );

  void SetSelectionProp(const double& theRed = 1, 
			const double& theGreen = 1,
			const double& theBlue = 0, 
			const int& theWidth = 5);
  void SetSelectionTolerance(const double& theTolNodes = 0.025, 
			     const double& theTolCell = 0.001);

public slots:
  void onSelectionChanged();
  void onEmitSelectionChanged();

signals:
  void selectionChanged();

public slots:
  virtual void activateZoom();
  virtual void activateWindowFit();
  virtual void activateRotation();
  virtual void activatePanning(); 
  virtual void activateGlobalPanning(); 

protected:  
  // Main process VTK event method
  static
  void
  ProcessEvents(vtkObject* theObject, 
		unsigned long theEvent,
		void* theClientData, 
		void* theCallData);

  SVTK_Viewer* myModel;

  // Used to process VTK events
  vtkSmartPointer<vtkCallbackCommand> myEventCallbackCommand;
  // Priority at which events are processed
  float myPriority;

  vtkSmartPointer<SVTK_Selector> mySelector;

  void initInteractorStyle( SVTK_InteractorStyle* );

  typedef vtkSmartPointer<SVTK_InteractorStyle> PInteractorStyle;
  QValueStack<PInteractorStyle> myInteractorStyles;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
