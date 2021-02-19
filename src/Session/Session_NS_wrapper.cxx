// Copyright (C) 2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "Session_NS_wrapper.hxx"

#include "SALOME_Container_i.hxx"
#include "SALOME_Launcher.hxx"
#include "utilities.h"

Engines_Container_i *OldStyleNS::activateContainer(CORBA::ORB_var orb, PortableServer::POA_var poa, int argc, char **argv)
{
  Engines_Container_i *_container = nullptr;
  try
  {
    MESSAGE("Container thread started");

    // get or create the child POA

    PortableServer::POA_var factory_poa;
    try
    {
      factory_poa = poa->find_POA("factory_poa", 0);
      // 0 = no activation (already done if exists)
    }
    catch (PortableServer::POA::AdapterNonExistent &)
    {
      MESSAGE("factory_poa does not exists, create...");
      // define policy objects
      PortableServer::ImplicitActivationPolicy_var implicitActivation =
          poa->create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
      // default = NO_IMPLICIT_ACTIVATION
      PortableServer::ThreadPolicy_var threadPolicy =
          poa->create_thread_policy(PortableServer::ORB_CTRL_MODEL);
      // default = ORB_CTRL_MODEL, other choice SINGLE_THREAD_MODEL

      // create policy list
      CORBA::PolicyList policyList;
      policyList.length(2);
      policyList[0] = PortableServer::ImplicitActivationPolicy::
          _duplicate(implicitActivation);
      policyList[1] = PortableServer::ThreadPolicy::
          _duplicate(threadPolicy);

      PortableServer::POAManager_var nil_mgr = PortableServer::POAManager::_nil();
      factory_poa = poa->create_POA("factory_poa",
                                    nil_mgr,
                                    policyList);
      //with nil_mgr instead of pman,
      //a new POA manager is created with the new POA

      // destroy policy objects
      implicitActivation->destroy();
      threadPolicy->destroy();

      // obtain the factory poa manager
      PortableServer::POAManager_var pmanfac = factory_poa->the_POAManager();
      pmanfac->activate();
      MESSAGE("pmanfac->activate()");
    }

    char *containerName = (char *)"";
    if (argc > 1)
    {
      containerName = argv[1];
    }
    _container = new Engines_Container_i(orb, poa, containerName, argc, argv, true, false);
  }
  catch (CORBA::SystemException &)
  {
    INFOS("Caught CORBA::SystemException.");
  }
  catch (PortableServer::POA::WrongPolicy &)
  {
    INFOS("Caught CORBA::WrongPolicyException.");
  }
  catch (PortableServer::POA::ServantAlreadyActive &)
  {
    INFOS("Caught CORBA::ServantAlreadyActiveException");
  }
  catch (CORBA::Exception &)
  {
    INFOS("Caught CORBA::Exception.");
  }
  catch (...)
  {
    INFOS("Caught unknown exception.");
  }
  return _container;
}

void OldStyleNS::activateContainerManager(CORBA::ORB_var orb)
{
  try {
    PortableServer::POA_var root_poa=PortableServer::POA::_the_root_poa();
    std::cout << "Activate SalomeLauncher ......!!!! " << std::endl;
    new SALOME_Launcher(orb,root_poa);
  }
  catch(CORBA::SystemException&) {
    INFOS("Caught CORBA::SystemException.");
  }
  catch(PortableServer::POA::WrongPolicy&) {
    INFOS("Caught CORBA::WrongPolicyException.");
  }
  catch(PortableServer::POA::ServantAlreadyActive&) {
    INFOS("Caught CORBA::ServantAlreadyActiveException");
  }
  catch(CORBA::Exception&) {
    INFOS("Caught CORBA::Exception.");
  }
  catch(...) {
    INFOS("Caught unknown exception.");
  }
}

Engines_Container_i *NewStyleNS::activateContainer(CORBA::ORB_var orb, PortableServer::POA_var poa, int argc, char **argv)
{
  return KERNEL::getContainerSA();
}

void NewStyleNS::activateContainerManager(CORBA::ORB_var orb)
{
  KERNEL::getLauncherSA();
}
