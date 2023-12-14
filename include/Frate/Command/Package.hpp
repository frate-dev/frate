#include <Frate/Command.hpp>

namespace Frate::Command::Packages {


  bool options(std::shared_ptr<Interface> inter);

  /*
   * Add package command adds a package to a project
   * if a package is not exact then it will prompt the user with search results for package
   * if -l is not specified then it will prompt the user for a version
   * if -l is specified then it will install the latest version
   */
  bool add(std::shared_ptr<Interface> inter);

  /*
   * Remove package command, removes a package from the project
   * TODO: implement local package search
   */
  bool remove(std::shared_ptr<Interface> inter);

  //List packages command, lists installed packages
  bool list(std::shared_ptr<Interface> inter);

  /*
   * Get package command, lists information about a pacakge
   * TODO: implement search if package is not exact
   */
  bool get(std::shared_ptr<Interface> inter);

  /*
   * Search command
   * Search for a package and return a bool if command run successfully
   */
  bool search(std::shared_ptr<Interface> inter);

  //MOSTLY INTERNAL STUFF
  //Search for a packages and return a vector of results
  std::vector<Package> search(std::string& query);

  //Get the first result from the search
  std::pair<bool,Package> searchGetFirst(std::string& query);

  //Prompt the user to choose from the search results
  std::pair<bool, Package> searchWithPrompt(std::string& query, bool latest);

  //Uh this should be pretty self explanatory
  bool dependenciesConflict(std::vector<Package> deps, std::string &name);
  
  // Prompts the user with a list of packages and returns the chosen package
  Package promptSearchResults(std::string &query);

  std::string promptForVersion(Package &chosen_package);

  //Get the exact package from the query
  std::pair<bool,Package> getExact(std::string query);

}

