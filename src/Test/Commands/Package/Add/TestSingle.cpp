#ifdef TEST
#include <Frate/Test/Test.hpp>

bool Tests::Command::testAddPackage(std::string package_name) {
  std::cout << "Testing add package command" << std::endl;
  auto [argc, argv] = genCommand("frate add p "+package_name+" -l");
  ::Command::Interface *inter = new ::Command::Interface(argc, argv);

  inter->pro->project_path = std::filesystem::path(test_path);

  if (!inter->execute()) {
    cleanUp(test_path);
    std::cout << "Failed to add package" << std::endl;
    return false;
  }

  std::ifstream config_file(test_path / "frate-project.json");
  try{
    nlohmann::json config;
    config_file >> config;
    if(config["dependencies"].size() != 1){
      cleanUp(test_path);
      std::cout << "Failed to add package : no pacakge add dected frate-project.json" << std::endl;
      return false;
    }
  }catch(...){
    cleanUp(test_path);
    std::cout << "Failed to add package : could not open file - file possibly never created" << std::endl;
    return false;
  }

  return true;
}
#endif
