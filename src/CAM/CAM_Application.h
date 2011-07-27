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

#ifndef CAM_APPLICATION_H
#define CAM_APPLICATION_H

#include "CAM.h"

#include <STD_Application.h>
#include <QList>

class QMenu;
class CAM_Module;

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

class CAM_EXPORT CAM_Application : public STD_Application  
{
  Q_OBJECT

public:
  typedef QList<CAM_Module*> ModuleList;

public:
  CAM_Application( const bool = true );
  virtual ~CAM_Application();

  virtual void        start();

  CAM_Module*         activeModule() const;
  CAM_Module*         module( const QString& ) const;

  ModuleList          modules() const;
  void                modules( ModuleList& ) const;
  void                modules( QStringList&, const bool loaded = true ) const;

  virtual void        addModule( CAM_Module* );

  virtual void        loadModules();
  virtual CAM_Module* loadModule( const QString&, const bool = true );

  virtual bool        activateModule( const QString& );

  virtual void        contextMenuPopup( const QString&, QMenu*, QString& );

  QString             moduleName( const QString& ) const;
  QString             moduleTitle( const QString& ) const;
  QString             moduleIcon( const QString& ) const;
  bool                isModuleAccessible( const QString& ) const;

  virtual void        createEmptyStudy();

protected:
  virtual SUIT_Study* createNewStudy();
  virtual void        updateCommandsStatus();

  virtual void        moduleAdded( CAM_Module* );
  virtual void        beforeCloseDoc( SUIT_Study* );
  virtual void        afterCloseDoc();
  virtual bool        activateModule( CAM_Module* = 0 );

  virtual void        setActiveStudy( SUIT_Study* );

  QString             moduleLibrary( const QString&, const bool = true ) const;

private:
  void                readModuleList();

private:
  typedef struct { QString name, title, internal, icon; bool isSingleton; } ModuleInfo;
  typedef QList<ModuleInfo> ModuleInfoList;

private:
  CAM_Module*         myModule;        //!< active module
  ModuleList          myModules;       //!< loaded modules list
  ModuleInfoList      myInfoList;      //!< modules info list
  bool                myAutoLoad;      //!< auto loading flag
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
