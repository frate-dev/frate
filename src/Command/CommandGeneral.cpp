#include <stdint.h> 
#include "termcolor/termcolor.hpp"
#include <Frate/Command.hpp>
#include <cstddef>
#include <iostream>

namespace Command {
  bool Interface::LoadPackageJson() {
    using nlohmann::json;
    std::fstream file;
    std::string file_name = "config.json";
    try {
      file.open((pro->project_path / file_name).string());
      json data = json::parse(file);
      pro->fromJson(data);
      //Simplfied this fucking code
    } catch (json::exception &e) {
      std::cout << termcolor::bright_red << "Error: Could not load: " << (pro->project_path / file_name) << termcolor::reset << std::endl;
      return false;
    }
    file.close();

    return true;
  };

} // namespace Command
