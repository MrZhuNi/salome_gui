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
//  SALOME Plot3d : implementation of desktop and GUI kernel
//
//  File   : Plot3d_SetupSurfacesDlg.h
//  Author : Sergey LITONIN

#ifndef PLOT3D_SETUPSURFACESDLG_H
#define PLOT3D_SETUPSURFACESDLG_H

#include "SVTK.h"

#include <QtxDialog.h>
#include <QVector>
#include <QList>

class QtxGroupBox;
class QTableWidget;
class QwtLegend;
class QToolButton;

#if defined WIN32
#pragma warning( disable: 4251 )
#endif

/*!
  \class Plot3d_SetupSurfacesDlg
  Dialog box for setup Plot3d surfaces parameters 
*/

class SVTK_EXPORT Plot3d_SetupSurfacesDlg : public QtxDialog
{ 
  Q_OBJECT

public:
  Plot3d_SetupSurfacesDlg( QWidget* theParent = 0 );
  virtual ~Plot3d_SetupSurfacesDlg();

  void                      SetParameters( const QVector< QString >& theTexts );
  void                      GetParameters( QVector< QString >& theText ) const;

  const QList< int >&       GetRemovedIndexes() const;

private slots:

  void                      onRemove();

private:

  void                      setText( const int theRow,
                                     const int theCol,
                                     const QString& theText );
private:

  QtxGroupBox*              myGrp;
  QTableWidget*             myTable;
  QToolButton*              myRemoveBtn;
  QList< int >              myRemovedIndexes;
};

#endif 
