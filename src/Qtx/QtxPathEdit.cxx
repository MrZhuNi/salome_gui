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
// File:      QtxPathEdit.cxx
// Author:    Sergey TELKOV

#include "QtxPathEdit.h"

#include <QLayout>
#include <QDirModel>
#include <QLineEdit>
#include <QCompleter>
#include <QToolButton>
#include <QFileDialog>
#include <QRegExpValidator>

static const char* browse_icon[] = {
"16 16 5 1",
"  c none",
". c #ffff00",
"# c #848200",
"a c #ffffff",
"b c #000000",
"                ",
"          bbb   ",
"         b   b b",
"              bb",
"  bbb        bbb",
" ba.abbbbbbb    ",
" b.a.a.a.a.b    ",
" ba.a.a.a.ab    ",
" b.a.abbbbbbbbbb",
" ba.ab#########b",
" b.ab#########b ",
" bab#########b  ",
" bb#########b   ",
" bbbbbbbbbbb    ",
"                ",
"                "
};

/*!
  \class QtxPathEdit
  \brief The QtxPathEdit class represents a widget for file or directory
  path preference items editing.

  The path preference item is represented as the line edit box for the
  direct path editing and small button clicking on which invokes browse
  dialog box. The widget can be used in different modes: "Open File",
  "Save File", "Select Directory". The mode defines the type of the
  standard browse dialog box which is invoked on the button clicking.

  Initial path value can be set with setPath() method. Chosen path
  can be retrieved with the path() method. The widget mode can be set
  with setPathType() and retrieved with pathType() method.

  In addition, file/direcrory filters (wildcards) can be set with the
  setPathFilter() method and retrieved with pathFilter() method.
*/

/*!
  \brief Constructor
  \param type widget mode (Qtx::PathType)
  \param parent parent widget
  \sa pathType(), setPathType()
*/
QtxPathEdit::QtxPathEdit( const Qtx::PathType type, QWidget* parent )
: QFrame( parent ),
  myType( type )
{
  initialize();
}

/*!
  \brief Constructor

  Qtx::PT_OpenFile mode is used by default.

  \param parent parent widget
  \sa pathType(), setPathType()
*/
QtxPathEdit::QtxPathEdit( QWidget* parent )
: QFrame( parent ),
  myType( Qtx::PT_OpenFile )
{
  initialize();
}

/*!
  \brief Destructor
*/
QtxPathEdit::~QtxPathEdit()
{
}

/*!
  \brief Get widget mode.
  \return currently used widget mode (Qtx::PathType)
  \sa setPathType()
*/
Qtx::PathType QtxPathEdit::pathType() const
{
  return myType;
}

/*!
  \brief Set widget mode.
  \param type new widget mode (Qtx::PathType)
  \sa pathType()
*/
void QtxPathEdit::setPathType( const Qtx::PathType type )
{
  if ( myType == type )
    return;

  myType = type;
  updateState();
}

/*!
  \brief Get currently selected path.
  \return file or directory path entered by the user
  \sa setPath()
*/
QString QtxPathEdit::path() const
{
  return myPath->text();
}

/*!
  \brief Set path.
  \param txt file or directory path
  \sa path()
*/
void QtxPathEdit::setPath( const QString& txt )
{
  myPath->setText( txt );
}

/*!
  \brief Get currently used path filters.
  \return file or directory path filters
  \sa setPathFilter()
*/
QString QtxPathEdit::pathFilter() const
{
  return myFilter;
}

/*!
  \brief Set path filters.
  \param f new file or directory path filters
  \sa pathFilter()
*/
void QtxPathEdit::setPathFilter( const QString& f )
{
  if ( myFilter == f )
    return;

  myFilter = f;
  updateState();
}

/*!
  \brief Get currently used validator.
  \return validator
  \sa setValidator()
*/
const QValidator* QtxPathEdit::validator() const
{
  return lineEdit()->validator();
}

/*!
  \brief Set validator.
  \param v new validator
  \sa validator()
*/
void QtxPathEdit::setValidator( QValidator* v )
{
  lineEdit()->setValidator( v );
}

/*!
  \brief Called when user clicks "Browse" button.

  Invokes standard browsng dialog box depending on the used widget mode.

  \param on (not used)
  \sa mode(), setMode()
*/
void QtxPathEdit::onBrowse( bool /*on*/ )
{
  QString path;
  QString initial = QFileInfo( myPath->text() ).path();
  switch ( pathType() )
  {
  case Qtx::PT_OpenFile:
    path = QFileDialog::getOpenFileName( myPath, QString(), initial, pathFilter() );
    break;
  case Qtx::PT_SaveFile:
    path = QFileDialog::getSaveFileName( myPath, QString(), initial, pathFilter() );
    break;
  case Qtx::PT_Directory:
    path = QFileDialog::getExistingDirectory( myPath, QString(), initial );
    break;
  }

  if ( !path.isEmpty() )
    myPath->setText( QDir::convertSeparators( path ) );

  myPath->setFocus();
}

/*!
  \brief Get internal line edit widget.
  \return line edit box widget
*/
QLineEdit* QtxPathEdit::lineEdit() const
{
  return myPath;
}

/*!
  \brief Perform internal widget intialization.
*/
void QtxPathEdit::initialize()
{
  QHBoxLayout* base = new QHBoxLayout( this );
  base->setMargin( 0 );
  base->setSpacing( 5 );

  base->addWidget( myPath = new QLineEdit( this ) );
  myPath->setValidator( new QRegExpValidator( QRegExp( "^([\\w/]{2}|[A-Z]:)[^:;\\*\\?]*[\\w\\\\/\\.]$" ), myPath ) );

  QToolButton* browse = new QToolButton( this );
  browse->setIcon( QPixmap( browse_icon ) );
  base->addWidget( browse );

  connect( browse, SIGNAL( clicked( bool ) ), this, SLOT( onBrowse( bool ) ) );

  setFocusProxy( myPath );

  updateState();
}

/*!
  \brief Update widget state.
*/
void QtxPathEdit::updateState()
{
  myPath->setCompleter( Qtx::pathCompleter( pathType(), pathFilter() ) );
}
