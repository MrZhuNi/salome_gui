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

#include "QxGraph_View.h"
#include "QxGraph_Canvas.h"
#include "QxGraph_CanvasView.h"

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>

//QT Include
#include <qmainwindow.h>
#include <qlayout.h>

/*!
  Constructor
*/
QxGraph_View::QxGraph_View( QWidget* theParent ): QWidget( theParent )
{
  printf("Constructor 1 of QxGraph_View\n");
  init(theParent);
}

/*!
  Constructor
*/
QxGraph_View::QxGraph_View( QxGraph_View* theParent ): QWidget( theParent )
{
  printf("Constructor 2 of QxGraph_View\n");
  init(theParent);
}

/*!
  Initialization
*/
void QxGraph_View::init( QWidget* theParent )
{
  if ( theParent->inherits( "QMainWindow" ) ) {
    ( ( QMainWindow* )theParent )->setCentralWidget( this );
  }
  else {
    QBoxLayout* layout = new QVBoxLayout( theParent );
    layout->addWidget( this );
  }

  initCanvas();
  initCanvasViews();
}

/*!
  Canvas initialization
*/
void QxGraph_View::initCanvas()
{
  myCanvas = new QxGraph_Canvas(resMgr());
}

/*!
  Canvas views initialization
*/
void QxGraph_View::initCanvasViews()
{
  QBoxLayout * layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);

  for (int i = 0; i < 2; i++)
  {
    QxGraph_CanvasView* aCanvasView = new QxGraph_CanvasView( myCanvas, this );
    myCanvasViews.append(aCanvasView);
    layout->addWidget(aCanvasView);
    aCanvasView->hide();
  }

  // the first view is shown and is the current
  myCanvasViews.first()->show();
  myCurrentView = myCanvasViews.first();
}

/*!
  Get resource manager
*/
SUIT_ResourceMgr* QxGraph_View::resMgr() const
{
  return SUIT_Session::session()->resourceMgr();
}
