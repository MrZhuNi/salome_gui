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
#ifndef PLOT3D_SETUPSPECIFICCOLORSCALEDLG_H
#define PLOT3D_SETUPSPECIFICCOLORSCALEDLG_H

#include "Plot3d.h"

#include "Plot3d_ColorDic.h"

#include <QtxDialog.h>

class QtxColorButton;
class QtxGroupBox;

class QCheckBox;
class QSpinBox;
class QDoubleSpinBox;
class QTableWidget;
class QTableWidgetItem;

/*
  Class       : Plot3d_SetupSpecificColorScaleDlg
  Description : Dialog box for setup specific color scale
*/
class PLOT3D_EXPORT Plot3d_SetupSpecificColorScaleDlg : public QtxDialog
{ 
  Q_OBJECT

  class ItemDelegate;

public:
  Plot3d_SetupSpecificColorScaleDlg( QWidget* theParent = 0 );
  virtual ~Plot3d_SetupSpecificColorScaleDlg();

  void                      SetMajorScale( const Value2ColorList& theMajorScale );
  const Value2ColorList&    GetMajorScale() const;

  void                      SetNewVar( const bool theIsNew, 
                                       const double theMin = 0, 
                                       const double theMax = 0 );

  static Value2ColorList    interpolateRange( const Value2Color& theMinimum,
                                              const Value2Color& theMaximum,
                                              const int theNumber,
                                              const bool theIsMajor = false,
                                              const bool theIsLogarithmic = false );

protected slots:
  virtual void              accept();

private slots:
  void                      onGenerateTable();
  void                      onColorBtn();
  void                      onImportSettings();
  void                      onExportSettings();

private:
  void                      clearTable();
  void                      updateTable( const Value2ColorList& );

  bool                      value( const int theRow, double& theValue ) const;
  void                      setValue( const int theRow, const double theValue );
  void                      setValue( const int theRow, const QString& theValue );

  QColor                    color( QPushButton* theBtn ) const;
  void                      setColor( QPushButton* theBtn, const QColor& theColor );

  void                      setColor( const int theRow, const QColor& theColor );

  void                      setEditable( const int theRow, const int theCol, const bool theState );

  bool                      checkMajorScale( QString& theMessage ) const;

private:
  QSpinBox*                 myNbSpecificIntervalsSpinBox;

  QtxGroupBox*              myAutoValues;
  QDoubleSpinBox*           myMinimumSpinBox;
  QDoubleSpinBox*           myMaximumSpinBox;
  QCheckBox*                myIsLogarithmic;

  QtxGroupBox*              myAutoColors;
  QtxColorButton*           myMinimumBtn;
  QtxColorButton*           myMaximumBtn;

  QPushButton*              myGenerateButton;

  QTableWidget*             myTable;

  Value2Color               myMinimum;
  Value2Color               myMaximum;

  Value2ColorList           myMajorScale;
  Value2ColorList           myMinorScale;
};

#endif 
