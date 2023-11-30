#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>
#include <Frate/Command.hpp>
#include <Frate/Utils/General.hpp>
#include <git2.h>
#include <inja.hpp>

namespace Generators::CMakeList {
  using nlohmann::json;
  bool downloadCMakeListsTemplate(std::shared_ptr<Command::Project> _){

    std::cout << "Downloading CMakeLists.txt" << std::endl;
    std::ofstream file;
    json CMakeListsTemplateIndex = Utils::fetchJson("https://github.com/frate-dev/templates/releases/latest/download/index.json");
    try{
      file.open(static_cast<std::string>(std::getenv("HOME")) + "/.config/frate/" + "templates.json");
      file << CMakeListsTemplateIndex.dump(2);
    }catch(...){
      Utils::debug("Error while opening file: templates.json");
      return false;
    }
      
    return true;
  }
  bool createCMakeLists(std::shared_ptr<Command::Project> pro){
  std::cout << "Creating CMakeLists.txt" << std::endl;
  
  #ifdef DEBUG
    std::cout << pro->toJson().dump(2) << std::endl;
  #endif
  std::string CPM = Utils::fetchText("https://raw.githubusercontent.com/cpm-cmake/CPM.cmake/v0.38.6/cmake/CPM.cmake");
  std::ofstream CPMFile;
  try{
    if(!std::filesystem::exists(pro->project_path / "cmake"))
      std::filesystem::create_directories(pro->project_path / "cmake");
    CPMFile.open(pro->project_path / "cmake/CPM.cmake");
  }catch(...){
    Utils::debug("Error while opening file: CPM.cmake");
    return false;
  }
  std::string template_index = static_cast<std::string>(std::getenv("HOME"))+ "/.config/frate/templates.json";

  std::filesystem::create_directories(static_cast<std::string>(std::getenv("HOME")) + "/.config/frate");
  std::ifstream  template_index_file;

  if (!std::filesystem::exists(template_index)){
    downloadCMakeListsTemplate(pro); 
  }
  std::string repo_url;
  std::cout << "file: " << template_index << std::endl;
  json templateIndex = json::parse(std::ifstream(template_index));
  std::cout << templateIndex.dump(2) << std::endl;
  for(json temp: templateIndex){
    if (temp["name"] == pro->template_name){
      repo_url = temp["git"];
    }
  }

  CPMFile << CPM;
  inja::Environment env;
  git_repository *repo = NULL; 
  std::cout << "Cloning " << repo_url << " into " << (pro->project_path / "templates").c_str() << std::endl;
  git_clone(&repo, repo_url.c_str(), (pro->project_path / "templates").c_str(), NULL);
  std::string CMakeListsExecutable =  env.render_file(pro->project_path /"templates" /"CMakeLists.tmpl", pro->toJson());
  std::ofstream file;
  std::string file_name = "CMakeLists.txt";

  try{
    remove((pro->project_path / file_name).c_str());
  }catch(...){
    Utils::debug("Error while removing file: " + file_name);
    return false;
  }

  try{
    file.open(pro->project_path / file_name);
  }catch(...){
    Utils::debug("Error while opening file: " + file_name);
    return false;
  }
  //std::cout << CMakeListsExecutable << std::endl;
  file << CMakeListsExecutable;
  return true;
  }
}




