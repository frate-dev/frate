#include <memory>
#include <vector>
#include "./Command.hpp"

namespace Generators{
  namespace GitIgnore{
    bool create(std::shared_ptr<Command::Project> pro);
  }
  namespace Readme{
    bool create(std::shared_ptr<Command::Project> pro);
  }
  namespace Toolchain{
    std::string generateToolchain(std::string toolchain);
  }
  namespace CMakeList{
    typedef struct Dep {
      std::string fetch_declare;
      std::string fetch_make_available;
      std::string target_link_libraries;
    } Dep;
    typedef struct CMakeContext {
      std::string cmake_minimum_required;
      std::string project_name;
      std::string project_version;
      std::string build_type;
      std::string cxx_version;
      std::string compiler;
      std::string source_dir;
      std::string build_dir;
      std::string fetch_content;
      std::string include_fetch;
      std::string files;
      std::string include_dir;
      std::string add_executable;
      std::string testing;
      std::string mode;
      std::string set_build_dir;
      std::vector<Dep> dependencies;
    } CMakeContext;
    /*
     * Generate the dependencies for the project
     * @param ctx: the context of the command
     * @return a vector of dependencies that will be later combined to build the cmake file
     */

    bool create(std::shared_ptr<Command::Project> pro); 
    bool createCMakeLists(std::shared_ptr<Command::Project> pro);
  }

  namespace ConfigJson{

    typedef struct Config_s{
      std::string cmake_version;
      std::string project_name;
      std::string project_version;
      std::string include_dir;
      std::string compiler;
      std::string name;
      std::string src_dir;
      std::string build_dir;
      std::string lang;
      std::string lang_version;
      std::string authors_str;
    } Config;
    [[deprecated("Use project->writeConfig() instead")]]
    bool writeConfig(std::shared_ptr<Command::Project>& ctx);
  }

}



