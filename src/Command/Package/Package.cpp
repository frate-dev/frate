#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp> 
#include <Frate/Command/CommandMode.hpp>
#include <Frate/Command.hpp>
#include "Frate/Dependency.hpp"

namespace Frate::Command::Packages {
  using namespace Utils::CLI;
  
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("m,mode", "make changes to compile mode", cxxopts::value<std::string>())
      //TODO: make git version, and stuff like that work to make packages more relaxed
      ("g,git", "git repo to clone", cxxopts::value<std::string>())
      ("package-version", "version for package", cxxopts::value<std::string>())
      ("t,target_link", "how to link to package", cxxopts::value<std::string>())
      ("e,exact", "Exact package", cxxopts::value<bool>()->default_value("false"))
      ("l,latest", "Latest package", cxxopts::value<bool>()->default_value("false"))
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    inter->options->help();
    return inter->parse();
  }
  bool dependenciesConflict(std::vector<Dependency> deps, std::string &name) {
    if (deps.size() == 0) {
      Utils::error << "No dependencies found" << std::endl;
      return false;
    }
    for (Dependency dep : deps) {
      if (dep.name == name) {
        return true;
      }
    }

    return false;
  }

  
  std::pair<bool, Package> getExact(std::string query){
    json index = fetchIndex();
    for(Package pi: index){
      if(pi.name == query){
        return {true, pi};
      }
    }
    return {false, {}};
  }

}

