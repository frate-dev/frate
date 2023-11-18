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

  bool dependenciesConflict(std::vector<Package> deps, std::string &name);

  Package promptSearchResults(std::string &query);

  std::string promptForVersion(Package &chosen_package);

  //Get the exact package from the query
  std::pair<bool,Package> get(std::string query);


  
  //Add package command
  bool add(Interface* inter);
  //Removed packackage command
  bool remove(Interface *inter);

  //Search command
  //Search for a package and return a bool if command run successfully
  bool search(Interface* inter);

  //MOSTLY INTERNAL STUFF
  //Search for a packages and return a vector of results
  std::vector<Package> search(std::string& query);
  //Get the first result from the search
  std::pair<bool,Package> searchGetFirst(std::string& query);
  //Prompt the user to choose from the search results
  std::pair<bool, Package> searchWithPrompt(std::string& query, bool latest);


}
