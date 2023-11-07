#include "./Generators.hpp"
#include "../Command/Command.hpp"
#include <string>
#include <iostream>
#include <regex>
#include <termcolor/termcolor.hpp>

namespace Generators::ConfigJson{
  bool readUserInput(std::shared_ptr<Command::Project> pro, std::shared_ptr<Config> config_json) {
    //TODO: Convert this to the new prompt system
    
    while(!validateLang("📚Language->" + pro->lang + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid language - retry" << termcolor::reset << std::endl;
      
    }
    
    while (!validateProjectName("📖Project name->" + pro->project_name + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid project name - retry" << termcolor::reset << std::endl;
      
    }
    
    while (!validateCmakeVersion("🔨Cmake version->" + pro->cmake_version + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid cmake version - retry" << termcolor::reset << std::endl;
      
    }

    while (!validateProjectVersion("🗃️Version->" + pro->project_version + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid project version - retry" << termcolor::reset << std::endl;
      
    }
    
    while (!validateLanguageVersion("📰Language Standard->" + pro->lang_version + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid language version - retry" << termcolor::reset << std::endl;
      
    }
    
    while (!validateCompiler("💽Compiler->", pro, config_json)) {
      std::cout << termcolor::red << "Invalid compiler - retry" << termcolor::reset << std::endl;
      
    }
    
    while (!validateSourceDir("⛲Source Dir->" + pro->src_dir + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid source directory - retry" << termcolor::reset << std::endl;
      
    }
    
    while (!validateBuildDir("🛠️Build Dir->" + pro->build_dir + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid build directory - retry" << termcolor::reset << std::endl;
      
    }
    
    while (!validateIncludeDir("🫃Include Dir->" + pro->include_dir + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid include directory - retry" << termcolor::reset << std::endl;
      
    }
    return true;
  }

  bool writeConfig(std::shared_ptr<Command::Project> &pro) {
    std::ofstream file;
    std::string file_name = "config.json";
    file.open(pro->project_path / file_name);
    file << pro->toJson();
    file << '\n';
    file.close();
    return true;
  }

} // namespace Generators::ConfigJson
