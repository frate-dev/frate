#include <Frate/System.hpp>

std::string Frate::System::getGccVersion(std::filesystem::path gccPath){
  (void)gccPath;
  //std::string gccVersion = Frate::Utils::CLI::run("gcc --version");
  // std::regex regex("gcc \\(.*\\) ([0-9]+\\.[0-9]+\\.[0-9]+)");
  // std::smatch match;
  // std::regex_search(gccVersion, match, regex);
  // return match[1];
  return "9.3.0";
}
