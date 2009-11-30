// Copyright (C) 2008  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
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
// File:    SalomeApp_NoteBookDlg.cxx
// Author : Roman NIKOLAEV, Open CASCADE S.A.S.
// Module : GUI

#include "SalomeApp_NoteBookDlg.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Notebook.h"
#include "SalomeApp_Study.h"

#include <SUIT_MessageBox.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>

#include <QApplication>
#include <QFont>
#include <QGroupBox>
#include <QLayout>
#include <QPushButton>

#define VARIABLE_COLUMN   0
#define EXPRESSION_COLUMN 1
#define VALUE_COLUMN      2

#define VARIABLE_COLUMN_SIZE   120
#define EXPRESSION_COLUMN_SIZE 180
#define VALUE_COLUMN_SIZE      100
#define GAP_SIZE                80

///////////////////////////////////////////////////////////////////////////
//                 NoteBook_TableRow class                               //
///////////////////////////////////////////////////////////////////////////
//============================================================================
/*! Function : NoteBook_TableRow
 *  Purpose  : Constructor
 */
//============================================================================
NoteBook_TableRow::NoteBook_TableRow( int index, QWidget* parent ):
  QWidget( parent ),
  myIndex( index ),
  myRowHeaderItem( new QTableWidgetItem() ),
  myVariableItem( new QTableWidgetItem() ),
  myExpressionItem( new QTableWidgetItem() ),
  myValueItem( new QTableWidgetItem() )
{
  Qt::ItemFlags aFlags = myValueItem->flags();
  myValueItem->setFlags( aFlags ^ Qt::ItemIsEditable );
}

//============================================================================
/*! Function : ~NoteBook_TableRow
 *  Purpose  : Destructor
 */
//============================================================================
NoteBook_TableRow::~NoteBook_TableRow()
{
}

//============================================================================
/*! Function : AddToTable
 *  Purpose  : Add this row to the table theTable
 */
//============================================================================
void NoteBook_TableRow::AddToTable( QTableWidget *theTable )
{
  int aPosition = theTable->rowCount();
  int aRowCount = aPosition+1;
  theTable->setRowCount( aRowCount );
  myRowHeaderItem->setText( QString::number( aRowCount ) );

  theTable->setVerticalHeaderItem( aPosition, myRowHeaderItem );
  theTable->setItem( aPosition, VARIABLE_COLUMN,   myVariableItem );
  theTable->setItem( aPosition, EXPRESSION_COLUMN, myExpressionItem );
  theTable->setItem( aPosition, VALUE_COLUMN,      myValueItem );
}

//============================================================================
/*! Function : SetVariable
 *  Purpose  : 
 */
//============================================================================
void NoteBook_TableRow::SetVariable( const QString& theVariable )
{
  myVariableItem->setText( theVariable );
}

//============================================================================
/*! Function : SetExpression
 *  Purpose  : 
 */
//============================================================================
void NoteBook_TableRow::SetExpression( const QString& theExpression )
{
  myExpressionItem->setText( theExpression );
}

//============================================================================
/*! Function : SetValue
 *  Purpose  : 
 */
//============================================================================
void NoteBook_TableRow::SetValue( const QString& theValue )
{
  myValueItem->setText( theValue );
}

//============================================================================
/*! Function : GetVariable
 *  Purpose  : Return variable name
 */
//============================================================================
QString NoteBook_TableRow::GetVariable() const
{
  return myVariableItem->text();
}

//============================================================================
/*! Function : GetExpression
 *  Purpose  : Return variable expression
 */
//============================================================================
QString NoteBook_TableRow::GetExpression() const
{
  return myExpressionItem->text(); 
}

//============================================================================
/*! Function : GetValue
 *  Purpose  : Return variable value
 */
//============================================================================
QString NoteBook_TableRow::GetValue() const
{
  return myValueItem->text(); 
}

//============================================================================
/*! Function : IsRealValue
 *  Purpose  : Return true if theValue string is real value, otherwise return 
 *             false
 */
//============================================================================
bool NoteBook_TableRow::IsRealValue( const QString theValue, double* theResult )
{
  bool aResult = false;
  double aDResult = theValue.toDouble( &aResult );
  if( aResult && theResult )
    *theResult = aDResult;
  
  return aResult;
}

//============================================================================
/*! Function : IsBooleanValue
 *  Purpose  : Return true if theValue String is boolean value, otherwise return 
 *             false
 */
//============================================================================
bool NoteBook_TableRow::IsBooleanValue( const QString theValue, bool* theResult )
{
  bool aResult = false;
  bool aBResult; 
  if( theValue.compare( "True" ) == 0 )
  {
    aBResult = true;
    aResult = true;
  }
  else if( theValue.compare( "False" ) == 0 )
  {
    aBResult = false;
    aResult = true;
  }
  if( aResult && theResult )
    *theResult = aBResult;
  
  return aResult;
}

//============================================================================
/*! Function : IsIntegerValue
 *  Purpose  : Return true if theValue string is integer value, otherwise return 
 *             false
 */
//============================================================================
bool NoteBook_TableRow::IsIntegerValue( const QString theValue, int* theResult )
{
  bool aResult = false;
  int anIResult;
  anIResult = theValue.toInt( &aResult );

  if( aResult && theResult )
    *theResult = anIResult;  
  
  return aResult;
}

//============================================================================
/*! Function : IsValidStringValue
 *  Purpose  : Return true if theValue string is valid, otherwise return 
 *             false
 *             The string are always valid for the moment
 *             The whole notebook is verified on apply
 */
//============================================================================
bool NoteBook_TableRow::IsValidStringValue( const QString theValue )
{
  return true;
}

///////////////////////////////////////////////////////////////////////////
//                      NoteBook_Table class                             //
///////////////////////////////////////////////////////////////////////////
//============================================================================
/*! Function : NoteBook_Table
 *  Purpose  : Constructor
 */
//============================================================================
NoteBook_Table::NoteBook_Table( QWidget* parent )
  : QTableWidget( parent )
{
  setColumnCount( 3 );
  setSelectionMode( QAbstractItemView::SingleSelection );
  
  // Add headers columns
  QFont aFont = QFont();
  aFont.setBold( true );
  aFont.setPointSize( 10 );
  
  // "Variable" column
  QTableWidgetItem* aVariableHeader = new QTableWidgetItem();
  aVariableHeader->setText( tr( "VARIABLE" ) );
  aVariableHeader->setFont( aFont );
  setHorizontalHeaderItem( VARIABLE_COLUMN, aVariableHeader );
  setColumnWidth( VARIABLE_COLUMN, VARIABLE_COLUMN_SIZE );

  // "Expression" column
  QTableWidgetItem* anExpressionHeader = new QTableWidgetItem();
  anExpressionHeader->setText( tr( "EXPRESSION" ) );
  anExpressionHeader->setFont( aFont );
  setHorizontalHeaderItem( EXPRESSION_COLUMN, anExpressionHeader );
  setColumnWidth( EXPRESSION_COLUMN, EXPRESSION_COLUMN_SIZE );
  setSortingEnabled( false );

  // "Value" column
  QTableWidgetItem* aValueHeader = new QTableWidgetItem();
  aValueHeader->setText( tr( "VALUE" ) );
  aValueHeader->setFont( aFont );
  setHorizontalHeaderItem( VALUE_COLUMN, aValueHeader );
  setColumnWidth( VALUE_COLUMN, VALUE_COLUMN_SIZE );
  setSortingEnabled( false );
  
  connect( this, SIGNAL( itemChanged( QTableWidgetItem* ) ),
           this, SLOT( onItemChanged( QTableWidgetItem* ) ) );
}

//============================================================================
/*! Function : ~NoteBook_Table
 *  Purpose  : Destructor
 */
//============================================================================
NoteBook_Table::~NoteBook_Table()
{
  clear();
}

//============================================================================
/*! Function : clear
 *  Purpose  : Clear contents of the table
 */
//============================================================================
void NoteBook_Table::clear()
{
  bool isBlocked = blockSignals( true );
  for( int i = 0, n = myRows.size(); i < n; i++ )
  {
    if( NoteBook_TableRow* aRow = myRows[ i ] )
    {
      delete aRow;
      aRow = 0;
    }
  }
  myRows.clear();
  setRowCount( 0 );
  blockSignals( isBlocked );
}

//============================================================================
/*! Function : getUniqueIndex
 *  Purpose  : Get a unique index for the new row
 */
//============================================================================
int NoteBook_Table::getUniqueIndex() const
{
  int anIndex = 0;
  if( !myRows.isEmpty() )
    if( NoteBook_TableRow* aRow = myRows.last() )
      anIndex = aRow->GetIndex();

  anIndex++;
  return anIndex;
}

//============================================================================
/*! Function : markItem
 *  Purpose  : Mark an item at the given column of the given row by red or
 *             black color (red color means incorrect value, black - correct)
 */
//============================================================================
void NoteBook_Table::markItem( NoteBook_TableRow* theRow, int theColumn, bool theIsCorrect )
{
  if( QTableWidgetItem* anItem =
      theColumn == VARIABLE_COLUMN ? theRow->GetVariableItem() :
      theColumn == EXPRESSION_COLUMN ? theRow->GetExpressionItem() : theRow->GetValueItem() )
  {
    bool isBlocked = blockSignals( true );
    anItem->setForeground( QBrush( theIsCorrect ? Qt::black : Qt::red ) );
    blockSignals( isBlocked );
  }
}

//============================================================================
/*! Function : markRow
 *  Purpose  : Mark the given row by red or black color
 *             (red color means incorrect value, black - correct)
 */
//============================================================================
void NoteBook_Table::markRow( NoteBook_TableRow* theRow, bool theIsCorrect )
{
  markItem( theRow, VARIABLE_COLUMN, theIsCorrect );
  markItem( theRow, EXPRESSION_COLUMN, theIsCorrect );
  markItem( theRow, VALUE_COLUMN, theIsCorrect );
}

//============================================================================
/*! Function : checkItem
 *  Purpose  : Check validity of the item by its color
 */
//============================================================================
bool NoteBook_Table::checkItem( NoteBook_TableRow* theRow, int theColumn ) const
{
  if( QTableWidgetItem* anItem =
      theColumn == VARIABLE_COLUMN ? theRow->GetVariableItem() :
      theColumn == EXPRESSION_COLUMN ? theRow->GetExpressionItem() : theRow->GetValueItem() )
    return anItem->foreground().color() == Qt::black;
  return false;
}

//============================================================================
/*! Function : checkRow
 *  Purpose  : Check validity of the row by a color of its first item
 */
//============================================================================
bool NoteBook_Table::checkRow( NoteBook_TableRow* theRow ) const
{
  return checkItem( theRow, VARIABLE_COLUMN ) &&
         checkItem( theRow, EXPRESSION_COLUMN ) &&
         checkItem( theRow, VALUE_COLUMN );
}

//============================================================================
/*! Function : correctData
 *  Purpose  : Try to correct data of the table
 */
//============================================================================
void NoteBook_Table::correctData( int theBaseRowIndex )
{
  for( int i = 0, n = myRows.size(); i < n; i++ )
  {
    if( NoteBook_TableRow* aRow = myRows[ i ] )
    {
      if( aRow->GetIndex() != theBaseRowIndex )
      {
        QString aName = aRow->GetVariable();
        QString anExpression = myNoteBook->expression( aName );
      }
    }
  }
}

//============================================================================
/*! Function : updateExpressions
 *  Purpose  : Update variable expressions according to myNoteBook data
 *             (used after renaming or removing of some variable)
 */
//============================================================================
void NoteBook_Table::updateExpressions( int theBaseRowIndex )
{
  bool isBlocked = blockSignals( true );
  for( int i = 0, n = myRows.size(); i < n; i++ )
  {
    if( NoteBook_TableRow* aRow = myRows[ i ] )
    {
      if( aRow->GetIndex() != theBaseRowIndex )
      {
        QString aName = aRow->GetVariable();
        QString anExpression = myNoteBook->expression( aName );
        aRow->SetExpression( anExpression );
      }
    }
  }
  blockSignals( isBlocked );
}

//============================================================================
/*! Function : updateValues
 *  Purpose  : Update variable values according to myNoteBook data
 */
//============================================================================
void NoteBook_Table::updateValues()
{
  bool isBlocked = blockSignals( true );
  for( int i = 0, n = myRows.size(); i < n; i++ )
  {
    if( NoteBook_TableRow* aRow = myRows[ i ] )
    {
      QString aName = aRow->GetVariable();
      QVariant aValue = myNoteBook->get( aName );
      aRow->SetValue( aValue.toString() );
    }
  }
  blockSignals( isBlocked );
}

//============================================================================
/*! Function : setExpression
 *  Purpose  : Try to set expression of the variable
 */
//============================================================================
bool NoteBook_Table::setExpression( const QString& theName,
                                    const QString& theExpression,
                                    bool theIsNew,
                                    QString& theErrorType,
                                    QString& theErrorMessage )
{
  theErrorType.clear();
  theErrorMessage.clear();

  int iVal;
  double dVal;
  bool bVal;
  try
  {
    if( NoteBook_TableRow::IsIntegerValue( theExpression, &iVal ) )
      myNoteBook->set( theName, iVal, theIsNew );
    else if( NoteBook_TableRow::IsRealValue( theExpression, &dVal ) )
      myNoteBook->set( theName, dVal, theIsNew );
    else if( NoteBook_TableRow::IsBooleanValue( theExpression, &bVal ) )
      myNoteBook->set( theName, bVal, theIsNew );
    else if( NoteBook_TableRow::IsValidStringValue( theExpression ) )
      myNoteBook->set( theName, theExpression, theIsNew );
  }
  catch( const SALOME::NotebookError& ex ) {
    theErrorType = tr( "NOTEBOOK_ERROR" );
    theErrorMessage = ex.Reason.in();
  } catch( const SALOME::ExpressionError& ex ) {
    theErrorType = tr( "EXPRESSION_ERROR" );
    theErrorMessage = ex.Reason.in();
  } catch( const SALOME::CalculationError& ex ) {
    theErrorType = tr( "CALCULATION_ERROR" );
    theErrorMessage = ex.Reason.in();
  } catch( const SALOME::TypeError& ex ) {
    theErrorType = tr( "TYPE_ERROR" );
    theErrorMessage = ex.Reason.in();
  }

  return theErrorType.isEmpty();
}

//============================================================================
/*! Function : setExpression
 *  Purpose  : Try to rename the variable
 */
//============================================================================
bool NoteBook_Table::renameVariable( const QString& theOldName,
                                     const QString& theNewName,
                                     QString& theErrorType,
                                     QString& theErrorMessage )
{
  theErrorType.clear();
  theErrorMessage.clear();

  try
  {
    myNoteBook->rename( theOldName, theNewName );
  }
  catch( const SALOME::NotebookError& ex ) {
    theErrorType = tr( "NOTEBOOK_ERROR" );
    theErrorMessage = ex.Reason.in();
  } catch( const SALOME::ExpressionError& ex ) {
    theErrorType = tr( "EXPRESSION_ERROR" );
    theErrorMessage = ex.Reason.in();
  } catch( const SALOME::CalculationError& ex ) {
    theErrorType = tr( "CALCULATION_ERROR" );
    theErrorMessage = ex.Reason.in();
  } catch( const SALOME::TypeError& ex ) {
    theErrorType = tr( "TYPE_ERROR" );
    theErrorMessage = ex.Reason.in();
  }

  return theErrorType.isEmpty();
}

//============================================================================
/*! Function : Init
 *  Purpose  : Add variables in the table from theNoteBook
 */
//============================================================================
void NoteBook_Table::Init( SalomeApp_Notebook* theNoteBook )
{
  myNoteBook = theNoteBook;

  bool isBlocked = blockSignals( true );

  clear();
  myVariableMap.clear();

  // Add all variables into the table
  QStringList aVariables = myNoteBook->parameters();
  QStringListIterator anIter( aVariables );
  while( anIter.hasNext() )
  {
    QString aVariable = anIter.next();
    if( aVariable.left( 2 ).compare( "__" ) == 0 ) // tmp variable
      continue;
    QString aVariableValue = myNoteBook->expression(aVariable);
    AddRow( aVariable, aVariableValue );
  }

  // Add empty row
  AddRow();

  updateValues();

  blockSignals( isBlocked );
}

//============================================================================
/*! Function : IsValid
 *  Purpose  : Check validity of the table data
 */
//============================================================================
bool NoteBook_Table::IsValid() const
{
  int aNumRows = myRows.count();
  if( aNumRows == 0 )
    return true;

  if( !myRows[ aNumRows - 1 ]->GetVariable().isEmpty() ||
      !myRows[ aNumRows - 1 ]->GetExpression().isEmpty() ||
      !myRows[ aNumRows - 1 ]->GetValue().isEmpty() )
    return false;

  for( int i = 0, n = myRows.size(); i < n; i++ )
    if( NoteBook_TableRow* aRow = myRows[ i ] )
      if( !checkRow( aRow ) )
        return false;

  return true;
}

//============================================================================
/*! Function : AddRow
 *  Purpose  : Add a row into the table
 */
//============================================================================
void NoteBook_Table::AddRow( const QString& theName, const QString& theExpression )
{
  int anIndex = getUniqueIndex();
  NoteBook_TableRow* aRow = new NoteBook_TableRow( anIndex, this );
  aRow->SetVariable( theName );
  aRow->SetExpression( theExpression );
  aRow->AddToTable( this );
  myRows.append( aRow );

  myVariableMap.insert( anIndex, NoteBoox_Variable( theName, theExpression ) );
}

//============================================================================
/*! Function : GetRowByItem
 *  Purpose  : 
 */
//============================================================================
NoteBook_TableRow* NoteBook_Table::GetRowByItem( const QTableWidgetItem* theItem ) const
{
  int aCurrentRow = row( theItem );
  if( aCurrentRow >= 0 && aCurrentRow < myRows.size() )
    return myRows.at( aCurrentRow );
  return NULL;
}

//============================================================================
/*! Function : IsLastRow
 *  Purpose  : Return true if theRow is last row in the table
 */
//============================================================================
bool NoteBook_Table::IsLastRow( const NoteBook_TableRow* theRow ) const
{
  return theRow == myRows.last();
}

//============================================================================
/*! Function : onItemChanged
 *  Purpose  : [slot] called then table item changed
 */
//============================================================================
void NoteBook_Table::onItemChanged( QTableWidgetItem* theItem )
{
  NoteBook_TableRow* aRow = GetRowByItem( theItem );
  if( !aRow )
    return;

  int anIndex = aRow->GetIndex();
  if( !myVariableMap.contains( anIndex ) )
    return;

  NoteBoox_Variable& aVariable = myVariableMap[ anIndex ];
  QString aNamePrevious = aVariable.Name;
  QString anExpressionPrevious = aVariable.Expression;
  bool isCompletePrevious = !aVariable.Name.isEmpty() && !aVariable.Expression.isEmpty();

  bool isCorrectPrevious = checkRow( aRow );
  markRow( aRow, true );

  QString aName = aRow->GetVariable();
  QString anExpression = aRow->GetExpression();
  bool isComplete = !aName.isEmpty() && !anExpression.isEmpty();

  aVariable.Name = aName;
  aVariable.Expression = anExpression;

  QString anErrorType, anErrorMessage;

  int aCurrentColumn = column( theItem );
  if( aCurrentColumn == VARIABLE_COLUMN && isCompletePrevious && isCorrectPrevious && aName != aNamePrevious )
  {
    if( !renameVariable( aNamePrevious, aName, anErrorType, anErrorMessage ) )
    {
      SUIT_MessageBox::warning( this, anErrorType, anErrorMessage );
      markRow( aRow, false );
      return;
    }
    updateExpressions( anIndex );
    return; // not necessary to update notebook data after renaming
  }

  if( isComplete )
  {
    if( !setExpression( aName, anExpression, !isCompletePrevious, anErrorType, anErrorMessage ) )
    {
      SUIT_MessageBox::warning( this, anErrorType, anErrorMessage );
      markRow( aRow, false );
      return;
    }
    myNoteBook->update( true );
    updateValues();
  }

  if( IsLastRow( aRow ) && isComplete )
  {
    bool isBlocked = blockSignals( true );
    AddRow();
    blockSignals( isBlocked );
  }
}

//============================================================================
/*! Function : RemoveSelected
 *  Purpose  : Remove selected rows in the table
 */
//============================================================================
void NoteBook_Table::RemoveSelected()
{
  bool isBlocked = blockSignals( true );

  QList<QTableWidgetItem*> aSelectedItems = selectedItems();
  if( aSelectedItems.size() == 1 )
  {
    QTableWidgetItem* anItem = aSelectedItems.first();
    if( NoteBook_TableRow* aRow = GetRowByItem( anItem ) )
    {
      if( IsLastRow( aRow ) )
      {
        aRow->SetVariable( QString() );
        aRow->SetExpression( QString() );
      }
      else
      {
        int anIndex = aRow->GetIndex();
        QString aName = aRow->GetVariable();
        if( myVariableMap.contains( anIndex ) )
          myVariableMap.remove( anIndex );

        int nRow = row( anItem );
        removeRow( nRow );
        myRows.removeAt( nRow );

        myNoteBook->remove( aName );
        updateExpressions( anIndex );
      }
    }

    // renumber header items
    for( int i = 0, n = myRows.size(); i < n; i++ )
      myRows[i]->GetRowHeaderItem()->setText( QString::number( i+1 ) );
  }

  blockSignals( isBlocked );
}

///////////////////////////////////////////////////////////////////////////
//                  SalomeApp_NoteBookDlg class                          //
///////////////////////////////////////////////////////////////////////////
//============================================================================
/*! Function : SalomeApp_NoteBookDlg
 *  Purpose  : Constructor
 */
//============================================================================
SalomeApp_NoteBookDlg::SalomeApp_NoteBookDlg( QWidget* parent, SalomeApp_Study* theStudy )
  : QDialog( parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint ),
    myNoteBook( 0 )
{
  setModal( false );
  setWindowTitle( tr( "NOTEBOOK_TITLE" ) );

  // Table
  myTable = new NoteBook_Table( this );
  
  // Buttons
  QPushButton* aRemoveBtn = new QPushButton( tr( "BUT_REMOVE" ) );
  QSpacerItem* aSpacer1 = new QSpacerItem( 5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum );
  QPushButton* aDumpBtn = new QPushButton( tr( "BUT_DUMP" ) );
  QPushButton* anUpdateStudyBtn = new QPushButton( tr( "BUT_UPDATE_STUDY" ) );
  
  QGroupBox* aGroupBox = new QGroupBox( this );

  QPushButton* aCloseBtn = new QPushButton( tr( "BUT_CLOSE" ) );
  QSpacerItem* aSpacer2 = new QSpacerItem( 5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum );
  QPushButton* aHelpBtn = new QPushButton( tr( "BUT_HELP" ) );

  QBoxLayout* aLayout = new QHBoxLayout( aGroupBox );
  aLayout->setMargin( 11 );
  aLayout->setSpacing( 6 );
  aLayout->addWidget( aCloseBtn );
  aLayout->addItem( aSpacer2 );
  aLayout->addWidget( aHelpBtn );
  
  QGridLayout* aTopLayout = new QGridLayout( this );
  aTopLayout->setMargin( 11 );
  aTopLayout->setSpacing( 6 );
  aTopLayout->addWidget( myTable,          0, 0, 1, 4 );
  aTopLayout->addWidget( aRemoveBtn,       1, 0, 1, 1 );
  aTopLayout->addItem( aSpacer1,           1, 1, 2, 1 );
  aTopLayout->addWidget( aDumpBtn,         1, 2, 1, 1 );
  aTopLayout->addWidget( anUpdateStudyBtn, 1, 3, 1, 1 );
  aTopLayout->addWidget( aGroupBox,        2, 0, 1, 4 );

  setMinimumWidth( VARIABLE_COLUMN_SIZE +
                   EXPRESSION_COLUMN_SIZE +
                   VALUE_COLUMN_SIZE +
                   GAP_SIZE );

  QWidgetList aWidgetList;
  aWidgetList.append( myTable );
  aWidgetList.append( aRemoveBtn );
  aWidgetList.append( aDumpBtn );
  aWidgetList.append( anUpdateStudyBtn );
  aWidgetList.append( aCloseBtn );
  aWidgetList.append( aHelpBtn );
  Qtx::setTabOrder( aWidgetList );

  connect( aRemoveBtn,       SIGNAL( clicked() ), this, SLOT( onRemove() ) );
  connect( aDumpBtn,         SIGNAL( clicked() ), this, SLOT( onDump() ) );
  connect( anUpdateStudyBtn, SIGNAL( clicked() ), this, SLOT( onUpdateStudy() ) );
  connect( aCloseBtn,        SIGNAL( clicked() ), this, SLOT( onClose() ) );
  connect( aHelpBtn,         SIGNAL( clicked() ), this, SLOT( onHelp() ) );
 
  //aDumpBtn->hide();

  Init( theStudy );
}

//============================================================================
/*! Function : ~SalomeApp_NoteBookDlg
 *  Purpose  : Destructor
 */
//============================================================================
SalomeApp_NoteBookDlg::~SalomeApp_NoteBookDlg()
{
  if( myNoteBook )
  {
    delete myNoteBook;
    myNoteBook = 0;
  }
}

//============================================================================
/*! Function : Init()
 *  Purpose  : init variable table
 */
//============================================================================
void SalomeApp_NoteBookDlg::Init( SalomeApp_Study* theStudy )
{
  // delete the current notebook (if can based on another study)
  if( myNoteBook )
    delete myNoteBook;

  // create new notebook based on the given study
  myNoteBook = new SalomeApp_Notebook( theStudy );
  myTable->Init( myNoteBook );
}

//============================================================================
/*! Function : onRemove
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onRemove()
{
  myTable->RemoveSelected();
}

//============================================================================
/*! Function : onDump
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onDump()
{
  SUIT_MessageBox::information( this, tr( "NOTEBOOK_TITLE" ), QString( myNoteBook->dump() ) );
}

//============================================================================
/*! Function : onUpdateStudy
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onUpdateStudy()
{
  if( !myTable->IsValid() )
  {
    SUIT_MessageBox::warning( this, tr( "ERROR" ), tr( "INCORRECT_DATA_ON_UPDATE" ) );
    return;
  }

  QApplication::setOverrideCursor( Qt::WaitCursor );
  myNoteBook->update( false );
  QApplication::restoreOverrideCursor();
}

//============================================================================
/*! Function : onClose
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onClose()
{
  if( !myTable->IsValid() &&
      SUIT_MessageBox::question( this, tr( "CLOSE_CAPTION" ), tr( "INCORRECT_DATA_ON_CLOSE" ),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Cancel ) != QMessageBox::Ok )
    return;

  // update only variables
  myNoteBook->update( true );

  accept();
}

//============================================================================
/*! Function : onHelp
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onHelp()
{
  QString aHelpFileName( "using_notebook.html" );
  LightApp_Application* app = (LightApp_Application*)( SUIT_Session::session()->activeApplication() );
  if( app )
    app->onHelpContextModule( "GUI", aHelpFileName );
  else
  {
    QString platform;
#ifdef WIN32
    platform = "winapplication";
#else
    platform = "application";
#endif
    SUIT_MessageBox::warning( this, tr( "WRN_WARNING" ),
                              tr( "EXTERNAL_BROWSER_CANNOT_SHOW_PAGE" ).
                              arg( app->resourceMgr()->stringValue("ExternalBrowser", platform ) ).
                              arg( aHelpFileName ) );
  }
}
