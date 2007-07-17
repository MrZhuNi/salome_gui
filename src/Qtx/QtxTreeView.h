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
// File:      QtxTreeView.h
// Author:    Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//

#ifndef QTXTREEVIEW_H
#define QTXTREEVIEW_H

#include "Qtx.h"

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

#include <QTreeView>

class QTX_EXPORT QtxTreeView : public QTreeView
{
  Q_OBJECT

  class Header;

public:
  QtxTreeView( QWidget* = 0 );
  virtual ~QtxTreeView();
};

#ifdef WIN32
#pragma warning( default:4251 )
#endif

#endif // QTXTREEVIEW_H
