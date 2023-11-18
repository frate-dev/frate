#include <CMaker/Command.hpp>
#include <CMaker/Command/Modes.hpp>
#include <CMaker/Command/RemoteServers.hpp>
#include <CMaker/Utils/CLI.hpp>
namespace Command{
  bool OptionsInit::List(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    inter->options->help();
    return inter->parse();
  }
  // bool modesList(Interface* interface){
  //   for(auto mode : interface->pro->modes){
  //     std::cout << mode.name << std::endl;
  //   }
  //   return true;
  // }

  bool Interface::list(){
    std::string target;
    if(args->operator[]("subcommand").count() > 0){
      target = args->operator[]("subcommand").as<std::string>();
    }else{
      std::cout << "No target provided" << ENDL;
      return false;
    }

    if(target == "modes" || target == "m"){
      return Modes::list(this);
    }else if(target == "servers" || target == "remote-servers"){
      return RemoteServers::list(this);
    }else if(target == "installed-packages" || target == "packages" || target == "p"){
      //TODO: List installed packages
    }else if(target == "flags" || target == "f"){
      //TODO: List flags
    }else if(target == "authors" || target == "a"){
      //TODO: List authors
    }else if(target == "licenses" || target == "l"){
      //TODO: List possible liceses
    }else if(target == "modes"){
      //TODO: List modes
    }else if(target == "available-toolchains"){
      //TODO: List available toolchains
    }else if(target == "installed-toolchains"){
      //TODO: List installed toolchains
    }
    return true;

  }
}
