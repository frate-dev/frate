#include "../Command.hpp"


namespace Command::Packages {
  bool checkForOverlappingDependencies(std::vector<Package> deps, std::string &name);

  Package promptSearchResults(std::string &query);

  std::string promptForVersion(Package &chosen_package);

  Package get(std::string query, std::vector<Package> deps);

  bool add(Interface* inter);
}
