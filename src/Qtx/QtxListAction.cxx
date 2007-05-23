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
// File:      QtxListAction.cxx
// Author:    Sergey TELKOV

#include "QtxListAction.h"

#include <QMenu>
#include <QLabel>
#include <QLayout>
#include <QToolTip>
#include <QToolbar>
#include <QWMatrix>
#include <QMouseEvent>
#include <QListWidget>
#include <QToolButton>
#include <QObjectList>
#include <QApplication>

/*!
  \class QtxListAction::ScrollEvent
  Event for the scrolling in the list of actions
*/
class QtxListAction::ScrollEvent : public QEvent
{
public:
  enum { Scroll = User + 1 };

  ScrollEvent( bool down ) : QEvent( (QEvent::Type)Scroll ), myDown( down ) {}
  virtual ~ScrollEvent() {}

  bool isDown() const { return myDown; }

private:
  bool myDown;
};

/*!
  \class QtxListAction::ListWidget
  List of actions
*/
class QtxListAction::ListWidget : public QListWidget
{
public:
  ListWidget( QWidget* parent = 0 ) : QListWidget( parent ) {}
  virtual ~ListWidget() {}

protected:
  virtual void scrollContentsBy( int dx, int dy )
  {
    QListWidget::scrollContentsBy( dx, dy );
    if ( dy != 0 )
      QApplication::postEvent( viewport(), new ScrollEvent( dy <= 0 ) );
  }
};

/*!
  \class QtxListAction::ListFrame
  Expanding frame with action list and comment
*/
class QtxListAction::ListFrame: public QMenu
{
public:
  ListFrame( QtxListAction*, QWidget* parent );
  virtual ~ListFrame();

  void                    clear();
  const QStringList       names() const;
  void                    addNames( const QStringList& );

  void                    setSingleComment( const QString& );
  void                    setMultipleComment( const QString& );

  int                     selected() const;
  void                    setSelected( const int );

  int                     linesNumber() const;
  int                     charsNumber() const;

  void                    setLinesNumber( const int );
  void                    setCharsNumber( const int );

  virtual QSize           sizeHint() const;
  virtual QSize           minimumSizeHint() const;

  virtual bool            eventFilter( QObject*, QEvent* );

  virtual void            setVisible( bool );

protected:
  virtual void            keyPressEvent( QKeyEvent* );

private:
  void                    accept();
  void                    updateComment();
  void                    setNames( const QStringList& );
  void                    removePostedEvens( QObject*, int );

private:
  QListWidget*            myList;
  QStringList             myNames;
  QtxListAction*          myAction;
  QLabel*                 myComment;

  int                     myLines;
  int                     myChars;

  QString                 mySingleComment;
  QString                 myMultipleComment;
};

/*!
  Constructor
*/
QtxListAction::ListFrame::ListFrame( QtxListAction* a, QWidget* parent )
: QMenu( parent ),
myList( 0 ),
myLines( 5 ),
myChars( 5 ),
myAction( a ),
myComment( 0 )
{
  QVBoxLayout* top = new QVBoxLayout( this );
  top->setMargin( 0 );
  QFrame* main = new QFrame( this );
  main->setFrameStyle( QFrame::Panel | QFrame::Raised );
  top->addWidget( main );

  QVBoxLayout* base = new QVBoxLayout( main );
  base->setMargin( 3 );
  base->setSpacing( 2 );

  myList = new ListWidget( main );
  myList->setSelectionMode( QListWidget::MultiSelection );
  myList->setVerticalScrollMode( QListWidget::ScrollPerItem );
  myList->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
  myList->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  myList->viewport()->installEventFilter( this );
  myList->viewport()->setMouseTracking( true );
  myList->setFocusPolicy( Qt::NoFocus );

  myComment = new QLabel( main );
  myComment->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  myComment->setAlignment( Qt::AlignCenter );
  myMultipleComment = "%1";

  base->addWidget( myList );
  base->addWidget( myComment );
}

/*!
  Destructor
*/
QtxListAction::ListFrame::~ListFrame()
{
}

/*!
  Clears list of names [ public ]
*/
void QtxListAction::ListFrame::clear()
{
	myNames.clear();
	setNames( myNames );
}

/*!
  Adds a names to the list. Truncates the name to fit to the frame width.
  Use QtxListAction::setCharsNumber( int ) to set the width in characters. [ public ]
*/
void QtxListAction::ListFrame::addNames( const QStringList& names )
{
	for ( QStringList::ConstIterator it = names.begin(); it != names.end(); ++it )
		myNames.append( *it );
	setNames( myNames );
}

/*!
  Sets a names to the list. Truncates the name to fit to the frame width.
  Use QtxListAction::setCharsNumber( int ) to set the width in characters. [ public ]
*/
void QtxListAction::ListFrame::setNames( const QStringList& names )
{
  if ( !myList )
		return;

	myList->clear();
  QStringList strList;
	for ( QStringList::const_iterator it = names.begin(); it != names.end(); ++it )
	{
		QString s = *it;
    QFontMetrics fm = myList->fontMetrics();
    int maxW = charsNumber() * fm.maxWidth();
    int w = fm.width( s );
    if ( w > maxW )
    {
      QString extra( "..." );
      int len = s.length();
      int extraLen = fm.width( extra ) + 1;
      while ( true )
      {
        w = fm.width( s, --len );
        if ( w + extraLen < maxW )
        {
          s = s.left( len );
          break;
        }
      }
      s += extra;
    }
    strList.append( s );
  }
  myList->addItems( strList );
}

/*!
  \return list of names
*/
const QStringList QtxListAction::ListFrame::names() const
{
	return myNames;
}

/*!
    Returns max number of lines shown without activation of vertical scroll bar. [ public ]
*/
int QtxListAction::ListFrame::linesNumber() const
{
  return myLines;
}

/*!
    Returns max number of chars in line (the rest will be truncated). [ public ]
*/
int QtxListAction::ListFrame::charsNumber() const
{
  return myChars;
}

/*!
    Sets max number of lines shown without activation of vertical scroll bar. [ public ]
*/
void QtxListAction::ListFrame::setLinesNumber( const int maxLines )
{
  myLines = maxLines;
}

/*!
    Sets max number of chars in line ( the rest will be truncated ). [ public ]
*/
void QtxListAction::ListFrame::setCharsNumber( const int maxChars )
{
	if ( myChars == maxChars )
		return;

  myChars = maxChars;
	setNames( myNames );
}

/*!
    Sets the format of single comment. [ public ]
*/
void QtxListAction::ListFrame::setSingleComment( const QString& comment )
{
  mySingleComment = comment;
	setNames( myNames );
  updateComment();
}

/*!
    Sets the format of multiple comment. [ public ]
*/
void QtxListAction::ListFrame::setMultipleComment( const QString& comment )
{
  myMultipleComment = comment;
	setNames( myNames );
  updateComment();
}

/*!
    Updates comment display. [ public ]
*/
void QtxListAction::ListFrame::updateComment()
{
	QString com;
	int selNum = selected();
	if ( selNum > 1 )
		com = myMultipleComment;
	else if ( selNum > 0 && !mySingleComment.isEmpty() )
		com = mySingleComment;

	if ( !com.isEmpty() )
		com = com.arg( selNum );

  myComment->setText( com );
}

QSize QtxListAction::ListFrame::sizeHint() const
{
  return QSize( myList->fontMetrics().maxWidth() * charsNumber() + 10,
                qMax( 1, linesNumber() ) * ( myList->fontMetrics().height() + 2 ) +
                myComment->sizeHint().height() );
}

QSize QtxListAction::ListFrame::minimumSizeHint() const
{
  return QSize( myList->fontMetrics().maxWidth() * charsNumber() + 10,
                qMax( 1, linesNumber() ) * ( myList->fontMetrics().height() + 2 ) +
                myComment->sizeHint().height() );
}

/*!
  Validates the action. [ private slot ]
*/
void QtxListAction::ListFrame::accept()
{
  int sel = selected();
  if ( sel && myAction )
    myAction->onMultiple( sel );
}

/*!
  Initializes / shows the frame. [ virtual public slot ]
*/
void QtxListAction::ListFrame::setVisible( bool on )
{
  if ( on )
  {
    myList->setFocus();
    myList->scrollToItem( myList->item( 0 ), QListWidget::PositionAtTop );
    setSelected( 0 );
    updateComment();
  }

  QMenu::setVisible( on );
}

/*!
    Processes KeyUp/KeyDown, PageUp/PageDown, CR and Esc keys.
    Returns 'true' if event is eaten, 'false' otherwise. [ private ]
*/
void QtxListAction::ListFrame::keyPressEvent( QKeyEvent* e )
{
  if ( e->type() == QEvent::KeyRelease )
    return;

  e->accept();

  int selNum = selected();
  switch( e->key() )
  {
  case Qt::Key_Up:
    setSelected( qMax( 1, selNum - 1 ) );
    break;
  case Qt::Key_Down:
    setSelected( qMax( 1, selNum + 1 ) );
    break;
  case Qt::Key_PageUp:
    setSelected( qMax( 1, selNum - linesNumber() ) );
    break;
  case Qt::Key_PageDown:
	  setSelected( selNum += linesNumber() );
    break;
  case Qt::Key_Home:
	  setSelected( 1 );
		break;
  case Qt::Key_End:
	  setSelected( myList->count() );
		break;
  case Qt::Key_Return:
    accept();
    break;
  case Qt::Key_Escape:
    hide();
    break;
  }
}

/*!
  Watches mouse events on the viewport of the list. [virtual public]
*/
bool QtxListAction::ListFrame::eventFilter( QObject* o, QEvent* e )
{
  bool res = true;

  switch( e->type() )
  {
  case QEvent::MouseMove:
    {
      QMouseEvent* me = (QMouseEvent*)e;
      if ( !myList->viewport()->rect().contains( me->pos() ) )
        setSelected( 0 );
      else if ( myList->itemAt( me->pos() ) )
        setSelected( myList->row( myList->itemAt( me->pos() ) ) + 1 );
    }
    break;
  case QEvent::MouseButtonRelease:
    accept();
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonDblClick:
    break;
  case ScrollEvent::Scroll:
    {
      ScrollEvent* se = (ScrollEvent*)e;
      QPoint pos = myList->viewport()->mapFromGlobal( QCursor::pos() );
      if ( myList->viewport()->rect().contains( pos ) )
      {
        if ( myList->itemAt( pos ) )
          setSelected( myList->row( myList->itemAt( pos ) ) + 1 );
      }
      else if ( se->isDown() )
        setSelected( myList->row( myList->itemAt( myList->viewport()->rect().bottomLeft() -
                                                  QPoint( 0, myList->fontMetrics().height() / 2 ) ) ) + 1 );
      else
        setSelected( myList->row( myList->itemAt( myList->viewport()->rect().topLeft() +
                                                  QPoint( 0, myList->fontMetrics().height() / 2 ) ) ) + 1 );
    }
    break;
  default:
    res = false;
    break;
  }

  if ( res )
    return true;
  else
    return QMenu::eventFilter( o, e );
}

/*!
  return number of selected items
*/
int QtxListAction::ListFrame::selected() const
{
	int sel = 0;
  QModelIndexList indexes = myList->selectionModel()->selectedRows();
  for ( QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); ++it )
    sel = qMax( sel, (*it).row() + 1 );

	return sel;
}

/*!
    Selects the actions [ 0 - lastSel ]. [ public ]
*/
void QtxListAction::ListFrame::setSelected( const int lastSel )
{
	int last = qMin( lastSel, (int)myList->count() );

  QItemSelection selection;
  QItemSelectionModel* selModel = myList->selectionModel();

	for ( int i = 0; i < last; i++ )
    selection.select( selModel->model()->index( i, 0 ), selModel->model()->index( i, 0 ) );

  selModel->select( selection, QItemSelectionModel::ClearAndSelect );

	int item = last - 1;

  myList->scrollToItem( myList->item( item ) );
  myList->clearFocus();

  removePostedEvens( myList->viewport(), ScrollEvent::Scroll );

  updateComment();
}

void QtxListAction::ListFrame::removePostedEvens( QObject* o, int type )
{
  class Filter : public QObject
  {
  public:
    Filter() : QObject( 0 ) {}
    virtual bool eventFilter( QObject*, QEvent* )
    {
      return true;
    }
  };

  Filter f;
  o->installEventFilter( &f );
  QApplication::sendPostedEvents( o, type );
}

/*!
  \class QtxListAction
  Action with list of items
*/

/*!
  Constructs an list action with given parent and name. If toggle is true the
  action will be a toggle action, otherwise it will be a command action.
*/
QtxListAction::QtxListAction( QObject* parent )
: QWidgetAction( parent ),
myFrame( 0 )
{
  initialize();
}

/*!
  This constructor creates an action with the following properties: the
  description text, the icon or iconset icon, the menu text and keyboard
  accelerator. It is a child of given parent and named specified name.
  If toggle is true the action will be a toggle action, otherwise it will
  be a command action.
*/
QtxListAction::QtxListAction( const QIcon& icon, const QString& text, int accel, QObject* parent )
: QWidgetAction( parent ),
myFrame( 0 )
{
  setIcon( icon );
  setText( text );
  setShortcut( accel );

  initialize();
}

/*!
  This constructor creates an action with the following properties: the
  description text, the menu text and keyboard accelerator. It is a child
  of given parent and named specified name. If toggle is true the action
  will be a toggle action, otherwise it will be a command action.
*/
QtxListAction::QtxListAction( const QString& text, int accel, QObject* parent )
: QWidgetAction( parent ),
myFrame( 0 )
{
  setText( text );
  setShortcut( accel );

  initialize();
}

/*!
  Destructor.
*/
QtxListAction::~QtxListAction()
{
  delete myFrame;
  myFrame = 0;
}

/*!
	Name: popupMode [public]
	Desc: Returns popup mode. If popup mode "Item" (default) then action will
	      be added into popup menu as menu item. If popup mode "SubMenu" then
		    action will be added into popup menu as sub menu with list of items.
*/
int QtxListAction::popupMode() const
{
  return menu() ? SubMenu : Item;
}

/*!
	Name: setPopupMode [public]
	Desc: Set the popup mode. Popup mode define way in this action will be
	      added into popup menu. This function should be used before addTo.
*/
void QtxListAction::setPopupMode( const int mode )
{
  if ( mode == popupMode() )
    return;

  if ( mode == Item )
  {
    delete menu();
    setMenu( 0 );
  }
  else
    setMenu( new QMenu( 0 ) );

  onChanged();
}

/*!
  \return list of names
*/
QStringList QtxListAction::names() const
{
  QStringList lst;
  if ( myFrame )
    lst = myFrame->names();
  return lst;
}

/*!
	Name: addNames [public]
	Desc: Fills the list of actions. Removes the old contents from
	      the list if 'clear' is true.
*/
void QtxListAction::addNames( const QStringList& names, bool clear )
{
  if ( !myFrame )
    return;

  if ( clear )
    myFrame->clear();

	myFrame->addNames( names );

  onChanged();
}

/*!
  Returns the number of lines wich will be visible in list frame.
*/
int QtxListAction::linesNumber() const
{
  return myFrame->linesNumber();
}

/*!
  Returns the number of chars wich will be visible in the each line.
*/
int QtxListAction::charsNumber() const
{
  return myFrame->charsNumber();
}

/*!
	Sets max number of lines that list frame shows without vertical scroll bar. Default value is 7.
*/
void QtxListAction::setLinesNumber( const int nlines )
{
  myFrame->setLinesNumber( nlines );
}

/*!
	Sets max number of characters in a line which list frame shows without truncation.
  Default value is 5 (the widest char size is used).
*/

void QtxListAction::setCharsNumber( const int nchars )
{
  myFrame->setCharsNumber( nchars );
}

/*!
	Name: setComment [public]
	Desc: Sets the format Qt string for comments displayed under the list
	      of actions for one action and for several actions.
		    Ex. "Undo %1 actions" format string will work as "Undo 3 actions"
		    when 3 actions are selected. The default format string is "%1".
*/
void QtxListAction::setComment( const QString& c, const QString& sc )
{
  if ( !myFrame )
    return;

  myFrame->setSingleComment( sc.isEmpty() ? c : sc );
	myFrame->setMultipleComment( c );
}

QWidget* QtxListAction::createWidget( QWidget* parent )
{
  if ( parent && parent->inherits( "QMenu" ) )
    return 0;

  QToolButton* tb = new QToolButton( parent );
  tb->setText( text() );
  tb->setIcon( icon() );
  tb->setPopupMode( QToolButton::MenuButtonPopup );
  tb->setMenu( myFrame );
  tb->setEnabled( isEnabled() && !names().isEmpty() );
  tb->setToolTip( toolTip() );
  connect( tb, SIGNAL( clicked( bool ) ), this, SLOT( onSingle( bool ) ) );

  return tb;
}

void QtxListAction::deleteWidget( QWidget* widget )
{
  delete widget;
}

/*!
	Name: initialize [private]
	Desc: Initialization of object QtxListAction.
*/

void QtxListAction::initialize()
{
  setPopupMode( Item );

	myFrame = new QtxListAction::ListFrame( this, 0 );
  myFrame->setLinesNumber( 7 );
  myFrame->setCharsNumber( 5 );

	myFrame->hide();

  connect( this, SIGNAL( changed() ), this, SLOT( onChanged() ) );
  connect( this, SIGNAL( triggered( bool ) ), this, SLOT( onSingle( bool ) ) );
}

void QtxListAction::onChanged()
{
	QStringList lst = myFrame->names();

  if ( menu() )
  {
    menu()->clear();
    for ( QStringList::iterator iter = lst.begin(); iter != lst.end(); ++iter )
    {
      QAction* a = new QAction( *iter, menu() );
      menu()->addAction( a );
      connect( a, SIGNAL( triggered( bool ) ), this, SLOT( onTriggered( bool ) ) );
    }
  }

  QList<QWidget*> widList = createdWidgets();
	for ( QList<QWidget*>::iterator it = widList.begin(); it != widList.end(); ++it )
  {
		(*it)->setEnabled( isEnabled() && !lst.isEmpty() );
    QToolButton* tb = ::qobject_cast<QToolButton*>( *it );
    if ( tb )
    {
      tb->setText( text() );
      tb->setIcon( icon() );
      tb->setToolTip( toolTip() );
    }
  }
}

/*!
	Name: onSingle [private slot]
	Desc: Called when a single action is selected.
*/

void QtxListAction::onSingle( bool )
{
  onMultiple( 1 );
}

/*!
	Name: onMultiple [private slot]
	Desc: Called when multiple actions are selected.
*/

void QtxListAction::onMultiple( const int numActions )
{
  if ( myFrame )
    myFrame->hide();

  if ( numActions > 0 )
    emit activated( numActions );
}

/*!
	Name: onTriggered [private slot]
	Desc: Called when a sub menu item is activated.
*/

void QtxListAction::onTriggered( bool )
{
  if ( !menu() )
    return;

  QList<QAction*> actionList = menu()->actions();
  int idx = actionList.indexOf( ::qobject_cast<QAction*>( sender() ) );
  if ( idx < 0 )
    return;

  emit activated( idx + 1 );
}
