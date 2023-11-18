#include <CMaker/Command.hpp>

namespace Command {
  bool OptionsInit::Build(Interface* inter){
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("m,mode", "Architecture to build", cxxopts::value<std::string>())
      ("t,target", "Target to build", cxxopts::value<std::string>());
    return inter->parse();
  }
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
