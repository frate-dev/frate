#include <Frate/System.hpp>
#include <Frate/Utils/General.hpp>
#include <regex>

std::string Frate::System::getGccVersion(std::filesystem::path gccPath){
  (void)gccPath;
  std::string gccVersion = Frate::Utils::hSystemWithOutput("gcc --version").std_out;
  std::regex regex("gcc \\(.*\\) ([0-9]+\\.[0-9]+\\.[0-9]+)");
  std::smatch match;
  std::regex_search(gccVersion, match, regex);
  return match[1];
}
