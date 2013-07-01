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

  QGroupBox* aPropGroup = new QGroupBox( aMainFrame );

  QLabel* aPositionXLabel = new QLabel( tr( "POSITION_X" ), aPropGroup );
  myPositionX = new QtxDoubleSpinBox( -1e6, 1e6, 1, aPropGroup );

  QLabel* aPositionYLabel = new QLabel( tr( "POSITION_Y" ), aPropGroup );
  myPositionY = new QtxDoubleSpinBox( -1e6, 1e6, 1, aPropGroup );

  QLabel* aScalingXLabel = new QLabel( tr( "SCALING_X" ), aPropGroup );
  myScalingX = new QtxDoubleSpinBox( 1e-6, 1e6, 1, aPropGroup );

  QLabel* aScalingYLabel = new QLabel( tr( "SCALING_Y" ), aPropGroup );
  myScalingY = new QtxDoubleSpinBox( 1e-6, 1e6, 1, aPropGroup );

  QLabel* aRotationAngleLabel = new QLabel( tr( "ROTATION_ANGLE" ), aPropGroup );
  myRotationAngle = new QtxDoubleSpinBox( -1e6, 1e6, 1, aPropGroup );

  QLabel* anOpacityLabel = new QLabel( tr( "OPACITY" ), aPropGroup );
  myOpacity = new QtxDoubleSpinBox( 0, 1, 0.1, aPropGroup );

  QLabel* aZValueLabel = new QLabel( tr( "Z_VALUE" ), aPropGroup );
  myZValue = new QtxDoubleSpinBox( -1e6, 1e6, 1, aPropGroup );

  myIsLockAspectRatio = new QCheckBox( tr( "LOCK_ASPECT_RATIO" ), aPropGroup );

  QGridLayout* aPropLayout = new QGridLayout( aPropGroup );
  aPropLayout->setMargin( 5 );
  aPropLayout->setSpacing( 5 );
  aPropLayout->addWidget( aPositionXLabel,     0, 0 );
  aPropLayout->addWidget( myPositionX,         0, 1 );
  aPropLayout->addWidget( aPositionYLabel,     1, 0 );
  aPropLayout->addWidget( myPositionY,         1, 1 );
  aPropLayout->addWidget( aScalingXLabel,      2, 0 );
  aPropLayout->addWidget( myScalingX,          2, 1 );
  aPropLayout->addWidget( aScalingYLabel,      3, 0 );
  aPropLayout->addWidget( myScalingY,          3, 1 );
  aPropLayout->addWidget( aRotationAngleLabel, 4, 0 );
  aPropLayout->addWidget( myRotationAngle,     4, 1 );
  aPropLayout->addWidget( aZValueLabel,        5, 0 );
  aPropLayout->addWidget( myZValue,            5, 1 );
  aPropLayout->addWidget( anOpacityLabel,      6, 0 );
  aPropLayout->addWidget( myOpacity,           6, 1 );
  aPropLayout->addWidget( myIsLockAspectRatio, 7, 0, 1, 2 );
  aPropLayout->setColumnStretch( 1, 1 );

  QVBoxLayout* aMainLayout = new QVBoxLayout( aMainFrame );
  aMainLayout->setMargin( 5 );
  aMainLayout->setSpacing( 5 );
  aMainLayout->addWidget( aPropGroup );

  setButtonPosition( Center, OK );
  setButtonPosition( Center, Cancel );
  setMinimumWidth( 300 );
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
                                       const bool theIsLockAspectRatio )
{
  myPositionX->setValue( thePositionX );
  myPositionY->setValue( thePositionY );
  myScalingX->setValue( theScalingX );
  myScalingY->setValue( theScalingY );
  myRotationAngle->setValue( theRotationAngle );
  myZValue->setValue( theZValue );
  myOpacity->setValue( theOpacity );
  myIsLockAspectRatio->setChecked( theIsLockAspectRatio );
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
                                       bool& theIsLockAspectRatio ) const
{
  thePositionX = myPositionX->value();
  thePositionY = myPositionY->value();
  theScalingX = myScalingX->value();
  theScalingY = myScalingY->value();
  theRotationAngle = myRotationAngle->value();
  theZValue = myZValue->value();
  theOpacity = myOpacity->value();
  theIsLockAspectRatio = myIsLockAspectRatio->isChecked();
}
