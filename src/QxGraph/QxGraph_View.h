//  SALOME QxGraph : build Supervisor viewer into desktop
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

#ifndef QXGRAPH_VIEW_H
#define QXGRAPH_VIEW_H

#include "QxGraph.h"

//#include <qaction.h>
#include <qwidget.h>

#ifdef WIN32
#pragma warning ( disable:4251 )
#endif

class SUIT_ResourceMgr;
class QxGraph_Canvas;
class QxGraph_CanvasView;

class QXGRAPH_EXPORT QxGraph_View : public QWidget {
  Q_OBJECT

 public:
  QxGraph_View(QWidget* theParent);
  QxGraph_View(QxGraph_View* theParent);

  ~QxGraph_View() {};

  SUIT_ResourceMgr* resMgr() const;

 protected:
  void init(QWidget* theParent);
  virtual void initCanvas();
  virtual void initCanvasViews();

 private:
  QxGraph_Canvas*               myCanvas;
  QPtrList<QxGraph_CanvasView> myCanvasViews;

  QxGraph_CanvasView*           myCurrentView;

};

#ifdef WIN32
#pragma warning ( default:4251 )
#endif

#endif
