#include <CMaker/Generators/Generators.hpp>
#include <format>
#include <CMaker/Utils/CLI.hpp>


namespace Generators::GitIgnore{

bool write_gitignore(std::string gitignore, std::filesystem::path gitignore_path) {
    try{
      std::ofstream file;
      file.open(gitignore_path);
      file << gitignore;
      file.close();
    }catch(std::exception &e){
      std::cout << "Failed to create gitignore" << std::endl;
      return false;
    }
    return true;
}

  using namespace Utils::CLI;
  bool create(std::shared_ptr<Command::Project> pro){
    std::filesystem::path gitignore_path = pro->project_path / ".gitignore";
    std::string gitignore = std::format(R"VOG(
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
{}/*
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
    )VOG", pro->build_dir);

    if(std::filesystem::exists(gitignore_path)){
      std::cout << "Gitignore already exists" << std::endl;
      Prompt<bool> *prompt = new Prompt<bool>("Do you want to overwrite it?");
      prompt->Color(Ansi::RED)->ExitOnFailure()->Run();
      if(prompt->Get()){
      write_gitignore(gitignore, gitignore_path);
        return true;
      }else{
        return false;
      }
    }else{
      write_gitignore(gitignore, gitignore_path);
    }

    return true;
  }  
}
