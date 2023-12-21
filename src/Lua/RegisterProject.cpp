#include <Frate/LuaAPI.hpp>
#include <Frate/Mode.hpp>
#include <Frate/Project.hpp>
#include <Frate/ProjectPrompt.hpp>

namespace Frate::LuaAPI {
  bool registerProject(sol::state &lua, std::shared_ptr<Command::Project> pro) {

    lua.set("project", pro);
    lua.new_usertype<Command::Package>("Package",
        "new", sol::no_constructor,
        "name", &Command::Package::name,
        "git", &Command::Package::git,
        "git_short", &Command::Package::git_short,
        "git_prefixed", &Command::Package::git_prefixed,
        "selected_version", &Command::Package::selected_version,
        "versions", &Command::Package::versions,
        "target_link", &Command::Package::target_link,
        "description", &Command::Package::description,
        "git_description", &Command::Package::git_description,
        "language", &Command::Package::language,
        "license", &Command::Package::license,
        "owner", &Command::Package::owner,
        "owner_type", &Command::Package::owner_type,
        "stars", &Command::Package::stars,
        "forks", &Command::Package::forks,
        "open_issues", &Command::Package::open_issues,
        "watchers", &Command::Package::watchers
        ); 

    lua.new_usertype<Command::Mode>("Mode",
        "new", sol::no_constructor,
        "name", &Command::Mode::name,
        "flags", &Command::Mode::flags,
        "dependencies", &Command::Mode::dependencies
        );


    lua.new_usertype<Command::Project>("Project"
        "new", sol::no_constructor,
        "name", &Command::Project::name,
        "version", &Command::Project::version,
        "description", &Command::Project::description,
        "authors", &Command::Project::authors,
        "dependencies", &Command::Project::dependencies,
        "toolchains", &Command::Project::toolchains,
        "flags", &Command::Project::flags,
        "modes", &Command::Project::modes,
        "libs", &Command::Project::libs,
        "license", &Command::Project::license,
        "git", &Command::Project::git,
        "cmake_version", &Command::Project::cmake_version,
        "build_command", &Command::Project::build_command,
        "build_dir", &Command::Project::build_dir,
        "src_dir", &Command::Project::src_dir,
        "include_dir", &Command::Project::include_dir,
        "lang_version", &Command::Project::lang_version,
        "lang", &Command::Project::lang,
        "project_type", &Command::Project::type,
        "keywords", &Command::Project::keywords,
        "prompts", &Command::Project::prompts
          );

    lua.new_usertype<Command::ProjectPrompt>("ProjectPrompt",
        "value", &Command::ProjectPrompt::value,
        "default", &Command::ProjectPrompt::default_value,
        "getstr", &Command::ProjectPrompt::get<std::string>,
        "getint", &Command::ProjectPrompt::get<int>,
        "getbool", &Command::ProjectPrompt::get<bool>,
        "getfloat", &Command::ProjectPrompt::get<float>
        );



    return true;
  }
}
