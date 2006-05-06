// Copyright (C) 2005  CEA/DEN, EDF R&D, OPEN CASCADE, PRINCIPIA R&D
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
// See http://www.salome-platform.org/
//
#include "OCCViewer_ClippingDlg.h"

#include <QtxDblSpinBox.h>
#include <QtxAction.h>

#include "SUIT_Session.h"
#include "SUIT_ViewWindow.h"
#include "OCCViewer_ViewWindow.h"
#include "OCCViewer_ViewPort3d.h"

#include <V3d_View.hxx>
//#include <V3d.hxx>
#include <Geom_Plane.hxx>
#include <Prs3d_Presentation.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_InteractiveContext.hxx>
#include <IntAna_IntConicQuad.hxx>
#include <gp_Lin.hxx>
#include <gp_Pln.hxx>

// QT Includes
#include <qapplication.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>

/*!
  Constructor
  \param view - view window
  \param parent - parent widget
  \param name - dialog name
  \param modal - is this dialog modal
  \param fl - flags
*/
OCCViewer_ClippingDlg::OCCViewer_ClippingDlg( OCCViewer_ViewWindow* view, QWidget* parent, const char* name, bool modal, WFlags fl )
: QDialog( parent, "OCCViewer_ClippingDlg", modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu ),
  myView( view )
{
  setCaption( tr( "Clipping" ) );
  
  QVBoxLayout* topLayout = new QVBoxLayout( this );
  topLayout->setMargin( 11 ); topLayout->setSpacing( 6 );
  
  /***************************************************************/
  GroupPoint = new QGroupBox( this, "GroupPoint" );
  GroupPoint->setTitle( tr("Base point") );
  GroupPoint->setColumnLayout(0, Qt::Vertical );
  GroupPoint->layout()->setSpacing( 0 );
  GroupPoint->layout()->setMargin( 0 );
  QGridLayout* GroupPointLayout = new QGridLayout( GroupPoint->layout() );
  GroupPointLayout->setAlignment( Qt::AlignTop );
  GroupPointLayout->setSpacing( 6 );
  GroupPointLayout->setMargin( 11 );
  
  // Controls
  const double min = -1e+06;
  const double max =  1e+06;
  const double step = 5;

  TextLabelX = new QLabel( GroupPoint, "TextLabelX" );
  TextLabelX->setText( tr("X:") );
  GroupPointLayout->addWidget( TextLabelX, 0, 0 );
  
  SpinBox_X = new QtxDblSpinBox( min, max, step, GroupPoint, "SpinBox_X" );
  GroupPointLayout->addWidget( SpinBox_X, 0, 1 );

  TextLabelY = new QLabel( GroupPoint, "TextLabelY" );
  TextLabelY->setText( tr("Y:") );
  GroupPointLayout->addWidget( TextLabelY, 0, 2 );

  SpinBox_Y = new QtxDblSpinBox( min, max, step, GroupPoint, "SpinBox_Y" );
  GroupPointLayout->addWidget( SpinBox_Y, 0, 3 );

  TextLabelZ = new QLabel( GroupPoint, "TextLabelZ" );
  TextLabelZ->setText( tr("Z:") );
  GroupPointLayout->addWidget( TextLabelZ, 0, 4 );

  SpinBox_Z = new QtxDblSpinBox( min, max, step, GroupPoint, "SpinBox_Z" );
  GroupPointLayout->addWidget( SpinBox_Z, 0, 5 );

  resetButton  = new QPushButton( GroupPoint, "resetButton" );
  resetButton->setText( tr( "Reset"  ) );
  GroupPointLayout->addWidget( resetButton, 0, 6 );

  /***************************************************************/
  GroupDirection = new QGroupBox( this, "GroupDirection" );
  GroupDirection->setTitle( tr("Direction") );
  GroupDirection->setColumnLayout(0, Qt::Vertical );
  GroupDirection->layout()->setSpacing( 0 );
  GroupDirection->layout()->setMargin( 0 );
  QGridLayout* GroupDirectionLayout = new QGridLayout( GroupDirection->layout() );
  GroupDirectionLayout->setAlignment( Qt::AlignTop );
  GroupDirectionLayout->setSpacing( 6 );
  GroupDirectionLayout->setMargin( 11 );
  
  // Controls
  TextLabelDx = new QLabel( GroupDirection, "TextLabelDx" );
  TextLabelDx->setText( tr("Dx:") );
  GroupDirectionLayout->addWidget( TextLabelDx, 0, 0 );
  
  SpinBox_Dx = new QtxDblSpinBox( min, max, step, GroupDirection, "SpinBox_Dx" );
  GroupDirectionLayout->addWidget( SpinBox_Dx, 0, 1 );

  TextLabelDy = new QLabel( GroupDirection, "TextLabelDy" );
  TextLabelDy->setText( tr("Dy:") );
  GroupDirectionLayout->addWidget( TextLabelDy, 0, 2 );
  
  SpinBox_Dy = new QtxDblSpinBox( min, max, step, GroupDirection, "SpinBox_Dy" );
  GroupDirectionLayout->addWidget( SpinBox_Dy, 0, 3 );

  TextLabelDz = new QLabel( GroupDirection, "TextLabelDz" );
  TextLabelDz->setText( tr("Dz:") );
  GroupDirectionLayout->addWidget( TextLabelDz, 0, 4 );
  
  SpinBox_Dz = new QtxDblSpinBox( min, max, step, GroupDirection, "SpinBox_Dz" );
  GroupDirectionLayout->addWidget( SpinBox_Dz, 0, 5 );

  invertButton  = new QPushButton( GroupDirection, "invertButton" );
  invertButton->setText( tr( "Invert"  ) );
  GroupDirectionLayout->addWidget( invertButton, 0, 6 );
 
  DirectionCB = new QComboBox( GroupDirection, "DirectionCB" );
  DirectionCB->insertItem(tr("CUSTOM"));
  DirectionCB->insertItem(tr("||X-Y"));
  DirectionCB->insertItem(tr("||Y-Z"));
  DirectionCB->insertItem(tr("||Z-X"));
  GroupDirectionLayout->addMultiCellWidget( DirectionCB, 1, 1, 0, 5 );
  
  /***************************************************************/
  
  PreviewChB = new QCheckBox( tr("Preview") ,this, "PreviewChB" );
  PreviewChB->setChecked( true );
  
  /***************************************************************/
  QGroupBox* GroupButtons = new QGroupBox( this, "GroupButtons" );
  GroupButtons->setColumnLayout(0, Qt::Vertical );
  GroupButtons->layout()->setMargin( 0 ); GroupButtons->layout()->setSpacing( 0 ); 
  QHBoxLayout* GroupButtonsLayout = new QHBoxLayout( GroupButtons->layout() );
  GroupButtonsLayout->setAlignment( Qt::AlignTop );
  GroupButtonsLayout->setMargin( 11 ); GroupButtonsLayout->setSpacing( 6 );
  
  buttonApply = new QPushButton( GroupButtons, "buttonApply" );
  buttonApply->setText( tr( "BUT_APPLY"  ) );
  buttonApply->setAutoDefault( TRUE ); 
  buttonApply->setDefault( TRUE );
  GroupButtonsLayout->addWidget( buttonApply );
  
  GroupButtonsLayout->addStretch();
  
  buttonClose = new QPushButton( GroupButtons, "buttonClose" );
  buttonClose->setText( tr( "BUT_CLOSE"  ) );
  buttonClose->setAutoDefault( TRUE );
  GroupButtonsLayout->addWidget( buttonClose );
  /***************************************************************/
  
  topLayout->addWidget( GroupPoint );
  topLayout->addWidget( GroupDirection );
  
  topLayout->addWidget( PreviewChB );

  topLayout->addWidget( GroupButtons );

  /* initializations */

  SpinBox_X->setValue( 0.0 );
  SpinBox_Y->setValue( 0.0 );
  SpinBox_Z->setValue( 0.0 );

  SpinBox_Dx->setValue( 1.0 );
  SpinBox_Dy->setValue( 1.0 );
  SpinBox_Dz->setValue( 1.0 );

  /* signals and slots connections */
  connect( resetButton,  SIGNAL (clicked() ), this, SLOT( onReset() ) );
  connect( invertButton, SIGNAL (clicked() ), this, SLOT( onInvert() ) ) ;

  connect( SpinBox_X,  SIGNAL ( valueChanged( double ) ),  this, SLOT( onValueChanged() ) );
  connect( SpinBox_Y,  SIGNAL ( valueChanged( double ) ),  this, SLOT( onValueChanged() ) );
  connect( SpinBox_Z,  SIGNAL ( valueChanged( double ) ),  this, SLOT( onValueChanged() ) );
  connect( SpinBox_Dx, SIGNAL ( valueChanged( double ) ),  this, SLOT( onValueChanged() ) );
  connect( SpinBox_Dy, SIGNAL ( valueChanged( double ) ),  this, SLOT( onValueChanged() ) );
  connect( SpinBox_Dz, SIGNAL ( valueChanged( double ) ),  this, SLOT( onValueChanged() ) );
   
  connect( DirectionCB, SIGNAL ( activated ( int ) ), this, SLOT( onModeChanged( int ) ) ) ;

  connect( PreviewChB, SIGNAL ( toggled ( bool ) ), this, SLOT( onPreview( bool ) ) ) ;
  
  connect( buttonClose, SIGNAL( clicked() ), this, SLOT( ClickOnClose() ) ) ;
  connect( buttonApply, SIGNAL( clicked() ), this, SLOT( ClickOnApply() ) );
  
  myBusy = false;
}

/*!
  Destructor
  Destroys the object and frees any allocated resources
*/
OCCViewer_ClippingDlg::~ OCCViewer_ClippingDlg()
{
  // no need to delete child widgets, Qt does it all for us
}


/*!
  Custom handling of close event: erases preview
*/
void OCCViewer_ClippingDlg::closeEvent( QCloseEvent* e )
{
  erasePreview();
  
  // Set the clipping plane back
  Handle(V3d_View) aView3d = myView->getViewPort()->getView();
  if ( !aView3d.IsNull() && !myClippingPlane.IsNull() )
    aView3d->SetPlaneOn( myClippingPlane );
  
  if (!myView->isCuttingPlane())
    myAction->setOn( false );
  
  QDialog::closeEvent( e );
}


/*!
  Custom handling of show event: displays preview
*/
void OCCViewer_ClippingDlg::showEvent( QShowEvent* e )
{
  ReserveClippingPlane();
  
  QDialog::showEvent( e );
  onPreview( PreviewChB->isChecked() );
}


/*!
  Custom handling of hide event: erases preview
*/
void OCCViewer_ClippingDlg::hideEvent( QHideEvent* e )
{
  erasePreview();
  QDialog::hideEvent( e );
}


/*!
  SLOT on close button click: erases preview and rejects dialog
*/
void OCCViewer_ClippingDlg::ClickOnClose()
{
  erasePreview();

  // Set the clipping plane back
  Handle(V3d_View) aView3d = myView->getViewPort()->getView();
  if ( !aView3d.IsNull() && !myClippingPlane.IsNull() )
    aView3d->SetPlaneOn( myClippingPlane );

  if (!myView->isCuttingPlane())
    myAction->setOn( false );
  
  reject();
}


/*!
  SLOT on apply button click: sets cutting plane
*/
void OCCViewer_ClippingDlg::ClickOnApply()
{
  qApp->processEvents();
  QApplication::setOverrideCursor( Qt::WaitCursor );
  qApp->processEvents();
  
  myView->setCuttingPlane( true, SpinBox_X->value() , SpinBox_Y->value() , SpinBox_Z->value(),
	                         SpinBox_Dx->value(), SpinBox_Dy->value(), SpinBox_Dz->value() );
  
  QApplication::restoreOverrideCursor(); 
  
  erasePreview();
  
  ReserveClippingPlane();
}

/*!
  SLOT on reset button click: sets default values
*/
void OCCViewer_ClippingDlg::onReset()
{
  myBusy = true;
  SpinBox_X->setValue(0);
  SpinBox_Y->setValue(0);
  SpinBox_Z->setValue(0);
  myBusy = false;

  if ( PreviewChB->isChecked() )
    {
      erasePreview();
      displayPreview();
    }
}

/*!
  SLOT on invert button click: inverts normal of cutting plane
*/
void OCCViewer_ClippingDlg::onInvert()
{
  double Dx = SpinBox_Dx->value();
  double Dy = SpinBox_Dy->value();
  double Dz = SpinBox_Dz->value();
  
  myBusy = true;
  SpinBox_Dx->setValue( -Dx );
  SpinBox_Dy->setValue( -Dy );
  SpinBox_Dz->setValue( -Dz );
  myBusy = false;

  if ( PreviewChB->isChecked() )
    {
      erasePreview();
      displayPreview();
    }
}

/*!
  SLOT: called on mode changed
*/
void OCCViewer_ClippingDlg::onModeChanged( int mode )
{
  bool isUserMode = (mode==0);
  
  TextLabelX->setEnabled( isUserMode );
  TextLabelY->setEnabled( isUserMode );
  TextLabelZ->setEnabled( isUserMode );

  SpinBox_X->setEnabled( isUserMode );
  SpinBox_Y->setEnabled( isUserMode );
  SpinBox_Z->setEnabled( isUserMode );

  TextLabelDx->setEnabled( isUserMode );
  TextLabelDy->setEnabled( isUserMode );
  TextLabelDz->setEnabled( isUserMode );

  SpinBox_Dx->setEnabled( isUserMode );
  SpinBox_Dy->setEnabled( isUserMode );
  SpinBox_Dz->setEnabled( isUserMode );
  
  if ( isUserMode )
    return;

  double aDx = 0, aDy = 0, aDz = 0;

  if ( mode == 1 )
    {
      aDz = 1;
      TextLabelZ->setEnabled( true );
      SpinBox_Z->setEnabled( true );
      SpinBox_Z->setFocus();
    }
  else if ( mode == 2 )
    {
      aDx = 1;
      TextLabelX->setEnabled( true );
      SpinBox_X->setEnabled( true );
      SpinBox_X->setFocus();
    }
  else if ( mode == 3 )
    {
      aDy = 1;
      TextLabelY->setEnabled( true );
      SpinBox_Y->setEnabled( true );
      SpinBox_Y->setFocus();
    }
  
  myBusy = true;
  SpinBox_Dx->setValue( aDx );
  SpinBox_Dy->setValue( aDy );
  SpinBox_Dz->setValue( aDz );
  myBusy = false;

  if ( PreviewChB->isChecked() )
    {
      erasePreview();
      displayPreview();
    }
}


/*!
  Displays preview of clipping plane
*/
void OCCViewer_ClippingDlg::displayPreview()
{
  if ( myBusy || !isValid() )
    return;

  OCCViewer_Viewer* anOCCViewer = (OCCViewer_Viewer*)myView->getViewManager()->getViewModel();
  if (!anOCCViewer)
    return;
  
  Handle(AIS_InteractiveContext) ic = anOCCViewer->getAISContext();

  double aXMin, aYMin, aZMin, aXMax, aYMax, aZMax;
  aXMin = aYMin = aZMin = DBL_MAX;
  aXMax = aYMax = aZMax = -DBL_MAX;

  bool isFound = false;
  AIS_ListOfInteractive aList;
  ic->DisplayedObjects( aList );
  for ( AIS_ListIteratorOfListOfInteractive it( aList ); it.More(); it.Next() )
  {
    Handle(AIS_InteractiveObject) anObj = it.Value();
    if ( !anObj.IsNull() && anObj->HasPresentation() &&
         !anObj->IsKind( STANDARD_TYPE(AIS_Plane) ) )
    {
      Handle(Prs3d_Presentation) aPrs = anObj->Presentation();
      if ( !aPrs->IsEmpty() && !aPrs->IsInfinite() )
      {
        isFound = true;
        double xmin, ymin, zmin, xmax, ymax, zmax;
        aPrs->MinMaxValues( xmin, ymin, zmin, xmax, ymax, zmax );
        aXMin = QMIN( aXMin, xmin );  aXMax = QMAX( aXMax, xmax );
        aYMin = QMIN( aYMin, ymin );  aYMax = QMAX( aYMax, ymax );
        aZMin = QMIN( aZMin, zmin );  aZMax = QMAX( aZMax, zmax );
      }
    }
  }

  double aSize = 50;
  
  gp_Pnt aBasePnt( SpinBox_X->value(),  SpinBox_Y->value(),  SpinBox_Z->value() );
  gp_Dir aNormal( SpinBox_Dx->value(), SpinBox_Dy->value(), SpinBox_Dz->value() );
  gp_Pnt aCenter = aBasePnt;
  
  if ( isFound )
    {
      // compute clipping plane size
      aCenter = gp_Pnt( ( aXMin + aXMax ) / 2, ( aYMin + aYMax ) / 2, ( aZMin + aZMax ) / 2 );
      double aDiag = aCenter.Distance(gp_Pnt(aXMax, aYMax, aZMax ))*2;
      aSize = aDiag * 1.1;

      // compute clipping plane center ( redefine the base point )
      IntAna_IntConicQuad intersector = IntAna_IntConicQuad();
      
      intersector.Perform( gp_Lin( aCenter, aNormal), gp_Pln( aBasePnt, aNormal), Precision::Confusion() );
      if ( intersector.IsDone() && intersector.NbPoints() == 1 )
	aBasePnt = intersector.Point( 1 );
    }
  
  myPreviewPlane = new AIS_Plane( new Geom_Plane( aBasePnt, aNormal ) );
  myPreviewPlane->SetSize( aSize, aSize );
  
  // Deactivate clipping planes
  myView->getViewPort()->getView()->SetPlaneOff();

  ic->Display( myPreviewPlane, 1, -1, false );
  ic->SetWidth( myPreviewPlane, 10, false );
  ic->SetMaterial( myPreviewPlane, Graphic3d_NOM_PLASTIC, false );
  ic->SetTransparency( myPreviewPlane, 0.5, false );
  ic->SetColor( myPreviewPlane, Quantity_Color( 85 / 255., 85 / 255., 255 / 255., Quantity_TOC_RGB ), false );
  
  anOCCViewer->update();
}


/*!
  Erases preview of clipping plane
*/
void OCCViewer_ClippingDlg::erasePreview ()
{
  OCCViewer_Viewer* anOCCViewer = (OCCViewer_Viewer*)myView->getViewManager()->getViewModel();
  if (!anOCCViewer)
    return;
  
  Handle(AIS_InteractiveContext) ic = anOCCViewer->getAISContext();
  
  if ( !myPreviewPlane.IsNull() && ic->IsDisplayed( myPreviewPlane ) )
    {
      ic->Erase( myPreviewPlane, false, false );
      ic->Remove( myPreviewPlane, false );
      myPreviewPlane.Nullify();
    }
  
  anOCCViewer->update();
}


/*!
  SLOT: called on value changes (co-ordinates of point or normal)
*/
void OCCViewer_ClippingDlg::onValueChanged()
{
  if ( PreviewChB->isChecked() )
    {
      erasePreview();
      displayPreview();
    }
}


/*!
  SLOT: called on preview check box toggled
*/
void OCCViewer_ClippingDlg::onPreview( bool on )
{
  erasePreview();

  if ( on ) 
    displayPreview();
}

/*!
  \return true if plane parameters are valid
*/
bool OCCViewer_ClippingDlg::isValid()
{
  return ( SpinBox_Dx->value()!=0 || SpinBox_Dy->value()!=0 || SpinBox_Dz->value()!=0 );
}

/*!
  Remember the current clipping plane
*/
void OCCViewer_ClippingDlg::ReserveClippingPlane()
{
  Handle(V3d_View) aView3d = myView->getViewPort()->getView();
  if ( !aView3d.IsNull() )
    {
      aView3d->InitActivePlanes();
      if ( aView3d->MoreActivePlanes() )
	myClippingPlane = aView3d->ActivePlane();
    }
}
