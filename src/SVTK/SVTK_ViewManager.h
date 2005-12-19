#ifndef SVTK_VIEWMANAGER_H
#define SVTK_VIEWMANAGER_H

#include "SUIT_ViewManager.h"
#include "SVTK.h"

class SUIT_Desktop;

//! Extend SUIT_ViewManager to deal with SVTK_Viewer
class SVTK_EXPORT SVTK_ViewManager : public SUIT_ViewManager
{
  Q_OBJECT

public:
  //! Construct the view manager
  SVTK_ViewManager( SUIT_Study* study, SUIT_Desktop* );

  //! Destroy the view manager
  virtual ~SVTK_ViewManager();

protected:
  void setViewName( SUIT_ViewWindow* theView );

private:
  int               myId;
  static  int       _SVTKViewMgr_Id;
};

#endif
