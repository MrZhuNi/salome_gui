//  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SPlot2d_Histogram.h
//  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)

#ifndef SPlot2d_Histogram_h
#define SPlot2d_Histogram_h

#include "SPlot2d.h"  

#include "Plot2d_Histogram.h"

#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif
#include "SALOME_InteractiveObject.hxx"

class SPLOT2D_EXPORT SPlot2d_Histogram : public Plot2d_Histogram
{
public:

  SPlot2d_Histogram();
  virtual ~SPlot2d_Histogram();
  SPlot2d_Histogram( const SPlot2d_Histogram& hist );
  SPlot2d_Histogram& operator= ( const SPlot2d_Histogram& hist );

  virtual bool                             hasIO() const;
  virtual Handle(SALOME_InteractiveObject) getIO() const;
  virtual void                             setIO( const Handle(SALOME_InteractiveObject)& );
  
private:
  Handle(SALOME_InteractiveObject) myIO;
};

#endif // SPlot2d_Histogram_h