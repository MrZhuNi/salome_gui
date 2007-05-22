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

/*!
  \class:   SUIT_MessageBox
  Message dialog box for SUIT-based application
  Module:  SUIT
  Created: UI team, 02.10.00
*/

#include "SUIT_MessageBox.h"

#include "SUIT_OverrideCursor.h"

#include <QMessageBox>
#include <QPushButton>
#include <QApplication>

/*!
  Constructor
*/
SUIT_MessageBox::SUIT_MessageBox( QWidget* parent )
: QMessageBox( parent )
{
}

/*!
  Constructor
*/
SUIT_MessageBox::SUIT_MessageBox( Icon icon, const QString& title, const QString& text,
                                  StandardButtons buttons, QWidget* parent, Qt::WindowFlags f )
: QMessageBox( icon, title, text, buttons, parent, f )
{
}

/*!
  Destructor
*/
SUIT_MessageBox::~SUIT_MessageBox()
{
}

/*!
  Returns the text of the specified button
*/
QString SUIT_MessageBox::buttonText( StandardButton btn ) const
{
  QString res;
  QAbstractButton* b = button( btn );
  if ( b )
    res = b->text();
  return res;
}

/*!
  Set the text of the specified button
*/
void SUIT_MessageBox::setButtonText( StandardButton btn, const QString& text )
{
  QAbstractButton* b = button( btn );
  if ( b )
    b->setText( text );
}

/*!
  Shows critical message box with specified standard buttons. [ static ]
*/
SUIT_MessageBox::StandardButton SUIT_MessageBox::critical( QWidget* parent, const QString& title, const QString& text,
                                                           StandardButtons buttons, StandardButton defaultButton )
{
  return QMessageBox::critical( parent, title, text, buttons, defaultButton );
}

/*!
  Shows information message box with specified standard buttons. [ static ]
*/
SUIT_MessageBox::StandardButton SUIT_MessageBox::information( QWidget* parent, const QString& title, const QString& text,
                                                              StandardButtons buttons, StandardButton defaultButton )
{
  return QMessageBox::information( parent, title, text, buttons, defaultButton );
}

/*!
  Shows question message box with specified standard buttons. [ static ]
*/
SUIT_MessageBox::StandardButton SUIT_MessageBox::question( QWidget* parent, const QString& title, const QString& text,
                                                           StandardButtons buttons, StandardButton defaultButton )
{
  return QMessageBox::question( parent, title, text,buttons, defaultButton );
}

/*!
  Shows warning message box with specified standard buttons. [ static ]
*/
SUIT_MessageBox::StandardButton SUIT_MessageBox::warning( QWidget* parent, const QString& title, const QString& text,
                                                          SUIT_MessageBox::StandardButtons buttons, StandardButton defaultButton )
{
  return QMessageBox::warning( parent, title, text, buttons, defaultButton );
}

/*!
  Shows critical message box. Some buttons can be renamed. Variable number of arguments
  should be specified starting from \param btn as pairs of StandardButton and QString.
  After the last pair 0 (zero) value should be specified. [ static ]
*/
SUIT_MessageBox::StandardButton SUIT_MessageBox::critical( QWidget* parent, const QString& title,
                                                           const QString& text, StandardButtons buttons,
                                                           StandardButton defaultButton, StandardButton btn, ... )
{
  va_list args;
  va_start( args, btn );
  return messageBox( SUIT_MessageBox::Critical, parent, title, text,
                     buttons, defaultButton, messageMap( btn, args ) );
}

/*!
  Shows information message box. Some buttons can be renamed. Variable number of arguments
  should be specified starting from \param btn as pairs of StandardButton and QString.
  After the last pair 0 (zero) value should be specified. [ static ]
*/
SUIT_MessageBox::StandardButton SUIT_MessageBox::information( QWidget* parent, const QString& title,
                                                              const QString& text,
                                                              SUIT_MessageBox::StandardButtons buttons,
                                                              SUIT_MessageBox::StandardButton defaultButton,
                                                              SUIT_MessageBox::StandardButton btn, ... )
{
  va_list args;
  va_start( args, btn );
  return messageBox( SUIT_MessageBox::Information, parent, title, text,
                     buttons, defaultButton, messageMap( btn, args ) );
}

/*!
  Shows question message box. Some buttons can be renamed. Variable number of arguments
  should be specified starting from \param btn as pairs of StandardButton and QString.
  After the last pair 0 (zero) value should be specified. [ static ]
*/
SUIT_MessageBox::StandardButton SUIT_MessageBox::question( QWidget* parent, const QString& title,
                                                           const QString& text, StandardButtons buttons,
                                                           StandardButton defaultButton, StandardButton btn, ... )
{
  va_list args;
  va_start( args, btn );
  return messageBox( SUIT_MessageBox::Question, parent, title, text,
                     buttons, defaultButton, messageMap( btn, args ) );
}

/*!
  Shows warning message box. Some buttons can be renamed. Variable number of arguments
  should be specified starting from \param btn as pairs of StandardButton and QString.
  After the last pair 0 (zero) value should be specified. [ static ]
*/
SUIT_MessageBox::StandardButton SUIT_MessageBox::warning( QWidget* parent, const QString& title,
                                                          const QString& text, StandardButtons buttons,
                                                          StandardButton defaultButton, StandardButton btn, ... )
{
  va_list args;
  va_start( args, btn );
  return messageBox( SUIT_MessageBox::Warning, parent, title, text,
                     buttons, defaultButton, messageMap( btn, args ) );
}

/*!
  Shows critical message box with user specified buttons. Each button decribed by two
  parameters: int - button id and QString - button text. First button specified by \param btn0
  and \param txt0, following buttons specified as variable number of arguments which 
  should be started from \param btn as pairs of int and QString.
  After the last pair 0 (zero) value should be specified. [ static ]
*/
int SUIT_MessageBox::critical( QWidget* parent, const QString& title, const QString& text,
                               int defaultButton, int btn0, QString txt0, int btn, ... )
{
  va_list args;
  va_start( args, btn );
  return messageBox( SUIT_MessageBox::Critical, parent, title, text,
                     defaultButton, messageList( btn0, txt0, btn, args ) );
}

/*!
  Shows information message box with user specified buttons. Each button decribed by two
  parameters: int - button id and QString - button text. First button specified by \param btn0
  and \param txt0, following buttons specified as variable number of arguments which 
  should be started from \param btn as pairs of int and QString.
  After the last pair 0 (zero) value should be specified. [ static ]
*/
int SUIT_MessageBox::information( QWidget* parent, const QString& title, const QString& text,
                                  int defaultButton, int btn0, QString txt0, int btn, ... )
{
  va_list args;
  va_start( args, btn );
  return messageBox( SUIT_MessageBox::Information, parent, title, text,
                     defaultButton, messageList( btn0, txt0, btn, args ) );
}

/*!
  Shows question message box with user specified buttons. Each button decribed by two
  parameters: int - button id and QString - button text. First button specified by \param btn0
  and \param txt0, following buttons specified as variable number of arguments which 
  should be started from \param btn as pairs of int and QString.
  After the last pair 0 (zero) value should be specified. [ static ]
*/
int SUIT_MessageBox::question( QWidget* parent, const QString& title, const QString& text,
                               int defaultButton, int btn0, QString txt0, int btn, ... )
{
  va_list args;
  va_start( args, btn );
  return messageBox( SUIT_MessageBox::Question, parent, title, text,
                     defaultButton, messageList( btn0, txt0, btn, args ) );
}

/*!
  Shows warning message box with user specified buttons. Each button decribed by two
  parameters: int - button id and QString - button text. First button specified by \param btn0
  and \param txt0, following buttons specified as variable number of arguments which 
  should be started from \param btn as pairs of int and QString.
  After the last pair 0 (zero) value should be specified. [ static ]
*/
int SUIT_MessageBox::warning( QWidget* parent, const QString& title, const QString& text,
                              int defaultButton, int btn0, QString txt0, int btn, ... )
{
  va_list args;
  va_start( args, btn );
  return messageBox( SUIT_MessageBox::Warning, parent, title, text,
                     defaultButton, messageList( btn0, txt0, btn, args ) );
}

/*!
  Shows critical message box with user specified buttons. Each button decribed by button text.
  Variable number of arguments should be started from \param txt. After the last text 0 (zero)
  value should be specified. [ static ]
*/
int SUIT_MessageBox::critical( QWidget* parent, const QString& title, const QString& text, char* txt, ... )
{
  va_list args;
  va_start( args, txt );
  return messageBox( SUIT_MessageBox::Critical, parent, title, text,
                     0, messageList( txt, args ) );
}

/*!
  Shows information message box with user specified buttons. Each button decribed by button text.
  Variable number of arguments should be started from \param txt. After the last text 0 (zero)
  value should be specified. [ static ]
*/
int SUIT_MessageBox::information( QWidget* parent, const QString& title, const QString& text, char* txt, ... )
{
  va_list args;
  va_start( args, txt );
  return messageBox( SUIT_MessageBox::Information, parent, title, text,
                     0, messageList( txt, args ) );
}

/*!
  Shows question message box with user specified buttons. Each button decribed by button text.
  Variable number of arguments should be started from \param txt. After the last text 0 (zero)
  value should be specified. [ static ]
*/
int SUIT_MessageBox::question( QWidget* parent, const QString& title, const QString& text, char* txt, ... )
{
  va_list args;
  va_start( args, txt );
  return messageBox( SUIT_MessageBox::Question, parent, title, text,
                     0, messageList( txt, args ) );
}

/*!
  Shows warning message box with user specified buttons. Each button decribed by button text.
  Variable number of arguments should be started from \param txt. After the last text 0 (zero)
  value should be specified. [ static ]
*/
int SUIT_MessageBox::warning( QWidget* parent, const QString& title, const QString& text, char* txt, ... )
{
  va_list args;
  va_start( args, txt );
  return messageBox( SUIT_MessageBox::Warning, parent, title, text,
                     0, messageList( txt, args ) );
}

SUIT_MessageBox::StandardButton SUIT_MessageBox::messageBox( SUIT_MessageBox::Icon icon, QWidget* parent,
                                                             const QString& title, const QString& text,
                                                             StandardButtons buttons, StandardButton defaultButton,
                                                             const ButtonMap& map )
{
  SUIT_MessageBox msgBox( icon, title, text, buttons, parent );
  for ( ButtonMap::const_iterator it = map.begin(); it != map.end(); ++it )
    msgBox.setButtonText( it.key(), it.value() );

  if ( defaultButton != NoButton )
    msgBox.setDefaultButton( ::qobject_cast<QPushButton*>( msgBox.button( defaultButton ) ) );

  SUIT_OverrideCursor cw( parent ? parent->cursor() : Qt::ArrowCursor );

  StandardButton res = NoButton;
  if ( msgBox.exec() == -1 )
    res = QMessageBox::Cancel;
  else
    res = msgBox.standardButton( msgBox.clickedButton() );

  QApplication::processEvents();

  return res;
}

int SUIT_MessageBox::messageBox( Icon icon, QWidget* parent, const QString& title, const QString& text,
                                 const int defaultButton, const ButtonList& lst )
{
  SUIT_MessageBox msgBox( icon, title, text, NoButton, parent );

  QMap<QAbstractButton*, int> map;
  for ( ButtonList::const_iterator it = lst.begin(); it != lst.end(); ++it )
  {
    int btn = (*it).first;
    QString txt = (*it).second;
    ButtonRole role = InvalidRole;

    if ( btn == defaultButton )
      role = AcceptRole;

    QPushButton* pb = msgBox.addButton( txt, role );
    map.insert( pb, btn );

    if ( btn == defaultButton )
      msgBox.setDefaultButton( pb );
  }

  SUIT_OverrideCursor cw( parent ? parent->cursor() : Qt::ArrowCursor );

  int res = NoButton;
  if ( msgBox.exec() == -1 )
    res = Cancel;
  else
    res = map[msgBox.clickedButton()];

  QApplication::processEvents();

  return res;
}

SUIT_MessageBox::ButtonMap SUIT_MessageBox::messageMap( StandardButton btn, va_list& args )
{
  ButtonMap map;
  StandardButton cur = btn;
  while ( !cur )
  {
    QString name = va_arg( args, QString );
    map.insert( cur, name );
    cur = va_arg( args, StandardButton );
  }

  va_end( args );

  return map;
}

SUIT_MessageBox::ButtonList SUIT_MessageBox::messageList( int btn0, QString txt0, int btn, va_list& args )
{
  ButtonList lst;
  lst.append( QPair<int, QString>( btn0, txt0 ) );
  int cur = btn;
  while ( !cur )
  {
    QString name = va_arg( args, QString );
    lst.append( QPair<int, QString>( cur, name ) );
    cur = va_arg( args, int );
  }

  va_end( args );

  return lst;
}

SUIT_MessageBox::ButtonList SUIT_MessageBox::messageList( char* txt, va_list& args )
{
  int i = 0;
  ButtonList lst;
  char* cur = txt;
  while ( cur )
  {
    lst.append( QPair<int, QString>( i++, cur ) );
    cur = va_arg( args, char* );
  }

  va_end( args );

  return lst;
}
