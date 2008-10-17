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
#include "OCCViewer_CreateRestoreViewDlg.h"
#include "OCCViewer_ViewModel.h"
#include "OCCViewer_ViewPort3d.h"

#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QEvent>
#include <QKeyEvent>

/*!
  Constructor
*/
OCCViewer_CreateRestoreViewDlg::OCCViewer_CreateRestoreViewDlg( QWidget* aWin, OCCViewer_Viewer* curModel )
: QDialog( aWin )
{
  setWindowTitle( tr( "CAPTION" ) );

  myParametersMap = curModel->getViewAspects();

  myKeyFlag = 0;

  int aQuantityOfItems = myParametersMap.count();
	
  setFixedSize( 400, 300 );

  QGridLayout* aGrid = new QGridLayout( this );
  aGrid->setMargin( 5 );
  aGrid->setSpacing( 10 );

  QWidget* aWidget1 = new QWidget( this );
  QWidget* aWidget2 = new QWidget( this );
	
  QHBoxLayout* aLayout = new QHBoxLayout( aWidget1 );
	
  myListBox = new QListWidget( aWidget1 );
  myListBox->installEventFilter( this );

  myCurViewPort = new OCCViewer_ViewPort3d( aWidget1, curModel->getViewer3d(), V3d_ORTHOGRAPHIC );
  myCurViewPort->getView()->SetBackgroundColor( Quantity_NOC_BLACK );

  myListBox->setEditTriggers( QAbstractItemView::DoubleClicked );
	
  if ( aQuantityOfItems )
  {
    myListBox->clear();
    for( int i = 0; i < aQuantityOfItems; i++ ) {
      myListBox->insertItem( i, myParametersMap[ i ].name );
      myListBox->item( i )->setFlags( myListBox->item( i )->flags() | Qt::ItemIsEditable );
    }
    
    changeImage( myListBox->item( 0 ) );
  }
  else
  {
    myListBox->clear();
    myListBox->insertItem( 0, "No Items" );
  }

  connect( myListBox, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( changeImage( QListWidgetItem* ) ) );
  connect( myListBox, SIGNAL( itemChanged( QListWidgetItem* ) ), this, SLOT( editItemText( QListWidgetItem* ) ) );
	
  aLayout->addWidget( myListBox );
  aLayout->addWidget( myCurViewPort, 30 );

  QHBoxLayout* aButtonLayout = new QHBoxLayout( aWidget2 );
  aButtonLayout->setMargin( 0 );
  aButtonLayout->setSpacing( 5 );

  QPushButton* theOk     = new QPushButton( tr( "Ok" ), aWidget2 );            theOk->setAutoDefault( false );
  QPushButton* theCancel = new QPushButton( tr( "Cancel" ), aWidget2 );		 theCancel->setAutoDefault( false );
  QPushButton* theDelete = new QPushButton( tr( "Delete" ), aWidget2 );		 theDelete->setAutoDefault( false );
  QPushButton* theClearAll = new QPushButton( tr( "Clear List" ), aWidget2 );  theClearAll->setAutoDefault( false );

  aButtonLayout->addWidget( theOk );
  aButtonLayout->addWidget( theCancel );
  aButtonLayout->addWidget( theDelete );
  aButtonLayout->addWidget( theClearAll );

  aGrid->addWidget( aWidget1, 0, 0 );
  aGrid->addWidget( aWidget2, 1, 0 );
	
  connect( theOk, SIGNAL( clicked() ), this, SLOT( OKpressed() ) );
  connect( theCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
  connect( theDelete, SIGNAL( clicked() ), this, SLOT( deleteSelectedItems() ) );
  connect( theClearAll, SIGNAL( clicked() ), this, SLOT( clearList() ) );
}

/*!
  Destructor
*/
OCCViewer_CreateRestoreViewDlg::~OCCViewer_CreateRestoreViewDlg()
{
}

/*!
  Changes image in accordance with item
  \param curItem - item contains info about view parameters
*/
void OCCViewer_CreateRestoreViewDlg::changeImage( QListWidgetItem* curItem )
{
  if( curItem && ( curItem->flags() & Qt::ItemIsEditable ) )
  {
    int lowLevel  = -1;
    int highLevel = -1;
    int index = curItem->listWidget()->row( curItem );
    if( myKeyFlag == 2 )
    {
      for( int i = 0; i < (int)myListBox->count(); i++ )
      {
	if( myListBox->item( i )->isSelected() && i != index )
	{
	  myListBox->clearSelection();
	  if( i > index )
	  {
	    lowLevel  = index;
	    highLevel = i;
	  }
	  else
	  {
	    lowLevel  = i;
	    highLevel = index;
	  }
	  for( int j = lowLevel; j <= highLevel; j++ )
	    myListBox->item( j )->setSelected( TRUE );
	  break;
	}
	if( myListBox->item( i )->isSelected() && i == index )
	  myListBox->item( i )->setSelected( TRUE );
      }
    }
    
    Handle(V3d_View) aView3d = myCurViewPort->getView();
    myCurrentItem = myParametersMap[ index ];
    
    Standard_Boolean prev = aView3d->SetImmediateUpdate( Standard_False );
    aView3d->SetScale( myCurrentItem.scale );
    aView3d->SetCenter( myCurrentItem.centerX, myCurrentItem.centerY );
    aView3d->SetProj( myCurrentItem.projX, myCurrentItem.projY, myCurrentItem.projZ );
    aView3d->SetTwist( myCurrentItem.twist );
    aView3d->SetAt( myCurrentItem.atX, myCurrentItem.atY, myCurrentItem.atZ );
    aView3d->SetImmediateUpdate( prev );
    aView3d->SetEye( myCurrentItem.eyeX, myCurrentItem.eyeY, myCurrentItem.eyeZ );
    aView3d->SetAxialScale( myCurrentItem.scaleX, myCurrentItem.scaleY, myCurrentItem.scaleZ );
  }
}

/*!
  \return current view parameters (corresponding to current item)
*/
viewAspect OCCViewer_CreateRestoreViewDlg::currentItem() const
{
  return myCurrentItem;
}

/*!
  Deletes selected items from list view
*/
void OCCViewer_CreateRestoreViewDlg::deleteSelectedItems()
{
  if( myListBox->count() )
  {
    int curIndex = -1;
    for( int i = 0; i < (int)myListBox->count(); i++ )
      if( myListBox->item( i )->isSelected() && ( myListBox->item( i )->flags() & Qt::ItemIsEditable ) )
      {
	QListWidgetItem* anItemToDelete = myListBox->takeItem( i );
	delete anItemToDelete;
	for( int j = i; j < (int)myParametersMap.count(); j++ )
	  if( j != myParametersMap.count() - 1 )
	    myParametersMap[ j ] = myParametersMap[ j + 1 ];
	  else
	    myParametersMap.removeAt( j );
	if( i != myListBox->count() )
	  curIndex = i;
	else
	  curIndex = i - 1;
	i--;
      }
    if( curIndex >= 0 )
    {
      myListBox->setCurrentItem( myListBox->item( curIndex ) );
      changeImage( myListBox->item( curIndex ) );
    }
  }
  if( !myListBox->count() )
  {
    myListBox->clear();
    myListBox->insertItem( 0, "No Items" );
  }
}

/*!
  Clears list of view aspects
*/
void OCCViewer_CreateRestoreViewDlg::clearList()
{
  myListBox->clear();
  myListBox->insertItem( 0, "No Items" );
  
  myParametersMap.clear();
}

/*!
  \return const reference to all view aspects
*/
const viewAspectList& OCCViewer_CreateRestoreViewDlg::parameters() const
{
  return myParametersMap;
}

/*!
  Renames key of view aspect map in accordance with item name
  \param anItem - item
*/
void OCCViewer_CreateRestoreViewDlg::editItemText( QListWidgetItem* anItem )
{
  int index = anItem->listWidget()->row( anItem );
  myParametersMap[ index ].name = anItem->text().toLatin1();
}

/*!
  Custom event filter
*/
bool OCCViewer_CreateRestoreViewDlg::eventFilter( QObject* anObj, QEvent* anEv )
{
  if( anEv->type() == QEvent::KeyPress )
  {
    QKeyEvent* aKeyEv = ( QKeyEvent* )anEv;
    if( aKeyEv->key() == Qt::Key_Control )
    {
      myKeyFlag = 1;
      myListBox->setSelectionMode( QAbstractItemView::MultiSelection );	
    }
    else if( aKeyEv->key() == Qt::Key_Shift )
    {
      myKeyFlag = 2;
      myListBox->setSelectionMode( QAbstractItemView::MultiSelection );	
    }
    else
      myListBox->setSelectionMode( QAbstractItemView::SingleSelection );
  }
  if( anEv->type() == QEvent::KeyRelease )
    myKeyFlag = 0;
  
  if( !myKeyFlag )
  {
    if( anEv->type() == QEvent::KeyPress || anEv->type() == QEvent::MouseButtonPress )
      myListBox->setSelectionMode( QAbstractItemView::SingleSelection );
  }
  return QWidget::eventFilter( anObj, anEv );
}

/*!
  SLOT: called on OK click, emits dlgOk() and closes dialog
*/
void OCCViewer_CreateRestoreViewDlg::OKpressed()
{
  emit dlgOk();
  accept();
}

