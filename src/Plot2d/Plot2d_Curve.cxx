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
// File   : Plot2d_Curve.cxx
// Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//

#include "Plot2d_Curve.h"
#include "Plot2d_ViewFrame.h"
#include <QColor>

#include <float.h>

/*!
Constructor
*/
Plot2d_Curve::Plot2d_Curve()
: myHorTitle( "" ), myVerTitle( "" ), 
myHorUnits( "" ), myVerUnits( "" ), 
myAutoAssign( true ), 
myColor( 0,0,0 ), 
myMarker( Plot2d::Circle ), 
myLine( Plot2d::Solid ), 
myLineWidth( 0 ),
myYAxis( QwtPlot::yLeft ),
myNbMarkers( 1 )
{
}

/*!
Destructor
*/
Plot2d_Curve::~Plot2d_Curve()
{
}

/*!
Copy constructor. Makes deep copy of data.
*/
Plot2d_Curve::Plot2d_Curve( const Plot2d_Curve& curve )
{
  myAutoAssign = curve.isAutoAssign();
  myHorTitle   = curve.getHorTitle();
  myVerTitle   = curve.getVerTitle();
  myHorUnits   = curve.getHorUnits();
  myVerUnits   = curve.getVerUnits();
  myColor      = curve.getColor();
  myMarker     = curve.getMarker();
  myLine       = curve.getLine();
  myLineWidth  = curve.getLineWidth();
  myPoints     = curve.getPointList();
  mySymbolsColorMap = curve.getColorMap();
  myNbMarkers  = curve.getNbMarkers();
}

/*!
operator=. Makes deep copy of data.
*/
Plot2d_Curve& Plot2d_Curve::operator=( const Plot2d_Curve& curve )
{
  myAutoAssign = curve.isAutoAssign();
  myHorTitle   = curve.getHorTitle();
  myVerTitle   = curve.getVerTitle();
  myHorUnits   = curve.getHorUnits();
  myVerUnits   = curve.getVerUnits();
  myColor      = curve.getColor();
  myMarker     = curve.getMarker();
  myLine       = curve.getLine();
  myLineWidth  = curve.getLineWidth();
  myPoints     = curve.getPointList();
  mySymbolsColorMap = curve.getColorMap();
  myNbMarkers  = curve.getNbMarkers();
  return *this;
}

/*!
\return title of table
*/
QString Plot2d_Curve::getTableTitle() const
{
  return QString();
}

/*!
Sets curve's horizontal title
*/
void Plot2d_Curve::setHorTitle( const QString& title )
{
  myHorTitle = title;
}

/*!
Gets curve's horizontal title
*/
QString Plot2d_Curve::getHorTitle() const
{
  return myHorTitle;
}

/*!
Sets curve's vertical title
*/
void Plot2d_Curve::setVerTitle( const QString& title )
{
  myVerTitle = title;
}

/*!
Gets curve's vertical title
*/
QString Plot2d_Curve::getVerTitle() const
{
  return myVerTitle;
}

/*!
Sets curve's horizontal units
*/
void Plot2d_Curve::setHorUnits( const QString& units )
{
  myHorUnits = units;
}

/*!
Gets curve's horizontal units
*/
QString Plot2d_Curve::getHorUnits() const
{
  return myHorUnits;
}

/*!
Sets curve's vertical units
*/
void Plot2d_Curve::setVerUnits( const QString& units )
{
  myVerUnits = units;
}

/*!
Gets curve's vertical units
*/
QString Plot2d_Curve::getVerUnits() const
{
  return myVerUnits;
}

/*!
Adds one point for curve.
*/
void Plot2d_Curve::addPoint( double theX, double theY, const QString& theTxt, int theColorId )
{
  Plot2d_Point aPoint;
  aPoint.x = theX;
  aPoint.y = theY;
  aPoint.text = theTxt;
  aPoint.colorId = theColorId;
  myPoints.append(aPoint);
}

/*!
Insert one point for curve on some position.
*/
void Plot2d_Curve::insertPoint( int thePos, double theX, double theY, const QString& theTxt, int theColorId )
{
  Plot2d_Point aPoint;
  aPoint.x = theX;
  aPoint.y = theY;
  aPoint.text = theTxt;
  aPoint.colorId = theColorId;

  pointList::iterator aIt;
  int aCurrent = 0;
  for(aIt = myPoints.begin(); aIt != myPoints.end(); ++aIt) {
    if (thePos == aCurrent) {
      myPoints.insert(aIt, aPoint);
      return;
    }
    aCurrent++;  
  }
  myPoints.append(aPoint);
}

/*!
Delete one point for curve on some position.
*/
void Plot2d_Curve::deletePoint(int thePos)
{
  if ( thePos >= 0 && thePos < myPoints.count() )
    myPoints.removeAt( thePos );
}

/*!
Remove all points for curve.
*/
void Plot2d_Curve::clearAllPoints()
{
  myPoints.clear();
}

/*!
Gets curve's data : abscissas of points
*/
pointList Plot2d_Curve::getPointList() const
{
  return myPoints;
}

/*!
Gets curve's data : abscissas of points
*/
pointList& Plot2d_Curve::getPointList()
{
  return myPoints;
}

/*!
Gets curve's data : symbols color map
*/
colorMap Plot2d_Curve::getColorMap() const
{
  return mySymbolsColorMap;
}

/*!
Gets curve's data : symbols color map
*/
colorMap& Plot2d_Curve::getColorMap()
{
  return mySymbolsColorMap;
}

/*!
Sets curve's data. 
*/
void Plot2d_Curve::setData( const double* hData, const double* vData, 
                           long size, const QStringList& lst, const int* theColorIds )
{
  clearAllPoints();
  QStringList::const_iterator anIt = lst.begin(), aLast = lst.end(); 
  for( long i = 0; i < size; i++, anIt++ )
    addPoint( hData[i], vData[i], anIt>=aLast ? QString() : *anIt, ( theColorIds ? theColorIds[i] : 0 ) );
}

/*!
Gets curve's data : abscissas of points
*/
double* Plot2d_Curve::horData( const double theMultiplier ) const
{
  int aNPoints = nbPoints();
  double* aX = new double[aNPoints];
  for (int i = 0; i < aNPoints; i++) {
    aX[i] = myPoints[i].x * theMultiplier;
  }
  return aX;
}

/*!
Gets curve's data : ordinates of points
*/
double* Plot2d_Curve::verData() const
{
  int aNPoints = nbPoints();
  double* aY = new double[aNPoints];
  for (int i = 0; i < aNPoints; i++) {
    aY[i] = myPoints[i].y;
  }
  return aY;
}

/*!
Gets curve's data : color ids of points
*/
int* Plot2d_Curve::colorData() const
{
  int aNPoints = nbPoints();
  int* aZ = new int[aNPoints];
  for (int i = 0; i < aNPoints; i++) {
    aZ[i] = myPoints[i].colorId;
  }
  return aZ;
}

/*!
Gets curve's data : number of points
*/
int Plot2d_Curve::nbPoints() const
{
  return myPoints.count();
}

/*!
Returns true if curve has no data
*/
bool Plot2d_Curve::isEmpty() const
{
  return myPoints.isEmpty();
}

/*!
Sets curve's AutoAssign flag - in this case attributes will be set automatically
*/
void Plot2d_Curve::setAutoAssign( bool on )
{
  myAutoAssign = on;
}

/*!
Gets curve's AutoAssign flag state
*/
bool Plot2d_Curve::isAutoAssign() const
{
  return myAutoAssign;
}

/*!
Sets curve's color ( and resets AutoAssign flag )
*/
void Plot2d_Curve::setColor( const QColor& color )
{
  myColor = color;
  myAutoAssign = false;
}

/*!
Gets curve's color
*/
QColor Plot2d_Curve::getColor() const
{
  return myColor;
}

/*!
Sets symbol's color ( and resets AutoAssign flag )
*/
void Plot2d_Curve::setColorById( const int theColorId, const QColor& theColor )
{
  mySymbolsColorMap[theColorId] = theColor;
  myAutoAssign = false;
}

/*!
Gets curve's color
*/
QColor Plot2d_Curve::getColorById( const int theColorId ) const
{
  return mySymbolsColorMap[theColorId];
}

/*!
Sets curve's marker ( and resets AutoAssign flag )
*/
void Plot2d_Curve::setMarker( Plot2d::MarkerType marker )
{
  myMarker = marker;
  myAutoAssign = false;
}

/*!
Gets curve's marker
*/
Plot2d::MarkerType Plot2d_Curve::getMarker() const
{
  return myMarker;
}

/*!
Sets number of markers per step ( and resets AutoAssign flag ). 
*/
void Plot2d_Curve::setNbMarkers( const double nbMarkers )
{
  myNbMarkers = nbMarkers;
  myAutoAssign = false;
}

/*!
Gets number of markers per step ( and resets AutoAssign flag ). 
*/
double Plot2d_Curve::getNbMarkers() const
{
  return myNbMarkers;
}

/*!
Sets curve's line type and width ( and resets AutoAssign flag )
NOTE : A line width of 0 will produce a 1 pixel wide line using a fast algorithm for diagonals. 
A line width of 1 will also produce a 1 pixel wide line, but uses a slower more accurate 
algorithm for diagonals. 
For horizontal and vertical lines a line width of 0 is the same as a line width of 1.
*/
void Plot2d_Curve::setLine( Plot2d::LineType line, const int lineWidth )
{
  myLine = line;
  myLineWidth = lineWidth;
  if ( myLineWidth < 0 ) myLineWidth = 0;
  myAutoAssign = false;
}

/*!
Gets curve's line type
*/
Plot2d::LineType Plot2d_Curve::getLine() const
{
  return myLine;
}

/*!
Gets curve's line width
*/
int Plot2d_Curve::getLineWidth() const
{
  return myLineWidth;
}

/*!
Sets curve's y axis
*/
void Plot2d_Curve::setYAxis(QwtPlot::Axis theYAxis)
{
  if(theYAxis == QwtPlot::yLeft || theYAxis == QwtPlot::yRight)
    myYAxis = theYAxis;
}

/*!
Gets curve's y axis
*/
QwtPlot::Axis Plot2d_Curve::getYAxis() const
{
  return myYAxis;
}

/*!
Gets curve's minimal abscissa
*/
double Plot2d_Curve::getMinX() const
{
  pointList::const_iterator aIt;
  double aMinX = 1e150;
  //int aCurrent = 0;
  for(aIt = myPoints.begin(); aIt != myPoints.end(); ++aIt) {
    if ( (*aIt).x < aMinX )
      aMinX = (*aIt).x;
  }
  return aMinX;
}

/*!
Gets curve's maxiaml abscissa
*/
double Plot2d_Curve::getMaxX() const
{
  QList<Plot2d_Point>::const_iterator aIt;
  double aMaxX = -1e150;
  //int aCurrent = 0;
  for(aIt = myPoints.begin(); aIt != myPoints.end(); ++aIt) {
    if ( (*aIt).x > aMaxX )
      aMaxX = (*aIt).x;
  }
  return aMaxX;
}

/*!
Gets curve's minimal ordinate
*/
double Plot2d_Curve::getMinY() const
{
  pointList::const_iterator aIt;
  double aMinY = 1e150;
  //int aCurrent = 0;
  for(aIt = myPoints.begin(); aIt != myPoints.end(); ++aIt) {
    if ( (*aIt).y < aMinY )
      aMinY = (*aIt).y;
  }
  return aMinY;
}

/*!
Gets curve's maximal ordinate
*/
double Plot2d_Curve::getMaxY() const
{
  QList<Plot2d_Point>::const_iterator aIt;
  double aMaxY = -1e150;
  //int aCurrent = 0;
  for(aIt = myPoints.begin(); aIt != myPoints.end(); ++aIt) {
    if ( (*aIt).y > aMaxY )
      aMaxY = (*aIt).y;
  }
  return aMaxY;
}

/*!
Gets curve's minimal positive abscissa
*/
double Plot2d_Curve::getMinPositiveX() const
{
  pointList::const_iterator aIt;
  double aMinX = 1e150;
  //int aCurrent = 0;
  for(aIt = myPoints.begin(); aIt != myPoints.end(); ++aIt) {
    if ( (*aIt).x < aMinX && (*aIt).x > DBL_MIN )
      aMinX = (*aIt).x;
  }
  return aMinX;
}

/*!
Gets curve's minimal positive ordinate
*/
double Plot2d_Curve::getMinPositiveY() const
{
  pointList::const_iterator aIt;
  double aMinY = 1e150;
  //int aCurrent = 0;
  for(aIt = myPoints.begin(); aIt != myPoints.end(); ++aIt) {
    if ( (*aIt).y < aMinY && (*aIt).y > DBL_MIN )
      aMinY = (*aIt).y;
  }
  return aMinY;
}

/*!
Changes text assigned to point of curve
\param theInd -- index of point
\param theTxt -- new text
*/
void Plot2d_Curve::setText( const int theInd, const QString& theTxt )
{
  if( theInd < 0 || theInd >= myPoints.count() )
    return;

  myPoints[theInd].text = theTxt;
}

/*!
\return text assigned to point
\param ind -- index of point
*/
QString Plot2d_Curve::text( const int theInd ) const
{
  if( theInd < 0 || theInd >= myPoints.count() )
    return QString();
  else
    return myPoints[theInd].text;
}

/*!
Changes symbol color id assigned to a point of the curve
\param theInd     -- index of point
\param theColorId -- new color id
*/
void Plot2d_Curve::setSymbolColorId( const int theInd, const int theColorId )
{
  if( theInd < 0 || theInd >= myPoints.count() )
    return;

  myPoints[theInd].colorId = theColorId;
}

/*!
\return symbol color id assigned to point
\param theInd -- index of point
*/
int Plot2d_Curve::symbolColorId( const int theInd ) const
{
  if( theInd < 0 || theInd >= myPoints.count() )
    return 0;
  else
    return myPoints[theInd].colorId;
}

/*!
\brief Build symbols color map generating random colors for point's ids.
\param theBackWidget the background widget
\param theMaxAttempts the maximum number of attepts to generate a new color different from the background
*/
void Plot2d_Curve::buildSymbolsColorMap( const QWidget* theBackWidget, const int theMaxAttempts )
{
  QColor aBackColor = ( theBackWidget ? theBackWidget->palette().color( QPalette::Background ) : Qt::white );
  QColor aColor;

  pointList::const_iterator aIt;
  for(aIt = myPoints.begin(); aIt != myPoints.end(); ++aIt) 
  {
    // The zero id is mapped to the default color (black)
    if ( !mySymbolsColorMap.contains( (*aIt).colorId ) && (*aIt).colorId > 0 )
    {
      bool bOk = false;
      int cnt = 1;
      while ( !bOk ) {
        int aRed    = (int)( 256.0 * rand() / RAND_MAX);    // generate random color
        int aGreen  = (int)( 256.0 * rand() / RAND_MAX);    // ...
        int aBlue   = (int)( 256.0 * rand() / RAND_MAX);    // ...

        aColor      = QColor( aRed, aGreen, aBlue );

        cnt++;
        if ( cnt ==  theMaxAttempts )
          bOk = true;
        else
          bOk = !Plot2d_Plot2d::closeColors( aColor, aBackColor );
      }
      mySymbolsColorMap[ (*aIt).colorId ] = aColor;
    }
  }
}
