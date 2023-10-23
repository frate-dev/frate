#include "Command.hpp"
#include "toml.hpp"
#include <iostream>
#include <memory>
#include <cxxopts.hpp>
#include <string>

namespace Command {
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
} 
