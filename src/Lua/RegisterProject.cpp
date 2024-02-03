#include <Frate/Lua/LuaAPI.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Project/Mode.hpp>
#include <Frate/Project/ProjectPrompt.hpp>

namespace Frate::Lua {
  [[deprecated("Use the template environment instead")]]
  bool registerProject(sol::state &lua, std::shared_ptr<Project::Config> pro) {

    lua.set("project", pro);
    // clang-format off
    lua.new_usertype<Command::Package>("Package",
        "new",
        sol::no_constructor,
        "name",
        &Command::Package::name,
        "git",
        &Command::Package::git,
        "git_short",
        &Command::Package::git_short,
        "git_prefixed",
        &Command::Package::git_prefixed,
        "versions",
        &Command::Package::versions,
        "target_link",
        &Command::Package::target_link,
        "description",
        &Command::Package::description,
        "git_description",
        &Command::Package::git_description,
        "language",
        &Command::Package::language,
        "license",
        &Command::Package::license,
        "owner",
        &Command::Package::owner,
        "owner_type",
        &Command::Package::owner_type,
        "stars",
        &Command::Package::stars,
        "forks",
        &Command::Package::forks,
        "open_issues",
        &Command::Package::open_issues,
        "watchers",
        &Command::Package::watchers);
    lua.new_usertype<Command::Dependency>("Dependency",
        "new",
        sol::no_constructor,
        "name",
        &Command::Dependency::name,
        "version",
        &Command::Dependency::version,
        "git",
        &Command::Dependency::git,
        "git_short",
        &Command::Dependency::git_short,
        "git_prefixed",
        &Command::Dependency::git_prefixed,
        "target_link", 
        &Command::Dependency::target_link);

    lua.new_usertype<Project::Mode>("Mode",
        "new",
        sol::no_constructor,
        "name",
        &Project::Mode::name,
        "flags",
        &Project::Mode::flags,
        "dependencies",
        &Project::Mode::dependencies);

    lua.new_usertype<Project::Config>("Project"
        "new",
        sol::no_constructor,
        "name",
        &Project::Config::name,
        "version",
        &Project::Config::version,
        "description",
        &Project::Config::description,
        "authors",
        &Project::Config::authors,
        "dependencies",
        &Project::Config::dependencies,
        "toolchains",
        &Project::Config::toolchains,
        "flags",
        &Project::Config::flags,
        "modes",
        &Project::Config::modes,
        "libs",
        &Project::Config::libs,
        "license",
        &Project::Config::license,
        "git",
        &Project::Config::git,
        "cmake_version",
        &Project::Config::cmake_version,
        "build_command",
        &Project::Config::build_command,
        "build_dir",
        &Project::Config::build_dir,
        "src_dir",
        &Project::Config::src_dir,
        "include_dir",
        &Project::Config::include_dir,
        "lang_version",
        &Project::Config::lang_version,
        "lang",
        &Project::Config::lang,
        "project_type",
        &Project::Config::type,
        "keywords",
        &Project::Config::keywords,
        "prompts",
        &Project::Config::prompts);

    lua.new_usertype<Project::ProjectPrompt>(
        "ProjectPrompt",
        "value",
        &Project::ProjectPrompt::value,
        "default",
        &Project::ProjectPrompt::default_value,
        "get_str",
        &Project::ProjectPrompt::get<std::string>,
        "get_int",
        &Project::ProjectPrompt::get<int>,
        "get_bool",
        &Project::ProjectPrompt::get<bool>,
        "get_float",
        &Project::ProjectPrompt::get<float>);


    // clang-format on
    return true;
  }
} // namespace Frate::Lua
