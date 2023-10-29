#include <filesystem>
#include <cpr/cpr.h>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
namespace Utils{
  using nlohmann::json;
  std::string getFolderName(){
    std::string directory_name = std::filesystem::current_path();
    directory_name = directory_name.substr(directory_name.find_last_of("/\\") + 1);
    return directory_name;
  }
  std::vector<std::string> split(std::string str, char delimiter){
    std::vector<std::string> result;

    std::string word = "";

    for (char x : str) {
      if (x == delimiter) {
        result.push_back(word);
        word = "";
      } else {
        word = word + x;
      }
    }
    if(result.size() == 0){
      result.push_back(word);
    }
    return result;
  }
  //"https://github.com/cmaker-dev/index/releases/latest/download/index.json"
  std::string fetchText(std::string url) {
    std::cout << "Downloading index.json" << std::endl;
    cpr::Response r = cpr::Get(cpr::Url{url});
    std::cout << r.status_code << std::endl;
    
    switch(r.status_code){
      case 200:
        std::cout << "Downloaded index.json" << std::endl;
        break;
      case 404:
        std::cout << "Failed to download index.json" << std::endl;
        std::cout << "Error: " << r.error.message << std::endl;
        exit(-1);
        break;
      default:
        std::cout << "Failed to download index.json" << std::endl;
        std::cout << "Error: " << r.error.message << std::endl;
        exit(-1);
        break;
    }
    return r.text;
  }
  json fetchJson(std::string url) {
    std::cout << "Downloading index.json" << std::endl;
    cpr::Response r = cpr::Get(cpr::Url{url});
    std::cout << r.status_code << std::endl;
    
    switch(r.status_code){
      case 200:
        std::cout << "Downloaded index.json" << std::endl;
        break;
      case 404:
        std::cout << "Failed to download index.json" << std::endl;
        std::cout << "Error: " << r.error.message << std::endl;
        exit(-1);
        break;
      default:
        std::cout << "Failed to download index.json" << std::endl;
        std::cout << "Error: " << r.error.message << std::endl;
        exit(-1);
        break;
    }
    try {
      return json::parse(r.text);
    } catch (json::parse_error& e) {
      std::cout << "Failed to parse index.json" << std::endl;
      std::cout << "Error: " << e.what() << std::endl;
      std::cout << "At: " << e.byte << std::endl;
      std::cout << "Text: " <<r.text << std::endl;
      exit(-1);
    }
  }
}
