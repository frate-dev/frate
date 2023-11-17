#include <CMaker/Command/Command.hpp>

namespace Command {

  bool Interface::build() {
    std::string target;
    std::string mode;
    if(this->args->count("target")){
      target = this->args->operator[]("target").as<std::string>();
    }
    if (this->args->operator[]("subcommand").as<std::string>() == "target") {
      //TODO: handle build here
    }
    return true;
  }
}
