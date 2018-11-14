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
// File   : Plot2d_ViewWindow.h
// Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//

#ifndef PLOT2D_VIEWWINDOW_H
#define PLOT2D_VIEWWINDOW_H

#include "Plot2d.h"
#include <SUIT_ViewWindow.h>
#include <QMap>

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

class SUIT_Desktop;
class Plot2d_Viewer;
class Plot2d_ViewFrame;
class QtxAction;
class QImage;
class QMenu;
class QToolBar;

class PLOT2D_EXPORT Plot2d_ViewWindow : public SUIT_ViewWindow  
{
  Q_OBJECT

public:
  enum { DumpId, PrintId,
	 FitAllId, FitRectId, ZoomId,
	 PanId, GlobalPanId,
	 PModeXLinearId, PModeXLogarithmicId, 
   PModeYLinearId, PModeYLogarithmicId,
	 CurvPointsId, CurvLinesId, CurvSplinesId, 
	 LegendId,
	 CurvSettingsId, CurvesSettingsId, CommentsSettingsId,
	 CloneId };

public:
  Plot2d_ViewWindow( SUIT_Desktop*, Plot2d_Viewer* );
  virtual ~Plot2d_ViewWindow();

  Plot2d_Viewer*    getModel();
  void              putInfo( const QString&);
  Plot2d_ViewFrame* getViewFrame();
  QToolBar*         getToolBar();
  void              contextMenuPopup( QMenu* );

  virtual bool      eventFilter( QObject*, QEvent* );

  void              createActions();
  void              createToolBar();

  virtual QString   getVisualParameters();
  virtual void      setVisualParameters( const QString& );
  
  virtual void      RefreshDumpImage();
  virtual QImage    dumpView();

public slots:
  void              onChangeHorMode();
  void              onChangeVerMode();
  void              onChangeCurveMode();
  void              onChangeLegendMode();
  
  void              onFitAll();
  void              onFitRect();
  void              onZoom();
  void              onPanning();
  void              onGlobalPanning();
  void              onViewHorMode();
  void              onViewVerMode();
  void              onLegend();
  void              onCurves();

  void              onDumpView();
  void              onPrintView();

protected:
  virtual bool      dumpViewToFormat( const QImage&, 
				      const QString&, 
				      const QString& );
  virtual QString   filter() const;

signals:
  void              cloneView();

private:
  Plot2d_Viewer*    myModel;
  Plot2d_ViewFrame* myViewFrame;
  int               myToolBar;
  QImage            myDumpImage;
};

#ifdef WIN32
//#pragma warning( default:4251 )
#endif

#endif // PLOT2D_VIEWWINDOW_H
