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
#ifndef PLOT3D_COLORDIC_H
#define PLOT3D_COLORDIC_H

#include "Plot3d.h"

#include <Quantity_Color.hxx>
#include <TCollection_AsciiString.hxx>

#include <QColor>
#include <QPair>

class vtkLookupTable;

typedef QPair< double, QColor >      Value2Color;
typedef QList< Value2Color >         Value2ColorList;
typedef QListIterator< Value2Color > Value2ColorListIterator;

class PLOT3D_EXPORT Plot3d_ColorDic
{
public:
  enum ScaleMode { Linear = 0, Logarithmic, Specific };

public:
  Plot3d_ColorDic();
  Plot3d_ColorDic( const int );
  Plot3d_ColorDic( const int, const double, const double );
  ~Plot3d_ColorDic();

  int                            GetNumber() const;
  void                           SetNumber( const int );

  double                         GetMin() const;
  double                         GetMax() const;
  void                           SetMin( const double );
  void                           SetMax( const double );
  void                           SetRange( const double, const double );

  void                           GetHSVRange( double&, double&,
                                              double&, double&,
                                              double&, double& );
  void                           SetHSVRange( const double, const double,
                                              const double, const double,
                                              const double, const double );

  ScaleMode                      GetScaleMode() const;
  void                           SetScaleMode( const ScaleMode );

  const Value2ColorList&         GetMainSpecificScale() const;
  void                           SetMainSpecificScale( const Value2ColorList& );

  const Value2ColorList&         GetSpecificScale() const;
  void                           SetSpecificScale( const Value2ColorList& );

  int                            GetTimeStep() const;
  void                           SetTimeStep( const int );

  TCollection_AsciiString        GetQuantity() const;
  void                           SetQuantity( const TCollection_AsciiString& );

  bool                           GetColorByIndex( const int, Quantity_Color& ) const;
  bool                           GetColorByValue( const double, Quantity_Color& ) const;

  int                            GetColorMode() const;
  void                           SetColorMode( const int );

  void                           GetCustomColors( QColor&, QColor& ) const;
  void                           SetCustomColors( const QColor&, const QColor& );

private:
  void                           Init();
  void                           UpdateLookupTable();
  bool                           GetSpecificColor( const double, double[3] ) const;
  int                            Index( const double ) const;
  double                         Value( const int ) const;
  int                            HueFromValue( const int, const int, const int ) const;

private:
  int                            myNum;
  double                         myMin;
  double                         myMax;
  int                            myTimeStep;
  TCollection_AsciiString        myQuantity;
  int                            myColorMode;
  QColor                         myCustomColors[2];

  double                         myHueMin;
  double                         myHueMax;
  double                         mySaturationMin;
  double                         mySaturationMax;
  double                         myValueMin;
  double                         myValueMax;

  ScaleMode                      myScaleMode;
  Value2ColorList                myMainSpecificColorScale;
  Value2ColorList                mySpecificColorScale;

  vtkLookupTable*                myLookupTable;
};

#endif
