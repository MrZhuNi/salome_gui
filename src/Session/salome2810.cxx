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
  constexpr char MAIN_PROGRAM[] = "SALOME_Session_Server_No_Server";
  constexpr char NO_SERVER_ENV_VAR[] = "SALOME_EMB_SERVANT";
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
  pe.insert(NO_SERVER_ENV_VAR,"1");
  //resource file retrieve
  QString resfile;
  {
    QProcess proc;
    proc.setProcessEnvironment(pe);
    proc.setProgram("python3");
    proc.setArguments({"-c","from launchConfigureParser import userFile ; import sys ; sys.stdout.write(userFile(\"SalomeApp\",\"salome\"))"});
    proc.start();
    proc.waitForFinished(-1);
    if(proc.exitStatus() != QProcess::NormalExit)
    {
      std::cerr << "Fail to retrieve resource file from launchConfigureParser python module !" << std::endl;
      return 1;
    }
    QByteArray val(proc.readAllStandardOutput());
    resfile = QString::fromUtf8(val);
  }
  //
  QProcess proc;
  proc.setProcessEnvironment(pe);
  proc.setProgram(MAIN_PROGRAM);

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
  args << QString("--resources=%1").arg(resfile) << "--modules" << "(SHAPER:GEOM:SMESH)";
  if( pe.contains("VERBOSE") )
  {
    std::cout << "Overloaded env var :" << std::endl;
    std::cout << " - " << NO_SERVER_ENV_VAR << std::endl;
    std::cout << " - " << APPCONFIG << " = " << appconfig_val.toStdString() << std::endl;
    std::cout << "Command launched :" << std::endl;
    std::cout << MAIN_PROGRAM << " " << args.join(" ").toStdString() << std::endl;
  }
  proc.setArguments(args);
  proc.setProcessChannelMode( QProcess::ForwardedErrorChannel );
  proc.start();
  proc.waitForFinished(-1);
  return proc.exitCode();
}
