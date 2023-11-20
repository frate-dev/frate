#pragma once
#include "nlohmann/json_fwd.hpp"
#include <exception>
#include <initializer_list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cxxopts.hpp>
#include <Frate/Utils/CLI.hpp>


#define ENDL "\n"

namespace Command {
  using nlohmann::json;
  using Utils::CLI::Prompt;
  using namespace Utils::CLI::Ansi;

  using namespace cxxopts;

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

  typedef struct RemoteServer{
    std::string name;
    std::string ip;
    std::string username;
    std::string authMethod;
    std::optional<std::string> password;
    std::optional<std::string> key;
    int port;
  } RemoteServer;
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
    std::vector<Package> dependencies{};
  } Mode;

  typedef struct Project_s {
    std::string project_name;
    std::string project_description;
    std::string project_type = ProjectType::EXECUTABLE;
    RemoteServer build_server;
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
    std::vector<RemoteServer> build_servers;
    std::vector<Package> dependencies;
    std::string build_dir{"build"};
    Package testing_lib;
    std::string project_version{"0.0.1"};
    std::vector<std::string> flags; 
    std::shared_ptr<cxxopts::ParseResult> args;
    std::vector<std::string> toolchains;
    void fromJson(json j);
    nlohmann::json toJson();
  } Project;
  typedef struct Handler_s Handler;
  typedef struct Handler_s {
    std::vector<std::string> aliases;
    std::vector<std::string> flags{};
    std::vector<Handler> subcommands{};
    std::vector<std::string> positional_args{};
    std::string docs{""};
    std::function<bool()> callback{
      []() -> bool {
        std::cout << "This command has not been implemented yet" << std::endl;
        return false;
      }
    };
  } Handler;
  class Interface{
    private:
      //Commands;
      bool init();
      bool add();
      bool get();
      bool remove();
      bool update();
      bool run();
      bool help();
      bool search();
      bool ftp();
      bool watch();
      bool toolchains();
      bool clean();
      bool build();
      bool list();
      //TODO: setup register comamnd
      bool registerCommand(
          std::initializer_list<std::string> &alisas,
          std::initializer_list<std::string> &flags,
          std::function<bool()> &callback
          );
      void getHelpString(std::string name,std::vector<Handler> &handlers,bool is_subcommand = false);
      void getHelpString(Handler &handler);
      bool runCommand(std::string,std::vector<Handler>&);
    public:
      std::shared_ptr<Project> pro;
      bool project_present{false};
      std::vector<Handler> commands{};
      //All sub command getters
      std::vector<Handler> getAddHandlers();
      std::vector<Handler> getListHandlers();
      std::vector<Handler> getSearchHandlers();
      std::vector<Handler> getRemoveHandlers();
      std::vector<Handler> getUpdateHandlers();

      bool skip_prompts{false};
      bool parse();
      std::shared_ptr<cxxopts::Options> options;
      std::shared_ptr<cxxopts::ParseResult> args;
      char** argv;
      int argc;
      bool confirm_all{false};
      Interface(int argc, char **argv);
      ~Interface();
      bool InitHeader();
      bool CreateCMakelists();
      bool LoadPackageJson();
  };
  //TODO: To be imlemented later to get information about a target or action
  typedef struct Info_s {
    std::string name;
    std::string description;
    std::vector<std::string> valid_flags;
  } Info;

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
      bool Flags(Interface*);
      bool Mode(Interface*);
      bool Watch(Interface*);
      bool Clean(Interface*);
      bool Build(Interface*);
      bool List(Interface*);
  };

  std::string downloadIndex();

  json fetchIndex();

  bool updateIndex();

}
