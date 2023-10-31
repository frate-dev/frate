#include "Command.hpp"
#include <nlohmann/json.hpp>
#include "../Utils/General.hpp"
#include <iostream>
#include <cstdlib>


namespace Command {
  using nlohmann::json;

  bool update(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (args["subcommand"].as<std::string>() == "packages") {
      std::cout << "Updating packages" << ENDL;
      updateIndex();
    }
    return true;
  }
}
