//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  SALOME RegistryDisplay : GUI for Registry server implementation
//  File   : RegWidgetFactory.cxx
//  Author : Pascale NOYRET, EDF
//  Module : SALOME
//  $Header$
//
# include "RegWidget.h"
# include "utilities.h"

using namespace std;

extern "C"
{

void OnGUIEvent( CORBA::ORB_var &orb, QWidget *parent, const char *name )
{
  // N.B. parent can be 0
 RegWidget* ptrRegWidget = RegWidget::GetRegWidget( orb, parent, name );
 //ASSERT ( !ptrRegWidget );
 ptrRegWidget->show();
 ptrRegWidget->raise();
 ptrRegWidget->setActiveWindow();
 return;
}

}