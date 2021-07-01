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

#include <Container_init_python.hxx> // this include must be the first one as it includes Python.h
#include "LightDesktop.h"

#include "utilities.h"


#include <QtxLogoMgr.h>
#include <QtxActionMenuMgr.h>
#include <QtxActionToolMgr.h>


#include <QFileDialog>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QKeySequence>
#include <QMessageBox>
#include <QPushButton>

#include <stdlib.h>
#include <iostream>
#include <fstream>

/*!\class SUIT_Desktop
 * Provide desktop management:\n
 * \li menu manager
 * \li tool manager
 * \li windows
 */

/*!
  Constructor.
*/
LightDesktop::LightDesktop()
: QtxMainWindow()
{
  myMenuMgr = new QtxActionMenuMgr( this );
  myToolMgr = new QtxActionToolMgr( this );
  myLogoMgr = new QtxLogoMgr( menuBar() );

  createMenus();
}

/*!
  Destructor.
*/
LightDesktop::~LightDesktop()
{

}

void LightDesktop::createMenus()
{
  //QPushButton *mybutton = new QPushButton("Test", this);
  //connect(mybutton, &QPushButton::released, this, &LightDesktop::RunScript);

  //this->setCentralWidget(mybutton);
  QMenu *fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction("Open File", this, &LightDesktop::RunScript, QKeySequence::Open);
  fileMenu->addAction("Dummy", this, SLOT( Dummy()), QKeySequence::New);
}

void LightDesktop::Dummy()
{
  QMessageBox msgBox;
  msgBox.setText("Dummy text");
  msgBox.exec();
}

void LightDesktop::RunScript()
{
  QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Python File"), "/home/B61570/work_in_progress/salome2810", "Python Files (*.py)");

  std::ifstream ifs(fileName.toStdString());
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );


  // Lock Python Global Interperter
  MESSAGE("Starting Python script");
  MESSAGE("**********************");
  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject* myMainMod = PyImport_AddModule("__main__");
  PyObject* myMainDict = PyModule_GetDict(myMainMod);

  PyObject* obj = PyRun_String(content.c_str(), Py_file_input, myMainDict, NULL);

  if(obj == NULL){
    MESSAGE("Script crashed");
    PyErr_Print();
    PyGILState_Release(gstate);
  } else{
    MESSAGE("ALL is well");
    Py_DECREF(obj);
  }
  // Get data from script
  //PyObject* py_i = PyObject_GetAttrString(myMainMod, "i");
  //double i = PyFloat_AsDouble(py_i);
  //Display value of i value
  //std::cerr << "i: " << i << std::endl;
  // Release GIL
  PyGILState_Release(gstate);
}