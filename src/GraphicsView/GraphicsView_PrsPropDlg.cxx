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

#include "GraphicsView_PrsPropDlg.h"

#include <QtxDoubleSpinBox.h>

#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>

//================================================================
// Function : GraphicsView_PrsPropDlg
// Purpose  : 
//================================================================
GraphicsView_PrsPropDlg::GraphicsView_PrsPropDlg( QWidget* theParent )
: QtxDialog( theParent, true, true, QtxDialog::OKCancel )
{
  setWindowTitle( tr( "PROPERTIES" ) );

  QFrame* aMainFrame = mainFrame();

  // Geometry
  QGroupBox* aGeomGroup = new QGroupBox( tr( "GEOMETRY" ), aMainFrame );

  QLabel* aPositionXLabel = new QLabel( tr( "POSITION_X" ), aGeomGroup );
  myPositionX = new QtxDoubleSpinBox( -1e6, 1e6, 1, aGeomGroup );

  QLabel* aPositionYLabel = new QLabel( tr( "POSITION_Y" ), aGeomGroup );
  myPositionY = new QtxDoubleSpinBox( -1e6, 1e6, 1, aGeomGroup );

  QLabel* aScalingXLabel = new QLabel( tr( "SCALING_X" ), aGeomGroup );
  myScalingX = new QtxDoubleSpinBox( -1e6, 1e6, 1, aGeomGroup );

  QLabel* aScalingYLabel = new QLabel( tr( "SCALING_Y" ), aGeomGroup );
  myScalingY = new QtxDoubleSpinBox( -1e6, 1e6, 1, aGeomGroup );

  QLabel* aRotationAngleLabel = new QLabel( tr( "ROTATION_ANGLE" ), aGeomGroup );
  myRotationAngle = new QtxDoubleSpinBox( -1e6, 1e6, 1, aGeomGroup );

  QGridLayout* aGeomLayout = new QGridLayout( aGeomGroup );
  aGeomLayout->setMargin( 5 );
  aGeomLayout->setSpacing( 5 );
  aGeomLayout->addWidget( aPositionXLabel,     0, 0 );
  aGeomLayout->addWidget( myPositionX,         0, 1 );
  aGeomLayout->addWidget( aPositionYLabel,     1, 0 );
  aGeomLayout->addWidget( myPositionY,         1, 1 );
  aGeomLayout->addWidget( aScalingXLabel,      2, 0 );
  aGeomLayout->addWidget( myScalingX,          2, 1 );
  aGeomLayout->addWidget( aScalingYLabel,      3, 0 );
  aGeomLayout->addWidget( myScalingY,          3, 1 );
  aGeomLayout->addWidget( aRotationAngleLabel, 4, 0 );
  aGeomLayout->addWidget( myRotationAngle,     4, 1 );
  aGeomLayout->setColumnStretch( 1, 1 );

  // Representation
  QGroupBox* aReprGroup = new QGroupBox( tr( "REPRESENTATION" ), aMainFrame );

  QLabel* aZValueLabel = new QLabel( tr( "Z_VALUE" ), aReprGroup );
  myZValue = new QtxDoubleSpinBox( -1e6, 1e6, 1, aReprGroup );

  QLabel* anOpacityLabel = new QLabel( tr( "OPACITY" ), aReprGroup );
  myOpacity = new QtxDoubleSpinBox( 0, 1, 0.1, aReprGroup );

  QGridLayout* aReprLayout = new QGridLayout( aReprGroup );
  aReprLayout->setMargin( 5 );
  aReprLayout->setSpacing( 5 );
  aReprLayout->addWidget( aZValueLabel,   0, 0 );
  aReprLayout->addWidget( myZValue,       0, 1 );
  aReprLayout->addWidget( anOpacityLabel, 1, 0 );
  aReprLayout->addWidget( myOpacity,      1, 1 );
  aReprLayout->setColumnStretch( 1, 1 );

  // Interaction
  QGroupBox* anInterGroup = new QGroupBox( tr( "INTERACTION" ), aMainFrame );

  myIsLockAspectRatio = new QCheckBox( tr( "LOCK_ASPECT_RATIO" ), anInterGroup );
  myIsSmoothTransformation = new QCheckBox( tr( "SMOOTH_TRANSFORMATION" ), anInterGroup );

  QGridLayout* anInterLayout = new QGridLayout( anInterGroup );
  anInterLayout->setMargin( 5 );
  anInterLayout->setSpacing( 5 );
  anInterLayout->addWidget( myIsLockAspectRatio,      0, 0 );
  anInterLayout->addWidget( myIsSmoothTransformation, 1, 0 );
  anInterLayout->setColumnStretch( 1, 1 );

  QVBoxLayout* aMainLayout = new QVBoxLayout( aMainFrame );
  aMainLayout->setMargin( 5 );
  aMainLayout->setSpacing( 5 );
  aMainLayout->addWidget( aGeomGroup );
  aMainLayout->addWidget( aReprGroup );
  aMainLayout->addWidget( anInterGroup );

  setButtonPosition( Center, OK );
  setButtonPosition( Center, Cancel );
  setMinimumWidth( 250 );
}

//================================================================
// Function : ~GraphicsView_PrsPropDlg
// Purpose  : 
//================================================================
GraphicsView_PrsPropDlg::~GraphicsView_PrsPropDlg()
{
}

//=============================================================================
// Function : setData
// Purpose  : 
//=============================================================================
void GraphicsView_PrsPropDlg::setData( const double thePositionX,
                                       const double thePositionY,
                                       const double theScalingX,
                                       const double theScalingY,
                                       const double theRotationAngle,
                                       const double theZValue,
                                       const double theOpacity,
                                       const bool theIsLockAspectRatio,
                                       const bool theIsSmoothTransformation )
{
  myPositionX->setValue( thePositionX );
  myPositionY->setValue( thePositionY );
  myScalingX->setValue( theScalingX );
  myScalingY->setValue( theScalingY );
  myRotationAngle->setValue( theRotationAngle );
  myZValue->setValue( theZValue );
  myOpacity->setValue( theOpacity );
  myIsLockAspectRatio->setChecked( theIsLockAspectRatio );
  myIsSmoothTransformation->setChecked( theIsSmoothTransformation );
}

//=============================================================================
// Function : getData
// Purpose  : 
//=============================================================================
void GraphicsView_PrsPropDlg::getData( double& thePositionX,
                                       double& thePositionY,
                                       double& theScalingX,
                                       double& theScalingY,
                                       double& theRotationAngle,
                                       double& theZValue,
                                       double& theOpacity,
                                       bool& theIsLockAspectRatio,
                                       bool& theIsSmoothTransformation ) const
{
  thePositionX = myPositionX->value();
  thePositionY = myPositionY->value();
  theScalingX = myScalingX->value();
  theScalingY = myScalingY->value();
  theRotationAngle = myRotationAngle->value();
  theZValue = myZValue->value();
  theOpacity = myOpacity->value();
  theIsLockAspectRatio = myIsLockAspectRatio->isChecked();
  theIsSmoothTransformation = myIsSmoothTransformation->isChecked();
}
