// Copyright (C) 2007-2021  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

#ifndef MINIVIEWER_H
#define MINIVIEWER_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkDataSet.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

class MiniViewer : public QVTKOpenGLNativeWidget {
    Q_OBJECT
public:
    explicit MiniViewer(QWidget* parent = nullptr);

    //! Add a data set to the scene
    /*!
    \param[in] dataSet The data set to add
  */
    void addDataSet(vtkSmartPointer<vtkDataSet> dataSet);

    //! Remove the data set from the scene
    void removeDataSet();

public slots:
    //! Zoom to the extent of the data set in the scene
    void zoomToExtent();

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
};

#endif // MINIVIEWER_H