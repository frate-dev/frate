#include <Frate/Generators.hpp>
#include <Frate/Lua/LuaAPI.hpp>
#include <inja.hpp>

namespace Frate::Generators::Project {
  using inja::Environment;

  bool initializeLua(Environment &env, sol::state &lua,
                     std::shared_ptr<Project::Config> pro) {
    Lua::registerAPI(lua);
    Utils::info << "Registering project" << std::endl;
    if (!Lua::registerProject(lua, pro)) {
      Utils::error << "Error while registering project" << std::endl;
      return false;
    }
    Utils::info << "Registering project scripts at: "
                << pro->template_path / "scripts" << std::endl;
    if (!Lua::registerProjectScripts(env, lua,
                                        pro->template_path / "scripts", pro)) {
      Utils::error << "Error while registering project scripts" << std::endl;
      return false;
    }
    return true;
  }
} // namespace Frate::Generators::Project
