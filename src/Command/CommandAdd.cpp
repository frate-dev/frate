#include "../Generators/Generators.hpp"
#include "Command.hpp"
#include <algorithm>
#include <cpr/cpr.h>
#include <cxxopts.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace Command {
  bool add(std::shared_ptr<Context> &ctx, cxxopts::ParseResult &args) {
    args.count("subcommand");
    if (args.count("subcommand") != 0) {
      std::string subcommand = args["subcommand"].as<std::string>();
      if (subcommand == "dep") {
        addDependency(ctx, args);
      }
      if (subcommand == "flag") {
        addFlag(ctx, args);
      }
    }
    return true;
  }
  bool addFlag(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (args.count("help")) {
      std::cout << "Usage: addFlag [options] flags" << std::endl;
    }
    if (args.count("subcommand") == 0) {
      for (auto flag : args["subcommand"].as<std::vector<std::string>>()) {
        ctx->flags.push_back(flag);
      }
    }
    return true;
  }

  nlohmann::json fetch(cpr::Url url) {
    cpr::Response r = cpr::Get(cpr::Url{url});
    // add error handling
    r.status_code == 200
      ? [&url](cpr::Response r) -> void{
        std::cout << r.error.message << std::endl;
        std::cout << "couldn't connect to " << url  << std::endl;
        exit(1);
      }(r)
    : [&url](cpr::Response r) -> void{
      std::cout << r.status_code << std::endl;
      std::cout << "fetching data from:"  << url << std::endl;
    }(r);
    try {
      nlohmann::json json = nlohmann::json::parse(r.text);
      return json;
    } catch (const std::exception &e) {
      std::cout << "Error: " << e.what() << std::endl;
      exit(1);
    }
  }

  bool addDependency(std::shared_ptr<Context> &ctx, cxxopts::ParseResult &args) {
    std::cout << "addDependency" << std::endl;
    if (args.count("help")) {
      std::cout << "Usage: add dep [options] name url branch" << std::endl;
    }
    nlohmann::json json = fetch(
        "https://raw.githubusercontent.com/cmaker-dev/index/main/index.json");
    for (auto package : json) {
      if (package["Name"] == args["args"].as<std::vector<std::string>>()[0]) {
        nlohmann::json package_data =
          fetch("https://raw.githubusercontent.com/cmaker-dev/index/main/" +
              static_cast<std::string>(package["Name"]) + "/info.json");
        dependency dependency_new = {
          .name = static_cast<std::string>(package_data["Name"]),
          .url = static_cast<std::string>(package_data["Homepage"]),
          .version = static_cast<std::string>(package_data["Tag"]),
          .target_link = static_cast<std::string>(package_data["Link"])};

        if (std::any_of(ctx->dependencies.begin(), ctx->dependencies.end(),
              [&dependency_new](dependency &dep) {
              return dep.name == dependency_new.name;
              })) {
          std::cout << "Dependency already exists not adding" << std::endl;
        } else {
          ctx->dependencies.push_back(dependency_new);
        }
      }
    }
    std::cout << ctx->dependencies.size() << std::endl;
    Generators::ConfigToml::writeConfig(ctx);
    Generators::CMakeList::create(ctx);
    return true;
  }
} // namespace Command
