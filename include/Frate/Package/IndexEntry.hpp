

#include <string>
#include <vector>
namespace Frate::Package {
  class IndexEntry{
    private:
      std::string name;
      std::string git;
      std::string git_short;
      std::string git_prefixed;
      std::vector<std::string> versions;
      std::string target_link;
      std::string description;
      // std::string selected_version;
      std::string git_description;
      std::string language;
      std::string license;
      std::string owner;
      std::string owner_type;
      int stars;
      int forks;
      int open_issues;
      int watchers;
    public:
      IndexEntry() = default;

  };
}
