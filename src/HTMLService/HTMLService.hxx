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
#ifndef HTMLSERVICE_H
#define HTMLSERVICE_H

#if defined HTMLSERVICE_EXPORTS
#if defined WIN32
#define HTMLSERVICE_EXPORT		__declspec( dllexport )
#else
#define HTMLSERVICE_EXPORT
#endif
#else
#if defined WIN32
#define HTMLSERVICE_EXPORT		__declspec( dllimport )
#else
#define HTMLSERVICE_EXPORT
#endif
#endif

#if defined SOLARIS
#define bool  int
#define false 0
#define true  1
#endif

#if defined WIN32
#pragma warning ( disable: 4251 )
#endif

#if defined ( _DEBUG ) || defined ( DEBUG )
#include <assert.h>
#define HTMLSERVICE_VERIFY(x)				(assert(x))
#define HTMLSERVICE_ASSERT(x)	            (assert(x))
#else
#define HTMLSERVICE_VERIFY(x)				(x)
#define HTMLSERVICE_ASSERT(x)
#endif


#endif