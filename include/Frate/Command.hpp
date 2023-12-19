#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <iostream>
#include <cxxopts.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Frate.hpp>

#define ENDL "\n"

namespace Frate::Command {

  using nlohmann::json;
  using Utils::CLI::Prompt;
  using namespace Utils::CLI::Ansi;
  using namespace cxxopts;

  class Package {
    public:
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
      // json toJson();
      // void fromJson(json j);
      bool addCallback(sol::state &lua);
      friend void from_json(const json &j, Package& package);
      friend void to_json(json &j, const Package& package);
      Package(){};
  };

  typedef struct RemoteServer_s{
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
      return type == EXECUTABLE 
        || type == HEADER_ONLY 
        || type == STATIC_LIBRARY 
        || type == SHARED_LIBRARY;
    }
  };

  class Mode{
    public:
      std::string name;
      std::vector<std::string> flags;
      std::vector<Package> dependencies{};
      friend void from_json(const json &j, Mode& mode);
      friend void to_json(json &j, const Mode& mode);
      Mode(std::string name = "", std::vector<std::string> flags = {}, std::vector<Package> dependencies = {});
  };

  class ProjectPrompt{
    public:
      ProjectPrompt(){};
      std::string value{""};
      std::string text{""};
      std::string type{"string"};
      std::string default_value{""};
      bool required{false};
      std::vector<std::string> options{};
      friend void from_json(const json &j, ProjectPrompt& prompt);
      friend void to_json(json &j, const ProjectPrompt& prompt);
      std::function<bool(std::string)> validator{
        [this](std::string s) -> bool {
          if(options.size() == 0) {
            return true;
          }else{
            for (std::string option: options){
              if (s == option){
                return true;
              }
            }
            return false;
          }
        }
      };
      template<typename T>
        T get();
  };

  //TODO: MAKE MOST OF THESE OPTIONAL
  class Template;
  class Project {
    public:
      Project();
      std::string name;
      std::string description;
      std::string type{""};
      RemoteServer build_server;
      std::filesystem::path path;
      std::string git{"null"};
      std::string homepage{"null"};
      std::string bugs{"null"};
      std::string lang{"cpp"};
      std::string cmake_version{"3.28"};
      std::string lang_version{"20"};
      std::string compiler{"g++"};
      std::string license{""};
      std::string default_mode{"Release"};
      std::string build_command{"cmake --build ."};
      std::vector<Mode> modes{
        Mode("Release",{"-O2"}),
        Mode("Debug",{"-g"}),
        Mode("Test",{"-g"})
      };
      std::vector<std::string> authors{};
      std::vector<std::string> keywords{};
      std::string src_dir{"src"};
      std::string include_dir{"include"};
      std::vector<RemoteServer> build_servers{};
      std::vector<Package> dependencies{};
      std::string build_dir{"build"};
      Package testing_lib;
      std::string version{"0.0.1"};
      std::vector<std::string> flags{};
      std::vector<std::string> toolchains {};
      std::vector<std::string> libs{};
      //void fromJson(json j);
      //nlohmann::json toJson();
      bool save();
      void checkKeys(json j);
      std::string getPath(){
        return path.string();
      };
      std::unordered_map<std::string,ProjectPrompt> prompts{};
      std::unordered_map<std::string,json> variables{};
      friend void from_json(const json &j, Project& pro);
      friend void to_json(json &j, const Project& pro);
      void fromTemplate(Template &t);
  };
  class Template {
    public:
      Template();
      std::string name;
      std::string description;
      std::string version;
      std::string src_dir{"src"};
      std::string include_dir{"include"};
      std::string build_dir{"build"};
      std::vector<Package> dependencies{};
      std::string default_mode{"Release"};
      std::vector<Mode> modes{};
      std::vector<std::string> flags{};
      std::vector<std::string> system_libs{};
      std::vector<std::string> supported_toolchains{};
      std::vector<std::string> supported_languages{};
      std::vector<std::string> supported_language_versions{};
      std::string min_cmake_version{"3.10"};
      std::vector<std::string> supported_compilers{};
      std::vector<std::string> keywords{};
      std::unordered_map<std::string,ProjectPrompt> prompts{};
      std::unordered_map<std::string,json> variables{};
      friend void from_json(const json &j, Template& t);
      friend void to_json(json &j, const Template& t);
      friend void to_project(Project &pro, const Template &t);
  };


  typedef struct Handler_s Handler;
  class Interface;

  typedef struct Handler_s {
    std::vector<std::string> aliases;
    std::vector<std::string> flags{};
    std::vector<Handler> subcommands{};
    std::vector<std::string> positional_args{};
    std::string docs{""};
    std::function<bool(std::shared_ptr<Interface> )> callback{
      [](std::shared_ptr<Interface> inter) -> bool {
        (void)inter;
        std::cout << "This command has not been implemented yet" << std::endl;
        return false;
      }
    };
    bool implemented{true};
    bool requires_project{true};
    bool unlimited_args{false};
  } Handler;

  class Interface : public std::enable_shared_from_this<Interface>{
    public:
      Interface(int argc, char **argv);
      void getHelpString(std::string name,std::vector<Handler> &handlers,bool is_subcommand = false);
      void getHelpString(Handler &handler);
      //bool runCommand(std::string,std::vector<Handler>&);
      std::shared_ptr<Project> pro;
      bool project_present{false};
      //bool verbose{false};
      std::vector<Handler> commands{};

      //bool execute();
      bool parse();
      std::shared_ptr<cxxopts::Options> options;
      std::shared_ptr<cxxopts::ParseResult> args;
      char** argv;
      int argc;
      bool confirm_all{false};
      bool InitHeader();
      bool CreateCMakelists();
      bool loadProjectJson();
      ~Interface();
  };

  bool execute(std::shared_ptr<Interface> inter);


  namespace OptionsInit{
      bool Main(std::shared_ptr<Interface> inter);
  };

  std::string downloadIndex();

  json fetchIndex();

  bool updateIndex();

  bool runCommand(std::shared_ptr<Interface> inter,std::string command, std::vector<Handler> &handlers);
}
