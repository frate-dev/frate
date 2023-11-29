#ifdef TEST
#include "Frate/Command.hpp"
#include <Frate/Test/Test.hpp>

bool Tests::Command::testAddPackageMultiple() {
  std::cout << "Testing add multiple packages : adding SDL and fmt" << std::endl;

  auto [argc, argv] = genCommand("frate add p SDL2 fmt -l");

  ::Command::Interface *inter = new ::Command::Interface(argc, argv);

  inter->pro->project_path = std::filesystem::path(test_path);

  if (!inter->execute()) {
    cleanUp(test_path);
    std::cout << "Failed to add package" << std::endl;
    return false;
  }

  std::ifstream config_file(test_path / "frate-project.json");

  nlohmann::json config;
  try {
    std::cout << "attempting to read config file" << std::endl;
    config_file >> config;
  } catch (...) {
    cleanUp(test_path);
    std::cout << "Failed to add package : could not open file - file possibly never created" << std::endl;
    return false;
  }
  
  bool foundSDL = false;

  for(nlohmann::json &dep : config["dependencies"]){
    if(dep["name"] == "SDL2"){
      foundSDL = true;
    }
  }

  if(!foundSDL){
    cleanUp(test_path);
    std::cout << "Failed to add package : could not find SDL2 in frate-project.json" << std::endl;
    return false;
  }

  bool foundfmt = false;

  for(nlohmann::json &dep : config["dependencies"]){
    if(dep["name"] == "fmt"){
      foundfmt = true;
    }
  }

  if(!foundfmt){
    cleanUp(test_path);
    std::cout << "Failed to add package : could not find fmt in frate-project.json" << std::endl;
    return false;
  }

  return true;
}
#endif
