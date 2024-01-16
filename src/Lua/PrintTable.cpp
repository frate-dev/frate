#include <Frate/Lua/LuaAPI.hpp>

namespace Frate::Lua {
  void print_table(sol::table in_table, int indent = 0) {
    for (auto [key, value] : in_table) {
      if (value.is<sol::table>()) {
        print_table(value.as<sol::table>(), indent + 1);
      } else if (value.is<std::string>()) {
        for (int i = 0; i < indent; i++) {
          std::cout << "  ";
        }
        std::cout << key.as<std::string>() << " : " << value.as<std::string>()
                  << std::endl;
      } else if (value.is<int>()) {
        for (int i = 0; i < indent; i++) {
          std::cout << "  ";
        }
        std::cout << key.as<std::string>() << " : " << value.as<int>()
                  << std::endl;
      } else if (value.is<float>()) {
        for (int i = 0; i < indent; i++) {
          std::cout << "  ";
        }
        std::cout << key.as<std::string>() << " : " << value.as<float>()
                  << std::endl;
      }
    }
  }
} // namespace Frate::Lua

void Frate::Lua::FrateApi::print_table(sol::table in_table) {
  Frate::Lua::print_table(in_table);
}
