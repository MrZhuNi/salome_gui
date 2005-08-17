//  SALOME VTKViewer : build VTK viewer into Salome desktop
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SVTK_RenderWindowInteractor.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SVTK_RenderWindowInteractor_h
#define SVTK_RenderWindowInteractor_h

#include "SVTK.h"
#include "SVTK_Selection.h"
#include "SVTK_RenderWindow.h"

#include "SALOME_InteractiveObject.hxx"

// QT Includes
#include <qtimer.h>

// VTK Includes
#include <vtkVersion.h>

class vtkActorCollection;
class vtkGenericRenderWindowInteractor;

class SALOME_Actor;

// ------------------------------------------------------------
// :TRICKY: Fri Apr 21 22:19:27 2000 Pagey
// The Signal/Slot mechanism used by Qt requires that QObject 
// appear as the first class when using multiple inheritance. 
// Hence the order of the two classes QObject and vtkRenderWindowInteractor
// matters here. Be careful not to change it by accident. 
// ------------------------------------------------------------
class SVTK_EXPORT SVTK_RenderWindowInteractor: public SVTK_RenderWindow//, public vtkRenderWindowInteractor
{
  Q_OBJECT

public:
  SVTK_RenderWindowInteractor( QWidget*, const char* );
  ~SVTK_RenderWindowInteractor();

  vtkGenericRenderWindowInteractor* getInteractor() { return myInteractor; }

  // Description:
  // Initializes the event handlers without an XtAppContext.  This is
  // good for when you don`t have a user interface, but you still
  // want to have mouse interaction.
  virtual void Initialize();

  // Description:
  // Event loop notification member for Window size change
  virtual void UpdateSize(int x,int y);

  // Description:
  // Timer methods must be overridden by platform dependent subclasses.
  // flag is passed to indicate if this is first timer set or an update
  // as Win32 uses repeating timers, whereas X uses One shot more timer
  // if flag==VTKXI_TIMER_FIRST Win32 and X should createtimer
  // otherwise Win32 should exit and X should perform AddTimeOut()
  virtual int CreateTimer(int ) ; 
  virtual int DestroyTimer() ; 
  
  /* Selection Management */
  bool isInViewer( const Handle(SALOME_InteractiveObject)& IObject);
  bool isVisible( const Handle(SALOME_InteractiveObject)& IObject);
  void rename(const Handle(SALOME_InteractiveObject)& IObject, QString newName);

  // Displaymode management
  int GetDisplayMode();
  void SetDisplayMode(int);

  // Switch representation wireframe/shading
  void SetDisplayMode(const Handle(SALOME_InteractiveObject)& IObject, int theMode);

  // Change all actors to wireframe or surface
  void ChangeRepresentationToWireframe();
  void ChangeRepresentationToSurface();

  // Change to wireframe or surface a list of vtkactor
  void ChangeRepresentationToWireframe(vtkActorCollection* ListofActors);
  void ChangeRepresentationToSurface(vtkActorCollection* ListofActors);

  // Erase Display functions
  void EraseAll();
  void DisplayAll();
  void RemoveAll( const bool immediatly );
  void Erase(const Handle(SALOME_InteractiveObject)& IObject, 
	     bool immediatly=true);
  void Remove(const Handle(SALOME_InteractiveObject)& IObject, 
	      bool immediatly=true);
  void Display(const Handle(SALOME_InteractiveObject)& IObject, 
	       bool immediatly=true);

  void Display( SALOME_Actor* SActor, 
		bool immediatly = true );
  void Erase( SALOME_Actor* SActor, 
	      bool immediatly = true );
  void Remove( SALOME_Actor* SActor, 
	       bool updateViewer = true );

  // Transparency
  void SetTransparency(const Handle(SALOME_InteractiveObject)& IObject,
		       float trans);
  float GetTransparency(const Handle(SALOME_InteractiveObject)& IObject);

  // Color
  void SetColor(const Handle(SALOME_InteractiveObject)& IObject,
		QColor thecolor);
  QColor GetColor(const Handle(SALOME_InteractiveObject)& IObject);

  void Update();

  vtkRenderer* GetRenderer();

 protected:
  vtkGenericRenderWindowInteractor* myInteractor;

  // Timer used during various mouse events to figure 
  // out mouse movements. 
  QTimer *mTimer ;

  int myDisplayMode;
  
  // User for switching to stereo mode.
  int PositionBeforeStereo[2];

 protected:
  virtual void mouseMoveEvent( QMouseEvent* );
  virtual void mousePressEvent( QMouseEvent* );
  virtual void mouseReleaseEvent( QMouseEvent* );
  virtual void mouseDoubleClickEvent( QMouseEvent* );
  virtual void wheelEvent( QWheelEvent* );
  virtual void keyPressEvent( QKeyEvent* );
  virtual void keyReleaseEvent( QKeyEvent* );
  virtual void contextMenuEvent( QContextMenuEvent * e );

  virtual void paintEvent( QPaintEvent* );
  virtual void resizeEvent( QResizeEvent* );
  virtual void onChangeBackgroundColor();

 private slots:
  // Not all of these slots are needed in VTK_MAJOR_VERSION=3,
  // but moc does not understand "#if VTK_MAJOR_VERSION". Hence, 
  // we have to include all of these for the time being. Once,
  // this bug in MOC is fixed, we can separate these. 
  void TimerFunc() ;

 signals:
  void contextMenuRequested( QContextMenuEvent *e );

};


#endif
