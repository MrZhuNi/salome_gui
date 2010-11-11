#ifndef GRAPHICSVIEW_DEFS_H
#define GRAPHICSVIEW_DEFS_H

#include <QList>

class GraphicsView_Object;
typedef QList<GraphicsView_Object*> GraphicsView_ObjectList;
typedef QListIterator<GraphicsView_Object*> GraphicsView_ObjectListIterator;

enum GV_SelectionChangeStatus
{
  GVSCS_Invalid,
  GVSCS_Local,
  GVSCS_Global
};

enum GV_SelectionStatus
{
  GVSS_Invalid,
  GVSS_LocalChanged,
  GVSS_GlobalChanged,
  GVSS_NoChanged
};

#endif
