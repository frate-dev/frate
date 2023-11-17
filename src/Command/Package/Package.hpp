#include "../Command.hpp"


namespace Command::Packages {
  bool checkForOverlappingDependencies(std::vector<Package> deps, std::string &name);

  Package promptPackageSearchResults(std::string &query);

  std::string promptForVersion(Package &chosen_package);

  Package getDependency(std::string query, std::vector<Package> deps);

  bool addDependency(Interface* inter);
}
