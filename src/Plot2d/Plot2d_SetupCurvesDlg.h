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
#include <QVector>
#include <QList>

class QtxGroupBox;
class QTableWidget;
class QwtLegend;
class QToolButton;

#if defined WIN32
#pragma warning( disable: 4251 )
#endif

/*!
  \class Plot2d_SetupCurvesDlg
  Dialog box for setup Plot2d curves parameters 
*/

class PLOT2D_EXPORT Plot2d_SetupCurvesDlg : public QtxDialog
{ 
  Q_OBJECT

public:
  Plot2d_SetupCurvesDlg( QWidget* theParent = 0 );
  virtual ~Plot2d_SetupCurvesDlg();

  void                      SetParameters( const QVector< int >& theMarker,
                                           const QVector< QString >& theText,
                                           const QVector< QColor >& theColor,
                                           const QVector< int >& theNbMarkers );

  void                      GetParameters( QVector< int >& theMarker,
                                           QVector< QString >& theText,
                                           QVector< QColor >& theColor,
                                           QVector< int >& theNbMarkers ) const;

  const QList< int >&       GetRemovedIndexes() const;

protected:

  virtual bool              acceptData() const;

private slots:

  void                      onRemove();
  void                      onColorBtn();
  void                      onValueChanged( int, int );
  void                      onMarkerType();

private:

  const QStringList&        getMarkers() const;
  void                      updatePixmap( const int theRow );
  void                      setColor( QPushButton* theBtn, const QColor& theColor ) const;
  void                      setCombo( const int theRow, 
                                      const int theCol, 
                                      const QStringList& theItems,
                                      const int theCurrItem = 0,
                                      const int theWidth = -1 );
  void                      setText( const int theRow,
                                     const int theCol,
                                     const QString& theText );
private:

  QtxGroupBox*              myGrp;
  QTableWidget*             myTable;
  QToolButton*              myRemoveBtn;
  QList< int >              myRemovedIndexes;
};

/*!
  \class Plot2d_PixmapWg
  \internal
  \brief This class is derived from QWidget is intended for drawing curves symbol 
  inside its area. It contains QwtSymbol as its internal field and delegates paint 
  event to it for drawing
*/
class PLOT2D_EXPORT Plot2d_PixmapWg : public QWidget
{
public: 
  Plot2d_PixmapWg( QWidget* theParent );
  virtual ~Plot2d_PixmapWg();

  void setColor( const QColor& );
  void setMarkerType( const int );
  void setText( const QString& );

protected:
  virtual void paintEvent( QPaintEvent * event );

private:
  QwtSymbol mySymbol;
  QString myText;
};

#endif 





