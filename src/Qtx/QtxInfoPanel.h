// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

// File   : InfoPanel.h
// Author : Viktor UZLOV, Open CASCADE S.A.S. (viktor.uzlov@opencascade.com)
//
#ifndef QTXINFOPANEL_H
#define QTXINFOPANEL_H

#include "Qtx.h"

#include <QLayout>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

class QAction;

class QTX_EXPORT QtxInfoPanel : public QWidget
{
  Q_OBJECT

public:
  QtxInfoPanel( QWidget* = 0 );
  ~QtxInfoPanel();
  int                 addLabel( const QString&, Qt::Alignment = Qt::AlignLeft, const int = -1 );
  int                 addAction( QAction* action, const int = -1 );
  int                 addGroup( const QString&, const int = -1 );
  void                setVisible( const int , bool, const int = -1 );
  void                setEnabled( const int , bool, const int = -1 );
  void                remove( const int, const int = -1);
  void                clear(const int = -1);

private:
  void                alignLeft( QLayout* lay );
  QAction*            getAction( const int, const int = -1 );
  QToolBar*           getToolBar( const int );  
  int                 generateId() const;

  QToolBar*           tbar;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif // QTXINFOPANEL_H
