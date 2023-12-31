#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLI.hpp>

namespace Frate::Command::Packages {
  using namespace Utils::CLI;

 Package promptSearchResults(std::string &query){

   std::vector<Package> searchResults = _search(query);

   if(searchResults.size() == 1){
     std::cout << "Installing " << searchResults[0].name << std::endl;
     return searchResults[0];
   }

   if(searchResults.size() == 0){
     std::cout << "No results found" << std::endl;
     exit(0);
   }

   List packageList{};
    packageList.
     Numbered().
     ReverseIndexed();
   for(Package result: searchResults){
     packageList.pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
   }
   std::cout << packageList.Build() << std::endl;
   Prompt prompt("Select a package to install: ");
   for(size_t i = 0; i < searchResults.size(); i++){
     prompt.addOption(i);
   }
   prompt.run();
   auto [valid,index] = prompt.get<int>();

   if(!valid){
     std::cout << "Invalid option" << std::endl;
     exit(0);
   }else{
     return searchResults[index];
   }


 }

  std::string promptForVersion(Package &chosen_package){

    List list{}; 
    list.Numbered().ReverseIndexed();
    for(size_t i = 0; i < chosen_package.versions.size(); i++){
      list.pushBack(ListItem(chosen_package.versions[i]));
    }

    std::cout << list.Build() << std::endl;

    Prompt prompt("Select a version to install: ");

    for(size_t i = 0; i < chosen_package.versions.size(); i++){
      prompt.addOption(i);
    }

    prompt.run();

    auto [valid, index] = prompt.get<int>();

    if(!valid){
      std::cout << "Invalid option" << std::endl;
      exit(0);
    }else{
      return chosen_package.versions[index];
    }
  }
}
