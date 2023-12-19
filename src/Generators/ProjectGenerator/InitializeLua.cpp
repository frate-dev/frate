#include <Frate/Generators.hpp>
#include <inja.hpp>
#include <Frate/LuaAPI.hpp>


namespace Frate::Generators::Project {
  using inja::Environment;
  bool initializeLua(Environment &env, sol::state &lua, std::shared_ptr<Command::Project> pro){
    LuaAPI::registerAPI(lua);
    Utils::info << "Registering project" << std::endl;
    if(!LuaAPI::registerProject(lua, pro)){
      Utils::error << "Error while registering project" << std::endl;
      return false;
    }
    Utils::info << "Registering project scripts at: " << pro->path / "template/scripts" << std::endl;
    if(!LuaAPI::registerProjectScripts(env,lua, pro->path / "template/scripts", pro)){
      Utils::error << "Error while registering project scripts" << std::endl;
      return false;
    }
    return true;
  }
}
