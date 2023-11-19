#include <Frate/Command/Author.hpp>


namespace Command::Author {


  bool add(Interface *inter){
    if (inter->pro->args->count("args") == 0) {
      for (auto author : inter->pro->args->operator[]("args").as<std::vector<std::string>>()) {
        inter->pro->authors.push_back(author);
      }
    }
    return true;
  }

}
