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
//  File   : 
//  Author : 
//  Module : SALOME
//  $Header$

#ifndef SVTK_GenericRenderWindowInteractor_h
#define SVTK_GenericRenderWindowInteractor_h

#include "SVTK.h"

#include <qobject.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

class QTimer;
class QWidget;

class SVTK_Selector;
class SVTK_Renderer;


//============================================================================
//! Implemementation of the class is intended to implement Qt based CreateTimer and DestroyTimer functionality.
/*!
  The class intendes to implement platform indepenedant subclass of vtkRenderWindowInteractor.
  This is done by usage of Qt library.
  \note
  The Signal/Slot mechanism used by Qt requires that QObject 
  appear as the first class when using multiple inheritance. 
  Hence the order of the two classes QObject and vtkRenderWindowInteractor
  matters here. So, be careful, not to change the sequence of the inheritance by accident. 
*/
class SVTK_EXPORT QVTK_GenericRenderWindowInteractor: 
 public QObject,
 public vtkGenericRenderWindowInteractor
{
  Q_OBJECT;

 public:
  static QVTK_GenericRenderWindowInteractor* New();
  vtkTypeMacro(QVTK_GenericRenderWindowInteractor,vtkGenericRenderWindowInteractor);

  //! Starts the QTimer instance on defined microseconds
  virtual
  int
  CreateTimer( int ) ; 

  //! Stops the QTimer instance
  virtual
  int
  DestroyTimer() ; 

 protected slots:
  //! The slot connects to QTimer::timeout signal to invoke vtkCommand::TimerEvent
  void
  OnTimeOut();

 protected:
  QVTK_GenericRenderWindowInteractor(); //!< Instatiate QTimer
  ~QVTK_GenericRenderWindowInteractor(); //!< Destroy the instance of QTimer

  QTimer* myTimer; //!< Qt timer device
};


//============================================================================
//! This class introduce SALOME specific to the base one
/*!
  In this class new members is added (#mySelector and #myRenderWidget).
  They are used for initialization of #SVTK_InteractorStyle by redefinition of
  #SVTK_InteractorStyle::SetInteractor method
*/
class SVTK_EXPORT SVTK_GenericRenderWindowInteractor: public QVTK_GenericRenderWindowInteractor
{
 public:
  static SVTK_GenericRenderWindowInteractor* New();
  vtkTypeMacro(SVTK_GenericRenderWindowInteractor,QVTK_GenericRenderWindowInteractor);

  //! To get access to SVTK_Selector
  SVTK_Selector* 
  GetSelector(); 

  //! To initialize mySelector field
  void
  SetSelector(SVTK_Selector* theSelector);

  //! To get access to QWidget, where vtkRenderWindow maps to.
  QWidget*
  GetRenderWidget();

  //! To initialize myRenderWidget field.
  void
  SetRenderWidget(QWidget* theRenderWidget);

 protected:
  SVTK_GenericRenderWindowInteractor();
  ~SVTK_GenericRenderWindowInteractor();

  vtkSmartPointer<SVTK_Selector> mySelector; //!< Keeps a pointer to SVTK_Selector
  QWidget* myRenderWidget; //!< Keeps a pointer to QWidget, where vtkRenderWindow maps to.
};

#endif
