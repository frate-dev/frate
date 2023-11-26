#include <Frate/Command.hpp>
#include <nlohmann/json.hpp>
#include <Frate/Utils/General.hpp>
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
  std::vector<Handler> Interface::getUpdateHandlers(){
    return {
      Handler{
        .aliases = 
        {"index"},
          .docs = "Update package index",
          .callback = [this]() {
            return updateIndex();
          }
      }
    };
  }
  bool Interface::update() {
    std::vector<Handler> handlers = getUpdateHandlers();
    std::string target = "";
    if(this->args->count("subcommand") == 0){
      getHelpString("update",handlers);
      return false;
    }
    target = this->args->operator[]("subcommand").as<std::string>();

    return runCommand(target, handlers);

    return true;
  }
}
