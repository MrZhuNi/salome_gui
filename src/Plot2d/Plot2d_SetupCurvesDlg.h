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
//  File   : Plot2d_SetupCurvesDlg.h
//  Author : Sergey LITONIN

#ifndef PLOT2D_SETUPCURVESDLG_H
#define PLOT2D_SETUPCURVESDLG_H

#include "Plot2d.h"
#include <QtxDialog.h>
#include <qvaluevector.h>
#include <qvaluelist.h>

class QtxGroupBox;
class QwtLegend;
class QToolButton;
class QtxTable;

/*!
  \class Plot2d_SetupCurvesDlg
  Dialog box for setup Plot2d curves parameters 
*/

class PLOT2D_EXPORT Plot2d_SetupCurvesDlg : public QtxDialog
{ 
  Q_OBJECT

  class PixmapItem;

public:
  Plot2d_SetupCurvesDlg( QWidget* theParent = 0 );
  virtual ~Plot2d_SetupCurvesDlg();

  void                      SetParameters( const QValueVector< int >& theMarker,
                                           const QValueVector< QString >& theText,
                                           const QValueVector< QColor >& theColor,
                                           const QValueVector< int >& theNbMarkers );

  void                      GetParameters( QValueVector< int >& theMarker,
                                           QValueVector< QString >& theText,
                                           QValueVector< QColor >& theColor,
                                           QValueVector< int >& theNbMarkers ) const;

  const QValueList< int >&  GetRemovedIndexes() const;

protected:

  virtual bool              acceptData() const;

private slots:

  void                      onRemove();
  void                      onColorBtn();
  void                      onValueChanged( int, int );

private:

  const QStringList&        getMarkers() const;
  void                      updatePixmap( const int theRow );
  void                      setColor( QPushButton* theBtn, const QColor& theColor ) const;
  
private:

  QtxGroupBox*              myGrp;
  QtxTable*                 myTable;
  QToolButton*              myRemoveBtn;
  QValueList< int >         myRemovedIndexes;
};

#endif 





