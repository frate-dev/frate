#include "Command.hpp"
#include <iostream>
#include <string>
#include <cxxopts.hpp>
#include "../Generators/Generators.hpp"

namespace Command {


  bool add(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args){
    args.count("subcommand");
    if(args.count("subcommand") != 0) {
      std::string subcommand = args["subcommand"].as<std::string>();
      if (subcommand == "dep"){
        addDependency(ctx, args);
      }
      if (subcommand == "flag"){
        addFlag(ctx, args);
      }
    }
    return true;
  }
  bool addFlag(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if(args.count("help")) {
      std::cout << "Usage: addFlag [options] flags" << std::endl;
    }
    if(args.count("subcommand") == 0) {
      for(auto flag: args["subcommand"].as<std::vector<std::string>>() ) {
        ctx->flags.push_back(flag);
      }
    }
    return true;
  }

  bool addDependency(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    std::cout << "addDependency" << std::endl;
    if(args.count("help")) {
      std::cout << "Usage: add dep [options] name url branch" << std::endl;
    }

    //TODO: add dependency target_link to dependency
    if(args.count("subcommand") != 0) {
        struct dependency dependency_new;
        dependency_new.name = args["args"].as<std::vector<std::string>>()[0];
        dependency_new.url = args["args"].as<std::vector<std::string>>()[1];
        dependency_new.version = args["args"].as<std::vector<std::string>>()[2];
        dependency_new.target_link = args["args"].as<std::vector<std::string>>()[3];
        ctx->dependencies.push_back(dependency_new);
    }
    std::cout << ctx->dependencies.size() << std::endl;
    Generators::ConfigToml::writeConfig(ctx);
    Generators::CMakeList::create(ctx);
    return true;
  }
}
