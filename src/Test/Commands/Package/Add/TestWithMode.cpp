#ifdef TEST
#include <Frate/Test/Test.hpp>
bool Tests::Command::testAddPackageToMode(std::string mode_name, std::string package_name){
  using nlohmann::json;
  std::cout << "Testing add package to mode command" << std::endl;
  auto [argc, argv] = genCommand("frate add p "+package_name+" -l -m "+mode_name);

  ::Command::Interface *inter = new ::Command::Interface(argc, argv);

  inter->pro->project_path = std::filesystem::path(test_path);

  if (!inter->execute()) {
    cleanUp(test_path);
    std::cout << "Failed to add package : could not run command" << std::endl;
    return false;
  }

  std::ifstream config_file(test_path / "frate-project.json");

  nlohmann::json config;

  try {
    config_file >> config;
  } catch (...) {
    cleanUp(test_path);
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
#endif
