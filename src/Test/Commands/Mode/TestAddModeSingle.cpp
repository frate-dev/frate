#include "Frate/Test/Test.hpp"
#include "nlohmann/json_fwd.hpp"
#ifdef TEST

bool Tests::Command::testAddMode(std::string mode_name){
  Utils::info << "Testing add mode command normal conditions" << std::endl;
  if(!testNew()) return false;

  auto [failed, inter] = init("frate add mode " + mode_name);

  if(failed) return false;

  nlohmann::json config = inter->pro->toJson();

  for(auto &mode : config["modes"]){
    if(mode["name"] == mode_name){
      return true;
    }
  }
  
  Utils::error << "Failed to add mode : could not find " << mode_name << " in frate-project.json" << std::endl;
  return true;
}
#endif
