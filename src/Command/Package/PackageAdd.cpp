

#include <CMaker/Command/Package.hpp>
#include <CMaker/Utils/CLI.hpp>
#include <CMaker/Generators.hpp>
#include <CMaker/Utils/General.hpp> 
#include <CMaker/Command/CommandMode.hpp>


namespace Command::Packages {
  using Generators::CMakeList::createCMakeListsExecutable;
  using Generators::ConfigJson::writeConfig;
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
        chosen_package.selected_version = version;
      }else{
        if(chosen_package.versions.size() == 0){
          std::cout << "No versions found" << std::endl;
          return false;
        }
        version = chosen_package.versions[0];
        chosen_package.selected_version = version;
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



}
