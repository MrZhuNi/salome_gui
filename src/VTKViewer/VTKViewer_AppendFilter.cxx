//  SALOME OBJECT : kernel of SALOME component
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : VTKViewer_GeometryFilter.cxx
//  Author : 
//  Module : SALOME
//  $Header$

#include "VTKViewer_AppendFilter.h"

#include "VTKViewer_ConvexTool.h"

#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkGenericCell.h>
#include <vtkHexahedron.h>
#include <vtkMergePoints.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPyramid.h>
#include <vtkStructuredGrid.h>
#include <vtkTetra.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVoxel.h>
#include <vtkWedge.h>
#include <vtkDataSetCollection.h>

#include <vector>
#include <map>
using namespace std;


#ifdef _DEBUG_
//static int MYDEBUG = 0;
//static int MYDEBUGWITHFILES = 0;
#else
//static int MYDEBUG = 0;
//static int MYDEBUGWITHFILES = 0;
#endif

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

vtkCxxRevisionMacro(VTKViewer_AppendFilter, "$Revision$");
vtkStandardNewMacro(VTKViewer_AppendFilter);

VTKViewer_AppendFilter::VTKViewer_AppendFilter() 
{
  myDoMappingFlag=false;//modified by NIZNHY-PKV Tue Aug 30 10:09:50 2005ft
}

VTKViewer_AppendFilter::~VTKViewer_AppendFilter()
{}

void VTKViewer_AppendFilter::SetDoMappingFlag(const bool theFlag)
{
  myDoMappingFlag=theFlag;
}

bool VTKViewer_AppendFilter::DoMappingFlag() const
{
  return myDoMappingFlag;
}

void VTKViewer_AppendFilter::Execute()
{
  vtkAppendFilter::Execute();
  if (myDoMappingFlag){
    DoMapping();
  }
}

void VTKViewer_AppendFilter::Reset()
{
  myNodeIds.clear();
  myCellIds.clear();
  myNodeRanges.clear();
  myCellRanges.clear();
  //modified by NIZNHY-PKV Tue Aug 30 10:09:35 2005f
  myNodeMapObjIDVtkID.clear();
  myCellMapObjIDVtkID.clear();
  //modified by NIZNHY-PKV Tue Aug 30 10:09:39 2005t
}

void VTKViewer_AppendFilter::DoMapping()
{
  int i, j, i1, i2, iNodeCnt, iCellCnt; 
  IteratorOfDataMapOfIntegerInteger aMapIt;
  vtkIdType aNbPnts, aNbCells, aId;
  vtkDataSet *pDS;
  //
  Reset();
  //
  iNodeCnt=0;
  iCellCnt=0;
  for (i=0; i<NumberOfInputs; ++i) {
    pDS=(vtkDataSet *)Inputs[i];
    //
    // Nodes
    aNbPnts=pDS->GetNumberOfPoints();
    i1=myNodeIds.size();
    i2=i1+aNbPnts-1;
    myNodeRanges.push_back(i1);
    myNodeRanges.push_back(i2);
    //
    for(j=0; j<aNbPnts; ++j) {
      aId=(vtkIdType)j;
      myNodeIds.push_back(aId);
      //
      aMapIt=myNodeMapObjIDVtkID.find(aId);
      if (aMapIt==myNodeMapObjIDVtkID.end()) {
	// if not found
	myNodeMapObjIDVtkID[aId]=iNodeCnt;
      }
      ++iNodeCnt;
    }
    //
    // Cells
    aNbCells=pDS->GetNumberOfCells();
    i1=myCellIds.size();
    i2=i1+aNbCells-1;
    myCellRanges.push_back(i1);
    myCellRanges.push_back(i2);
    for(j=0; j<aNbCells; ++j) {
      aId=(vtkIdType)j;
      myCellIds.push_back(aId);
      //
      aMapIt=myCellMapObjIDVtkID.find(aId);
      if (aMapIt==myCellMapObjIDVtkID.end()) {
	// if not found
	myCellMapObjIDVtkID[aId]=iCellCnt;
      }
      ++iCellCnt;
    }
  }
}
//modified by NIZNHY-PKV Tue Aug 30 10:20:50 2005f
int VTKViewer_AppendFilter::GetElemVtkID(int theObjID)
{
  int aVtkID=-1;
  IteratorOfDataMapOfIntegerInteger aMapIt;
  //
  aMapIt=myCellMapObjIDVtkID.find(theObjID);
  if (aMapIt!=myCellMapObjIDVtkID.end()) {
    // found
    PairOfDataMapOfIntegerInteger& aPair=(*aMapIt);
    aVtkID=aPair.second;
  }
  return aVtkID;
}

int VTKViewer_AppendFilter::GetNodeVtkID(int theObjID)
{
  int aVtkID=-1;
  IteratorOfDataMapOfIntegerInteger aMapIt;
  //
  aMapIt=myNodeMapObjIDVtkID.find(theObjID);
  if (aMapIt!=myNodeMapObjIDVtkID.end()) {
    // found
    PairOfDataMapOfIntegerInteger& aPair=(*aMapIt);
    aVtkID=aPair.second;
  }
  return aVtkID;
}
//modified by NIZNHY-PKV Tue Aug 30 10:20:54 2005t

int VTKViewer_AppendFilter::GetNodeObjId(int theVtkID, 
					 int& theInputIndex)
{
  int aNb, aNbRanges, aRetID, i, i1, i2, j;
  //
  aRetID=-1;
  theInputIndex=-1;
  //
  aNb=myNodeIds.size();
  if (theVtkID<0 ||  theVtkID>=aNb) {
    return aRetID;
  }
  //
  aRetID=(int)myNodeIds[theVtkID];
  //
  aNbRanges=myNodeRanges.size()/2;
  for (i=0; i<aNbRanges; ++i) {
    j=2*i;
    i1=myNodeRanges[j];
    i2=myNodeRanges[j+1];
    if (theVtkID>=i1 && theVtkID<=i2) {
      theInputIndex=i;
    }
  }
  //
  return aRetID;
}

int VTKViewer_AppendFilter::GetElemObjId(int theVtkID,
					 int& theInputIndex)
{
  int aNb, aNbRanges, aRetID, i, i1, i2, j;
  //
  aRetID=-1;
  theInputIndex=-1;
  //
  aNb=myCellIds.size();
  if (theVtkID<0 ||  theVtkID>=aNb) {
    return aRetID;
  }
  //
  aRetID=(int)myCellIds[theVtkID];
  //
  aNbRanges=myCellRanges.size()/2;
  for (i=0; i<aNbRanges; ++i) {
    j=2*i;
    i1=myCellRanges[j];
    i2=myCellRanges[j+1];
    if (theVtkID>=i1 && theVtkID<=i2) {
      theInputIndex=i;
    }
  }
  //
  return aRetID;
}


