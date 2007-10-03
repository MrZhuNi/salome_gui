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
//  SALOME Plot2d : implementation of desktop and GUI kernel
//
//  File   : Plot2d_SetupCurveDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "Plot2d_SetupCurveDlg.h"
#include "Plot2d_ViewFrame.h"
#include "SUIT_Tools.h"
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qtoolbutton.h>
#include <qgroupbox.h>
#include <qcolordialog.h>
#include <qlineedit.h>
#include <qlistbox.h>

#ifndef WNT
using namespace std;
#endif

#define MARGIN_SIZE      11
#define SPACING_SIZE     6
#define MIN_COMBO_WIDTH  100
#define MIN_SPIN_WIDTH   50
#define MAX_LINE_WIDTH   100

/*!
  Constructor
*/
Plot2d_SetupCurveDlg::Plot2d_SetupCurveDlg( QWidget* parent )
: QtxDialog( parent, "Plot2d_SetupCurveDlg", true, true, OK  | Cancel,
            WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu ),
 myCurrent( -1 ),
 myCurveBox( 0 )
{
  createControls( 0 );
}

/*!
  Constructor
*/
Plot2d_SetupCurveDlg::Plot2d_SetupCurveDlg( Plot2d_ViewFrame* vf, QWidget* parent )
: QtxDialog( parent, "Plot2d_SetupCurveDlg", true, true, OK  | Cancel,
            WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu ),
 myCurrent( -1 ),
 myCurveBox( 0 )
{
  createControls( vf );
}

/*!
  Destructor
*/
Plot2d_SetupCurveDlg::~Plot2d_SetupCurveDlg()
{
}

/*!
  createControls
*/
void Plot2d_SetupCurveDlg::createControls( Plot2d_ViewFrame* vf )
{
  if ( vf )
    setCaption( tr("TOT_PLOT2D_EDIT_LEGEND") );
  else
    setCaption( tr("TLT_SETUP_CURVE") );
  setSizeGripEnabled( TRUE );

  QFrame* main = mainFrame();
  QVBoxLayout* layout = new QVBoxLayout( main, 0, 5 );
  QGroupBox* grp = new QGroupBox( 1, Qt::Vertical, "", main );
  grp->setFrameStyle( QFrame::NoFrame );
  grp->setInsideMargin( 0 );
  layout->addWidget( grp );

  if ( vf )
  {
    QGroupBox* crvGrp = new QGroupBox( 1, Qt::Horizontal, tr( "CURVES" ), grp );
    crvGrp->setInsideMargin( MARGIN_SIZE );
    myCurveBox = new QListBox( crvGrp );
    myCurveBox->setSelectionMode( QListBox::NoSelection );
    connect( myCurveBox, SIGNAL( currentChanged( QListBoxItem* ) ), this, SLOT( onCurveChanged() ) );
  }

  QGroupBox* TopGroup = new QGroupBox( 1, Qt::Horizontal, tr( "CURVE_PROPS" ), grp );
  TopGroup->setInsideMargin( MARGIN_SIZE );

  TopGroup->setColumnLayout( 0, Qt::Vertical );
  TopGroup->layout()->setSpacing( 0 ); TopGroup->layout()->setMargin( 0 );
  QGridLayout* TopGroupLayout = new QGridLayout( TopGroup->layout() );
  TopGroupLayout->setAlignment( Qt::AlignTop );
  TopGroupLayout->setSpacing( SPACING_SIZE ); TopGroupLayout->setMargin( MARGIN_SIZE );

  QLabel* nameLab = new QLabel( tr( "CURVE_NAME" ), TopGroup );
  myName = new QLineEdit( TopGroup );
  
  QLabel* aLineTypeLab = new QLabel( tr( "CURVE_LINE_TYPE_LAB" ), TopGroup );
  myLineCombo = new QComboBox( false, TopGroup );
  myLineCombo->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myLineCombo->setMinimumWidth( MIN_COMBO_WIDTH );
  myLineCombo->insertItem( tr( "NONE_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "SOLID_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DASH_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DOT_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DASHDOT_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DAHSDOTDOT_LINE_LBL" ) );
  myLineCombo->setCurrentItem( 1 ); // SOLID by default

  QLabel* aLineWidthLab = new QLabel( tr( "CURVE_LINE_WIDTH_LAB" ), TopGroup );
  myLineSpin = new QSpinBox( 0, MAX_LINE_WIDTH, 1, TopGroup );
  myLineSpin->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myLineSpin->setMinimumWidth( MIN_SPIN_WIDTH );
  myLineSpin->setValue( 0 );        // default width is 0

  QLabel* aMarkerLab = new QLabel( tr( "CURVE_MARKER_TYPE_LAB" ), TopGroup );
  myMarkerCombo = new QComboBox( false, TopGroup );
  myMarkerCombo->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myMarkerCombo->setMinimumWidth( MIN_COMBO_WIDTH );
  myMarkerCombo->insertItem( tr( "NONE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "CIRCLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "RECTANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "DIAMOND_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "DTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "UTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "LTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "RTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "CROSS_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "XCROSS_MARKER_LBL" ) );
  myMarkerCombo->setCurrentItem( 1 ); // CIRCLE by default

  QLabel* aColorLab = new QLabel( tr( "CURVE_COLOR_LAB" ), TopGroup );
  myColorBtn = new QToolButton( TopGroup );
  myColorBtn->setMinimumSize(25, 25);

  TopGroupLayout->addWidget( nameLab, 0, 0 );
  TopGroupLayout->addMultiCellWidget( myName, 0, 0, 1, 2 );
  TopGroupLayout->addWidget( aLineTypeLab, 1, 0 );
  TopGroupLayout->addMultiCellWidget( myLineCombo, 1, 1, 1, 2 );
  TopGroupLayout->addWidget( aLineWidthLab, 2, 0 );
  TopGroupLayout->addMultiCellWidget( myLineSpin, 2, 2, 1, 2 );
  TopGroupLayout->addWidget( aMarkerLab, 3, 0 );
  TopGroupLayout->addMultiCellWidget( myMarkerCombo, 3, 3, 1, 2 );
  TopGroupLayout->addWidget( aColorLab, 4, 0 );
  TopGroupLayout->addWidget( myColorBtn, 4, 1 );
  TopGroupLayout->setColStretch( 2, 6 );

  setBorderEnabled( true, BottomArea );
	setButtonPosition( Left,  OK  );
	setButtonPosition( Right, Cancel );
  ((QPushButton*)(button( OK )))->setAutoDefault( true );

  connect( myName, SIGNAL( textChanged( const QString& ) ), this, SLOT( onNameChanged( const QString& ) ) );
  connect( myColorBtn,  SIGNAL( clicked() ), this, SLOT( onColorChanged() ) );
  connect( this, SIGNAL( dlgOk() ), this, SLOT( onOk() ) ) ;
  setColor( QColor( 0, 0, 0 ) );

  SUIT_Tools::centerWidget( this, (QWidget*)parent() );

  if ( vf )
    init( vf );
}

/*!
  onOk
*/
void Plot2d_SetupCurveDlg::onOk()
{
  if ( myCurrent != -1 )
    storeCurveProps( myCurrent );
}


/*!
  Sets name of curve
*/
void Plot2d_SetupCurveDlg::setName( const QString txt )
{
  myName->setText( txt );
}

/*!
  Returns name of curve
*/
QString Plot2d_SetupCurveDlg::getName( const int id ) const
{
  if ( id == -1 )
    return myName->text();
  else
    return myCurvesProps[ id ].name;
}

/*!
  Sets line style and width
*/
void Plot2d_SetupCurveDlg::setLine( const int line, const int width )
{
  myLineCombo->setCurrentItem( line );
  myLineSpin->setValue( width );
}

/*!
  Gets line style
*/
int Plot2d_SetupCurveDlg::getLine( const int id ) const
{
  if ( id == -1 )
    return myLineCombo->currentItem();
  else
    return myCurvesProps[ id ].line;
}

/*!
  Gets line width
*/
int Plot2d_SetupCurveDlg::getLineWidth( const int id ) const
{
  if ( id == -1 )
    return myLineSpin->value();
  else
    return myCurvesProps[ id ].width;
}

/*!
  Sets marker style
*/
void Plot2d_SetupCurveDlg::setMarker( const int marker )
{
  myMarkerCombo->setCurrentItem( marker );
}

/*!
  Gets marker style
*/
int Plot2d_SetupCurveDlg::getMarker( const int id ) const 
{
  if ( id == -1 )
    return myMarkerCombo->currentItem();
  else
    return myCurvesProps[ id ].marker;
}

/*!
  Sets color
*/
void Plot2d_SetupCurveDlg::setColor( const QColor& color )
{
  QPalette pal = myColorBtn->palette();
  QColorGroup ca = pal.active();
  ca.setColor( QColorGroup::Button, color );
  QColorGroup ci = pal.inactive();
  ci.setColor( QColorGroup::Button, color );
  pal.setActive( ca );
  pal.setInactive( ci );
  myColorBtn->setPalette( pal );
}

/*!
  Gets color
*/
QColor Plot2d_SetupCurveDlg::getColor( const int id ) const 
{
  if ( id == -1 )
    return myColorBtn->palette().active().button();
  else
    return myCurvesProps[ id ].color;
}

/*!
  <Color> button slot, invokes color selection dialog box
*/
void Plot2d_SetupCurveDlg::onColorChanged()
{
  QColor color = QColorDialog::getColor( getColor() );
  if ( color.isValid() ) {
    setColor( color );
  }
}

/*!
  init dialog by several curves
*/
void Plot2d_SetupCurveDlg::init( Plot2d_ViewFrame* vf )
{
  if ( !vf || !myCurveBox )
    return;

  curveList lst;
  if ( !vf->getCurves( lst ) )
    return;

  curveList::const_iterator crvIt = lst.begin();
  for ( ; crvIt != lst.end(); ++crvIt )
  {
    CurveProp newProp;
    Plot2d_Curve* crv = *crvIt;
    newProp.name = crv->getVerTitle();
    newProp.line = (int)crv->getLine();
    newProp.width = crv->getLineWidth();
    newProp.marker = (int)crv->getMarker();
    newProp.color = crv->getColor();
    myCurvesProps.insert( myCurvesProps.count(), newProp );
    // add curve to list box
    myCurveBox->insertItem( newProp.name );
  }
  setCurrentCurve( 0 );
}

/*!
  Sets current curve property
*/
void Plot2d_SetupCurveDlg::setCurrentCurve( const int id )
{
  if ( myCurrent == id ) return;
  myCurveBox->setCurrentItem( id );
  //retrieveCurveProps( 0 );
  //myCurrent = id;
}

/*!
  getNbCurves, return number of curves
*/
int Plot2d_SetupCurveDlg::getNbCurves() const
{
  return myCurvesProps.count();
}

/*!
  slot, invokes when current curve changed
*/
void Plot2d_SetupCurveDlg::onCurveChanged()
{
  int newId = myCurveBox->currentItem();
  if ( myCurrent == newId )
    return;
  if ( myCurrent != -1 )
    storeCurveProps( myCurrent );
  myCurrent = newId;
  retrieveCurveProps( newId );
}

/*!
  slot, invokes when current curve name changed
*/
void Plot2d_SetupCurveDlg::onNameChanged( const QString& txt )
{
  if ( myCurveBox && myCurrent != -1 )
  {
    myCurveBox->blockSignals( true );
    myCurveBox->changeItem( txt, myCurrent );
    myCurveBox->blockSignals( false );
  }
}

/*!
  storeCurveProps, stores curve properties
*/
bool Plot2d_SetupCurveDlg::storeCurveProps( const int id )
{
  if ( id < 0 || id > (int)myCurvesProps.count() - 1 )
    return false;
  CurveProp& curProp = myCurvesProps[ id ];
  curProp.name   = getName();
  curProp.line   = getLine();
  curProp.width  = getLineWidth();
  curProp.marker = getMarker();
  curProp.color  = getColor();
  return true;
}

/*!
  retrieveCurveProps, retrievs curve properties
*/
void Plot2d_SetupCurveDlg::retrieveCurveProps( const int id )
{
  if ( id < 0 || id > (int)myCurvesProps.count() - 1 )
    return;
  const CurveProp& curProp = myCurvesProps[ id ];
  myName->blockSignals( true );
  myName->setText( curProp.name );
  myName->blockSignals( false );
  //setName( curProp.name );
  setLine( curProp.line, curProp.width );
  setMarker( curProp.marker );
  setColor( curProp.color );
}
