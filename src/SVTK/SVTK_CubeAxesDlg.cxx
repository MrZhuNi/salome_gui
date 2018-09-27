//  VISU VISUGUI : GUI for SMESH component
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SVTK_CubeAxesDlg.cxx
//  Author : Sergey LITONIN
//  Module : VISU

#include "SVTK_CubeAxesDlg.h"

#include "SVTK_ViewWindow.h"
#include "SVTK_FontWidget.h"

#include "SVTK_CubeAxesActor2D.h"

#include "QtxAction.h"
#include "QtxIntSpinBox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>

#include <vtkAxisActor2D.h>
#include <vtkTextProperty.h>

#define DEFAULT_FONT_SIZE 16

/*!
  Constructor
*/
SVTK_CubeAxesDlg::AxisWidget::AxisWidget (QWidget* theParent)
:  QFrame(theParent)
{
  QList< QLabel* > aLabels;

  // "Name" grp

  myNameGrp = new QGroupBox(SVTK_CubeAxesDlg::tr("AXIS_NAME"), this);
  myNameGrp->setCheckable( true );

  QVBoxLayout* aVBox = new QVBoxLayout;
  
  QHBoxLayout* aHBox = new QHBoxLayout;
  aHBox->setMargin(0);
  aHBox->setSpacing(5);
  QLabel* aLabel = new QLabel(SVTK_CubeAxesDlg::tr("NAME"));
  aHBox->addWidget(aLabel);
  myAxisName = new QLineEdit;
  aHBox->addWidget(myAxisName);
  aLabels.append(aLabel);
  aVBox->addLayout(aHBox);

  aHBox = new QHBoxLayout;
  aHBox->setMargin(0);
  aHBox->setSpacing(5);
  aLabel = new QLabel(SVTK_CubeAxesDlg::tr("FONT"));
  aHBox->addWidget(aLabel);
  myNameFont = new SVTK_FontWidget(myNameGrp);
  aHBox->addWidget(myNameFont);
  aLabels.append(aLabel);
  aVBox->addLayout(aHBox);

  myNameGrp->setLayout(aVBox);

  // "Labels" grp

  myLabelsGrp = new QGroupBox(SVTK_CubeAxesDlg::tr("LABELS"), this);
  myLabelsGrp->setCheckable( true );

  aVBox = new QVBoxLayout;

  aHBox = new QHBoxLayout;
  aHBox->setMargin(0);
  aHBox->setSpacing(5);
  aLabel = new QLabel(SVTK_CubeAxesDlg::tr("NUMBER"));
  aHBox->addWidget(aLabel);
  myLabelNumber = new QtxIntSpinBox(2,25,1,myLabelsGrp);
  aHBox->addWidget(myLabelNumber, 1);
  myAdjustRange = new QCheckBox(SVTK_CubeAxesDlg::tr("ADJUST_AUTOMATICALLY"));
  myAdjustRange->setChecked(false);
  aHBox->addWidget(myAdjustRange);
  aLabels.append(aLabel);
  aVBox->addLayout(aHBox);

  aHBox = new QHBoxLayout;
  aHBox->setMargin(0);
  aHBox->setSpacing(5);
  aLabel = new QLabel(SVTK_CubeAxesDlg::tr("OFFSET"));
  aHBox->addWidget(aLabel);
  myLabelOffset = new QtxIntSpinBox(0,100,1,myLabelsGrp);
  aHBox->addWidget(myLabelOffset);
  aLabels.append(aLabel);
  aVBox->addLayout(aHBox);

  aHBox = new QHBoxLayout;
  aHBox->setMargin(0);
  aHBox->setSpacing(5);
  aLabel = new QLabel(SVTK_CubeAxesDlg::tr("FORMAT"));
  aHBox->addWidget(aLabel);
  myLabelFormat = new QLineEdit(myLabelsGrp);
  aHBox->addWidget(myLabelFormat);
  aLabels.append(aLabel);
  aVBox->addLayout(aHBox);

  myLabelFormat->setToolTip( SVTK_CubeAxesDlg::tr("FORMAT_TOOLTIP") );

  aHBox = new QHBoxLayout;
  aHBox->setMargin(0);
  aHBox->setSpacing(5);
  aLabel = new QLabel(SVTK_CubeAxesDlg::tr("FONT"));
  aHBox->addWidget(aLabel);
  myLabelsFont = new SVTK_FontWidget(myLabelsGrp);
  aHBox->addWidget(myLabelsFont);
  aLabels.append(aLabel);
  aVBox->addLayout(aHBox);

  myLabelsGrp->setLayout(aVBox);

  // "Tick marks" grp

  myTicksGrp = new QGroupBox(SVTK_CubeAxesDlg::tr("TICK_MARKS"), this);
  myTicksGrp->setCheckable( true );

  aVBox = new QVBoxLayout;

  aHBox = new QHBoxLayout;
  aHBox->setMargin(0);
  aHBox->setSpacing(5);
  aLabel = new QLabel(SVTK_CubeAxesDlg::tr("LENGTH"));
  aHBox->addWidget(aLabel);
  myTickLength = new QtxIntSpinBox(0,100,1,myTicksGrp);
  aHBox->addWidget(myTickLength);
  aLabels.append(aLabel);
  aVBox->addLayout(aHBox);

  myTicksGrp->setLayout(aVBox);

  // Layout

  QVBoxLayout* aLay = new QVBoxLayout(this);
  aLay->setMargin(5);
  aLay->setSpacing(5);
  aLay->addWidget(myNameGrp);
  aLay->addWidget(myLabelsGrp);
  aLay->addWidget(myTicksGrp);

  // init
  myNameGrp->setChecked( true );
  myLabelsGrp->setChecked( true );
  myTicksGrp->setChecked( true );

  myAdjustRange->setVisible( false );

  // Adjust label widths
  QList< QLabel* >::iterator anIter;
  int aMaxWidth = 0;
  for (anIter = aLabels.begin(); anIter != aLabels.end(); anIter++)
    aMaxWidth = qMax(aMaxWidth, (*anIter)->sizeHint().width());
  for (anIter = aLabels.begin(); anIter != aLabels.end(); anIter++)
    (*anIter)->setFixedWidth(aMaxWidth);

  // Connections
  connect( myAdjustRange, SIGNAL( toggled( bool ) ), this, SLOT( onAdjustRange( bool ) ) );
}

/*!
  Destructor
*/
SVTK_CubeAxesDlg::AxisWidget::~AxisWidget()
{
}

void SVTK_CubeAxesDlg::AxisWidget::UseName(const bool toUse)
{
  myNameGrp->setChecked(toUse);
}

void SVTK_CubeAxesDlg::AxisWidget::SetName(const QString& theName)
{
  myAxisName->setText(theName);
}

void SVTK_CubeAxesDlg::AxisWidget::SetNameFont(const QColor& theColor,
                                               const int theFont,
                                               const int theSize,
                                               const bool theIsBold,
                                               const bool theIsItalic,
                                               const bool theIsShadow)
{
  myNameFont->SetData(theColor, theFont, theSize, theIsBold, theIsItalic, theIsShadow);
}

void SVTK_CubeAxesDlg::AxisWidget::SetAdjustRangeEnabled( const bool theIsEnabled )
{
  myAdjustRange->setVisible( theIsEnabled );
}

void SVTK_CubeAxesDlg::AxisWidget::SetAdjustRange( const bool theState )
{
  myAdjustRange->setChecked( theState );
}

bool SVTK_CubeAxesDlg::AxisWidget::GetAdjustRange() const
{
  return myAdjustRange->isChecked();
}

void SVTK_CubeAxesDlg::AxisWidget::onAdjustRange( bool theState )
{
  myLabelNumber->setEnabled( !theState );
}

bool SVTK_CubeAxesDlg::AxisWidget::ReadData(vtkAxisActor2D* theActor)
{
  if (theActor == 0)
    return false;

  // Font sizes of title and labels are calculated on the base of FontFactor and LabelFactor
  // (vtkAxisActor2D completely ignores FontSize of the text property).
  double aFontFactor = theActor->GetFontFactor();
  double aLabelFactor = theActor->GetLabelFactor();

  int aTitleFontSize = (int)( DEFAULT_FONT_SIZE * aFontFactor );
  int aLabelsFontSize = (int)( aTitleFontSize * aLabelFactor );

  // Name

  bool useName = theActor->GetTitleVisibility();
  QString aTitle(theActor->GetTitle());

  QColor aTitleColor(255, 255, 255);
  int aTitleFontFamily = VTK_ARIAL;
  //int aTitleFontSize = 12;
  bool isTitleBold = false;
  bool isTitleItalic = false;
  bool isTitleShadow = false;

  vtkTextProperty* aTitleProp = theActor->GetTitleTextProperty();
  if (aTitleProp !=0)
  {
    vtkFloatingPointType c[ 3 ];
    aTitleProp->GetColor(c);
    aTitleColor.setRgb((int)(c[ 0 ] * 255), (int)(c[ 1 ] * 255), (int)(c[ 2 ] * 255));
    aTitleFontFamily = aTitleProp->GetFontFamily();
    //aTitleFontSize = aTitleProp->GetFontSize();
    isTitleBold = aTitleProp->GetBold() ? true : false;
    isTitleItalic = aTitleProp->GetItalic() ? true : false;
    isTitleShadow = aTitleProp->GetShadow() ? true : false;
  }

  myNameGrp->setChecked(useName);
  myAxisName->setText(aTitle);
  myNameFont->SetData(aTitleColor, aTitleFontFamily, aTitleFontSize, isTitleBold, isTitleItalic, isTitleShadow);

  // Labels

  bool useLabels = theActor->GetLabelVisibility();
  int nbLabels = theActor->GetNumberOfLabels();
  int anOffset = theActor->GetTickOffset();
  char* aFormat = theActor->GetLabelFormat();

  QColor aLabelsColor(255, 255, 255);
  int aLabelsFontFamily = VTK_ARIAL;
  //int aLabelsFontSize = 12;
  bool isLabelsBold = false;
  bool isLabelsItalic = false;
  bool isLabelsShadow = false;

  vtkTextProperty* aLabelsProp = theActor->GetLabelTextProperty();
  if (aLabelsProp !=0)
  {
    vtkFloatingPointType c[ 3 ];
    aLabelsProp->GetColor(c);
    aLabelsColor.setRgb((int)(c[ 0 ] * 255), (int)(c[ 1 ] * 255), (int)(c[ 2 ] * 255));
    aLabelsFontFamily = aLabelsProp->GetFontFamily();
    //aLabelsFontSize = aLabelsProp->GetFontSize();
    isLabelsBold = aLabelsProp->GetBold() ? true : false;
    isLabelsItalic = aLabelsProp->GetItalic() ? true : false;
    isLabelsShadow = aLabelsProp->GetShadow() ? true : false;
  }

  myLabelsGrp->setChecked(useLabels);
  myLabelNumber->setValue(nbLabels);
  myLabelOffset->setValue(anOffset);
  myLabelFormat->setText(aFormat);
  myLabelsFont->SetData(aLabelsColor, aLabelsFontFamily, aLabelsFontSize, isLabelsBold, isLabelsItalic, isLabelsShadow);

  // Tick marks
  bool useTickMarks = theActor->GetTickVisibility();
  int aTickLength = theActor->GetTickLength();

  myTicksGrp->setChecked(useTickMarks);
  myTickLength->setValue(aTickLength);

  return true;
}

bool SVTK_CubeAxesDlg::AxisWidget::Apply(vtkAxisActor2D* theActor)
{
   if (theActor == 0)
    return false;

  // Name

  theActor->SetTitleVisibility(myNameGrp->isChecked() ? 1 : 0);
  theActor->SetTitle(myAxisName->text().toLatin1());

  QColor aTitleColor(255, 255, 255);
  int aTitleFontFamily = VTK_ARIAL;
  int aTitleFontSize = 12;
  bool isTitleBold = false;
  bool isTitleItalic = false;
  bool isTitleShadow = false;

  myNameFont->GetData(aTitleColor, aTitleFontFamily, aTitleFontSize, isTitleBold, isTitleItalic, isTitleShadow);
  vtkTextProperty* aTitleProp = theActor->GetTitleTextProperty();
  if (aTitleProp)
  {
    aTitleProp->SetColor(aTitleColor.red() / 255.,
                          aTitleColor.green() / 255.,
                          aTitleColor.blue() / 255.);
    aTitleProp->SetFontFamily(aTitleFontFamily);
    //aTitleProp->SetFontSize(aTitleFontSize);

    aTitleProp->SetBold(isTitleBold ? 1 : 0);
    aTitleProp->SetItalic(isTitleItalic ? 1 : 0);
    aTitleProp->SetShadow(isTitleShadow ? 1 : 0);

    aTitleProp->Modified();
    theActor->SetTitleTextProperty(aTitleProp);
  }

  double aFontFactor = (double)aTitleFontSize / (double)DEFAULT_FONT_SIZE;
  theActor->SetFontFactor( aFontFactor );

  // Labels

  theActor->SetLabelVisibility(myLabelsGrp->isChecked() ? 1 : 0);

  int nbLabels = myLabelNumber->value();
  theActor->SetNumberOfLabels(nbLabels);

  int anOffset = myLabelOffset->value();
  theActor->SetTickOffset(anOffset);

  QString aFormat = myLabelFormat->text();
  theActor->SetLabelFormat( aFormat.toLatin1().constData() );

  QColor aLabelsColor(255, 255, 255);
  int aLabelsFontFamily = VTK_ARIAL;
  int aLabelsFontSize = 12;
  bool isLabelsBold = false;
  bool isLabelsItalic = false;
  bool isLabelsShadow = false;

  myLabelsFont->GetData(aLabelsColor, aLabelsFontFamily, aLabelsFontSize, isLabelsBold, isLabelsItalic, isLabelsShadow);
  vtkTextProperty* aLabelsProp = theActor->GetLabelTextProperty();
  if (aLabelsProp)
  {
    aLabelsProp->SetColor(aLabelsColor.red() / 255.,
                          aLabelsColor.green() / 255.,
                          aLabelsColor.blue() / 255.);
    aLabelsProp->SetFontFamily(aLabelsFontFamily);
    //aLabelsProp->SetFontSize(aLabelsFontSize);

    aLabelsProp->SetBold(isLabelsBold ? 1 : 0);
    aLabelsProp->SetItalic(isLabelsItalic ? 1 : 0);
    aLabelsProp->SetShadow(isLabelsShadow ? 1 : 0);

    aLabelsProp->Modified();
    theActor->SetLabelTextProperty(aLabelsProp);
  }

  double aLabelFactor = (double)aLabelsFontSize / (double)aTitleFontSize;
  theActor->SetLabelFactor( aLabelFactor );

  // Tick marks
  theActor->SetTickVisibility(myTicksGrp->isChecked());
  int aTickLength = myTickLength->value();
  theActor->SetTickLength(aTickLength);

  return true;
}

/*
  Class       : SVTK_CubeAxesDlg
  Description : Dialog for specifynig cube axes properties
*/

/*!
  Constructor
*/
SVTK_CubeAxesDlg::SVTK_CubeAxesDlg(QtxAction* theAction,
				   SVTK_ViewWindow* theParent,
				   const char* theName):
  SVTK_DialogBase(theAction,
		  theParent, 
		  theName),
  myMainWindow(theParent)
{
  setWindowTitle(tr("CAPTION"));

  QVBoxLayout* aLay = new QVBoxLayout(this);
  aLay->setMargin(5);
  aLay->setSpacing(5);
  aLay->addWidget(createMainFrame(this));
  aLay->addWidget(createButtonFrame(this));

  connect(theParent, SIGNAL(Show( QShowEvent * )), this, SLOT(onParentShow()));
  connect(theParent, SIGNAL(Hide( QHideEvent * )), this, SLOT(onParentHide()));
}

/*!
  Create frame containing dialog's input fields
*/
QWidget* SVTK_CubeAxesDlg::createMainFrame(QWidget* theParent)
{
  QFrame* aFrame = new QFrame(theParent);

  myTabWg = new QTabWidget(aFrame);

  myAxes[ 0 ] = new AxisWidget(myTabWg);
  myAxes[ 1 ] = new AxisWidget(myTabWg);
  myAxes[ 2 ] = new AxisWidget(myTabWg);

  myTabWg->addTab(myAxes[ 0 ], tr("X_AXIS"));
  myTabWg->addTab(myAxes[ 1 ], tr("Y_AXIS"));
  myTabWg->addTab(myAxes[ 2 ], tr("Z_AXIS"));

  myIsVisible = new QCheckBox(tr("IS_VISIBLE"), aFrame);

  QVBoxLayout* aLay = new QVBoxLayout(aFrame);
  aLay->setMargin(0);
  aLay->setSpacing(5);
  aLay->addWidget(myTabWg);
  aLay->addWidget(myIsVisible);

  return aFrame;
}

/*!
  Create frame containing buttons
*/
QWidget* SVTK_CubeAxesDlg::createButtonFrame(QWidget* theParent)
{
  QFrame* aFrame = new QFrame(theParent);
  aFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);

  myOkBtn    = new QPushButton(tr("BUT_OK"), aFrame);
  myApplyBtn = new QPushButton(tr("BUT_APPLY"), aFrame);
  myCloseBtn = new QPushButton(tr("BUT_CLOSE"), aFrame);

  QSpacerItem* aSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

  QHBoxLayout* aLay = new QHBoxLayout(aFrame);
  aLay->setMargin(5);
  aLay->setSpacing(5);

  aLay->addWidget(myOkBtn);
  aLay->addWidget(myApplyBtn);
  aLay->addItem(aSpacer);
  aLay->addWidget(myCloseBtn);

  connect(myOkBtn,    SIGNAL(clicked()), SLOT(onOk()));
  connect(myApplyBtn, SIGNAL(clicked()), SLOT(onApply()));
  connect(myCloseBtn, SIGNAL(clicked()), SLOT(onClose()));

  return aFrame;
}

/*!
  Destructor
*/
SVTK_CubeAxesDlg::~SVTK_CubeAxesDlg()
{
}

/*!
  Update dialog fields, connect signals and slots, show dialog
*/
void SVTK_CubeAxesDlg::Update()
{
  myActor = myMainWindow->GetCubeAxes();

  myAxes[ 0 ]->ReadData(myActor->GetXAxisActor2D());
  myAxes[ 1 ]->ReadData(myActor->GetYAxisActor2D());
  myAxes[ 2 ]->ReadData(myActor->GetZAxisActor2D());

  myAxes[ 0 ]->SetAdjustRange( myActor->GetAdjustXRange() );
  myAxes[ 1 ]->SetAdjustRange( myActor->GetAdjustYRange() );
  myAxes[ 2 ]->SetAdjustRange( myActor->GetAdjustZRange() );

  myIsVisible->setChecked(myActor->GetVisibility() ? true : false);
}

/*!
  Show/hide tab with X-dimension controls
*/
void SVTK_CubeAxesDlg::SetDimensionXEnabled( const bool theIsEnabled )
{
  myTabWg->setTabEnabled( 0, theIsEnabled );
}

/*!
  Show/hide tab with Y-dimension controls
*/
void SVTK_CubeAxesDlg::SetDimensionYEnabled( const bool theIsEnabled )
{
  myTabWg->setTabEnabled( 1, theIsEnabled );
}

/*!
  Show/hide tab with Z-dimension controls
*/
void SVTK_CubeAxesDlg::SetDimensionZEnabled( const bool theIsEnabled )
{
  myTabWg->setTabEnabled( 2, theIsEnabled );
}

/*!
  Show hide the checkboxes "Adjust automatically" in each tab
*/
void SVTK_CubeAxesDlg::SetAdjustRangeEnabled( const bool theIsEnabled )
{
  for( int i = 0; i <= 2; i++ )
    myAxes[ i ]->SetAdjustRangeEnabled( theIsEnabled );
}

/*!
  Verify validity of entry data
*/
bool SVTK_CubeAxesDlg::isValid() const
{
  return true;
}

/*!
  Verify validity of entry data
*/
bool SVTK_CubeAxesDlg::onApply()
{
  bool isOk = true;

  try
  {
    QWidget *aCurrWid = this->focusWidget();
    aCurrWid->clearFocus();
    aCurrWid->setFocus();

    isOk = isOk && myAxes[ 0 ]->Apply(myActor->GetXAxisActor2D());
    isOk = isOk && myAxes[ 1 ]->Apply(myActor->GetYAxisActor2D());
    isOk = isOk && myAxes[ 2 ]->Apply(myActor->GetZAxisActor2D());

    myActor->SetAdjustXRange( myAxes[0]->GetAdjustRange() );
    myActor->SetAdjustYRange( myAxes[1]->GetAdjustRange() );
    myActor->SetAdjustZRange( myAxes[2]->GetAdjustRange() );

    if (myIsVisible->isChecked())
      myActor->VisibilityOn();
    else
      myActor->VisibilityOff();

    myMainWindow->UpdateCubeAxes( false );

    if (isOk)
      myMainWindow->Repaint();
  }
  catch(...)
  {
    isOk = false;
  }

  return isOk;
}

/*!
  SLOT called when "Ok" button pressed.
*/
void SVTK_CubeAxesDlg::onOk()
{
  if (onApply())
    onClose();
}

/*!
  SLOT: called when "Close" button pressed. Close dialog
*/
void SVTK_CubeAxesDlg::onClose()
{
  reject();
}
