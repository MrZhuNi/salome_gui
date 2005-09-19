//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : SVTK_SelectionEvent.h
//  Author : 
//  Module : SALOME
//  $Header$

#ifndef SVTK_SELECTIONEVENT_H
#define SVTK_SELECTIONEVENT_H

#include "SVTK_Selection.h"

struct SVTK_SelectionEvent
{
  Selection_Mode mySelectionMode;
  int myX;
  int myY;
  int myLastX;
  int myLastY;
  bool myIsRectangle;
  bool myIsCtrl;
  bool myIsShift;
  int myKeyCode;
  int myRepeatCount;
};

#endif
