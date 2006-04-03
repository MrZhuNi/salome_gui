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
// See http://www.salome-platform.org/
//
#ifndef STD_APPLICATION_H
#define STD_APPLICATION_H

#include "STD.h"

#include <SUIT_Application.h>

#include <SUIT_Desktop.h>
#include <SUIT_ViewManager.h>

#include <qmap.h>
#include <qptrlist.h>

class QToolBar;
class QtxAction;
class QPopupMenu;
class SUIT_Operation;
class SUIT_ViewWindow;
class SUIT_ToolWindow;

typedef QPtrList<SUIT_ViewManager> ViewManagerList;

#if defined WIN32
#pragma warning( disable: 4251 )
#endif

class STD_EXPORT STD_Application : public SUIT_Application
{
  Q_OBJECT

public:
  STD_Application();
  virtual ~STD_Application();

  virtual QString       applicationName() const;

  virtual bool          isPossibleToClose();
  virtual bool          useFile( const QString& );

  virtual void          createEmptyStudy();

  void                  setEditEnabled( const bool );
  bool                  isEditEnabled() const { return myEditEnabled; }

  void                  clearViewManagers();
  virtual void          addViewManager( SUIT_ViewManager* );
  virtual void          removeViewManager( SUIT_ViewManager* );

  SUIT_ViewManager*     activeViewManager() const;
  SUIT_ViewManager*     viewManager( const QString& ) const;

  bool                  containsViewManager( SUIT_ViewManager* ) const;

  ViewManagerList       viewManagers() const;
  void                  viewManagers( ViewManagerList& ) const;
  void                  viewManagers( const QString&, ViewManagerList& ) const;

  virtual QString       getFileFilter() const { return QString::null; }
  virtual QString       getFileName( bool open, const QString& initial, const QString& filters, 
				     const QString& caption, QWidget* parent );
  QString               getDirectory( const QString& initial, const QString& caption, QWidget* parent );

  virtual void          start();

  virtual void          closeApplication();

  virtual void          contextMenuPopup( const QString&, QPopupMenu*, QString& ) {}

signals:
  /*!emit that view manager added*/
  void                  viewManagerAdded( SUIT_ViewManager* );
  /*!emit that view manager removed*/
  void                  viewManagerRemoved( SUIT_ViewManager* );
  /*!emit that view manager activated*/
  void                  viewManagerActivated( SUIT_ViewManager* );

public slots:
  virtual void          onNewDoc();
  virtual void          onCloseDoc( bool ask = true );
  virtual void          onSaveDoc();
  virtual bool          onSaveAsDoc();

  virtual void          onOpenDoc();
  virtual bool          onOpenDoc( const QString& );

  virtual void          onLoadDoc();
  virtual bool          onLoadDoc( const QString& );

  virtual void          onExit();

  virtual void          onCopy();
  virtual void          onPaste();

  virtual void          onViewStatusBar( bool );

  virtual void          onHelpAbout();

  virtual void          onDesktopClosing( SUIT_Desktop*, QCloseEvent* );
  virtual void          onConnectPopupRequest( SUIT_PopupClient*, QContextMenuEvent* );

private slots:
  virtual void          onViewManagerActivated( SUIT_ViewManager* );

public:
  enum {  MenuFileId = 1,
          MenuViewId = 2,
          MenuEditId = 3,
          MenuHelpId = 7
       };

  enum {  FileNewId,   FileOpenId,   FileLoadId, FileCloseId,
	  FileSaveId,  FileSaveAsId, FileExitId, 
	  ViewStatusBarId, ViewWindowsId, NewWindowId,
          EditCutId, EditCopyId, EditPasteId,
          HelpAboutId,
	  UserID
       };
 
protected:
  virtual void          createActions();
  virtual void          updateDesktopTitle();
  virtual void          updateCommandsStatus();

  virtual void          setDesktop( SUIT_Desktop* );

  virtual void          loadPreferences();
  virtual void          savePreferences();

  virtual void          studySaved( SUIT_Study* );
  virtual void          studyOpened( SUIT_Study* );
  virtual void          studyCreated( SUIT_Study* );

  virtual void          beforeCloseDoc( SUIT_Study* theDoc );
  virtual void          afterCloseDoc();

  virtual void          setActiveViewManager( SUIT_ViewManager* );

private:
  ViewManagerList       myViewMgrs;
  SUIT_ViewManager*     myActiveViewMgr;

private:
  bool                  myEditEnabled;
  bool                  myClosePermanently;
};

#if defined WIN32
#pragma warning( default: 4251 )
#endif

#endif
