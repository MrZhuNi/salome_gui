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
// TableViewer_ViewModel.cxx: implementation of the TableViewer_ViewModel class.

#include "TableViewer_ViewWindow.h"
#include "TableViewer_ViewModel.h"
#include "TableViewer_Tool.h"

#include <SUIT_Desktop.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SUIT_Application.h>
#include <SUIT_ViewManager.h>

#include <HTMLService_HTMLTable.hxx>
#include <HTMLService_HTMLTableCell.hxx>

#include <QtxToolBar.h>
#include <QtxTable.h>
#include <QtxTableInterface.h>

#include <QImage>
#include <QPixmap>
#include <QApplication>
#include <QFont>
#include <QItemSelection>
#include <QItemSelectionModel>

/*!
  Constructor
*/
TableViewer_ViewWindow::TableViewer_ViewWindow( SUIT_Desktop* theDesktop,
                                                TableViewer_Viewer* theModel )
:SUIT_ViewWindow( theDesktop )
{
  myModel = theModel;
  myToolBar = new QtxToolBar( true, tr("LBL_TOOLBAR_LABEL"), this );
}

/*!
  Destructor
*/
TableViewer_ViewWindow::~TableViewer_ViewWindow()
{
}

QtxTableInterface* TableViewer_ViewWindow::table() const
{
  return myTable;
}


/*!
  Internal initialization
*/
void TableViewer_ViewWindow::initLayout()
{
  myTable = createTable();
  QWidget* aWidget = dynamic_cast<QWidget*>( myTable );
  if ( aWidget )
    setCentralWidget( aWidget );

  createActions();
  createToolBar();
}

QImage TableViewer_ViewWindow::dumpView()
{
  QWidget* aWidget = dynamic_cast<QWidget*>( table() );
  return aWidget ? QPixmap::grabWidget( aWidget ).toImage() : QImage();
  //return QPixmap::grabWindow( table()->winId() ).toImage();
}

void TableViewer_ViewWindow::createActions()
{
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();

  createAction( DumpId, tr( "MEN_DUMP" ),
                resMgr->loadPixmap( "TableViewer", tr( "ICON_DUMP" ) ),
                tr( "TIP_DUMP" ), tr( "PRP_DUMP" ) );

  createAction( CopyId, tr( "MEN_COPY" ),
                resMgr->loadPixmap( "STD", tr( "ICON_EDIT_COPY" ) ),
                tr( "TIP_COPY" ), tr( "PRP_COPY" ) );

  createAction( PasteId, tr( "MEN_PASTE" ),
                resMgr->loadPixmap( "STD", tr( "ICON_EDIT_PASTE" ) ),
                tr( "TIP_PASTE" ), tr( "PRP_PASTE" ) );

  createAction( PrintId, tr( "MEN_PRINT" ),
                resMgr->loadPixmap( "STD", tr( "ICON_PRINT" ) ),
                tr( "TIP_PRINT" ), tr( "PRP_PRINT" ) );

  createAction( ExportId, tr( "MEN_EXPORT" ),
                resMgr->loadPixmap( "TableViewer", tr( "ICON_EXPORT" ) ),
                tr( "TIP_EXPORT" ), tr( "PRP_EXPORT" ) );
}

int TableViewer_ViewWindow::numRows( const ContentType type ) const
{
  int aRows = 0;
  switch ( type ) {
    case HorizontalHeader:
      aRows = 1;
      break;
    case Cells:
    case VerticalHeader:
      aRows = myTable->rowCount();
      break;
    default:
      break;
  }
  return aRows;
}

int TableViewer_ViewWindow::numCols( const ContentType type ) const
{
  int aCols = 0;
  switch ( type ) {
    case VerticalHeader:
      aCols = 1;
      break;
    case Cells:
    case HorizontalHeader:
      aCols = myTable->columnCount();
      break;
    default:
      break;
  }
  return aCols;
}

QString TableViewer_ViewWindow::text( const ContentType type, const int row, const int col ) const
{
  QVariant aValue = value( type, row, col, Qt::DisplayRole );
  return aValue.isValid() ? aValue.toString() : "";
}

QString TableViewer_ViewWindow::image( const ContentType, const int, const int ) const
{
  return "";
}

QFont TableViewer_ViewWindow::font( const ContentType type, const int row,
                                    const int col ) const
{
  QVariant aValue = value( type, row, col, Qt::FontRole );

  QFont aFont;
  if ( aValue.isValid() && aValue.canConvert( QVariant::Font ) )
    aFont = aValue.value<QFont>();
  else {
    QWidget* aWidget = dynamic_cast<QWidget*>( myTable );
    if ( aWidget )
      aFont = aWidget->font();
  }
  return aFont;
}

int TableViewer_ViewWindow::fontFlags( const ContentType type, const int row, const int col ) const
{
  int aFlags = 0;
  QFont aFont = font( type, row, col );
  if ( aFont.bold() )
    aFlags |= HTMLService_HTMLText::Bold;
  if ( aFont.italic() )
    aFlags |= HTMLService_HTMLText::Bold;
  if ( aFont.underline() )
    aFlags |= HTMLService_HTMLText::Underline;
  if ( aFont.strikeOut() )
    aFlags |= HTMLService_HTMLText::StrikeOut;
  // 'Subscript' type is absent for export in HTML file

  return aFlags;
}

QColor TableViewer_ViewWindow::foregroundColor( const ContentType type, const int row,
                                                const int col ) const
{
  QVariant aValue = value( type, row, col, Qt::ForegroundRole );

  QColor aColor;
  if ( aValue.isValid() && aValue.canConvert( QVariant::Color ) )
    aColor = aValue.value<QColor>();

  return aColor;
}

QColor TableViewer_ViewWindow::backgroundColor( const ContentType type, const int row,
                                                const int col ) const
{
  QVariant aValue = value( type, row, col, Qt::BackgroundRole );

  QColor aColor;
  if ( aValue.isValid() && aValue.canConvert( QVariant::Color ) )
    aColor = aValue.value<QColor>();
  return aColor;
}

QVariant TableViewer_ViewWindow::value( const ContentType theType, const int theRow,
                                        const int theColumn, const int theRole ) const
{
  QVariant aValue;
  switch ( theType ) {
    case VerticalHeader:
    case HorizontalHeader: {
      bool aHor = theType == HorizontalHeader;
      aValue = myTable->headerData( aHor ? theColumn : theRow,
                                    aHor ? Qt::Horizontal : Qt::Vertical,
                                    theRole );
    }
    break;
    case Cells:
      aValue = myTable->data( theRow, theColumn, theRole );
      break;
    default:
      break;
  }
  return aValue;
}

void TableViewer_ViewWindow::createToolBar()
{
  myToolBar->addAction( myActionsMap[DumpId] );
  myToolBar->addAction( myActionsMap[CopyId] );
  myToolBar->addAction( myActionsMap[PasteId] );
  myToolBar->addAction( myActionsMap[PrintId] );
  myToolBar->addAction( myActionsMap[ExportId] );
}

void TableViewer_ViewWindow::actionActivated( const int id )
{
  switch ( id ) {
    case DumpId:
       onDumpView();
      break;
    case ExportId:
      exportData();
      break;
    case CopyId:
      copyData();
      break;
    case PasteId:
      pasteData();
      break;
    case PrintId:
      printData();
      break;
    default:
      break;
  }
}

void TableViewer_ViewWindow::registerAction( const int id, QtxAction* a )
{
  if ( !a )
    return;

  myActionsMap.insert( id, a );
  connect( a, SIGNAL( triggered() ), this, SLOT( onActivated() ) );
}

QtxAction* TableViewer_ViewWindow::createAction( const int id, const QString& menu, const QPixmap& ico,
                                         const QString& tip, const QString& status,
                                         const int accel, QObject* parent )
{
  QtxAction* a = new QtxAction( menu, ico, menu, accel, parent ? parent : myDesktop );
  a->setToolTip( tip );
  a->setStatusTip( status );
  registerAction( id, a );
  return a;
}

void TableViewer_ViewWindow::selectionChanged()
{
  bool anEnable = myTable->getSelectedIndexes().count() > 0;
  myActionsMap[CopyId]->setEnabled( anEnable );
  myActionsMap[PasteId]->setEnabled( anEnable && myCopyLst.count() > 0 );
}

void TableViewer_ViewWindow::onActivated()
{
  const QObject* obj = sender();
  const QtxAction* a = ::qobject_cast<const QtxAction*>( obj );
  if ( !a )
    return;

  int id = -1;
  for ( ActionsMap::const_iterator it = myActionsMap.begin();
        it != myActionsMap.end() && id == -1; ++it ) {
    if ( it.value() == a )
      id = it.key();
  }

  if ( id == -1 )
    return;

  actionActivated( id );
}

void TableViewer_ViewWindow::exportData()
{
  SUIT_Application* app = SUIT_Session::session()->activeApplication();
  QString fileName = app->getFileName( false, QString::null, tr( "EXPORT_FILTER" ),
                                       tr( "EXPORT_TABLE" ), 0 );
  if ( fileName.isEmpty() )
    return;

  exportData( fileName );
}

Handle(HTMLService_HTMLTable) TableViewer_ViewWindow::exportToHTML()
{
  int rows = numRows( Cells );
  int cols = numCols( Cells );
  int horOffset = numRows( HorizontalHeader );
  int verOffset = numCols( VerticalHeader );

  Handle(HTMLService_HTMLTable) table = new HTMLService_HTMLTable( rows + horOffset,
                                                                   cols + verOffset );
  QString title = getViewManager() ? getViewManager()->getTitle() : "";
  SUIT_ViewWindow* anActiveWnd = getViewManager() ? getViewManager()->getActiveView() : 0;
  if ( anActiveWnd )
    title = anActiveWnd->windowTitle();

  table->SetTitle( TableViewer_Tool::ToExtString( title ) );
  table->SetWidth( 100, true );
  table->SetBorder( 2 );

  exportTableData( table, HorizontalHeader, 0, verOffset );
  exportTableData( table, VerticalHeader, horOffset, 0 );
  exportTableData( table, Cells, horOffset, verOffset );

  Handle(HTMLService_HTMLTableCell) ltCell = table->GetCell( 0, 0 );
  if ( !ltCell.IsNull() ) {
    ltCell->SetHeaderCell( true );
    ltCell->SetBackgroundColor( Quantity_NOC_GRAY );
  }

  return table;
}

void TableViewer_ViewWindow::exportData( const QString& theFileName )
{
  QApplication::setOverrideCursor( Qt::WaitCursor );

  exportToHTML()->GenerateFile( TableViewer_Tool::ToExtString( theFileName ), true );

  QApplication::restoreOverrideCursor();
}

void TableViewer_ViewWindow::copyData()
{
  QModelIndexList anItems = myTable->getSelectedIndexes();
  if ( anItems.count() <= 0 )
    return;
  int aLeftCol = myTable->columnCount(), aTopRow = myTable->rowCount();
  QModelIndexList::const_iterator anIt = anItems.begin(), aLast = anItems.end();
  int aRow, aCol;
  for ( ; anIt != aLast; ++anIt ) {
    aRow = (*anIt).row();
    aCol = (*anIt).column();
    if ( !canPaste( aRow, aCol, "" ) )
      continue;
    if ( aCol < aLeftCol )
      aLeftCol = aCol;
    if ( aRow < aTopRow )
      aTopRow = aRow;
  }
  myCopyLst.clear();
  TableDataItem aCopyItem;
  for ( anIt = anItems.begin(); anIt != aLast; ++anIt ) {
    aRow = (*anIt).row();
    aCol = (*anIt).column();
    if ( !canCopy( aRow, aCol ) )
      continue;
    aCopyItem.myRow = aRow-aTopRow;
    aCopyItem.myCol = aCol-aLeftCol;
    aCopyItem.myText = value( Cells, aRow, aCol, Qt::DisplayRole );
    aCopyItem.myBgCol = value( Cells, aRow, aCol, Qt::BackgroundRole );
    aCopyItem.myFgCol = value( Cells, aRow, aCol, Qt::ForegroundRole );
    aCopyItem.myFont = value( Cells, aRow, aCol, Qt::FontRole );
    myCopyLst.append( aCopyItem );
  }
}

void TableViewer_ViewWindow::pasteData()
{
  QModelIndexList anItems = myTable->getSelectedIndexes();
  if ( anItems.count() <= 0 || myCopyLst.count() <= 0 )
    return;
  int aLeftCol = myTable->columnCount(), aTopRow = myTable->rowCount();
  QModelIndexList::const_iterator anIt = anItems.begin(), aLast = anItems.end();
  int aCol, aRow;
  for ( ; anIt != aLast; ++anIt ) {
    aRow = (*anIt).row();
    aCol = (*anIt).column();
    if ( !canPaste( aRow, aCol, "" ) )
      continue;
    if ( aCol < aLeftCol )
      aLeftCol = aCol;
    if ( aRow < aTopRow )
      aTopRow = aRow;
  }
  QList<TableDataItem>::const_iterator aCopyIt = myCopyLst.begin(),
                                       aCopyLast = myCopyLst.end();
  //int aCol, aRow;
  TableDataItem aCopyItem;
  for ( ; aCopyIt != aCopyLast; aCopyIt++ ) {
    aCopyItem = *aCopyIt;
    aCol = aCopyItem.myCol+aLeftCol;
    aRow = aCopyItem.myRow+aTopRow;
    if ( !canPaste( aRow, aCol, aCopyItem.myText.toString() ) )
      continue;

    myTable->setData( aRow, aCol, aCopyItem.myText, Qt::DisplayRole );
    myTable->setData( aRow, aCol, aCopyItem.myBgCol, Qt::BackgroundRole );
    myTable->setData( aRow, aCol, aCopyItem.myFgCol, Qt::ForegroundRole );
    myTable->setData( aRow, aCol, aCopyItem.myFont, Qt::FontRole );
  }
}

/**
 *
 */
bool TableViewer_ViewWindow::canPasteData()
{
  QModelIndexList anItems = myTable->getSelectedIndexes();
  if ( anItems.count() <= 0 || myCopyLst.count() <= 0 )
    return false;

  bool aCanPaste = true;
  int aLeftCol = myTable->columnCount(), aTopRow = myTable->rowCount();
  QModelIndexList::const_iterator anIt = anItems.begin(), aLast = anItems.end();
  int aCol, aRow;
  for ( ; anIt != aLast; ++anIt ) {
    aRow = (*anIt).row();
    aCol = (*anIt).column();
    if ( !canPaste( aRow, aCol, "" ) )
      continue;
    if ( aCol < aLeftCol )
      aLeftCol = aCol;
    if ( aRow < aTopRow )
      aTopRow = aRow;
  }
  QList<TableDataItem>::const_iterator aCopyIt = myCopyLst.begin(),
                                       aCopyLast = myCopyLst.end();
  //int aCol, aRow;
  TableDataItem aCopyItem;
  for ( ; aCopyIt != aCopyLast && aCanPaste; aCopyIt++ ) {
    aCopyItem = *aCopyIt;
    aCol = aCopyItem.myCol+aLeftCol;
    aRow = aCopyItem.myRow+aTopRow;
    aCanPaste = canPaste( aRow, aCol, aCopyItem.myText.toString() );
  }
  return aCanPaste;
}

void TableViewer_ViewWindow::printData()
{
  QImage img = dumpView();
  printImage( img, this );
}

void TableViewer_ViewWindow::exportTableData( Handle(HTMLService_HTMLTable)& table,
                                              const ContentType type,
                                              const int rowOffset, const int colOffset )
{
  if ( table.IsNull() )
    return;

  bool isExpImg = true;
  bool dotToComma = false;
  int precision;
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  if ( resMgr ) {
    isExpImg = resMgr->booleanValue( "TableViewer", "export_table_images", true );
    dotToComma = resMgr->booleanValue( "TableViewer", "replace_dot_by_comma", true );
    precision = resMgr->integerValue( "TableViewer", "number_precision", 6 );
  }

  for ( int c = 0, nbC = numCols( type ); c < nbC; c++ ) {
    for ( int r = 0, nbR = numRows( type ); r < nbR; r++ ) {
      Handle(HTMLService_HTMLTableCell) cell = 
         table->GetCell( r + rowOffset, c + colOffset );
      if ( cell.IsNull() )
        continue;
      cell->SetHeaderCell( type != Cells );
      if ( cell->IsHeaderCell() )
        cell->SetBackgroundColor( Quantity_NOC_GRAY );
      if ( isExpImg ) {
        QString img = image( type, r, c );
        if ( !img.isEmpty() )
          cell->InsertImage( TableViewer_Tool::ToExtString( img ), 32, 32 );
      }
      QString txt = text( type, r, c );
      if ( !txt.isEmpty() ) {
        bool aDbl = false;
        double aVal = TableViewer_Tool::ToDouble( txt, aDbl );
        if ( aDbl )
          cell->InsertText( TableViewer_Tool::ToExtString( 
                            TableViewer_Tool::DoubleToQString( aVal,
                                     precision, false, dotToComma ) ) );
        else
          cell->InsertText( TableViewer_Tool::ToExtString( txt ) );
      }
      if ( foregroundColor( type, r, c ).isValid() )
        cell->SetForegroundColor( TableViewer_Tool::Color( foregroundColor( type, r, c ) ) );
      if ( backgroundColor( type, r, c ).isValid() )
        cell->SetBackgroundColor( TableViewer_Tool::Color( backgroundColor( type, r, c ) ) );
      cell->SetFontFlags( fontFlags( type, r, c ) );
    }
  }
}

bool TableViewer_ViewWindow::canCopy( const int theRow, const int theCol )
{
  return true;
}

bool TableViewer_ViewWindow::canPaste( const int theRow, const int theCol, const QString& )
{
  return theRow < myTable->rowCount() && theRow >= 0 &&
         theCol < myTable->columnCount() & theCol >= 0;
}

QtxTableInterface* TableViewer_ViewWindow::createTable()
{
  QtxTableInterface* aTable = new QtxTable( this );
  connect( aTable->selectionModel(), SIGNAL( selectionChanged(
                       const QItemSelection&, const QItemSelection& ) ),
           this, SLOT( selectionChanged() ) );
  return aTable;
}

