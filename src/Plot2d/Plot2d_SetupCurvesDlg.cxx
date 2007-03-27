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
#include <QtxTable.h>
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

#define PIXMAP_COL      0
#define MARKER_COL      2
#define TEXT_COL        1
#define COLOR_COL       3
#define NB_MARKERS_COL  4

class Plot2d_SetupCurvesDlg::PixmapItem : public QTableItem
{
public: 
  PixmapItem( QTable* theTable );
  virtual ~PixmapItem();

  virtual void paint( QPainter *p, const QColorGroup &cg,
                      const QRect &cr, bool selected );

  void setColor( const QColor& );
  void setMarkerType( const int );

private:
  QwtSymbol mySymbol;
};

Plot2d_SetupCurvesDlg::PixmapItem::PixmapItem( QTable* theTable )
: QTableItem( theTable, Never )
{
  mySymbol.setSize( QSize( 8, 8 ) );
  mySymbol.setStyle( QwtSymbol::None );
}

Plot2d_SetupCurvesDlg::PixmapItem::~PixmapItem()
{

}

//=============================================================================
// Function : setColor
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::PixmapItem::setColor( const QColor& theColor )
{
  mySymbol.setBrush( QBrush( theColor ) );
  mySymbol.setPen( QPen( theColor ) );
}

//=============================================================================
// Function : setMarkerType
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::PixmapItem::setMarkerType( const int  theType )
{
  QwtSymbol::Style aStyle = QwtSymbol::None;
  switch( theType )
  {
  case Plot2d_Curve::Circle   : aStyle = QwtSymbol::Ellipse; break;
  case Plot2d_Curve::Rectangle: aStyle = QwtSymbol::Rect; break;
  case Plot2d_Curve::Diamond  : aStyle = QwtSymbol::Diamond; break;
  case Plot2d_Curve::DTriangle: aStyle = QwtSymbol::DTriangle; break;
  case Plot2d_Curve::UTriangle: aStyle = QwtSymbol::UTriangle; break;
  case Plot2d_Curve::LTriangle: aStyle = QwtSymbol::LTriangle; break;
  case Plot2d_Curve::RTriangle: aStyle = QwtSymbol::RTriangle; break;
  case Plot2d_Curve::Cross    : aStyle = QwtSymbol::Cross; break;
  case Plot2d_Curve::XCross   : aStyle = QwtSymbol::XCross; break;
  }

  mySymbol.setStyle( aStyle );
}

//=============================================================================
// Function : paint
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::PixmapItem::paint( QPainter *p, const QColorGroup &cg,
                                               const QRect &cr, bool selected )
{
  QTableItem::paint( p, cg, cr, selected );
  QPoint c = cr.center();
  QRect r( c.x() - 4 - cr.left(), c.y() - 4 - cr.top(), 8, 8 );
  mySymbol.draw( p, r ); // MarkerType
}

/*!
  Constructor
*/
Plot2d_SetupCurvesDlg::Plot2d_SetupCurvesDlg( QWidget* theParent )
: QtxDialog( theParent, "Plot2d_SetupCurvesDlg", true, true, /*OK | Apply | Close*/ 
             OK | Cancel )
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  setCaption( tr( "SETUP_CURVES" ) );

  QFrame* aMainFrame = mainFrame();
  myGrp = new QtxGroupBox( 1, Qt::Horizontal, tr( "PARAMETERS" ), aMainFrame );

  // Create table

  myTable = new QtxTable( myGrp );

  myTable->setNumRows( 0 );
  myTable->setNumCols( 5 );
  
  QHeader* aHeaders = myTable->horizontalHeader();
  aHeaders->setLabel( PIXMAP_COL, "Fig" );
  aHeaders->setLabel( MARKER_COL, tr( "MARKER" ) );
  aHeaders->setLabel( TEXT_COL, tr( "TEXT" ) );
  aHeaders->setLabel( COLOR_COL, tr( "COLOR" ) );
  aHeaders->setLabel( NB_MARKERS_COL, tr( "NB_MARKERS" ) );
  aHeaders->setResizeEnabled( false );

  myTable->verticalHeader()->hide();
  myTable->setLeftMargin( 0 );

  myTable->setColumnStretchable( MARKER_COL, false );
  myTable->setColumnStretchable( TEXT_COL, true );
  myTable->setColumnStretchable( COLOR_COL, false );
  myTable->setColumnStretchable( NB_MARKERS_COL, false );

  // Minus button
  QPixmap minusPix = aResMgr->loadPixmap( "Plot2d", tr( "ICON_MINUS" ) );
  myRemoveBtn = new QToolButton( 0 );
  myRemoveBtn->setIconSet( minusPix );
  myRemoveBtn->setFixedSize( minusPix.size() /*+ QSize( 2, 2 )*/ );
  myGrp->insertTitleWidget( myRemoveBtn );

  QVBoxLayout* aLay = new QVBoxLayout( aMainFrame, 5, 0 );
  aLay->addWidget( myGrp );

  connect( myRemoveBtn, SIGNAL( clicked() ), SLOT( onRemove() ) );
  connect( myTable, SIGNAL( valueChanged( int, int ) ), SLOT( onValueChanged( int, int ) ) );

  myTable->setColumnWidth( PIXMAP_COL, 24 );
  setButtonPosition( Right, Cancel );
}
/*!
  Destructor
*/
Plot2d_SetupCurvesDlg::~Plot2d_SetupCurvesDlg()
{
}

//=============================================================================
// Function : SetParameters
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::SetParameters( const QValueVector< int >& theMarker,
                                           const QValueVector< QString >& theText,
                                           const QValueVector< QColor >& theColor,
                                           const QValueVector< int >& theNbMarkers )
{
  int nbRows = QMAX( QMAX( theMarker.size(), theText.size()), 
                     QMAX( theColor.size(), theNbMarkers.size() ) );
  
  myTable->setNumRows( nbRows );

  const QStringList& aMarkers = getMarkers();

  QFontMetrics fm( myTable->font() );
  int aTextLength = 0;

  for ( int i = 0; i < nbRows; i++ )
  {
    // Pixmap
    PixmapItem* aPixItem = new PixmapItem( myTable ) ;
    myTable->setItem( i, PIXMAP_COL, aPixItem );

    // Marker type
    QComboTableItem* aCombo = new QComboTableItem( myTable, aMarkers, false );
    myTable->setItem( i, MARKER_COL, aCombo );
    aCombo->setCurrentItem( theMarker[ i ] );

    // Text
    myTable->setText( i, TEXT_COL, theText[ i ] );
    aTextLength = QMAX( aTextLength, fm.width( theText[ i ] ) );

    // Color 
    QPushButton* aBtn = new QPushButton( myTable );
    myTable->setCellWidget( i, COLOR_COL, aBtn );
    setColor( aBtn, theColor[ i ] );
    connect( aBtn, SIGNAL( clicked() ), SLOT( onColorBtn() ) );

    // Nb markers
    myTable->setText( i, NB_MARKERS_COL, QString( "%1" ).arg( theNbMarkers[ i ] ) );
  }

  myTable->setColumnWidth( TEXT_COL, aTextLength + 10 );
  myTable->setColumnWidth( COLOR_COL, fm.width( tr( "COLOR" ) ) + 10 );
  myTable->setColumnWidth( NB_MARKERS_COL, fm.width( tr( "NB_MARKERS" ) ) + 10 );

  myRemovedIndexes.clear();

  updatePixmap( -1 );
}

//=============================================================================
// Function : GetParameters
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::GetParameters( QValueVector< int >& theMarkers,
                                           QValueVector< QString >& theTexts,
                                           QValueVector< QColor >& theColors,
                                           QValueVector< int >& theNbMarkers ) const
{
  int nbRows = myTable->numRows();

  theMarkers.resize( nbRows );
  theTexts.resize( nbRows );
  theColors.resize( nbRows );
  theNbMarkers.resize( nbRows );

  for ( int i = 0; i < nbRows; i++ )
  {
    // Marker type
    QComboTableItem* aCombo = (QComboTableItem*)myTable->item( i, MARKER_COL );
    theMarkers[ i ] = aCombo ? aCombo->currentItem() : -1;

    // Text
    QString aText = myTable->text( i, TEXT_COL );
    theTexts[ i ] = aText;

    // Color
    QColor aColor;
    QPushButton* aBtn = dynamic_cast< QPushButton* >( myTable->cellWidget( i, COLOR_COL ) );
    if ( aBtn )
      aColor = aBtn->paletteBackgroundColor();
    theColors[ i ] = aColor;
    
    // Nb markers
    QString aStr = myTable->text( i, NB_MARKERS_COL );
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
const QValueList< int >& Plot2d_SetupCurvesDlg::GetRemovedIndexes() const
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
  int nbRows = myTable->numRows();
  for ( int i = 0; i < nbRows; i++ )
  {
    QString aStr = myTable->text( i, NB_MARKERS_COL );
    bool isOk = false;
    int nbMarkers = aStr.toInt( &isOk );
    if ( !isOk || nbMarkers<= 0 )
    {
      SUIT_MessageBox::info1( (QWidget*)this, tr( "PLOT2D_INSUFFICIENT_DATA" ), 
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
  QValueList< int > toRemove;

  int aCurrRow = myTable->currentRow();
  int aCurrCol = myTable->currentColumn();

  int i, n;
  for ( i = 0, n = myTable->numRows(); i < n; i++ )
  {
    if ( myTable->isRowSelected( i ) || aCurrRow == i )
      toRemove.append( i );
  }

  if ( !toRemove.count() )
    return;

  int nbRemoved = 0;
  QValueList< int >::iterator anIter;
  for ( anIter = toRemove.begin(); anIter != toRemove.end(); ++anIter )
  {
    int aRow = *anIter - nbRemoved;
    myTable->removeRow( aRow );
    myRemovedIndexes.append( aRow );
    nbRemoved++;
  }

  int nbRows = myTable->numRows();
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

  QColor anOldColor = aBtn->paletteBackgroundColor();
  QColor aNewColor = QColorDialog::getColor( anOldColor, this );
  if ( !aNewColor.isValid() )
    return;
    
  setColor( aBtn, aNewColor );
  
  for ( int i = 0, n = myTable->numRows(); i < n; i++ )
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
// Function : updatePixmap
// Purpose  : 
//=============================================================================
void Plot2d_SetupCurvesDlg::updatePixmap( const int theRow )
{
  QValueList< int > aRows;
  if ( theRow < 0 )
  {
    for ( int i = 0, n = myTable->numRows(); i < n; i++ )
      aRows.append( i );
  }
  else 
    aRows.append( theRow );

  QValueList< int >::iterator anIter;
  for ( anIter = aRows.begin(); anIter != aRows.end(); ++anIter )
  {
    int r = *anIter;
    PixmapItem* anItem = (PixmapItem*)myTable->item( r, PIXMAP_COL );
    if ( anItem )
    {
      // Marker type
      QComboTableItem* aCombo = (QComboTableItem*)myTable->item( r, MARKER_COL );
      if ( aCombo )
        anItem->setMarkerType( aCombo->currentItem() );
      
      // Color
      QColor aColor;
      QPushButton* aBtn = dynamic_cast< QPushButton* >( myTable->cellWidget( r, COLOR_COL ) );
      if ( aBtn )
        anItem->setColor( aBtn->paletteBackgroundColor() );

      myTable->updateCell( theRow, PIXMAP_COL );
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
  theBtn->setPaletteBackgroundColor( theColor );

  QPalette aPalette = theBtn->palette();

  /*aPalette.setColor( QColorGroup::Background, theColor );
  aPalette.setColor( QColorGroup::Foreground, theColor );
  aPalette.setColor( QColorGroup::Base, theColor );
  aPalette.setColor( QColorGroup::Text, theColor );
  aPalette.setColor( QColorGroup::Button, theColor );
  aPalette.setColor( QColorGroup::ButtonText, theColor );*/

  //aPalette.setColor( QColorGroup::Light, theColor );
  aPalette.setColor( QColorGroup::Midlight, theColor );
  //aPalette.setColor( QColorGroup::Dark, theColor );
  //aPalette.setColor( QColorGroup::Mid, theColor );
  //aPalette.setColor( QColorGroup::Shadow, theColor );
  
  theBtn->setPalette( aPalette );
}
























