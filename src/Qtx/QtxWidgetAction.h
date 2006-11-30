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
// File:      QtxWidgetAction.h
// Author:    Sergey LITONIN

#ifndef QTXWIDGETACTION_H
#define QTXWIDGETACTION_H

#include "QtxAction.h"

/*
  Class       : QtxWidgetAction
  Description : Action having look and fill of arbitrary widget
*/

class QTX_EXPORT QtxWidgetAction : public QtxAction
{
  Q_OBJECT

public:

  QtxWidgetAction( QObject* theParent = 0, const char* theName = 0, bool theIsToggle = false );
  virtual ~QtxWidgetAction();

  virtual bool   addTo( QWidget* );
  virtual bool   addTo( QWidget*, const int );
  virtual bool   removeFrom( QWidget* );

  void           setWidget( QWidget* );
  QWidget*       widget() const;

public slots:
    virtual void setEnabled( bool );
    virtual void setVisible( bool );

private:

  QWidget* myWg;
};

#endif
