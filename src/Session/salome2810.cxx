// Copyright (C) 2021  CEA/DEN, EDF R&D
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

#include <QProcessEnvironment>
#include <QDir>
#include <QProcess>

#include <iostream>

int main(int argc, char *argv[])
{
  const char *MODULES[]={"SHAPERSTUDY","GEOM","SMESH"};
  constexpr char GUI_ROOT_DIR[]="GUI_ROOT_DIR";
  constexpr char GEOM_ROOT_DIR[]="GEOM_ROOT_DIR";
  constexpr char SMESH_ROOT_DIR[]="SMESH_ROOT_DIR";
  constexpr char SHAPER_ROOT_DIR[]="SHAPER_ROOT_DIR";
  constexpr char SHAPERSTUDY_ROOT_DIR[]="SHAPERSTUDY_ROOT_DIR";
  constexpr char APPCONFIG[]="SalomeAppSLConfig";
  QProcessEnvironment pe(QProcessEnvironment::systemEnvironment());
  for(auto elt : {GUI_ROOT_DIR,GEOM_ROOT_DIR,SMESH_ROOT_DIR,SHAPER_ROOT_DIR,SHAPERSTUDY_ROOT_DIR} )
  {
    if( !pe.contains(elt) || pe.value(elt).isEmpty() )
    {
      std::cerr << elt << " is not defined in your environment !" << std::endl;
      return 1;
    }
  }
  // fill LightAppConfig env var
  QString gui_root_dir( QDir::fromNativeSeparators(pe.value(GUI_ROOT_DIR)) );
  QString shaper_root_dir( QDir::fromNativeSeparators(pe.value(SHAPER_ROOT_DIR)) );
  QString shaperstudy_root_dir( QDir::fromNativeSeparators(pe.value(SHAPERSTUDY_ROOT_DIR)) );
  QString geom_root_dir( QDir::fromNativeSeparators(pe.value(GEOM_ROOT_DIR)) );
  QString smesh_root_dir( QDir::fromNativeSeparators(pe.value(SMESH_ROOT_DIR)) );
  QString appconfig_val( QString("%1:%2:%3:%4:%5")
  .arg( QDir::toNativeSeparators( QString("%1/share/salome/resources/gui").arg(gui_root_dir) ) )
  .arg( QDir::toNativeSeparators( QString("%1/share/salome/resources/shaper").arg(shaper_root_dir) ) )
  .arg( QDir::toNativeSeparators( QString("%1/share/salome/resources/shaperstudy").arg(shaperstudy_root_dir) ) )
  .arg( QDir::toNativeSeparators( QString("%1/share/salome/resources/geom").arg(geom_root_dir) ) )
  .arg( QDir::toNativeSeparators( QString("%1/share/salome/resources/smesh").arg(smesh_root_dir) ) ) );
  pe.insert(APPCONFIG,appconfig_val);
  //tells shutup to salome.salome_init invoked at shaper engine ignition
  pe.insert("SALOME_EMB_SERVANT","1");
  //
  QProcess proc;
  proc.setProcessEnvironment(pe);
  proc.setProgram("SALOME_Session_Server");

  QStringList args({"--with","Registry","(","--salome_session","theSession",")","--with","ModuleCatalog","(","-common"});
  QStringList catalogs;
  for(std::size_t im = 0 ; im < sizeof(MODULES)/sizeof(decltype(MODULES[0])) ; ++im )
  {
    QString root_dir = pe.value( QString("%1_ROOT_DIR").arg(MODULES[im]) );
    catalogs << QDir::toNativeSeparators( QString("%1/share/salome/resources/%2/%3Catalog.xml").arg(root_dir).arg(QString(MODULES[im]).toLower()).arg(MODULES[im]) );
  }
  args << catalogs.join("::");
  args << ")";
  args << "--with" << "SALOMEDS" <<  "(" << ")" << "--with" << "Container" << "(" << "FactoryServer" << ")" << "--with" << "SalomeAppEngine" << "(" << ")" << "CPP";
  args << "--resources=/home/H87074/.config/salome/SalomeApprc.9.5.BR" << "--modules" << "(SHAPER:GEOM:SMESH)";
  proc.setArguments(args);
  proc.setProcessChannelMode( QProcess::ForwardedErrorChannel );
  proc.start();
  proc.waitForFinished(-1);
  return proc.exitCode();
}
