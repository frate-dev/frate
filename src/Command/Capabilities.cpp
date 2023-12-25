#include <Frate/System.hpp>
#include <filesystem>
#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"
#include "Frate/Utils/Macros.hpp"


namespace Frate::System {
  void Capabilities::search_path(std::filesystem::path& path) {
    for (auto& p : std::filesystem::directory_iterator(path)) {
      if (p.is_regular_file()) {
        std::string filename = p.path().filename().string();
#ifdef _WIN32
        filename = filename.substr(0, filename.find(".exe"));
#endif
        if (filename.find("gcc") != std::string::npos) {
          this->compilers[filename] = Capability();
          this->compilers[filename].path = p.path();
          this->compilers[filename].installed = true;
          this->compilers[filename].version = getGccVersion(p.path());
          Utils::info << "Found gcc(" << p.path()
            << ") with version: " << this->compilers[filename].version
            << std::endl;

        } else if (filename.find("cmake") != std::string::npos) {
          this->cmake.path = p.path();
          this->cmake.installed = true;
          this->cmake.version = getCmakeVersion(p.path());
          Utils::info << "Found cmake(" << p.path()
            << ") with version: " << this->cmake.version << std::endl;
        }
      }
    }
  }
  bool Capabilities::search() {
    std::string path_env = std::getenv("PATH");
#ifdef _WIN32
    const char delimiter = ';';
    const std::string extension = ".exe";
#else
    const char delimiter = ':';
    const std::string extension = "";
#endif

    std::vector<std::string> paths = Utils::split(path_env, delimiter);
    for(std::filesystem::path path : paths){
      if(!std::filesystem::exists(path)){
        continue;
      }
      search_path(path);
    }


    return false;
  }

  Capabilities::Capabilities(){

  }

  Capability Capabilities::getLatestCompiler(std::string compiler){

    return Capability();
  }
  void from_json(const nlohmann::json &json_obj, Capabilities& capabilities){
    FROM_JSON_FIELD(capabilities, compilers);
    FROM_JSON_FIELD(capabilities, make);
    FROM_JSON_FIELD(capabilities, cmake);
    FROM_JSON_FIELD(capabilities, git);
    FROM_JSON_FIELD(capabilities, zip);
    FROM_JSON_FIELD(capabilities, unzip);
    FROM_JSON_FIELD(capabilities, tar);
    FROM_JSON_FIELD(capabilities, archive_compress);
    FROM_JSON_FIELD(capabilities, archive_expand);
  }
  void to_json(nlohmann::json &json_obj, const Capabilities& capabilities){
    TO_JSON_FIELD(capabilities, compilers);
    TO_JSON_FIELD(capabilities, make);
    TO_JSON_FIELD(capabilities, cmake);
    TO_JSON_FIELD(capabilities, git);
    TO_JSON_FIELD(capabilities, zip);
    TO_JSON_FIELD(capabilities, unzip);
    TO_JSON_FIELD(capabilities, tar);
    TO_JSON_FIELD(capabilities, archive_compress);
    TO_JSON_FIELD(capabilities, archive_expand);
  }
}
