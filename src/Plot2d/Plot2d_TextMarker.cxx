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
#include "Plot2d_TextMarker.h"

//=============================================================================
// Function : Plot2d_TextMarker
// Purpose  : Constructor
//=============================================================================
Plot2d_TextMarker::Plot2d_TextMarker() : QwtPlotMarker()
{
}

//=============================================================================
// Function : Plot2d_TextMarker
// Purpose  : Destructor
//=============================================================================
Plot2d_TextMarker::~Plot2d_TextMarker()
{
}

//=============================================================================
// Function : setData
// Purpose  : 
//=============================================================================
void Plot2d_TextMarker::setData( const QString& theText,
                                 const double theX,
                                 const double theY,
                                 const Qt::Alignment theAlignment )
{
  QwtText aLabel = label();
  aLabel.setText( theText );
  setLabel( aLabel );

  QwtSymbol aSymbol = symbol();
  if( theAlignment == Qt::AlignCenter )
    aSymbol.setStyle( QwtSymbol::NoSymbol );
  else
  {
    aSymbol.setStyle( QwtSymbol::Ellipse );
    aSymbol.setSize( QSize( 3, 3 ) );
  }
  setSymbol( aSymbol );

  setLabelAlignment( theAlignment );

  setValue( theX, theY );
}

//=============================================================================
// Function : getData
// Purpose  : 
//=============================================================================
void Plot2d_TextMarker::getData( QString& theText,
                                 double& theX,
                                 double& theY,
                                 Qt::Alignment& theAlignment ) const
{
  theText = label().text();
  theX = xValue();
  theY = yValue();
  theAlignment = labelAlignment();
}
