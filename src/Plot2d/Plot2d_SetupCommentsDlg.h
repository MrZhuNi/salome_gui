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
#ifndef PLOT2D_SETUPCOMMENTSDLG_H
#define PLOT2D_SETUPCOMMENTSDLG_H

#include "Plot2d.h"

#include <QtxDialog.h>

#include <QVector>
#include <QList>

class QTableWidget;

/*!
  \class Plot2d_SetupCommentsDlg
  Dialog box for setup Plot2d comments parameters 
*/

class PLOT2D_EXPORT Plot2d_SetupCommentsDlg : public QtxDialog
{ 
  Q_OBJECT

  class ItemDelegate;

public:
  Plot2d_SetupCommentsDlg( QWidget* theParent = 0 );
  virtual ~Plot2d_SetupCommentsDlg();

  void                      SetParameters( const QVector< QString >& theTextList,
                                           const QVector< double >& theXList,
                                           const QVector< double >& theYList,
                                           const QVector< Qt::Alignment >& theAlignmentList );

  void                      GetParameters( QVector< QString >& theTextList,
                                           QVector< double >& theXList,
                                           QVector< double >& theYList,
                                           QVector< Qt::Alignment >& theAlignmentList ) const;

  const QList< int >&       GetRemovedIndexes() const;

private slots:
  void                      onRemove();

private:
  void                      getAlignments( QStringList& theStrings,
                                           QList< Qt::Alignment >& theValues ) const;

  void                      setCombo( const int theRow, 
                                      const int theCol, 
                                      const QStringList& theStrings,
                                      const QList< Qt::Alignment >& theValues,
                                      const Qt::Alignment theCurrentValue );

  void                      setText( const int theRow,
                                     const int theCol,
                                     const QString& theText );

  QString                   getText( const int theRow,
                                     const int theCol ) const;

private:
  QTableWidget*             myTable;
  QList< int >              myRemovedIndexes;
};

#endif 
