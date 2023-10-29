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
    }
    return true;
  }
  bool removeDep(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (args.count("help")) {
      std::cout << "Usage: remove dep [options] dep" << std::endl;
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
