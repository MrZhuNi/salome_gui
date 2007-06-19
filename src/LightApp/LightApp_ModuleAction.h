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
// File   : LightApp_ModuleAction.h
// Author : Sergey TELKOV, Vadim SANDLER
//

#ifndef LIGHTAPP_MODULEACTION_H
#define LIGHTAPP_MODULEACTION_H

#include "LightApp.h"

#include <QtxActionSet.h>

#ifdef WIN32
#pragma warning ( disable:4251 )
#endif

class QtxComboBox;

class LIGHTAPP_EXPORT LightApp_ModuleAction : public QtxActionSet
{
  Q_OBJECT

public:
  LightApp_ModuleAction( const QString&, QObject* = 0 );
  LightApp_ModuleAction( const QString&, const QIcon&, QObject* = 0 );
  virtual ~LightApp_ModuleAction();

  QStringList      modules() const;

  QIcon            moduleIcon( const QString& ) const;
  void             setModuleIcon( const QString&, const QIcon& );

  void             insertModule( const QString&, const QIcon&, const int = -1 );
  void             removeModule( const QString& );

  QString          activeModule() const;
  void             setActiveModule( const QString& );

signals:
  void             moduleActivated( const QString& );

protected:
  virtual QWidget* createWidget( QWidget* );
  virtual bool     isEmptyAction() const;

private:
  void             init();

  QAction*         moduleAction( const QString& ) const;
  int              moduleId( const QString& ) const;

  void             update();
  void             update( QtxComboBox* );

  QAction*         active() const;
  void             activate( int, bool = true );

private slots:
  void             onTriggered( int );
  void             onComboActivated( int );
};

#endif // LIGHTAPP_MODULEACTION_H
