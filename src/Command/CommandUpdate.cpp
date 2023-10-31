#include "Command.hpp"
#include <nlohmann/json.hpp>
#include "../Utils/General.hpp"
#include <iostream>
#include <cstdlib>


namespace Command {
  using nlohmann::json;

  bool Interface::update() {
    if (this->args->operator[]("subcommand").as<std::string>() == "packages") {
      std::cout << "Updating packages" << ENDL;
      updateIndex();
    }
    return true;
  }
}
