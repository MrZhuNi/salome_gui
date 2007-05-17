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
#include "QtxLogoMgr.h"

#include <QLabel>
#include <QImage>
#include <QBitmap>
#include <QLayout>
#include <QMenuBar>
#include <QPointer>
#include <QApplication>

/*!
  class: LogoBox [internal]
*/

class QtxLogoMgr::LogoBox : public QWidget
{
public:
  LogoBox( QMenuBar* );

  QMenuBar*      menuBar() const;
  virtual bool   eventFilter( QObject*, QEvent* );
  void           setLabels( const QList<QLabel*>& );

protected:
  virtual void   customEvent( QEvent* );

private:
  void           updateCorner();
  void           updateContents();

private:
  typedef QPointer<QWidget> WidgetPtr;

private:
  QMenuBar*      myMB;
  QList<QLabel*> myLabels;
  WidgetPtr      myCornWid;
};

QtxLogoMgr::LogoBox::LogoBox( QMenuBar* mb )
: QWidget( mb ),
myMB( mb ),
myCornWid( 0 )
{
  myMB->installEventFilter( this );
  updateCorner();
}

QMenuBar* QtxLogoMgr::LogoBox::menuBar() const
{
  return myMB;
}

bool QtxLogoMgr::LogoBox::eventFilter( QObject* o, QEvent* e )
{
  if ( o != menuBar() )
    return false;

  if ( e->type() == QEvent::MenubarUpdated )
    updateCorner();

  if ( e->type() == QEvent::ChildAdded || e->type() == QEvent::ChildRemoved )
  {
    updateCorner();
    QApplication::postEvent( this, new QEvent( QEvent::User ) );
  }

  return false;
}

void QtxLogoMgr::LogoBox::setLabels( const QList<QLabel*>& labs )
{
  for ( QList<QLabel*>::iterator it = myLabels.begin(); it != myLabels.end(); ++it )
  {
    if ( !labs.contains( *it ) )
      delete *it;
  }

  myLabels = labs;
  updateContents();
}

void QtxLogoMgr::LogoBox::customEvent( QEvent* )
{
  updateCorner();
}

void QtxLogoMgr::LogoBox::updateCorner()
{
  if ( menuBar()->cornerWidget() == this )
    return;

  myCornWid = menuBar()->cornerWidget();
  myMB->setCornerWidget( this );
  updateContents();
}

void QtxLogoMgr::LogoBox::updateContents()
{
  if ( layout() )
    delete layout();

  QHBoxLayout* base = new QHBoxLayout( this );
  base->setMargin( 0 );
  base->setSpacing( 3 );

  if ( myCornWid )
    base->addWidget( myCornWid );

  for ( QList<QLabel*>::const_iterator it = myLabels.begin(); it != myLabels.end(); ++it )
    base->addWidget( *it );

  QApplication::sendPostedEvents();
}

/*!
  class: QtxLogoMgr [public]
*/

/*!
  Constructor
*/
QtxLogoMgr::QtxLogoMgr( QMenuBar* mb )
: QObject( mb )
{
  myBox = new LogoBox( mb );
}

/*!
  Destructor
*/
QtxLogoMgr::~QtxLogoMgr()
{
}

/*!
  Returns the menubar.
*/
QMenuBar* QtxLogoMgr::menuBar() const
{
  return myBox->menuBar();
}

/*!
  Returns the count of the existed logos.
*/
int QtxLogoMgr::count() const
{
  return myLogos.count();
}

/*!
  Insert new logo pixmap to the menu bar area
*/
void QtxLogoMgr::insert( const QString& id, const QPixmap& pix, const int index )
{
  if ( pix.isNull() )
    return;

  LogoInfo& inf = insert( id, index );

  inf.pix = pix;

  generate();
}

/*!
  Insert new logo movie to the menu bar area
*/
void QtxLogoMgr::insert( const QString& id, QMovie* movie, const int index )
{
  if ( !movie )
    return;

  LogoInfo& inf = insert( id, index );

  inf.mov = movie;
  movie->setParent( this );
  movie->setCacheMode( QMovie::CacheAll );
  movie->jumpToFrame( 0 );

  generate();
}

/*!
  Insert new logo information structure into the logos list
*/
QtxLogoMgr::LogoInfo& QtxLogoMgr::insert( const QString& id, const int index )
{
  LogoInfo empty;
  empty.id = id;
  empty.mov = 0;

  int idx = find( id );
  if ( idx < 0 )
  {
    idx = index < (int)myLogos.count() ? index : -1;
    if ( idx < 0 )
    {
      myLogos.append( empty );
      idx = myLogos.count() - 1;
    }
    else
      myLogos.insert( idx, empty );
  }

  LogoInfo& inf = myLogos[idx];

  return inf;
}

/*!
  Removes a logo
*/
void QtxLogoMgr::remove( const QString& id )
{
  int idx = find( id );
  if ( idx < 0 )
    return;

  myLogos.removeAt( idx );

  generate();
}

/*!
  Removes all logos 
*/
void QtxLogoMgr::clear()
{
  myLogos.clear();
  generate();
}

/*!
  Start the animation for specified movie or for all movies if id is empty.
*/
void QtxLogoMgr::startAnimation( const QString& id )
{
  QList<QMovie*> movList;
  movies( id, movList );

  for ( QList<QMovie*>::iterator it = movList.begin(); it != movList.end(); ++it )
    (*it)->start();
}

/*!
  Stop the animation for specified movie or for all movies if id is empty.
*/
void QtxLogoMgr::stopAnimation( const QString& id )
{
  QList<QMovie*> movList;
  movies( id, movList );

  for ( QList<QMovie*>::iterator it = movList.begin(); it != movList.end(); ++it )
    (*it)->stop();
}

/*!
  Pause/Continue the animation for specified movie or for all movies if id is empty.
*/
void QtxLogoMgr::pauseAnimation( const bool pause, const QString& id )
{
  QList<QMovie*> movList;
  movies( id, movList );

  for ( QList<QMovie*>::iterator it = movList.begin(); it != movList.end(); ++it )
    (*it)->setPaused( pause );
}

/*!
  Inserts logo to menu bar
*/
void QtxLogoMgr::generate()
{
  if ( !menuBar() )
    return;

  QList<QLabel*> labels;
  for ( LogoList::const_iterator it = myLogos.begin(); it != myLogos.end(); ++it )
  {
    QPixmap pix = (*it).pix;
    QMovie* mov = (*it).mov;
    if ( !pix.isNull() && !pix.mask() )
    {
      QBitmap bm;
      QImage img = pix.toImage();
      if ( img.hasAlphaChannel() )
        bm = QPixmap::fromImage( img.createAlphaMask() );
      else
        bm = QPixmap::fromImage( img.createHeuristicMask() );
      pix.setMask( bm );
    }

    QLabel* logoLab = new QLabel( myBox );
    if ( mov )
      logoLab->setMovie( mov );
    else
    {
      logoLab->setPixmap( (*it).pix );
//      if ( !pix.mask().isNull() )
//  	    logoLab->setMask( pix.mask() );
    }

    logoLab->setScaledContents( false );
    logoLab->setAlignment( Qt::AlignCenter );

    labels.append( logoLab );
  }

  myBox->setLabels( labels );
}

/*!
  \return index of found logo
  \param id - logo id
*/
int QtxLogoMgr::find( const QString& id ) const
{
  int idx = -1;
  for ( uint i = 0; i < myLogos.count() && idx < 0; i++ )
  {
    if ( myLogos.at( i ).id == id )
      idx = i;
  }
  return idx;
}

/*!
  \return list of movies according to specified id
  \param id - logo id
*/
void QtxLogoMgr::movies( const QString& id, QList<QMovie*>& lst ) const
{
  lst.clear();
  for ( LogoList::const_iterator it = myLogos.begin(); it != myLogos.end(); ++it )
  {
    if ( (*it).mov && ( id.isEmpty() || id == (*it).id ) )
      lst.append( (*it).mov );
  }
}
