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
// File:    SalomeApp_NoteBookDlg.h
// Author : Roman NIKOLAEV, Open CASCADE S.A.S.
// Module : GUI

#ifndef SALOMEAPP_NOTEBOOKDLG_H
#define SALOMEAPP_NOTEBOOKDLG_H

#include "SalomeApp.h"

#include "SALOMEDSClient_ClientFactory.hxx" 
#include CORBA_SERVER_HEADER(SALOMEDS)

#include <QDialog>
#include <QTableWidget>
#include <QList>

class QWidget;
class QPushButton;
class QTableWidgetItem;

class SALOMEAPP_EXPORT NoteBook_TableRow : public QWidget
{
 public:
  NoteBook_TableRow(QWidget* parent=0);
  virtual ~NoteBook_TableRow();
  
  void SetNameEditable(bool enable);
  bool IsNameEditable();

  void AddToTable(QTableWidget *theTable);
  
  void SetName(const QString theName);
  void SetValue(const QString theValue);

  QString GetValue() const;
  QString GetName() const;

  bool CheckName();
  bool CheckValue();

  void setValueEditable(bool enable);
  void setNameEditable(bool enable);

  QTableWidgetItem* GetVariableItem();
  QTableWidgetItem* GetNameItem();

  static bool IsRealValue(const QString theValue, double* theResult = 0);
  static bool IsIntegerValue(const QString theValue, int* theResult = 0);
  static bool IsBooleanValue(const QString theValue, bool* theResult = 0);
  
 private:
  QTableWidgetItem* myRowHeader;
  QTableWidgetItem* myVariableName;
  QTableWidgetItem* myVariableValue;
  bool              isNameEditable;

};

class SALOMEAPP_EXPORT NoteBook_Table : public QTableWidget
{
  Q_OBJECT
 public:
  NoteBook_Table(QWidget * parent = 0);
  virtual ~NoteBook_Table();

  void Init(_PTR(Study) theStudy);
  static QString Variable2String(const std::string& theVarName,
				 _PTR(Study) theStudy);
  void AddEmptyRow();
  NoteBook_TableRow* GetRowByItem(const QTableWidgetItem* theItem) const;
  bool IsLastRow(const NoteBook_TableRow* aRow) const;

  void RemoveSelected();

  void SetIncorrectItem(QTableWidgetItem* theItem);
  void RemoveIncorrectItem();
  
  void SetProcessItemChangedSignalFlag(const bool enable);
  bool GetProcessItemChangedSignalFlag()const;

  bool IsUniqueName(const NoteBook_TableRow* theRow) const;
  QList<NoteBook_TableRow*> GetRows() const;
  
  public slots:
    void onItemChanged(QTableWidgetItem* theItem);
    void onItemSelectionChanged();

    
 signals:
    void incorrectItemAdded();
    void incorrectItemRemoved();
    void selectionChanged(bool);

 private:
  bool isProcessItemChangedSignal;
  QTableWidgetItem*                  myIncorrectItem;
  QList<NoteBook_TableRow*>          myRows;
};

class SALOMEAPP_EXPORT SalomeApp_NoteBookDlg : public QDialog 
{
  Q_OBJECT
 public:
  SalomeApp_NoteBookDlg(QWidget * parent , _PTR(Study) theStudy);
  virtual ~SalomeApp_NoteBookDlg();
  
 public slots:
   void onOK();
   void onApply();
   void onUpdateStudy();
   void onRemove();
   void onIncorrectItemAdded();
   void onIncorrectItemRemoved();
   void onTableSelectionChanged(bool flag);

 private:
  NoteBook_Table*  myTable;
  QPushButton*     myRemoveButton;
  QPushButton*     myUpdateStudyBtn;
  QPushButton*     myOkBtn;
  QPushButton*     myApplyBtn;
  QPushButton*     myCancelBtn;
  
  _PTR(Study)      myStudy;
  
};

#endif //SALOMEAPP_NOTEBOOKDLG_H
