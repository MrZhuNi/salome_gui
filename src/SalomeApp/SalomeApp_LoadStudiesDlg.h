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
#ifndef STD_LOADSTUDIESDLG_H
#define STD_LOADSTUDIESDLG_H

#include <STD.h>

#include <QDialog>

class QLabel;
class QListWidget;
class QPushButton;

/*!\class SalomeApp_LoadStudiesDlg
 * \brief Describes a dialog box that gives a list of opened studies.
 * 
 */
class STD_EXPORT SalomeApp_LoadStudiesDlg : public QDialog
{ 
   Q_OBJECT

public:
   SalomeApp_LoadStudiesDlg( QWidget* parent = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
   ~SalomeApp_LoadStudiesDlg() {}

  /*!\var TextLabel1
   * \brief stores a dialog text label
   */
  QLabel* TextLabel1;
  
  /*!\var buttonOk
   * \brief stores a dialog button OK
   */
  QPushButton* buttonOk;

  /*!\var buttonCancel
   * \brief stores a dialog button Cancel
   */  
  QPushButton* buttonCancel;

  /*!\var ListComponent
   * \brief stores a dialog list compoent
   */ 
  QListWidget* ListComponent;

};

#endif // STD_LOADSTUDIESDLG_H
