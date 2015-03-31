// Copyright (C) 2010-2015  CEA/DEN, EDF R&D
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
// File   : PyEditor.cxx
// Author : Maxim GLIBIN, Open CASCADE S.A.S. (maxim.glibin@opencascade.com)
//

#include <QApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>

#include "PyEditor_EditorWindow.h"

#include <QtxTranslator.h>

int main( int argc, char *argv[] )
{
    QApplication anApplication( argc, argv );

    // Load translations
    QtxTranslator aTranslator, aTranslatorQt;
    QString aLanguage = QLocale::system().name().split('_', QString::SkipEmptyParts)[0];
    if ( !aLanguage.isEmpty() )
    {
      if ( aTranslatorQt.load( QString( "qt_%1" ).arg( aLanguage ), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) ) )
        anApplication.installTranslator( &aTranslatorQt );

      QDir appDir = QApplication::applicationDirPath();
      appDir.cdUp(); appDir.cdUp();

      if ( aTranslator.load( QString( "PyEditor_msg_%1" ).arg( aLanguage ), appDir.filePath( "share/salome/resources/gui" ) ) )
        anApplication.installTranslator( &aTranslator );
    }

    PyEditor_EditorWindow anEditorWin;
    anEditorWin.resize( 650, 700 );
    anEditorWin.show();

    return anApplication.exec();
}
