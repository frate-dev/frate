#ifdef TEST
#include "Frate/Command.hpp"
#include <Frate/Test/Test.hpp>

bool Tests::Command::testAddPackageMultiple() {
  std::cout << "Testing add multiple packages : adding SDL and fmt" << std::endl;

  auto [failed, inter] = init("frate add p SDL2 fmt -l");

  if(failed){
    Frate::error << "Failed to add package : could not initialize test" << std::endl;
    return false;
  }

  nlohmann::json config = inter->pro->toJson();

  
  bool foundSDL = false;

  for(nlohmann::json &dep : config["dependencies"]){
    if(dep["name"] == "SDL2"){
      foundSDL = true;
    }
  }

  if(!foundSDL){
    cleanUp(test_path);
    Frate::error << "Failed to add package : could not find SDL2 in frate-project.json" << std::endl;
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
    Frate::error << "Failed to add package : could not find fmt in frate-project.json" << std::endl;
    return false;
  }

  return true;
}
#endif
