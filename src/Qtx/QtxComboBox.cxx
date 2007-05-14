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
// File:      QtxComboBox.cxx
// Author:    Sergey TELKOV

#include "QtxComboBox.h"

#include <QLineEdit>

/*!
  \class QtxComboBox
  \brief Enhanced version of Qt combo box class.
  \warning The implementation is not yet finished!

  In addition to the QComboBox class, QtxComboBox supports 
  adding/removing the items with the associated unique identifiers.
  It also provides a way to set "cleared" state to the combo box -
  when no item is selected.

  \todo Finalize the implementation: support adding/removing items by ID.
*/

/*!
  \brief Constructor.
  \param parent parent widget
*/
QtxComboBox::QtxComboBox( QWidget* parent )
: QComboBox( parent ),
  myCleared( false )
{
  connect( this, SIGNAL( activated( int ) ), this, SLOT( onActivated( int ) ) );
  connect( this, SIGNAL( activated( const QString& ) ), this, SLOT( onActivated( const QString& ) ) );
}

/*!
  \brief Destructor.

  Does nothing currently.
*/
QtxComboBox::~QtxComboBox()
{
}

/*!
  \brief Check if the combo box is in the "cleared" state.
  \return \c true if combobox is in the "cleared" state
*/
bool QtxComboBox::isCleared() const
{
  return myCleared;
}

/*!
  \brief Set "cleared" state.
  \param isClear new "cleared" state
*/
void QtxComboBox::setCleared( const bool isClear )
{
  if ( myCleared == isClear )
    return;
    
  myCleared = isClear;
    
  if ( isEditable() )
  {
    if ( myCleared )
      lineEdit()->setText( "" );
    else
      lineEdit()->setText( itemText( currentIndex() ) );
  }
    
  update();
}

/*!
  \brief Set current item.

  Does nothing if the item index is out of range.

  \param idx item index
*/
void QtxComboBox::setCurrentIndex( int idx )
{
  if ( idx < 0 || idx >= count() )
    return;
    
  myCleared = false;
  QComboBox::setCurrentIndex( idx );
}

/*!
  \brief Get current item ID.
  \return item id
*/
int QtxComboBox::currentId() const
{
  return id( currentIndex() );
}

/*!
  \brief Set current item by ID.
  \param num item ID
*/
void QtxComboBox::setCurrentId( int num )
{
  setCurrentIndex( index( num ) );
}

/*!
  \brief Customize paint event.
  \param e paint event
*/
void QtxComboBox::paintEvent( QPaintEvent* e )
{
  if ( !count() || !myCleared || isEditable() )
    QComboBox::paintEvent( e );
  else
    paintClear( e );
}

/*!
  \brief Called when any item is activated by the user.
  \param idx activated item index (not used)
*/
void QtxComboBox::onActivated( int /*idx*/ )
{
  resetClear();
}

/*!
  \brief Called when any item is activated by the user.
  \param txt activated item text (not used)
*/
void QtxComboBox::onActivated( const QString& /*txt*/ )
{
  resetClear();
}

/*!
  \brief Reset "cleared" state and update the combo box.
*/
void QtxComboBox::resetClear()
{
  if ( !myCleared )
    return;
  
  myCleared = false;
  update();
}

/*!
  \brief Draw combobox in the "cleared" state.
  \param e paint event
*/
void QtxComboBox::paintClear( QPaintEvent* e )
{
  int curIndex = currentIndex();
  QString curText = itemText( curIndex );
  QIcon curIcon = itemIcon( curIndex );
    
  bool upd = updatesEnabled();
  setUpdatesEnabled( false );
    
  setItemIcon( curIndex, QIcon() );
  setItemText( curIndex, QString::null );

  QComboBox::paintEvent( e );
    
  setItemText( curIndex, curText );
  setItemIcon( curIndex, curIcon );
    
  setUpdatesEnabled( upd );
}

/*!
  \brief Get item ID by the index.
  \param idx item index
  \return item ID or -1 if index is invalid.
*/
int QtxComboBox::id( const int idx ) const
{
  int id = -1;
  if ( myIndexId.contains( idx ) )
    id = myIndexId[idx];
  return id;
}

/*!
  \brief Get item index by the ID.
  \param id item ID
  \return item index or -1 if ID is invalid.
*/
int QtxComboBox::index( const int id ) const
{
  int idx = -1;
  for ( IndexIdMap::ConstIterator it = myIndexId.begin(); it != myIndexId.end() && idx == -1; ++it )
  {
    if ( it.value() == id )
      idx = it.key();
  }
  return idx;
}

/*!
  \fn void QtxComboBox::activatedId( int id )
  \brief Emitted when the item with identificator \a id is activated.
  \param id item ID
*/

/*!
  \fn void QtxComboBox::highlightedId( int id )
  \brief Emitted when the item with identificator \a id is highlighted.
  \param id item ID
*/
