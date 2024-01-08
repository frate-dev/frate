#include <Frate/Command/Author.hpp>
#include <Frate/Frate.hpp>
#include <Frate/Project/Config.hpp>

namespace Frate::Command::Author {

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "authors"});
    // clang-format off
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>())
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())
      ("authors", "Authors", cxxopts::value<std::vector<std::string>>());
    // clang-format on
    return inter->parse();
  }

  bool add(std::shared_ptr<Interface> inter) {
    options(inter);
    std::vector<std::string> authors;
    // Utils::Info info;
    // Utils::Error error;
    if (inter->args->count("authors") == 0) {
      Utils::info << "No authors specified" << std::endl;
      return false;
    }
    authors = inter->args->operator[]("authors").as<std::vector<std::string>>();
    for (std::string author : authors) {
      for (auto current_author : inter->pro->authors) {
        if (current_author == author) {
          Utils::error << "Author " << author << " already exists" << std::endl;
          return false;
        }
      }
      inter->pro->authors.emplace_back(author);
      Utils::info << "Added author " << author << std::endl;
    }
    return true;
  }

  bool remove(std::shared_ptr<Interface> inter) {
    options(inter);
    std::vector<std::string> authors;
    if (inter->args->count("authors") == 0) {
      Utils::info << "No authors specified" << std::endl;
      return false;
    }
    authors = inter->args->operator[]("authors").as<std::vector<std::string>>();
    for (std::string author : authors) {
      for (auto current_author : inter->pro->authors) {
        if (current_author == author) {
          inter->pro->authors.erase(std::remove(inter->pro->authors.begin(),
                                                inter->pro->authors.end(),
                                                author),
                                    inter->pro->authors.end());
          Utils::info << "Removed author " << author << std::endl;
          return true;
        }
      }
      Utils::error << "Author " << author << " does not exist" << std::endl;
      return false;
    }
    return true;
  }

  bool list(std::shared_ptr<Interface> inter) {
    for (auto author : inter->pro->authors) {
      std::cout << author << std::endl;
    }
    return true;
  }
} // namespace Frate::Command::Author
