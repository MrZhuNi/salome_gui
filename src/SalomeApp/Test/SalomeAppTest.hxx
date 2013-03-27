// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

#ifndef _SalomeAppTEST_HXX_
#define _SalomeAppTEST_HXX_

#include <cppunit/extensions/HelperMacros.h>

#ifdef WIN32
# if defined SALOMEAPPTEST_EXPORTS || defined SalomeAppTest_EXPORTS
#  define SALOMEAPPTEST_EXPORT __declspec( dllexport )
# else
#  define SALOMEAPPTEST_EXPORT __declspec( dllimport )
# endif
#else
# define SALOMEAPPTEST_EXPORT
#endif

class SALOMEAPPTEST_EXPORT SalomeAppTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( SalomeAppTest );
  CPPUNIT_TEST( testSalomeApp );
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();
  void testSalomeApp();
};

#endif
