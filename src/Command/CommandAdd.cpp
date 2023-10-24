#include "Command.hpp"
#include <iostream>
#include <string>
#include <cxxopts.hpp>

namespace Command {


  bool  add(){
    return true;
  }


  bool addDependency(std::shared_ptr<Context> ctx, cxxopts::ParseResult& args) {
    // TODO: Add dependency to config.toml
    std::cout << "addDependency" << std::endl;
    if(args.count("help")) {
      std::cout << "Usage: addDependency [options] name url branch" << std::endl;
    }
    if(args.count("subcommand") != 0) {
        struct dependency dependency_new;
        dependency_new.name = args["subcommand"].as<std::vector<std::string>>()[0];
        dependency_new.url = args["subcommand"].as<std::vector<std::string>>()[1];
        dependency_new.version = args["subcommand"].as<std::vector<std::string>>()[2];
        ctx->dependencies.push_back(dependency_new);
    }
    std::cout << ctx->dependencies.size() << std::endl;
    Command::writePackageToml(ctx);
    Command::createCMakelists(ctx);
    return true;
  }
}
