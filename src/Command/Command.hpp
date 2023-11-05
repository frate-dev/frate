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


#define ENDL "\n"

namespace Command {
  using nlohmann::json;
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
      std::cout << "file config.json exists" << std::endl;
      std::cout << "do you want to overwrite it?[y/n]:";
      std::string input;
      std::getline(std::cin, input);
      if (input != "y"){
        exit(1);
      }
      file.close();
      return true;
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

  typedef struct Package_s {
    std::string name;
    std::string url;
    std::vector<std::string> versions;
    std::string target_link;
    std::string description;
    int score;
    json toJson();
    void fromJson(json j);
  } Package;

  typedef struct Dependency_s {
    std::string name;
    std::string url;
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

  typedef struct Project_s {
    std::string project_name;
    std::string project_type;
    std::string project_description;
    BuildServer  build_server;
    std::filesystem::path project_path;
    std::string git{"null"};
    std::string lang{"cpp"};
    std::string cmake_version{"3.10"};
    std::string lang_version{"20"};
    std::string compiler{"g++"};
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
      bool addFlag();
      bool server();
      bool setBuildServer(std::vector<BuildServer> servers);
      bool getBuildServer();
      bool addAuthors();
      bool addDependency();
      bool ftp();
      bool watch();
      bool clean();
    public:
      std::shared_ptr<Project> pro;
      bool parse();
      std::shared_ptr<cxxopts::Options> options;
      std::shared_ptr<cxxopts::ParseResult> args;
      char** argv;
      int argc;
      Interface(int argc, char **argv);
      ~Interface();
      bool InitHeader();
      bool CreateCMakelists();
      bool LoadPackageJson();
  };
  namespace OptionsInit{
      bool Init(Interface*);
      bool Add(Interface*);
      bool Remove(Interface*);
      bool Server(Interface*);
      bool Update(Interface*);
      bool Main(Interface*);
      bool Watch(Interface*);
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
