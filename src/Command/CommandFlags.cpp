#include "Command.hpp"
#include "toml.hpp"
#include <iostream>
#include <memory>
#include <cxxopts.hpp>
#include <string>

namespace Command {
  bool addFlag(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
      return true;
  }
} 
