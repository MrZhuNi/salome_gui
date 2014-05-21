// File:      Plot3d_ColorDic.cxx
// Created:   November, 2004
// Author:    OCC team
// Copyright (C) CEA 2004

#include "Plot3d_ColorDic.h"

#include <Precision.hxx>

#include <vtkLookupTable.h>

//=======================================================================
//function : Plot3d_ColorDic
//purpose  : Constructor
//=======================================================================
Plot3d_ColorDic::Plot3d_ColorDic()
: myNum( 99 ),
  myMin( 0 ),
  myMax( 1 ),
  myTimeStep( 0 ),
  myHueMin( 0.667 ),
  myHueMax( 0 ),
  mySaturationMin( 1 ),
  mySaturationMax( 1 ),
  myValueMin( 1 ),
  myValueMax( 1 ),
  myScaleMode( Linear )
{
  Init();
}

//=======================================================================
//function : Plot3d_ColorDic
//purpose  : Constructor
//=======================================================================
Plot3d_ColorDic::Plot3d_ColorDic( const int num )
: myNum( num ),
  myMin( 0 ),
  myMax( 1 ),
  myTimeStep( 0 ),
  myHueMin( 0.667 ),
  myHueMax( 0 ),
  mySaturationMin( 1 ),
  mySaturationMax( 1 ),
  myValueMin( 1 ),
  myValueMax( 1 ),
  myScaleMode( Linear )
{
  Init();
}

//=======================================================================
//function : Plot3d_ColorDic
//purpose  : Constructor
//=======================================================================
Plot3d_ColorDic::Plot3d_ColorDic( const int num, const double min, const double max )
: myNum( num ),
  myMin( min ),
  myMax( max ),
  myTimeStep( 0 ),
  myHueMin( 0.667 ),
  myHueMax( 0 ),
  mySaturationMin( 1 ),
  mySaturationMax( 1 ),
  myValueMin( 1 ),
  myValueMax( 1 ),
  myScaleMode( Linear )
{
  Init();
}

//=======================================================================
//function : ~Plot3d_ColorDic
//purpose  : Destructor
//=======================================================================
Plot3d_ColorDic::~Plot3d_ColorDic()
{
  myLookupTable->Delete();
}

//=======================================================================
//function : init
//purpose  : Define initial values
//=======================================================================
void Plot3d_ColorDic::Init()
{
  myColorMode = 0;
  myCustomColors[ 0 ] = Qt::blue;
  myCustomColors[ 1 ] = Qt::red;
  myLookupTable = vtkLookupTable::New();
  UpdateLookupTable();
}

//=======================================================================
//function : UpdateLookupTable
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::UpdateLookupTable()
{
  myLookupTable->SetHueRange( myHueMin, myHueMax );
  myLookupTable->SetSaturationRange( mySaturationMin, mySaturationMax );
  myLookupTable->SetValueRange( myValueMin, myValueMax );

  if( myScaleMode != Specific )
    myLookupTable->SetScale( (int)myScaleMode );

  myLookupTable->SetNumberOfTableValues( myNum );
  myLookupTable->SetRange( GetMin(), GetMax() );

  myLookupTable->Build();
}

//=======================================================================
//function : GetNumber
//purpose  : 
//=======================================================================
int Plot3d_ColorDic::GetNumber() const
{
  return myNum;
}

//=======================================================================
//function : SetNumber
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetNumber( const int num )
{
  myNum = num;
  UpdateLookupTable();
}

//=======================================================================
//function : GetMin
//purpose  : 
//=======================================================================
double Plot3d_ColorDic::GetMin() const
{
  return myMin;
}

//=======================================================================
//function : GetMax
//purpose  : 
//=======================================================================
double Plot3d_ColorDic::GetMax() const
{
  return myMax;
}

//=======================================================================
//function : SetMin
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetMin( const double min )
{
  SetRange( min, GetMax() );
}

//=======================================================================
//function : SetMax
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetMax( const double max )
{
  SetRange( GetMin(), max );
}

//=======================================================================
//function : SetRange
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetRange( const double min, const double max )
{
  myMin = Min( min, max );
  myMax = Max( min, max );
  UpdateLookupTable();
}

//=======================================================================
//function : GetHSVRange
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::GetHSVRange( double& hueMin, double& hueMax,
                                       double& satMin, double& satMax,
                                       double& valMin, double& valMax )
{
  hueMin = myHueMin;
  hueMax = myHueMax;
  satMin = mySaturationMin;
  satMax = mySaturationMax;
  valMin = myValueMin;
  valMax = myValueMax;
}

//=======================================================================
//function : SetHSVRange
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetHSVRange( const double hueMin, const double hueMax,
                                       const double satMin, const double satMax,
                                       const double valMin, const double valMax )
{
  // additional check for achromatic colors, which have a hue value of -1
  myHueMin = hueMin < 0 ? hueMax : hueMin;
  myHueMax = hueMax < 0 ? hueMin : hueMax;
  mySaturationMin = satMin;
  mySaturationMax = satMax;
  myValueMin = valMin;
  myValueMax = valMax;
  UpdateLookupTable();
}

//=======================================================================
//function : GetScaleMode
//purpose  : 
//=======================================================================
Plot3d_ColorDic::ScaleMode Plot3d_ColorDic::GetScaleMode() const
{
  return myScaleMode;
}

//=======================================================================
//function : SetScaleMode
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetScaleMode( const Plot3d_ColorDic::ScaleMode type )
{
  myScaleMode = type;
  UpdateLookupTable();
}

//=======================================================================
//function : GetMainSpecificScale
//purpose  : 
//=======================================================================
const Value2ColorList& Plot3d_ColorDic::GetMainSpecificScale() const
{
  return myMainSpecificColorScale;
}

//=======================================================================
//function : SetMainSpecificScale
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetMainSpecificScale( const Value2ColorList& theSpecificColorScale )
{
  myScaleMode = Specific;
  myMainSpecificColorScale = theSpecificColorScale;
}

//=======================================================================
//function : GetSpecificScale
//purpose  : 
//=======================================================================
const Value2ColorList& Plot3d_ColorDic::GetSpecificScale() const
{
  return mySpecificColorScale;
}

//=======================================================================
//function : SetSpecificScale
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetSpecificScale( const Value2ColorList& theSpecificColorScale )
{
  myScaleMode = Specific;
  mySpecificColorScale = theSpecificColorScale;
  if ( !mySpecificColorScale.isEmpty() )
    SetRange( mySpecificColorScale.first().first, mySpecificColorScale.last().first );
  UpdateLookupTable();
}

//=======================================================================
//function : GetTimeStep
//purpose  : 
//=======================================================================
int Plot3d_ColorDic::GetTimeStep() const
{
  return myTimeStep;
}

//=======================================================================
//function : SetTimeStep
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetTimeStep( const int step )
{
  myTimeStep = step;
}

//=======================================================================
//function : GetQuantity
//purpose  : 
//=======================================================================
TCollection_AsciiString Plot3d_ColorDic::GetQuantity() const
{
  return myQuantity;
}

//=======================================================================
//function : SetQuantity
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetQuantity( const TCollection_AsciiString& quant )
{
  myQuantity = quant;
}

//=======================================================================
//function : GetColorByIndex
//purpose  : 
//=======================================================================
bool Plot3d_ColorDic::GetColorByIndex( const int idx, Quantity_Color& c ) const
{
  return GetColorByValue( Value( idx ), c );
}

//=======================================================================
//function : GetColorByValue
//purpose  : 
//=======================================================================
bool Plot3d_ColorDic::GetColorByValue( const double val, Quantity_Color& c ) const
{
  if ( val < GetMin() - Precision::Confusion() ||
       val > GetMax() + Precision::Confusion() )
    return false;

  double rgba[3];
  if ( myScaleMode == Specific )
  {
    if ( !GetSpecificColor( val, rgba ) )
      return false;
  }
  else
    myLookupTable->GetColor( val, rgba );

  c = Quantity_Color( rgba[0], rgba[1], rgba[2], Quantity_TOC_RGB );

  return true;
}

//=======================================================================
//function : GetColorMode
//purpose  : 
//=======================================================================
int Plot3d_ColorDic::GetColorMode() const
{
  return myColorMode;
}

//=======================================================================
//function : SetColorMode
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetColorMode( const int mode )
{
  myColorMode = mode;
}

//=======================================================================
//function : GetCustomColors
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::GetCustomColors( QColor& theBottom, QColor& theTop ) const
{
  theBottom = myCustomColors[ 0 ];
  theTop = myCustomColors[ 1 ];
}

//=======================================================================
//function : SetCustomColors
//purpose  : 
//=======================================================================
void Plot3d_ColorDic::SetCustomColors( const QColor& theBottom, const QColor& theTop )
{
  myCustomColors[ 0 ] = theBottom;
  myCustomColors[ 1 ] = theTop;
}

//=======================================================================
//function : GetSpecificColor
//purpose  : 
//=======================================================================
bool Plot3d_ColorDic::GetSpecificColor( const double val, double rgb[3] ) const
{
  if( mySpecificColorScale.size() < 2 )
    return false;

  Value2ColorListIterator anIter( mySpecificColorScale );
  Value2Color aValue2ColorRef = anIter.next();
  while ( anIter.hasNext() )
  {
    Value2Color aValue2Color = anIter.next();
    if ( val > aValue2ColorRef.first - Precision::Confusion() &&
         val < aValue2Color.first + Precision::Confusion() )
    {
      QColor aColor = aValue2ColorRef.second;
      rgb[0] = aColor.redF();
      rgb[1] = aColor.greenF();
      rgb[2] = aColor.blueF();
      return true;
    }
    aValue2ColorRef = aValue2Color;
  }
  return false;
}

//=======================================================================
//function : Index
//purpose  : 
//=======================================================================
int Plot3d_ColorDic::Index( const double val ) const
{
  if ( !GetNumber() || val < GetMin() || val > GetMax() )
    return -1;

  double step = ( GetMax() - GetMin() ) / GetNumber();
  double range = val - GetMin();

  int index = (int)( range / ( step != 0 ? step : 1 ) );

  index = Min( index, GetNumber() - 1 );

  return index;
}

//=======================================================================
//function : Value
//purpose  : 
//=======================================================================
double Plot3d_ColorDic::Value( const int index ) const
{
  double value = 0.0;
  if ( !GetNumber() || index < 0 || index >= GetNumber() )
    return value;

  if( index == 0 )
    return GetMin();

  if( index == GetNumber() - 1 )
    return GetMax();

  double min = GetMin();
  double max = GetMax();
  if( myScaleMode == Linear )
    value = min + index * ( qAbs( max - min ) / ( GetNumber() - 1 ) );
  else if( myScaleMode == Logarithmic )
  {
    if ( min > 0 && max > 0 )
    {
      double logMin = log10( min );
      double logMax = log10( max );
      double logVal = logMin + index * ( qAbs( logMax - logMin ) / ( GetNumber() - 1 ) );
      value = pow( 10, logVal );
    }
  }
  return value;
}

//=======================================================================
//function : HueFromValue
//purpose  : 
//=======================================================================
int Plot3d_ColorDic::HueFromValue( const int val, const int min, const int max ) const
{
  static const int HUE[10] = {230, 210, 195, 180, 160, 80, 60, 50, 30, 0};

  int hue = HUE[0];

  if ( min != max )
  {
    double aPosition = 9.0 * ( val - min ) / ( max - min );

    if ( aPosition > 0.0 )
    {
      if ( aPosition >= 9.0 )
        hue = HUE[9];
      else
      {
        int idx = (int)aPosition;
        hue = HUE[idx] + int( ( aPosition - idx ) * ( HUE[idx + 1] - HUE[idx] ) );
      }
    }
  }
  return hue;
}
