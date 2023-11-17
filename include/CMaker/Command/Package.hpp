#include <CMaker/Command.hpp>

namespace Command::Packages {

  const Info info = {
    .name = "packages",
    .description = "Manage packages for your project",
    .valid_flags = {
      "--mode",
      "-m",
      "--target",
      "-t",
      "--help"
    }
  };

  bool checkForOverlappingDependencies(std::vector<Package> deps, std::string &name);

  Package promptSearchResults(std::string &query);

  std::string promptForVersion(Package &chosen_package);

  std::pair<bool,Package> get(std::string query, std::vector<Package> deps);

  std::pair<bool,Package> get(std::string query);

  bool add(Interface* inter);
  bool remove(Interface *inter);

  std::vector<Package> search(std::string& query);

  bool search(Interface* inter, std::string& query);

}
