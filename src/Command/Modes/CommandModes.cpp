#include <Frate/Command.hpp>
#include <Frate/Generators.hpp>
#include <sys/socket.h>

namespace Frate::Command{
}


namespace Frate:: Command::Modes{
  bool getModeName(Mode &mode){
    Prompt *name = new Prompt("Name: ");
    name->Run();
    auto [valid,mode_name] = name->Get<std::string>();
    if(!valid){
      Frate::error << "Failed to get mode name" << std::endl;
      return false;
    }
    mode.name = mode_name;
    return true;
  }
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())
      ("args", "Subcommand to run", cxxopts::value<std::string>());
    return inter->parse();
  }

  bool add(std::shared_ptr<Interface> interface){
    options(interface);
    std::cout << "Adding mode" << std::endl; 
    Mode mode;
    mode.name = interface->args->operator[]("args").as<std::string>();

    interface->pro->modes.push_back(mode);

    std::cout << "Writing frate-project.json" << std::endl;

    if(!interface->pro->save()) return false;

    return true;
  }
  bool remove(std::shared_ptr<Interface> inter){
    options(inter);
    std::string mode_name = inter->args->operator[]("args").as<std::string>(); 
    std::cout << "Removing mode: " << mode_name << std::endl;
    std::erase_if(inter->pro->modes, [&mode_name](Mode &mode){
        return mode.name == mode_name;
        });
    if(!inter->pro->save()) return false;

    return true;
  }
  bool list(std::shared_ptr<Interface> interface){
    for(auto mode : interface->pro->modes){
      std::cout << mode.name << std::endl;
    }
    return true;
  }
}
