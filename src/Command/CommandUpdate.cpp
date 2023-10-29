#include "Command.hpp"
#include <nlohmann/json.hpp>
#include "../Utils/General.hpp"

namespace Command {
  using nlohmann::json;
  bool updatePackages(){
    json data = Utils::fetchJson("https://github.com/cmaker-dev/index/releases/latest/download/index.json");
    return true;
  }

  bool update(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (args["subcommand"].as<std::string>() == "packages") {
      updatePackages();
    }
    return true;
  }
}
