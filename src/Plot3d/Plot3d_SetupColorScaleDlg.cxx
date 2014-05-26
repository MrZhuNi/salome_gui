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
#include "Plot3d_SetupColorScaleDlg.h"

#include "Plot3d_ColorDic.h"
#include "Plot3d_SetupSpecificColorScaleDlg.h"

#include <SUIT_MessageBox.h>

#include <QtxColorButton.h>
#include <QtxGridBox.h>
#include <QtxIntSpinBox.h>

#include <QButtonGroup>
#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>

#include <Precision.hxx>

//=============================================================================
// Function : Plot3d_SetupColorScaleDlg
// Purpose  : Constructor
//=============================================================================
Plot3d_SetupColorScaleDlg::Plot3d_SetupColorScaleDlg( QWidget* theParent,
                                                      bool theIsGlobal )
: QtxDialog( theParent, true, false, QtxDialog::OKCancel ),
  myIsGlobal( theIsGlobal )
{
  setWindowTitle( tr( "SETUP_COLOR_SCALE" ) );

  QFrame* aMainFrame = mainFrame();

  QGroupBox* aColorScaleGroup = new QGroupBox( tr( "COLOR_SCALE" ), aMainFrame );

  QtxGridBox* aScaleModeGroup = new QtxGridBox( 1, Qt::Vertical, aColorScaleGroup );
  aScaleModeGroup->setInsideMargin( 0 );

  QAbstractButton* aScaleModeLinear = new QRadioButton( tr( "LINEAR" ), aScaleModeGroup );
  QAbstractButton* aScaleModeLogarithmic = new QRadioButton( tr( "LOGARITHMIC" ), aScaleModeGroup );
  QAbstractButton* aScaleModeSpecific = new QRadioButton( tr( "SPECIFIC" ), aScaleModeGroup );
  QAbstractButton* aScaleModeSpecificSetup = new QPushButton( tr( "SETUP" ), aScaleModeGroup );
  aScaleModeLinear->setChecked( true );
  aScaleModeSpecificSetup->setEnabled( false );

  myColorScaleMode = new QButtonGroup( aScaleModeGroup );
  myColorScaleMode->setExclusive( true );
  myColorScaleMode->addButton( aScaleModeLinear, Plot3d_ColorDic::Linear );
  myColorScaleMode->addButton( aScaleModeLogarithmic, Plot3d_ColorDic::Logarithmic );
  myColorScaleMode->addButton( aScaleModeSpecific, Plot3d_ColorDic::Specific );

  QWidget* anIntervalGroup = new QWidget( aColorScaleGroup );

  QLabel* anIntervalLabel = new QLabel( tr( "NUMBER_OF_INTERVALS" ), anIntervalGroup );
  myInterval = new QtxIntSpinBox( 2, 99, 1, anIntervalGroup );

  QLabel* aColorsLabel = new QLabel( tr( "COLORS" ), anIntervalGroup );
  myColorMode = new QComboBox( anIntervalGroup );
  myColorMode->addItems( QStringList()
    << tr( "BLUE_RED" )
    << tr( "BLUE_WHITE" )
    << tr( "MONOCHROME" ) 
    << tr( "CUSTOM" ) );

  myBottomColor = new QtxColorButton( anIntervalGroup );
  myTopColor = new QtxColorButton( anIntervalGroup );

  QGridLayout* anIntervalGroupLayout = new QGridLayout( anIntervalGroup );
  anIntervalGroupLayout->setMargin( 0 );
  anIntervalGroupLayout->setSpacing( 5 );
  anIntervalGroupLayout->addWidget( anIntervalLabel, 0, 0 );
  anIntervalGroupLayout->addWidget( myInterval, 0, 1, 1, 3 );
  anIntervalGroupLayout->addWidget( aColorsLabel, 1, 0 );
  anIntervalGroupLayout->addWidget( myColorMode, 1, 1 );
  anIntervalGroupLayout->addWidget( myBottomColor, 1, 2 );
  anIntervalGroupLayout->addWidget( myTopColor, 1, 3 );

  QVBoxLayout* aColorScaleLayout = new QVBoxLayout( aColorScaleGroup );
  aColorScaleLayout->setMargin( 5 );
  aColorScaleLayout->setSpacing( 5 );
  aColorScaleLayout->addWidget( aScaleModeGroup );
  aColorScaleLayout->addWidget( anIntervalGroup );

  QGroupBox* aRangeGroup = new QGroupBox( tr( "RANGE" ), aMainFrame );

  QLabel* aMinLabel = new QLabel( tr( "MIN" ), aRangeGroup );
  myMinLimit = new QDoubleSpinBox( aRangeGroup );
  myMinLimit->setMinimum( -DBL_MAX );
  myMinLimit->setMaximum( DBL_MAX );
  myMinLimit->setKeyboardTracking( false );

  QLabel* aMaxLabel = new QLabel( tr( "MAX" ), aRangeGroup );
  myMaxLimit = new QDoubleSpinBox( aRangeGroup );
  myMaxLimit->setMinimum( -DBL_MAX );
  myMaxLimit->setMaximum( DBL_MAX );
  myMaxLimit->setKeyboardTracking( false );

  myMinLimit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred ) );
  myMaxLimit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred ) );

  myMinLimit->setDecimals( 6 );
  myMaxLimit->setDecimals( 6 );

  QHBoxLayout* aRangeLayout = new QHBoxLayout( aRangeGroup );
  aRangeLayout->setMargin( 5 );
  aRangeLayout->setSpacing( 5 );
  aRangeLayout->addWidget( aMinLabel );
  aRangeLayout->addWidget( myMinLimit );
  aRangeLayout->addWidget( aMaxLabel );
  aRangeLayout->addWidget( myMaxLimit );

  QVBoxLayout* aMainLayout = new QVBoxLayout( aMainFrame );
  aMainLayout->setMargin( 5 );
  aMainLayout->setSpacing( 5 );
  aMainLayout->addWidget( aColorScaleGroup );
  aMainLayout->addWidget( aRangeGroup );

  connect( myMinLimit, SIGNAL( valueChanged( double ) ), this, SLOT( onMinMaxChanged( double ) ) );
  connect( myMaxLimit, SIGNAL( valueChanged( double ) ), this, SLOT( onMinMaxChanged( double ) ) );

  connect( myInterval, SIGNAL( valueChanged( int ) ), this, SLOT( onIntervalNumberChanged( int ) ) );

  connect( myColorMode, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onColorModeChanged( int ) ) );

  connect( myColorScaleMode, SIGNAL( buttonClicked( int ) ), this, SLOT( onColorScaleModeChanged( int ) ) );
  connect( aScaleModeSpecific, SIGNAL( toggled( bool ) ), aScaleModeSpecificSetup, SLOT( setEnabled( bool ) ) );
  connect( aScaleModeSpecificSetup, SIGNAL( clicked() ), this, SLOT( onColorScaleSpecificSetup() ) );

  aScaleModeSpecific->setVisible( !myIsGlobal );
  aScaleModeSpecificSetup->setVisible( !myIsGlobal );
  aRangeGroup->setVisible( !myIsGlobal );

  onColorModeChanged( 0 ); // Blue-Red by default

  setButtonPosition( Right, Cancel );
}

//=============================================================================
// Function : ~Plot3d_SetupColorScaleDlg
// Purpose  : Destructor
//=============================================================================
Plot3d_SetupColorScaleDlg::~Plot3d_SetupColorScaleDlg()
{
}

//=============================================================================
// Function : setData
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::setData( const ColorDicData& theColorDicData )
{
  myMinLimit->setValue( theColorDicData.Min );
  myMaxLimit->setValue( theColorDicData.Max );

  setColorScaleMode( theColorDicData.ScaleMode );

  myInterval->setValue( theColorDicData.Num );

  myColorMode->setCurrentIndex( theColorDicData.ColorMode );

  myBottomColor->setColor( theColorDicData.CustomColors[0] );
  myTopColor->setColor( theColorDicData.CustomColors[1] );

  myMajorSpecificColorScale = theColorDicData.MainSpecificColorScale;
}

//=============================================================================
// Function : getData
// Purpose  : 
//=============================================================================
ColorDicData Plot3d_SetupColorScaleDlg::getData() const
{
  ColorDicData aColorDicData;

  aColorDicData.Min = myMinLimit->value();
  aColorDicData.Max = myMaxLimit->value();

  aColorDicData.ScaleMode = (Plot3d_ColorDic::ScaleMode)colorScaleMode();

  aColorDicData.Num = myInterval->value();

  aColorDicData.ColorMode = myColorMode->currentIndex();

  QColor aBottomColor = myBottomColor->color();
  QColor aTopColor = myTopColor->color();
  aColorDicData.HueMin = aBottomColor.hueF();
  aColorDicData.HueMax = aTopColor.hueF();
  aColorDicData.SaturationMin = aBottomColor.saturationF();
  aColorDicData.SaturationMax = aTopColor.saturationF();
  aColorDicData.ValueMin = aBottomColor.valueF();
  aColorDicData.ValueMax = aTopColor.valueF();
  aColorDicData.CustomColors[0] = aBottomColor;
  aColorDicData.CustomColors[1] = aTopColor;

  aColorDicData.MainSpecificColorScale = myMajorSpecificColorScale;
  aColorDicData.SpecificColorScale = myMinorSpecificColorScale;

  return aColorDicData;
}

//=============================================================================
// Function : onMinMaxChanged
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::onMinMaxChanged( double value )
{
  checkScaleMode();
}

//=============================================================================
// Function : onIntervalNumberChanged
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::onIntervalNumberChanged( int num )
{
  updateColorScale();
}

//=============================================================================
// Function : onColorModeChanged
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::onColorModeChanged( int mode )
{
  bool custom = false;
  QColor bottomColor, topColor;
  switch( mode )
  {
    case Custom:
      custom = true;
      break;
    case BlueWhite:
      bottomColor = Qt::blue;
      topColor = QColor( 128, 255, 255 );
      break;
    case Monochrome:
      bottomColor = Qt::black;
      topColor = QColor( 200, 200, 200 );
      break;
    case BlueRed:
    default:
      bottomColor = Qt::blue;
      topColor = Qt::red;
  }

  myBottomColor->setEnabled( custom );
  myTopColor->setEnabled( custom );

  if( !custom )
  {
    myBottomColor->setColor( bottomColor );
    myTopColor->setColor( topColor );
  }
}

//=============================================================================
// Function : onColorScaleModeChanged
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::onColorScaleModeChanged( int mode )
{
  checkScaleMode();

  updateColorScale();
  updateState();
  updateMinMax();
}

//=============================================================================
// Function : onColorScaleSpecificSetup
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::onColorScaleSpecificSetup()
{
  Plot3d_SetupSpecificColorScaleDlg* aSetupDlg = new Plot3d_SetupSpecificColorScaleDlg( this );
  aSetupDlg->SetMajorScale( myMajorSpecificColorScale );
  
  if( aSetupDlg->exec() )
  {
    myMajorSpecificColorScale = aSetupDlg->GetMajorScale();
    updateColorScale();
    updateState();
    updateMinMax();
  }
  delete aSetupDlg;
}

//=============================================================================
// Function : colorScaleMode
// Purpose  : 
//=============================================================================
int Plot3d_SetupColorScaleDlg::colorScaleMode() const
{
  return myColorScaleMode->checkedId();
}

//=============================================================================
// Function : setColorScaleMode
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::setColorScaleMode( const int theMode )
{
  if( QAbstractButton* aBtn = myColorScaleMode->button( theMode ) )
    aBtn->setChecked( true );
  onColorScaleModeChanged( theMode );
}

//=============================================================================
// Function : checkScaleMode
// Purpose  : 
//=============================================================================
int Plot3d_SetupColorScaleDlg::checkScaleMode()
{
  if ( colorScaleMode() == Plot3d_ColorDic::Logarithmic )
  {
    double aCorrectValue = 1.e-6;
    double aMin = myMinLimit->value();
    double aMax = myMaxLimit->value();
    bool isMinIncorrect = aMin < 0 || fabs( aMin ) < Precision::Confusion();
    bool isMaxIncorrect = aMax < 0 || fabs( aMax ) < Precision::Confusion();
    if ( isMinIncorrect || isMaxIncorrect )
    {
      int anAnswer = 0;
      if( myIsGlobal )
      {
        SUIT_MessageBox::warning( this, tr( "WARNING" ), tr( "INCORRECT_RANGE" ) );
        setColorScaleMode( Plot3d_ColorDic::Linear );
      }
      else
      {
        if ( SUIT_MessageBox::warning( this, tr( "WARNING" ), tr( "INCORRECT_RANGE" ),
                                       tr( "CORRECT_RANGE" ), tr( "SWITCH_TO_LINEAR" ), 0, 1 ) == 0 )
        { // correct range
          if ( isMinIncorrect )
            aMin = aCorrectValue;

          if ( isMaxIncorrect )
            aMax = aCorrectValue;

          bool minBlock = myMinLimit->blockSignals( true );
          bool maxBlock = myMaxLimit->blockSignals( true );

          myMinLimit->setValue( aMin );
          myMaxLimit->setValue( aMax );

          myMinLimit->blockSignals( minBlock );
          myMaxLimit->blockSignals( maxBlock );
        }
        else // switch to linear mode
          setColorScaleMode( Plot3d_ColorDic::Linear );
      }
    }
  }
  return colorScaleMode();
}

//=============================================================================
// Function : updateState
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::updateState()
{
  bool isSpecificColorScale = colorScaleMode() == Plot3d_ColorDic::Specific;

  myMinLimit->setEnabled( !isSpecificColorScale );
  myMaxLimit->setEnabled( !isSpecificColorScale );

  myColorMode->setEnabled( !isSpecificColorScale );
  myBottomColor->setEnabled( !isSpecificColorScale );
  myTopColor->setEnabled( !isSpecificColorScale );

  if( isSpecificColorScale )
  {
    int aNbIntervals = myMajorSpecificColorScale.count() - 1;
    if( aNbIntervals > 0 )
      myInterval->setMinimum( 2 * aNbIntervals );
  }
  else
    myInterval->setMinimum( 2 );
}

//=============================================================================
// Function : updateColorScale
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::updateColorScale()
{
  if( colorScaleMode() == Plot3d_ColorDic::Specific )
  {
    updateMajorSpecificColorScale();
    updateMinorSpecificColorScale();
  }
}

//=============================================================================
// Function : updateMajorSpecificColorScale
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::updateMajorSpecificColorScale()
{
  if ( myMajorSpecificColorScale.size() < 2 )
  {
    myMajorSpecificColorScale.clear();
    myMajorSpecificColorScale.append( Value2Color( myMinLimit->value(), myBottomColor->color() ) );
    myMajorSpecificColorScale.append( Value2Color( myMaxLimit->value(), myTopColor->color() ) );
  }
}

//=============================================================================
// Function : updateMinorSpecificColorScale
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::updateMinorSpecificColorScale()
{
  myMinorSpecificColorScale.clear();

  int aNbMajorValues = myMajorSpecificColorScale.count();
  int aNbSubIntervals = myInterval->value() / ( aNbMajorValues - 1 );

  Value2ColorListIterator anIter( myMajorSpecificColorScale );
  Value2Color aValue2Color, aValue2ColorRef = anIter.next();
  while ( anIter.hasNext() )
  {
    aValue2Color = anIter.next();
    Value2ColorList aSubList =
      Plot3d_SetupSpecificColorScaleDlg::interpolateRange( aValue2ColorRef,
                                                           aValue2Color,
                                                           aNbSubIntervals );
    myMinorSpecificColorScale << aSubList;
    aValue2ColorRef = aValue2Color;
  }
  myMinorSpecificColorScale << aValue2Color;
}

//=============================================================================
// Function : updateMinMax
// Purpose  : 
//=============================================================================
void Plot3d_SetupColorScaleDlg::updateMinMax()
{
  double aMin = myMinLimit->value();
  double aMax = myMaxLimit->value();
  if ( colorScaleMode() == Plot3d_ColorDic::Specific &&
       !myMajorSpecificColorScale.isEmpty() )
  {
    aMin = myMajorSpecificColorScale.first().first;
    aMax = myMajorSpecificColorScale.last().first;
  }

  myMinLimit->setValue( aMin );
  myMaxLimit->setValue( aMax );
}
