#include <string>
#include <unordered_map>
#include <vector>


namespace Frate::System {

  class Capabilities {
    public:
      bool cmake_installed;
      std::string cmake_version;

      bool git_installed;
      std::string git_version;

      bool zip_installed;
      std::string zip_version;

      bool unzip_installed;
      std::string unzip_version;

      bool tar_installed;
      std::string tar_version;

      // compilers and installed versions
      std::unordered_map<std::string,std::vector<std::string>> compilers;


      //Gets the capabilities of the system
      Capabilities();

  };

}
