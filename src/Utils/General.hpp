#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace Utils {
  using nlohmann::json;
  std::string getFolderName();
  std::vector<std::string> split(std::string str, char delimiter);
  void toLower(std::string& str);
  json fetchJson(std::string url);
  std::string fetchText(std::string url);
}
