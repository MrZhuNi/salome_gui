// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, CSGROUP
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

#ifndef  SALOMEDSIMPL_ADAPT_DEF_HXX
#define  SALOMEDSIMPL_ADAPT_DEF_HXX

#ifdef WIN32
# if defined SALOMEDSIMPL_EXPORTS || defined SalomeDSImpl_EXPORTS
#  define SALOMEDSIMPL_EXPORT __declspec( dllexport )
# else
#  define SALOMEDSIMPL_EXPORT __declspec( dllimport )
# endif
#else
# define SALOMEDSIMPL_EXPORT
#endif

#include <memory>

template<class T> class clt_std_shared_ptr: public std::shared_ptr<T>
{
public:
  clt_std_shared_ptr() {}
  clt_std_shared_ptr(std::nullptr_t p): std::shared_ptr<T>(p){}

  template<class Y>
  explicit clt_std_shared_ptr(Y * p)
  {
    std::shared_ptr<T>::reset(p);
  }

  template<class Y>
  explicit clt_std_shared_ptr(const Y & p)
  {
    std::shared_ptr<T>::reset(p.IsNull()? 0:p.GetPersistentCopy());
  }

  template<class Y>
  clt_std_shared_ptr(clt_std_shared_ptr<Y> const & r):
    std::shared_ptr<T>(std::dynamic_pointer_cast<T>(r))
  {}

  template<class Y>
  clt_std_shared_ptr & operator=(clt_std_shared_ptr<Y> const & r)
  {
    clt_std_shared_ptr<T>(r).swap(*this);
    return *this;
  }

  template<class Y> clt_std_shared_ptr& operator()(Y * p) // Y must be complete
  {
    if(T* pt = dynamic_cast<T*>(p))
      std::shared_ptr<T>::reset(pt);
    else
      throw std::exception(std::bad_cast());
    return *this;
  }

};

#define _CLASS(Class) SALOMEDSImplAdapt_##Class
#define _CAST(Class, ptr_Obj) dynamic_cast<_CLASS(Class)*>(ptr_Obj)
#define _PTR(Class)  clt_std_shared_ptr<_CLASS(Class)>

#endif
