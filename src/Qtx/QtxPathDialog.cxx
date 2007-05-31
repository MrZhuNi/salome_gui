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
// File:      QtxPathDialog.cxx
// Author:    Sergey TELKOV

#include "QtxPathDialog.h"

#include "QtxGridBox.h"
#include "QtxGroupBox.h"

#include <QDir>
#include <QLabel>
#include <QPixmap>
#include <QLayout>
#include <QLineEdit>
#include <QObjectList>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

static const char* open_icon[] = {
"16 16 5 1",
"  c none",
". c #ffff00",
"# c #848200",
"a c #ffffff",
"b c #000000",
"                ",
"          bbb   ",
"         b   b b",
"              bb",
"  bbb        bbb",
" ba.abbbbbbb    ",
" b.a.a.a.a.b    ",
" ba.a.a.a.ab    ",
" b.a.abbbbbbbbbb",
" ba.ab#########b",
" b.ab#########b ",
" bab#########b  ",
" bb#########b   ",
" bbbbbbbbbbb    ",
"                ",
"                "
};

/*!
  Constructor.
*/
QtxPathDialog::QtxPathDialog( const bool import, QWidget* parent, const bool modal,
                              const bool resize, const int buttons, Qt::WindowFlags f )
: QtxDialog( parent, modal, resize, buttons, f ),
myDefault( -1 ),
myEntriesFrame( 0 ),
myOptionsFrame( 0 )
{
	initialize();

	setWindowTitle( tr( import ? "Open file" : "Save file" ) );

	setDefaultEntry( createFileEntry( tr( "File name" ), import ? OpenFile : SaveFile ) );
	QLineEdit* le = fileEntry( defaultEntry() );
	if ( le )
		le->setMinimumWidth( 200 );

	validate();

	setFocusProxy( le );

  updateVisibility();
}

/*!
  Constructor.
*/
QtxPathDialog::QtxPathDialog( QWidget* parent, const bool modal,
                              const bool resize, const int buttons, Qt::WindowFlags f )
: QtxDialog( parent, modal, resize, buttons, f ),
myDefault( -1 ),
myEntriesFrame( 0 ),
myOptionsFrame( 0 )
{
	initialize();

  updateVisibility();
}

/*!
  Destructor.
*/
QtxPathDialog::~QtxPathDialog()
{
}

/*!
  \return file name
*/
QString QtxPathDialog::fileName() const
{
	return fileName( defaultEntry() );
}

/*!
  Sets file name
  \param txt - new file name
  \param autoExtension - auto extension determination by file
*/
void QtxPathDialog::setFileName( const QString& txt, const bool autoExtension )
{
	setFileName( defaultEntry(), txt, autoExtension );
}

/*!
  \return filter
*/
QString QtxPathDialog::filter() const
{
	return filter( defaultEntry() );
}

/*!
  Changes filter (filter is a list of masks, separated by ';;')
  \param fltr - new filter
*/
void QtxPathDialog::setFilter( const QString& fltr )
{
	setFilter( defaultEntry(), fltr );
}

/*!
  Shows/hides path dialog
*/
void QtxPathDialog::setVisible( bool on )
{
  if ( on )
    updateVisibility();

	QtxDialog::setVisible( on );
}

/*!
  SLOT: called if user click button to show standard file dialog
*/
void QtxPathDialog::onBrowse()
{
	const QObject* obj = sender();

	int id = -1;

	for ( FileEntryMap::Iterator it = myEntries.begin(); it != myEntries.end() && id == -1; ++it )
  {
		if ( it.value().btn == obj )
			id = it.key();
  }

	if ( id == -1 )
		return;

	FileEntry& entry = myEntries[id];

	bool isDir = entry.mode != OpenFile && entry.mode != SaveFile;

	if ( !entry.dlg )
	{
		entry.dlg = new QFileDialog( this, windowTitle(), QDir::current().path() );
		switch ( entry.mode )
		{
		case NewDir:
		case OpenDir:
		case SaveDir:
			isDir = true;
			entry.dlg->setFileMode( QFileDialog::DirectoryOnly );
			break;
		case SaveFile:
			entry.dlg->setFileMode( QFileDialog::AnyFile );
			break;
		case OpenFile:
		default:
			entry.dlg->setFileMode( QFileDialog::ExistingFiles );
			break;
		}
	}

	if ( !isDir )
  {
    QStringList fList = prepareFilters( entry.filter );
    if ( !fList.isEmpty() )
		  entry.dlg->setFilters( fList );
  }
	entry.dlg->selectFile( fileName( id ) );

	if ( entry.dlg->exec() != Accepted )
		return;

	QStringList fileList = entry.dlg->selectedFiles();
  QString fName = !fileList.isEmpty() ? fileList.first() : QString();

	if ( fName.isEmpty() )
		return;

  if ( Qtx::extension( fName ).isEmpty() && !isDir )
		fName = autoExtension( fName, entry.dlg->selectedFilter() );

	fName = QDir::convertSeparators( fName );
	QString prev = QDir::convertSeparators( fileName( id ) );
	if ( isDir )
	{
		while ( prev.length() && prev.at( prev.length() - 1 ) == QDir::separator() )
			prev.remove( prev.length() - 1, 1 );
		while ( fName.length() && fName.at( fName.length() - 1 ) == QDir::separator() )
			fName.remove( fName.length() - 1, 1 );
	}

	if ( prev == fName )
		return;

	setFileName( id, fName );
	fileNameChanged( id, fName );

	if ( id == defaultEntry() )
		emit fileNameChanged( fName );
}

/*!
  SLOT: called if user presses RETURN in line edit
*/
void QtxPathDialog::onReturnPressed()
{
	const QObject* obj = sender();

	int id = -1;
	for ( FileEntryMap::Iterator it = myEntries.begin(); it != myEntries.end() && id == -1; ++it )
  {
		if ( it.value().edit == obj )
			id = it.key();
  }

	if ( id == -1 )
		return;

	fileNameChanged( id, fileName( id ) );

	if ( id == defaultEntry() )
		emit fileNameChanged( fileName() );
}

/*!
  SLOT: called if text in line edit is changed
*/
void QtxPathDialog::onTextChanged( const QString& )
{
	validate();
}

/*!
  Checks validity of text and according to it enables/disables OK, Yes buttons
*/
void QtxPathDialog::validate()
{
	setButtonEnabled( isValid(), OK | Yes );
}

/*!
  \return true if selected file is valid
*/
bool QtxPathDialog::isValid()
{
	bool ok = true;
	for ( FileEntryMap::Iterator it = myEntries.begin(); it != myEntries.end() && ok; ++it )
  {
		if ( it.value().edit->isEnabled() )
			ok = !it.value().edit->text().trimmed().isEmpty();
  }

	return ok;
}

/*!
  \return true entered data is accepted
*/
bool QtxPathDialog::acceptData() const
{
	bool ok = true;

	QWidget* parent = (QWidget*)this;

	FileEntryMap::ConstIterator it;
	for ( it = myEntries.begin(); it != myEntries.end() && ok; ++it )
	{
		const FileEntry& entry = it.value();
		QFileInfo fileInfo( entry.edit->text() );
		if ( entry.edit->text().isEmpty() )
		{
			QMessageBox::critical( parent, windowTitle(), tr( "File name not specified" ),
								   QMessageBox::Ok, QMessageBox::NoButton );
			ok = false;
		}
		else switch ( entry.mode )
		{
		case OpenFile:
			if ( !fileInfo.exists() )
			{
				QMessageBox::critical( parent, windowTitle(), tr( "File \"%1\" does not exist" ).arg( fileInfo.filePath() ),
									   QMessageBox::Ok, QMessageBox::NoButton );
				ok = false;
			}
			break;
		case SaveFile:
			if ( fileInfo.exists() )
				ok = QMessageBox::warning( parent, windowTitle(), tr( "File \"%1\" already exist. Do you want to overwrite it?" ).arg( fileInfo.filePath() ),
										   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes;
			break;
		case OpenDir:
			if ( !fileInfo.exists() || !fileInfo.isDir() )
			{
				QMessageBox::critical( parent, windowTitle(), tr( "Directory \"%1\" does not exist" ).arg( fileInfo.filePath() ),
									   QMessageBox::Ok, QMessageBox::NoButton );
				ok = false;
			}
			break;
		case SaveDir:
			if ( fileInfo.exists() && !fileInfo.isDir() )
			{
				QMessageBox::critical( parent, windowTitle(), tr( "Directory \"%1\" can't be created because file with the same name exist" ).arg( fileInfo.filePath() ),
									   QMessageBox::Ok, QMessageBox::NoButton );
				ok = false;
			}
			break;
		case NewDir:
			if ( fileInfo.exists() )
			{
				if ( !fileInfo.isDir() )
				{
					QMessageBox::critical( parent, windowTitle(), tr( "Directory \"%1\" can't be created because file with the same name exist" ).arg( fileInfo.filePath() ),
										   QMessageBox::Ok, QMessageBox::NoButton );
					ok = false;
				}
				else if ( QDir( fileInfo.filePath() ).count() > 2 )
					ok = QMessageBox::warning( parent, windowTitle(), tr( "Directory \"%1\" not empty. Do you want to remove all files in this directory?" ).arg( fileInfo.filePath() ),
											   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes;
			}
			break;
		default:
			break;
		}

		if ( !ok )
			entry.edit->setFocus();
	}

	return ok;
}

/*!
  Some custom activity on file name changing (must be redefined, default implementation is empty
*/
void QtxPathDialog::fileNameChanged( int, QString )
{
}

/*!
  \return frame with options
*/
QFrame* QtxPathDialog::optionsFrame()
{
	return myOptionsFrame;
}

/*!
  \return file name
  \param id - id of file entry
*/
QString QtxPathDialog::fileName( const int id ) const
{
	QString res;
	if ( myEntries.contains( id ) )
		res = myEntries[id].edit->text();
	return res;
}

/*!
  Change file name of file entry
  \param id - id of file entry
  \param txt - new file name
  \param autoExt - assign extension automatically
*/
void QtxPathDialog::setFileName( const int id, const QString& txt, const bool autoExt )
{
	int mode;
	QLineEdit* le = fileEntry( id, mode );

	if ( le )
	{
		if ( autoExt && ( mode == OpenFile || mode == SaveFile ) )
			le->setText( autoExtension( txt, filter( id ) ) );
		else
			le->setText( txt );
	}
}

/*!
  \return filter
  \param id - id of file entry
*/
QString QtxPathDialog::filter( const int id ) const
{
	QString res;
	if ( myEntries.contains( id ) )
		res = myEntries[id].filter;
	return res;
}

void QtxPathDialog::setFilter( const int id, const QString& filter )
{
	if ( myEntries.contains( id ) )
		myEntries[id].filter = filter;
}

/*!
  \return line edit of file entry
  \param id - id of file entry
*/
QLineEdit* QtxPathDialog::fileEntry( const int id ) const
{
	QLineEdit* le = 0;
	if ( myEntries.contains( id ) )
		le = myEntries[id].edit;

	return le;
}

/*!
  \return line edit and mode of file entry
  \param id - id of file entry
  \param theMode - for return mode of file entry
*/
QLineEdit* QtxPathDialog::fileEntry( const int theId, int& theMode ) const
{
	QLineEdit* le = 0;
	if ( myEntries.contains( theId ) )
	{
		le = myEntries[theId].edit;
		theMode = myEntries[theId].mode;
	}

	return le;
}

/*!
  Creates file entry
  \return id of just created file entry
  \param lab - title of entry
  \param mode - mode of entry
  \param id - proposed id (if it is -1, then id will be chosen automatically)
*/
int QtxPathDialog::createFileEntry( const QString& lab, const int mode, const QString& filter, const int id )
{
	int num = id;
	if ( num == -1 )
	{
		num--;
		while ( myEntries.contains( num ) )
			num--;
	}

	FileEntry entry;
	entry.dlg = 0;
	entry.mode = mode;
  entry.filter = filter;

	new QLabel( lab, myEntriesFrame );
	entry.edit = new QLineEdit( myEntriesFrame );
	entry.btn = new QPushButton( myEntriesFrame );
	entry.btn->setAutoDefault( false );
	entry.btn->setIcon( QPixmap( open_icon ) );

	connect( entry.btn, SIGNAL( clicked() ), this, SLOT( onBrowse() ) );
	connect( entry.edit, SIGNAL( returnPressed() ), this, SLOT( onReturnPressed() ) );
	connect( entry.edit, SIGNAL( textChanged( const QString& ) ), this, SLOT( onTextChanged( const QString& ) ) );

	myEntries.insert( num, entry );

	return num;
}

/*!
  \return id of default entry
*/
int QtxPathDialog::defaultEntry() const
{
	return myDefault;
}

/*!
  Change default entry id
  \param id - new default entry id
*/
void QtxPathDialog::setDefaultEntry( const int id )
{
	myDefault = id;
}

/*!
  Initialize dialog layout
*/
void QtxPathDialog::initialize()
{
	setWindowTitle( tr( "File dialog" ) );

	QVBoxLayout* main = new QVBoxLayout( mainFrame() );
  main->setMargin( 0 );

	QtxGroupBox* base = new QtxGroupBox( "", mainFrame() );
	main->addWidget( base );

  QtxGridBox*  mainGroup = new QtxGridBox( 1, Qt::Horizontal, base, 0 );
  base->setWidget( mainGroup );

	myEntriesFrame = new QtxGridBox( 3, Qt::Horizontal, mainGroup );
	myOptionsFrame = new QFrame( mainGroup );
}

/*!
  \return list of filters
*/
QStringList QtxPathDialog::prepareFilters( const QString& filter ) const
{
	QStringList res;
  bool allFilter = false;
	if ( !filter.isEmpty() )
	{
		res = filter.split( ";;" );
		for ( QStringList::ConstIterator it = res.begin(); it != res.end() && !allFilter; ++it )
		{
			QStringList wildCards = filterWildCards( *it );
			allFilter = wildCards.indexOf( "*.*" ) != -1;
		}
	}

  if ( !allFilter )
	  res.append( tr( "All files (*.*)" ) );

	return res;
}

/*!
  \return list of filters with filtered wild cards
*/
QStringList QtxPathDialog::filterWildCards( const QString& theFilter ) const
{
	QStringList res;

	int b = theFilter.lastIndexOf( "(" );
	int e = theFilter.lastIndexOf( ")" );
	if ( b != -1 && e != -1 )
	{
		QString content = theFilter.mid( b + 1, e - b - 1 ).trimmed();
		QStringList lst = content.split( " " );
		for ( QStringList::ConstIterator it = lst.begin(); it != lst.end(); ++it )
    {
			if ( (*it).indexOf( "." ) != -1 )
				res.append( (*it).trimmed() );
    }
	}
	return res;
}

/*!
  \return file name with assigned extension
  \param theFileName - source file name
  \param theFilter - list of filters
*/
QString QtxPathDialog::autoExtension( const QString& theFileName, const QString& theFilter ) const
{
	QString fName = theFileName;

	if ( fName.isEmpty() )
		return fName;

	QString filter;
  QStringList filters = prepareFilters( theFilter );
  if ( !filters.isEmpty() )
    filter = filters.first();

	QStringList wildCards = filterWildCards( filter );
	if ( !wildCards.isEmpty() )
	{
		QString ext = wildCards.first();
		if ( ext.indexOf( "." ) != -1 )
			ext = ext.mid( ext.indexOf( "." ) + 1 );

		if ( !ext.isEmpty() && !ext.contains( "*" ) )
			fName = QDir::convertSeparators( fName ) + QString( "." ) + ext;
	}

	return fName;
}

/*!
  \return true if widget has visible children
  \param wid - widget
*/
bool QtxPathDialog::hasVisibleChildren( QWidget* wid ) const
{
	bool res = false;
	if ( wid )
	{
		const QObjectList& aChildren = wid->children();
    for ( QObjectList::const_iterator it = aChildren.begin(); it != aChildren.end() && !res; ++it )
		{
		  if ( (*it)->isWidgetType() )
				res = ((QWidget*)(*it))->isVisibleTo( wid );
    }
	}
	return res;
}

void QtxPathDialog::updateVisibility()
{
  if ( hasVisibleChildren( myEntriesFrame ) )
	  myEntriesFrame->show();
  else
	  myEntriesFrame->hide();

  if ( hasVisibleChildren( myOptionsFrame ) )
	  myOptionsFrame->show();
  else
	  myOptionsFrame->hide();
}
