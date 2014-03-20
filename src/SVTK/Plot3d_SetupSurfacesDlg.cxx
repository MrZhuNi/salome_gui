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
//  SALOME Plot3d : implementation of desktop and GUI kernel
//
//  File   : Plot3d_SetupSurfacesDlg.cxx
//  Author : Sergey LITONIN

#include "Plot3d_SetupSurfacesDlg.h"

#include <QtxGroupBox.h>

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>

#include <QComboBox>
#include <QHeaderView>
#include <QLayout>
#include <QTableWidget>
#include <QToolButton>

#define TEXT_COL 0

/*!
  \brief Constructor
  \param theParent parent widget
*/
Plot3d_SetupSurfacesDlg::Plot3d_SetupSurfacesDlg( QWidget* theParent )
: QtxDialog( theParent, true, false, QtxDialog::OKCancel )
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  setWindowTitle( tr( "SETUP_SURFACES" ) );

  QFrame* aMainFrame = mainFrame();

  QVBoxLayout* aMainLay = new QVBoxLayout( aMainFrame );
  aMainLay->setMargin( 5 );

  myGrp = new QtxGroupBox( aMainFrame ); 
  myGrp->setTitle( tr( "PARAMETERS" ) );
  aMainLay->addWidget( myGrp );

  QVBoxLayout* aLay = new QVBoxLayout( myGrp );
  aLay->setMargin( 0 );

  // Create table
  myTable = new QTableWidget( myGrp );
  myTable->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

  aLay->addWidget( myTable );

  myTable->setRowCount( 0 );
  myTable->setColumnCount( 1 );

  QStringList aLabels;
  aLabels.append( tr( "TEXT" ) );
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
  myTable->horizontalHeader()->setHighlightSections( false );

  // Minus button
  QPixmap minusPix = aResMgr->loadPixmap( "VTKViewer", tr( "ICON_SVTK_MINUS" ) );
  myRemoveBtn = new QToolButton( 0 );
  myRemoveBtn->setIcon( minusPix );
  myRemoveBtn->setFixedSize( minusPix.size() );
  myGrp->insertTitleWidget( myRemoveBtn );

  connect( myRemoveBtn, SIGNAL( clicked() ), SLOT( onRemove() ) );
  connect( myTable, SIGNAL( valueChanged( int, int ) ), SLOT( onValueChanged( int, int ) ) );

  setButtonPosition( Right, Cancel );
  setMinimumWidth( 500 );
  setMinimumHeight( 250 );
}

/*!
  \brief Destructor
*/
Plot3d_SetupSurfacesDlg::~Plot3d_SetupSurfacesDlg()
{
}

/*!
  \brief Sets text in specified cell
  \param theRow row
  \param theCol column
  \param theText text
*/
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
void Plot3d_SetupSurfacesDlg::SetParameters( const QVector< QString >& theTexts )
{
  int nbRows = theTexts.size();
  
  myTable->setRowCount( nbRows );

  for ( int i = 0; i < nbRows; i++ )
  {
    QString aText = theTexts[ i ];
    aText.replace( QChar('\n'), QChar( ' ' ) );
    setText( i, TEXT_COL, aText );
  }

  myRemovedIndexes.clear();
}

//=============================================================================
// Function : GetParameters
// Purpose  : 
//=============================================================================
void Plot3d_SetupSurfacesDlg::GetParameters( QVector< QString >& theTexts ) const
{
  int nbRows = myTable->rowCount();

  theTexts.resize( nbRows );

  for ( int i = 0; i < nbRows; i++ )
  {
    QTableWidgetItem* it = myTable->item( i, TEXT_COL );
    theTexts[ i ] = it ? it->text() : "";
    theTexts[ i ].replace( ' ', '\n' );
  }
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
