#include <CMaker/Command.hpp>
#include <CMaker/Command/Package.hpp>

namespace Command {
  bool Interface::get(){
    std::string target;
    if (args->count("subcommand") == 0) {
      this->help();
      return false;
    }else{
      target = args->operator[]("subcommand").as<std::string>();
      if(target == "package"){
        return Packages::get(this);
      }
    }
  }
}
