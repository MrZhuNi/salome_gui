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
#include "Plot3d_SetupSpecificColorScaleDlg.h"

#include <QtxColorButton.h>
#include <QtxGroupBox.h>

#include <SUIT_MessageBox.h>

#include <LDOM_Document.hxx>
#include <LDOM_Element.hxx>
#include <LDOM_XmlWriter.hxx>
#include <LDOMParser.hxx>
#include <LDOMString.hxx>

#include <Precision.hxx>

#include <QApplication>
#include <QCheckBox>
#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QDoubleValidator>
#include <QFileDialog>
#include <QHeaderView>
#include <QItemDelegate>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>

#include <vtkLookupTable.h>

#define VALUE_COL 0
#define COLOR_COL 1

static const LDOMString NB_INTERVALS;
static const LDOMString VALUE;
static const LDOMString AUTO_VALUES;
static const LDOMString STATE;
static const LDOMString MIN;
static const LDOMString MAX;
static const LDOMString LOGARITHMIC;
static const LDOMString AUTO_COLORS;
static const LDOMString RED;
static const LDOMString GREEN;
static const LDOMString BLUE;
static const LDOMString COLOR_SCALE;
static const LDOMString SCALE_POINT;
static const LDOMString COLOR;

//=============================================================================
// Function : initDomStringConstants
// Purpose  : 
//=============================================================================
static void initDomStringConstants()
{
  if( NB_INTERVALS == NULL )
  {
    (LDOMString&) NB_INTERVALS = LDOMString( "NumberOfIntervals" );
    (LDOMString&) VALUE        = LDOMString( "value" );

    (LDOMString&) AUTO_VALUES  = LDOMString( "AutoValues" );
    (LDOMString&) STATE        = LDOMString( "state" );
    (LDOMString&) MIN          = LDOMString( "min" );
    (LDOMString&) MAX          = LDOMString( "max" );
    (LDOMString&) LOGARITHMIC  = LDOMString( "logarithmic" );

    (LDOMString&) AUTO_COLORS  = LDOMString( "AutoColors" );
    (LDOMString&) RED          = LDOMString( "red" );
    (LDOMString&) GREEN        = LDOMString( "green" );
    (LDOMString&) BLUE         = LDOMString( "blue" );

    (LDOMString&) COLOR_SCALE  = LDOMString( "ColorScale" );
    (LDOMString&) SCALE_POINT  = LDOMString( "ScalePoint" );
    (LDOMString&) COLOR        = LDOMString( "color" );
  }
}

/*
  Class       : Plot3d_SetupSpecificColorScaleDlg::ItemDelegate
  Description : Class, which provides editing facilities for data items from a model
*/
class Plot3d_SetupSpecificColorScaleDlg::ItemDelegate : public QItemDelegate
{
public:
  ItemDelegate( QObject* = 0 );

public:
  virtual QWidget* createEditor( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const;
  virtual void     updateEditorGeometry( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const;

private:
  QDoubleValidator* myDoubleValidator;
};

//=============================================================================
// Function : Plot3d_SetupSpecificColorScaleDlg::ItemDelegate
// Purpose  : Constructor
//=============================================================================
Plot3d_SetupSpecificColorScaleDlg::ItemDelegate::ItemDelegate( QObject* theParent )
: QItemDelegate( theParent )
{
  myDoubleValidator = new QDoubleValidator( this );
  myDoubleValidator->setDecimals( 6 );
}

//=============================================================================
// Function : createEditor
// Purpose  : 
//=============================================================================
QWidget* Plot3d_SetupSpecificColorScaleDlg::ItemDelegate::createEditor( QWidget* theParent,
                                                                    const QStyleOptionViewItem& theOption,
                                                                    const QModelIndex& ) const
{
  QLineEdit* aLineEdit = new QLineEdit( theParent );
  aLineEdit->setValidator( myDoubleValidator );
  return aLineEdit;
}

//=============================================================================
// Function : updateEditorGeometry
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::ItemDelegate::updateEditorGeometry( QWidget* theEditor,
                                                                        const QStyleOptionViewItem& theOption,
                                                                        const QModelIndex& ) const
{
  theEditor->setGeometry( theOption.rect );
}


//=============================================================================
// Function : Plot3d_SetupSpecificColorScaleDlg
// Purpose  : Constructor
//=============================================================================
Plot3d_SetupSpecificColorScaleDlg::Plot3d_SetupSpecificColorScaleDlg( QWidget* theParent )
: QtxDialog( theParent, true, false, QtxDialog::OKCancel )
{
  setWindowTitle( tr( "SETUP_SPECIFIC_COLOR_SCALE" ) );

  QFrame* aMainFrame = mainFrame();

  QVBoxLayout* aMainLayout = new QVBoxLayout( aMainFrame );
  aMainLayout->setMargin( 5 );
  aMainLayout->setSpacing( 5 );

  // Parameters
  QtxGroupBox* aParametersGrp = new QtxGroupBox( tr( "PARAMETERS" ), aMainFrame ); 
  aMainLayout->addWidget( aParametersGrp );

  // Number of specific intervals
  QLabel* aNbSpecificIntervalsLabel = new QLabel( tr( "NUMBER_OF_SPECIFIC_INTERVALS" ), aParametersGrp );
  myNbSpecificIntervalsSpinBox = new QSpinBox( aParametersGrp );
  myNbSpecificIntervalsSpinBox->setRange( 1, 99 );
  myNbSpecificIntervalsSpinBox->setValue( 1 );

  // Auto values
  myAutoValues = new QtxGroupBox( tr( "AUTO_VALUES" ), aParametersGrp );
  myAutoValues->setCheckable( true );
  myAutoValues->setChecked( true );

  QLabel* aMinimumValueLabel = new QLabel( tr( "MINIMUM" ), myAutoValues );
  myMinimumSpinBox = new QDoubleSpinBox( myAutoValues );
  myMinimumSpinBox->setMinimum( -DBL_MAX );
  myMinimumSpinBox->setMaximum( DBL_MAX );
  myMinimumSpinBox->setDecimals( 6 );

  QLabel* aMaximumValueLabel = new QLabel( tr( "MAXIMUM" ), myAutoValues );
  myMaximumSpinBox = new QDoubleSpinBox( myAutoValues );
  myMaximumSpinBox->setMinimum( -DBL_MAX );
  myMaximumSpinBox->setMaximum( DBL_MAX );
  myMaximumSpinBox->setDecimals( 6 );

  myIsLogarithmic = new QCheckBox( tr( "LOGARITHMIC_INTERPOLATION" ) );
  myIsLogarithmic->setChecked( false );

  QGridLayout* anAutoValuesLayout = new QGridLayout( myAutoValues );
  anAutoValuesLayout->setMargin( 5 );
  anAutoValuesLayout->setSpacing( 5 );
  anAutoValuesLayout->addWidget( aMinimumValueLabel, 0, 0, 1, 1 );
  anAutoValuesLayout->addWidget( myMinimumSpinBox,   0, 1, 1, 1 );
  anAutoValuesLayout->addWidget( aMaximumValueLabel, 1, 0, 1, 1 );
  anAutoValuesLayout->addWidget( myMaximumSpinBox,   1, 1, 1, 1 );
  anAutoValuesLayout->addWidget( myIsLogarithmic,    2, 0, 1, 2 );

  // Auto colors
  myAutoColors = new QtxGroupBox( tr( "AUTO_COLORS" ), aParametersGrp );
  myAutoColors->setCheckable( true );
  myAutoColors->setChecked( true );

  QLabel* aMinimumColorLabel = new QLabel( tr( "MINIMUM" ), myAutoColors );
  myMinimumBtn = new QtxColorButton( myAutoColors );

  QLabel* aMaximumColorLabel = new QLabel( tr( "MAXIMUM" ), myAutoColors );
  myMaximumBtn = new QtxColorButton( myAutoColors );

  QGridLayout* anAutoColorsLayout = new QGridLayout( myAutoColors );
  anAutoColorsLayout->setMargin( 5 );
  anAutoColorsLayout->setSpacing( 5 );
  anAutoColorsLayout->addWidget( aMinimumColorLabel, 0, 0, 1, 1 );
  anAutoColorsLayout->addWidget( myMinimumBtn,       0, 1, 1, 1 );
  anAutoColorsLayout->addWidget( aMaximumColorLabel, 0, 2, 1, 1 );
  anAutoColorsLayout->addWidget( myMaximumBtn,       0, 3, 1, 1 );

  // Generate table
  myGenerateButton = new QPushButton( tr( "GENERATE_TABLE" ), aParametersGrp );

  // Common
  QGridLayout* aParametersLayout = new QGridLayout( aParametersGrp );
  aParametersLayout->setMargin( 5 );
  aParametersLayout->setSpacing( 5 );
  aParametersLayout->addWidget( aNbSpecificIntervalsLabel,    0, 0, 1, 1 );
  aParametersLayout->addWidget( myNbSpecificIntervalsSpinBox, 0, 1, 1, 1 );
  aParametersLayout->addWidget( myAutoValues,                 1, 0, 1, 2 );
  aParametersLayout->addWidget( myAutoColors,                 2, 0, 1, 2 );
  aParametersLayout->addWidget( myGenerateButton,              3, 0, 1, 2 );

  // Color scale
  QtxGroupBox* aColorScaleGrp = new QtxGroupBox( tr( "COLOR_SCALE" ), aMainFrame ); 
  aMainLayout->addWidget( aColorScaleGrp );

  // Table
  myTable = new QTableWidget( aColorScaleGrp );
  myTable->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

  myTable->setRowCount( 0 );
  myTable->setColumnCount( 2 );

  QStringList aLabels;
  aLabels.append( tr( "VALUE" ) );
  aLabels.append( tr( "COLOR" ) );
  myTable->setHorizontalHeaderLabels( aLabels );
  myTable->setSelectionMode( QTableWidget::NoSelection );

  myTable->horizontalHeader()->setResizeMode( VALUE_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setResizeMode( COLOR_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setHighlightSections( false );
  myTable->verticalHeader()->setHighlightSections( false );

  myTable->setColumnWidth( VALUE_COL, 150 );
  myTable->setColumnWidth( COLOR_COL, 50 );

  myTable->setItemDelegateForColumn( VALUE_COL, new Plot3d_SetupSpecificColorScaleDlg::ItemDelegate( myTable ) );

  QBoxLayout* aColorScaleLayout = new QVBoxLayout( aColorScaleGrp );
  aColorScaleLayout->setMargin( 5 );
  aColorScaleLayout->setSpacing( 5 );
  aColorScaleLayout->addWidget( myTable );

  // Import/export setting
  QtxGroupBox* anImportExportGrp = new QtxGroupBox( tr( "IMPORT_EXPORT_SETTINGS" ), aMainFrame ); 
  aMainLayout->addWidget( anImportExportGrp );

  QPushButton* anImportButton = new QPushButton( tr( "IMPORT_SETTINGS" ), anImportExportGrp );
  QPushButton* anExportButton = new QPushButton( tr( "EXPORT_SETTINGS" ), anImportExportGrp );

  QBoxLayout* anImportExportLayout = new QHBoxLayout( anImportExportGrp );
  anImportExportLayout->setMargin( 5 );
  anImportExportLayout->setSpacing( 5 );
  anImportExportLayout->addWidget( anImportButton );
  anImportExportLayout->addWidget( anExportButton );

  connect( myGenerateButton, SIGNAL( clicked() ), this, SLOT( onGenerateTable() ) );
  connect( anImportButton, SIGNAL( clicked() ), this, SLOT( onImportSettings() ) );
  connect( anExportButton, SIGNAL( clicked() ), this, SLOT( onExportSettings() ) );

  setButtonPosition( Right, Cancel );
  setMinimumHeight( 600 );
}

//=============================================================================
// Function : ~Plot3d_SetupSpecificColorScaleDlg
// Purpose  : Destructor
//=============================================================================
Plot3d_SetupSpecificColorScaleDlg::~Plot3d_SetupSpecificColorScaleDlg()
{
}

//=============================================================================
// Function : SetMajorScale
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::SetMajorScale( const Value2ColorList& theMajorScale )
{
  int aNbValues = theMajorScale.count();
  myNbSpecificIntervalsSpinBox->setValue( aNbValues - 1 );

  if ( aNbValues >= 2 )
  {
    Value2Color aValue2ColorMin = theMajorScale.first();
    myMinimumSpinBox->setValue( aValue2ColorMin.first );
    myMinimumBtn->setColor( aValue2ColorMin.second );

    Value2Color aValue2ColorMax = theMajorScale.last();
    myMaximumSpinBox->setValue( aValue2ColorMax.first );
    myMaximumBtn->setColor( aValue2ColorMax.second );
  }

  updateTable( theMajorScale );
}

//=============================================================================
// Function : SetNewVar
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::SetNewVar( const bool theIsNew, 
                                                   const double theMin, 
                                                   const double theMax )
{
  if ( theIsNew )
  {
    myMinimumSpinBox->setValue( theMin );
    myMaximumSpinBox->setValue( theMax );
    myGenerateButton->setText( tr( "GENERATE_TABLE_FOR_NEW" ) );
  }
  else 
    myGenerateButton->setText( tr( "GENERATE_TABLE" ) );
}

//=============================================================================
// Function : GetMajorScale
// Purpose  : 
//=============================================================================
const Value2ColorList& Plot3d_SetupSpecificColorScaleDlg::GetMajorScale() const
{
  return myMajorScale;
}

//=============================================================================
// Function : accept
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::accept()
{
  QString aMessage;

  myMajorScale.clear();
  int aNbValues = myTable->rowCount();
  for ( int aRow = 0; aRow < aNbValues; aRow++ )
  {
    Value2Color aValue2Color;

    double aValue = 0.0;
    bool ok = value( aRow, aValue );
    if ( !ok )
    {
      aMessage = tr( "WRN_UNINITIALIZED_VALUES" );
      break;
    }
    aValue2Color.first = aValue;

    QPushButton* aBtn = dynamic_cast< QPushButton* >( myTable->cellWidget( aRow, COLOR_COL ) );
    aValue2Color.second = color( aBtn );

    myMajorScale.append( aValue2Color );
  }

  if ( !aMessage.isEmpty() || !checkMajorScale( aMessage ) )
  {
    SUIT_MessageBox::warning( this, tr( "WARNING" ), aMessage );
    myMajorScale.clear();
    return;
  }

  QtxDialog::accept();
}

//=============================================================================
// Function : onGenerateTable
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::onGenerateTable()
{
  int aNbIntervals = myNbSpecificIntervalsSpinBox->value();

  double aMinimumValue = myMinimumSpinBox->value();
  double aMaximumValue = myMaximumSpinBox->value();
  bool anIsLogarithmic = myIsLogarithmic->isChecked();

  QString aMessage;
  if ( aMinimumValue > aMaximumValue ||
       fabs( aMaximumValue - aMinimumValue ) < Precision::Confusion() )
    aMessage = tr( "WRN_INCORRECT_RANGE_MIN_MAX" );
  else if ( anIsLogarithmic &&
            ( aMinimumValue < 0 || fabs( aMinimumValue ) < Precision::Confusion() ||
              aMaximumValue < 0 || fabs( aMaximumValue ) < Precision::Confusion() ) )
    aMessage = tr( "WRN_INCORRECT_RANGE_LOGARITHMIC" );

  if ( !aMessage.isEmpty() )
  {
    SUIT_MessageBox::warning( this, tr( "WARNING" ), aMessage );
    return;
  }

  Value2Color aMinimum( aMinimumValue, myMinimumBtn->color() );
  Value2Color aMaximum( aMaximumValue, myMaximumBtn->color() );

  Value2ColorList aData = interpolateRange( aMinimum, aMaximum, aNbIntervals - 1, true, anIsLogarithmic );
  aData.prepend( aMinimum );
  aData.append( aMaximum );

  updateTable( aData );
}

//=============================================================================
// Function : onColorBtn
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::onColorBtn()
{ 
  QObject* aSender = (QObject*)sender();
  if ( !aSender )
    return;

  QPushButton* aBtn = dynamic_cast< QPushButton* >( aSender );
  if ( !aBtn )
    return;

  QColor anOldColor = color( aBtn );
  QColor aNewColor = QColorDialog::getColor( anOldColor, this );
  if ( !aNewColor.isValid() )
    return;
    
  setColor( aBtn, aNewColor );
}

//=============================================================================
// Function : clearTable
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::clearTable()
{
  QList<QPushButton*> aButtonList = myTable->findChildren<QPushButton*>();
  for ( int i = 0, n = aButtonList.count(); i < n; i++ )
  {
    if( QPushButton* aButton = aButtonList.at( i ) )
    {
      delete aButton;
      aButton = 0;
    }
  }
  aButtonList.clear();

  myTable->setRowCount( 0 );
}

//=============================================================================
// Function : updateTable
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::updateTable( const Value2ColorList& theMajorScale )
{
  int aNbValues = theMajorScale.count();
  myNbSpecificIntervalsSpinBox->setValue( aNbValues - 1 );

  clearTable();

  myTable->setRowCount( aNbValues );

  int aRow = 0;
  Value2ColorListIterator anIter( theMajorScale );
  while ( anIter.hasNext() )
  {
    Value2Color aValue2Color = anIter.next();

    // Value
    if( myAutoValues->isChecked() )
      setValue( aRow, aValue2Color.first );

    // Color
    setColor( aRow, ( myAutoColors->isChecked() ) ? aValue2Color.second : Qt::black );

    aRow++;
  }
}

//=============================================================================
// Function : value
// Purpose  : 
//=============================================================================
bool Plot3d_SetupSpecificColorScaleDlg::value( const int theRow, double& theValue ) const
{
  if ( QTableWidgetItem* anItem = myTable->item( theRow, VALUE_COL ) )
  {
    QString aText = anItem->text();
    bool ok = false;
    theValue = aText.toDouble( &ok );
    return ok;
  }
  return false;
}

//=============================================================================
// Function : setValue
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::setValue( const int theRow, const double theValue )
{
  setValue( theRow, QString::number( theValue ) );
}

//=============================================================================
// Function : setValue
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::setValue( const int theRow, const QString& theValue )
{
  QTableWidgetItem* anItem = myTable->item( theRow, VALUE_COL );
  if ( !anItem )
  {
    anItem = new QTableWidgetItem( theValue );
    myTable->setItem( theRow, VALUE_COL, anItem );
  }
  else 
    anItem->setText( theValue );
}

//=============================================================================
// Function : setColor
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::setColor( QPushButton* theBtn, const QColor& theColor )
{
  if( theBtn )
  {
    QPalette aPal = theBtn->palette();
    aPal.setColor( theBtn->backgroundRole(), theColor );
    theBtn->setPalette( aPal );
  }
}

//=============================================================================
// Function : setColor
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::setColor( const int theRow, const QColor& theColor )
{
  QPushButton* aBtn = new QPushButton( myTable );
  myTable->setCellWidget( theRow, COLOR_COL, aBtn );
  setColor( aBtn, theColor );
  connect( aBtn, SIGNAL( clicked() ), SLOT( onColorBtn() ) );
}

//=============================================================================
// Function : color
// Purpose  : 
//=============================================================================
QColor Plot3d_SetupSpecificColorScaleDlg::color( QPushButton* theBtn ) const
{
  return theBtn ? theBtn->palette().color( theBtn->backgroundRole() ) : QColor();
}

//=============================================================================
// Function : setEditable
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::setEditable( const int theRow, 
                                                 const int theCol,
                                                 const bool theState )
{
  QTableWidgetItem* anItem = myTable->item( theRow, theCol );
  if ( !anItem )
  {
    anItem = new QTableWidgetItem();
    myTable->setItem( theRow, theCol, anItem );
  }

  Qt::ItemFlags aFlags = anItem->flags();
  aFlags = theState ? aFlags | Qt::ItemIsEditable : aFlags ^ Qt::ItemIsEditable;
  anItem->setFlags( aFlags );
}

//=============================================================================
// Function : checkMajorScale
// Purpose  : 
//=============================================================================
bool Plot3d_SetupSpecificColorScaleDlg::checkMajorScale( QString& theMessage ) const
{
  if ( myMajorScale.count() < 2 )
    return false;

  // check data
  Value2ColorListIterator anIter( myMajorScale );
  Value2Color aValue2Color, aValue2ColorRef = anIter.next();
  while ( anIter.hasNext() )
  {
    aValue2Color = anIter.next();
    if ( aValue2Color.first < aValue2ColorRef.first )
    {
      theMessage = tr( "WRN_INCONSISTENT_VALUES" );
      return false;
    }
    aValue2ColorRef = aValue2Color;
  }

  return true;
}

//=============================================================================
// Function : interpolateRange
// Purpose  : 
//=============================================================================
Value2ColorList Plot3d_SetupSpecificColorScaleDlg::interpolateRange( const Value2Color& theMinimum,
                                                                 const Value2Color& theMaximum,
                                                                 const int theNumber,
                                                                 const bool theIsMajor,
                                                                 const bool theIsLogarithmic )
{
  Value2ColorList aResult;

  double aMin = theMinimum.first;
  QColor aColorMin = theMinimum.second;
  double aHueMin = aColorMin.hueF();
  double aSaturationMin = aColorMin.saturationF();
  double aValueMin = aColorMin.valueF();

  double aMax = theMaximum.first;
  QColor aColorMax = theMaximum.second;
  double aHueMax = aColorMax.hueF();
  double aSaturationMax = aColorMax.saturationF();
  double aValueMax = aColorMax.valueF();

  // additional check for achromatic colors, which
  // have a hue value of -1 (see CATHAREGUI_ColorDic)
  double aHueMinTmp = aHueMin;
  double aHueMaxTmp = aHueMax;
  aHueMin = aHueMinTmp < 0 ? aHueMaxTmp : aHueMinTmp;
  aHueMax = aHueMaxTmp < 0 ? aHueMinTmp : aHueMaxTmp;

  double aRange[2] = { aMin, aMax };

  vtkLookupTable* aLookupTable = vtkLookupTable::New();
  aLookupTable->SetRange( aRange );
  aLookupTable->SetNumberOfTableValues( theNumber + ( theIsMajor ? 2 : 0 ) );

  aLookupTable->SetHueRange( aHueMin, aHueMax );
  aLookupTable->SetSaturationRange( aSaturationMin, aSaturationMax );
  aLookupTable->SetValueRange( aValueMin, aValueMax );
  
  aLookupTable->Build();

  int anIndexFirst = 0;
  int anIndexLast = theNumber - 1;
  int aNumber = theNumber;
  if ( theIsMajor )
  {
    anIndexFirst++;
    anIndexLast++;
    aNumber++;
  }

  if ( theIsLogarithmic )
  {
    if ( aMin > 0 && aMax > 0 )
    {
      aMin = log10( aMin );
      aMax = log10( aMax );
    }
    else
      return aResult;
  }

  double rgba[4];
  Value2Color aValue2Color;
  for ( int anIndex = anIndexFirst; anIndex <= anIndexLast; anIndex++ )
  {
    aLookupTable->GetTableValue( anIndex, rgba );
    double aValue = aMin + ( aMax - aMin ) * (double)anIndex / (double)( aNumber );
    aValue2Color.first = theIsLogarithmic ? pow( 10, aValue ) : aValue;
    aValue2Color.second.setRgbF( rgba[0], rgba[1], rgba[2] );
    aResult.append( aValue2Color );
  }

  aLookupTable->Delete();

  return aResult;
}

//=============================================================================
// Function : onImportSettings
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::onImportSettings()
{
  char* aHomePath = getenv( "HOME" );
  QString aPath = aHomePath ? aHomePath : "";

  QString aFilter = tr( "XML_FILTER" );
  QString aFileName = QFileDialog::getOpenFileName( this, tr( "IMPORT_SETTINGS" ), aPath, aFilter );
  if( aFileName.isEmpty() )
    return;

  QApplication::setOverrideCursor( Qt::WaitCursor );

  ::initDomStringConstants();

  LDOMParser aParser;
  if( aParser.parse( aFileName.toLatin1().constData() ) )
  {
    QApplication::restoreOverrideCursor();
    SUIT_MessageBox::critical( this, tr( "ERROR" ), tr( "IMPORT_FAILED" ), tr( "BUT_OK" ) );
    return;
  }

  LDOM_Document aDoc = aParser.getDocument();
  LDOM_Element aDocElement = aDoc.getDocumentElement();

  LDOM_Element anElement, aSubElement, aSubElement2;
  TCollection_AsciiString aString, aStringR, aStringG, aStringB;
  QColor aColor;

  // Number of specific intervals
  anElement = aDocElement.GetChildByTagName( NB_INTERVALS );
  if( !anElement.isNull() )
  {
    aString = anElement.getAttribute( VALUE );
    if( aString.IsIntegerValue() )
      myNbSpecificIntervalsSpinBox->setValue( aString.IntegerValue() );
  }

  // Auto values
  anElement = aDocElement.GetChildByTagName( AUTO_VALUES );
  if( !anElement.isNull() )
  {
    aString = anElement.getAttribute( STATE );
    if( aString.IsIntegerValue() )
      myAutoValues->setChecked( (bool)aString.IntegerValue() );

    aString = anElement.getAttribute( MIN );
    if( aString.IsRealValue() )
      myMinimumSpinBox->setValue( aString.RealValue() );

    aString = anElement.getAttribute( MAX );
    if( aString.IsRealValue() )
      myMaximumSpinBox->setValue( aString.RealValue() );

    aString = anElement.getAttribute( LOGARITHMIC );
    if( aString.IsIntegerValue() )
      myIsLogarithmic->setChecked( (bool)aString.IntegerValue() );
  }

  // Auto colors
  anElement = aDocElement.GetChildByTagName( AUTO_COLORS );
  if( !anElement.isNull() )
  {
    aString = anElement.getAttribute( STATE );
    if( aString.IsIntegerValue() )
      myAutoColors->setChecked( (bool)aString.IntegerValue() );

    aSubElement = anElement.GetChildByTagName( MIN );
    if( !aSubElement.isNull() )
    {
      aStringR = aSubElement.getAttribute( RED );
      aStringG = aSubElement.getAttribute( GREEN );
      aStringB = aSubElement.getAttribute( BLUE );
      if( aStringR.IsIntegerValue() &&
          aStringG.IsIntegerValue() &&
          aStringB.IsIntegerValue() )
      {
        QColor aColor( aStringR.IntegerValue(),
                       aStringG.IntegerValue(),
                       aStringB.IntegerValue() );
        myMinimumBtn->setColor( aColor );
      }
    }

    aSubElement = anElement.GetChildByTagName( MAX );
    if( !aSubElement.isNull() )
    {
      aStringR = aSubElement.getAttribute( RED );
      aStringG = aSubElement.getAttribute( GREEN );
      aStringB = aSubElement.getAttribute( BLUE );
      if( aStringR.IsIntegerValue() &&
          aStringG.IsIntegerValue() &&
          aStringB.IsIntegerValue() )
      {
        QColor aColor( aStringR.IntegerValue(),
                       aStringG.IntegerValue(),
                       aStringB.IntegerValue() );
        myMaximumBtn->setColor( aColor );
      }
    }
  }

  // Color scale
  int aRow = 0;
  clearTable();
  anElement = aDocElement.GetChildByTagName( COLOR_SCALE );
  for( aSubElement = anElement.GetChildByTagName( SCALE_POINT );
       !aSubElement.isNull();
       aSubElement = aSubElement.GetSiblingByTagName(), aRow++ )
  {
    aString = aSubElement.getAttribute( VALUE );

    QColor aColor;
    aSubElement2 = aSubElement.GetChildByTagName( COLOR );
    if( !aSubElement2.isNull() )
    {
      aStringR = aSubElement2.getAttribute( RED );
      aStringG = aSubElement2.getAttribute( GREEN );
      aStringB = aSubElement2.getAttribute( BLUE );
      if( aStringR.IsIntegerValue() &&
          aStringG.IsIntegerValue() &&
          aStringB.IsIntegerValue() )
        aColor = QColor( aStringR.IntegerValue(),
                         aStringG.IntegerValue(),
                         aStringB.IntegerValue() );
    }

    myTable->setRowCount( aRow + 1 );
    setValue( aRow, QString( aString.ToCString() ) );
    setColor( aRow, aColor );
  }

  QApplication::restoreOverrideCursor();
}

//=============================================================================
// Function : onExportSettings
// Purpose  : 
//=============================================================================
void Plot3d_SetupSpecificColorScaleDlg::onExportSettings()
{
  char* aHomePath = getenv( "HOME" );
  QString aPath = aHomePath ? aHomePath : "";

  QString aFilter = tr( "XML_FILTER" );
  QString aSelectedFilter;
  QString aFileName = QFileDialog::getSaveFileName( this, tr( "EXPORT_SETTINGS" ), aPath,
                                                    aFilter, &aSelectedFilter );
  if( aFileName.isEmpty() )
    return;

  if( aSelectedFilter.indexOf( "xml" ) != -1 )
  {
    QString aSuffix = QFileInfo( aFileName ).suffix();
    if( aSuffix.toLower() != "xml" )
      aFileName += ".xml";
  }

  QApplication::setOverrideCursor( Qt::WaitCursor );

  ::initDomStringConstants();

  LDOM_Document aDoc = LDOM_Document::createDocument( "Settings" );
  LDOM_Element aDocElement = aDoc.getDocumentElement();

  // Number of specific intervals
  LDOM_Element aNbIntervalsElement = aDoc.createElement( NB_INTERVALS );
  aNbIntervalsElement.setAttribute( VALUE, LDOMString( myNbSpecificIntervalsSpinBox->value() ) );
  aDocElement.appendChild( aNbIntervalsElement );

  // Auto values
  TCollection_AsciiString aMinValue( myMinimumSpinBox->value() );
  TCollection_AsciiString aMaxValue( myMaximumSpinBox->value() );

  LDOM_Element anAutoValuesElement = aDoc.createElement( AUTO_VALUES );
  anAutoValuesElement.setAttribute( STATE, LDOMString( (int)myAutoValues->isChecked() ) );
  anAutoValuesElement.setAttribute( MIN, LDOMString( aMinValue.ToCString() ) );
  anAutoValuesElement.setAttribute( MAX, LDOMString( aMaxValue.ToCString() ) );
  anAutoValuesElement.setAttribute( LOGARITHMIC, LDOMString( (int)myIsLogarithmic->isChecked() ) );
  aDocElement.appendChild( anAutoValuesElement );

  // Auto colors
  QColor aMinColor( myMinimumBtn->color() );
  QColor aMaxColor( myMaximumBtn->color() );

  LDOM_Element anAutoColorsElement = aDoc.createElement( AUTO_COLORS );
  anAutoColorsElement.setAttribute( STATE, LDOMString( (int)myAutoColors->isChecked() ) );

  LDOM_Element aMinColorElement = aDoc.createElement( MIN );
  aMinColorElement.setAttribute( RED, LDOMString( aMinColor.red() ) );
  aMinColorElement.setAttribute( GREEN, LDOMString( aMinColor.green() ) );
  aMinColorElement.setAttribute( BLUE, LDOMString( aMinColor.blue() ) );
  anAutoColorsElement.appendChild( aMinColorElement );

  LDOM_Element aMaxColorElement = aDoc.createElement( MAX );
  aMaxColorElement.setAttribute( RED, LDOMString( aMaxColor.red() ) );
  aMaxColorElement.setAttribute( GREEN, LDOMString( aMaxColor.green() ) );
  aMaxColorElement.setAttribute( BLUE, LDOMString( aMaxColor.blue() ) );
  anAutoColorsElement.appendChild( aMaxColorElement );

  aDocElement.appendChild( anAutoColorsElement );

  // Color scale
  LDOM_Element aColorScaleElement = aDoc.createElement( COLOR_SCALE );

  int aNbValues = myTable->rowCount();
  for( int aRow = 0; aRow < aNbValues; aRow++ )
  {
    QString aValue;
    if( QTableWidgetItem* anItem = myTable->item( aRow, VALUE_COL ) )
      aValue = anItem->text();

    QPushButton* aBtn = dynamic_cast< QPushButton* >( myTable->cellWidget( aRow, COLOR_COL ) );
    QColor aColor = color( aBtn );

    LDOM_Element aScalePointElement = aDoc.createElement( SCALE_POINT );
    aScalePointElement.setAttribute( VALUE, LDOMString( aValue.toLatin1().constData() ) );

    LDOM_Element aColorElement = aDoc.createElement( COLOR );
    aColorElement.setAttribute( RED, LDOMString( aColor.red() ) );
    aColorElement.setAttribute( GREEN, LDOMString( aColor.green() ) );
    aColorElement.setAttribute( BLUE, LDOMString( aColor.blue() ) );
    aScalePointElement.appendChild( aColorElement );

    aColorScaleElement.appendChild( aScalePointElement );
  }
  aDocElement.appendChild( aColorScaleElement );

  // Write data to file
  FILE* aFile = fopen( aFileName.toLatin1().constData(), "wt" );
  if( aFile )
  {
    LDOM_XmlWriter aWriter( aFile );
    aWriter.SetIndentation( 2 );
    aWriter << aDoc;
    fclose( aFile );
  }
  else
  {
    QApplication::restoreOverrideCursor();
    SUIT_MessageBox::critical( this, tr( "ERROR" ), tr( "EXPORT_FAILED" ), tr( "BUT_OK" ) );
  }

  QApplication::restoreOverrideCursor();
}
