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
// See http://www.salome-platform.org/
//

#ifndef LIGHTAPP_SHOW_HIDE_OPERATION_HEADER
#define LIGHTAPP_SHOW_HIDE_OPERATION_HEADER

#include "LightApp_Operation.h"

class LightApp_Displayer;
class LIGHTAPP_EXPORT LightApp_ShowHideOp : public LightApp_Operation
{
  Q_OBJECT

public:
    typedef enum { DISPLAY, ERASE, DISPLAY_ONLY, ERASE_ALL } ActionType;

public:
  LightApp_ShowHideOp( ActionType );
  ~LightApp_ShowHideOp();
  
protected:
  virtual void startOperation();

private:
  ActionType   myActionType;
};

#endif

