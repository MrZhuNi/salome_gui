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
//  SALOME Plot2d : implementation of desktop and GUI kernel
//
//  File   : Plot2d_SetupCurvesDlg.cxx
//  Author : Sergey LITONIN

#include "Plot2d_SetupCurvesDlg.h"
#include <QtxGroupBox.h>
#include <qtablewidget.h>
#include <qtoolbutton.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qfontmetrics.h>
#include <qcolordialog.h>

#include <qpen.h>
#include <qwt_symbol.h>
#include <Plot2d_Curve.h>
#include <SUIT_MessageBox.h>
#include <qheaderview.h>
#include <qcombobox.h>
#include <qset.h>
#include "Plot2d.h"
#include <qevent.h>
#include <qpainter.h>

#define PIXMAP_COL      0
#define TEXT_COL        1
#define MARKER_COL      2
#define COLOR_COL       3
#define NB_MARKERS_COL  4


Plot2d_PixmapWg::Plot2d_PixmapWg( QWidget* theParent )
: QWidget( theParent )
{
  mySymbol.setSize( QSize( 8, 8 ) );
  mySymbol.setStyle( QwtSymbol::NoSymbol );
}

Plot2d_PixmapWg::~Plot2d_PixmapWg()
{

}

//=============================================================================
// Function : setColor
// Purpose  : 
//=============================================================================
void Plot2d_PixmapWg::setColor( const QColor& theColor )
{
  mySymbol.setBrush( QBrush( theColor ) );
  mySymbol.setPen( QPen( theColor ) );
}

//=============================================================================
// Function : setMarkerType
// Purpose  : 
//=============================================================================
void Plot2d_PixmapWg::setMarkerType( const int  theType )
{
  QwtSymbol::Style aStyle = QwtSymbol::NoSymbol;
  switch( theType )
  {
  case Plot2d::Circle   : aStyle = QwtSymbol::Ellipse; break;
  case Plot2d::Rectangle: aStyle = QwtSymbol::Rect; break;
  case Plot2d::Diamond  : aStyle = QwtSymbol::Diamond; break;
  case Plot2d::DTriangle: aStyle = QwtSymbol::DTriangle; break;
  case Plot2d::UTriangle: aStyle = QwtSymbol::UTriangle; break;
  case Plot2d::LTriangle: aStyle = QwtSymbol::LTriangle; break;
  case Plot2d::RTriangle: aStyle = QwtSymbol::RTriangle; break;
  case Plot2d::Cross    : aStyle = QwtSymbol::Cross; break;
  case Plot2d::XCross   : aStyle = QwtSymbol::XCross; break;
  }

  mySymbol.setStyle( aStyle );
}

//=============================================================================
// Function : setText
// Purpose  : 
//=============================================================================
void Plot2d_PixmapWg::setText( const QString& theText )
{
  myText = theText;
}

//=============================================================================
// Function : paint
// Purpose  : 
//=============================================================================
void Plot2d_PixmapWg::paintEvent( QPaintEvent* e )
{
  QRect r = e->rect();
  QPainter p( this );

  if ( myText.isEmpty() )
  {
    mySymbol.draw( &p, r.center() );
  }
  else 
  {
      const QSize& s = mySymbol.size();
      int x = r.left() + s.width() / 2 + 2;
      int y = r.top() + qMax( s.height(), r.height() ) / 2;
      mySymbol.draw( &p, x, y );

      p.setPen( QColor( 0, 0, 0 ) );
      int textH = QFontMetrics( font() ).ascent();
      x = r.left() + s.width() + 10;
      y = qMax( r.top(), r.top() + ( r.height() - textH ) / 2 + textH );
      p.drawText( QPoint( x, y ), myText );
  }
}

/*!
  \brief Constructor
  \param theParent parent widget
*/
Plot2d_SetupCurvesDlg::Plot2d_SetupCurvesDlg( QWidget* theParent )
: QtxDialog( theParent, true, false, QtxDialog::OKCancel )
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  setWindowTitle( tr( "SETUP_CURVES" ) );

  QFrame* aMainFrame = mainFrame();

  QVBoxLayout* aMainLay = new QVBoxLayout( aMainFrame );
  aMainLay->setMargin( 5 );

  myGrp = new QtxGroupBox( aMainFrame ); 
  myGrp->setTitle( tr( "PARAMETERS" ) );
  aMainLay->addWidget( myGrp );

  QVBoxLayout* aLay = new QVBoxLayout( myGrp );
  aLay->setMargin( 0 );

  // Create table
  myTable = new QTableWidget( myGrp );
  myTable->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

  aLay->addWidget( myTable );

  myTable->setRowCount( 0 );
  myTable->setColumnCount( 5 );

  QStringList aLabels;
  aLabels.append( tr( "FIG" ) );
  aLabels.append( tr( "TEXT" ) );
  aLabels.append( tr( "MARKER" ) );
  aLabels.append( tr( "COLOR" ) );
  aLabels.append( tr( "NB_MARKERS" ) );
  myTable->setHorizontalHeaderLabels( aLabels );
  myTable->verticalHeader()->hide();
  myTable->setSelectionMode( QTableWidget::NoSelection );

  QComboBox* aCombo = new QComboBox( 0 );
  myTable->verticalHeader()->setDefaultSectionSize( aCombo->sizeHint().height() );
  delete aCombo;
  
  //myTable->setLeftMargin( 0 );
  int l, t, r, b;
  myTable->getContentsMargins ( &l, &t, &r, &b );
  myTable->setContentsMargins ( 0, t, r, b );

  myTable->horizontalHeader()->setResizeMode( PIXMAP_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setResizeMode( TEXT_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setResizeMode( MARKER_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setResizeMode( COLOR_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setResizeMode( NB_MARKERS_COL, QHeaderView::Fixed );
  myTable->horizontalHeader()->setHighlightSections( false );

  // Minus button
  QPixmap minusPix = aResMgr->loadPixmap( "Plot2d", tr( "ICON_MINUS" ) );
  myRemoveBtn = new QToolButton( 0 );
  myRemoveBtn->setIcon( minusPix );
  myRemoveBtn->setFixedSize( minusPix.size() /*+ QSize( 2, 2 )*/ );
  myGrp->insertTitleWidget( myRemoveBtn );

  connect( myRemoveBtn, SIGNAL( clicked() ), SLOT( onRemove() ) );
  connect( myTable, SIGNAL( valueChanged( int, int ) ), SLOT( onValueChanged( int, int ) ) );

  setButtonPosition( Right, Cancel );
  setMinimumHeight( 250 );
}

/*!
  \brief Destructor
*/
Plot2d_SetupCurvesDlg::~Plot2d_SetupCurvesDlg()
{
}

/*!
  \brief Insert combo-box widget in specified cell 
  \param theRow row
  \param theCol column
  \param theItems list of combo-box items
  \param theCurrItem current combo-box item 
*/
void Plot2d_SetupCurvesDlg::setCombo( const int theRow, 
                                      const int theCol, 
                                      const QStringList& theItems,
                                      const int theCurrItem,
                                      const int theWidth  )
{
  QComboBox* aCombo = new QComboBox( myTable );
  aCombo->addItems( theItems );
  aCombo->setCurrentIndex( theCurrItem );
  if ( theWidth > 0 )
    aCombo->setFixedWidth( theWidth );
  myTable->setCellWidget( theRow, theCol, aCombo );
  connect( aCombo, SIGNAL( activated( int ) ), SLOT( onMarkerType() ) );
}

/*!
  \brief Sets text in specified cell
  \param theRow row
  \param theCol column
  \param theText text
*/
void Plot2d_SetupCurvesDlg::setText( const int theRow,
                                     const int theCol,
                                     const QString& theText )
{
  QTableWidgetItem* anItem = myTable->item( theRow, theCol );
  if ( !anItem )
  {
    anItem = new QTableWidgetItem( theText );
    myTable->setItem( theRow, theCol, anItem );
  }
  else 
    anItem->setText( theText );
}

//=============================================================================
// Function : SetParameters
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::SetParameters( const QVector< int >& theMarker,
                                           const QVector< QString >& theText,
                                           const QVector< QColor >& theColor,
                                           const QVector< int >& theNbMarkers )
{
  int nbRows = qMax( qMax( theMarker.size(), theText.size()), 
                     qMax( theColor.size(), theNbMarkers.size() ) );
  
  myTable->setRowCount( nbRows );

  const QStringList& aMarkers = getMarkers();

  QFontMetrics fm( myTable->font() );
  int aTextLength = 0;

  // calculate width of combo-box
  QStringList::const_iterator anIter;
  int strWidth = 0;
  for ( anIter = aMarkers.begin(); anIter != aMarkers.end(); ++anIter )
    strWidth = qMax( strWidth, fm.width( *anIter ) );
  strWidth += 20;
    
  for ( int i = 0; i < nbRows; i++ )
  {
    // Pixmap
    Plot2d_PixmapWg* aPixWg = new Plot2d_PixmapWg( myTable ) ;
    myTable->setCellWidget( i, PIXMAP_COL, aPixWg );

    // Marker type
    setCombo( i, MARKER_COL, aMarkers, theMarker[ i ]/*, strWidth*/ );

    // Text
        
    setText( i, TEXT_COL, theText[ i ] );
    aTextLength = qMax( aTextLength, fm.width( theText[ i ] ) );

    // Color 
    QPushButton* aBtn = new QPushButton( myTable );
    myTable->setCellWidget( i, COLOR_COL, aBtn );
    setColor( aBtn, theColor[ i ] );
    connect( aBtn, SIGNAL( clicked() ), SLOT( onColorBtn() ) );

    // Nb markers
    setText( i, NB_MARKERS_COL, QString( "%1" ).arg( theNbMarkers[ i ] ) );
  }

  myTable->setColumnWidth( PIXMAP_COL, 24 );
  myTable->setColumnWidth( TEXT_COL, aTextLength + 10 );
  myTable->setColumnWidth( MARKER_COL, strWidth + 10 );
  myTable->setColumnWidth( COLOR_COL, fm.width( tr( "COLOR" ) ) + 10 );
  myTable->setColumnWidth( NB_MARKERS_COL, fm.width( tr( "NB_MARKERS" ) ) + 10 );
  int aWidth = myTable->columnWidth( PIXMAP_COL ) + myTable->columnWidth( TEXT_COL )+
               myTable->columnWidth( MARKER_COL ) + myTable->columnWidth( COLOR_COL ) +
               myTable->columnWidth( NB_MARKERS_COL );

  QFrame* aWg = (QFrame*)myTable->viewport()->parentWidget();

  int fw = aWg->frameWidth();
  int lw = aWg->lineWidth();
  int mlw = aWg->midLineWidth();
  aWg->setFixedWidth( aWidth + 2 * fw );
  //aWg->setMinimumWidth( aWidth + 2 * fw );
  
  myRemovedIndexes.clear();

  updatePixmap( -1 );
}

//=============================================================================
// Function : GetParameters
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::GetParameters( QVector< int >& theMarkers,
                                           QVector< QString >& theTexts,
                                           QVector< QColor >& theColors,
                                           QVector< int >& theNbMarkers ) const
{
  int nbRows = myTable->rowCount();

  theMarkers.resize( nbRows );
  theTexts.resize( nbRows );
  theColors.resize( nbRows );
  theNbMarkers.resize( nbRows );

  for ( int i = 0; i < nbRows; i++ )
  {
    // Marker type
    QComboBox* aCombo = dynamic_cast<QComboBox*>( myTable->cellWidget( i, MARKER_COL ) );
    theMarkers[ i ] = aCombo ? aCombo->currentIndex() : -1;

    // Text
    QTableWidgetItem* it = myTable->item( i, TEXT_COL );
    theTexts[ i ] = it ? it->text() : "";

    // Color
    QColor aColor;
    QPushButton* aBtn = dynamic_cast< QPushButton* >( myTable->cellWidget( i, COLOR_COL ) );
    if ( aBtn )
      aColor = aBtn->palette().color( aBtn->backgroundRole() );
    
    theColors[ i ] = aColor;
    
    // Nb markers
    it = myTable->item( i, NB_MARKERS_COL );
    QString aStr = it ? it->text() : "";
    bool isOk = false;
    int nbMarkers = aStr.toInt( &isOk );
    if ( isOk )
      theNbMarkers[ i ] = nbMarkers;
    else 
      theNbMarkers[ i ] = -1;
  }
}

//=============================================================================
// Function : GetRemovedIndexes
// Purpose  : 
//=============================================================================
const QList< int >& Plot2d_SetupCurvesDlg::GetRemovedIndexes() const
{
  return myRemovedIndexes;
}

//=============================================================================
// Function : acceptData
// Purpose  : 
//=============================================================================
bool Plot2d_SetupCurvesDlg::acceptData() const
{
  ((Plot2d_SetupCurvesDlg*)this)->setButtonFocus( OK );
  int nbRows = myTable->rowCount();
  for ( int i = 0; i < nbRows; i++ )
  {
    QTableWidgetItem* it = myTable->item( i, NB_MARKERS_COL );
    QString aStr = it ? it->text() : "";
    bool isOk = false;
    int nbMarkers = aStr.toInt( &isOk );
    if ( !isOk || nbMarkers<= 0 )
    {
      SUIT_MessageBox::information( (QWidget*)this, tr( "PLOT2D_INSUFFICIENT_DATA" ), 
        tr( "PLOT2D_ENTER_VALID_DATA" ), tr( "BUT_OK" ) );
      return false;
    }
  }

  return true;
}

//=============================================================================
// Function : getMarkers
// Purpose  : 
//=============================================================================
const QStringList& Plot2d_SetupCurvesDlg::getMarkers() const
{
  static QStringList aList;

  if ( aList.isEmpty() )
  {
    aList.append( tr( "NONE_MARKER_LBL" ) );
    aList.append( tr( "CIRCLE_MARKER_LBL" ) );
    aList.append( tr( "RECTANGLE_MARKER_LBL" ) );
    aList.append( tr( "DIAMOND_MARKER_LBL" ) );
    aList.append( tr( "DTRIANGLE_MARKER_LBL" ) );
    aList.append( tr( "UTRIANGLE_MARKER_LBL" ) );
    aList.append( tr( "LTRIANGLE_MARKER_LBL" ) );
    aList.append( tr( "RTRIANGLE_MARKER_LBL" ) );
    aList.append( tr( "CROSS_MARKER_LBL" ) );
    aList.append( tr( "XCROSS_MARKER_LBL" ) );
  }

  return aList;
}

//=============================================================================
// Function : onRemove
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::onRemove( )
{
  QList< int > toRemove;

  int aCurrRow = myTable->currentRow();
  int aCurrCol = myTable->currentColumn();

  // get selected rows
  QSet< int > aSelRows;
  QList<QTableWidgetSelectionRange> aRegs = myTable->selectedRanges();
  QList<QTableWidgetSelectionRange>::iterator selIter;
  for ( selIter = aRegs.begin(); selIter != aRegs.end(); ++selIter )
  {
    const QTableWidgetSelectionRange& aReg = *selIter;
    for ( int i = aReg.topRow(), n = aReg.bottomRow(); i < n; i++ )
      aSelRows.insert( i );
  }

  int i, n;
  for ( i = 0, n = myTable->rowCount(); i < n; i++ )
  {
    if ( aSelRows.contains( i ) || aCurrRow == i )
      toRemove.append( i );
  }

  if ( !toRemove.count() )
    return;

  int nbRemoved = 0;
  QList< int >::iterator anIter;
  for ( anIter = toRemove.begin(); anIter != toRemove.end(); ++anIter )
  {
    int aRow = *anIter - nbRemoved;
    myTable->removeRow( aRow );
    myRemovedIndexes.append( aRow );
    nbRemoved++;
  }

  int nbRows = myTable->rowCount();
  if ( aCurrRow < nbRows && aCurrRow >= 0 && aCurrCol >= 0 )
    myTable->setCurrentCell( aCurrRow, aCurrCol );
  else if ( nbRows > 0 && aCurrCol >= 0 )
    myTable->setCurrentCell( nbRows - 1, aCurrCol );
}

//=============================================================================
// Function : onColorBtn
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::onColorBtn()
{ 
  QObject* aSender = (QObject*)sender();
  if ( !aSender )
    return;

  QPushButton* aBtn = dynamic_cast< QPushButton* >( aSender );
  if ( !aBtn )
    return;

  QColor anOldColor = aBtn->palette().color( aBtn->backgroundRole() );
  QColor aNewColor = QColorDialog::getColor( anOldColor, this );
  if ( !aNewColor.isValid() )
    return;
    
  setColor( aBtn, aNewColor );
  
  for ( int i = 0, n = myTable->rowCount(); i < n; i++ )
  {
    QWidget* aWg = myTable->cellWidget( i, COLOR_COL );
    if ( aWg == aBtn )
    {
      updatePixmap( i );
      break;
    }
  }
}

//=============================================================================
// Function : onColorBtn
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::onMarkerType()
{
  QObject* aSender = (QObject*)sender();
  if ( !aSender )
    return;

  QComboBox* aCombo = dynamic_cast< QComboBox* >( aSender );
  if ( !aCombo )
    return;

  for ( int i = 0, n = myTable->rowCount(); i < n; i++ )
  {
    QWidget* aWg = myTable->cellWidget( i, MARKER_COL );
    if ( aWg == aCombo )
    {
      updatePixmap( i );
      break;
    }
  }
}

//=============================================================================
// Function : updatePixmap
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::updatePixmap( const int theRow )
{
  QList< int > aRows;
  if ( theRow < 0 )
  {
    for ( int i = 0, n = myTable->rowCount(); i < n; i++ )
      aRows.append( i );
  }
  else 
    aRows.append( theRow );

  QList< int >::iterator anIter;
  for ( anIter = aRows.begin(); anIter != aRows.end(); ++anIter )
  {
    int r = *anIter;
    Plot2d_PixmapWg* aWg = dynamic_cast<Plot2d_PixmapWg*>( myTable->cellWidget( r, PIXMAP_COL ) );
    if ( aWg )
    {
      // Marker type
      QComboBox* aCombo = dynamic_cast<QComboBox*>( myTable->cellWidget( r, MARKER_COL ) );
      int aMType = aCombo ? aCombo->currentIndex() : Plot2d::None;
      aWg->setMarkerType( aMType );
      
      // Color
      QColor aColor;
      QPushButton* aBtn = dynamic_cast< QPushButton* >( myTable->cellWidget( r, COLOR_COL ) );
      if ( aBtn )
      {
        aColor = aBtn->palette().color( aBtn->backgroundRole() );
        aWg->setColor( aColor );
      }

      aWg->repaint();
    }
  }
}

//=============================================================================
// Function : onValueChanged
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::onValueChanged( int theRow, int theCol )
{
  if ( theCol == COLOR_COL || theCol == MARKER_COL )
    updatePixmap( theRow );
}

//=============================================================================
// Function : setColor
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::setColor( QPushButton* theBtn, const QColor& theColor ) const
{
  QPalette aPal = theBtn->palette();
  aPal.setColor( theBtn->backgroundRole(), theColor );
  theBtn->setPalette( aPal );

  /*theBtn->setPaletteBackgroundColor( theColor );

  QPalette aPalette = theBtn->palette();
  aPalette.setColor( QColorGroup::Midlight, theColor );
  theBtn->setPalette( aPalette );*/
}
