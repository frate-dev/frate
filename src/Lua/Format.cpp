#include <Frate/LuaAPI.hpp>


namespace Frate::LuaAPI {
  std::string FrateApi::format(const std::string &str, sol::variadic_args var_args) {
    std::vector<std::string> args;
    std::string result;

    for (const auto &arg : var_args) {
      if (arg.is<std::string>()) {
        args.push_back(arg.as<std::string>());
      } else if (arg.is<int>()) {
        args.push_back(std::to_string(arg.as<int>()));
      } else if (arg.is<bool>()) {
        args.push_back(arg.as<bool>() ? "true" : "false");
      } else if (arg.is<float>()) {
        args.push_back(std::to_string(arg.as<float>()));
      } else {
        args.push_back("nil");
      }
    }

    for (size_t i = 0; i < str.size(); i++) {
      if (str[i] == '{') {
        if (str[i + 1] == '{') {
          result += '{';
          i++;
        } else {
          size_t j = i + 1;
          while (j < str.size() && str[j] != '}') {
            j++;
          }
          if (j == str.size()) {
            throw std::runtime_error("Invalid format string");
          }
          std::string index = str.substr(i + 1, j - i - 1);
          if (index == "0") {
            result += args[0];
          } else {
            int idx = std::stoi(index);
            if (idx < 1 || idx > args.size()) {
              throw std::runtime_error("Invalid format string");
            }
            result += args[idx - 1];
          }
          i = j;
        }
      } else {
        result += str[i];
      }
    }

    return result;
  }
}
