#include <CMaker/Command/Command.hpp>
#include <nlohmann/json.hpp>
#include <CMaker/Utils/General.hpp>
#include <iostream>
#include <cstdlib>


namespace Command {
  using nlohmann::json;

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
