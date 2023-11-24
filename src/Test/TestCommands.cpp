#ifdef TEST
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <catch2/catch_test_macros.hpp>
#include <Frate/Command.hpp>
#include <cxxopts.hpp>
#include <string>
#include <Frate/Test/Test.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Command.hpp>

namespace Tests{
  std::string genBase64String(int random_string_length) {
    std::string base64_chars =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789-_";

    std::string random_string;
    for (int i = 0; i < random_string_length; ++i) {
      random_string += base64_chars[(rand() + time(NULL)) % base64_chars.length()];
    }
    return random_string;
  }
}



namespace Tests::Command {
  using nlohmann::json;

  const std::filesystem::path test_path = std::filesystem::path("/tmp/frate-test");
  
  void cleanUp(){
    try{
      std::filesystem::remove_all(test_path);
    }catch(...){
      std::cout << "Failed to clean up test directory" << std::endl;
    }
  }
  
  void init(){
    cleanUp();
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

  bool testNew(){
    std::cout << "Testing new command" << std::endl;
    init();
    auto [argc,argv] = genCommand("frate new test -d");
    ::Command::Interface *inter = new ::Command::Interface(argc,argv);

    inter->pro->project_path = std::filesystem::path(test_path);


    if(!inter->execute()){
      cleanUp();
      std::cout << "Failed to create new project" << std::endl;
      return false;
    }
    return true;

  }

  bool testAddPackage(std::string package_name) {
    std::cout << "Testing add package command" << std::endl;
    auto [argc, argv] = genCommand("frate add p "+package_name+" -l");
    ::Command::Interface *inter = new ::Command::Interface(argc, argv);

    inter->pro->project_path = std::filesystem::path(test_path);

    if (!inter->execute()) {
      cleanUp();
      std::cout << "Failed to add package" << std::endl;
      return false;
    }
    
    std::ifstream config_file(test_path / "config.json");
    try{
      nlohmann::json config;
      config_file >> config;
      if(config["dependencies"].size() != 1){
        cleanUp();
        std::cout << "Failed to add package : no pacakge add dected config.json" << std::endl;
        return false;
      }
    }catch(...){
      cleanUp();
      std::cout << "Failed to add package : could not open file - file possibly never created" << std::endl;
      return false;
    }

    return true;
  }

  bool testAddPackageMultiple() {
    std::cout << "Testing add multiple packages : adding SDL and fmt" << std::endl;

    auto [argc, argv] = genCommand("frate add p SDL2 fmt -l");

    ::Command::Interface *inter = new ::Command::Interface(argc, argv);

    inter->pro->project_path = std::filesystem::path(test_path);

    if (!inter->execute()) {
      cleanUp();
      std::cout << "Failed to add package" << std::endl;
      return false;
    }

    std::ifstream config_file(test_path / "config.json");

    nlohmann::json config;
    try {
      std::cout << "attempting to read config file" << std::endl;
      config_file >> config;
    } catch (...) {
      cleanUp();
      std::cout << "Failed to add package : could not open file - file possibly never created" << std::endl;
      return false;
    }

    std::cout << "checking config for 2 packages" << std::endl;
    if (config["dependencies"].size() != 2) {
      cleanUp();
      std::cout << "Failed to add package : expected 2 packages to be added" << std::endl;
      return false;
    }

    std::cout << "checking config for SDL package" << std::endl;
    if(config["dependencies"][0]["name"] != "SDL2"){
      cleanUp();
      std::cout << "Failed to add package : expected SDL2 to be added got: " << config["dependencies"][0]["name"] << std::endl;
      return false;
    }
    std::cout << "checking config for fmt package" << std::endl;
    if (config["dependencies"][1]["name"] != "fmt") {
      cleanUp();
      std::cout << "Failed to add package : expected fmt to be added got: " << config["dependencies"][1]["name"] << std::endl;
      return false;
    }
    return true;
  }

  bool testRemovePackage(){
    std::cout << "Testing remove package command" << std::endl;
    auto [argc, argv] = genCommand("frate remove p cxxopts");

    ::Command::Interface *inter = new ::Command::Interface(argc, argv);

    if (!inter->execute()) {
      cleanUp();
      std::cout << "Failed to remove package : could not run command" << std::endl;
      return false;
    }

    std::cout << "Searching for cxxopts in config.json" << std::endl;

    std::ifstream config_file(test_path / "config.json");

    nlohmann::json config;

    try {
      config_file >> config;
    } catch (...) {
      cleanUp();
      std::cout << "Failed to remove package : could not open file - file possibly never created" << std::endl;
      return false;
    }


    for (auto& dep : config["dependencies"]) {
      if (dep["name"] == "cxxopts") {
        std::cout << "Failed to remove package : cxxopts still in config.json" << std::endl;
        cleanUp();
        return false;
      }
    }

    return true;
  }
  bool testAddPackageToMode(std::string mode_name, std::string package_name){
    std::cout << "Testing add package to mode command" << std::endl;
    auto [argc, argv] = genCommand("frate add p "+package_name+" -l -m "+mode_name);

    ::Command::Interface *inter = new ::Command::Interface(argc, argv);

    inter->pro->project_path = std::filesystem::path(test_path);

    if (!inter->execute()) {
      cleanUp();
      std::cout << "Failed to add package : could not run command" << std::endl;
      return false;
    }

    std::ifstream config_file(test_path / "config.json");

    nlohmann::json config;

    try {
      config_file >> config;
    } catch (...) {
      cleanUp();
      std::cout << "Failed to add package : could not open file - file possibly never created" << std::endl;
      return false;
    }

    for(json mode : config["modes"]){
      if(mode["name"] == mode_name){
        for(json dep : mode["dependencies"]){
          if(dep["name"] == package_name){
            return true;
          }
        }
      }
    }

    std::cout << "Failed to add package to mode "+mode_name+" : the selected mode either doesn't exist or we couldn't find " + package_name << std::endl;

    return false;

  }


  TEST_CASE("TestCommands", "[commands]"){
    REQUIRE(testNew());
    REQUIRE(testAddPackage("cxxopts"));
    REQUIRE(testAddPackageMultiple());
    REQUIRE(testRemovePackage());
    REQUIRE(testAddPackageToMode("Debug","cxxopts"));
    REQUIRE(testAddPackageToMode("Test","cxxopts"));
    REQUIRE(testAddPackageToMode("Release","cxxopts"));
    //REQUIRE(testCommandAdd());
  }
}

#endif
