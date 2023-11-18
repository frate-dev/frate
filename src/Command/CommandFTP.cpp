#include <iostream>
#include <CMaker/Command.hpp>
#include <CMaker/Utils/CLI.hpp>
#include <CMaker/Utils/General.hpp>

namespace Command{
  using std::filesystem::directory_iterator;
  using Utils::CLI::Prompt;
  using namespace Utils::CLI::Ansi;
  bool Interface::ftp() {

    Prompt<bool> *prompt = new Prompt<bool>(
        "Are you sure you would like to delete the entire project?");
    prompt->Color(RED)->Run();
    if (!prompt->Get()) {
      std::cout << "Aborting..." << std::endl;
      return false;
    }
    for (auto &p : directory_iterator(pro->project_path)) {
      std::filesystem::path path = p.path();
      try{
        if(std::filesystem::is_directory(path)){
          std::filesystem::remove_all(path);
          std::cout << "Deleting: " << path << std::endl;
        }else{
          if(path.filename() != "cmaker"){
            std::filesystem::remove(path);
            std::cout << "Deleting: " << path << std::endl;
          }
        }
      }catch(std::exception &e){
        std::cout << "Failed to delete: " << path << std::endl;
        return false;
      }
    }
    return true;
  }
}
