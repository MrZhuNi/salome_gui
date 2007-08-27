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
//  SALOME Plot2d : implementation of desktop and GUI kernel
//
//  File   : Plot2d_SetupCurveDlg.h
//  Author : Vadim SANDLER
//  Module : SALOME

#ifndef Plot2d_SetupCurveDlg_H
#define Plot2d_SetupCurveDlg_H

#include <QtxDialog.h>

#include <qmap.h>

#include "Plot2d.h"

/*!
  \class Plot2d_SetupCurveDlg
  Dialog box for setup Plot2d curve
*/

class QListBox;
class QLineEdit;
class QPushButton;
class QComboBox;
class QSpinBox;
class QToolButton;
class Plot2d_ViewFrame;

class PLOT2D_EXPORT Plot2d_SetupCurveDlg : public QtxDialog
{
  Q_OBJECT

public:
  Plot2d_SetupCurveDlg( QWidget* parent = 0 );
  Plot2d_SetupCurveDlg( Plot2d_ViewFrame* vf, QWidget* parent = 0 );
  ~Plot2d_SetupCurveDlg();

public:
  void    setCurrentCurve( const int );

  void    setName( const QString );
  QString getName( const int id = -1 ) const;
  void    setLine( const int line, const int width );
  int     getLine( const int id = -1 ) const;
  int     getLineWidth( const int id = -1 ) const;
  void    setMarker( const int marker );
  int     getMarker( const int id = -1 ) const;
  void    setColor( const QColor& color );
  QColor  getColor( const int id = -1 ) const;
  int     getNbCurves() const;
  
private:
  void   createControls( Plot2d_ViewFrame* vf );
  void   init( Plot2d_ViewFrame* vf );
  bool   storeCurveProps( const int id = -1 );
  void   retrieveCurveProps( const int id );

private slots:
  void   onCurveChanged();
  void   onColorChanged();
  void   onNameChanged( const QString& );
  void   onOk();

private:

  struct CurveProp
  {
    CurveProp()
    { line = width = marker = -1; }
    int     line;
    int     width;
    int     marker;
    QColor  color;
    QString name;
  };

  int                  myCurrent;
  QListBox*            myCurveBox;
  QMap<int, CurveProp> myCurvesProps;
  QLineEdit*           myName;
  QComboBox*           myLineCombo;
  QSpinBox*            myLineSpin;
  QComboBox*           myMarkerCombo;
  QToolButton*         myColorBtn;
};

#endif // Plot2d_SetupCurveDlg_H
