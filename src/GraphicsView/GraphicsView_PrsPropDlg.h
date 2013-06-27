// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#ifndef GRAPHICSVIEW_PRSPROPDLG_H
#define GRAPHICSVIEW_PRSPROPDLG_H

#include "GraphicsView.h"

#include <QtxDialog.h>

class QtxDoubleSpinBox;

/*
  Class       : GraphicsView_PrsPropDlg
  Description : Dialog for managing presentation properties
*/
class GRAPHICSVIEW_API GraphicsView_PrsPropDlg : public QtxDialog
{
  Q_OBJECT

public:
  GraphicsView_PrsPropDlg( QWidget* = 0 );
  virtual ~GraphicsView_PrsPropDlg();

public:
  void                     setData( const double thePositionX,
                                    const double thePositionY,
                                    const double theScalingX,
                                    const double theScalingY,
                                    const double theRotationAngle );

  void                     getData( double& thePositionX,
                                    double& thePositionY,
                                    double& theScalingX,
                                    double& theScalingY,
                                    double& theRotationAngle ) const;

private:
  QtxDoubleSpinBox*        myPositionX;
  QtxDoubleSpinBox*        myPositionY;
  QtxDoubleSpinBox*        myScalingX;
  QtxDoubleSpinBox*        myScalingY;
  QtxDoubleSpinBox*        myRotationAngle;
};

#endif
