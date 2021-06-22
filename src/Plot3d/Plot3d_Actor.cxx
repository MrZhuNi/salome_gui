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
#include "Plot3d_Actor.h"

#include "Plot3d_ColorDic.h"

#include "SALOME_ExtractGeometry.h"

#include <vtkDataSetMapper.h>
#include <vtkFloatArray.h>
#include <vtkImplicitBoolean.h>
#include <vtkImplicitFunctionCollection.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkTextProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkWarpScalar.h>

vtkStandardNewMacro(Plot3d_Actor);

//=============================================================================
// Function : Plot3d_Actor
// Purpose  : Constructor
//=============================================================================
Plot3d_Actor::Plot3d_Actor()
{
  // Input data
  myNX = 0;
  myNY = 0;
  myMinValue = 0;
  myMaxValue = 0;
  myValueScaleFactorDegree = 0; // degree of the value scale factor (value = value_base * 10^degree)

  // Pipeline
  myWarpScalar = vtkWarpScalar::New();
  myWarpScalar->Delete();
  myWarpScalar->SetScaleFactor( 1 );

  myImplicitBoolean = vtkImplicitBoolean::New();
  myImplicitBoolean->Delete();
  myImplicitBoolean->SetOperationTypeToIntersection();

  for( int anIndex = 1; anIndex <= 6; anIndex++ )
  {
    vtkSmartPointer<vtkPlane> aPlane = vtkPlane::New();
    aPlane->Delete();
    myImplicitBoolean->GetFunction()->AddItem( aPlane );
  }

  myExtractGeometry = SALOME_ExtractGeometry::New();
  myExtractGeometry->Delete();
  myExtractGeometry->SetImplicitFunction( myImplicitBoolean );
  myExtractGeometry->SetExtractInside( 1 );

  myMapper = vtkDataSetMapper::New();
  myMapper->Delete();

  // Other
  myColorDic = new Plot3d_ColorDic();

  myIsGlobalColorDic = false;
  myGlobalColorDic = 0;

  myIsDistance = false;

  myStartPoint = 0;
  myEndPoint = 0;

  myTimeUnit = -1;

  // Scalar bar
  myToDisplayScalarBar = false;

  myScalarBarActor = vtkScalarBarActor::New();
  myScalarBarActor->SetVisibility( false );
  
  // Title props
  QColor aTextColor = Qt::white;

  vtkTextProperty* aScalarBarTitleProp = vtkTextProperty::New();
  aScalarBarTitleProp->SetColor( aTextColor.redF(), aTextColor.greenF(), aTextColor.blueF() );
  aScalarBarTitleProp->SetFontFamilyToArial();
  int aSize = 24;
  aScalarBarTitleProp->SetFontSize( aSize );
  myScalarBarActor->SetTitleTextProperty( aScalarBarTitleProp );
  aScalarBarTitleProp->Delete();

  // Label props
  vtkTextProperty* aScalarBarLabelProp = vtkTextProperty::New();
  aScalarBarLabelProp->SetColor( aTextColor.redF(), aTextColor.greenF(), aTextColor.blueF() );
  aScalarBarLabelProp->SetFontFamilyToArial();
  myScalarBarActor->SetLabelTextProperty( aScalarBarLabelProp );
  aScalarBarLabelProp->Delete();

  // Position
  double aXPos = 0.01, aYPos = 0.1;
  myScalarBarActor->SetPosition( aXPos, aYPos );

  // Width
  double aWidth = 0.10, aHeight = 0.80;
  myScalarBarActor->SetWidth( aWidth );
  myScalarBarActor->SetHeight( aHeight );

  // Number of labels and Maximum number of colors
  myScalarBarActor->SetNumberOfLabels( 5 );
  myScalarBarActor->SetMaximumNumberOfColors( 99 );  

  // ScalarBar widget
  myScalarBarWg = vtkScalarBarWidget::New();
  myScalarBarWg->SetScalarBarActor( myScalarBarActor.GetPointer() );

  // Property
  this->GetProperty()->ShadingOff();
  this->GetProperty()->LightingOff();

  // Lookup table
  myLookupTable = vtkLookupTable::New();
  myLookupTable->Delete();
  myLookupTable->SetHueRange( 0.667, 0.0 );

  // Real (non-clipped) bounds
  myRealBounds[0] = VTK_DOUBLE_MAX;
  myRealBounds[1] = VTK_DOUBLE_MIN;
  myRealBounds[2] = VTK_DOUBLE_MAX;
  myRealBounds[3] = VTK_DOUBLE_MIN;
  myRealBounds[4] = VTK_DOUBLE_MAX;
  myRealBounds[5] = VTK_DOUBLE_MIN;
}

//=============================================================================
// Function : ~Plot3d_Actor
// Purpose  : Destructor
//=============================================================================
Plot3d_Actor::~Plot3d_Actor()
{
  if( myColorDic )
  {
    delete myColorDic;
    myColorDic = 0;
  }
}

//=============================================================================
// Function : SetVisibility
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetVisibility( int theVisibility )
{
  Superclass::SetVisibility( theVisibility );

  myScalarBarActor->SetVisibility( GetVisibility() && myToDisplayScalarBar );

  if ( !theVisibility || !myScalarBarActor->GetLookupTable() || !myToDisplayScalarBar )
    myScalarBarWg->EnabledOff();
  else
    myScalarBarWg->EnabledOn();
}

//=============================================================================
// Function : SetMapper
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetMapper( vtkMapper* theMapper )
{
  Superclass::SetMapper(theMapper);
}

//=============================================================================
// Function : AddToRender
// Purpose  : 
//=============================================================================
void Plot3d_Actor::AddToRender( vtkRenderer* theRenderer )
{
  Superclass::AddToRender( theRenderer );

  if ( vtkRenderWindow *win = theRenderer->GetRenderWindow() )
  {
    if ( vtkRenderWindowInteractor *interactor = win->GetInteractor() ) 
    {
      myScalarBarWg->SetInteractor( interactor );
      if ( myToDisplayScalarBar )
        myScalarBarWg->EnabledOn();
    }
  }

  theRenderer->AddActor( myScalarBarActor.GetPointer() );
}

//=============================================================================
// Function : RemoveFromRender
// Purpose  : 
//=============================================================================
void Plot3d_Actor::RemoveFromRender( vtkRenderer* theRenderer )
{
  myScalarBarWg->EnabledOff();
  theRenderer->RemoveActor( myScalarBarActor.GetPointer() );

  Superclass::RemoveFromRender( theRenderer );
}

//=============================================================================
// Function : SetClippingPlanesEnabled
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetClippingPlanesEnabled( const bool theState )
{
  if( theState )
  {
    myWarpScalar->Update();
    myExtractGeometry->SetInput( myWarpScalar->GetPolyDataOutput() );
    myExtractGeometry->Update();
    myMapper->SetInput( myExtractGeometry->GetOutput() );
  }
  else
    myMapper->SetInput( myWarpScalar->GetPolyDataOutput() );
  SetMapper( myMapper );
  myMapper->Update();
}

//=============================================================================
// Function : SetClippingPlanes
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetClippingPlanes( const double theXMin,
                                      const double theXMax,
                                      const double theYMin,
                                      const double theYMax,
                                      const double theZMin,
                                      const double theZMax )
{
  if( vtkPlane* aPlaneXMin = vtkPlane::SafeDownCast( myImplicitBoolean->GetFunction()->GetItemAsObject( 0 ) ) )
  {
    aPlaneXMin->SetNormal( -1, 0, 0 );
    aPlaneXMin->SetOrigin( theXMin, 0, 0 );
  }
  if( vtkPlane* aPlaneXMax = vtkPlane::SafeDownCast( myImplicitBoolean->GetFunction()->GetItemAsObject( 1 ) ) )
  {
    aPlaneXMax->SetNormal( 1, 0, 0 );
    aPlaneXMax->SetOrigin( theXMax, 0, 0 );
  }
  if( vtkPlane* aPlaneYMin = vtkPlane::SafeDownCast( myImplicitBoolean->GetFunction()->GetItemAsObject( 2 ) ) )
  {
    aPlaneYMin->SetNormal( 0, -1, 0 );
    aPlaneYMin->SetOrigin( 0, theYMin, 0 );
  }
  if( vtkPlane* aPlaneYMax = vtkPlane::SafeDownCast( myImplicitBoolean->GetFunction()->GetItemAsObject( 3 ) ) )
  {
    aPlaneYMax->SetNormal( 0, 1, 0 );
    aPlaneYMax->SetOrigin( 0, theYMax, 0 );
  }
  if( vtkPlane* aPlaneZMin = vtkPlane::SafeDownCast( myImplicitBoolean->GetFunction()->GetItemAsObject( 4 ) ) )
  {
    aPlaneZMin->SetNormal( 0, 0, -1 );
    aPlaneZMin->SetOrigin( 0, 0, theZMin );
  }
  if( vtkPlane* aPlaneZMax = vtkPlane::SafeDownCast( myImplicitBoolean->GetFunction()->GetItemAsObject( 5 ) ) )
  {
    aPlaneZMax->SetNormal( 0, 0, 1 );
    aPlaneZMax->SetOrigin( 0, 0, theZMax );
  }
}

//=============================================================================
// Function : GetColorDic
// Purpose  : 
//=============================================================================
Plot3d_ColorDic* Plot3d_Actor::GetColorDic()
{
  return myColorDic;
}

//=======================================================================
//function : SetIsGlobalColorDic
//purpose  : 
//=======================================================================
void Plot3d_Actor::SetIsGlobalColorDic( const bool theIsGlobalColorDic )
{
  myIsGlobalColorDic = theIsGlobalColorDic;
}

//=======================================================================
//function : IsGlobalColorDic
//purpose  : 
//=======================================================================
bool Plot3d_Actor::IsGlobalColorDic() const
{
  return myIsGlobalColorDic;
}

//=============================================================================
// Function : SetGlobalColorDic
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetGlobalColorDic( Plot3d_ColorDic* theColorDic )
{
  myGlobalColorDic = theColorDic;
}

//=============================================================================
// Function : GetScalarBarActor
// Purpose  : 
//=============================================================================
vtkSmartPointer<vtkScalarBarActor> Plot3d_Actor::GetScalarBarActor() const 
{ 
  return myScalarBarActor;
}

//=============================================================================
// Function : DisplayScalarBar
// Purpose  : 
//=============================================================================
void Plot3d_Actor::DisplayScalarBar( const bool theToDisplay )
{
  myToDisplayScalarBar = theToDisplay;
  myScalarBarActor->SetVisibility( GetVisibility() && theToDisplay );
  myScalarBarWg->SetEnabled( GetVisibility() && theToDisplay ? 1 : 0 );
}

//=============================================================================
// Function : Build
// Purpose  : 
//=============================================================================
void Plot3d_Actor::Build( const int theNX,
                          const int theNY,
                          const QList<QPointF>& thePntList,
                          const QList<double>& theValueList,
                          const double theMinValue,
                          const double theMaxValue,
                          const int theValueScaleFactorDegree )
{
  myNX = theNX;
  myNY = theNY;
  myPntList = thePntList;
  myValueList = theValueList;
  myMinValue = theMinValue;
  myMaxValue = theMaxValue;
  myValueScaleFactorDegree = theValueScaleFactorDegree;

  myRealBounds[0] = VTK_DOUBLE_MAX;
  myRealBounds[1] = VTK_DOUBLE_MIN;
  myRealBounds[2] = VTK_DOUBLE_MAX;
  myRealBounds[3] = VTK_DOUBLE_MIN;
  myRealBounds[4] = theMinValue;
  myRealBounds[5] = theMaxValue;

  vtkPolyData* aPointSet = vtkPolyData::New();
  aPointSet->Allocate( ( theNX - 1 ) * ( theNY - 1 ) );

  vtkPoints* aPoints = vtkPoints::New();
  QListIterator<QPointF> aPntIter( thePntList );
  while( aPntIter.hasNext() )
  {
    const QPointF& aPnt = aPntIter.next();
    double x = aPnt.x();
    double y = aPnt.y();
    aPoints->InsertNextPoint( x, y, 0 );

    myRealBounds[0] = qMin( myRealBounds[0], x );
    myRealBounds[1] = qMax( myRealBounds[1], x );
    myRealBounds[2] = qMin( myRealBounds[2], y );
    myRealBounds[3] = qMax( myRealBounds[3], y );
  }
  aPointSet->SetPoints( aPoints );

  vtkIdType pts[ 4 ];
  for( int i = 0; i < theNX - 1; i++ )
  {
    for( int j = 0; j < theNY - 1; j++ )
    {
      pts[0] = j + theNY * i;
      pts[1] = j + theNY * i + 1;
      pts[2] = j + theNY * ( i + 1 ) + 1;
      pts[3] = j + theNY * ( i + 1 );
      aPointSet->InsertNextCell( VTK_QUAD, 4, pts );
    }
  }

  vtkFloatArray* aFloatArray = vtkFloatArray::New();
  QListIterator<double> aValueIter( theValueList );
  while( aValueIter.hasNext() )
  {
    const double aValue = aValueIter.next();
    aFloatArray->InsertNextTuple1( aValue );
  }

  vtkPointData* aPointData = aPointSet->GetPointData();
  aPointData->SetScalars( aFloatArray );

  myWarpScalar->SetInput( aPointSet );

  if( theValueScaleFactorDegree != 0 )
  {
    double aScaleFactor = pow( 0.1, (double)theValueScaleFactorDegree );
    myWarpScalar->SetScaleFactor( aScaleFactor );
  }

  myMapper->SetInput( myWarpScalar->GetPolyDataOutput() );
  myMapper->SetScalarRange( theMinValue, theMaxValue );

  SetMapper( myMapper );

  aPoints->Delete();
  aFloatArray->Delete();
  aPointSet->Delete();

  myColorDic->SetRange( theMinValue, theMaxValue );

  RecomputeLookupTable();
}

//=============================================================================
// Function : RecomputeLookupTable
// Purpose  : 
//=============================================================================
void Plot3d_Actor::RecomputeLookupTable()
{
  Plot3d_ColorDic* aColorDic = IsGlobalColorDic() && myGlobalColorDic ? myGlobalColorDic : myColorDic;
  if( !aColorDic )
    return;

  vtkDataSetMapper* aMapper = dynamic_cast<vtkDataSetMapper*>( GetMapper() );
  if( !aMapper )
    return;

  double myMinimum = aColorDic->GetMin();
  double myMaximum = aColorDic->GetMax();

  int aScaleMode = (int)aColorDic->GetScaleMode();

  int nbColors = aColorDic->GetNumber();
  if ( aScaleMode == Plot3d_ColorDic::Specific )
  {
    nbColors = 1000;
    if( nbColors == 0 )
      return;
  }

  if( myMinimum > myMaximum ||
      myMinimum < RealFirst() / 2. ||
      myMaximum > RealLast() / 2. )
    return; // protection from uninitialized bounds (empty results)

  double aMin = myMinimum;
  double aMax = myMaximum;
  double range[2] = { aMin, aMax };

  double aHueMin, aHueMax, aSaturationMin, aSaturationMax, aValueMin, aValueMax;
  aColorDic->GetHSVRange( aHueMin, aHueMax, aSaturationMin, aSaturationMax, aValueMin, aValueMax );

  double range1[2] = { 0, 0 };
  if ( aScaleMode == Plot3d_ColorDic::Linear ||
       aScaleMode == Plot3d_ColorDic::Specific )
  {
    double delta = ( aMax - aMin ) / nbColors;
    range1[0] = aMin - 2 * delta;
    range1[1] = aMax + 2 * delta;
  }
  else if ( aScaleMode == Plot3d_ColorDic::Logarithmic )
  {
    if( aMin > 0 && aMax > 0 )
    {
      double aLogMin = log10( aMin );
      double aLogMax = log10( aMax );
      double aLogDelta = ( aLogMax - aLogMin ) / nbColors;
      range1[0] = pow( 10, aLogMin - 2 * aLogDelta );
      range1[1] = pow( 10, aLogMax + 2 * aLogDelta );
    }
  }

  vtkLookupTable* aTable = dynamic_cast<vtkLookupTable*>( aMapper->GetLookupTable() );

  if ( range[0] == range[1] )
  {
    aTable->SetRange( range );
    aTable->SetNumberOfTableValues( 1 );
    aTable->SetTableValue( 0, 0, 0, 1 );

    myLookupTable->SetRange( range );
    myLookupTable->SetNumberOfTableValues( 1 );
    myLookupTable->SetTableValue( 0, 0, 0, 1 );
  }
  else
  {
    if ( aScaleMode == Plot3d_ColorDic::Specific )
    {
      const Value2ColorList& aSpecificScale = aColorDic->GetSpecificScale();
      if ( aSpecificScale.size() < 2 )
        return;

      myLookupTable->SetRange( range );
      myLookupTable->SetNumberOfTableValues( nbColors );
      myLookupTable->SetScaleToLinear();

      Value2ColorListIterator anIter( aSpecificScale );
      Value2Color aValue2ColorRef = anIter.next();
      Value2Color aValue2Color = anIter.next();

      QColor aColor;
      double rgba[4];

      double valMin = range[0];
      double valMax = range[1];
      double valRange = valMax - valMin;
      double valStep = valRange / (double)nbColors;
      for ( int anIndex = 0; anIndex < nbColors; anIndex++ )
      {
        double val = valMin + valStep * (double)anIndex;
        while ( val > aValue2Color.first && anIter.hasNext() )
        {
          aValue2ColorRef = aValue2Color;
          aValue2Color = anIter.next();
        }

        aColor = aValue2ColorRef.second;
        rgba[0] = aColor.redF();
        rgba[1] = aColor.greenF();
        rgba[2] = aColor.blueF();
        rgba[3] = 1.0;

        myLookupTable->SetTableValue( anIndex, rgba );
      }
    }
    else
    {
      myLookupTable->SetRange( range );
      myLookupTable->SetNumberOfTableValues( nbColors );
      myLookupTable->SetHueRange( aHueMin, aHueMax );
      myLookupTable->SetSaturationRange( aSaturationMin, aSaturationMax );
      myLookupTable->SetValueRange( aValueMin, aValueMax );
      myLookupTable->SetScale( aScaleMode );
      myLookupTable->ForceBuild();
    }

    // TODO: number of colors should be computed (recieved from the filter)
    aTable->SetNumberOfTableValues( nbColors + 4 );
    aTable->SetScale( myLookupTable->GetScale() );
    aTable->SetRange( range1 );
    aTable->Build();
    double rgba[4];

    aTable->SetTableValue( 0, 0, 0, 0, 1 );
    myLookupTable->GetTableValue(0, rgba);
    aTable->SetTableValue( 1, rgba );
    for (int i = 0; i < nbColors; i++) {
      myLookupTable->GetTableValue(i, rgba);
      aTable->SetTableValue(i+2, rgba);
    }
    aTable->SetTableValue( nbColors + 2, rgba );
    aTable->SetTableValue( nbColors + 3, 0, 0, 0, 1 );
  }

  Modified();

  aMapper->SetScalarRange( range1 );
  aMapper->SetLookupTable( aTable );

  myScalarBarActor->SetLookupTable( myLookupTable );
}

//=============================================================================
// Function : SetObjectName
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetObjectName( const QString& theObjectName )
{
  myObjectName = theObjectName;
}

//=============================================================================
// Function : GetObjectName
// Purpose  : 
//=============================================================================
const QString& Plot3d_Actor::GetObjectName() const
{
  return myObjectName;
}

//=============================================================================
// Function : SetQuantityName
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetQuantityName( const QString& theQuantityName )
{
  myQuantityName = theQuantityName;
}

//=============================================================================
// Function : GetQuantityName
// Purpose  : 
//=============================================================================
const QString& Plot3d_Actor::GetQuantityName() const
{
  return myQuantityName;
}

//=============================================================================
// Function : SetUnits
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetUnits( const QString& theUnits )
{
  myUnits = theUnits;
}

//=============================================================================
// Function : GetUnits
// Purpose  : 
//=============================================================================
const QString& Plot3d_Actor::GetUnits() const
{
  return myUnits;
}

//=============================================================================
// Function : SetIsDistance
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetIsDistance( const bool theIsDistance )
{
  myIsDistance = theIsDistance;
}

//=============================================================================
// Function : GetIsDistance
// Purpose  : 
//=============================================================================
bool Plot3d_Actor::GetIsDistance() const
{
  return myIsDistance;
}

//=============================================================================
// Function : SetBoundaryPoints
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetBoundaryPoints( const int theStartPoint,
                                      const int theEndPoint )
{
  myStartPoint = theStartPoint;
  myEndPoint = theEndPoint;
}

//=============================================================================
// Function : GetBoundaryPoints
// Purpose  : 
//=============================================================================
void Plot3d_Actor::GetBoundaryPoints( int& theStartPoint,
                                      int& theEndPoint ) const
{
  theStartPoint = myStartPoint;
  theEndPoint = myEndPoint;
}

//=============================================================================
// Function : SetTextColor
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetTextColor( const QColor& theColor )
{
  vtkTextProperty* aScalarBarTitleProp = myScalarBarActor->GetTitleTextProperty();
  aScalarBarTitleProp->SetColor( theColor.redF(), theColor.greenF(), theColor.blueF() );

  vtkTextProperty* aScalarBarLabelProp = myScalarBarActor->GetLabelTextProperty();
  aScalarBarLabelProp->SetColor( theColor.redF(), theColor.greenF(), theColor.blueF() );
}

//=============================================================================
// Function : GetRealBounds
// Purpose  : 
//=============================================================================
void Plot3d_Actor::GetRealBounds( double theBounds[6] ) const
{
  theBounds[0] = myRealBounds[0];
  theBounds[1] = myRealBounds[1];
  theBounds[2] = myRealBounds[2];
  theBounds[3] = myRealBounds[3];
  theBounds[4] = myRealBounds[4];
  theBounds[5] = myRealBounds[5];
}

//=============================================================================
// Function : SetTimeUnit
// Purpose  : 
//=============================================================================
void Plot3d_Actor::SetTimeUnit( const int theTimeUnit )
{
  myTimeUnit = theTimeUnit;
}

//=============================================================================
// Function : GetTimeUnit
// Purpose  : 
//=============================================================================
int Plot3d_Actor::GetTimeUnit() const
{
  return myTimeUnit;
}
