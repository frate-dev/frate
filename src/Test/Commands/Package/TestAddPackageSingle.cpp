#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <fstream>

bool Tests::Command::testAddPackage(std::string package_name) {
  std::cout << "Testing add package command" << std::endl;

  if (!testNew()) {
    return false;
  }

  auto [failed, inter] = init("frate add p " + package_name + " -l");

  if (failed) {
    Frate::Utils::error
        << "Failed to add package : could not initialize interface"
        << std::endl;
    return false;
  }

  std::ifstream config_file(test_path / Frate::Constants::PROJECT_CONFIG_NAME);
  try {
    nlohmann::json config;
    config_file >> config;
    if (config["dependencies"].size() != 1) {

      Frate::Utils::error << "Failed to add package : no pacakge add detected " +
                            Frate::Constants::PROJECT_CONFIG_NAME 
                          << std::endl;
      return false;
    }
  } catch (...) {

    Frate::Utils::error << "Failed to add package : could not open file - file "
                           "path: " +
                               (test_path / Frate::Constants::PROJECT_CONFIG_NAME).string() +
                               "possibly never created"
                        << std::endl;
    return false;
  }

  return true;
}
#endif
