#include <Frate/Package.hpp>

namespace Frate::Command {
  void to_json(json &j, const Package &p) {
    j = json{
      {"name", p.name},
      {"selected_version,", p.selected_version},
      {"versions", p.versions},
      {"stars", p.stars},
      {"forks", p.forks},
      {"open_issues", p.open_issues},
      {"watchers", p.watchers},
      {"score", p.score},
      {"git", p.git},
      {"git_short", p.git_short},
      {"git_prefixed", p.git_prefixed},
      {"git_description", p.git_description},
      {"target_link", p.target_link},
      {"description", p.description},
      {"language", p.language},
      {"license", p.license},
      {"owner", p.owner},
      {"owner_type", p.owner_type},
    };
  }
  void from_json(const json &j, Package &p){
    j.at("name").get_to(p.name);
    if(j.contains("selected_version")){
      j.at("selected_version").get_to(p.selected_version);
    }
  
    if(j.contains("versions")){
      if(!j.at("versions").is_null()){
        j.at("versions").get_to(p.versions);
      }
    }
    j.at("stars").get_to(p.stars);
    j.at("forks").get_to(p.forks);
    j.at("open_issues").get_to(p.open_issues);
    j.at("watchers").get_to(p.watchers);
    if(j.contains("score")){
      j.at("score").get_to(p.score);
    }
    j.at("git").get_to(p.git);
    j.at("git_short").get_to(p.git_short);
    j.at("git_prefixed").get_to(p.git_prefixed);
    j.at("git_description").get_to(p.git_description);
    j.at("target_link").get_to(p.target_link);
    j.at("description").get_to(p.description);
    j.at("language").get_to(p.language);
    j.at("license").get_to(p.license);
    j.at("owner").get_to(p.owner);
    j.at("owner_type").get_to(p.owner_type);
  }
}
