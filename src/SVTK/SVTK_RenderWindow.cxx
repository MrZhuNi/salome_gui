#include "SVTK_RenderWindow.h"
#include "SVTK_Trihedron.h"
#include "SVTK_CubeAxesActor2D.h"
#include "SALOME_Actor.h"

#include "VTKViewer_Transform.h"
#include "VTKViewer_Utilities.h"

#include "SVTK_Renderer.h"

#include "QtxAction.h"

#include "SUIT_Session.h"
#include "SUIT_ToolButton.h"
#include "SUIT_MessageBox.h"

#include "SUIT_Tools.h"
#include "SUIT_ResourceMgr.h"

#include <vtkRenderWindow.h>

#if QT_VERSION > 300
#include <qcursor.h>
#endif

//----------------------------------------------------------------------------
SVTK_RenderWindow
::SVTK_RenderWindow(QWidget* parent, const char* name) :
  QMainWindow(parent, name, 
	      Qt::WStyle_NoBorder | 
	      Qt::WDestructiveClose | 
	      Qt::WResizeNoErase | 
	      Qt::WRepaintNoErase),
    myRenderWindow(vtkRenderWindow::New())
{
  myRenderWindow->Delete();

#ifndef WNT
  myRenderWindow->SetDisplayId((void*)x11Display());
#endif
  myRenderWindow->SetWindowId((void*)winId());
  myRenderWindow->DoubleBufferOn();
  setMouseTracking(true);

}

SVTK_RenderWindow
::~SVTK_RenderWindow() 
{}


vtkRenderWindow* 
SVTK_RenderWindow
::getRenderWindow()
{ 
  return myRenderWindow.GetPointer(); 
}
