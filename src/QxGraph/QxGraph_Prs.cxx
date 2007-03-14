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

#include "QxGraph_Prs.h"

#include "QxGraph_Canvas.h"
#include "QxGraph_Def.h" // for debug only

#include "SUIT_Session.h" // for debug only

/*!
  Constructor
*/
QxGraph_Prs::QxGraph_Prs(QxGraph_Canvas* theCanvas):
  myCanvas(theCanvas),
  myDMode(0)
{
  myCanvas->addPrs(this);
}

/*!
  Destructor
*/
QxGraph_Prs::~QxGraph_Prs()
{

}

/*!
  Add item to display in the view with index theDMode
*/
void QxGraph_Prs::addItem(QCanvasItem* theItem, int theDMode)
{
  if ( theDMode == -1 ) // add item for the current display mode
    myDisplayMap[myDMode].append(theItem);
  else
    myDisplayMap[theDMode].append(theItem);
}

/*!
  Add a QCanvasRectangle item for display mode DMode
*/
QCanvasItem* QxGraph_Prs::addRectangleItem(QRect theRect, int theDMode)
{
  QCanvasRectangle* aRectItem;
  if ( myCanvas )
  {
    QCanvasRectangle* aRectItem = new QCanvasRectangle(theRect, myCanvas);
    aRectItem->setZ(0);
    aRectItem->show();
    myCanvas->update();
    
    // test drawing features: brush, pen ...
    QBrush aBr(SUIT_Session::session()->resourceMgr()->colorValue( "QxGraph", "NodeBody", RECTANGLE_BODY ));
    aRectItem->setBrush(aBr);
  }
  addItem(aRectItem);
  return aRectItem;
}

/*!
  Add a QCanvasPolygon item for display mode DMode
*/
QCanvasItem* QxGraph_Prs::addPolygonItem(QPointArray thePA, int theDMode)
{
  QCanvasPolygon* aPolyItem;
  if ( myCanvas )
  {
    aPolyItem = new QCanvasPolygon(myCanvas);
    aPolyItem->setZ(0);
    aPolyItem->setPoints(thePA);
    aPolyItem->show();
    myCanvas->update();
    
    // test drawing features: brush, pen ...
    QBrush aBr(SUIT_Session::session()->resourceMgr()->colorValue( "QxGraph", "NodeBody", RECTANGLE_BODY ));
    aPolyItem->setBrush(aBr);
    QPen aPen(Qt::black,2);
    aPolyItem->setPen(aPen);
  }
  addItem(aPolyItem);
  return aPolyItem;
}

/*!
  Add a QCanvasLine item for display mode DMode
*/
QCanvasItem* QxGraph_Prs::addLineItem(QPoint theStart, QPoint theEnd, int theDMode)
{
  QCanvasLine* aLineItem;
  if ( myCanvas )
  {
    aLineItem = new QCanvasLine(myCanvas);
    aLineItem->setZ(0);
    aLineItem->setPoints(theStart.x(), theStart.y(), theEnd.x(), theEnd.y());
    aLineItem->show();
    myCanvas->update();
  
    // test drawing features: brush, pen ...
    QPen aPen(Qt::black,2);
    aLineItem->setPen(aPen);
  }
  addItem(aLineItem);
  return aLineItem;
}

/*!
  Add a QCanvasEllipse item for display mode DMode
*/
QCanvasItem* QxGraph_Prs::addEllipseItem(int theW, int theH, int theStartAngle, int theAngle, int theDMode)
{
  QCanvasEllipse* aEllipseItem;
  if ( myCanvas )
  {
    aEllipseItem = new QCanvasEllipse(theW, theH, theStartAngle, theAngle, myCanvas);
    aEllipseItem->setZ(0);
    aEllipseItem->show();
    myCanvas->update();
    
    // test drawing features: brush, pen ...
    QBrush aBr(SUIT_Session::session()->resourceMgr()->colorValue( "QxGraph", "NodeBody", RECTANGLE_BODY ));
    aEllipseItem->setBrush(aBr);
    QPen aPen(Qt::black,2);
    aEllipseItem->setPen(aPen);
  }
  addItem(aEllipseItem);
  return aEllipseItem;
}

/*!
  Add a QCanvasText item for display mode DMode
*/
QCanvasItem* QxGraph_Prs::addTextItem(QString theText, int theDMode)
{
  QCanvasText* aTextItem;
  if ( myCanvas )
  {
    aTextItem = new QCanvasText(theText, myCanvas);
    aTextItem->setZ(0);
    aTextItem->show();
    myCanvas->update();
    
    // test drawing features: font, color, text flags ...
    aTextItem->setColor(Qt::darkBlue);
    //aTextItem->setFont(QFont("Times"/*"Helvetica"*/, 14, QFont::Normal, true));
  }
  addItem(aTextItem);
  return aTextItem;
}
