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

#include <QDialog>
#include <QTableWidget>
#include <QList>

class QPushButton;
class QTableWidgetItem;

class SalomeApp_Notebook;
class SalomeApp_Study;

struct NoteBoox_Variable
{
  NoteBoox_Variable() {}
  NoteBoox_Variable( const QString& theName, const QString& theValue )
  {
    Name = theName;
    Value = theValue;
  }
  QString Name;
  QString Value;
};

typedef QMap< int, NoteBoox_Variable > VariableMap;

class SALOMEAPP_EXPORT NoteBook_TableRow : public QWidget
{
 public:
  NoteBook_TableRow(int, QWidget* parent=0);
  virtual ~NoteBook_TableRow();

  int                       GetIndex() const { return myIndex; }
  
  void                      AddToTable(QTableWidget *theTable);
  
  void                      SetName(const QString theName);
  void                      SetValue(const QString theValue);

  QString                   GetValue() const;
  QString                   GetName() const;

  QTableWidgetItem*         GetVariableItem();
  QTableWidgetItem*         GetNameItem();
  QTableWidgetItem*         GetHeaderItem();

  static bool               IsRealValue(const QString theValue, double* theResult = 0);
  static bool               IsIntegerValue(const QString theValue, int* theResult = 0);
  static bool               IsBooleanValue(const QString theValue, bool* theResult = 0);
  static bool               IsValidStringValue(const QString theName);
  
 private:
  int                       myIndex;
  QTableWidgetItem*         myRowHeader;
  QTableWidgetItem*         myVariableName;
  QTableWidgetItem*         myVariableValue;
};

class SALOMEAPP_EXPORT NoteBook_Table : public QTableWidget
{
  Q_OBJECT

public:
  NoteBook_Table( QWidget* parent = 0 );
  virtual ~NoteBook_Table();

  void                      Init( SalomeApp_Notebook* theNoteBook );

  bool                      IsValid() const;

  void                      AddRow( const QString& theName = QString::null,
                                    const QString& theValue = QString::null );

  NoteBook_TableRow*        GetRowByItem(const QTableWidgetItem* theItem) const;
  bool                      IsLastRow(const NoteBook_TableRow* aRow) const;

  void                      RemoveSelected();

protected slots:
  void                      onItemChanged(QTableWidgetItem* theItem);

private:
  void                      printVariableMap(); // tmp
  int                       getUniqueIndex() const;
  void                      markItem( NoteBook_TableRow* theRow, int theColumn, bool theIsCorrect );
  bool                      checkItem( QTableWidgetItem* theItem ) const;

private:
  QList<NoteBook_TableRow*> myRows;

  VariableMap               myVariableMap;

  SalomeApp_Notebook*       myNoteBook;
};

class SALOMEAPP_EXPORT SalomeApp_NoteBookDlg : public QDialog 
{
  Q_OBJECT

public:
  SalomeApp_NoteBookDlg(QWidget* parent, SalomeApp_Study* theStudy);
  virtual ~SalomeApp_NoteBookDlg();

  void                      Init(SalomeApp_Study* theStudy);
  
public slots:
   void                     onRemove();
   void                     onUpdateStudy();
   void                     onClose();
   void                     onHelp();

protected:
   bool                     updateStudy();
   void                     clearStudy();

private:
  NoteBook_Table*           myTable;
  QPushButton*              myRemoveButton;
  QPushButton*              myUpdateStudyBtn;
  QPushButton*              myCloseBtn;
  QPushButton*              myHelpBtn;
  
  SalomeApp_Notebook*       myNoteBook;
};

#endif //SALOMEAPP_NOTEBOOKDLG_H
