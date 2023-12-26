#ifdef TEST
#include <Frate/Utils/General.hpp>
#include <Frate/Test/Test.hpp>
#include <Frate/Interface.hpp>
#include <filesystem>
#include <memory>
#include <Frate/Project.hpp>
#include <fstream>

namespace Tests::Command {
  using std::filesystem::path;
  using Frate::Command::Interface;


  std::filesystem::path genTestDirectory(){
    Frate::Utils::info << "Generating test directory" << std::endl;
    std::string random_string = genBase64String(10);
    std::filesystem::path test_path =
      std::filesystem::path("/tmp/frate-test-" + random_string);

    while(std::filesystem::exists(test_path)){
      random_string = genBase64String(10);
      test_path =
        std::filesystem::path("/tmp/frate-test-" + random_string);
    }

    try{
      std::filesystem::create_directory(test_path);
    }catch(...){
      Frate::Utils::error << "Failed to create test directory" << std::endl;
      return "";
    }

    Frate::Utils::info << "Test directory generated at" << test_path << std::endl;
    return test_path;
  }
  void cleanUp(path test_path) {
    try {
      std::filesystem::remove_all(test_path);
    } catch (...) {
      Frate::Utils::error << "Failed to clean up test directory" << std::endl;
    }
  }
  std::vector<std::string> parseArguments(const std::string& input) {
    std::vector<std::string> args;
    std::string arg;
    bool inQuotes = false;

    for (char ch : input) {
      if (ch == '\'') {
        inQuotes = !inQuotes;
      } else if (ch == ' ' && !inQuotes) {
        if (!arg.empty()) {
          args.emplace_back(arg);
          arg.clear();
        }
      } else {
        arg += ch;
      }
    }

    if (!arg.empty()) {
      args.emplace_back(arg);
    }

    return args;
  }

  std::pair<int, char**> genCommand(std::string command){
    std::vector<std::string> command_split = parseArguments(command);
    char **argv = new char*[command_split.size()];
    for(int i = 0; i < command_split.size(); i++){
      argv[i] = new char[command_split[i].size()];
      strcpy(argv[i], command_split[i].c_str());
    }

    return std::make_pair(command_split.size(), argv);
  }
  std::pair<bool, std::shared_ptr<Interface>> init(std::string command,bool check_config) {

    auto [argc, argv] = genCommand(command);

    Frate::Command::Interface inter(argc, argv);

    std::shared_ptr<Interface> inter_ptr = std::make_shared<Interface>(inter);


    inter.pro->path = std::filesystem::path(test_path);

    if (!Frate::Command::execute(inter_ptr)) {


      Frate::Utils::error << "Failed to run command: " << command << " : could not execute" << std::endl;
      return std::make_pair(true,inter_ptr);
    }
    if(check_config){
      std::ifstream config_file(test_path / "frate-project.json");
      nlohmann::json config;

      if(!std::filesystem::exists(test_path / "frate-project.json")){
        Frate::Utils::error << "Failed to run command : " << command << " : no config file" << std::endl;
        return std::make_pair(true,inter_ptr);
      }

      if(!validateProjectJson(inter_ptr)){
        Frate::Utils::error << "Failed to run command : " << command << " : invalid json" << std::endl;
        return std::make_pair(true,inter_ptr);
      }
    }
    return std::make_pair(false,inter_ptr);

  }
}
#endif
