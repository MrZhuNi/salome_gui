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
#include "Plot3d_FitDataDlg.h"

#include "Plot3d_ViewWindow.h"

#include <QLabel>
#include <QLayout>
#include <QValidator>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QLineEdit>

#define SPACING_SIZE      6
#define MARGIN_SIZE       11
#define MIN_EDIT_SIZE     100

/*!
  Constructor 
*/
Plot3d_FitDataDlg::Plot3d_FitDataDlg( QWidget* theParent, bool theIs3D, int the2DNormalAxis )
: QDialog( theParent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint )
{
  myIsEnableX = theIs3D || the2DNormalAxis != Plot3d_ViewWindow::AxisX;
  myIsEnableY = theIs3D || the2DNormalAxis != Plot3d_ViewWindow::AxisY;
  myIsEnableZ = theIs3D || the2DNormalAxis != Plot3d_ViewWindow::AxisZ;

  setModal( true );
  setWindowTitle( tr( "FIT_DATA_TLT" ) );
  setSizeGripEnabled( true );

  QGridLayout* aTopLayout = new QGridLayout( this ); 
  aTopLayout->setSpacing( SPACING_SIZE );
  aTopLayout->setMargin( MARGIN_SIZE );

  myRangeGrp = new QGroupBox( tr( "ENABLE" ), this );
  myRangeGrp->setCheckable( true );
  myRangeGrp->setChecked( false );
  QGridLayout* aGridLayout = new QGridLayout( myRangeGrp );
  myRangeGrp->setLayout( aGridLayout );
  aGridLayout->setAlignment( Qt::AlignTop );
  aGridLayout->setMargin( MARGIN_SIZE );
  aGridLayout->setSpacing( SPACING_SIZE );

  QDoubleValidator* aValidator = new QDoubleValidator( this );

  if( myIsEnableX )
  {
    myXMinEdit = new QLineEdit( myRangeGrp );
    myXMinEdit->setValidator( aValidator );
    myXMinEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    myXMinEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
    myXMinEdit->setText( "0.0" );

    myXMaxEdit = new QLineEdit( myRangeGrp );
    myXMaxEdit->setValidator( aValidator );
    myXMaxEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    myXMaxEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
    myXMaxEdit->setText( "0.0" );
  }

  if( myIsEnableY )
  {
    myYMinEdit = new QLineEdit( myRangeGrp );
    myYMinEdit->setValidator( aValidator );
    myYMinEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    myYMinEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
    myYMinEdit->setText( "0.0" );

    myYMaxEdit = new QLineEdit( myRangeGrp );
    myYMaxEdit->setValidator( aValidator );
    myYMaxEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    myYMaxEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
    myYMaxEdit->setText( "0.0" );
  }

  if( myIsEnableZ )
  {
    myZMinEdit = new QLineEdit( myRangeGrp );
    myZMinEdit->setValidator( aValidator );
    myZMinEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    myZMinEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
    myZMinEdit->setText( "0.0" );

    myZMaxEdit = new QLineEdit( myRangeGrp );
    myZMaxEdit->setValidator( aValidator );
    myZMaxEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    myZMaxEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
    myZMaxEdit->setText( "0.0" );
  }

  myXLabel = myYLabel = myZLabel = 0;

  if( myIsEnableX )
  {
    myXLabel = new QLabel( tr( "X" ), myRangeGrp );
    QFont font = myXLabel->font(); font.setBold( true );
    myXLabel->setFont( font );

    aGridLayout->addWidget( myXLabel,       0,    0 );
    aGridLayout->addWidget( new QLabel( tr( "MIN_VALUE_LAB" ), myRangeGrp ), 
                                            0,    1 );
    aGridLayout->addWidget( myXMinEdit,     0,    2 );
    aGridLayout->addWidget( new QLabel( tr( "MAX_VALUE_LAB" ), myRangeGrp ), 
                                            0,    3 );
    aGridLayout->addWidget( myXMaxEdit,     0,    4 );
  }

  if( myIsEnableY )
  {
    myYLabel = new QLabel( tr( "Y" ), myRangeGrp );
    QFont font = myYLabel->font(); font.setBold( true );
    myYLabel->setFont( font );

    aGridLayout->addWidget( myYLabel,       1,    0 );
    aGridLayout->addWidget( new QLabel( tr( "MIN_VALUE_LAB" ), myRangeGrp ), 
                                            1,    1 );
    aGridLayout->addWidget( myYMinEdit,     1,    2 );
    aGridLayout->addWidget( new QLabel( tr( "MAX_VALUE_LAB" ), myRangeGrp ), 
                                            1,    3 );
    aGridLayout->addWidget( myYMaxEdit,     1,    4 );
  }

  if( myIsEnableZ )
  {
    myZLabel = new QLabel( tr( "Z" ), myRangeGrp );
    QFont font = myZLabel->font(); font.setBold( true );
    myZLabel->setFont( font );

    aGridLayout->addWidget( myZLabel,       2,    0 );
    aGridLayout->addWidget( new QLabel( tr( "MIN_VALUE_LAB" ), myRangeGrp ), 
                                            2,    1 );
    aGridLayout->addWidget( myZMinEdit,     2,    2 );
    aGridLayout->addWidget( new QLabel( tr( "MAX_VALUE_LAB" ), myRangeGrp ), 
                                            2,    3 );
    aGridLayout->addWidget( myZMaxEdit,     2,    4 );
  }

  myOkBtn = new QPushButton( tr( "BUT_OK" ), this );
  myOkBtn->setAutoDefault( true );
  myOkBtn->setDefault( true );

  myCancelBtn = new QPushButton(  tr( "BUT_CANCEL" ), this );
  myCancelBtn->setAutoDefault( true );

  aTopLayout->addWidget( myRangeGrp, 0, 0, 1, 3 );
  aTopLayout->addWidget( myOkBtn, 1, 0 );
  aTopLayout->setColumnStretch( 1, 5 );
  aTopLayout->addWidget( myCancelBtn, 1, 2 );

  // connect signals
  connect( myOkBtn,     SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( myCancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*!
  Sets axis titles
*/
void Plot3d_FitDataDlg::setAxisTitles( const QString& theXTitle,
                                       const QString& theYTitle,
                                       const QString& theZTitle )
{
  if( myIsEnableX )
    myXLabel->setText( theXTitle );
  if( myIsEnableY )
    myYLabel->setText( theYTitle );
  if( myIsEnableZ )
    myZLabel->setText( theZTitle );
}

/*!
  Sets range
*/
void Plot3d_FitDataDlg::setRange( const bool theIsEnabled,
                                  const double xMin, 
                                  const double xMax,
                                  const double yMin,
                                  const double yMax,
                                  const double zMin,
                                  const double zMax ) 
{
  myRangeGrp->setChecked( theIsEnabled );
  if( myIsEnableX )
  {
    myXMinEdit->setText( QString::number( xMin ) );
    myXMaxEdit->setText( QString::number( xMax ) );
  }
  if( myIsEnableY )
  {
    myYMinEdit->setText( QString::number( yMin ) );
    myYMaxEdit->setText( QString::number( yMax ) );
  }
  if( myIsEnableZ )
  {
    myZMinEdit->setText( QString::number( zMin ) );
    myZMaxEdit->setText( QString::number( zMax ) );
  }
}

/*!
  Gets range
*/
void Plot3d_FitDataDlg::getRange( bool& theIsEnabled,
                                  double& xMin, 
                                  double& xMax,
                                  double& yMin,
                                  double& yMax,
                                  double& zMin,
                                  double& zMax )
{
  theIsEnabled = myRangeGrp->isChecked();
  xMin = myIsEnableX ? myXMinEdit->text().toDouble() : 0;
  xMax = myIsEnableX ? myXMaxEdit->text().toDouble() : 0;
  yMin = myIsEnableY ? myYMinEdit->text().toDouble() : 0;
  yMax = myIsEnableY ? myYMaxEdit->text().toDouble() : 0;
  zMin = myIsEnableZ ? myZMinEdit->text().toDouble() : 0;
  zMax = myIsEnableZ ? myZMaxEdit->text().toDouble() : 0;
}
