#pragma once
#include "Frate/Project/Config.hpp"
#include "Frate/Utils/Logging.hpp"
#include "inja.hpp"
#include <Frate/FrateException.hpp>
#include <Frate/Lua/Exceptions.hpp>
#include <sol/state.hpp>
#include <string>
#include <unordered_map>

namespace Frate::Lua {

  class TemplateEnvironmentException : public FrateException {
  public:
    explicit TemplateEnvironmentException(const std::string &message)
        : FrateException(message) {}
  };
  /*
   * This class is responsible for handling the lua template environent,
   * managing macros, pre and post scripts, and templating files. It is designed
   * in such a way that allows complete control of over the template render
   * pipeline.
   * To use this, start by registering all of your scripts using your method of
   * choice, then use templateFile to render your file to the output path
   */
  class TemplateEnvironment {
  private:
    std::shared_ptr<sol::state> lua;
    inja::Environment env;
    std::unordered_map<std::string, std::string> macro_scripts;
    std::unordered_map<std::string, std::string> init_scripts;
    std::unordered_map<std::string, std::string> post_scripts;
    std::shared_ptr<Project::Config> pro;

    void register_user_types();
    void register_frate_api();
    void register_inja_function(std::string name, std::string lua_script_text);

  public:
    /*
     * A project config is required to register the project with the lua
     * environment
     * @param pro: The project config
     * @throws LuaException: If there is an error while registering the project
     */
    TemplateEnvironment(std::shared_ptr<Project::Config> pro)
        : pro(pro) {

      lua = std::make_shared<sol::state>();

      macro_scripts = std::unordered_map<std::string, std::string>();
      init_scripts = std::unordered_map<std::string, std::string>();
      post_scripts = std::unordered_map<std::string, std::string>();

      if(pro == nullptr){
        throw TemplateEnvironmentException("Project is null on construction");
      }

      try {
        Utils::verbose << "Registering frate api" << std::endl;
        register_frate_api();

      } catch (std::exception &e) {
        Utils::error << e.what() << std::endl;
        throw TemplateEnvironmentException("Error registering frate api");
      }

      try {
        Utils::verbose << "Registering user types" << std::endl;
        register_user_types();

      } catch (std::exception &e) {
        Utils::error << e.what() << std::endl;
        throw TemplateEnvironmentException("Error registering user types");
      }
    };

    ~TemplateEnvironment() = default;
    /*
     * Registers a new macro script the name of the script is the identifier
     * @note Using relativePathToNamespace to get a namespaced script name is
     * advised
     * @param name: The name of the script
     * @param script_text: The text of the script
     * @throws LuaException: If there is an error while registering the script
     */
    void registerMacroScript(std::string name, std::string script_text);
    /*
     * Registers a new init script the name of the script is the identifier
     * currently the name is not used for anything
     * @param name: The name of the script
     * @param script_text: The text of the script
     * @throws LuaException: If there is an error while registering the script
     */
    void registerInitScript(std::string name, std::string script_text);
    /*
     * Registers a new post script the name of the script is the identifier
     * currently the name is not used for anything
     * @param name: The name of the script
     * @param script_text: The text of the script
     * @throws LuaException: If there is an error while registering the script
     */
    void registerPostScript(std::string name, std::string script_text);

    /*
     * @param input_file: The path to the input .inja file
     * @param output_file: The path to the output file
     * @throws LuaException: If there is an error while templating the file
     * @throws std::exception: If there is an error while writing the file
     */
    void templateFile(std::filesystem::path input_file,
                      std::filesystem::path output_file);
    /*
     * Runs __init__ scripts assuming they have been registered
     * @throws LuaException: If there is an error while running the script
     */
    void runInitScripts();
    /*
     * Runs __post__ scripts assuming they have been registered
     * @throws LuaException: If there is an error while running the script
     */
    void runPostScripts();

    /*
     * Providing this a root path and a file path it will return the dot
     * notation version of the path while also removing the extension
     * @param root_path: The root path which you would like to base the relative
     * path off of
     * @param file_path: The file path which you would like to convert to dot
     * notation
     * @return: The dot notation version of the file path
     * @example: relativePathToNamespace("/home/user/project",
     * "/home/user/project/src/main.cpp") -> "src.main"
     */
    static std::string relativePathToNamespace(std::filesystem::path root_path,
                                               std::filesystem::path file_path);
    /*
     * Providing this a file path it will return the dot notation version of the
     * path while also removing the extension
     * @param file_path: The file path which you would like to convert to dot
     * notation
     * @return: The dot notation version of the file path
     * @example: relativePathToNamespace("src/main.cpp") -> "src.main"
     */
    static std::string
    relativePathToNamespace(std::filesystem::path relative_path);


    // Getters

    [[nodiscard]] const std::shared_ptr<Project::Config> &getProjectConfig() const {
      return pro;
    }

    [[nodiscard]] const std::unordered_map<std::string, std::string> &getMacroScripts() const {
      return macro_scripts;
    }

    [[nodiscard]] const std::unordered_map<std::string, std::string> &getInitScripts() const {
      return init_scripts;
    }

    [[nodiscard]] const std::unordered_map<std::string, std::string> &getPostScripts() const {
      return post_scripts;
    }

    [[nodiscard]] const std::shared_ptr<sol::state> &getLuaState() const {
      return lua;
    }

    [[nodiscard]] const inja::Environment &getInjaEnv() const {
      return env;
    }

    //Setters
    
    void setProjectConfig(const std::shared_ptr<Project::Config> &pro) {
      TemplateEnvironment::pro = pro;
    }

    // Display

    friend std::ostream &operator<<(std::ostream &os,
                                    const TemplateEnvironment &environment) {

      for(auto [name, script] : environment.macro_scripts){
        os << "Macro Script: " << name << std::endl;
        os << script << std::endl;
      }
      for(auto [name, script] : environment.init_scripts){
        os << "Init Script: " << name << std::endl;
        os << script << std::endl;
      }
      for(auto [name, script] : environment.post_scripts){
        os << "Post Script: " << name << std::endl;
        os << script << std::endl;
      }
      return os;
    }

  };
} // namespace Frate::Lua
