#ifndef SVTK_VIEWMODEL_H
#define SVTK_VIEWMODEL_H

#include "SVTK.h"
#include "SVTK_ViewModelBase.h"

#include "VTKViewer_ViewModel.h"

#include "SALOME_Prs.h"
#include "SALOME_InteractiveObject.hxx"

#include <qcolor.h>

class SVTK_ViewWindow;

class SVTK_EXPORT SVTK_Viewer : public SVTK_ViewModelBase, public SALOME_View 
{
  Q_OBJECT;

public:
  typedef SVTK_ViewWindow TViewWindow;
  static QString Type() { return "VTKViewer"; }

  SVTK_Viewer();
  virtual ~SVTK_Viewer();

  virtual SUIT_ViewWindow* createView(SUIT_Desktop*);

  virtual void             setViewManager(SUIT_ViewManager* theViewManager);
  virtual void             contextMenuPopup( QPopupMenu* );
  virtual QString          getType() const { return Type(); }

  QColor backgroundColor() const;
  void   setBackgroundColor( const QColor& );

  int    trihedronSize() const;
  void   setTrihedronSize( const int );

public:
  void enableSelection(bool isEnabled);
  bool isSelectionEnabled() const { return mySelectionEnabled; }

  void enableMultiselection(bool isEnable);
  bool isMultiSelectionEnabled() const { return myMultiSelectionEnabled; }

  int  getSelectionCount() const;

  /* Reimplemented from SALOME_View */
  void                           Display( const SALOME_VTKPrs* );
  void                           Erase( const SALOME_VTKPrs*, const bool = false );
  void                           EraseAll( const bool = false );
  SALOME_Prs*                    CreatePrs( const char* entry = 0 );
  virtual void                   BeforeDisplay( SALOME_Displayer* d );
  virtual void                   AfterDisplay ( SALOME_Displayer* d );
  virtual bool                   isVisible( const Handle(SALOME_InteractiveObject)& );
  virtual void                   Repaint();

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
