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
#ifndef PLOT3D_FITDATADLG_H
#define PLOT3D_FITDATADLG_H

#include "Plot3d.h"

#include <QDialog>

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

class PLOT3D_EXPORT Plot3d_FitDataDlg : public QDialog
{
  Q_OBJECT

public:
  Plot3d_FitDataDlg( QWidget* theParent, bool theIs3D );

  void setAxisTitles( const QString& theXTitle,
                      const QString& theYTitle,
                      const QString& theZTitle );

  void setRange( const bool theIsEnabled,
                 const double xMin,
                 const double xMax,
                 const double yMin,
                 const double yMax,
                 const double zMin = 0,
                 const double zMax = 0 );

  void getRange( bool& theIsEnabled,
                 double& xMin,
                 double& xMax,
                 double& yMin,
                 double& yMax,
                 double& zMin,
                 double& zMax );

private:
  QGroupBox*              myRangeGrp;

  QLabel*                 myXLabel;
  QLineEdit*              myXMinEdit;
  QLineEdit*              myXMaxEdit;

  QLabel*                 myYLabel;
  QLineEdit*              myYMinEdit;
  QLineEdit*              myYMaxEdit;

  QLabel*                 myZLabel;
  QLineEdit*              myZMinEdit;
  QLineEdit*              myZMaxEdit;

  QPushButton*            myOkBtn;
  QPushButton*            myCancelBtn;

  bool                    myIs3D;
};

#endif
