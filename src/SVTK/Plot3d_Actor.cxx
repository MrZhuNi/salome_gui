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

#include <vtkFloatArray.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkTextProperty.h>
#include <vtkWarpScalar.h>

vtkStandardNewMacro(Plot3d_Actor);

//=============================================================================
// Function : Plot3d_Actor
// Purpose  : Constructor
//=============================================================================
Plot3d_Actor::Plot3d_Actor()
{
  myIsDistance = false;

  myStartPoint = 0;
  myEndPoint = 0;

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
}

//=============================================================================
// Function : ~Plot3d_Actor
// Purpose  : Destructor
//=============================================================================
Plot3d_Actor::~Plot3d_Actor()
{
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
                          const double theMaxValue )
{
  vtkPolyData* aPointSet = vtkPolyData::New();
  aPointSet->Allocate( ( theNX - 1 ) * ( theNY - 1 ) );

  vtkPoints* aPoints = vtkPoints::New();
  QListIterator<QPointF> aPntIter( thePntList );
  while( aPntIter.hasNext() )
  {
    const QPointF& aPnt = aPntIter.next();
    aPoints->InsertNextPoint( aPnt.x(), aPnt.y(), 0 );
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

  vtkWarpScalar* aWarpScalar = vtkWarpScalar::New();
  aWarpScalar->SetInput( aPointSet );
  aWarpScalar->SetScaleFactor( 1 );

  vtkPolyDataMapper* aMapper = vtkPolyDataMapper::New();
  aMapper->SetInput( aWarpScalar->GetPolyDataOutput() );
  aMapper->SetScalarRange( theMinValue, theMaxValue );

  SetMapper( aMapper );

  vtkLookupTable* aLookupTable = vtkLookupTable::New();
  aLookupTable->SetHueRange( 0.66667, 0.0 );

  aMapper->SetLookupTable( aLookupTable );
  myScalarBarActor->SetLookupTable( aLookupTable );

  aPoints->Delete();
  aFloatArray->Delete();
  aPointSet->Delete();
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
QString Plot3d_Actor::GetUnits() const
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
                                      int& theEndPoint )
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
