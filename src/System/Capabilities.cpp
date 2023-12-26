#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"
#include "Frate/Utils/Macros.hpp"
#include <Frate/System/Capabilities.hpp>
#include <Frate/System/Capability.hpp>
#include <filesystem>

namespace Frate::System {
  using std::filesystem::directory_entry;

  void Capabilities::search_path(std::filesystem::path &path) {
    for (const directory_entry &current_path :
         std::filesystem::directory_iterator(path)) {

      if (current_path.is_regular_file()) {

        std::string filename = current_path.path().filename().string();

#ifdef _WIN32

        filename = filename.substr(0, filename.find(".exe"));

#endif
        if (filename.find("gcc") != std::string::npos) {
          Utils::info << "Found gcc: " + filename << std::endl;
          if (this->compilers[filename].installed) {
            continue;
          }

          this->get_compilers_capability(current_path.path(), filename);

          Utils::info << "Found gcc: " + filename << " with version"
                      << this->compilers[filename].version << std::endl;

        } else if (filename == "cmake") {
          if (this->cmake.installed) {
            continue;
          }

          this->get_cmake_capability(current_path.path(), filename);
          Utils::info << "Found cmake: " + filename << " with version"
                      << this->cmake.version << std::endl;
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
    const std::string extension;
#endif

    std::vector<std::string> paths = Utils::split(path_env, delimiter);
    for (std::filesystem::path path : paths) {
      if (!std::filesystem::exists(path)) {
        continue;
      }
      search_path(path);
    }

    return false;
  }

  Capabilities::Capabilities() = default;

  Capability Capabilities::getLatestCompiler(std::string compiler) {

    return {};
  }

  void from_json(const nlohmann::json &json_obj, Capabilities &capabilities) {
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

  void to_json(nlohmann::json &json_obj, const Capabilities &capabilities) {
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
} // namespace Frate::System
