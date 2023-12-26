#include <Frate/Dependency.hpp>
#include <Frate/Utils/Macros.hpp>
#include <Frate/Utils/Logging.hpp>


namespace Frate::Command {
  Dependency::Dependency():name(""),version(""),git(""),git_short(""),git_prefixed(""){
  }
  Dependency::Dependency(Package &package, std::string &version){
    this->name = package.name;
    this->version = version;
    this->git = package.git;
    this->git_short = package.git_short;
    this->git_prefixed = package.git_prefixed;
  }
  void to_json(json &json_obj, const Dependency &dep){
    TO_JSON_FIELD(dep,name);
    TO_JSON_FIELD(dep,version);
    TO_JSON_FIELD(dep,git);
    TO_JSON_FIELD(dep,git_short);
    TO_JSON_FIELD(dep,git_prefixed);
  }
  void from_json(const json &json_obj, Dependency &dep){
    FROM_JSON_FIELD(dep,name);
    FROM_JSON_FIELD(dep,version);
    FROM_JSON_FIELD(dep,git);
    FROM_JSON_FIELD(dep,git_short);
    FROM_JSON_FIELD(dep,git_prefixed);
  }
}
