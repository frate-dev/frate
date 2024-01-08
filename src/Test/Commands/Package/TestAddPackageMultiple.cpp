#ifdef TEST
#include <Frate/Project/Config.hpp>
#include <Frate/Test/Test.hpp>

bool Tests::Command::testAddPackageMultiple(std::vector<std::string> packages) {
  std::cout << "Testing add multiple packages : adding ";

  if (!testNew())
    return false;

  std::string command = "frate add p ";

  for (auto package : packages) {
    std::cout << package << " ";
    command += package + " ";
  }

  command += "-l";
  auto [failed, inter] = init(command);

  if (failed)
    return false;

  nlohmann::json config = *inter->pro;

  std::vector<std::pair<std::string, bool>> package_results;

  for (std::string &p : packages) {
    package_results.emplace_back(std::make_pair(p, false));
  }

  for (nlohmann::json &dep : config["dependencies"]) {
    for (auto &p : package_results) {
      if (dep["name"] == p.first) {
        p.second = true;
      }
    }
  }

  for (auto &p : package_results) {
    if (!p.second) {
      Frate::Utils::error << "Failed to add package : could not find "
                          << p.first << " in frate-project.json" << std::endl;
      return false;
    }
  }

  return true;
}
#endif
