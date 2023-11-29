#ifdef TEST
#include <Frate/Test/Test.hpp>


bool Tests::Command::testRemovePackage(){
  std::cout << "Testing remove package command" << std::endl;
  auto [argc, argv] = genCommand("frate remove p cxxopts");

  ::Command::Interface *inter = new ::Command::Interface(argc, argv);


  inter->pro->project_path = std::filesystem::path(test_path);

  if (!inter->execute()) {
    cleanUp(test_path);
    std::cout << "Failed to remove package : could not run command" << std::endl;
    return false;
  }

  if(!validateProjectJson(inter)){
    cleanUp(test_path);
    return false;
  }
  std::cout << "Searching for cxxopts in config.json" << std::endl;

  std::ifstream config_file(test_path / "config.json");

  nlohmann::json config;

  try {
    config_file >> config;
  } catch (...) {
    cleanUp(test_path);
    std::cout << "Failed to remove package : could not open file - file possibly never created" << std::endl;
    return false;
  }


  for (auto& dep : config["dependencies"]) {
    if (dep["name"] == "cxxopts") {
      std::cout << "Failed to remove package : cxxopts still in config.json" << std::endl;
      cleanUp(test_path);
      return false;
    }
  }

  return true;
}
#endif
