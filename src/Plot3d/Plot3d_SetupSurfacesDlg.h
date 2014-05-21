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
#ifndef PLOT3D_SETUPSURFACESDLG_H
#define PLOT3D_SETUPSURFACESDLG_H

#include "Plot3d.h"

#include "Plot3d_SetupColorScaleDlg.h"

#include <QtxDialog.h>
#include <QVector>
#include <QList>

class QtxGroupBox;
class QTableWidget;
class QwtLegend;
class QToolButton;

/*
  Class       : Plot3d_SetupSurfacesDlg
  Description : Dialog box for setup Plot3d surfaces parameters
*/
class PLOT3D_EXPORT Plot3d_SetupSurfacesDlg : public QtxDialog
{ 
  Q_OBJECT

public:
  Plot3d_SetupSurfacesDlg( QWidget* theParent = 0 );
  virtual ~Plot3d_SetupSurfacesDlg();

  void                      SetParameters( const QStringList& theTexts,
                                           const ColorDicDataList& theColorDicDataList );
  void                      GetParameters( QStringList& theTexts,
                                           ColorDicDataList& theColorDicDataList ) const;

  const QList< int >&       GetRemovedIndexes() const;

private slots:
  void                      onRemove();
  void                      onColorScaleBtn();

private:
  void                      setText( const int theRow,
                                     const int theCol,
                                     const QString& theText );
private:

  QtxGroupBox*              myGrp;
  QTableWidget*             myTable;
  QToolButton*              myRemoveBtn;

  ColorDicDataList          myColorDicDataList;

  QList< int >              myRemovedIndexes;
};

#endif 
