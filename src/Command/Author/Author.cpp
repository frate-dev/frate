#include <Frate/Command/Author.hpp>


namespace Command::Author {

  bool options(Interface *inter){
    inter->InitHeader();
    inter->options->parse_positional({"command","subcommand","args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>())
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())
      ("args", "Authors", cxxopts::value<std::vector<std::string>>());
    return true;
  }
  bool add(Interface *inter){
    options(inter);
    if (inter->pro->args->count("args") > 0) {
      for (auto author : inter->pro->args->operator[]("args").as<std::vector<std::string>>()) {
        inter->pro->authors.push_back(author);
      }
    }
    return true;
  }
  bool list(Interface *inter){
    for (auto author : inter->pro->authors) {
      std::cout << author << std::endl;
    }
    return true;
  }
}
