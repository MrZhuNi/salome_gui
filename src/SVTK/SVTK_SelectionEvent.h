// Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//  File   : SVTK_SelectionEvent.h
//  Author : 

#ifndef SVTK_SELECTIONEVENT_H
#define SVTK_SELECTIONEVENT_H

#include "SVTK_Selection.h"

//! The structure is used for passing all infromation necessary for claculation of the selection.
/*!
  The SVTK_SelectionEvent is generated by #SVTK_InteractorStyle.
  In its turn the #SVTK_InteractorStyle uses for it generation #vtkRenderWindowInteractor and
  #SVTK_Selector instances.
 */
struct SVTK_SelectionEvent
{
  //! Through usage of the parameter any new type of selection can be introduced
  Selection_Mode mySelectionMode; 
  int myX; //!< Current X position of the mouse
  int myY; //!< Current Y position of the mouse
  int myLastX; //!< Last X position of the mouse before starting any complex operation (like rectangle selection)
  int myLastY; //!< Last Y position of the mouse before starting any complex operation
  bool myIsRectangle; //!< A flag to define is it necessary provide rectangle selection for current #Selection_Mode
  bool myIsCtrl; //!< Is Ctrl key are pressed
  bool myIsShift; //!< Is Shift key are pressed
  int myKeyCode; //!< Code of the pressed key
  int myRepeatCount; //!< Number of repetion of the key pressing
};

#endif
