#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Project/Mode.hpp>
#include <sys/socket.h>

namespace Frate::Command {}

namespace Frate::Command::Modes {

  bool getModeName(Project::Mode &mode) {
    Utils::CLI::Prompt name("Name: ");
    name.run();
    auto mode_name = name.get<std::string>();
    mode.name = mode_name;
    return true;
  }

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()(
        "command", "Command to run",
        cxxopts::value<std::string>()->default_value("help"))(
        "subcommand", "Subcommand to run", cxxopts::value<std::string>())(
        "args", "Subcommand to run", cxxopts::value<std::string>());
    return inter->parse();
  }

  bool add(std::shared_ptr<Interface> interface) {
    options(interface);
    std::cout << "Adding mode" << std::endl;
    Project::Mode mode;
    mode.name = interface->args->operator[]("args").as<std::string>();

    interface->pro->modes.emplace_back(mode);

    return true;
  }

  bool remove(std::shared_ptr<Interface> inter) {
    options(inter);
    std::string mode_name = inter->args->operator[]("args").as<std::string>();
    std::cout << "Removing mode: " << mode_name << std::endl;
    std::erase_if(inter->pro->modes, [&mode_name](Project::Mode &mode) {
      return mode.name == mode_name;
    });

    return true;
  }

  bool list(std::shared_ptr<Interface> interface) {
    for (auto mode : interface->pro->modes) {
      std::cout << mode.name << std::endl;
    }
    return true;
  }
} // namespace Frate::Command::Modes
