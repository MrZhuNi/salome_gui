// Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

//  File   : SVTK_FrameBuffer.h
//  Module : SALOME
//
#ifndef SVTK_FRAMEBUFFER_H
#define SVTK_FRAMEBUFFER_H

#include "SVTK.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

class SVTK_EXPORT SVTK_FrameBuffer
{
public:
  SVTK_FrameBuffer();
  ~SVTK_FrameBuffer();

  bool init( const GLsizei&, const GLsizei& );
  void release();

  void bind();
  void unbind();

private:
  GLuint textureId;
  GLuint fboId;
  GLuint rboId;
};

#endif