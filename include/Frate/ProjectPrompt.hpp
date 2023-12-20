#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>


namespace Frate::Command {
  using nlohmann::json;
  class ProjectPrompt{
    public:
      ProjectPrompt(){};
      std::string value{""};
      std::string text{""};
      std::string type{"string"};
      std::string default_value{""};
      bool required{false};
      std::vector<std::string> options{};
      friend void from_json(const json &j, ProjectPrompt& prompt);
      friend void to_json(json &j, const ProjectPrompt& prompt);
      std::function<bool(std::string)> validator{
        [this](std::string s) -> bool {
          if(options.size() == 0) {
            return true;
          }else{
            for (std::string option: options){
              if (s == option){
                return true;
              }
            }
            return false;
          }
        }
      };
      template<typename T>
        T get();
  };
}
