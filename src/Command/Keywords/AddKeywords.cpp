#include <Frate/Command/Keywords.hpp>
#include <Frate/Project/Config.hpp>

namespace Frate::Command::Keywords {
  bool add(std::shared_ptr<Interface> inter) {
    options(inter);
    if (inter->args->count("keywords") < 1) {
      std::cout << "No keyword specified" << std::endl;
      return false;
    }
    std::vector<std::string> keywords =
        inter->args->operator[]("keywords").as<std::vector<std::string>>();
    for (auto &keyword : keywords) {
      for (std::string &current_keyword : inter->pro->keywords) {
        if (current_keyword == keyword) {
          Utils::error << "Keyword already exists" << std::endl;
          return false;
        }
      }
      inter->pro->keywords.emplace_back(keyword);
    }
    return true;
  }
} // namespace Frate::Command::Keywords
