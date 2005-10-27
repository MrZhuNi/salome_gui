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

// ------------------------------------------------------------
// :TRICKY: Fri Apr 21 22:19:27 2000 Pagey
// The Signal/Slot mechanism used by Qt requires that QObject 
// appear as the first class when using multiple inheritance. 
// Hence the order of the two classes QObject and vtkRenderWindowInteractor
// matters here. Be careful not to change it by accident. 
// ------------------------------------------------------------
class SVTK_EXPORT QVTK_GenericRenderWindowInteractor: 
 public QObject,
 public vtkGenericRenderWindowInteractor
{
  Q_OBJECT;

 public:
  static QVTK_GenericRenderWindowInteractor* New();
  vtkTypeMacro(QVTK_GenericRenderWindowInteractor,vtkGenericRenderWindowInteractor);

  virtual
  int
  CreateTimer( int ) ; 

  virtual
  int
  DestroyTimer() ; 

 protected slots:
  void
  OnTimeOut();

 protected:
  QVTK_GenericRenderWindowInteractor();
  ~QVTK_GenericRenderWindowInteractor();

  QTimer* myTimer ;
};


// ------------------------------------------------------------
class SVTK_EXPORT SVTK_GenericRenderWindowInteractor: public QVTK_GenericRenderWindowInteractor
{
 public:
  static SVTK_GenericRenderWindowInteractor* New();
  vtkTypeMacro(SVTK_GenericRenderWindowInteractor,QVTK_GenericRenderWindowInteractor);

  SVTK_Selector* 
  GetSelector();

  void
  SetSelector(SVTK_Selector* theSelector);

  QWidget*
  GetRenderWidget();

  void
  SetRenderWidget(QWidget* theRenderWidget);

 protected:
  SVTK_GenericRenderWindowInteractor();
  ~SVTK_GenericRenderWindowInteractor();

  vtkSmartPointer<SVTK_Selector> mySelector;
  QWidget* myRenderWidget;
};

#endif
