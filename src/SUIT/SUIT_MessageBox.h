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

#ifndef SUIT_MESSAGEBOX_H
#define SUIT_MESSAGEBOX_H

#include "SUIT.h"

#include <QMap>
#include <QList>
#include <QMessageBox>

#include <stdarg.h>

/*!
  \class SUIT_MessageBox
  \brief Message dialog box for SUIT-based application
*/
class SUIT_EXPORT SUIT_MessageBox : public QMessageBox
{
public:
  SUIT_MessageBox( QWidget* = 0 );
  SUIT_MessageBox( Icon, const QString&, const QString&, StandardButtons buttons = NoButton,
                   QWidget* = 0, Qt::WindowFlags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint );
  ~SUIT_MessageBox();

  QString               buttonText( StandardButton ) const;
  void                  setButtonText( StandardButton, const QString& );

  static StandardButton critical( QWidget* parent, const QString& title, const QString& text,
                                  StandardButtons buttons = Ok, StandardButton defaultButton = NoButton );
  static StandardButton information( QWidget* parent, const QString& title, const QString& text,
                                     StandardButtons buttons = Ok, StandardButton defaultButton = NoButton );
  static StandardButton question( QWidget* parent, const QString& title, const QString& text,
                                  StandardButtons buttons = Ok, StandardButton defaultButton = NoButton );
  static StandardButton warning( QWidget* parent, const QString& title, const QString& text,
                                 StandardButtons buttons = Ok, StandardButton defaultButton = NoButton );

  static StandardButton critical( QWidget* parent, const QString& title, const QString& text,
                                  StandardButtons buttons, StandardButton defaultButton, StandardButton, ... );
  static StandardButton information( QWidget* parent, const QString& title, const QString& text,
                                     StandardButtons buttons, StandardButton defaultButton, StandardButton, ... );
  static StandardButton question( QWidget* parent, const QString& title, const QString& text,
                                  StandardButtons buttons, StandardButton defaultButton, StandardButton, ... );
  static StandardButton warning( QWidget* parent, const QString& title, const QString& text,
                                 StandardButtons buttons, StandardButton defaultButton, StandardButton, ... );

  static int            critical( QWidget* parent, const QString& title, const QString& text,
                                  int defaultButton, int, QString, int, ... );
  static int            information( QWidget* parent, const QString& title, const QString& text,
                                     int defaultButton, int, QString, int, ... );
  static int            question( QWidget* parent, const QString& title, const QString& text,
                                  int defaultButton, int, QString, int, ... );
  static int            warning( QWidget* parent, const QString& title, const QString& text,
                                 int defaultButton, int, QString, int, ... );

  static int            critical( QWidget* parent, const QString& title, const QString& text, char*, ... );
  static int            information( QWidget* parent, const QString& title, const QString& text, char*, ... );
  static int            question( QWidget* parent, const QString& title, const QString& text, char*, ... );
  static int            warning( QWidget* parent, const QString& title, const QString& text, char*, ... );

private:
  typedef QMap<StandardButton, QString> ButtonMap;
  typedef QList< QPair<int, QString> > ButtonList;

private:
  static StandardButton messageBox( Icon icon, QWidget* parent, const QString& title, const QString& text,
                                    StandardButtons buttons, StandardButton defaultButton, const ButtonMap& );

  static int            messageBox( Icon icon, QWidget* parent, const QString& title, const QString& text,
                                    const int defaultButton, const ButtonList& );

  static ButtonMap      messageMap( StandardButton, va_list& );
  static ButtonList     messageList( int, QString, int, va_list& );
  static ButtonList     messageList( char*, va_list& );
};

#endif
