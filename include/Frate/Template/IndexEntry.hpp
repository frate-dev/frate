#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace Frate::Project {

  class TemplateIndexEntry {
  private:
    std::string name;
    std::string description;
    std::string head;
    std::unordered_map<std::string, std::string> branches;
    std::vector<std::string> branch_names;
    std::string git;

  public:
    friend void from_json(const nlohmann::json &j, TemplateIndexEntry &t);
    friend void to_json(nlohmann::json &j, const TemplateIndexEntry &t);

    std::string &getName() { return name; }

    std::string &getDescription() { return description; }

    std::string &getHead() { return head; }

    std::string &getGit() { return git; }

    std::string &getBranchHash(const std::string &branch) {
      return branches[branch];
    }

    std::vector<std::string>& getBranches() {
      if(!branch_names.empty()) {
        return branch_names;
      }
      for (auto &branch : branches) {
        branch_names.push_back(branch.first);
      }
      return branch_names;
    }
  };
} // namespace Frate::Project
