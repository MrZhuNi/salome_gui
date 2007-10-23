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
// File:      QtxColorButton.cxx
// Author:    Sergey TELKOV

#include "QtxColorButton.h"

#include <qevent.h>
#include <qstyle.h>
#include <qlayout.h>
#include <qbitmap.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qobjectlist.h>
#include <qcolordialog.h>

/*!
  \class QtxColorButton::Popup
*/

class QtxColorButton::Popup : public QPopupMenu
{
public:
  Popup( QWidget* = 0 );
  virtual ~Popup();

  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;

  virtual void setMinimumSize( int, int );
  virtual void setMaximumSize( int, int );

  virtual void resize( int, int );
};


QtxColorButton::Popup::Popup( QWidget* p )
: QPopupMenu( p )
{
}

QtxColorButton::Popup::~Popup()
{
}

QSize QtxColorButton::Popup::sizeHint() const
{
  return QFrame::sizeHint();
}

QSize QtxColorButton::Popup::minimumSizeHint() const
{
  return QFrame::minimumSizeHint();
}

void QtxColorButton::Popup::setMinimumSize( int, int )
{
}

void QtxColorButton::Popup::setMaximumSize( int, int )
{
}

void QtxColorButton::Popup::resize( int w, int h )
{
  QSize sz = sizeHint();
  QPopupMenu::resize( QMAX( sz.width(), w ), QMAX( sz.height(), h ) );
}

/*!
  \class QtxColorButton
  \brief The QtxColorButton class implements a widget for color
  preference items editing.

  The color preference item is represented as the colored button with
  assocoiated popup menu whihc is called when the user presses the small 
  arrow button near it. The popup menu allows selecting of the color
  from the predefined set. In addition it contains the button which
  invokes standard "Select color" dialog box.

  Initial color value can be set with setColor() method. Chosen color
  can be retrieved with the color() method.
*/

/*!
  \brief Constructor.
  \param parent parent widget
*/
QtxColorButton::QtxColorButton( QWidget* parent )
: QToolButton( parent )
{
  setToggleButton( false );

  QPopupMenu* pm = new Popup( this );
  QGridLayout* grid = new QGridLayout( pm );
  grid->setMargin( 5 );
  grid->setSpacing( 0 );

  QValueList<QColor> cList = colorsList();
  int w = 8;
  int h = cList.count() / w;

  for ( int y = 0; y < h; y++ )
  {
    for ( int x = 0; x < w; x++ )
    {
      QColor c = cList[x * h + y];
      QToolButton* btn = new QToolButton( pm );
      btn->setAutoRaise( true );
      btn->setToggleButton( true );
      myColors.insert( btn, c );
      grid->addWidget( btn, y + 1, x );
      
      btn->installEventFilter( this );

      connect( btn, SIGNAL( toggled( bool ) ), this, SLOT( onToggled( bool ) ) );

      updateButton( btn );
    }
  }

  myAutoButton = new QToolButton( pm );
  myAutoButton->setText( tr( "Auto" ) );
  myAutoButton->setAutoRaise( true );
  myAutoButton->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
  grid->addMultiCellWidget( myAutoButton, 0, 0, 0, grid->numCols() - 1 );
  connect( myAutoButton, SIGNAL( clicked() ), this, SLOT( onAutoClicked() ) );

  QToolButton* other = new QToolButton( pm );
  other->setText( tr( "Other colors..." ) );
  other->setAutoRaise( true );
  other->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
  grid->addMultiCellWidget( other, grid->numRows(), grid->numRows(), 0, grid->numCols() - 1 );
  connect( other, SIGNAL( clicked() ), this, SLOT( onDialogClicked() ) );

  other->installEventFilter( this );

  setPopup( pm );
  setPopupDelay( 0 );

  connect( this, SIGNAL( clicked() ), this, SLOT( onClicked() ) );
  connect( pm,   SIGNAL( aboutToShow() ),   this, SLOT( onAboutToShow() ) );

  updateButton( this );
}

/*!
  \brief Destructor.
*/
QtxColorButton::~QtxColorButton()
{
}

/*!
  \brief Get currently selected color.

  Returns null QColor if no color is selected.

  \return selected color
  \sa setColor()
*/
QColor QtxColorButton::color() const
{
  return myColors.contains( this ) ? myColors[this] : QColor();
}

/*!
  \brief Set color.
  \param c color to be set as current
  \sa color()
*/
void QtxColorButton::setColor( const QColor& c )
{
  myColors.insert( this, c );
  updateState();
  update();
  updateButton( this );
}

QSize QtxColorButton::sizeHint() const
{
  return QToolButton::sizeHint();
//  return QSize( 32, 16 );
}

bool QtxColorButton::isAutoEnabled() const
{
  return myAutoButton->isVisibleTo( myAutoButton->parentWidget() );
}

void QtxColorButton::setAutoEnabled( bool on )
{
  myAutoButton->setShown( on );
}

QString QtxColorButton::autoText() const
{
  return myAutoButton->text();
}

void QtxColorButton::setAutoText( const QString& txt )
{
  myAutoButton->setText( txt );
}

/*!
  \brief Filter events for the child widgets.
  \param o event receiver object
  \param e event
  \return \c true if the event should be filtered
*/
bool QtxColorButton::eventFilter( QObject* o, QEvent* e )
{
  if ( e->type() == QEvent::Leave )
    updateButton( ::qt_cast<QToolButton*>( o ) );
  return QToolButton::eventFilter( o, e );
}

/*!
  \brief Called when the popup menu is about to show.

  Updates the menu and child widgets state.
*/
void QtxColorButton::onAboutToShow()
{
  updateState();

  QPopupMenu* pm = popup();
  if ( !pm )
    return;

  pm->adjustSize();
}

/*!
  \brief Called when the button is clicked by the user.

  Emits the signal clicked( QColor ).

  \param on button state (not used)
*/
void QtxColorButton::onClicked()
{
  emit clicked( color() );
}

/*!
  \brief Called when any color selection button from popup menu
  is clicked.

  Changes the currently selected color and emits the signal
  changed( QColor ).

  \param on button state
*/
void QtxColorButton::onToggled( bool on )
{
  const QToolButton* tb = ::qt_cast<QToolButton*>( sender() );
  if ( !tb )
    return;

  QColor old = color();

  if ( on && myColors.contains( tb ) )
  {
    myColors.insert( this, myColors[tb] );
    updateButton( this );
  }

  if ( popup() )
    popup()->hide();

  updateState();

  if ( old != color() )
    emit changed( color() );
}

/*!
  \brief Called the "Other colors" child button from popup menu
  is clicked.

  Invokes standard "Select color" dialog box allowing user to select
  custom color. If the current color is changed by the user, emits 
  the signal changed( QColor ).

  \param on (not used)
*/
void QtxColorButton::onDialogClicked()
{
  if ( popup() )
    popup()->hide();

  QColor c = QColorDialog::getColor( color(), this );
  if ( !c.isValid() )
    return;

  QColor old = color();

  setColor( c );

  if ( old != color() )
    emit changed( color() );
}

void QtxColorButton::onAutoClicked()
{
  if ( popup() )
    popup()->hide();

  setColor( QColor() );
}

/*!
  \brief Customize paint event for the widget.
  \param e paint event
*/
void QtxColorButton::paintEvent( QPaintEvent* e )
{
  QToolButton::paintEvent( e );
/*
  int m = 2;

  QRect r = rect();
  r.setTopLeft( r.topLeft() + QPoint( m, m ) );
  r.setBottomRight( r.bottomRight() - QPoint( m, m ) );

  if ( isDown() )
    r.moveBy( 1, 1 );

  QPixmap pix( r.size() );
  pix.fill( paletteBackgroundColor() );

  if ( color().isValid() )
    drawColor( pix, color() );
  else
  {
    QPainter p( &pix );
    p.drawRect( 2, 2, pix.width() - 4, pix.height() - 4 );
    p.fillRect( 3, 3, pix.width() - 6, pix.height() - 6, QBrush( Qt::BDiagPattern ) );
    p.end();
  }

  QPainter p( this );
  p.drawPixmap( r, pix );
  p.end();
*/
}

void QtxColorButton::drawButton( QPainter* painter )
{
  QPixmap base( size() );
  base.fill( paletteBackgroundColor() );
  QPainter bp( &base );

  QToolButton::drawButton( &bp );

  QRect r = style().visualRect( style().querySubControlMetrics( QStyle::CC_ToolButton, this, QStyle::SC_ToolButton ), this );

  int m = 2;
  r.setTopLeft( r.topLeft() + QPoint( m, m ) );
  r.setBottomRight( r.bottomRight() - QPoint( m, m ) );

  if ( isDown() )
    r.moveBy( 1, 1 );

  QPixmap pix( r.size() );
  pix.fill( paletteBackgroundColor() );

  if ( color().isValid() )
    drawColor( pix, color() );
  else
  {
    QPainter p( &pix );
    p.drawRect( 2, 2, pix.width() - 4, pix.height() - 4 );
    p.fillRect( 3, 3, pix.width() - 6, pix.height() - 6, QBrush( Qt::BDiagPattern ) );
    p.end();
  }

  bp.drawPixmap( r, pix );
  bp.end();

  painter->drawPixmap( 0, 0, base );
}

/*!
  \brief Update widget state.
*/
void QtxColorButton::updateState()
{
  QObjectList* lst = popup()->queryList( "QToolButton", 0, true, false );
  if ( !lst )
    return;

  for ( QObjectListIt cit( *lst ); cit.current(); ++cit )
  {
    QToolButton* tb = ::qt_cast<QToolButton*>( cit.current() );
    if ( tb )
      updateButton( tb );
  }
}

/*!
  \brief Update child button state.
  \param btn child button
*/
void QtxColorButton::updateButton( QToolButton* btn )
{
  if ( !btn )
    return;

  QColor c = color();
  bool block = btn->signalsBlocked();
  btn->blockSignals( true );
  btn->setOn( false );
  QColor clr;
  if ( myColors.contains( btn ) )
  {
    clr = myColors[btn];
    btn->setOn( clr == c );
  }
  if ( myColors.contains( btn ) || btn == this )
    btn->setIconSet( buttonIcon( clr ) );

  btn->setDown( false );
  btn->blockSignals( block );
}

/*!
  \brief Generate (if necessary) or get the icon for the button.
  \param c color to be used for the icon
  \return icon pixmap for the button
*/
QPixmap QtxColorButton::buttonIcon( const QColor& c ) const
{
  static QMap<int, QPixmap> pixMap;

  if ( pixMap.contains( c.rgb() ) )
    return pixMap[c.rgb()];

  QPixmap pix( 16, 16 );

  QColor bg = Qt::white;
  if ( bg == c )
    bg = Qt::gray;
  pix.fill( bg );

  drawColor( pix, c );

  pix.setMask( pix.createHeuristicMask() );

  pixMap.insert( c.rgb(), pix );

  return pix;
}

/*!
  \brief Draw pixmap.
  \param pd paint device
  \param c color
  \param m margin
*/
void QtxColorButton::drawColor( QPixmap& pix, const QColor& c, const int m ) const
{
  if ( !c.isValid() )
    return;

  QPainter p( &pix );
  p.setPen( Qt::black );
  p.fillRect( m, m, pix.width() - 2 * m - 1, pix.height() - 2 * m - 1, QBrush( c ) );
  p.drawRect( m, m, pix.width() - 2 * m - 1, pix.height() - 2 * m - 1 );
  p.end();
}

/*!
  \brief Get predefined list of colors to be used in the popup menu.
  \return list of colors
*/
QValueList<QColor> QtxColorButton::colorsList() const
{
  QValueList<QColor> lst;

  for ( int g = 0; g < 4; g++ )
  {
    for ( int r = 0; r < 4; r++ )
    {
      for ( int b = 0; b < 3; b++ )
        lst.append( QColor( qRgb( r * 255 / 3, g * 255 / 3, b * 255 / 2 ) ) );
    }
  }
  return lst;
}

/*!
  \fn void QtxColorButton::clicked( QColor color );
  \brief This signal is emitted when the widget button is clicked by 
         the user.
  \param color current color
*/

/*!
  \fn void QtxColorButton::changed( QColor color );
  \brief This signal is emitted when the current color is changed.
  \param color new current color
*/
