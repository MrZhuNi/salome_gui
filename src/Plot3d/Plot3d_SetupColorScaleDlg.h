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
#ifndef PLOT3D_SETUPCOLORSCALEDLG_H
#define PLOT3D_SETUPCOLORSCALEDLG_H

#include "Plot3d.h"

#include "Plot3d_ColorDic.h"

#include <QtxDialog.h>

class QButtonGroup;
class QComboBox;
class QDoubleSpinBox;

class QtxColorButton;
class QtxIntSpinBox;

class Plot3d_ColorDic;

struct ColorDicData
{
  int Num;
  double Min, Max;
  double HueMin, HueMax;
  double SaturationMin, SaturationMax;
  double ValueMin, ValueMax;
  Plot3d_ColorDic::ScaleMode ScaleMode;
  Value2ColorList MainSpecificColorScale;
  Value2ColorList SpecificColorScale;
  int TimeStep;
  TCollection_AsciiString Quantity;
  int ColorMode;
  QColor CustomColors[2];

  ColorDicData()
  {
    Num = 99;
    Min = 0;
    Max = 1;
    HueMin = 0.667;
    HueMax = 0;
    SaturationMin = 1;
    SaturationMax = 1;
    ValueMin = 1;
    ValueMax = 1;
    TimeStep = 0;
    ColorMode = 0;
  };
};

typedef QList        < ColorDicData > ColorDicDataList;
typedef QListIterator< ColorDicData > ColorDicDataListIterator;

/*
  Class       : Plot3d_SetupColorScaleDlg
  Description : Dialog box for setup color scale
*/
class PLOT3D_EXPORT Plot3d_SetupColorScaleDlg : public QtxDialog
{ 
  Q_OBJECT

public:
  enum { BlueRed = 0, BlueWhite, Monochrome, Custom };

public:
  Plot3d_SetupColorScaleDlg( QWidget* theParent = 0,
                             bool theIsGlobal = false );
  virtual ~Plot3d_SetupColorScaleDlg();

  void                      setData( const ColorDicData& theColorDicData );
  ColorDicData              getData() const;

private slots:
  void                      onMinMaxChanged( double );
  void                      onIntervalNumberChanged( int );
  void                      onColorModeChanged( int );
  void                      onColorScaleModeChanged( int );
  void                      onColorScaleSpecificSetup();

private:
  int                       colorScaleMode() const;
  void                      setColorScaleMode( const int );

  int                       checkScaleMode();

  void                      updateState();
  void                      updateColorScale();
  void                      updateMajorSpecificColorScale();
  void                      updateMinorSpecificColorScale();
  void                      updateMinMax();

private:
  bool                      myIsGlobal;

  QtxIntSpinBox*            myInterval;

  QComboBox*                myColorMode;
  QtxColorButton*           myBottomColor;
  QtxColorButton*           myTopColor;

  QButtonGroup*             myColorScaleMode;

  QDoubleSpinBox*           myMinLimit;
  QDoubleSpinBox*           myMaxLimit;

  Value2ColorList           myMajorSpecificColorScale;
  Value2ColorList           myMinorSpecificColorScale;
};

#endif 
