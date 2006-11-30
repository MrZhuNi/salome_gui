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
// File:      QtxWidgetAction.ñxx
// Author:    Sergey LITONIN

#include "QtxWidgetAction.h"

#include <qwidget.h>

/*
  Class       : QtxWidgetAction
  Description : Action having look and fill of arbitrary widget
*/

//================================================================
// Function : QtxWidgetAction
// Purpose  : 
//================================================================
QtxWidgetAction::QtxWidgetAction( QObject* theParent, 
                                                  const char* theName, 
                                                  bool theIsToggle )
: QtxAction( theParent, theName, theIsToggle ),
  myWg( 0 )
{
}

QtxWidgetAction::~QtxWidgetAction()
{
  if ( myWg && !myWg->parent() )
    delete myWg;
}

/*!
  addTo
  redefined to reparent action
*/
bool QtxWidgetAction::addTo( QWidget* theParent )
{
  if ( myWg )
    myWg->reparent( theParent, QPoint(), isVisible() );
  
  //return QtxAction::addTo( theParent );
  return true;
}

/*!
  addTo
  redefined to reparent action
*/
bool QtxWidgetAction::addTo( QWidget* theParent, const int theIndex )
{
  if ( myWg )
    myWg->reparent( theParent, QPoint(), isVisible() );
  return true;
}

/*!
  removeFrom
  redefined to remove widget action from parent
*/
bool QtxWidgetAction::removeFrom( QWidget* theParent )
{
  if ( myWg )
    myWg->reparent( 0, QPoint(), false );
  return QtxAction::removeFrom( theParent );
}

/*!
  setEnabled
  enable/disbale widget of action
*/
void QtxWidgetAction::setEnabled( bool theState )
{
  QtxAction::setEnabled( theState );
  if ( myWg )
    myWg->setEnabled( theState );
}

/*!
  setVisible
  enable/disbale widget of action
*/
void QtxWidgetAction::setVisible( bool theState )
{
  QtxAction::setVisible( theState );
  if ( myWg )
    myWg->setShown( parent() && theState );
}

/*!
  setWidget
  sets widget to action
*/
void QtxWidgetAction::setWidget( QWidget* theWg )
{
  myWg = theWg;
}

/*!
  widget
  \return widget of action
*/
QWidget* QtxWidgetAction::widget() const
{
  return myWg;
}

