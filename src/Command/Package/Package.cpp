#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp> 
#include <Frate/Command/CommandMode.hpp>

namespace Frate::Command::Packages {
  using namespace Utils::CLI;
  
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("m,mode", "make changes to compile mode", cxxopts::value<std::string>())
      //TODO: make git version, and stuff like that work to make packages more relaxed
      ("g,git", "git repo to clone", cxxopts::value<std::string>())
      ("v,version", "version for package", cxxopts::value<std::string>())
      ("t,target_link", "how to link to package", cxxopts::value<std::string>())
      ("e,exact", "Exact package", cxxopts::value<bool>()->default_value("false"))
      ("l,latest", "Latest package", cxxopts::value<bool>()->default_value("false"))
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    inter->options->help();
    return inter->parse();
  }
  bool dependenciesConflict(std::vector<Package> deps, std::string &name) {
    if (deps.size() == 0) {
      Utils::error << "No dependencies found" << std::endl;
      return false;
    }
    for (Package dep : deps) {
      if (dep.name == name) {
        return true;
      }
    }

    return false;
  }

  
  std::pair<bool, Package> getExact(std::string query){
    json index = fetchIndex();
    for(Package pi: index){
      if(pi.name == query){
        return {true, pi};
      }
    }
    return {false, {}};
  }

}


namespace Frate::Command {
  using nlohmann::json;

//   json Package::toJson() {
//     json data;
//     data["name"] = this->name;
//     data["versions"] = this->versions;
//     data["description"] = this->description;
//     data["target_link"] = this->target_link;
//     data["git"] = this->git;
//     data["git_short"] = this->git_short;
//     data["git_prefixed"] = this->git_prefixed;
//     data["selected_version"] = this->selected_version; 
//     data["git_description"] = this->git_description;
//     data["language"] = this->language;
//     data["license"] = this->license;
//     data["owner"] = this->owner;
//     data["owner_type"] = this->owner_type;
//     data["stars"] = this->stars;
//     data["forks"] = this->forks;
//     data["open_issues"] = this->open_issues;
//     data["watchers"] = this->watchers;
//     return data;
//   }
// 
//   void Package::fromJson(json data) {
//     this->name = data["name"];
// 
//     if(data["versions"].is_null()){
//       this->versions = {"master"};
//     }else{
//       this->versions = data["versions"];
//     }
// 
//     this->description = data["description"];
// 
//     if(target_link == ""){
//       this->target_link = this->name;
//     }else{
//       this->target_link = data["target_link"];
//     }
// 
//     this->git = data["git"];
//     this->git_short = data["git_short"];
//     this->git_prefixed = data["git_prefixed"];
//     this->git_description = data["git_description"];
//     this->language = data["language"];
//     this->license = data["license"];
//     this->owner = data["owner"];
//     this->owner_type = data["owner_type"];
//     this->stars = data["stars"].get<int>();
//     this->forks = data["forks"].get<int>();
//     this->open_issues = data["open_issues"].get<int>();
//     this->watchers = data["watchers"].get<int>();
// 
//   }

}
