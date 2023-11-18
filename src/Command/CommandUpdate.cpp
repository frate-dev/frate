#include <CMaker/Command.hpp>
#include <nlohmann/json.hpp>
#include <CMaker/Utils/General.hpp>
#include <iostream>
#include <cstdlib>


namespace Command {
  using nlohmann::json;
  bool OptionsInit::Update(Interface *inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    return inter->parse();
  }

  bool Interface::update() {
    if(this->args->count("subcommand") == 0){
      std::cout << "Usage: update (index)" << ENDL;
      return true;
    }
    if (this->args->operator[]("subcommand").as<std::string>() == "index") {
      std::cout << "Updating packages" << ENDL;
      updateIndex();
    }
    return true;
  }
}
