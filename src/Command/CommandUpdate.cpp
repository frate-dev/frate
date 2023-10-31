#include "Command.hpp"
#include <nlohmann/json.hpp>
#include "../Utils/General.hpp"
#include <iostream>
#include <cstdlib>


namespace Command {
  using nlohmann::json;
  bool updatePackages(){

    json data = Utils::fetchJson("https://github.com/cmaker-dev/index/releases/latest/download/index.json");
    std::string command = "mkdir -p " + static_cast<std::string>(std::getenv("HOME"))  + "/.config/cmaker";
    system(command.c_str());

    std::ofstream file;
    file.open(static_cast<std::string>(std::getenv("HOME")) + "/.config/cmaker/index.json");
    file << data.dump(2);
    file.close();
    return true;
  }

  bool update(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (args["subcommand"].as<std::string>() == "packages") {
      updatePackages();
    }
    return true;
  }
}
