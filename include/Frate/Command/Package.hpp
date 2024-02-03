#pragma once
#include "Frate/Dependency.hpp"
#include "cxxopts.hpp"
#include <Frate/Interface.hpp>
#include <Frate/Package.hpp>

namespace Frate::Command::Packages {

  bool options(std::shared_ptr<Interface> inter);

  /*
   * Add package command adds a package to a project
   * if a package is not exact then it will prompt the user with search results
   * for package if -l is not specified then it will prompt the user for a
   * version if -l is specified then it will install the latest version
   */
  bool add(std::shared_ptr<Interface> inter);

  /*
   * Remove package command, removes a package from the project
   */
  bool remove(std::shared_ptr<Interface> inter);

  // List packages command, lists installed packages
  bool list(std::shared_ptr<Interface> inter);

  /*
   * Get package command, lists information about a pacakge
   */
  bool get(std::shared_ptr<Interface> inter);

  /*
   * Search command
   * Search for a package and return a bool if command run successfully
   */
  bool search(std::shared_ptr<Interface> inter);

  // MOSTLY INTERNAL STUFF
  // Search for a packages and return a vector of results
  std::vector<std::pair<Package, int>> _search(std::string &query);

  // Get the first result from the search
  std::pair<bool, Package> searchGetFirst(std::string &query);

  // Prompt the user to choose from the search results
  std::pair<bool, Dependency> searchWithPrompt(std::string &query, bool latest);

  // Uh this should be pretty self explanatory
  bool dependenciesConflict(std::vector<Dependency> deps, std::string &name);

  // Prompts the user with a list of packages and returns the chosen package
  Package promptSearchResults(std::string &query);

  std::string promptForVersion(Package &chosen_package);

  // Get the exact package from the query
  std::pair<bool, Package> getExact(std::string query);

} // namespace Frate::Command::Packages

// namespace Frate::Command {
//   class AddPackageHandler : public CommandHandler {
//   public:
//     AddPackageHandler(std::shared_ptr<Interface> inter)
//         : CommandHandler(inter) {
//       // clang-format off
//       this->addOptions({
//           {"l,latest", "latest", cxxopts::value<bool>()->default_value("false")},
//           {"n,name", "name", cxxopts::value<std::string>()},
//           });
//       // clang-format on
//     }
// 
//     ~AddPackageHandler() override = default;
// 
//     void run() override;
//     void registerOptions() override;
//     void checkInput() override;
//   };
// 
// } // namespace Frate::Command
