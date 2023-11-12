#include "Command.hpp"
#include <cstddef>
#include <format>
#include <iostream>

namespace Command {
  bool Interface::LoadPackageJson() {
    using nlohmann::json;
    std::fstream file;
    std::string file_name = "config.json";
    try {
      std::cout << "Project path: " << pro->project_path << std::endl;
      file.open((pro->project_path / file_name).string());
      json data = json::parse(file);
      pro->fromJson(data);
      //Simplfied this fucking code
    } catch (json::exception &e) {
      std::cout << "Error: Could not load: " << (pro->project_path / file_name) << std::endl;
      return false;
    }
    file.close();

    return true;
  };

} // namespace Command
