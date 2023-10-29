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
    ctx->dependencies.erase(
        std::remove_if(ctx->dependencies.begin(), ctx->dependencies.end(),
          [&name_to_remove](const dependency &dep) {
          std::cout << dep.name << std::endl;

          for (std::string name : name_to_remove) {
            std::cout << name << std::endl;
            std::cout << (dep.name == name) << ENDL;
          }
          for (std::string name : name_to_remove)
            return dep.name == name;
          return false;
          }));

    Generators::ConfigToml::writeConfig(ctx);
    Generators::CMakeList::create(ctx);
    return true;
  }
} // namespace Command
