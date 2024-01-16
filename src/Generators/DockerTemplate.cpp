#include <Frate/Command.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Lua/LuaAPI.hpp>
#include <inja.hpp>
#include <sol/sol.hpp>

/*
 * For generating a dockerfile
 * @param inter: the interface object
 * @return true if the generation was successful
 *
 * thoughts:
 * - should we use a template for this?
 * - how can we inject in env variables to
 *   enable cross platform builds?
 * - what about env variables for packaging
 *   and deployment of the lib/executable?
 * - where should we store the lua scripts for
 *   setting up the dockerfile?
 */

namespace Frate::Generators::DockerTemplate {
  bool create(std::shared_ptr<Command::Interface> inter) {
    inja::Environment env;
    sol::state lua;
    Lua::registerAPI(lua);

    if (!Lua::registerProject(lua, inter->pro)) {
      Utils::debug("Error while registering project");
      return false;
    }

    if (!Lua::registerProjectScripts(
            env, lua, inter->pro->path / "templates/scripts", inter->pro)) {
      Utils::debug("Error while registering project scripts");
      return false;
    }
    return true;
  }

  bool remove(std::shared_ptr<Command::Interface> inter) {
    (void)inter;
    return true;
  }
} // namespace Frate::Generators::DockerTemplate
