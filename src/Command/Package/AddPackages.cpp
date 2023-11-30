#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp> 
#include <Frate/Command/CommandMode.hpp>


namespace Command::Packages {
  using Generators::CMakeList::createCMakeLists;
  using Generators::ConfigJson::writeConfig;


  bool addPackageToMode(Interface* inter, Package package, std::string selected_mode){
    Utils::Info info;
    info << "Adding package to mode " << selected_mode << std::endl;
    for(Mode &mode : inter->pro->modes){
      if(selected_mode == mode.name){
        mode.dependencies.push_back(package);
        return true;
      }
    }
    Utils::Error error;
    error << "Mode " << selected_mode << " not found";
    return false;
  }
  bool add(Interface* inter) {
    bool latest = false;
    std::string mode = "";
    std::string query = "";
    Utils::Error error;
    Utils::Info info;
    //TODO: Add support for multiple dependencies
    if (inter->args->count("args") == 0) {
      error << "No packages specified" << std::endl;
      return false;
    }
    if (inter->args->count("mode") != 0){
      mode = inter->args->operator[]("mode").as<std::string>();
    }
    if(inter->args->operator[]("latest").as<bool>()){
      latest = true;
    }






    std::vector<std::string> package_names = inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string package_name : package_names) { 
      info <<  "Searching for " << package_name << std::endl;
      auto [exact, exact_package] = getExact(package_name);
  

      Package chosen_package;

      if(!exact){
        error << "No exact match found" << std::endl;
        chosen_package = promptSearchResults(package_name);
      }else{
        info << "Exact match found" << std::endl;
        chosen_package = exact_package;
      }
      info << "Installing " << chosen_package.name << std::endl;


      std::string version = ""; 
      std::reverse(chosen_package.versions.begin(), chosen_package.versions.end());
      std::vector<std::string> versions = chosen_package.versions;

      if(!latest){
        version = promptForVersion(chosen_package);
        chosen_package.selected_version = version;
      }else{
        if(chosen_package.versions.size() == 0){
          error << "No versions found" << std::endl;
          return false;
        }
        version = chosen_package.versions[0];
        chosen_package.selected_version = version;
      }

      if(dependenciesConflict(inter->pro->dependencies, chosen_package.name)){
        error << "Package already installed" << std::endl;
        return false;
      }


      info << "Adding dependency to frate-project.json" << std::endl;
      //Reflecing the package to dependency
      // info << chosen_package.toJson() << ENDL;
      if(mode != ""){
        if(!addPackageToMode(inter, chosen_package, mode)){
          error << "Failed to add package to mode" << std::endl;
          return false;
        }
      }else{
        inter->pro->dependencies.push_back(chosen_package);
      }

      info << "Writing frate-project.json" << std::endl;
      if (!writeConfig(inter->pro)) {
        error << "Failed to write frate-project.json" << std::endl;
      }

      if (!createCMakeLists(inter->pro)) {
        error << "Failed to write CMakeLists.txt" << std::endl;
      }
    }

    return true;
  }



}
