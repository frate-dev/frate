#include "Command.hpp"
#include <cstddef>
#include <format>
#include <iostream>

namespace Command {
  bool Interface::LoadPackageJson() {
    using nlohmann::json;
    try {
      std::string file_name = "config.json";
      std::fstream file;
      file.open((pro->project_path / file_name).string());
      json data = json::parse(file);
      pro->fromJson(data);
      //Simplfied this fucking code
    } catch (json::exception &e) {
      std::cout << "Error: Could not load config.json" << std::endl;
      return false;
    }

    return true;
  };

} // namespace Command
