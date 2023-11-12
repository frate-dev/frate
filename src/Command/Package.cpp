#include "Command.hpp"
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
  }

} // namespace Command
