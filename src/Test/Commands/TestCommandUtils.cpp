#ifdef TEST
#include <Frate/Utils/General.hpp>
#include <filesystem>

namespace Tests::Command {
using std::filesystem::path;

  void cleanUp(path test_path) {
    try {
      std::filesystem::remove_all(test_path);
    } catch (...) {
      std::cout << "Failed to clean up test directory" << std::endl;
    }
  }

  void init(path test_path) {
    cleanUp(test_path);
    std::filesystem::create_directory(test_path);
  }

  std::pair<int, char**> genCommand(std::string args){
    std::vector<std::string> command_split = ::Utils::split(args, ' ');
    char** argv = new char*[command_split.size()];

    for(size_t i = 0; i < command_split.size(); i++){

      argv[i] = new char[command_split[i].size()];

      strcpy(argv[i], command_split[i].c_str());

    }

    return std::make_pair(command_split.size(), argv);
  }
}
#endif
