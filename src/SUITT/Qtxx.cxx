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

// File:      Qtxx.cxx
// Author:    Sergey TELKOV
//
#include "Qtxx.h"

#include <QDir>
#include <QMenu>
#include <QRegExp>
#include <QBitmap>
#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QDirModel>
#include <QFileInfo>
#include <QCompleter>
#include <QApplication>
#include <QDesktopWidget>
#include <QtDebug>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <clocale>

/*!
  \brief Return directory part of the file path.

  If the file path does not include directory part (the file is in the
  current directory), null string is returned.

  \param path file path
  \param abs if true (default) \a path parameter is treated as absolute file path
  \return directory part of the file path
*/
QString Qtxx::dir( const QString& path, const bool abs )
{
  QDir aDir = QFileInfo( path ).dir();
  QString dirPath = abs ? aDir.absolutePath() : aDir.path();
  if ( dirPath == QString( "." ) )
    dirPath = QString();
  return dirPath;
}

/*!
  Creates font from string description
*/
QFont Qtxx::stringToFont( const QString& fontDescription )
{
  QFont font;
  if ( fontDescription.trimmed().isEmpty() || !font.fromString( fontDescription ) )
    font = QFont( "Courier", 11 );
  return font;
}
