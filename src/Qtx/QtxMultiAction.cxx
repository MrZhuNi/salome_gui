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
// File:      QtxMultiAction.cxx
// Author:    Sergey TELKOV
//

#include "QtxMultiAction.h"

#include <QMenu>
#include <QLayout>
#include <QToolBar>
#include <QPainter>
#include <QToolButton>
#include <QApplication>
#include <QStyleOptionButton>

/*!
  \class QtxMultiAction::Filter
  \brief Waches for the buttons in the popup menu 
  to update the tool buttons state.
  \internal
*/

class QtxMultiAction::Filter : public QObject
{
public:
  //! \brief Constructor
  Filter( QObject* parent ) : QObject( parent ) {}
  //! \brief Destructor
  ~Filter() {}
  //! \brief Process events from the child tool buttons
  bool eventFilter( QObject* o, QEvent* e )
  {
    if ( e->type() == QEvent::Leave ) {
      QToolButton* tb = qobject_cast<QToolButton*>( o );
      if ( tb )
	tb->setDown( false );
    }
    return QObject::eventFilter( o, e );
  }
};

/*!
  \class QtxMultiAction::Button
  \brief Custom button to be used in the toolbar.
  \internal
*/

class QtxMultiAction::Button : public QToolButton
{
public:
  //! \brief Constructor
  Button( QWidget* parent = 0 ) : QToolButton( parent ) {}
  //! \brief Destructor
  ~Button() {};

protected:
  //! \brief Paint the button
  virtual void paintEvent( QPaintEvent* e )
  {
    QToolButton::paintEvent( e );

    int s = 4;
    int m = 2;
    int w = width();
    int h = height();

    QStyleOptionButton opt;
    opt.initFrom( this );
    if ( isDown() )
      opt.rect = QRect( w - s - m, h - s - m, s, s );
    else
      opt.rect = QRect( w - s - m - 1, h - s - m - 1, s, s );

    QPainter p( this );
    style()->drawPrimitive( QStyle::PE_IndicatorArrowDown, &opt, &p );
  }
};

/*!
  \class QtxMultiAction
  \brief The class QtxMultiAction implements modifiable action.

  The QtxMultiAction class provides a possibility to assign a set of actions 
  (insertAction() function). The action can be used in the toolbar (and even
  in the menu) to show drop-down menu with the list of the assigned actions.

  Initially the first action from the list becomes current and it is activated
  when the tool button is clicked by the user. If user presses and holds the mouse
  button at the tool button, it shows the popup menu with all the assigned actions.
  When the user selects any action from the popup menu, it becames current.
*/

/*!
  \brief Constructor.
  \param parent parent object
*/
QtxMultiAction::QtxMultiAction( QObject* parent )
: QtxActionSet( parent ),
  myCurrent( 0 )
{
  setVisible( true );
  setMenu( new QMenu( 0 ) );

  connect( this, SIGNAL( triggered( QAction* ) ), this, SLOT( onTriggered( QAction* ) ) );
}

/*!
  \brief Constructor.
  \param txt action menu text
  \param parent parent object
*/
QtxMultiAction::QtxMultiAction( const QString& txt, QObject* parent )
: QtxActionSet( parent ),
  myCurrent( 0 )
{
  setText( txt );
  setVisible( true );
  setMenu( new QMenu( 0 ) );

  connect( this, SIGNAL( triggered( QAction* ) ), this, SLOT( onTriggered( QAction* ) ) );
}

/*!
  \brief Constructor.
  \param ico action menu icon
  \param txt action menu text
  \param parent parent object
*/
QtxMultiAction::QtxMultiAction( const QIcon& ico, const QString& txt, QObject* parent )
: QtxActionSet( parent ),
  myCurrent( 0 )
{
  setIcon( ico );
  setText( txt );
  setVisible( true );
  setMenu( new QMenu( 0 ) );

  connect( this, SIGNAL( triggered( QAction* ) ), this, SLOT( onTriggered( QAction* ) ) );
}

/*!
  \brief Destructor
*/
QtxMultiAction::~QtxMultiAction()
{
}

/*!
  \brief Called when the user activates the current action 
  (for example by clicking the tool button).
  \param on (not used)
*/
void QtxMultiAction::onClicked( bool /*on*/ )
{
  if ( myCurrent )
    myCurrent->activate( QAction::Trigger );
}

/*!
  \brief Called when user activates any action from the
  dropdown menu.
  \param a action being activated
*/
void QtxMultiAction::onTriggered( QAction* a )
{
  if ( !a )
    return;

  QList<QWidget*> lst = createdWidgets();
  for ( QList<QWidget*>::iterator it = lst.begin(); it != lst.end(); ++it )
  {
    QToolButton* tb = ::qobject_cast<QToolButton*>( *it );
    if ( tb && tb->menu() )
      tb->menu()->hide();
  }

  if ( myCurrent != a )
  {
    myCurrent = a;
    updateAction();
  }
}

/*!
  \brief Update action.
*/
void QtxMultiAction::updateAction()
{
  QtxActionSet::updateAction();

  QList<QWidget*> lst = createdWidgets();
  for ( QList<QWidget*>::iterator it = lst.begin(); it != lst.end(); ++it )
    updateButton( ::qobject_cast<QToolButton*>( *it ) );
}

/*!
  \brief Update child (popup menu) action.
  \param w widget menu widget
*/
void QtxMultiAction::updateAction( QWidget* w )
{
  if ( !w )
    return;

  if ( w->inherits( "QMenu" ) )
  {
    QtxActionSet::updateAction( menu() );

    QApplication::instance()->removeEventFilter( this );

    menu()->removeAction( this );

    QApplication::instance()->installEventFilter( this );
  }
}

/*!
  \brief Check if the action itself should be invisible
  (only child action are shown)
  \return \c true if the action itself should be visible
*/
bool QtxMultiAction::isEmptyAction() const
{
  return false;
}

/*!
  \brief Create widget to be displayed in the toolbar.
  \param parent parent widget (should be toolbar)
  \return toolbar button
*/
QWidget* QtxMultiAction::createWidget( QWidget* parent )
{
  QToolBar* tb = ::qobject_cast<QToolBar*>( parent );
  if ( !tb )
    return 0;

  QToolButton* w = new Button( tb );
  w->setMenu( new QMenu( w ) );
  w->setFocusPolicy( Qt::NoFocus );
  w->setIconSize( tb->iconSize() );
  w->setToolButtonStyle( tb->toolButtonStyle() );

  connect( w, SIGNAL( clicked( bool ) ), this, SLOT( onClicked( bool ) ) );
  connect( tb, SIGNAL( iconSizeChanged( const QSize& ) ), w, SLOT( setIconSize( QSize ) ) );
  connect( tb, SIGNAL( toolButtonStyleChanged( Qt::ToolButtonStyle ) ),
           w, SLOT( setToolButtonStyle( Qt::ToolButtonStyle ) ) );

  updateButton( w );
  return w;
}

/*!
  \brief Called when the child action is added to this action.
  \param a child action being added
*/
void QtxMultiAction::actionAdded( QAction* a )
{
  if ( !myCurrent )
    myCurrent = a;
}

/*!
  \brief Called when the child action is removed from this action.
  \param a child action being removed
*/
void QtxMultiAction::actionRemoved( QAction* a )
{
  if ( myCurrent != a )
    return;

  myCurrent = actions().isEmpty() ? 0 : actions().first();

  updateAction();
}

/*!
  \brief Update toolbar button.
  \param btn toolbar button
*/
void QtxMultiAction::updateButton( QToolButton* btn )
{
  if ( !btn )
    return;

  btn->setIcon( myCurrent ? myCurrent->icon() : QIcon() );
  btn->setText( myCurrent ? myCurrent->text() : QString() );
  btn->setToolTip( myCurrent ? myCurrent->toolTip() : QString() );
  btn->setStatusTip( myCurrent ? myCurrent->statusTip() : QString() );

  QMenu* pm = btn->menu();
  if ( !pm )
    return;

  pm->clear();
  for ( int i = 0; pm->layout() && i < pm->layout()->count(); i++ )
    delete pm->layout()->widget();

  delete pm->layout();

  QVBoxLayout* vbox = new QVBoxLayout( pm );
  vbox->setMargin( 1 );
  vbox->setSpacing( 0 );
  Filter* filter = new Filter( vbox );
  QList<QAction*> actList = actions();
  for ( QList<QAction*>::iterator itr = actList.begin(); itr != actList.end(); ++itr )
  {
    QToolButton* b = new QToolButton( pm );
    b->setDefaultAction( *itr );
    b->setToolTip( (*itr)->toolTip() );
    b->setAutoRaise( true );
    b->setIconSize( btn->iconSize() );
    b->setToolButtonStyle( btn->toolButtonStyle() );
    b->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    b->installEventFilter( filter );
    vbox->addWidget( b );
  }
}
