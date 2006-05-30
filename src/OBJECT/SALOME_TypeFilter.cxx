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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_TypeFilter.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SALOME_TypeFilter.ixx"
#include "SALOME_InteractiveObject.hxx"
using namespace std;

/*!
  Constructor
  \param TheKind - type of filter
*/
SALOME_TypeFilter::SALOME_TypeFilter(const Standard_CString TheKind):
myKind(TheKind){}

/*!
  \return true if object passes filter
  \param anObj - object to be checked
*/
Standard_Boolean SALOME_TypeFilter::IsOk(const Handle(SALOME_InteractiveObject)& anObj) const 
{
  return anObj->isComponentType( myKind );
}
