#include "Frate/Command/Library.hpp"
#include "Frate/Generators.hpp"
#include "Frate/Project.hpp"
#include "Frate/Utils/General.hpp"
#include <Frate/Command/Actions/Remove.hpp>
#include <Frate/Command/Author.hpp>
#include <Frate/Command/Flags.hpp>
#include <Frate/Command/Modes.hpp>
#include <Frate/Command/Package.hpp>

namespace Frate::Command::Remove {

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    // clang-format off
    inter->options->allow_unrecognised_options().add_options()
      ("command","Command to run",cxxopts::value<std::string>()->default_value("help"))
      ("m,mode","make changes to compile mode",cxxopts::value<std::string>())
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())
      ("h,help", "Print usage")
      ("args","Arguments to pass to subcommand",cxxopts::value<std::vector<std::string>>());
    // clang-format on
    return inter->parse();
  }

  bool removeFlag(std::shared_ptr<Interface> inter) {
    if (inter->args->count("args") == 0) {
    }
    return true;
  }

  std::vector<Handler> handlers(std::shared_ptr<Interface> inter) {
    return {{
                .aliases = {"packages", "p", "package"},
                .flags = {"-l,--latest", "-m,--mode", "-t,--target"},
                .positional_args = {"package_name"},
                .docs = "Remove a package from the project",
                .callback = &Packages::remove,
                .unlimited_args = true,
            },
            {
                .aliases = {"flags", "f"},
                .flags = {"-m,--mode"},
                .positional_args = {"flags"},
                .docs = "Remove a flag from the project",
                .callback = &Flags::remove,
            },
            {
                .aliases = {"mode", "m"},
                .positional_args = {"mode"},
                .docs = "Remove a mode from the project",
                .callback = &Modes::remove,
            },
            {
                .aliases = {"libraries", "l", "lib"},
                .positional_args = {"lib"},
                .docs = "Remove a mode from the project",
                .callback = &Library::remove,
            },
            {
                .aliases = {"author", "auth"},
                .positional_args = {"author_name"},
                .docs = "Remove an author from the project",
                .callback = &Author::remove,
                .unlimited_args = true,
            }};
  }

  bool run(std::shared_ptr<Interface> inter) {
    options(inter);
    std::vector<Handler> removeHandlers = handlers(inter);
    std::string subcommand;

    if (inter->args->count("subcommand")) {

      subcommand = inter->args->operator[]("subcommand").as<std::string>();
    }
    else {
      Utils::error << "No subcommand given" << std::endl;

      inter->getHelpString("remove", removeHandlers);

      return false;
    }
    inter->pro->load();
    return runCommand(inter, subcommand, removeHandlers);
  }
} // namespace Frate::Command::Remove
