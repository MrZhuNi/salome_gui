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
// File:      QtxColorButton.h
// Author:    Sergey TELKOV

#ifndef QTXCOLORBUTTON_H
#define QTXCOLORBUTTON_H

#include "Qtx.h"

#include <qmap.h>
#include <qcolor.h>
#include <qpixmap.h>
#include <qtoolbutton.h>

class QPaintDevice;

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

class QTX_EXPORT QtxColorButton : public QToolButton
{
  Q_OBJECT

  class Popup;

public:
  QtxColorButton( QWidget* = 0 );
  virtual ~QtxColorButton();

  QColor        color() const;
  void          setColor( const QColor& );

  bool          eventFilter( QObject*, QEvent* );

  virtual QSize sizeHint() const;

  bool          isAutoEnabled() const;
  void          setAutoEnabled( bool );

  QString       autoText() const;
  void          setAutoText( const QString& );

signals:
  void          clicked( QColor );
  void          changed( QColor );

private slots:
  void          onClicked();
  void          onAboutToShow();
  void          onToggled( bool );

  void          onAutoClicked();
  void          onDialogClicked();

protected:
  virtual void  paintEvent( QPaintEvent* );
  virtual void  drawButton( QPainter* );

private:
  QValueList<QColor> colorsList() const;

  void          updateState();
  void          updateButton( QToolButton* );
  QPixmap       buttonIcon( const QColor& ) const;
  void          drawColor( QPixmap&, const QColor&, const int = 1 ) const;

private:
  typedef QMap<const QToolButton*, QColor> ColorMap;

private:
  ColorMap      myColors;
  QToolButton*  myAutoButton;
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif
