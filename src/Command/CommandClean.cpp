#include "Frate/Utils/General.hpp"
#include <Frate/Command.hpp>
#include <filesystem>

namespace Command {
  bool OptionsInit::Clean(Interface *inter){
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>())
      ("c,cache", "Cleans build and cache", cxxopts::value<bool>()->default_value("false"));
    return inter->parse();
  }

using namespace std::filesystem;
bool cleanCache(std::shared_ptr<Project> pro) {
  const std::vector<path> files_to_delete = {
      pro->project_path / "CMakeCache.txt",
      pro->project_path / "install_manifest.txt",
      pro->project_path / "cmake_install.cmake",
      pro->project_path / "Makefile"};
  const std::vector<std::filesystem::path> dirs_to_delete = {
      pro->project_path / "CMakeFiles/",
      pro->project_path / "_deps/"
  };

  for (auto &file : files_to_delete) {
    if (exists(file)) {
      try {
        std::cout << "Deleting: " << file << std::endl;
        std::filesystem::remove(file);
      } catch (std::exception &e) {
        Utils::debug(e.what());
      }
    }
  }
  for (auto &dir : dirs_to_delete) {
    if (exists(dir)) {
      try {
        std::cout << "Deleting: " << dir << std::endl;
        remove_all(dir);
      } catch (std::exception &e) {
        Utils::debug(e.what());
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
    if(exists(pro->project_path / "build")){
      for (directory_entry p : directory_iterator(pro->project_path / "build")) {
        std::cout << "Deleting" << p.path() << std::endl;
        if(p.is_directory()){
          remove_all(p.path());
        } else {
          std::filesystem::remove(p.path());
        }
      }
    }else{
      std::cout << "Could not find ./build" << std::endl;
    }

    return true;
  }
  } // namespace Command
