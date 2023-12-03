#ifdef TEST
#include <Frate/Test/Test.hpp>


bool Tests::Command::testRemovePackage(std::string package_name){
  std::cout << "Testing remove package command" << std::endl;

  cleanUp(test_path);
  
  if(!testNew()){
    Frate::error << "Failed to create new project" << std::endl;
    return false;
  }

  if(!testAddPackage(package_name)){
    Frate::error << "Failed to add package" << std::endl;
    return false;
  }

  auto [failed, inter] = init("frate remove p " + package_name);

  if (failed) {
    return false;
  }

  std::cout << "Searching for " +package_name+ " in frate-project.json" << std::endl;

  std::ifstream config_file(test_path / "frate-project.json");

  nlohmann::json config;

  try {
    config_file >> config;
  } catch (...) {
    cleanUp(test_path);
    Frate::error << "Failed to remove package : could not open file - file possibly never created" << std::endl;
    return false;
  }


  for (auto& dep : config["dependencies"]) {
    if (dep["name"] == package_name) {
      Frate::error << "Failed to remove package : " << package_name << " still in frate-project.json" << std::endl;
      cleanUp(test_path);
      return false;
    }
  }

  return true;
}
#endif