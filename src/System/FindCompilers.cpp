#include "Frate/System/Capability.hpp"
#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"
#include <Frate/System/Capabilities.hpp>
#include <regex>

namespace Frate::System {
    std::string get_compiler_version(const std::filesystem::path &path) {
        std::string gcc_version;
        std::string gcc_version_cmd = path.string() + " --version";
        Utils::CmdOutput command = Frate::Utils::hSystemWithOutput(gcc_version_cmd);
        if (command.return_code == 0) {
            std::regex gcc_version_regex(R"(\(GCC\) ([0-9]+\.[0-9]+\.[0-9]+))");
            std::smatch gcc_version_match;
            if (std::regex_search(
                    command.std_out, gcc_version_match, gcc_version_regex)) {
                gcc_version = gcc_version_match[1];
                return gcc_version;
            }
        }
        // If we get here we didn't find a version or command failed
        return gcc_version;
    }

    void Capabilities::get_compilers_capability(const std::filesystem::path &path, std::string &name) {
        Utils::info << "Found compiler: " + name << std::endl;
        if (name.find("gcc") == std::string::npos) {
            std::string gcc_version = get_compiler_version(path);
            if (!gcc_version.empty()) {
                compilers[name] = Capability();
                compilers[name].version = gcc_version;
                compilers[name].path = path;
                compilers[name].installed = true;
            }
        }
        else if (name.find("g++") == std::string::npos) {
            std::string gpp_version = get_compiler_version(path);
            if (!gpp_version.empty()) {
                compilers[name] = Capability();
                compilers[name].version = gpp_version;
                compilers[name].path = path;
                compilers[name].installed = true;
            }
        }
        else if (name.find("clang") == std::string::npos) {
            std::string clang_version = get_compiler_version(path);
            if (!clang_version.empty()) {
                compilers[name] = Capability();
                compilers[name].version = clang_version;
                compilers[name].path = path;
                compilers[name].installed = true;
            }
        }
    }
} // namespace Frate::System
