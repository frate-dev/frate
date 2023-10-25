#pragma once
#include <exception>
#include <memory>
#include "toml.hpp"
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cxxopts.hpp>


#define ENDL "\n"

namespace Command {
  [[deprecated("I DON'T KNOW WHAT THIS FUCKING DOES")]]
  bool handleCppProject();
  [[deprecated("I DON'T KNOW WHAT THIS FUCKING DOES")]]
  bool handleCProject();

  /*
   * Checks if the file exists
   * @param name the name of the file
   * @return bool -> file exists
   */
  inline bool file_exists(const std::string& name) {
    std::ifstream file;
    try{
      file.open(name);
    }catch(std::exception e){
      std::cout << "failed to open file: " << name << ENDL;
      exit(0);
    }
    // opening the file
    if (file) {
      std::cout << "file config.toml exists" << std::endl;
      std::cout << "do you want to overwrite it?[y/n]:";
      std::string input;
      std::getline(std::cin, input);
      if (input != "y"){
        exit(1);
      }
      file.close();
      return true;
    }
    return false;
  }
  /*
   * Initializes the options for the command line parser
   * @param new options object
   */
  void initOptions(cxxopts::Options& options);

  typedef struct dependency {
    std::string name;
    std::string version;
    std::string url;
  } dependency;



  typedef struct Context {
    /*
     * TODO: The name should be initialize to the current directory if not specified
     */
    std::string project_name;
    std::filesystem::path project_path;
    std::string git{"null"};
    std::string lang{"cpp"};
    std::string cmake_version{"3.10"};
    std::string lang_version{"20"};
    std::string compiler{"g++"};
    std::vector<std::string> authors;
    std::string src_dir{"src"};
    std::string include_dir{"include"};
    std::vector<dependency> dependencies;
    std::string build_dir{"build"};
    dependency testing_lib;
    std::string project_version{"0.0.1"};
    std::vector<std::string> flags; 
  } Context;
  /*
   * Loads the config.toml file into the project context
   * @param a blank project context
   * @return bool -> finished successfully
   */
  bool loadPackageToml(std::shared_ptr<Context> ctx);
  /*
   * Initializes the project
   * Prompts the user for information about the project
   * @param ctx the current project context
   * @return bool -> finished successfully
   */
  bool init(std::shared_ptr<Context>, cxxopts::ParseResult &args);
  /*
   * Builds and runs the project
   * @param ctx the current project context
   * @return bool -> finished successfully
   */
  bool run(std::shared_ptr<Context>);
  /*
   * Fuck this project function
   * Deletes the project entirely
   * @param ctx the current project context
   * @return bool -> finished successfully
   */
  bool ftp(std::shared_ptr<Context>);
  /*
   * Adds flags to build script
   * @param ctx project context
   * @return bool -> finished successfully
   */
  bool addFlag(std::shared_ptr<Context>&ctx, cxxopts::ParseResult&args);
  /*
   * prints all commands and their function and parameters
   * @return bool -> finished successfully
   */
  bool help();
  /*
   * adds a dependency to the config.toml
   * @returns bool -> finished successfully
   */
  bool addDependency(std::shared_ptr<Context> &ctx, cxxopts::ParseResult &args);
  /*
   * Generates cmake file for project based on the current project context
   * @returns bool -> finished successfully
   */
  bool createCMakelists(std::shared_ptr<Context> ctx);
  /*
   * What the fuck lucas
   */
  /*
   *  adds  dependency to toml and regenerates the CMakeLists.txt
   *  @returns bool -> finished successfully
   */
  bool add(std::shared_ptr<Context>&ctx, cxxopts::ParseResult &args);
  [[deprecated("use addDependency instead")]]
  bool addLib(std::shared_ptr<Context>, cxxopts::ParseResult &args);
}
