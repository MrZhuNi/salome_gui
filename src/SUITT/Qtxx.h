// Copyright (C) 2007-2015  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

// File:      Qtx.h
// Author:    Sergey TELKOV
//
#ifndef QTXX_H
#define QTXX_H

#if defined WIN32
#  if defined QTXX_EXPORTS || defined qtxx_EXPORTS
#    define QTX_EXPORT _declspec( dllexport )
#  else
#    define QTX_EXPORT _declspec( dllimport )
#  endif
#else
#  define QTX_EXPORT  
#endif

#if defined SOLARIS
#define bool  int
#define false 0
#define true  1
#endif

#include <QString>
#include <QList>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QGradient>

class QObject;
class QWidget;
class QCompleter;

typedef QList<int>    QIntList;       //!< list of int values
typedef QList<short>  QShortList;     //!< list of short int values
typedef QList<double> QDoubleList;    //!< list of double values
typedef QList<QColor> QColorList;     //!< list of colors

class QTX_EXPORT Qtxx
{
public:
  static QString     dir( const QString&, const bool = true );
  static QFont   stringToFont( const QString& fontDescription );
};

#endif
