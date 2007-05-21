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
// File:      QtxWorkspaceAction.h
// Author:    Sergey TELKOV

#ifndef QTXWORKSPACEACTION_H
#define QTXWORKSPACEACTION_H

#include "QtxActionSet.h"

class QMenu;
class QtxWorkspace;

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

class QTX_EXPORT QtxWorkspaceAction : public QtxActionSet
{
  Q_OBJECT

public:
  enum { Cascade    = 0x0001,
         Tile       = 0x0002,
         VTile      = 0x0004,
         HTile      = 0x0008,
         Windows    = 0x0010,
         Standard   = Cascade | Tile | Windows,
         Operations = Cascade | Tile | VTile | HTile,
         All        = Standard | HTile | VTile };

public:
  QtxWorkspaceAction( QtxWorkspace*, QObject* = 0 );
  virtual ~QtxWorkspaceAction();

  QtxWorkspace* workspace() const;

  int           items() const;
  void          setItems( const int );
  bool          hasItems( const int ) const;

  QIcon         icon( const int ) const;
  QString       text( const int ) const;
  int           accel( const int ) const;
  QString       statusTip( const int ) const;

  void          setAccel( const int, const int );
  void          setIcon( const int, const QIcon& );
  void          setText( const int, const QString& );
  void          setStatusTip( const int, const QString& );

  void          perform( const int );

public slots:
  void          tile();
  void          cascade();
  void          tileVertical();
  void          tileHorizontal();

private slots:
  void          onTriggered( int );
  void          onAboutToShow();
  void          onItemActivated( int );

protected:
  virtual void  addedTo( QWidget* );
  virtual void  removedFrom( QWidget* );

private:
  void          updateContent();
  void          updateWindows();

private:
  int           myFlags;
  QtxWorkspace* myWorkspace;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
