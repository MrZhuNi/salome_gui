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
#ifndef SALOMEAPP_FILTER_H
#define SALOMEAPP_FILTER_H

#include "SalomeApp.h"
#include "SUIT_SelectionFilter.h"

class SalomeApp_Study;

class SALOMEAPP_EXPORT SalomeApp_Filter: public SUIT_SelectionFilter
{
public:
  SalomeApp_Filter(SalomeApp_Study* study);
  ~SalomeApp_Filter();

protected:
  SalomeApp_Study* getStudy() const;

private:
  SalomeApp_Study* myStudy;
};

#endif