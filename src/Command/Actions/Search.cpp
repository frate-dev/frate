#include <Frate/Command/Actions/Search.hpp>
#include <Frate/Command/Package.hpp>

namespace Frate::Command::Search {

  using Frate::Command::Interface;

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "query"});
    // clang-format off
    inter->options->add_options()
      ("command","Command to run",cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())
      ("h,help", "Print usage")
      ("query","Arguments to pass to the command",cxxopts::value<std::string>());
    // clang-format on
    return inter->parse();
  }

  std::vector<Handler> handlers(std::shared_ptr<Interface> inter) {
    return {Handler{
        .aliases = {"package", "p"},
        .flags = {},
        .positional_args = {"query"},
        .docs = "Search for packages",
        .callback = &Packages::search,
    }};
  }

  bool run(std::shared_ptr<Interface> inter) {
    options(inter);
    std::string query;
    std::string target;
    std::vector<Handler> searchHandlers = handlers(inter);

    if (inter->args->operator[]("subcommand").count() == 0) {
      std::cout << termcolor::bright_red << "No subcommand specified"
                << std::endl;
      inter->getHelpString("search", searchHandlers);
      return false;
    }

    target = inter->args->operator[]("subcommand").as<std::string>();

    if (inter->args->operator[]("query").count() == 0) {
      std::cout << termcolor::bright_red << "No query specified" << std::endl;
      inter->getHelpString("search", searchHandlers);
      return false;
    }

    query = inter->args->operator[]("query").as<std::string>();

    return runCommand(inter, target, searchHandlers);
  }
} // namespace Frate::Command::Search
