#include "../Command.hpp"
#include "nlohmann/json_fwd.hpp"

namespace Command {
  using nlohmann::json;

  json Package::toJson() {
    json data;
    data["name"] = this->name;
    data["versions"] = this->versions;
    data["description"] = this->description;
    data["target_link"] = this->target_link;
    data["git"] = this->git;
    data["git_short"] = this->git_short;
    data["git_prefixed"] = this->git_prefixed;
    data["selected_version"] = this->selected_version;
    data["git_description"] = this->git_description;
    data["language"] = this->language;
    data["license"] = this->license;
    data["owner"] = this->owner;
    data["owner_type"] = this->owner_type;
    data["stars"] = this->stars;
    data["forks"] = this->forks;
    data["open_issues"] = this->open_issues;
    data["watchers"] = this->watchers;
    return data;
  }

  void Package::fromJson(json data) {
    this->name = data["name"];

    if(data["versions"].is_null()){
      this->versions = {"master"};
    }else{
      this->versions = data["versions"];
    }

    this->description = data["description"];

    if(target_link == ""){
      this->target_link = this->name;
    }else{
      this->target_link = data["target_link"];
    }


    this->git = data["git"];
    this->git_short = data["git_short"];
    this->git_prefixed = data["git_prefixed"];
    this->git_description = data["git_description"];
    this->language = data["language"];
    this->license = data["license"];
    this->owner = data["owner"];
    this->owner_type = data["owner_type"];
    this->stars = data["stars"].get<int>();
    this->forks = data["forks"].get<int>();
    this->open_issues = data["open_issues"].get<int>();
    this->watchers = data["watchers"].get<int>();

  }

} // namespace Command
