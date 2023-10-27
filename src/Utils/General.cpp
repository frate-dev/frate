#include <filesystem>
#include <iostream>
namespace Utils{
  std::string getFolderName(){
    std::string directory_name = std::filesystem::current_path();
    directory_name = directory_name.substr(directory_name.find_last_of("/\\") + 1);
    return directory_name;
  }
}
