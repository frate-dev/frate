#pragma once
#include <Frate/System/Capability.hpp>

namespace Frate::System {

    class Capabilities {
        /*
         * Given the provided path, this will search for certain keywords that
         * corrospond to a capabilities
         */
      private:
        void search_path(std::filesystem::path &path);
        void get_make_capability(
            const std::filesystem::path &path,
            std::string &name);
        void get_cmake_capability(const std::filesystem::path &path, std::string &name);
        void get_git_capability(const std::filesystem::path &path, std::string &name);
        void get_zip_capability(const std::filesystem::path &path, std::string &name);
        void get_unzip_capability(const std::filesystem::path &path, std::string &name);
        void get_tar_capability(const std::filesystem::path &path, std::string &name);
        void get_archive_compress_capability(const std::filesystem::path &path, std::string &name);
        void get_archive_expand_capability(const std::filesystem::path &path, std::string &name);
        void get_sccache_capability(const std::filesystem::path &path, std::string &name);
        void get_ninja_capability(const std::filesystem::path &path, std::string &name);
        void get_rsync_capability(const std::filesystem::path &path, std::string &name);
        void get_ssh_capability(const std::filesystem::path &path, std::string &name);
        void get_python_capability(const std::filesystem::path &path, std::string &name);
        void get_python3_capability(const std::filesystem::path &path, std::string &name);
        void get_python2_capability(const std::filesystem::path &path, std::string &name);
        void get_compilers_capability(const std::filesystem::path &path, std::string &name);

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
        Capability sccache;
        Capability ninja;
        Capability rsync;
        Capability python;
        Capability python3;
        Capability python2;
        Capability ssh;

        // compilers and installed versions
        std::unordered_map<std::string, Capability> compilers;

        friend void from_json(const nlohmann::json &json_obj, Capabilities &capabilities);
        friend void to_json(nlohmann::json &json_obj, const Capabilities &capabilities);

        Capability getLatestCompiler(std::string compiler);
        bool search();
        bool has(Capability capability);
    };

} // namespace Frate::System
