#include "Command.h"
#include "toml.hpp"
#include <iostream>
#include <memory>
#include <string>

namespace Command {
  bool addFlag(std::shared_ptr<Context> ctx, std::string flag) {
    loadPackageToml(ctx);
    for (auto &f : ctx->flags) {
      if (f == flag) {
        std::cout << "Flag already exists" << std::endl;
        return 1;
      }
    }
    ctx->flags.push_back(flag);
    writePackageToml(ctx);
    return true;
  }
} 
