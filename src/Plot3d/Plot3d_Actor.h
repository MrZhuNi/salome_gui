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
#ifndef PLOT3D_ACTOR_H
#define PLOT3D_ACTOR_H

#include "Plot3d.h"

#include "SALOME_Actor.h"

#include <QColor>
#include <QList>
#include <QPointF>
#include <QString>

class vtkDataSetMapper;
class vtkImplicitBoolean;
class vtkLookupTable;
class vtkScalarBarActor;
class vtkScalarBarWidget;
class vtkWarpScalar;

class SALOME_ExtractGeometry;

class Plot3d_ColorDic;

/*
  Class        : Plot3d_Actor
  Descrtiption : Class for presentation of the Plot3d graph
*/

class PLOT3D_EXPORT Plot3d_Actor : public SALOME_Actor
{
public:
  static Plot3d_Actor* New();
  vtkTypeMacro( Plot3d_Actor, SALOME_Actor );

  Plot3d_Actor();
  virtual ~Plot3d_Actor();

  virtual void                        AddToRender( vtkRenderer* theRenderer ); 
  virtual void                        RemoveFromRender( vtkRenderer* theRenderer );

  virtual void                        SetVisibility( int );

  virtual void                        SetMapper( vtkMapper* theMapper ); 

  void                                SetClippingPlanesEnabled( const bool theState );
  void                                SetClippingPlanes( const double theXMin,
                                                         const double theXMax,
                                                         const double theYMin,
                                                         const double theYMax,
                                                         const double theZMin,
                                                         const double theZMax );

  Plot3d_ColorDic*                    GetColorDic();

  void                                SetIsGlobalColorDic( const bool );
  bool                                IsGlobalColorDic() const;

  void                                SetGlobalColorDic( Plot3d_ColorDic* );

  vtkSmartPointer<vtkScalarBarActor>  GetScalarBarActor() const;
  void                                DisplayScalarBar( const bool );

  void                                Build( const int theNX,
                                             const int theNY,
                                             const QList<QPointF>& thePntList,
                                             const QList<double>& theValueList,
                                             const double theMinValue,
                                             const double theMaxValue,
                                             const int theValueScaleFactorDegree );

  int                                 GetNX() const { return myNX; }
  int                                 GetNY() const { return myNY; }
  const QList<QPointF>&               GetPntList() const { return myPntList; }
  const QList<double>&                GetValueList() const { return myValueList; }
  double                              GetMinValue() const { return myMinValue; }
  double                              GetMaxValue() const { return myMaxValue; }
  int                                 GetValueScaleFactorDegree() const { return myValueScaleFactorDegree; }

  void                                RecomputeLookupTable();

  void                                SetObjectName( const QString& );
  const QString&                      GetObjectName() const;

  void                                SetQuantityName( const QString& );
  const QString&                      GetQuantityName() const;

  void                                SetUnits( const QString& );
  const QString&                      GetUnits() const;

  void                                SetIsDistance( const bool );
  bool                                GetIsDistance() const;

  void                                SetBoundaryPoints( const int, const int );
  void                                GetBoundaryPoints( int&, int& ) const;

  void                                SetTextColor( const QColor& theColor );

  void                                GetRealBounds( double theBounds[6] ) const;

protected:
  int                                 myNX;
  int                                 myNY;
  QList<QPointF>                      myPntList;
  QList<double>                       myValueList;
  double                              myMinValue;
  double                              myMaxValue;
  int                                 myValueScaleFactorDegree;

  vtkSmartPointer<vtkWarpScalar>      myWarpScalar;

  vtkSmartPointer<vtkImplicitBoolean> myImplicitBoolean;
  vtkSmartPointer<SALOME_ExtractGeometry> myExtractGeometry;

  vtkSmartPointer<vtkDataSetMapper>   myMapper;

  Plot3d_ColorDic*                    myColorDic;

  bool                                myIsGlobalColorDic;
  Plot3d_ColorDic*                    myGlobalColorDic;

  vtkSmartPointer<vtkLookupTable>     myLookupTable;

  vtkSmartPointer<vtkScalarBarActor>  myScalarBarActor;
  vtkSmartPointer<vtkScalarBarWidget> myScalarBarWg;
  bool                                myToDisplayScalarBar;

  QString                             myObjectName;
  QString                             myQuantityName;
  QString                             myUnits;
  bool                                myIsDistance;

  int                                 myStartPoint;
  int                                 myEndPoint;

  double                              myRealBounds[6];
};

#endif
