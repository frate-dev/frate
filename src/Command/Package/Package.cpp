#include <CMaker/Command/Package.hpp>
#include <CMaker/Utils/CLI.hpp>
#include <CMaker/Generators.hpp>
#include <CMaker/Utils/General.hpp> 
#include <CMaker/Command/CommandMode.hpp>

namespace Command::Packages {
  using Generators::CMakeList::createCMakeListsExecutable;
  using Generators::ConfigJson::writeConfig;
  using namespace Utils::CLI;
  std::vector<Package> calculatePackageScores(std::string &query){
    std::vector<Package> results;
    json rawIndex = fetchIndex();
    Utils::toLower(query);


    for(json json: rawIndex){
      Package package;
      package.fromJson(json);

      int score = 0;
      score += Utils::getStringScore(package.name, query) * 10;
      score += package.stars / 100;
      score += Utils::getStringScore(package.description, query);
      score += Utils::getStringScore(package.target_link, query);
      package.score = score;

      results.push_back(package);
    }

    std::sort(results.begin(), results.end(), [](Package a, Package b){
        return a.score > b.score;
        });
    return results;
  }

  std::vector<Package> filterOutBest(std::vector<Package> &packages){

    int totalScore = std::accumulate(
        packages.begin(), packages.end(), 0, [](int a, Package b){
        return a + b.score;
        });

    int averageScore = totalScore / packages.size();

    auto filterResults = packages | std::views::filter(
        [&averageScore](Package package){
        return package.score > 2 * averageScore;
        });

    std::vector<Package> filterResultsVec(filterResults.begin(), filterResults.end());

    return filterResultsVec;
  }

  std::pair<bool, Package> getExactMatch(std::string &query){
    json rawIndex = fetchIndex();
    Utils::toLower(query);
    for(json json: rawIndex){
      Package package;
      package.fromJson(json);
      if(package.name == query){
        return {true, package};
      }
    }
    return {false, Package()};
  }

  bool search(Interface* inter){
    std::string query;
    if(inter->args->operator[]("query").count() == 0){
      std::cout << "No query provided" << ENDL;
      std::cout << "Usage: cmaker search p <query>" << ENDL;
      return false;
    }else{
      query = inter->args->operator[]("query").as<std::string>();
      std::vector<Package> results = calculatePackageScores(query);
      List *packageList = (new Utils::CLI::List())->
        ReverseIndexed();
      for(Package result: results){
        packageList->pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
      }

      std::cout << packageList->Build() << ENDL;

      return true;
    }
  }

  std::vector<Package> search(std::string &query){
    std::vector<Package> results = calculatePackageScores(query);
    return results;
  }

  std::pair<bool, Package> searchGetFirst(std::string& query){
    std::vector<Package> results = calculatePackageScores(query);
    if(results.size() == 0){
      return {false, Package()};
    }
    return {true, results[0]};
  }

  std::pair<bool, Package> searchWithPrompt(std::string& query, bool latest){
    std::vector<Package> results = calculatePackageScores(query);
    if(results.size() == 0){
      return {false, Package()};
    }
    List *packageList = (new Utils::CLI::List())->
      Numbered()->
      ReverseIndexed();
    for(Package result: results){
      packageList->pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
    }


    std::cout << packageList->Build() << std::endl;

    std::vector<Package> filterResultsVec = filterOutBest(results);

    Prompt<int> *prompt = new Prompt<int>("Select a package to install: ");
    for(size_t i = 0; i < filterResultsVec.size(); i++){
      prompt->AddOption(i);
    }
    prompt->Run();
    Package chosen_package = results[prompt->Get()];
    
    if(latest){
      chosen_package.selected_version = chosen_package.versions[0];
      return {true, chosen_package};
    }

    std::string version = promptForVersion(chosen_package);

    chosen_package.selected_version = version;

    return {true, chosen_package};
  }
  
  bool dependenciesConflict(std::vector<Package> deps, std::string &name) {
    if (deps.size() == 0) {
      return false;
    }
    for (Package dep : deps) {
      if (dep.name == name) {
        return true;
      }
    }

    return false;
  }
 Package promptSearchResults(std::string &query){

   std::vector<Package> searchResults = search(query);

   if(searchResults.size() == 1){
     std::cout << "Installing " << searchResults[0].name << std::endl;
     return searchResults[0];
   }

   if(searchResults.size() == 0){
     std::cout << "No results found" << std::endl;
     exit(0);
   }

   List *packageList = (new Utils::CLI::List())->
     Numbered()->
     ReverseIndexed();
   for(Package result: searchResults){
     packageList->pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
   }
   std::cout << packageList->Build() << std::endl;
   Prompt<int> *prompt = new Prompt<int>("Select a package to install: ");
   for(size_t i = 0; i < searchResults.size(); i++){
     prompt->AddOption(i);
   }
   prompt->Run();
   int index = prompt->Get();

   return searchResults[index];
 }

  std::string promptForVersion(Package &chosen_package){

    List* list = (new List())->Numbered()->ReverseIndexed();
    for(size_t i = 0; i < chosen_package.versions.size(); i++){
      list->pushBack(ListItem(chosen_package.versions[i]));
    }

    std::cout << list->Build() << std::endl;

    Prompt<int> *prompt = new Prompt<int>("Select a version to install: ");

    for(size_t i = 0; i < chosen_package.versions.size(); i++){
      prompt->AddOption(i);
    }

    prompt->ExitOnFailure()->Run();


    return chosen_package.versions[prompt->Get()];

  }

  
  std::pair<bool, Package> get(std::string query){
    json rawIndex = fetchIndex();
    Package package;
    for(auto& [name, pi]: rawIndex.items()){
      if(name == query){
        package.fromJson(pi);
        return {true, package};
      }
    }
    return {false, package};
  }
  bool remove(Interface *inter) {

    if (inter->args->count("args") == 0) {
      std::cout << R"EOF(
Usage remove dep:
   [args]: the dependencies to remove
   cmake remove dep [args] 

        )EOF" << std::endl;
      return false;
    }
    if (inter->args->count("mode") != 0){
      return ModeCommands::removePackages(inter, inter->args->operator[]("mode").as<std::string>());
    }

    std::vector<std::string> name_to_remove = inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string name : name_to_remove) {
      std::cout << "name:" << name << std::endl;
    }
    std::cout << "removing dependencies" << std::endl;
    std::erase_if(inter->pro->dependencies, [&name_to_remove](auto &dep) {
        for (std::string name : name_to_remove) {
          if (dep.name == name) {
            return true;
          }
        }
        return false;
    });

    Generators::ConfigJson::writeConfig(inter->pro);
    Generators::CMakeList::createCMakeListsExecutable(inter->pro);
    return true;
  }


  bool add(Interface* inter) {
    bool latest = false;
    //TODO: Add support for multiple dependencies
    if (inter->args->count("args") == 0) {
      std::cout << 
        "Usage add dep:" << ENDL
        "\t[inter->args]: the dependencies to project" << ENDL
        "\tcmake add dep [inter->args] " << std::endl;
      return false;
    }
    if (inter->args->count("mode") != 0){
      return ModeCommands::addPackages(inter, inter->args->operator[]("mode").as<std::string>());
    }
    if(inter->args->operator[]("latest").as<bool>()){
      latest = true;
    }

    std::vector<std::string> package_names = inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string package_name : package_names) { 

      Package chosen_package = promptSearchResults(package_name);

      std::cout << "Installing " << chosen_package.name << std::endl;


      std::string version = ""; 
      std::reverse(chosen_package.versions.begin(), chosen_package.versions.end());
      std::vector<std::string> versions = chosen_package.versions;

      if(!latest){
        version = promptForVersion(chosen_package);
      }else{
        if(chosen_package.versions.size() == 0){
          std::cout << "No versions found" << std::endl;
          return false;
        }
        version = chosen_package.versions[0];
      }
      if(dependenciesConflict(inter->pro->dependencies, chosen_package.name)){
        std::cout << "Package already installed" << std::endl;
        return false;
      }


      std::cout << "Adding dependency to config.json" << std::endl;
      //Reflecing the package to dependency
      inter->pro->dependencies.push_back(chosen_package);


      std::cout << "Writing config.json" << std::endl;
      if (!writeConfig(inter->pro)) {
        std::cout << "Failed to write config.json" << std::endl;
      }

      if (!createCMakeListsExecutable(inter->pro)) {
        std::cout << "Failed to write CMakeLists.txt" << std::endl;
      }
    }

    return true;
  }
} // namespace Command::Packages
