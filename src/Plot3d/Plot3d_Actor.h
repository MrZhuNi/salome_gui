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

class vtkLookupTable;
class vtkScalarBarActor;
class vtkScalarBarWidget;

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

  virtual void                        AddToRender( vtkRenderer* theRender ); 
  virtual void                        RemoveFromRender(vtkRenderer* theRendere);

  virtual void                        SetVisibility( int );

  virtual void                        SetMapper( vtkMapper* theMapper ); 

  Plot3d_ColorDic*                    GetColorDic();

  vtkSmartPointer<vtkScalarBarActor>  GetScalarBarActor() const;
  void                                DisplayScalarBar( const bool );

  void                                Build( const int theNX,
                                             const int theNY,
                                             const QList<QPointF>& thePntList,
                                             const QList<double>& theValueList,
                                             const double theMinValue,
                                             const double theMaxValue );

  void                                RecomputeLookupTable();

  void                                SetUnits( const QString& );
  QString                             GetUnits() const;

  void                                SetIsDistance( const bool );
  bool                                GetIsDistance() const;

  void                                SetBoundaryPoints( const int, const int );
  void                                GetBoundaryPoints( int&, int& );

  void                                SetTextColor( const QColor& theColor );

protected:
  Plot3d_ColorDic*                    myColorDic;

  vtkLookupTable*                     myLookupTable;

  vtkSmartPointer<vtkScalarBarActor>  myScalarBarActor;
  vtkSmartPointer<vtkScalarBarWidget> myScalarBarWg;
  bool                                myToDisplayScalarBar;

  QString                             myUnits;
  bool                                myIsDistance;

  int                                 myStartPoint;
  int                                 myEndPoint;
};

#endif
