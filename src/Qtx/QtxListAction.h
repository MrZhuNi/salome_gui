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
// File:      QtxListAction.hxx
// Author:    Sergey TELKOV (Based on code by Eugene AKSENOV)

#ifndef QTXLISTACTION_H
#define QTXLISTACTION_H

#include "Qtx.h"

#include <QStringList>
#include <QWidgetAction>

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

class QTX_EXPORT QtxListAction : public QWidgetAction
{
  Q_OBJECT

  class ListFrame;
  class ListWidget;
  class ScrollEvent;

public:
  //! Popup mode
  enum { 
    Item,         //!< action is added to popup menu as menu item
    SubMenu       //!< action is added to popup menu as sub menu with list of items
  } PopupMode;

public:
  QtxListAction( QObject* = 0 );
  QtxListAction( const QString&, int, QObject* );
  QtxListAction( const QIcon&, const QString&, int, QObject* );
  virtual ~QtxListAction();

  int              popupMode() const;
  void             setPopupMode( const int );

  QStringList      names() const;
  void             addNames( const QStringList&, bool = true );
  void             setComment( const QString&, const QString& = QString() );

  int              linesNumber() const;
  int              charsNumber() const;

  void             setLinesNumber( const int );
  void             setCharsNumber( const int );

signals:
  void             activated( int );

private slots:
  void             onChanged();
  void             onMultiple( const int );
  void             onSingle( bool = false );
  void             onTriggered( bool = false );

protected:
  virtual QWidget* createWidget( QWidget* );
  virtual void     deleteWidget( QWidget* );

private:
  void             initialize();

private:
  ListFrame*       myFrame;   //!< list of actions shown as submenu

  friend class QtxListAction::ListFrame;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
