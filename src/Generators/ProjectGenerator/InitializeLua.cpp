#include <Frate/Generators.hpp>
#include <inja.hpp>
#include <Frate/LuaAPI.hpp>


namespace Frate::Generators::Project {
  using inja::Environment;
  bool initializeLua(Environment &env, sol::state &lua, std::shared_ptr<Command::Project> pro){
    LuaAPI::registerAPI(lua);
    if(!LuaAPI::registerProject(lua, pro)){
      error << "Error while registering project" << std::endl;
      return false;
    }
    if(!LuaAPI::registerProjectScripts(env,lua, pro->path / "template/scripts", pro)){
      error << "Error while registering project scripts" << std::endl;
      return false;
    }
    return true;
  }
}
