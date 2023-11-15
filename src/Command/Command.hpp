#pragma once
#include "nlohmann/json_fwd.hpp"
#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cxxopts.hpp>
#include "../Utils/CLI.hpp"


#define ENDL "\n"

namespace Command {
  using nlohmann::json;
  using Utils::CLI::Prompt;
  using namespace Utils::CLI::Ansi;
  [[deprecated("I DON'T KNOW WHAT THIS FUCKING DOES")]]
  bool handleCppProject();
  [[deprecated("WE ARE A BIG PROJECT MUST HAVE DEPRECATED STUFF")]]
  bool handleCProject();

  /*
   * Checks if the file exists
   * @param name the name of the file
   * @return bool -> file exists
   */
  inline bool file_exists(const std::string& name) {
    std::ifstream file;
    try{
      file.open(name);
    }catch(std::exception &e){
      std::cout << "failed to open file: " << name << ENDL;
      exit(0);
    }
    // opening the file
    if (file) {
      
      Prompt<bool> *prompt = new Prompt<bool>("file config.json already exists\n Are you sure you would like to overwrite it?");
      prompt->Color(RED)->ExitOnFailure()->Run();
      if(prompt->Get()){
        file.close();
        return true;
      }else{
        exit(1);
      }
    }
    return false;
  }
  /*
   * Initializes the options for the command line parser
   * @param new options object
   */
  using namespace cxxopts;
  ParseResult mainOptions(int argc, char** argv);
  ParseResult initOptions(int argc, char** argv);
  ParseResult addOptions(int argc, char** argv);
  ParseResult removeOptions(int argc, char** argv);
  ParseResult updateOptions(int argc, char** argv);
  /*
   * Package structure from index
   * @param name the name of the package
   * @param url the url of the package
   * @param versions the versions of the package
   * @param target_link the target link of the package
   * @param description the description of the package
   * @param score the score of the package, used for sorting
   */
  typedef struct Package_s {
    std::string name;
    std::string git;
    std::string git_short;
    std::string git_prefixed;
    std::vector<std::string> versions;
    std::string target_link;
    std::string description;
    std::string selected_version;
    std::string git_description;
    std::string language;
    std::string license;
    std::string owner;
    std::string owner_type;
    int stars;
    int forks;
    int open_issues;
    int watchers;
    int score;
    json toJson();
    void fromJson(json j);
  } Package;//Deez nuts

  typedef struct Dependency_s {
    std::string name;
    std::string git;
    std::string version;
    std::string target_link;
  } Dependency;

  typedef struct BuildServer_s {
    std::string name;
    std::string ip;
    std::string username;
    std::string authMethod;
    std::optional<std::string> password;
    std::optional<std::string> key;
    int port;
  } BuildServer;

  namespace ProjectType {
    const std::string EXECUTABLE = "executable";
    const std::string HEADER_ONLY = "header_only";
    const std::string STATIC_LIBRARY = "static_library";
    const std::string SHARED_LIBRARY = "shared_library";
    constexpr bool validate(std::string type) {
      return type == EXECUTABLE || type == HEADER_ONLY || type == STATIC_LIBRARY || type == SHARED_LIBRARY;
    }
  };
  typedef struct Mode{
    std::string name;
    std::vector<std::string> flags;
    std::vector<Dependency> dependencies{};
  } Mode;
  typedef struct Project_s {
    std::string project_name;
    std::string project_description;
    std::string project_type = ProjectType::EXECUTABLE;
    BuildServer build_server;
    /*
     * This is the project path, it will be set to the current working directory and in debug mode if willl set the path to ./build/
     */
    std::filesystem::path project_path;
    std::string git{"null"};
    std::string lang{"cpp"};
    std::string cmake_version{"3.10"};
    std::string lang_version{"20"};
    std::string compiler{"g++"};
    std::vector<Mode> modes{
      Mode{.name = "Release", .flags={"-O2 "}}, 
      Mode{.name= "Debug", .flags= {"-g"}},
      Mode{.name= "Test", .flags={"-g"}}
    };
    std::vector<std::string> authors;
    std::string src_dir{"src"};
    std::string include_dir{"include"};
    std::vector<Dependency> dependencies;
    std::vector<BuildServer> build_servers;
    std::string build_dir{"build"};
    Dependency testing_lib;
    std::string project_version{"0.0.1"};
    std::vector<std::string> flags; 
    std::shared_ptr<cxxopts::ParseResult> args;
    std::vector<std::string> toolchains;
    void fromJson(json j);
    nlohmann::json toJson();
  } Project;
  class Interface{
    private:
      //Commands;
      bool init();
      bool add();
      bool remove();
      bool removeDep();
      bool update();
      bool run();
      bool help();
      bool search();
      bool addFlag();
      bool server();
      bool setBuildServer(std::vector<BuildServer> servers);
      bool getBuildServer();
      bool addAuthors();
      bool addDependency();
      bool ftp();
      bool modes();
      bool mode();
      bool watch();
      bool toolchains();
      bool clean();
      //TODO: setup register comamnd
      bool registerCommand(std::string name, std::vector<std::string> subcommands, std::function<bool()> func);
    public:
      std::shared_ptr<Project> pro;
      bool skip_prompts{false};
      bool parse();
      std::shared_ptr<cxxopts::Options> options;
      std::shared_ptr<cxxopts::ParseResult> args;
      char** argv;
      int argc;
      bool confirm_all = false;
      Interface(int argc, char **argv);
      ~Interface();
      bool InitHeader();
      bool CreateCMakelists();
      bool LoadPackageJson();
  };
  namespace OptionsInit{
      bool Init(Interface*);
      bool Search(Interface*);
      bool Add(Interface*);
      bool Remove(Interface*);
      bool Server(Interface*);
      bool Dependencies(Interface*);
      bool Update(Interface*);
      bool Main(Interface*);
      bool Modes(Interface*);
      bool Mode(Interface*);
      bool Watch(Interface*);
      bool Clean(Interface*);
      bool Toolchains(Interface*);
  };


  /*
   * Loads the config.json file into the project context
   * @param a blank project context
   * @return bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool loadPackageJson(std::shared_ptr<Project> pro);
  /*
   * Initializes the project
   * Prompts the user for information about the project
   * @param ctx the current project context
   * @return bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool init(std::shared_ptr<Project>, cxxopts::ParseResult &args);
  /*
   * Builds and runs the project
   * @param ctx the current project context
   * @return bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool run(std::shared_ptr<Project>);
  /*
   * Fuck this project function
   * Deletes the project entirely
   * @param ctx the current project context
   * @return bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool ftp(std::shared_ptr<Project>);
  /*
   * Adds flags to build script
   * @param ctx project context
   * @return bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool addFlag(std::shared_ptr<Project>, cxxopts::ParseResult &args);
  /*
   * prints all commands and their function and parameters
   * @return bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool help();
  /*
   * adds a dependency to the config.json
   * @returns bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool addDependency(std::shared_ptr<Project> pro, cxxopts::ParseResult &args);
  [[deprecated("Old function, Command::Interface ")]]
  bool addAuthor(std::shared_ptr<Project>ctx, cxxopts::ParseResult &args);
  /*
   * Generates cmake file for project based on the current project context
   * @returns bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool createCMakelists(std::shared_ptr<Project>);
  /*
   * What the fuck lucas
   */

  std::vector<Package> searchPackage(std::string query);

  std::pair<bool,Package> getExactPackage(std::string query);

  Package getDependency(std::string query, std::vector<Dependency> deps);

  std::string downloadIndex();
  /*
   *  adds  dependency to json and regenerates the CMakeLists.txt
   *  @returns bool -> finished successfully
   */
  [[deprecated("Old function, Command::Interface ")]]
  bool add(std::shared_ptr<Project> pro, cxxopts::ParseResult &args);

  json fetchIndex();
  void updateIndex();

  [[deprecated("Old function, Command::Interface ")]]
  bool update(std::shared_ptr<Project> pro, cxxopts::ParseResult &args);

  [[deprecated("Old function, Command::Interface ")]]
  bool addLib(std::shared_ptr<Project>, cxxopts::ParseResult &args);
  [[deprecated("Old function, Command::Interface ")]]
  bool update(std::shared_ptr<Project>, cxxopts::ParseResult &args);

  [[deprecated("Old function, Command::Interface ")]]
  bool dev(std::shared_ptr<Project>);


  [[deprecated("Old function, Command::Interface ")]]
  bool remove(std::shared_ptr<Project>,  cxxopts::ParseResult &args);
  [[deprecated("Old function, Command::Interface ")]]
  bool removeDep(std::shared_ptr<Project>, cxxopts::ParseResult &args);



}
