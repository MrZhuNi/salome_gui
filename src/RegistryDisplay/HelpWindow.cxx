//  SALOME RegistryDisplay : GUI for Registry server implementation
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : HelpWindow.cxx
//  Author : Pascale NOYRET, EDF
//  Module : SALOME
//  $Header$

# include "HelpWindow.hxx"
# include "utilities.h"

# include <QTextEdit>
# include <QTextStream> 
# include <QFile> 
# include <QColorGroup>

using namespace std;

/*!
  Constructor
*/
HelpWindow::HelpWindow(QWidget* parent, const char* name ) 
  : QMainWindow( parent, Qt::Window )
{
  BEGIN_OF("Constructeur HelpWindow");

  setObjectName( name );
  setAttribute( Qt::WA_DeleteOnClose );

  setWindowTitle( tr( "Help" ) );

  myTextView = new QTextEdit( this );
  myTextView->setObjectName( "myTextView" );
  myTextView->setReadOnly( true );
  QPalette pal = myTextView->palette();

  pal.setBrush( QPalette::Active, QPalette::Highlight,       QBrush( QColor( 0, 0, 128 ) ) );
  pal.setBrush( QPalette::Active, QPalette::HighlightedText, QBrush( Qt::white ) );
  pal.setBrush( QPalette::Active, QPalette::Base,            QBrush( QColor( 255,255,220 ) ) );
  pal.setBrush( QPalette::Active, QPalette::Text,            QBrush( Qt::black ) );

  pal.setBrush( QPalette::Inactive, QPalette::Highlight,       QBrush( QColor( 0, 0, 128 ) ) );
  pal.setBrush( QPalette::Inactive, QPalette::HighlightedText, QBrush( Qt::white ) );
  pal.setBrush( QPalette::Inactive, QPalette::Base,            QBrush( QColor( 255,255,220 ) ) );
  pal.setBrush( QPalette::Inactive, QPalette::Text,            QBrush( Qt::black ) );

  pal.setBrush( QPalette::Disabled, QPalette::Highlight,       QBrush( QColor( 0, 0, 128 ) ) );
  pal.setBrush( QPalette::Disabled, QPalette::HighlightedText, QBrush( Qt::white ) );
  pal.setBrush( QPalette::Disabled, QPalette::Base,            QBrush( QColor( 255,255,220 ) ) );
  pal.setBrush( QPalette::Disabled, QPalette::Text,            QBrush( Qt::black ) );

  myTextView->setPalette( pal );
  
  setCentralWidget( myTextView );
  setMinimumSize( 450, 250 );

  QFile f ( "tmp.txt" );
  if ( f.open( QIODevice::ReadOnly ) )   
    {
      QTextStream t( &f ); 
      while ( !t.atEnd() ) 
	{
	  myTextView->append(t.readLine());
	}
    }
  f.close();

  END_OF("Constructeur HelpWindow");
}

/*!
  Destructor
*/
HelpWindow::~HelpWindow()
{
  BEGIN_OF("Destructeur HelpWindow");
  END_OF("Destructeur HelpWindow");
};

/*!
  Sets text
*/
void HelpWindow::setText( const QString& text )
{
  myTextView->setText( text );
}


