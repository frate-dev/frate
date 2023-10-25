#include <memory>
#include <vector>
#include <format>
#include "../Command/Command.hpp"

namespace Generators::ConfigToml{

  typedef struct ConfigToml{
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
  } ConfigToml;

  bool readData(std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);
  bool writeConfig(std::shared_ptr<Command::Context> &ctx);

  /*
   * Validate cmakeVersion 
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the cmake version is valid
   */
  bool cmakeVersion(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);

  /*
   * Validates the project name
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the project name is valid
   */
  bool projectName(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);

  /*
   * Validates the project version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the project version is valid
   */
  bool projectVersion(std::string prefix,  std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);
  
  /*
   * Validates the language version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the language version is valid
   */
  bool languageVersion(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);

  /*
   * Validates the compiler 
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the language is valid
   */
  bool compiler(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);

  /*
   * Validates the source directory
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the source directory is valid
   */
  bool sourceDir(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);

  /*
   * Validates the build directory
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the build directory is valid
   */
  bool buildDir(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);

  /*
   * Reads the data from the user
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the data is valid
   */
  bool includeDir(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml);

}



namespace Generators::CMakeList{
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
  void generateDeps(std::shared_ptr<Command::Context> ctx, std::shared_ptr<CMakeContext> cmake_context);
  
  bool create(std::shared_ptr<Command::Context> ctx); 
}
