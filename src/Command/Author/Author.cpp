#include "Frate/Utils/General.hpp"
#include <Frate/Command/Author.hpp>
#include <Frate/Frate.hpp>


namespace Frate::Command::Author {

  bool options(std::shared_ptr<Interface> inter){
    inter->InitHeader();
    inter->options->parse_positional({"command","subcommand","authors"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>())
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())
      ("authors", "Authors", cxxopts::value<std::vector<std::string>>());
    return inter->parse();
  }
  bool add(std::shared_ptr<Interface> inter){
    options(inter);
    std::vector<std::string> authors;
    // Utils::Info info;
    // Utils::Error error;
    if(inter->args->count("authors") == 0){
      info << "No authors specified" << std::endl;
      return false;
    }
    authors = inter->args->operator[]("authors").as<std::vector<std::string>>();
    for (std::string author : authors) {
      for(auto current_author : inter->pro->authors){
        if(current_author == author){
          Frate::error << "Author " << author << " already exists" << std::endl;
          return false;
        }
      }
      inter->pro->authors.push_back(author);
      Frate::info << "Added author " << author << std::endl;
    }
    inter->pro->save();
    return true;
  }
  bool remove(std::shared_ptr<Interface> inter){
    options(inter);
    std::vector<std::string> authors;
    if(inter->args->count("authors") == 0){
      info << "No authors specified" << std::endl;
      return false;
    }
    authors = inter->args->operator[]("authors").as<std::vector<std::string>>();
    for (std::string author : authors) {
      for(auto current_author : inter->pro->authors){
        if(current_author == author){
          inter->pro->authors.erase(
              std::remove(
                inter->pro->authors.begin(), inter->pro->authors.end(), author
                ), inter->pro->authors.end()
              );
          info << "Removed author " << author << std::endl;
          inter->pro->save();
          return true;
        }
      }
      Frate::error << "Author " << author << " does not exist" << std::endl;
      return false;
    }
    return true;

  }
  bool list(std::shared_ptr<Interface> inter){
    for (auto author : inter->pro->authors) {
      std::cout << author << std::endl;
    }
    return true;
  }
}
