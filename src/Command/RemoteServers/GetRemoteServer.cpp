#include <Frate/Command/RemoteServers.hpp>  
#include <Frate/Utils/General.hpp>
#include <Frate/Project.hpp>

namespace Frate::Command::RemoteServers {

  bool  get(std::shared_ptr<Interface> inter){
    Utils::TableFormat table;
    table.width = 20;
    table << "Name" << "Address" << "Port" << "Username" << "AuthMethod" << ENDL;
    table << inter->pro->build_server.name << inter->pro->build_server.ip << inter->pro->build_server.port << inter->pro->build_server.username << inter->pro->build_server.authMethod << ENDL;
    return true;
  }

}
