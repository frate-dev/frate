#ifdef TEST
#include <Frate/Test/Test.hpp>
bool Tests::Command::testAddPackageToMode(std::string mode_name, std::string package_name){
  using nlohmann::json;
  std::cout << "Testing add package to mode command" << std::endl;
  
  if(!testNew()){
    Utils::error << "Failed to create new project" << std::endl;
    return false;
  }
  auto [failed,inter] = init("frate add p "+package_name + " -m " + mode_name + " -l");
  if(failed){
    Utils::error << "Failed to add package" << std::endl;
    return false;
  }

  json config = inter->pro->toJson();

  for(json mode : config["modes"]){
    if(mode["name"] == mode_name){
      for(json dep : mode["dependencies"]){
        if(dep["name"] == package_name){
          return true;
        }
      }
    }
  }

  Utils::error << "Failed to add package to mode "+mode_name+" : the selected mode either doesn't exist or we couldn't find " + package_name << std::endl;

  return false;

}
#endif
