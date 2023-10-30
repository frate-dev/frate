#include "../Generators/Generators.hpp"
#include "Command.hpp"
namespace Command {
  bool remove(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    args.count("subcommand");
    if (args.count("subcommand") != 0) {
      std::string subcommand = args["subcommand"].as<std::string>();
      if (subcommand == "dep") {
        removeDep(ctx, args);
      }
    }else{
    std::cout <<  R"EOF(
Usage remove:
  dep: removes dependencies
  lib:  removes libraries
  flag: removes  flags
        )EOF" << std::endl;
    }

    return true;
  }
  bool removeDep(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {

    if (args.count("args") == 0) {
      std::cout << R"EOF(
Usage remove dep:
   [args]: the dependencies to remove
   cmake remove dep [args] 

        )EOF" << std::endl;
      return false;
    }
    std::vector<std::string> name_to_remove = args["args"].as<std::vector<std::string>>();
    for (std::string name : name_to_remove) {
      std::cout << "args:" << name << std::endl;
    }
    std::cout << "removing dependencies" << std::endl;
    std::erase_if(ctx->dependencies, [&name_to_remove](auto &dep) {
        for (std::string name : name_to_remove) {
          if (dep.name == name) {
            return true;
          }
        }
        return false;
    });

    Generators::ConfigToml::writeConfig(ctx);
    Generators::CMakeList::create(ctx);
    return true;
  }
} // namespace Command