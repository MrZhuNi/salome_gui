//  Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
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

#ifndef QXGRAPH_VIEWMANAGER_H
#define QXGRAPH_VIEWMANAGER_H

#include "QxGraph.h"

#include "QxGraph_ViewModel.h"
#include "SUIT_ViewManager.h"

class SUIT_Desktop;

class QXGRAPH_EXPORT QxGraph_ViewManager : public SUIT_ViewManager
{
  Q_OBJECT

public:
  QxGraph_ViewManager( SUIT_Study* theStudy, SUIT_Desktop* theDesktop, SUIT_ViewModel* theViewModel = 0 );
  ~QxGraph_ViewManager();

  virtual void      contextMenuPopup( QPopupMenu* thePopup );

protected:
  void              setViewName(SUIT_ViewWindow* theView);

};

#endif
