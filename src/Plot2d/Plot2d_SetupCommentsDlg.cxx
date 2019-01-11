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
#include "Plot2d_SetupCommentsDlg.h"

#include <QtxGroupBox.h>

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>

#include <QComboBox>
#include <QHeaderView>
#include <QItemDelegate>
#include <QLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QToolButton>

#define TEXT_COL      0
#define X_COL         1
#define Y_COL         2
#define ALIGNMENT_COL 3

//=============================================================================
// Class    : Plot2d_SetupCommentsDlg::ItemDelegate
// Purpose  : 
//=============================================================================
class Plot2d_SetupCommentsDlg::ItemDelegate : public QItemDelegate
{
public:
  ItemDelegate( QObject* theParent, QValidator* theValidator )
    : myValidator( theValidator ) {}

public:
  virtual QWidget* createEditor( QWidget* theParent,
                                 const QStyleOptionViewItem& theOption,
                                 const QModelIndex& ) const
  {
    QLineEdit* aLineEdit = new QLineEdit( theParent );
    aLineEdit->setValidator( myValidator );
    return aLineEdit;
  }

private:
  QValidator* myValidator;
};

//=============================================================================
// Function : Plot2d_SetupCommentsDlg
// Purpose  : Constructor
//=============================================================================
Plot2d_SetupCommentsDlg::Plot2d_SetupCommentsDlg( QWidget* theParent )
: QtxDialog( theParent, true, false, QtxDialog::OKCancel )
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  setWindowTitle( tr( "SETUP_COMMENTS" ) );

  QValidator* aDoubleValidator = new QDoubleValidator( this );

  QFrame* aMainFrame = mainFrame();

  QtxGroupBox* aGrp = new QtxGroupBox( aMainFrame ); 
  aGrp->setTitle( tr( "PARAMETERS" ) );

  myTable = new QTableWidget( aGrp );
  myTable->setSelectionMode( QTableWidget::ExtendedSelection );

  myTable->setRowCount( 0 );
  myTable->setColumnCount( 4 );

  QStringList aLabels = QStringList() << tr( "TEXT" ) << tr( "X" ) << tr( "Y" ) << tr( "ALIGNMENT" );
  myTable->setHorizontalHeaderLabels( aLabels );

  myTable->horizontalHeader()->setResizeMode( TEXT_COL, QHeaderView::Stretch );
  myTable->horizontalHeader()->setResizeMode( X_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setResizeMode( Y_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setResizeMode( ALIGNMENT_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setHighlightSections( false );

  myTable->setColumnWidth( X_COL, 80 );
  myTable->setColumnWidth( Y_COL, 80 );
  myTable->setColumnWidth( ALIGNMENT_COL, 100 );

  myTable->verticalHeader()->setDefaultSectionSize( 20 );
  myTable->verticalHeader()->hide();

  myTable->setItemDelegateForColumn( X_COL, new ItemDelegate( myTable, aDoubleValidator ) );
  myTable->setItemDelegateForColumn( Y_COL, new ItemDelegate( myTable, aDoubleValidator ) );

  QPixmap aRemovePixmap = aResMgr->loadPixmap( "Plot2d", tr( "ICON_MINUS" ) );
  QToolButton* aRemoveBtn = new QToolButton( aGrp );
  aRemoveBtn->setIcon( aRemovePixmap );
  aRemoveBtn->setFixedSize( aRemovePixmap.size() );
  aGrp->insertTitleWidget( aRemoveBtn );

  QVBoxLayout* aGrpLayout = new QVBoxLayout( aGrp );
  aGrpLayout->setMargin( 5 );
  aGrpLayout->setSpacing( 5 );
  aGrpLayout->addWidget( myTable );

  QVBoxLayout* aMainLayout = new QVBoxLayout( aMainFrame );
  aMainLayout->setMargin( 0 );
  aMainLayout->setSpacing( 0 );
  aMainLayout->addWidget( aGrp );

  connect( aRemoveBtn, SIGNAL( clicked() ), SLOT( onRemove() ) );

  setButtonPosition( Right, Cancel );

  setMinimumSize( 500, 300 );
}

//=============================================================================
// Function : Plot2d_SetupCommentsDlg
// Purpose  : Destructor
//=============================================================================
Plot2d_SetupCommentsDlg::~Plot2d_SetupCommentsDlg()
{
}

//=============================================================================
// Function : getAlignments
// Purpose  : 
//=============================================================================
void Plot2d_SetupCommentsDlg::getAlignments( QStringList& theStrings,
                                             QList< Qt::Alignment >& theValues ) const
{
  static QStringList aStrings;
  static QList< Qt::Alignment > aValues;
  if( aStrings.isEmpty() && aValues.isEmpty() )
  {
    aStrings.append( tr( "TOP_LEFT" ) );
    aValues.append( Qt::AlignTop | Qt::AlignLeft );

    aStrings.append( tr( "TOP_RIGHT" ) );
    aValues.append( Qt::AlignTop | Qt::AlignRight );

    aStrings.append( tr( "BOTTOM_LEFT" ) );
    aValues.append( Qt::AlignBottom | Qt::AlignLeft );

    aStrings.append( tr( "BOTTOM_RIGHT" ) );
    aValues.append( Qt::AlignBottom | Qt::AlignRight );

    aStrings.append( tr( "CENTER" ) );
    aValues.append( Qt::AlignCenter );
  }
  theStrings = aStrings;
  theValues = aValues;
}

//=============================================================================
// Function : setCombo
// Purpose  : 
//=============================================================================
void Plot2d_SetupCommentsDlg::setCombo( const int theRow, 
                                        const int theCol, 
                                        const QStringList& theStrings,
                                        const QList< Qt::Alignment >& theValues,
                                        const Qt::Alignment theCurrentValue  )
{
  QComboBox* aCombo = new QComboBox( myTable );

  int anIndex = 0;
  QStringListIterator aStringIter( theStrings );
  QListIterator< Qt::Alignment > aValueIter( theValues );
  while( aStringIter.hasNext() && aValueIter.hasNext() )
  {
    const QString& aString = aStringIter.next();
    Qt::Alignment aValue = aValueIter.next();
    aCombo->addItem( aString, int( aValue ) );
    if( aValue == theCurrentValue )
      aCombo->setCurrentIndex( anIndex );
    anIndex++;
  }

  myTable->setCellWidget( theRow, theCol, aCombo );
}

//=============================================================================
// Function : setText
// Purpose  : 
//=============================================================================
void Plot2d_SetupCommentsDlg::setText( const int theRow,
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
// Function : getText
// Purpose  : 
//=============================================================================
QString Plot2d_SetupCommentsDlg::getText( const int theRow,
                                          const int theCol ) const
{
  if( QTableWidgetItem* anItem = myTable->item( theRow, theCol ) )
    return anItem->text();
  return QString();
}

//=============================================================================
// Function : SetParameters
// Purpose  : 
//=============================================================================
void Plot2d_SetupCommentsDlg::SetParameters( const QVector< QString >& theTextList,
                                             const QVector< double >& theXList,
                                             const QVector< double >& theYList,
                                             const QVector< Qt::Alignment >& theAlignmentList )
{
  int nbRows = qMax( qMax( theTextList.size(), theAlignmentList.size() ), 
                     qMax( theXList.size(), theYList.size() ) );
  myTable->setRowCount( nbRows );

  QStringList anAlignmentStrings;
  QList< Qt::Alignment > anAlignmentValues;
  getAlignments( anAlignmentStrings, anAlignmentValues );

  for ( int i = 0; i < nbRows; i++ )
  {
    setText( i, TEXT_COL, theTextList[ i ] );
    setText( i, X_COL, QString( "%1" ).arg( theXList[ i ] ) );
    setText( i, Y_COL, QString( "%1" ).arg( theYList[ i ] ) );
    setCombo( i, ALIGNMENT_COL, anAlignmentStrings, anAlignmentValues, theAlignmentList[ i ] );
  }

  myRemovedIndexes.clear();
}

//=============================================================================
// Function : GetParameters
// Purpose  : 
//=============================================================================
void Plot2d_SetupCommentsDlg::GetParameters( QVector< QString >& theTextList,
                                             QVector< double >& theXList,
                                             QVector< double >& theYList,
                                             QVector< Qt::Alignment >& theAlignmentList ) const
{
  int nbRows = myTable->rowCount();

  theTextList.resize( nbRows );
  theXList.resize( nbRows );
  theYList.resize( nbRows );
  theAlignmentList.resize( nbRows );

  bool anIsOk;
  for ( int i = 0; i < nbRows; i++ )
  {
    theTextList[ i ] = getText( i, TEXT_COL );

    anIsOk = false;
    double aX = getText( i, X_COL ).toDouble( &anIsOk );
    theXList[ i ] = anIsOk ? aX : 0;

    anIsOk = false;
    double aY = getText( i, Y_COL ).toDouble( &anIsOk );
    theYList[ i ] = anIsOk ? aY : 0;

    anIsOk = false;
    Qt::Alignment anAlignment = Qt::AlignTop | Qt::AlignLeft;
    if( QComboBox* aCombo = dynamic_cast<QComboBox*>( myTable->cellWidget( i, ALIGNMENT_COL ) ) )
    {
      anAlignment = (Qt::Alignment)aCombo->itemData( aCombo->currentIndex() ).toInt( &anIsOk );
      if( !anIsOk )
        anAlignment = Qt::AlignTop | Qt::AlignLeft;
    }
    theAlignmentList[ i ] = anAlignment;
  }
}

//=============================================================================
// Function : GetRemovedIndexes
// Purpose  : 
//=============================================================================
const QList< int >& Plot2d_SetupCommentsDlg::GetRemovedIndexes() const
{
  return myRemovedIndexes;
}

//=============================================================================
// Function : onRemove
// Purpose  : 
//=============================================================================
void Plot2d_SetupCommentsDlg::onRemove()
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
    for ( int i = aReg.topRow(), n = aReg.bottomRow(); i <= n; i++ )
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
