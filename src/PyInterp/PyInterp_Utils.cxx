// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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
//  Author : Adrien BRUNETON
//

#include "PyInterp_Utils.h"
#include <SALOME_Event.h>
#include <Utils_SALOME_Exception.hxx>
#include <Container_init_python.hxx>
#include <utilities.h>
#include <iostream>

/*!
  \class PyLockWrapper
  \brief Python GIL wrapper.
*/

PyObjWrapper PyLockWrapper::_imp_module(NULL);

/*!
  \brief Constructor. Automatically acquires GIL.
*/
PyLockWrapper::PyLockWrapper()
{
  _gil_state = PyGILState_Ensure();
}

bool PyLockWrapper::Initialize()
{
  if (!_imp_module.get())
    {
      PyLockWrapper lock;
      PyObjWrapper m2(PyImport_ImportModule("imp"));
      _imp_module = m2;
      if(!_imp_module)
        {
          PyErr_Print();
          return false;
        }
    }
  return true;
}

bool PyLockWrapper::AcquireImportLock()
{
  if (!_imp_module)
    return false;

  PyLockWrapper lock;
  PyObject_CallMethod(_imp_module, (char *)"acquire_lock", NULL);
  if (PyErr_Occurred())
    {
      PyErr_Print();
      return false;
    }
  return true;
}

bool PyLockWrapper::ReleaseImportLockIfLocked()
{
  if (!_imp_module)
    return false;
  bool ret = false;

  PyLockWrapper lock;
  PyObjWrapper m(PyObject_CallMethod(_imp_module, (char *)"lock_held", NULL));
  if (PyObject_IsTrue(m))
    {
      PyObject_CallMethod(_imp_module, (char *)"release_lock", NULL);
      if (PyErr_Occurred())
        {
          PyErr_Print();
          return false;
        }
      ret = true;
    }

  return ret;
}
/*!
  \brief Destructor. Automatically releases GIL.
*/
PyLockWrapper::~PyLockWrapper()
{
  PyGILState_Release(_gil_state);
}

