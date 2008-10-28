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

#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QFont>

#include <string>
#include <vector>

using namespace std;

#define DEFAULT_MARGIN  11
#define DEFAULT_SPACING 6
#define SPACER_SIZE     30
#define COLUMN_SIZE     175


///////////////////////////////////////////////////////////////////////////
//                 NoteBook_TableRow class                               //
///////////////////////////////////////////////////////////////////////////
NoteBook_TableRow::NoteBook_TableRow(QWidget* parent):
  QWidget(parent),
  myRowHeader(new QTableWidgetItem()),
  myVariableName(new QTableWidgetItem()),
  myVariableValue(new QTableWidgetItem())
{
}

NoteBook_TableRow::~NoteBook_TableRow()
{
}

void NoteBook_TableRow::AddToTable(QTableWidget *theTable)
{
  int aPosition = theTable->rowCount();
  int aRowCount = aPosition+1;
  theTable->setRowCount(aRowCount);
  myRowHeader->setText(QString::number(aRowCount));

  theTable->setVerticalHeaderItem(aPosition,myRowHeader);
  theTable->setItem(aPosition,0,myVariableName);
  theTable->setItem(aPosition,1,myVariableValue);
}

void NoteBook_TableRow::setEditable(bool enable)
{

}

void NoteBook_TableRow::SetName(const QString theName)
{
  myVariableName->setText(theName);
}
void NoteBook_TableRow::SetValue(const QString theValue)
{
  myVariableValue->setText(theValue);
}

///////////////////////////////////////////////////////////////////////////
//                      NoteBook_Table class                             //
///////////////////////////////////////////////////////////////////////////
NoteBook_Table::NoteBook_Table(QWidget * parent)
  :QTableWidget(parent)
{
  setColumnCount(2);

  //Add Headers Columns
  QFont aFont = QFont( "Liberation Sans", 10);
  aFont.setBold(true);
  
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

NoteBook_Table::~NoteBook_Table(){}

void NoteBook_Table::Init(_PTR(Study) theStudy)
{

  //Add all variables into the table
  vector<string> aVariables = theStudy->GetVariableNames();
  for(int iVar = 0; iVar < aVariables.size()  ; iVar++ ) {
    NoteBook_TableRow* aRow = new NoteBook_TableRow(this);
    aRow->SetName(QString(aVariables[iVar].c_str()));
    aRow->AddToTable(this);
  }

  //Add empty row

  NoteBook_TableRow* aRow = new NoteBook_TableRow(this);
  aRow->AddToTable(this);
}

void NoteBook_Table::onItemChanged(QTableWidgetItem* theItem)
{

}


///////////////////////////////////////////////////////////////////////////
//                  SalomeApp_NoteBookDlg class                          //
///////////////////////////////////////////////////////////////////////////
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
  aLayout->addWidget(myTable, 0, 0, 1, 5);
  
  //Buttons
  myOkBtn = new QPushButton(tr("BUT_OK"));
  aLayout->addWidget(myOkBtn, 1, 0, 1, 1);
  
  myApplyBtn = new QPushButton(tr("BUT_APPLY"));
  aLayout->addWidget(myApplyBtn, 1, 1, 1, 1);

  myUpdateStudyBtn = new QPushButton(tr("BUT_UPDATE_STUDY"));
  aLayout->addWidget(myUpdateStudyBtn, 1, 2, 1, 1);
  

  QSpacerItem* spacer =
    new QSpacerItem(SPACER_SIZE, SPACER_SIZE, QSizePolicy::Expanding, QSizePolicy::Minimum);
  aLayout->addItem(spacer, 1, 3, 1, 1);


  myCancelBtn = new QPushButton(tr("BUT_CANCEL"));
  aLayout->addWidget(myCancelBtn, 1, 4, 1, 1);

  connect( myOkBtn, SIGNAL(clicked()), this, SLOT(onOK()) );
  connect( myApplyBtn, SIGNAL(clicked()), this, SLOT(onApply()) );
  connect( myUpdateStudyBtn, SIGNAL(clicked()), this, SLOT(onUpdateStudy()) );
  connect( myCancelBtn, SIGNAL(clicked()), this, SLOT(reject()) );

  myTable->Init(myStudy);
}


SalomeApp_NoteBookDlg::~SalomeApp_NoteBookDlg(){}

void SalomeApp_NoteBookDlg::onOK()
{

}

void SalomeApp_NoteBookDlg::onApply()
{

}

void SalomeApp_NoteBookDlg::onUpdateStudy()
{

}
