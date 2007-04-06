// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
#ifndef QXGRAPH_ACTIVEITEM_H
#define QXGRAPH_ACTIVEITEM_H

#include "QxGraph.h"

#include <qpoint.h>

class QXGRAPH_EXPORT QxGraph_ActiveItem
{
 public:
  QxGraph_ActiveItem() {}
  ~QxGraph_ActiveItem() {}

  virtual bool isMoveable() = 0;
  virtual void beforeMoving() = 0;
  virtual void afterMoving() = 0;

  virtual bool isResizable(QPoint thePoint, int& theCursorType) { return false; }
  virtual bool isResizing() { return false; }
  virtual void beforeResizing(int theCursorType) {}
  virtual void resize(QPoint thePoint) {}
  virtual void afterResizing() {}

  virtual void hilight(const bool toHilight = true) = 0;
  virtual void select() = 0;
  virtual void showPopup() = 0;

  virtual QString getToolTipText(const QPoint& theMousePos, QRect& theRect) const = 0;
};

#endif
