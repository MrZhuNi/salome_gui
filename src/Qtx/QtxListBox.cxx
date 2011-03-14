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
// File:      QtxListBox.cxx
// Author:    Sergey TELKOV

#include "QtxListBox.h"

#include <QPixmap>
#include <QLineEdit>

#ifdef WIN32
#pragma warning ( disable: 4251 )
#endif

#include <QEvent>
#include <QScrollBar>
#include <QKeyEvent>

/*!
  Constructor
*/
QtxListBox::QtxListBox( QWidget* parent )
: QListWidget( parent ),
myEditor( 0 ),
myEditIndex( -1 ),
myEditState( false ),
myEditDefault( true ),
myModifEnabled( true )
{
  QScrollBar* hsb = horizontalScrollBar();
  if ( hsb )
    connect( hsb, SIGNAL( valueChanged( int ) ), SLOT( onContentsMoving() ) );
  
  QScrollBar* vsb = horizontalScrollBar();
  if ( vsb )
    connect( vsb, SIGNAL( valueChanged( int ) ), SLOT( onContentsMoving() ) );
}

/*!
  Destructor
*/
QtxListBox::~QtxListBox()
{
}

/*!
  \return true if editing is enabled
*/
bool QtxListBox::isEditEnabled() const
{
  return myEditState;
}

/*!
  Enables/disables editing
  \param on - new state
*/
void QtxListBox::setEditEnabled( bool on )
{
  if ( isEditEnabled() == on )
    return;

  myEditState = on;

  if ( !isEditEnabled() )
    endEdition( defaultEditAction() );
}

/*!
  \return default edit action
  \sa setDefaultEditAction()
*/
bool QtxListBox::defaultEditAction() const
{
  return myEditDefault;
}

/*!
  Changes default edit action. 
  Pressing of ENTER button always accepts new value of edited item.
  But other ways, such as focus out or edition of other item accepts
  new value of edited item only if "default edit action" is true
  \param def - new value
*/
void QtxListBox::setDefaultEditAction( bool def )
{
  myEditDefault = def;
}

/*!
  \return modification enabled state
  \sa setModificationEnabled()
*/
bool QtxListBox::isModificationEnabled() const
{
  return myModifEnabled;
}

/*!
  Changes "modification enabled" state
  If it is true, then pressing of CTRL + { Up, Down, Home, End } allows move items in list
  \param on - new state
*/
void QtxListBox::setModificationEnabled( bool on )
{
  myModifEnabled = on;
}

/*!
  \return current edited item
*/
QListWidgetItem* QtxListBox::editedItem() const
{
  return item( editedIndex() );
}

/*!
  \return current edited index
*/
int QtxListBox::editedIndex() const
{
  return myEditIndex;
}

/*!
  Starts edition of item
  \param idx - index of item
*/
void QtxListBox::startEdition( const int idx )
{
  if ( idx < 0 || editedIndex() == idx || !isEditEnabled() )
    return;

  QLineEdit* ed = editor();
  if ( !ed )
    return;

  endEdition( defaultEditAction() );

  myEditIndex = idx;

  ensureItemVisible( myEditIndex );

  ed->setText( item( myEditIndex )->text() );
  updateEditor();
  ed->show();

  ed->setFocus();
}

/*!
  Starts edition of item
  \param item - item to be edited
*/
void QtxListBox::startEdition( const QListWidgetItem* item )
{
  startEdition( row( item ) );
}

/*!
  Finishes edition of item
  \param action - if it is true, then new values must be accepted
*/
void QtxListBox::endEdition( const bool action )
{
  int idx = editedIndex();
  QLineEdit* ed = editor();

  if ( idx < 0 || !ed )
    return;

  myEditIndex = -1;

  ed->hide();

  if ( action )
  {
    int cur = currentRow();

    /*qt4
    if ( pixmap( idx ) )
      changeItem( *pixmap( idx ), ed->text(), idx );
    else
      changeItem( ed->text(), idx );
    */
    item( idx )->setText( ed->text() );

    setCurrentRow( cur );

    emit itemEdited( idx );
    emit itemEdited( item( idx ) );
  }
}

/*!
  Ensures that the item is visible.
  \param idx - index of item
*/
void QtxListBox::ensureItemVisible( const int idx )
{
  if ( idx < 0 )
    return;

  //qt4 if ( itemVisible( idx ) )
  //qt4  return;

  //qt4 setTopItem( idx );
}

/*!
  Ensures that the item is visible.
  \param item - item to be made visible
*/
void QtxListBox::ensureItemVisible( const QListWidgetItem* item )
{
  ensureItemVisible( row( item ) );
}

/*!
  \return validator of item editor
*/
const QValidator* QtxListBox::validator() const
{
  const QValidator* v = 0;
  if ( editor() )
    v = editor()->validator();
  return v;
}

/*!
  Removes validator of item editor
*/
void QtxListBox::clearValidator()
{
  if ( editor() )
    editor()->setValidator( 0 );
}

/*!
  Changes validator of item editor
  \param v - new validator
*/
void QtxListBox::setValidator( const QValidator* v )
{
  if ( editor() )
    editor()->setValidator( v );
}

/*!
  Moves item to top
  \param idx - index of item
*/
void QtxListBox::moveItemToTop( const int idx )
{
  moveItem( idx, -idx );
}

/*!
  Moves item to bottom
  \param idx - index of item
*/
void QtxListBox::moveItemToBottom( const int idx )
{
  moveItem( idx, count() - idx );
}

/*!
  Moves item
  \param idx - index of item
  \param step - changing of position
*/
void QtxListBox::moveItem( const int idx, const int step )
{
  QListWidgetItem* i = item( idx );
  if ( !i || step == 0 )
    return;

  QListWidgetItem* cur = currentItem();

  takeItem( idx );
  insertItem( qMax( 0, idx + step ), i );

  setCurrentItem( cur );

  int pos = row( i );
  if ( myEditIndex == idx )
    myEditIndex = pos;

  updateEditor();

  if ( idx != pos )
    emit itemMoved( idx, pos );
}

/*!
  Inserts empty item
  \param i - position of item (if it is less than 0, then current position is used)
*/
void QtxListBox::createItem( const int i )
{
  if ( !isEditEnabled() )
    return;

  int idx = i < 0 ? currentRow() : i;
  idx = idx < 0 ? count() : idx;
  idx = qMin( (int)count(), idx );

  insertItem( idx, "" );
  setCurrentRow( idx );
  startEdition( idx );
}

/*!
  Removes item
  \param i - position of item (if it is less than 0, then current position is used)
*/
void QtxListBox::deleteItem( const int i )
{
  if ( !isEditEnabled() )
    return;

  int idx = i < 0 ? currentRow() : i;
  if ( idx < 0 )
    return;

  if ( editedIndex() == idx )
    endEdition( defaultEditAction() );

  takeItem( idx );
  updateEditor();
}

/*!
  Scrolls the content so that the point is in the top-left corner.
  \param x, y - point co-ordinates
*/
void QtxListBox::setContentsPos( int x, int y )
{
  //qt4 QListWidget::setContentsPos( x, y );

  updateEditor();
}

/*!
  Custom event filter, performs finish of edition on focus out, escape/return/enter pressing
*/
bool QtxListBox::eventFilter( QObject* o, QEvent* e )
{
  if ( editor() == o )
  {
    if ( e->type() == QEvent::FocusOut )
      endEdition( defaultEditAction() );

    if ( e->type() == QEvent::KeyPress )
    {
      QKeyEvent* ke = (QKeyEvent*)e;
      if ( ke->key() == Qt::Key_Escape )
        endEdition( false );
      else if ( ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return )
        endEdition( true );
    }
  }

  return QListWidget::eventFilter( o, e );
}

/*!
  Custom key press event handler
  Allows to move items by CTRL + { Up, Down, Home, End }
*/
void QtxListBox::keyPressEvent( QKeyEvent* e )
{
  if ( e->key() == Qt::Key_Up && e->modifiers() & Qt::ControlModifier && isModificationEnabled() )
    moveItem( currentRow(), -1 );
  else if ( e->key() == Qt::Key_Down && e->modifiers() & Qt::ControlModifier && isModificationEnabled() )
    moveItem( currentRow(), 1 );
  else if ( e->key() == Qt::Key_Home && e->modifiers() & Qt::ControlModifier && isModificationEnabled() )
    moveItemToTop( currentRow() );
  else if ( e->key() == Qt::Key_End && e->modifiers() & Qt::ControlModifier && isModificationEnabled() )
    moveItemToBottom( currentRow() );
  else if ( e->key() == Qt::Key_Insert && e->modifiers() & Qt::ControlModifier )
    createItem( currentRow() );
  else if ( e->key() == Qt::Key_Delete && e->modifiers() & Qt::ControlModifier )
    deleteItem( currentRow() );
  else
    QListWidget::keyPressEvent( e );
}

/*!
  Custom resize event handler
*/
void QtxListBox::resizeEvent( QResizeEvent* e )
{
  QListWidget::resizeEvent( e );

  updateEditor();
}

/*!
  Custom mouse double click event handler
*/
void QtxListBox::mouseDoubleClickEvent( QMouseEvent* e )
{
  if ( isEditEnabled() )
    startEdition( itemAt( e->pos() ) );
  else
    QListWidget::mouseDoubleClickEvent( e );
}

/*!
  Updates editor on contents moving
*/
void QtxListBox::onContentsMoving()
{
  updateEditor();
}

/*!
  \return item editor
*/
QLineEdit* QtxListBox::editor() const
{
  if ( !myEditor )
  {
    QtxListBox* that = (QtxListBox*)this;
    that->createEditor();
  }
  return myEditor;
}

/*!
  Creates item editor
*/
void QtxListBox::createEditor()
{
  if ( myEditor )
    return;

  myEditor = new QLineEdit( viewport() );
  myEditor->setBackgroundRole( QPalette::Window );

  //qt4 myEditor->setLineWidth( 1 );
  //qt4 myEditor->setMidLineWidth( 0 );
  //qt4 myEditor->setFrameStyle( QFrame::Box | QFrame::Plain );
  myEditor->installEventFilter( this );

  myEditor->hide();

  //qt4 addChild( myEditor );
}

/*!
  Updates item editor
*/
void QtxListBox::updateEditor()
{
  if ( !editedItem() || !editor() )
    return;

  QRect r = visualItemRect( editedItem() );
  if ( !r.isValid() )
    return;

  //int m = /*editor()->lineWidth()*/ 1;
  r.setLeft( r.left() + 1 );
  
  QListWidgetItem* it = item( editedIndex() );
  if ( it )
  {
    const QIcon pix = it->icon();
    if ( !pix.isNull() )
    {
      QSize s = pix.actualSize( QSize( 100, 100 ) );
      r.setLeft( s.width() + 2 );
    }
  }

  editor()->setGeometry( r );
}
