#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>


namespace Frate::System {
  std::string getGccVersion(std::filesystem::path path);
  std::string getClangVersion(std::filesystem::path path);
  std::string getMsvcVersion(std::filesystem::path path);
  std::string getMingwVersion(std::filesystem::path path);
  std::string getGitVersion(std::filesystem::path path);
  std::string getCmakeVersion(std::filesystem::path path);
  std::string getMakeVersion(std::filesystem::path path);
  std::string getZipVersion(std::filesystem::path path);
  std::string getUnzipVersion(std::filesystem::path path);
  std::string getTarVersion(std::filesystem::path path);
  std::string getArchiveCompressVersion(std::filesystem::path path);
  std::string getArchiveExpandVersion(std::filesystem::path path);


  class Capability {
    public:
      Capability();
      std::string executable;
      std::string version;
      std::string path;
      bool installed;
      friend void from_json(const nlohmann::json &j, Capability& capability);
      friend void to_json(nlohmann::json &j, const Capability& capability);
  };

  class Capabilities {
    public:
      Capabilities();

      Capability make;
      Capability cmake;
      Capability git;
      Capability zip;
      Capability unzip;
      Capability tar;
      Capability archive_compress;
      Capability archive_expand;

      // compilers and installed versions
      std::unordered_map<std::string,Capability> compilers;

      friend void from_json(const nlohmann::json &j, Capabilities& capabilities);
      friend void to_json(nlohmann::json &j, const Capabilities& capabilities);

      Capability getLatestCompiler(std::string compiler);
      bool search();
  };

}
