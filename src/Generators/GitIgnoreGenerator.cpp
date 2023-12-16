#include "Frate/Utils/General.hpp"
#include <Frate/Generators.hpp>
#include <fstream>
#include <Frate/Utils/CLI.hpp>
#include <memory>


namespace Frate::Generators::GitIgnore{

bool write_gitignore(std::string gitignore, std::filesystem::path gitignore_path) {
    try{
      std::ofstream file;
      file.open(gitignore_path);
      file << gitignore;
      file.close();
    }catch(std::exception &e){
      Frate::error << "Failed to create gitignore" << std::endl;
      Utils::debug(e.what());
      return false;
    }
    return true;
}

  using namespace Utils::CLI;
  bool create(std::shared_ptr<Command::Interface> inter){
    std::filesystem::path gitignore_path = inter->pro->path / ".gitignore";
    std::string gitignore = R"VOG(
# CMake
CMakeLists.txt.user
CMakeFiles/
CMakeCache.txt
Makefile
_deps/
CMakeCache.txt
cmake_install.cmake
install_manifest.txt
compile_commands.json
# Build dir
{build_dir}/*
# VS Code
.vscode/
# CLion
.idea/
# XCode
*.xcodeproj
*.xcworkspace
# Visual Studio
*.sln
*.vcxproj
*.vcxproj.filters
*.vcxproj.user
*.vcproj
*.vcproj.*
# vim/nvim
*.swp
compile_commands.json
)VOG";

Utils::replaceKey(gitignore, "{build_dir}", inter->pro->build_dir);

    if(std::filesystem::exists(gitignore_path)){
      std::cout << "Gitignore already exists" << std::endl;
      Prompt prompt("Do you want to overwrite it?");
      prompt.Color(Ansi::RED).ExitOnFailure().Run();
      prompt.IsBool();
      auto [valid, value] = prompt.Get<bool>();
      if(!valid || !value){
        return false;
      }
      write_gitignore(gitignore, gitignore_path);
    }else{
      write_gitignore(gitignore, gitignore_path);
    }

    return true;
  }  
}
