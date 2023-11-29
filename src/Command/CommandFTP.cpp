#include <iostream>
#include <Frate/Command.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/General.hpp>

namespace Command{
  using namespace std::filesystem;
  using Utils::CLI::Prompt;
  using namespace Utils::CLI::Ansi;
  bool Interface::ftp() {

    Prompt *prompt = new Prompt(
        "Are you sure you would like to delete the entire project?");
    prompt->Color(RED)->Run();
    auto [valid, value] = prompt->Get<bool>();
    if (!valid || !value) {
      std::cout << "Aborting..." << std::endl;
      return false;
    }
    for (const directory_entry &p : directory_iterator(pro->project_path)  
          | std::views::filter(
            [](const directory_entry &p) { return p.path().filename() != "frate"; })
        ){
      std::filesystem::path path = p.path();
      try{
        if(std::filesystem::is_directory(path)){
          std::filesystem::remove_all(path);
          std::cout << "Deleting: " << path << std::endl;
        }else{
          std::filesystem::remove(path);
          std::cout << "Deleting: " << path << std::endl;
        }
      }catch(std::exception &e){
        std::cout << "Failed to delete: " << path << std::endl;
        Utils::debug(e.what());
        return false;
      }
    }
    return true;
  }
}
