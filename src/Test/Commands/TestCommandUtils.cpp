#ifdef TEST
#include <Frate/Utils/General.hpp>
#include <Frate/Test/Test.hpp>
#include <Frate/Command.hpp>
#include <filesystem>

namespace Tests::Command {
  using std::filesystem::path;

  extern Frate::Utils::Error error =Frate::Utils::Error();
  extern Frate::Utils::Info info =Frate::Utils::Info();
  extern Frate::Utils::Warning warning =Frate::Utils::Warning();

  std::filesystem::path genTestDirectory(){
    info << "Generating test directory" << std::endl;
    std::string random_string = genBase64String(10);
    std::filesystem::path test_path =
      std::filesystem::path("/tmp/frate-test-" + random_string);

    while(std::filesystem::exists(test_path)){
      random_string = genBase64String(10);
      test_path =
        std::filesystem::path("/tmp/frate-test-" + random_string);
    }

    std::filesystem::create_directory(test_path);
    info << "Test directory generated at" << test_path << std::endl;
    return test_path;
  }
  void cleanUp(path test_path) {
    try {
      std::filesystem::remove_all(test_path);
    } catch (...) {
      error << "Failed to clean up test directory" << std::endl;
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
          args.push_back(arg);
          arg.clear();
        }
      } else {
        arg += ch;
      }
    }

    if (!arg.empty()) {
      args.push_back(arg);
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
  std::pair<bool, Frate::Command::Interface*> init(std::string command,bool check_config) {

    auto [argc, argv] = genCommand(command);

   Frate::Command::Interface *inter = new Frate::Command::Interface(argc, argv);

    inter->pro->path = std::filesystem::path(test_path);

    if (!Frate::Command::execute(
          std::make_shared<Frate::Command::Interface>(*inter))) {


      error << "Failed to run command: " << command << " : could not execute" << std::endl;
      return std::make_pair(true,inter);
    }
    if(check_config){
      std::ifstream config_file(test_path / "frate-project.json");
      nlohmann::json config;

      if(!std::filesystem::exists(test_path / "frate-project.json")){
        error << "Failed to run command : " << command << " : no config file" << std::endl;
        return std::make_pair(true,inter);
      }


      // try {
      //   info << "attempting to read config file" << std::endl;
      //   config_file >> config;
      // } catch (...) {
      //   error << 
      //     "Failed to add package : could not open file - file possibly never created" 
      //     << std::endl;
      //   return std::make_pair(true,inter);
      // }

      if(!validateProjectJson(inter)){
        error << "Failed to run command : " << command << " : invalid json" << std::endl;
        return std::make_pair(true,inter);
      }
    }
    return std::make_pair(false,inter);

  }
}
#endif
