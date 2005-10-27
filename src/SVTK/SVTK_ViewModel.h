#ifndef SVTK_VIEWMODEL_H
#define SVTK_VIEWMODEL_H

#include "SVTK.h"
#include "SVTK_ViewModelBase.h"

#include "VTKViewer_ViewModel.h"

#include "SALOME_Prs.h"
#include "SALOME_InteractiveObject.hxx"

#include <qcolor.h>

class SVTK_ViewWindow;

//! Extends two interfaces #SVTK_ViewModelBase and #SALOME_View 
class SVTK_EXPORT SVTK_Viewer : public SVTK_ViewModelBase, public SALOME_View 
{
  Q_OBJECT;

public:
  typedef SVTK_ViewWindow TViewWindow;
  
  //! Define string representation of the viewer type
  static QString Type() { return "VTKViewer"; }

  SVTK_Viewer();
  virtual ~SVTK_Viewer();

  //! See #SUIT_ViewModel::createView
  virtual SUIT_ViewWindow* createView(SUIT_Desktop*);

  //! See #SUIT_ViewModel::createView
  virtual void setViewManager(SUIT_ViewManager* theViewManager);

  //! See #SUIT_ViewModel::contextMenuPopup
  virtual void contextMenuPopup( QPopupMenu* );

  //! See #SUIT_ViewModel::getType
  virtual QString getType() const { return Type(); }

  //! Get background color of the viewer
  QColor backgroundColor() const;

  //! Set background color to the viewer
  void setBackgroundColor( const QColor& );

  //! Get size of trihedron of the viewer (see #SVTK_Renderer::SetTrihedronSize)
  int trihedronSize() const;

  //! Set size of trihedron of the viewer (see #SVTK_Renderer::SetTrihedronSize)
  void setTrihedronSize( const int );

public:
  void enableSelection(bool isEnabled);
  bool isSelectionEnabled() const { return mySelectionEnabled; }

  void enableMultiselection(bool isEnable);
  bool isMultiSelectionEnabled() const { return myMultiSelectionEnabled; }

  int  getSelectionCount() const;

  /* Reimplemented from SALOME_View */

  //! See #SALOME_View::Display( const SALOME_Prs* )
  void Display( const SALOME_VTKPrs* ); 

  //! See #SALOME_View::Erase( const SALOME_VTKPrs*, const bool = false )
  void Erase( const SALOME_VTKPrs*, const bool = false );

  //! See #SALOME_View::EraseAll( const bool = false )
  void EraseAll( const bool = false );

  //! See #SALOME_View::CreatePrs( const char* entry = 0 )
  SALOME_Prs* CreatePrs( const char* entry = 0 );

  //! See #SALOME_View::BeforeDisplay( SALOME_Displayer* d )
  virtual void BeforeDisplay( SALOME_Displayer* d );

  //! See #SALOME_View::AfterDisplay( SALOME_Displayer* d )
  virtual void AfterDisplay( SALOME_Displayer* d );

  //! See #SALOME_View::isVisible( const Handle(SALOME_InteractiveObject)& )
  virtual bool isVisible( const Handle(SALOME_InteractiveObject)& );

  //! See #SALOME_View::Repaint()
  virtual void Repaint();

protected slots:
  void onMousePress(SUIT_ViewWindow*, QMouseEvent*);
  void onMouseMove(SUIT_ViewWindow*, QMouseEvent*);
  void onMouseRelease(SUIT_ViewWindow*, QMouseEvent*);

  void onDumpView();
  void onShowToolbar();
  void onChangeBgColor();

private:
  QColor myBgColor;
  int    myTrihedronSize;
  bool   mySelectionEnabled;
  bool   myMultiSelectionEnabled;
};

#endif
