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
#include "Plot3d_SetupSurfacesDlg.h"

#include <QtxGroupBox.h>

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>

#include <QComboBox>
#include <QHeaderView>
#include <QLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QToolButton>

#define TEXT_COL        0
#define COLOR_SCALE_COL 1

//=============================================================================
// Function : Plot3d_SetupSurfacesDlg
// Purpose  : Constructor
//=============================================================================
Plot3d_SetupSurfacesDlg::Plot3d_SetupSurfacesDlg( QWidget* theParent )
: QtxDialog( theParent, true, false, QtxDialog::OKCancel )
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  setWindowTitle( tr( "SETUP_SURFACES" ) );

  QFrame* aMainFrame = mainFrame();

  QVBoxLayout* aMainLay = new QVBoxLayout( aMainFrame );
  aMainLay->setMargin( 5 );

  QtxGroupBox* aParamGrp = new QtxGroupBox( aMainFrame ); 
  aParamGrp->setTitle( tr( "PARAMETERS" ) );
  aMainLay->addWidget( aParamGrp );

  QVBoxLayout* aLay = new QVBoxLayout( aParamGrp );
  aLay->setMargin( 5 );
  aLay->setSpacing( 5 );

  // Create table
  myTable = new QTableWidget( aParamGrp );
  myTable->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

  aLay->addWidget( myTable );

  myTable->setRowCount( 0 );
  myTable->setColumnCount( 2 );

  QStringList aLabels;
  aLabels.append( tr( "TEXT" ) );
  aLabels.append( tr( "COLOR_SCALE" ) );
  myTable->setHorizontalHeaderLabels( aLabels );
  myTable->verticalHeader()->hide();
  myTable->setSelectionMode( QTableWidget::NoSelection );

  QComboBox* aCombo = new QComboBox( 0 );
  myTable->verticalHeader()->setDefaultSectionSize( aCombo->sizeHint().height() );
  delete aCombo;

  int l, t, r, b;
  myTable->getContentsMargins( &l, &t, &r, &b );
  myTable->setContentsMargins( 0, t, r, b );

  myTable->horizontalHeader()->setResizeMode( TEXT_COL, QHeaderView::Stretch );
  myTable->horizontalHeader()->setResizeMode( COLOR_SCALE_COL, QHeaderView::ResizeToContents );
  myTable->horizontalHeader()->setHighlightSections( false );

  // Minus button
  QPixmap minusPix = aResMgr->loadPixmap( "VTKViewer", tr( "ICON_PLOT3D_MINUS" ) );
  QToolButton* aRemoveBtn = new QToolButton( 0 );
  aRemoveBtn->setIcon( minusPix );
  aRemoveBtn->setFixedSize( minusPix.size() );
  aParamGrp->insertTitleWidget( aRemoveBtn );

  connect( aRemoveBtn, SIGNAL( clicked() ), SLOT( onRemove() ) );

  // "Edit global color scale" button
  QPushButton* anEditGlobalBtn = new QPushButton( tr( "EDIT_GLOBAL_COLOR_SCALE" ), aParamGrp ); 
  aLay->addWidget( anEditGlobalBtn );

  connect( anEditGlobalBtn, SIGNAL( clicked() ), SLOT( onGlobalColorScale() ) );

  setButtonPosition( Right, Cancel );
  setMinimumWidth( 300 );
  setMinimumHeight( 250 );
}

//=============================================================================
// Function : Plot3d_SetupSurfacesDlg
// Purpose  : Destructor
//=============================================================================
Plot3d_SetupSurfacesDlg::~Plot3d_SetupSurfacesDlg()
{
}

//=============================================================================
// Function : setText
// Purpose  : 
//=============================================================================
void Plot3d_SetupSurfacesDlg::setText( const int theRow,
                                       const int theCol,
                                       const QString& theText )
{
  QTableWidgetItem* anItem = myTable->item( theRow, theCol );
  if ( !anItem )
  {
    anItem = new QTableWidgetItem( theText );
    myTable->setItem( theRow, theCol, anItem );
  }
  else 
    anItem->setText( theText );
}

//=============================================================================
// Function : SetParameters
// Purpose  : 
//=============================================================================
void Plot3d_SetupSurfacesDlg::SetParameters( const QStringList& theTexts,
                                             const ColorDicDataList& theColorDicDataList,
                                             const ColorDicData& theGlobalColorDicData )
{
  int nbRows = theTexts.size();
  
  myTable->setRowCount( nbRows );

  for ( int i = 0; i < nbRows; i++ )
  {
    QString aText = theTexts[ i ];
    //aText.replace( QChar('\n'), QChar( ' ' ) );
    setText( i, TEXT_COL, aText );

    QPushButton* aBtn = new QPushButton( tr( "EDIT" ), myTable );
    myTable->setCellWidget( i, COLOR_SCALE_COL, aBtn );
    connect( aBtn, SIGNAL( clicked() ), SLOT( onColorScaleBtn() ) );
  }

  myColorDicDataList = theColorDicDataList;

  myGlobalColorDicData = theGlobalColorDicData;

  myRemovedIndexes.clear();
}

//=============================================================================
// Function : GetParameters
// Purpose  : 
//=============================================================================
void Plot3d_SetupSurfacesDlg::GetParameters( QStringList& theTexts,
                                             ColorDicDataList& theColorDicDataList,
                                             ColorDicData& theGlobalColorDicData ) const
{
  int nbRows = myTable->rowCount();

  theTexts.clear();

  for ( int i = 0; i < nbRows; i++ )
  {
    QTableWidgetItem* it = myTable->item( i, TEXT_COL );
    QString aText = it ? it->text() : "";
    //aText.replace( ' ', '\n' );
    theTexts << aText;
  }

  theColorDicDataList = myColorDicDataList;

  theGlobalColorDicData = myGlobalColorDicData;
}

//=============================================================================
// Function : GetRemovedIndexes
// Purpose  : 
//=============================================================================
const QList< int >& Plot3d_SetupSurfacesDlg::GetRemovedIndexes() const
{
  return myRemovedIndexes;
}

//=============================================================================
// Function : onRemove
// Purpose  : 
//=============================================================================
void Plot3d_SetupSurfacesDlg::onRemove()
{
  QList< int > toRemove;

  int aCurrRow = myTable->currentRow();
  int aCurrCol = myTable->currentColumn();

  // get selected rows
  QSet< int > aSelRows;
  QList<QTableWidgetSelectionRange> aRegs = myTable->selectedRanges();
  QList<QTableWidgetSelectionRange>::iterator selIter;
  for ( selIter = aRegs.begin(); selIter != aRegs.end(); ++selIter )
  {
    const QTableWidgetSelectionRange& aReg = *selIter;
    for ( int i = aReg.topRow(), n = aReg.bottomRow(); i < n; i++ )
      aSelRows.insert( i );
  }

  int i, n;
  for ( i = 0, n = myTable->rowCount(); i < n; i++ )
  {
    if ( aSelRows.contains( i ) || aCurrRow == i )
      toRemove.append( i );
  }

  if ( !toRemove.count() )
    return;

  int nbRemoved = 0;
  QList< int >::iterator anIter;
  for ( anIter = toRemove.begin(); anIter != toRemove.end(); ++anIter )
  {
    int aRow = *anIter - nbRemoved;
    myTable->removeRow( aRow );
    myRemovedIndexes.append( aRow );
    nbRemoved++;
  }

  int nbRows = myTable->rowCount();
  if ( aCurrRow < nbRows && aCurrRow >= 0 && aCurrCol >= 0 )
    myTable->setCurrentCell( aCurrRow, aCurrCol );
  else if ( nbRows > 0 && aCurrCol >= 0 )
    myTable->setCurrentCell( nbRows - 1, aCurrCol );
}

//=============================================================================
// Function : onColorScaleBtn
// Purpose  : 
//=============================================================================
void Plot3d_SetupSurfacesDlg::onColorScaleBtn()
{
  QObject* aSender = sender();
  if( !aSender )
    return;

  int aRow = 0;
  for( int aRowRef = 0, aRowCount = myTable->rowCount(); aRowRef < aRowCount; aRowRef++ )
  {
    QWidget* aBtn = myTable->cellWidget( aRowRef, COLOR_SCALE_COL );
    if( aBtn == aSender )
    {
      aRow = aRowRef;
      break;
    }
  }

  if( aRow > myColorDicDataList.count() )
    return;

  ColorDicData aColorDicData = myColorDicDataList[ aRow ];

  Plot3d_SetupColorScaleDlg aDlg( this );
  aDlg.setData( aColorDicData );

  if( aDlg.exec() )
  {
    aColorDicData = aDlg.getData();
    myColorDicDataList[ aRow ] = aColorDicData;
  }
}

//=============================================================================
// Function : onGlobalColorScale
// Purpose  : 
//=============================================================================
void Plot3d_SetupSurfacesDlg::onGlobalColorScale()
{
  Plot3d_SetupColorScaleDlg aDlg( this, true );
  aDlg.setData( myGlobalColorDicData );

  if( aDlg.exec() )
    myGlobalColorDicData = aDlg.getData();
}
