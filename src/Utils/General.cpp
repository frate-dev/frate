#include <filesystem>
#include <iostream>
#include <vector>
namespace Utils{
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
}
