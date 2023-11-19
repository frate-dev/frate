#include <Frate/Command.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Command/Modes.hpp>
#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Command/Toolchains.hpp>
#include <Frate/Utils/CLI.hpp>
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
  
  std::vector<Handler> Interface::getListHandlers(){
    return {
      Handler{
        .aliases = 
        {"modes","m"},
        .docs = "List modes",
        .callback = [this]() {
          return Modes::list(this);
        }
      },
      Handler{
        .aliases = 
        {"servers","remote-servers"},
        .docs = "List remote servers",
        .callback = [this]() {
          return RemoteServers::list(this);
        }
      },
      Handler{
        .aliases = {"installed-packages","packages","p"},
        .flags = {"-m","--mode"},
        .docs = "List installed packages",
        .callback = [this]() {
          return Packages::list(this);
        }
      },
      Handler{
        .aliases = 
        {"flags","f"},
        .docs = "List flags",
        .callback = [this]() {
          //TODO: List flags
          return true;
        }
      },
      Handler{
        .aliases = 
        {"authors","a"},
        .docs = "List authors",
        .callback = [this]() {
          //TODO: List authors
          return true;
        }
      },
      Handler{
        .aliases = 
        {"available-targets","at"},
        .docs = "List available targets",
        .callback = [this]() {
          Command::Toolchains::list();
          return true;
        }
      },
      Handler{
        .aliases = 
        {"licenses","l"},
        .docs = "List possible licenses",
        .callback = [this]() {
          //TODO: List possible liceses
          return true;
        }
      },
    };
  }


  bool Interface::list(){
    std::vector<Handler> handlers = getListHandlers();
    std::string target;
    if(args->operator[]("subcommand").count() > 0){
      target = args->operator[]("subcommand").as<std::string>();
    }else{
      std::cout << "No target provided" << ENDL;
      getHelpString("list", handlers);
      return false;
    }

    for(auto handler : handlers){
      for(auto alias : handler.aliases){
        if(alias == target){
          return handler.callback();
        }
      }
    }
    std::cout << "Unknown target: " << target << ENDL;
    getHelpString("list", handlers);

    // if(target == "modes" || target == "m"){
    //   return Modes::list(this);
    // }else if(target == "servers" || target == "remote-servers"){
    //   return RemoteServers::list(this);
    // }else if(target == "installed-packages" || target == "packages" || target == "p"){
    //   return Packages::list(this);
    //   //TODO: List installed packages
    // }else if(target == "flags" || target == "f"){
    //   //TODO: List flags
    // }else if(target == "authors" || target == "a"){
    //   //TODO: List authors
    // }else if(target == "licenses" || target == "l"){
    //   //TODO: List possible liceses
    // }else if(target == "modes"){
    //   //TODO: List modes
    // }else if(target == "available-toolchains"){
    //   //TODO: List available toolchains
    // }else if(target == "installed-toolchains"){
    //   //TODO: List installed toolchains
    // }
    return true;

  }
}
