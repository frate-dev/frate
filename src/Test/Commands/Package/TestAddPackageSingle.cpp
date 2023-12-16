#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <fstream>

bool Tests::Command::testAddPackage(std::string package_name) {
  std::cout << "Testing add package command" << std::endl;
  auto [failed, inter] = init("frate add p "+package_name+" -l");

  if(failed){
    Frate::Utils::error << "Failed to add package : could not initialize interface" << std::endl;
    return false;
  }

  std::ifstream config_file(test_path / "frate-project.json");
  try{
    nlohmann::json config;
    config_file >> config;
    if(config["dependencies"].size() != 1){
      
      Frate::Utils::error << "Failed to add package : no pacakge add dected frate-project.json" << std::endl;
      return false;
    }
  }catch(...){
    
    Frate::Utils::error << "Failed to add package : could not open file - file possibly never created" << std::endl;
    return false;
  }

  return true;
}
#endif
