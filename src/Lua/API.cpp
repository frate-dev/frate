#include "Frate/Command.hpp"
#include <Frate/LuaAPI.hpp>
#include <format>


namespace Frate::LuaAPI {
  void registerAPI(sol::state &lua) {
    lua.set_function("format", [](const std::string &str, sol::table obj) {
      std::string result = str;
      for (const auto &[key, value] : obj) {
        result = std::format(result, value.as<std::string>());
      }
      return result;
    });
    lua.new_usertype<Command::ProjectPrompt>("ProjectPrompt",
        "value", &Command::ProjectPrompt::value,
        "default", &Command::ProjectPrompt::default_value,
        "getstr", &Command::ProjectPrompt::get<std::string>,
        "getint", &Command::ProjectPrompt::get<int>,
        "getbool", &Command::ProjectPrompt::get<bool>,
        "getfloat", &Command::ProjectPrompt::get<float>
    );
  }
}
