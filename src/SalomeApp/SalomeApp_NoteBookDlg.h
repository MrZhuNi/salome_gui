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


class QWidget;
class QPushButton;
class QTableWidgetItem;


class SALOMEAPP_EXPORT NoteBook_TableRow : public QWidget
{
 public:
  NoteBook_TableRow(QWidget* parent=0);
  virtual ~NoteBook_TableRow();
  
  void setEditable(bool enable);
  void AddToTable(QTableWidget *theTable);
  
  void SetName(const QString theName);
  void SetValue(const QString theValue);
  
 private:
  QTableWidgetItem* myRowHeader;
  QTableWidgetItem* myVariableName;
  QTableWidgetItem* myVariableValue;

};

class SALOMEAPP_EXPORT NoteBook_Table : public QTableWidget
{
  Q_OBJECT
 public:
  NoteBook_Table(QWidget * parent = 0);
  virtual ~NoteBook_Table();

  void Init(_PTR(Study) theStudy);
  
  public slots:
    void onItemChanged(QTableWidgetItem* theItem);

 private:
  bool isInitialized;
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

 private:
  NoteBook_Table*  myTable;
  QPushButton*     myOkBtn;
  QPushButton*     myApplyBtn;
  QPushButton*     myCancelBtn;
  QPushButton*     myUpdateStudyBtn;
  
  _PTR(Study) myStudy;
  
};

#endif //SALOMEAPP_NOTEBOOKDLG_H
