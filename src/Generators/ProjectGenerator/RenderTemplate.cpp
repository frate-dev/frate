#include "Frate/Utils/General.hpp"
#include <Frate/Generators.hpp>
#include <Frate/Project/Config.hpp>
#include <inja.hpp>

namespace Frate::Generators::Project {
  using inja::Environment;

  bool renderTemplate(Environment &env, std::shared_ptr<Project::Config> pro) {

    std::string CPM;

    CPM = Utils::fetchText("https://raw.githubusercontent.com/cpm-cmake/"
                           "CPM.cmake/v0.38.6/cmake/CPM.cmake");

    std::ofstream CPMFile;
    try {
      if (!std::filesystem::exists(pro->path / "cmake"))
        std::filesystem::create_directories(pro->path / "cmake");
      CPMFile.open(pro->path / "cmake/CPM.cmake");
    } catch (...) {
      Utils::debug("Error while opening file: CPM.cmake");
      return false;
    }

    CPMFile << CPM;

    // Array to store all the paths to remove at the end of the function
    std::vector<path> paths_to_remove = {
        pro->path / "__init__", pro->path / "__post__",
        pro->path / "frate-callbacks", pro->path / "LICENSE",
        pro->path / "cmake_includes"};

    // Array of all the file extensions that we're going to ignore when copying
    // files
    std::vector<std::string> source_file_extensions_to_remove;

    // Map of all the source file extensions that are related to the project
    // language
    std::unordered_map<std::string, std::vector<std::string>>
        related_source_exts = {};

    related_source_exts["cpp"] = {".cpp", ".cxx", ".cc",  ".C",
                                  ".c++", ".hpp", ".hxx", ".hh"};
    related_source_exts["c"] = {".c", ".h"};
    // Removes all source files that are not related to the project language
    for (auto [key, source_files] : related_source_exts) {
      if (pro->lang != key) {
        for (std::string source_file : source_files) {
          source_file_extensions_to_remove.emplace_back(source_file);
        }
      }
    }

    for (const path &current_p :
         std::filesystem::recursive_directory_iterator(pro->path)) {
      if (current_p.string().find("override/") != std::string::npos) {
        continue;
      }
      if (current_p.extension() == ".inja") {
        std::string rendered_file = env.render_file(current_p, *pro);
        std::string new_file = current_p.string();
        // Removes the .inja extension from the file
        new_file = new_file.replace(new_file.find(".inja"), 5, "");
        std::ofstream file;
        try {
          file.open(new_file);
        } catch (...) {
          Utils::error << "Error while opening file: " << new_file << std::endl;
          return false;
        }
        file << rendered_file;
        paths_to_remove.emplace_back(current_p);
      }
      // Removes the scripts from the project directory
      if (current_p.string().find("/scripts") != std::string::npos) {
        paths_to_remove.emplace_back(current_p);
      }
    }

    // Removes all unrelated source files from the project directory
    // Frate does allow you to make both a c and cpp project on top of each
    // other
    for (const path &current_p : std::filesystem::recursive_directory_iterator(
             pro->path / pro->src_dir)) {
      for (std::string ext : source_file_extensions_to_remove) {
        if (current_p.extension() == ext) {
          paths_to_remove.emplace_back(current_p);
        }
      }
    }

    // Iterates through all projects we marked for removal and removes them
    for (const path &p : paths_to_remove) {
      if (std::filesystem::is_directory(p)) {
        std::filesystem::remove_all(p);
      }
      else {
        std::filesystem::remove(p);
      }
    }

    return true;
  }

} // namespace Frate::Generators::Project
