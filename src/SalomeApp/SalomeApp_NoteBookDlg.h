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

class SalomeApp_Notebook;
class SalomeApp_Study;

struct NoteBoox_Variable
{
  NoteBoox_Variable() {}
  NoteBoox_Variable( const QString& theName, const QString& theExpression )
  {
    Name = theName;
    Expression = theExpression;
  }
  QString Name;
  QString Expression;
};

typedef QMap< int, NoteBoox_Variable > VariableMap;

class SALOMEAPP_EXPORT NoteBook_TableRow : public QWidget
{
 public:
  NoteBook_TableRow( int, QWidget* parent = 0 );
  virtual ~NoteBook_TableRow();

  int                       getIndex() const { return myIndex; }
  
  void                      addToTable( QTableWidget *theTable );
  
  void                      setVariable( const QString& theVariable );
  void                      setExpression( const QString& theExpression );
  void                      setValue( const QString& theValue );

  QString                   getVariable() const;
  QString                   getExpression() const;
  QString                   getValue() const;

  QTableWidgetItem*         getRowHeaderItem() { return myRowHeaderItem; }
  QTableWidgetItem*         getVariableItem() { return myVariableItem; }
  QTableWidgetItem*         getExpressionItem() { return myExpressionItem; }
  QTableWidgetItem*         getValueItem() { return myValueItem; }

  static bool               isRealValue( const QString theValue, double* theResult = 0 );
  static bool               isIntegerValue( const QString theValue, int* theResult = 0 );
  static bool               isBooleanValue( const QString theValue, bool* theResult = 0 );
  static bool               isValidStringValue( const QString theName );
  
 private:
  int                       myIndex;
  QTableWidgetItem*         myRowHeaderItem;
  QTableWidgetItem*         myVariableItem;
  QTableWidgetItem*         myExpressionItem;
  QTableWidgetItem*         myValueItem;
};

class SALOMEAPP_EXPORT NoteBook_Table : public QTableWidget
{
  Q_OBJECT

public:
  NoteBook_Table( QWidget* parent = 0 );
  virtual ~NoteBook_Table();

  void                      init( SalomeApp_Notebook* theNoteBook );
  void                      addUndefinedParameters( const QStringList& theParameters );

  bool                      isValid() const;

  void                      removeSelected();

  bool                      updateNoteBook();

protected slots:
  void                      onItemChanged( QTableWidgetItem* theItem );

private:
  void                      clear();
  int                       getUniqueIndex() const;
  void                      markItem( NoteBook_TableRow* theRow, int theColumn, bool theIsCorrect );
  bool                      checkItem( NoteBook_TableRow* theRow, int theColumn ) const;
  void                      markRow( NoteBook_TableRow* theRow, bool theIsCorrect );
  bool                      checkRow( NoteBook_TableRow* theRow ) const;
  void                      updateExpressions( int theBaseRowIndex );
  void                      updateValues();

  bool                      setExpression( const QString& theName,
                                           const QString& theExpression,
                                           bool theIsNew );

  bool                      renameVariable( const QString& theOldName,
                                            const QString& theNewName );

  void                      addRow( const QString& theName = QString::null,
                                    const QString& theExpression = QString::null );

  NoteBook_TableRow*        getRowByItem( const QTableWidgetItem* theItem ) const;
  bool                      isLastRow( const NoteBook_TableRow* aRow ) const;

  void                      processItemChanged( NoteBook_TableRow* theRow, int theColumn );

private:
  QList<NoteBook_TableRow*> myRows;

  VariableMap               myVariableMap;

  SalomeApp_Notebook*       myNoteBook;
};

class SALOMEAPP_EXPORT SalomeApp_NoteBookDlg : public QDialog 
{
  Q_OBJECT

public:
  SalomeApp_NoteBookDlg( QWidget* parent, SalomeApp_Study* theStudy );
  virtual ~SalomeApp_NoteBookDlg();

  void                      init( SalomeApp_Study* theStudy );
  void                      addUndefinedParameters( const QStringList& theParameters );
  
protected slots:
   void                     onRemove();
   void                     onDump();
   void                     onUpdateStudy();
   void                     onClose();
   void                     onHelp();

protected:
   bool                     updateStudy();
   void                     clearStudy();

private:
  NoteBook_Table*           myTable;
  SalomeApp_Notebook*       myNoteBook;
};

#endif //SALOMEAPP_NOTEBOOKDLG_H
