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
#include <SUIT_MessageBox.h>

#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QFont>
#include <QGroupBox>
#include <QList>

#include <string>
#include <vector>

using namespace std;

#define DEFAULT_MARGIN  11
#define DEFAULT_SPACING 6
#define SPACER_SIZE     120
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
NoteBook_TableRow::NoteBook_TableRow(QWidget* parent):
  QWidget(parent),
  myRowHeader(new QTableWidgetItem()),
  myVariableName(new QTableWidgetItem()),
  myVariableValue(new QTableWidgetItem()),
  isNameEditable(true)
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
/*! Function : SetNameEditable
 *  Purpose  : Set item with variable name editable/not editable
 *             and change flag isNameEditable
 */
//============================================================================
void NoteBook_TableRow::SetNameEditable(bool enable)
{
  isNameEditable = enable;
  Qt::ItemFlags f = myVariableName->flags();
  f = f & (isNameEditable ? Qt::ItemIsEditable : ~Qt::ItemIsEditable);
  myVariableName->setFlags(f);

  //Mark all not editabe variable names
  QFont aFont = QFont();
  aFont.setBold(true);
  myVariableName->setFont(aFont);
}

//============================================================================
/*! Function : setValueEditable
 *  Purpose  : Set item with variable value editable/not editable
 */
//============================================================================
void NoteBook_TableRow::setValueEditable(bool enable)
{
  Qt::ItemFlags f = myVariableValue->flags();
  f = enable ? (f | Qt::ItemIsEditable) : (f & ~Qt::ItemIsEditable);
  myVariableValue->setFlags(f);
}

//============================================================================
/*! Function : setNameEditable
 *  Purpose  : Set item with variable name editable/not editable
 */
//============================================================================
void NoteBook_TableRow::setNameEditable(bool enable)
{
  if(isNameEditable) {
    Qt::ItemFlags f = myVariableName->flags();
    f = enable ? (f | Qt::ItemIsEditable) : (f & ~Qt::ItemIsEditable);
    myVariableName->setFlags(f);
  }
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

bool NoteBook_TableRow::IsNameEditable()
{
  return isNameEditable;
}

bool NoteBook_TableRow::CheckName()
{
  bool aResult = false;
  QString aName = GetName();
  if( !aName.isEmpty() )
    aResult = true;
  
  return aResult;
}

//============================================================================
/*! Function : GetValue
 *  Purpose  : Return true if variable value correct, otherwise return false
 */
//============================================================================
bool NoteBook_TableRow::CheckValue()
{
  bool aResult = false;
  QString aValue = GetValue();
  if( (!aValue.isEmpty()) && 
      (IsRealValue(aValue)) ||
      IsIntegerValue(aValue)||
      IsBooleanValue(aValue)) 
    aResult = true;
  
  return aResult;
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
/*! Function : IsRealValue
 *  Purpose  : Return true if theValue string is real value, otherwise return 
 *             false
 */
//============================================================================
bool NoteBook_TableRow::IsRealValue(const QString theValue, double* theResult)
{
  bool aResult = false;
  double aDResult = theValue.toDouble(&aResult);
  if(theResult)
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
  if(theResult)
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

  if(theResult)
    *theResult = anIResult;  
  
  return aResult;
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
  :QTableWidget(parent),
   isProcessItemChangedSignal(false),
   myIncorrectItem(NULL)
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
  connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(onItemSelectionChanged()));

}

//============================================================================
/*! Function : ~NoteBook_Table
 *  Purpose  : Destructor
 */
//============================================================================
NoteBook_Table::~NoteBook_Table(){}

//============================================================================
/*! Function : Init
 *  Purpose  : Add variables in the table from theStudy
 */
//============================================================================
void NoteBook_Table::Init(_PTR(Study) theStudy)
{
  //Add all variables into the table
  vector<string> aVariables = theStudy->GetVariableNames();
  for(int iVar = 0; iVar < aVariables.size()  ; iVar++ ) {
    NoteBook_TableRow* aRow = new NoteBook_TableRow(this);
    aRow->SetName(QString(aVariables[iVar].c_str()));
    aRow->SetValue( Variable2String(aVariables[iVar],theStudy) );
    aRow->SetNameEditable(false); 
    aRow->AddToTable(this);
    myRows.append(aRow);
  }

  //Add empty row
  AddEmptyRow();
  isProcessItemChangedSignal = true;
}

//============================================================================
/*! Function : Variable2String
 *  Purpose  : Convert variable values to QString
 */
//============================================================================
QString NoteBook_Table::Variable2String(const string& theVarName,
                                        _PTR(Study) theStudy)
{
  QString aResult;
  if( theStudy->IsReal(theVarName) )
    aResult = QString::number(theStudy->GetReal(theVarName));
  else if( theStudy->IsInteger(theVarName) )
    aResult = QString::number(theStudy->GetInteger(theVarName));
  else if( theStudy->IsBoolean(theVarName) )
    aResult = theStudy->GetBoolean(theVarName) ? QString("True") : QString("False");
  
  return aResult;
}

//============================================================================
/*! Function : SetIncorrectItem
 *  Purpose  : 
 */
//============================================================================
void NoteBook_Table::SetIncorrectItem(QTableWidgetItem* theItem)
{
  if(!theItem)
    return;
  
  myIncorrectItem = theItem;
  isProcessItemChangedSignal = false;
  NoteBook_TableRow* except = GetRowByItem(myIncorrectItem);

  for (int i = 0; i < myRows.size(); i++) {
    myRows[i]->setNameEditable(false);
    myRows[i]->setValueEditable(false);
  }
  if(except) {
    if(column(theItem) == NAME_COLUMN )
      except->setNameEditable(true);
    else if(column(theItem) == VALUE_COLUMN)
      except->setValueEditable(true);
  }
  isProcessItemChangedSignal = true;
  emit incorrectItemAdded();
}

//============================================================================
/*! Function : RemoveIncorrectItem
 *  Purpose  : 
 */
//============================================================================
void NoteBook_Table::RemoveIncorrectItem()
{
  if(!myIncorrectItem)
    return;
  
  isProcessItemChangedSignal = false;
  for (int i = 0; i < myRows.size(); i++) {
    myRows[i]->setNameEditable(true);
    myRows[i]->setValueEditable(true);
  }

  myIncorrectItem = NULL;
  isProcessItemChangedSignal = true;
  emit incorrectItemRemoved();
}

void NoteBook_Table::AddEmptyRow()
{
  isProcessItemChangedSignal = false; 
  NoteBook_TableRow* aRow = new NoteBook_TableRow();  
  aRow->SetName(QString());
  aRow->SetValue(QString());
  aRow->AddToTable(this);
  myRows.append(aRow);
  isProcessItemChangedSignal = true;
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
  if(isProcessItemChangedSignal) {
    NoteBook_TableRow* aRow = GetRowByItem(theItem);
    if(aRow) {
      int aCurrentColumn = column(theItem);
      bool IsVariableComplited = false;
      
      //Case then varible name changed. 
      if(aCurrentColumn == NAME_COLUMN) {
        bool isVarNameCorrect = aRow->CheckName();
        if(!isVarNameCorrect) {
          SUIT_MessageBox::warning((QWidget*)parent(),
                                   tr( "WARNING" ), 
                                   tr( "VARNAME_INCORRECT" ).arg(aRow->GetName()));
          SetIncorrectItem(theItem);
          return;
        }
        if(!IsUniqueName(aRow)){
          SUIT_MessageBox::warning((QWidget*)parent(),
                                   tr( "WARNING" ), 
                                   tr( "VARNAME_EXISTS" ).arg(aRow->GetName()));
          SetIncorrectItem(theItem);
          return;
        }
        IsVariableComplited = isVarNameCorrect & aRow->CheckValue();
      }
      
      //Case then varible value changed. 
      else if(aCurrentColumn == VALUE_COLUMN){
        bool isVarValueCorrect = aRow->CheckValue();
        if(!isVarValueCorrect) {
          SUIT_MessageBox::warning((QWidget*)parent(),
                                   tr( "WARNING" ), 
                                   tr( "VARVALUE_INCORRECT" ).arg(aRow->GetValue()));
          SetIncorrectItem(theItem);
          return;
        }
        IsVariableComplited = isVarValueCorrect & aRow->CheckName();
      }
      
      if(myIncorrectItem)
        RemoveIncorrectItem();
      
      if(IsVariableComplited && IsLastRow(aRow))
        AddEmptyRow();
    }
  }
}

//============================================================================
/*! Function : IsUniqueName
 *  Purpose  : Return true if theName is unique name of the Variable
 */
//============================================================================
bool NoteBook_Table::IsUniqueName(const NoteBook_TableRow* theRow) const
{
  for(int i=0; i<myRows.size();i++) {
    if(myRows[i] == theRow ) 
      continue;
    if(myRows[i]->GetName().compare(theRow->GetName()) == 0)
      return false;
  }
  return true;
}

//============================================================================
/*! Function : onItemSelectionChanged
 *  Purpose  : [slot] 
 */
//============================================================================
void NoteBook_Table::onItemSelectionChanged()
{
  bool aResult = true;
  QList<QTableWidgetItem*> aSelectedItems = selectedItems();
  aResult &= (aSelectedItems.size() > 0);

  if(aResult) {
    for(int i=0; i < aSelectedItems.size(); i++ ) {
      NoteBook_TableRow* aRow = GetRowByItem(aSelectedItems[i]);
      if(aRow)
        aResult &= aRow->IsNameEditable();
    }
  }
  emit selectionChanged(aResult);
}

//============================================================================
/*! Function : RemoveSelected
 *  Purpose  : Remove selected rows in the table
 */
//============================================================================
void NoteBook_Table::RemoveSelected()
{
  isProcessItemChangedSignal = false;
  QList<QTableWidgetItem*> aSelectedItems = selectedItems();
  if( !(aSelectedItems.size() > 0)) {
    isProcessItemChangedSignal = true;
    return;
  }

  for(int i=0; i < aSelectedItems.size(); i++ ) {
    NoteBook_TableRow* aRow = GetRowByItem(aSelectedItems[i]);
    if(aRow) {
      if(IsLastRow(aRow)) {
        aRow->SetName(QString());
        aRow->SetValue(QString());
      }
      else {
        int nRow = row(aSelectedItems[i]);
        removeRow(nRow);
        myRows.removeAt(nRow);
      }
      if(aRow->GetVariableItem() == myIncorrectItem ||
         aRow->GetNameItem() == myIncorrectItem ) {
        RemoveIncorrectItem();
      }
    }
  }
  isProcessItemChangedSignal = true;
}

//============================================================================
/*! Function : SetProcessItemChangedSignalFlag
 *  Purpose  : 
 */
//============================================================================
void NoteBook_Table::SetProcessItemChangedSignalFlag(const bool enable)
{
  isProcessItemChangedSignal = enable;
}

//============================================================================
/*! Function : GetProcessItemChangedSignalFlag
 *  Purpose  : 
 */
//============================================================================
bool NoteBook_Table::GetProcessItemChangedSignalFlag() const
{
  return isProcessItemChangedSignal;
}

//============================================================================
/*! Function : GetRows
 *  Purpose  : 
 */
//============================================================================
QList<NoteBook_TableRow*> NoteBook_Table::GetRows() const
{
  return myRows;
}

///////////////////////////////////////////////////////////////////////////
//                  SalomeApp_NoteBookDlg class                          //
///////////////////////////////////////////////////////////////////////////
//============================================================================
/*! Function : SalomeApp_NoteBookDlg
 *  Purpose  : Constructor
 */
//============================================================================
SalomeApp_NoteBookDlg::SalomeApp_NoteBookDlg(QWidget * parent, _PTR(Study) theStudy):
  QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
  myStudy(theStudy)
{
  setModal(true);
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
    new QSpacerItem(SPACER_SIZE, 5 , QSizePolicy::Expanding, QSizePolicy::Minimum);
  aLayout->addItem(spacer, 1, 1, 2, 1);

  myUpdateStudyBtn = new QPushButton(tr("BUT_UPDATE_STUDY"));
  aLayout->addWidget(myUpdateStudyBtn, 1, 2, 1, 1);
  
  QGroupBox* groupBox = new QGroupBox(this);

  QGridLayout* aLayout1 = new QGridLayout(groupBox);

  aLayout1->setMargin(DEFAULT_MARGIN);
  aLayout1->setSpacing(DEFAULT_SPACING);

  myOkBtn = new QPushButton(tr("BUT_OK"));
  aLayout1->addWidget(myOkBtn, 0, 0, 1, 1);
  
  myApplyBtn = new QPushButton(tr("BUT_APPLY"));
  aLayout1->addWidget(myApplyBtn, 0, 1, 1, 1);  

  QSpacerItem* spacer1 =
    new QSpacerItem(SPACER_SIZE, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);
  aLayout1->addItem(spacer1, 0, 2, 1, 1);


  myCancelBtn = new QPushButton(tr("BUT_CANCEL"));
  aLayout1->addWidget(myCancelBtn, 0, 3, 1, 1);

  aLayout->addWidget(groupBox, 2, 0, 1, 3);

  connect( myOkBtn, SIGNAL(clicked()), this, SLOT(onOK()) );
  connect( myApplyBtn, SIGNAL(clicked()), this, SLOT(onApply()) );
  connect( myUpdateStudyBtn, SIGNAL(clicked()), this, SLOT(onUpdateStudy()) );
  connect( myCancelBtn, SIGNAL(clicked()), this, SLOT(reject()) );
  connect( myRemoveButton, SIGNAL(clicked()), this, SLOT(onRemove()));

  connect( myTable, SIGNAL(incorrectItemAdded()), this, SLOT(onIncorrectItemAdded()));
  connect( myTable, SIGNAL(incorrectItemRemoved()), this, SLOT(onIncorrectItemRemoved()));
  connect( myTable, SIGNAL(selectionChanged(bool)),this, SLOT(onTableSelectionChanged(bool)));

  myTable->Init(myStudy);
  myUpdateStudyBtn->setEnabled(false);
}

//============================================================================
/*! Function : ~SalomeApp_NoteBookDlg
 *  Purpose  : Destructor
 */
//============================================================================
SalomeApp_NoteBookDlg::~SalomeApp_NoteBookDlg(){}


//============================================================================
/*! Function : onIncorrectItemAdded
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onIncorrectItemAdded()
{
  myOkBtn->setEnabled(false);
  myApplyBtn->setEnabled(false);
}

//============================================================================
/*! Function : onIncorrectItemRemoved
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onIncorrectItemRemoved()
{
  myOkBtn->setEnabled(true);
  myApplyBtn->setEnabled(true);
}

//============================================================================
/*! Function : onTableSelectionChanged
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onTableSelectionChanged(bool flag)
{
  myRemoveButton->setEnabled(flag);
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
/*! Function : onOK
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onOK()
{
  onApply();
  hide();
}

//============================================================================
/*! Function : onApply
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onApply()
{
  QList<NoteBook_TableRow*> aRows = myTable->GetRows();
  double aDVal;
  int    anIVal;
  bool   aBVal;
  int i = 0;
  myTable->SetProcessItemChangedSignalFlag(false);
  
  for(;i<aRows.size()-1;i++) {
    
    if( NoteBook_TableRow::IsIntegerValue(aRows[i]->GetValue(),&anIVal) )
      myStudy->SetInteger(string(aRows[i]->GetName().toLatin1().constData()),anIVal);

    else if( NoteBook_TableRow::IsRealValue(aRows[i]->GetValue(),&aDVal) )
      myStudy->SetReal(string(aRows[i]->GetName().toLatin1().constData()),aDVal);
    
    else if( NoteBook_TableRow::IsBooleanValue(aRows[i]->GetValue(),&aBVal) )
      myStudy->SetBoolean(string(aRows[i]->GetName().toLatin1().constData()),aBVal);

    if( aRows[i]->IsNameEditable())
      aRows[i]->SetNameEditable(false);
  }
  myTable->SetProcessItemChangedSignalFlag(true);
}


//============================================================================
/*! Function : onUpdateStudy
 *  Purpose  : [slot]
 */
//============================================================================
void SalomeApp_NoteBookDlg::onUpdateStudy()
{

}
