#pragma once
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

  bool handleCppProject();

  bool handleCProject();


  inline bool file_exists(const std::string& name) {
    std::ifstream file;
    file.open(name);
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

  void initOptions(cxxopts::Options& options);

  typedef struct dependency {
    std::string name;
    std::string version;
    std::string url;
  } dependency;



  typedef struct Context {
    /*
     * TODO: This should be initialize to the current directory if not specified
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

  bool loadPackageToml(std::shared_ptr<Context> ctx);
  bool init(std::shared_ptr<Context>, cxxopts::ParseResult &args);
  bool run(std::shared_ptr<Context>);
  bool ftp(std::shared_ptr<Context>);
  bool addFlag(std::shared_ptr<Context>, cxxopts::ParseResult &args);
  bool help();
  bool addDependency(std::shared_ptr<Context>, cxxopts::ParseResult &args);

  bool writePackageToml(std::shared_ptr<Context> ctx);
  bool createCMakelists(std::shared_ptr<Context> ctx);
  bool addLib(std::shared_ptr<Context>, cxxopts::ParseResult &args);
}