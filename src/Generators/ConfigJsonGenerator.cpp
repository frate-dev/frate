#include <Frate/Generators.hpp>
#include <Frate/Command.hpp>
#include <string>
#include <termcolor/termcolor.hpp>

namespace Generators::ConfigJson{

  bool writeConfig(std::shared_ptr<Command::Project> &pro) {
    std::ofstream file;
    std::string file_name = "frate-project.json";
    file.open(pro->project_path / file_name);
    file << pro->toJson().dump(2);
    file << '\n';
    file.close();
    return true;
  }
}
