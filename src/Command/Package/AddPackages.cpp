#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp> 
#include <Frate/Command/CommandMode.hpp>


namespace Frate::Command::Packages {
  using Generators::CMakeList::createCMakeLists;
  using Generators::ConfigJson::writeConfig;


  bool addPackageToMode(std::shared_ptr<Interface> inter, Package package, std::string selected_mode){
    Utils::info << "Adding package to mode " << selected_mode << std::endl;
    for(Mode &mode : inter->pro->modes){
      if(selected_mode == mode.name){
        mode.dependencies.push_back(package);
        return true;
      }
    }
    Utils::error << "Mode " << selected_mode << " not found";
    return false;
  }
  bool add(std::shared_ptr<Interface> inter) {
    options(inter);
    bool latest = false;
    std::string mode = "";
    std::string query = "";
    std::string version = ""; 
    std::string git = "";
    std::string target_link = "";


    if (inter->args->count("mode") != 0){
      mode = inter->args->operator[]("mode").as<std::string>();
    }
    if (inter->args->count("git") != 0) {
      git = inter->args->operator[]("git").as<std::string>();
    }
    if (inter->args->count("package-version") != 0) {
      version = inter->args->operator[]("package-version").as<std::string>();
    }
    if (inter->args->count("target_link") != 0) {
      target_link = inter->args->operator[]("target_link").as<std::string>();
    }
    if(inter->args->operator[]("latest").as<bool>()){
      latest = true;
    }
    if (git != "") {
      std::cout << "Adding git package" << std::endl;
      std::cout << "Git: " << git << std::endl;
      std::cout << "Version: " << version << std::endl;
      Utils::info << "Adding git package" << std::endl;
      if (version == "") {
        Utils::error << "No version specified" << std::endl;
        Utils::error << "Please specify a version" << std::endl;
        return false;
      }
      if (target_link == "") {
        Prompt target_link_prompt("Specify target_link: "); 
        target_link_prompt.run();
        auto [valid, target_link] = target_link_prompt.get<std::string>();
        if (!valid) {
          Utils::error << "Failed to get target_link" << std::endl;
          return false;
        }
      }
      Package package;
      package.name = git;
      package.selected_version = version;
      package.target_link = target_link;
      package.git = git;
      package.versions.push_back(version);
      if (mode != "") {
        if (!addPackageToMode(inter, package, mode)) {
          Utils::error << "Failed to add package to mode" << std::endl;
          return false;
        }
        return true;
      }
      std::cout << "Package: " << nlohmann::json(package) << std::endl;
      inter->pro->dependencies.push_back(package);
      if (!inter->pro->save()){
        Utils::error << "Failed to save project" << std::endl;
        return false;
      };
      return true;
    }


    std::vector<std::string> package_names = inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string package_name : package_names) { 
      Utils::info <<  "Searching for " << package_name << std::endl;
      
      auto [exact, exact_package] = getExact(package_name);
      Package chosen_package;

      if(!exact){
        Utils::error << "No exact match found" << std::endl;
        chosen_package = promptSearchResults(package_name);
      }else{
        Utils::info << "Exact match found" << std::endl;
        chosen_package = exact_package;
      }
      Utils::info << "Installing " << chosen_package.name << std::endl;


      std::reverse(chosen_package.versions.begin(), chosen_package.versions.end());
      std::vector<std::string> versions = chosen_package.versions;

      if(!latest){
        version = promptForVersion(chosen_package);
        chosen_package.selected_version = version;
      }
      else{
        if(chosen_package.versions.size() == 0){
          Utils::error << "No versions found" << std::endl;
          return false;
        }
        version = chosen_package.versions[0];
        chosen_package.selected_version = version;
      }

      if(dependenciesConflict(inter->pro->dependencies, chosen_package.name)){
        Utils::error << "Package already installed" << std::endl;
        return false;
      }

      if(mode != ""){
        if(!addPackageToMode(inter, chosen_package, mode)){
          Utils::error << "Failed to add package to mode" << std::endl;
          return false;
        }
      }
      inter->pro->dependencies.push_back(chosen_package);
    }

    return true;
  }
}
