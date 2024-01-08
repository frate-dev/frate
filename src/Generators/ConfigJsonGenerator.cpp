#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <Frate/Project/Config.hpp>
#include <fstream>
#include <string>
#include <termcolor/termcolor.hpp>

namespace Frate::Generators::ConfigJson {
  // [[deprecated("Use project->writeConfig() instead")]]
  bool writeConfig(std::shared_ptr<Project::Config> &pro) {
    std::ofstream file;
    std::string file_name = "frate-project.json";
    file.open(pro->path / file_name);
    file << nlohmann::json(*pro).dump(2);
    file << '\n';
    file.close();
    return true;
  }
} // namespace Frate::Generators::ConfigJson
