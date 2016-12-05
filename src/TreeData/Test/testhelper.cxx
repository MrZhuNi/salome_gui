// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "testhelper.hxx"

#include <QFile>
#include <QString>

#include "QtHelper.hxx"
#include "MyDataModel.hxx"

// Standard C include (for getenv)
#include <stdlib.h>

QString TESTHELPER_testfilename(const char * basefilename) {
  QString aFile;
  char * GUI_ROOT_DIR = getenv("GUI_ROOT_DIR");
  QString * root;
  if ( GUI_ROOT_DIR != NULL ) {
    root = new QString(GUI_ROOT_DIR);
  }
  else {
    root = new QString("/home/gboulant/development/projets/salome/devel/XSALOME/install");
  }
  QString relativePathName = "/share/salome/resources/gui/testdata/";
  aFile.append(*root + relativePathName + basefilename);

  QLOG("The test file is : "<<aFile);
  return aFile;
}

/*!
 * This creates a dummy data object for the needs of the test
 * functions. The label is the basename of the spécified pathname.
 */
DataObject * TESTHELPER_dummyObject(QString label) {
  MyDataObject * dataObject = new MyDataObject();
  dataObject->setLabel(QCHARSTAR(label));
  return dataObject;
}


#define SEP ";"
/*!
 * This test function shows how it's possible to load data from a file
 * to populate the tree model.
 */
void TESTHELPER_loadDataFromFile(TreeModel * dataTreeModel, const QString &filename) {
  QFile file ( filename );
  file.open ( QIODevice::ReadOnly );
  
  MyDataObject * dataObject;
  while ( 1 ) {
    QByteArray byteArray = file.readLine();

    if ( byteArray.isEmpty() )
      break;
    
    QString data = (QString ( byteArray.mid(0, byteArray.size()-1))).trimmed();
    QStringList dataList = data.split ( SEP );
    // The data list is used here to set properties (and then the path
    // of location in the tree model).

    dataObject = new MyDataObject();
    // The label is autogenerated, but we may specify here a custom
    // one. We just fill the properties with data values read in the
    // file.
    dataObject->setProperty(MyDataObject::PROPERTY_KEY_TYPE,    QCHARSTAR(dataList[0]));
    dataObject->setProperty(MyDataObject::PROPERTY_KEY_REPFONC, QCHARSTAR(dataList[1]));
    dataObject->setProperty(MyDataObject::PROPERTY_KEY_CIRCUIT, QCHARSTAR(dataList[2]));
    if ( ! dataTreeModel->addData(dataObject) ) {
      QLOG("ERR: data not added");
    }
  }

  file.close();
}
