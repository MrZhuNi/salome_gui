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

#define DEFAULT_MARGIN  11
#define DEFAULT_SPACING 6
#define COLUMN_SIZE     180

#define NAME_COLUMN  0
#define VALUE_COLUMN 1


///////////////////////////////////////////////////////////////////////////
//                 NoteBook_TableRow class                               //
///////////////////////////////////////////////////////////////////////////
//============================================================================
/*! Function : NoteBook_TableRow
 *  Purpose  : Constructor
 */
//============================================================================
NoteBook_TableRow::NoteBook_TableRow(int index, QWidget* parent):
  QWidget(parent),
  myIndex(index),
  myRowHeader(new QTableWidgetItem()),
  myVariableName(new QTableWidgetItem()),
  myVariableValue(new QTableWidgetItem())
{
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
void NoteBook_TableRow::AddToTable(QTableWidget *theTable)
{
  int aPosition = theTable->rowCount();
  int aRowCount = aPosition+1;
  theTable->setRowCount(aRowCount);
  myRowHeader->setText(QString::number(aRowCount));

  theTable->setVerticalHeaderItem(aPosition,myRowHeader);
  theTable->setItem(aPosition, NAME_COLUMN, myVariableName);
  theTable->setItem(aPosition, VALUE_COLUMN, myVariableValue);
}

//============================================================================
/*! Function : SetName
 *  Purpose  : 
 */
//============================================================================
void NoteBook_TableRow::SetName(const QString theName)
{
  myVariableName->setText(theName);
}

//============================================================================
/*! Function : SetValue
 *  Purpose  : 
 */
//============================================================================
void NoteBook_TableRow::SetValue(const QString theValue)
{
  myVariableValue->setText(theValue);
}

//============================================================================
/*! Function : GetName
 *  Purpose  : Return variable name
 */
//============================================================================
QString NoteBook_TableRow::GetName() const
{
  return myVariableName->text();
}

//============================================================================
/*! Function : GetValue
 *  Purpose  : Return variable value
 */
//============================================================================
QString NoteBook_TableRow::GetValue() const
{
  return myVariableValue->text(); 
}

//============================================================================
/*! Function : GetVariableItem
 *  Purpose  : 
 */
//============================================================================
QTableWidgetItem* NoteBook_TableRow::GetVariableItem()
{
  return myVariableValue;
}

//============================================================================
/*! Function : GetNameItem
 *  Purpose  : 
 */
//============================================================================
QTableWidgetItem* NoteBook_TableRow::GetNameItem()
{
  return myVariableName;
}

//============================================================================
/*! Function : GetHeaderItem
 *  Purpose  : 
 */
//============================================================================
QTableWidgetItem* NoteBook_TableRow::GetHeaderItem()
{
  return myRowHeader;
}

//============================================================================
/*! Function : IsRealValue
 *  Purpose  : Return true if theValue string is real value, otherwise return 
 *             false
 */
//============================================================================
bool NoteBook_TableRow::IsRealValue(const QString theValue, double* theResult)
{
  bool aResult = false;
  double aDResult = theValue.toDouble(&aResult);
  if(aResult && theResult)
    *theResult = aDResult;
  
  return aResult;
}

//============================================================================
/*! Function : IsBooleanValue
 *  Purpose  : Return true if theValue String is boolean value, otherwise return 
 *             false
 */
//============================================================================
bool NoteBook_TableRow::IsBooleanValue(const QString theValue, bool* theResult){
  bool aResult = false;
  bool aBResult; 
  if(theValue.compare("True") == 0) {
    aBResult = true;
    aResult = true;
  }
  else if(theValue.compare("False") == 0) {
    aBResult = false;
    aResult = true;
  }
  if(aResult && theResult)
    *theResult = aBResult;
  
  return aResult;
}

//============================================================================
/*! Function : IsIntegerValue
 *  Purpose  : Return true if theValue string is integer value, otherwise return 
 *             false
 */
//============================================================================
bool NoteBook_TableRow::IsIntegerValue(const QString theValue, int* theResult)
{
  bool aResult = false;
  int anIResult;
  anIResult = theValue.toInt(&aResult);

  if(aResult && theResult)
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
bool NoteBook_TableRow::IsValidStringValue(const QString theValue)
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
NoteBook_Table::NoteBook_Table(QWidget * parent)
  :QTableWidget(parent)
{
  setColumnCount(2);
  setSelectionMode(QAbstractItemView::SingleSelection);
  
  //Add Headers Columns
  QFont aFont = QFont();
  aFont.setBold(true);
  aFont.setPointSize(10);
  
  //"Name" column
  QTableWidgetItem * aNameHeader = new QTableWidgetItem();
  aNameHeader->setText(tr("VARNAME_COLUMN"));
  aNameHeader->setFont(aFont);
  setHorizontalHeaderItem(0,aNameHeader);
  setColumnWidth ( 0, COLUMN_SIZE);

  //"Value" Column
  QTableWidgetItem * aValueHeader = new QTableWidgetItem();
  aValueHeader->setText(tr("VARVALUE_COLUMN"));
  aValueHeader->setFont(aFont);
  setHorizontalHeaderItem(1,aValueHeader);
  setColumnWidth ( 1, COLUMN_SIZE);
  setSortingEnabled(false);
  
  connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(onItemChanged(QTableWidgetItem*)));
}

//============================================================================
/*! Function : ~NoteBook_Table
 *  Purpose  : Destructor
 */
//============================================================================
NoteBook_Table::~NoteBook_Table(){}

void NoteBook_Table::printVariableMap()
{
  printf( "VariableMap:\n" );
  VariableMap::const_iterator it = myVariableMap.constBegin(), itEnd = myVariableMap.constEnd();
  for( ; it != itEnd; ++it )
  {
    int anIndex = it.key();
    const NoteBoox_Variable& aVariable = it.value();
    QString aName = aVariable.Name;
    QString aValue = aVariable.Value;
    printf( "%d - %s - %s\n", anIndex, aName.toLatin1().constData(), aValue.toLatin1().constData() );
  }
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
      theColumn == NAME_COLUMN ? theRow->GetNameItem() : theRow->GetVariableItem() )
  {
    bool isBlocked = blockSignals( true );
    anItem->setForeground( QBrush( theIsCorrect ? Qt::black : Qt::red ) );
    blockSignals( isBlocked );
  }
}

//============================================================================
/*! Function : checkItem
 *  Purpose  : Check validity of item by its color
 */
//============================================================================
bool NoteBook_Table::checkItem( QTableWidgetItem* theItem ) const
{
  return theItem->foreground().color() == Qt::black;
}

//============================================================================
/*! Function : Init
 *  Purpose  : Add variables in the table from theNoteBook
 */
//============================================================================
void NoteBook_Table::Init(SalomeApp_Notebook* theNoteBook)
{
  bool isBlocked = blockSignals( true );

  int aNumRows = myRows.count();
  if( aNumRows > 0 )
  {
    for( int i = 0; i < myRows.size(); i++ )
    {
      NoteBook_TableRow* aRow = myRows[ i ];
      if( aRow )
      {
	delete aRow;
	aRow = 0;
      }
    }
    myRows.clear();
  }
  setRowCount( 0 );

  myVariableMap.clear();

  //Add all variables into the table
  QStringList aVariables = theNoteBook->parameters();
  QStringListIterator anIter( aVariables );
  while( anIter.hasNext() )
  {
    QString aVariable = anIter.next();
    QString aVariableValue = theNoteBook->get(aVariable).toString();
    AddRow( aVariable, aVariableValue );
  }

  //Add empty row
  AddRow();

  myNoteBook = theNoteBook;

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

  if( !myRows[ aNumRows - 1 ]->GetName().isEmpty() ||
      !myRows[ aNumRows - 1 ]->GetValue().isEmpty() )
    return false;

  for( int aRow = 0, aRowCount = rowCount(); aRow < aRowCount; aRow++ )
    for( int aCol = 0, aColCount = columnCount(); aCol < aColCount; aCol++ )
      if( QTableWidgetItem* anItem = item( aRow, aCol ) )
        if( !checkItem( anItem ) )
          return false;

  return true;
}

//============================================================================
/*! Function : AddRow
 *  Purpose  : Add a row into the table
 */
//============================================================================
void NoteBook_Table::AddRow(const QString& theName, const QString& theValue)
{
  int anIndex = getUniqueIndex();
  NoteBook_TableRow* aRow = new NoteBook_TableRow(anIndex, this);
  aRow->SetName(theName);
  aRow->SetValue(theValue);
  aRow->AddToTable(this);
  myRows.append(aRow);

  myVariableMap.insert( anIndex, NoteBoox_Variable( theName, theValue ) );
}

//============================================================================
/*! Function : GetRowByItem
 *  Purpose  : 
 */
//============================================================================
NoteBook_TableRow* NoteBook_Table::GetRowByItem(const QTableWidgetItem* theItem) const
{
  int aCurrentRow = row(theItem);
  
  if( (myRows.size() <= aCurrentRow ) && (aCurrentRow < 0))
    return NULL;
  else
    return myRows.at(aCurrentRow);
}

//============================================================================
/*! Function : IsLastRow
 *  Purpose  : Return true if theRow is last row in the table
 */
//============================================================================
bool NoteBook_Table::IsLastRow(const NoteBook_TableRow* theRow) const
{
  return (myRows.last() == theRow);
}

//============================================================================
/*! Function : onItemChanged
 *  Purpose  : [slot] called then table item changed
 */
//============================================================================
void NoteBook_Table::onItemChanged(QTableWidgetItem* theItem)
{
  //printf( "onItemChanged( %d, %d )\n", theItem->row(), theItem->column() );

  //printf( "Before:\n" );
  //printVariableMap();

  NoteBook_TableRow* aRow = GetRowByItem( theItem );
  if( !aRow )
    return;

  int anIndex = aRow->GetIndex();
  if( !myVariableMap.contains( anIndex ) )
    return;

  NoteBoox_Variable& aVariable = myVariableMap[ anIndex ];

  QString aName = aRow->GetName();
  QString aValue = aRow->GetValue();
  bool isComplete = !aName.isEmpty() && !aValue.isEmpty();

  QString anErrorType, anErrorMessage;

  int aCurrentColumn = column( theItem );
  if( aCurrentColumn == NAME_COLUMN )
  {
    if( !aVariable.Name.isEmpty() && !aVariable.Value.isEmpty() && aName != aVariable.Name)
    { // just rename and return
      try
      {
        myNoteBook->rename( aVariable.Name, aName );
      }
      catch( const SALOME::NotebookError& ex ) {
        anErrorType = tr( "NOTEBOOK_ERROR" );
        anErrorMessage = ex.Reason.in();
      } catch( const SALOME::ExpressionError& ex ) {
        anErrorType = tr( "EXPRESSION_ERROR" );
        anErrorMessage = ex.Reason.in();
      } catch( const SALOME::CalculationError& ex ) {
        anErrorType = tr( "CALCULATION_ERROR" );
        anErrorMessage = ex.Reason.in();
      } catch( const SALOME::TypeError& ex ) {
        anErrorType = tr( "TYPE_ERROR" );
        anErrorMessage = ex.Reason.in();
      }

      if( !anErrorType.isEmpty() )
      {
        SUIT_MessageBox::warning( this, anErrorType, anErrorMessage );

        //aRow->SetValue( aVariable.Value );
        markItem( aRow, aCurrentColumn, false );

        return;
      }
    }
    aVariable.Name = aName;
    return;
  }

  if( isComplete )
  {
    int iVal;
    double dVal;
    bool bVal;
    QString anErrorType, anErrorMessage;
    try
    {
      //printf( "myNoteBook->set( %s, %s )\n", aName.toLatin1().constData(), aValue.toLatin1().constData() );
      if( NoteBook_TableRow::IsIntegerValue( aValue, &iVal ) )
        myNoteBook->set( aName, iVal );
      else if( NoteBook_TableRow::IsRealValue( aValue, &dVal ) )
        myNoteBook->set( aName, dVal );
      else if( NoteBook_TableRow::IsBooleanValue( aValue, &bVal ) )
        myNoteBook->set( aName, bVal );
      else if( NoteBook_TableRow::IsValidStringValue( aValue ) )
        myNoteBook->set( aName, aValue );
      aVariable.Value = aValue;
    }
    catch( const SALOME::NotebookError& ex ) {
      anErrorType = tr( "NOTEBOOK_ERROR" );
      anErrorMessage = ex.Reason.in();
    } catch( const SALOME::ExpressionError& ex ) {
      anErrorType = tr( "EXPRESSION_ERROR" );
      anErrorMessage = ex.Reason.in();
    } catch( const SALOME::CalculationError& ex ) {
      anErrorType = tr( "CALCULATION_ERROR" );
      anErrorMessage = ex.Reason.in();
    } catch( const SALOME::TypeError& ex ) {
      anErrorType = tr( "TYPE_ERROR" );
      anErrorMessage = ex.Reason.in();
    }

    if( !anErrorType.isEmpty() )
    {
      SUIT_MessageBox::warning( this, anErrorType, anErrorMessage );

      //aRow->SetValue( aVariable.Value );
      markItem( aRow, aCurrentColumn, false );

      return;
    }
  }

  markItem( aRow, NAME_COLUMN, true );
  markItem( aRow, VALUE_COLUMN, true );

  if( IsLastRow( aRow ) ) // && isComplete
  {
    bool isBlocked = blockSignals( true );
    AddRow();
    blockSignals( isBlocked );
  }

  //printf( "After:\n" );
  //printVariableMap();
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
        aRow->SetName( QString() );
        aRow->SetValue( QString() );
      }
      else
      {
        int nRow = row( anItem );
        int index = aRow->GetIndex();

        QString aName = aRow->GetName();
        if( myVariableMap.contains( index ) )
          myVariableMap.remove( index );

        removeRow( nRow );
        myRows.removeAt( nRow );

        myNoteBook->remove( aName );
      }
    }

    // renumber header items
    for( int i = 0; i < myRows.size(); i++ )
      myRows[i]->GetHeaderItem()->setText( QString::number( i+1 ) );
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
SalomeApp_NoteBookDlg::SalomeApp_NoteBookDlg(QWidget* parent, SalomeApp_Study* theStudy):
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  myNoteBook(0)
{
  setModal(false);
  setObjectName("SalomeApp_NoteBookDlg");
  setWindowTitle(tr("NOTEBOOK_TITLE"));

  QGridLayout* aLayout = new QGridLayout(this);
  aLayout->setMargin(DEFAULT_MARGIN);
  aLayout->setSpacing(DEFAULT_SPACING);

  //Table
  myTable = new NoteBook_Table(this);
  aLayout->addWidget(myTable, 0, 0, 1, 3);
  
  //Buttons
  myRemoveButton = new QPushButton(tr("BUT_REMOVE"));
  aLayout->addWidget(myRemoveButton, 1, 0, 1, 1);

  QSpacerItem* spacer =
    new QSpacerItem(DEFAULT_SPACING, 5 , QSizePolicy::Expanding, QSizePolicy::Minimum);
  aLayout->addItem(spacer, 1, 1, 2, 1);

  myUpdateStudyBtn = new QPushButton(tr("BUT_UPDATE_STUDY"));
  aLayout->addWidget(myUpdateStudyBtn, 1, 2, 1, 1);
  
  QGroupBox* groupBox = new QGroupBox(this);

  QBoxLayout* aLayout1 = new QHBoxLayout(groupBox);

  aLayout1->setMargin(DEFAULT_MARGIN);
  aLayout1->setSpacing(DEFAULT_SPACING);

  myCloseBtn = new QPushButton(tr("BUT_CLOSE"));
  aLayout1->addWidget(myCloseBtn);

  QSpacerItem* spacer1 =
    new QSpacerItem(DEFAULT_SPACING, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);
  aLayout1->addItem(spacer1);

  myHelpBtn = new QPushButton(tr("BUT_HELP"));
  aLayout1->addWidget(myHelpBtn);
  
  aLayout->addWidget(groupBox, 2, 0, 1, 3);

  setMinimumWidth( 2 * COLUMN_SIZE + 50 );

  QWidgetList aWidgetList;
  aWidgetList.append( myTable );
  aWidgetList.append( myRemoveButton );
  aWidgetList.append( myUpdateStudyBtn );
  aWidgetList.append( myCloseBtn );
  aWidgetList.append( myHelpBtn );
  Qtx::setTabOrder( aWidgetList );

  connect( myRemoveButton, SIGNAL(clicked()), this, SLOT(onRemove()));
  connect( myUpdateStudyBtn, SIGNAL(clicked()), this, SLOT(onUpdateStudy()) );
  connect( myCloseBtn, SIGNAL(clicked()), this, SLOT(onClose()) );
  connect( myHelpBtn, SIGNAL(clicked()), this, SLOT(onHelp()));
 
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
void SalomeApp_NoteBookDlg::Init(SalomeApp_Study* theStudy)
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
/*! Function : onUpdateStudy
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onUpdateStudy()
{
  //printf( "Notebook:\n%s", myNoteBook->dump() );
  //printf( "-----------------------------------------\n" );
  //return;

  //onApply();
  if( !myTable->IsValid() )
    return;

  QApplication::setOverrideCursor( Qt::WaitCursor );

  myNoteBook->update( false );
  //SUIT_MessageBox::warning( this, tr( "ERROR" ), tr( "ERR_UPDATE_STUDY_FAILED" ) );
    
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
      SUIT_MessageBox::question( this, tr( "CLOSE_CAPTION" ), tr( "INCORRECT_DATA" ),
                                 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel ) != QMessageBox::Ok )
    return;
  accept();
}

//============================================================================
/*! Function : onHelp
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onHelp()
{
  QString aHelpFileName("using_notebook.html");
  LightApp_Application* app = (LightApp_Application*)(SUIT_Session::session()->activeApplication());
  if (app)
    app->onHelpContextModule("GUI",aHelpFileName);
  else {
    QString platform;
#ifdef WIN32
    platform = "winapplication";
#else
    platform = "application";
#endif
    SUIT_MessageBox::warning(this, tr("WRN_WARNING"),
                             tr("EXTERNAL_BROWSER_CANNOT_SHOW_PAGE").
                             arg(app->resourceMgr()->stringValue("ExternalBrowser",
                                                                 platform)).
                             arg(aHelpFileName));
  }
}
