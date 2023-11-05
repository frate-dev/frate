#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <iostream>
namespace Utils {
  using nlohmann::json;
  std::string getFolderName();
  std::vector<std::string> split(std::string str, char delimiter);
  void toLower(std::string& str);
  json fetchJson(std::string url);
  std::string fetchText(std::string url);
  struct TableFormat {
      int width;
      char fill;
      TableFormat(): width(14), fill(' ') {}
      template<typename T>
      TableFormat& operator<<(const T& data) {
          std::cout<<  std::setw(width) << data << std::setfill(fill);
          return *this;
      }
  };
}
