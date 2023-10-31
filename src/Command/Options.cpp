
#include "Command.hpp"
#include <cxxopts.hpp>
#include <string>

namespace Command {
  using namespace cxxopts;
  /*
   * Initializes the options for options class
   * This allows us to use options in multiple places
   * @param options the options class to initialize
   */
  ParseResult mainOptions(int argc, char **argv) {
    Options options("Cmake-Generator", "Stop writing CMakeLists.txt files! let us suffer for you");
    options.parse_positional({"command"});
    options.allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("h,help", "Print usage");

    try {
      return options.parse(argc, argv);
    } catch (std::exception &e) {
      std::cout << "failed to parse options" << ENDL;
      exit(1);
    }
  }


  ParseResult initOptions(int argc, char **argv) {
    Options options(
        "Cmake-Generator", 
        "Stop writing CMakeLists.txt files! let us suffer for you");
    options.parse_positional({"command", "subcommand"});
    options.add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::vector<std::string>>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      ("name", "Name of the project", cxxopts::value<std::string>()->default_value("false"))
      ("language", "Language of the project", cxxopts::value<std::string>()->default_value("cpp"));
    try {
      return options.parse(argc, argv);
    } catch (std::exception &e) {
      std::cout << "failed to parse options" << ENDL;
      exit(1);
    }
  }
  ParseResult addOptions(int argc, char **argv) {
    Options options("Cmake-Generator",
        "Stop writing CMakeLists.txt files! let us suffer for you");
    options.parse_positional({"command", "subcommand", "args"});
    options.add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    options.help();
    try {
      return  options.parse(argc, argv);
    } catch (std::exception &e) {
      std::cout << "failed to parse options" << ENDL;
      exit(1);
    }
  }
  ParseResult removeOptions(int argc, char **argv) {
    Options options("Cmake-Generator",
        "Stop writing CMakeLists.txt files! let us suffer for you");
    options.parse_positional({"command", "subcommand", "args"});
    options.add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    try {
      return options.parse(argc, argv);
    } catch (std::exception &e) {
      std::cout << "failed to parse options" << ENDL;
      exit(1);
    }
  }
  ParseResult updateOptions(int argc, char **argv) {
    Options options("Cmake-Generator",
        "Stop writing CMakeLists.txt files! let us suffer for you");
    options.parse_positional({"command", "subcommand"});
    options.add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"));
    try {
      return options.parse(argc, argv);
    } catch (std::exception &e) {
      std::cout << "failed to parse options" << ENDL;
      exit(1);
    }
  }
} // namespace Command
