#include "Command.hpp"
#include <filesystem>


namespace Command{
  bool cleanCache(std::shared_ptr<Project> pro){
    const std::vector<std::filesystem::path> files_to_delete = {
      pro->project_path / "CMakeCache.txt",
      pro->project_path / "install_manifest.txt",
      pro->project_path / "Makefile"
    };
    const std::vector<std::filesystem::path> dirs_to_delete = {
      pro->project_path / "CMakeFiles",
      pro->project_path / "_deps"
    };

    for(auto& file: files_to_delete){
      if(std::filesystem::exists(file)){
        std::cout << "Deleting: " << file << std::endl;
        try{
          std::filesystem::remove(file);
        } catch(std::exception& e){
          std::cout << e.what() << std::endl;
          return false;
        }
      }
    }
    for(auto& dir: dirs_to_delete){
      if(std::filesystem::exists(dir)){
        std::cout << "Deleting: " << dir << std::endl;
        try{
          std::filesystem::remove_all(dir);
        } catch(std::exception& e){
          std::cout << e.what() << std::endl;
          return false;
        }
      }
    }


    return true;
  }


  bool Interface::clean(){
    bool clean_cache = false;
    if(args->operator[]("cache").as<bool>()){
      clean_cache = true;
    }
  
    if(clean_cache){
      if(!cleanCache(pro)){
        //WE HAD A FUCKING PROBLEM BRUH
        return false;
      }
    }


    //TODO: delete cache files and deps
    std::cout << "Cleaning: " << std::endl;
    std::cout << pro->project_path / "build/*" << std::endl;
    for(auto& p: std::filesystem::directory_iterator(pro->project_path / "build")){
      std::cout << p.path() << std::endl;
      if(p.is_directory()){
        std::filesystem::remove_all(p.path());
      } else {
        std::filesystem::remove(p.path());
      }
    }

    return true;
  }
}
