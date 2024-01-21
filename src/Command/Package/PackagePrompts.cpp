#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLIList.hpp>
#include <Frate/Utils/CLIPrompt.hpp>

namespace Frate::Command::Packages {
  using namespace Utils::CLI;

  Package promptSearchResults(std::string &query) {

    auto searchResults = _search(query);

    if (searchResults.size() == 1) {
      std::cout << "Installing " << searchResults[0].first.name << std::endl;
      return searchResults[0].first;
    }

    if (searchResults.size() == 0) {
      std::cout << "No results found" << std::endl;
      exit(0);
    }

    List packageList{};
    packageList.Numbered().ReverseIndexed();
    for (auto &result : searchResults) {
      packageList.pushBack(
          ListItem(result.first.name + " (" + result.first.git + ")",
                   result.first.description));
    }
    std::cout << packageList.Build() << std::endl;
    Prompt prompt("Select a package to install: ");
    for (size_t i = 0; i < searchResults.size(); i++) {
      prompt.addOption(i);
    }
    prompt.run();
    auto index = prompt.get<int>();

    return searchResults[index].first;
  }

  std::string promptForVersion(Package &chosen_package) {

    List list{};
    list.Numbered().ReverseIndexed();
    for (size_t i = 0; i < chosen_package.versions.size(); i++) {
      list.pushBack(ListItem(chosen_package.versions[i]));
    }

    std::cout << list.Build() << std::endl;

    Prompt prompt("Select a version to install: ");

    for (size_t i = 0; i < chosen_package.versions.size(); i++) {
      prompt.addOption(i);
    }

    prompt.run();
    auto index = prompt.get<int>();
    return chosen_package.versions[index];
  }
} // namespace Frate::Command::Packages
