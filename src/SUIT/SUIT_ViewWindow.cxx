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
// SUIT_ViewWindow.cxx: implementation of the SUIT_ViewWindow class.
//

#include "SUIT_ViewWindow.h"

#include "SUIT_Tools.h"
#include "SUIT_Study.h"
#include "SUIT_Desktop.h"
#include "SUIT_MessageBox.h"
#include "SUIT_Application.h"
#include "SUIT_ViewManager.h"
#include "QtxActionToolMgr.h"
#include "Qtx.h"

#include <QEvent>
#include <QIcon>
#include <QApplication>
#include <QContextMenuEvent>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>

/*!\class SUIT_ViewWindow
 * Class provide view window.
 */

/*! Dump view custom event*/
const int DUMP_EVENT = QEvent::User + 123;

/*! Constructor.*/
SUIT_ViewWindow::SUIT_ViewWindow( SUIT_Desktop* theDesktop )
: QMainWindow( theDesktop ),
  myManager( 0 )
{
  myDesktop = theDesktop;

  setWindowIcon( myDesktop->windowIcon() );

  setAttribute( Qt::WA_DeleteOnClose );

  myToolMgr = new QtxActionToolMgr( this );
}

/*! Destructor.*/
SUIT_ViewWindow::~SUIT_ViewWindow()
{
}

/*!
  Sets new view manager for window
  \param theManager - new view manager
*/
void SUIT_ViewWindow::setViewManager( SUIT_ViewManager* theManager )
{
  myManager = theManager;
}

/*!
  \return view manager of window
*/
SUIT_ViewManager* SUIT_ViewWindow::getViewManager() const
{
  return myManager;
}

/*!
  \return QImage, containing all scene rendering in window
*/
QImage SUIT_ViewWindow::dumpView()
{
  return QImage();
}

/*!
  Saves image to file according to the format
  \param image - image
  \param fileName - name of file
  \param format - string contains name of format (for example, "BMP"(default) or "JPEG", "JPG")
*/
bool SUIT_ViewWindow::dumpViewToFormat( const QImage& img, const QString& fileName, const QString& format )
{
  if( img.isNull() )
    return false;

  QString fmt = format;
  if( fmt.isEmpty() )
    fmt = QString( "BMP" ); // default format
  else if( fmt == "JPG" )
    fmt = "JPEG";

  QApplication::setOverrideCursor( Qt::WaitCursor );
  bool res = img.save( fileName, fmt.toLatin1() );
  QApplication::restoreOverrideCursor();
  return res;
}

/*!
  Saves scene rendering in window to file
  \param fileName - name of file
  \param format - string contains name of format (for example, "BMP"(default) or "JPEG", "JPG")
*/
bool SUIT_ViewWindow::dumpViewToFormat( const QString& fileName, const QString& format )
{
  return dumpViewToFormat( dumpView(), fileName, format );
}

/*!
  Prints given image
  \param theImage - the image to print
*/
void SUIT_ViewWindow::printImage( const QImage& theImage, QWidget* theWidget )
{
  if ( theImage.isNull() )
    return;

#if !defined(WIN32) && !defined(QT_NO_CUPS)
#if QT_VERSION < 0x040303
  if ( !Qtx::hasAnyPrinters() ) {
    SUIT_MessageBox::warning( this, tr( "WRN_WARNING" ),
                              tr( "WRN_NO_PRINTERS" ) );
    return;
  }
#endif
#endif

  // stored settings for further starts
  static QString aPrinterName;
  static int anOrientation = -1;

  QPrinter aPrinter;

  // restore settinds from previous launching

  // printer name
  if ( !aPrinterName.isEmpty() )
    aPrinter.setPrinterName( aPrinterName );
  else 
  {
    // Nothing to do for the first printing. aPrinter contains default printer name by default
  }

  if ( anOrientation >= 0 )
    aPrinter.setOrientation( (QPrinter::Orientation)anOrientation );
  else
    aPrinter.setOrientation( QPrinter::Landscape );


  QPrintDialog printDlg( &aPrinter, theWidget );
  printDlg.setPrintRange( QAbstractPrintDialog::AllPages );
  if ( printDlg.exec() != QDialog::Accepted ) 
    return;

  // store printer settings for further starts
  aPrinterName = aPrinter.printerName();
  anOrientation = aPrinter.orientation();

  int W, H;
  QPainter aPainter;

  // work arround for printing on real printer
  if ( aPrinter.outputFileName().isEmpty() && aPrinter.orientation() == QPrinter::Landscape )
  {
    aPrinter.setFullPage( true );
    // set paper orientation and rotate painter
    aPrinter.setOrientation( QPrinter::Portrait );

    W = aPrinter.height();
    H = aPrinter.width();

    int wBorder = aPrinter.paperRect().height() - W;
    int hBorder = aPrinter.paperRect().width() - H;

    aPainter.begin( &aPrinter );
    aPainter.translate( QPoint( H + hBorder, wBorder ) );
    aPainter.rotate( 90 );
  }
  else 
  {
    aPrinter.setFullPage( false );
    aPainter.begin( &aPrinter );
    W = aPrinter.width();
    H = aPrinter.height();
  }

  QImage anImage = theImage;
  if ( anImage.width() > W || anImage.height() > H )
    anImage = anImage.scaled( W, H, Qt::KeepAspectRatio, Qt::SmoothTransformation );

  // place image in the center of page
  int offsetW = ( W - anImage.width() ) / 2;
  int offsetH = ( H - anImage.height() ) / 2;

  aPainter.drawImage( offsetW, offsetH, anImage );

  aPainter.end();
}

/*!
  Set or clear flag Qt::WDestructiveClose
*/
void SUIT_ViewWindow::setDestructiveClose( const bool on )
{
  setAttribute( Qt::WA_DeleteOnClose, on );
}

/*! Close event \a theEvent.
*/
void SUIT_ViewWindow::closeEvent( QCloseEvent* e )
{
  e->ignore();
  emit closing( this );
}

/*! Context menu requested for event \a e.
*/
void SUIT_ViewWindow::contextMenuEvent( QContextMenuEvent* e )
{
  e->ignore();

  QMainWindow::contextMenuEvent( e );

  if ( e->isAccepted() )
    return;

  if ( e->reason() != QContextMenuEvent::Mouse )
    emit contextMenuRequested( e );
}

/*! Post events on dump view.
*/
void SUIT_ViewWindow::onDumpView()
{
  QApplication::postEvent( this, new QPaintEvent( QRect( 0, 0, width(), height() ) ) );
  QApplication::postEvent( this, new QEvent( (QEvent::Type)DUMP_EVENT ) );
}

/*!
  \return filters for image files
*/
QString SUIT_ViewWindow::filter() const
{
  return tr( "TLT_IMAGE_FILES" );
}

/*! Reaction view window on event \a e.
*/
bool SUIT_ViewWindow::event( QEvent* e )
{
  if ( e->type() == DUMP_EVENT )
  {
    bool bOk = false;
    if ( myManager && myManager->study() && myManager->study()->application() )
    {
      QImage im = dumpView();

      // get file name
      SUIT_Application* app = myManager->study()->application();
      QString fileName = app->getFileName( false, QString(), filter(), tr( "TLT_DUMP_VIEW" ), 0 );
      if ( !fileName.isEmpty() )
      {
	      QString fmt = SUIT_Tools::extension( fileName ).toUpper();
	      bOk = dumpViewToFormat( im, fileName, fmt );
      }
      else
	      bOk = true; // cancelled
    }
    if ( !bOk )
      SUIT_MessageBox::critical( this, tr( "ERROR" ), tr( "ERR_CANT_DUMP_VIEW" ) );

    return true;
  }
  return QMainWindow::event( e );
}

/*! Called by SUIT_Accel::onActivated() when a key accelerator was activated and this window was active
*/
bool SUIT_ViewWindow::onAccelAction( int _action )
{
  return action( _action );
}

/*! action  handle standard action (zoom, pan) or custom action.  to be redefined in successors.
*/
bool SUIT_ViewWindow::action( const int  )
{
  return true;
}

/*!
  \return string containing visual parameters of window
*/
QString   SUIT_ViewWindow::getVisualParameters()
{
  return "empty";
}

/*!
  Sets visual parameters of window by its string representation
  \param parameters - string with visual parameters
*/
void SUIT_ViewWindow::setVisualParameters( const QString& /*parameters*/ )
{
}

/*!
  \return associated tool bar manager
*/
QtxActionToolMgr* SUIT_ViewWindow::toolMgr() const
{
  return myToolMgr;
}

/*!
  \return window unique identifier  
*/
int SUIT_ViewWindow::getId() const
{
  return int(long(this));
}
