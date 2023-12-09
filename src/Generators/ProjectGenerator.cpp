
#include "Frate/Command.hpp"
#include "Frate/LuaAPI.hpp"
#include "Frate/Utils/CLI.hpp"
#include "inja.hpp"
#include "nlohmann/json_fwd.hpp"
#include <Frate/Generators.hpp>
#include <filesystem>
#include <git2/types.h>
#include <git2/clone.h>

namespace Frate::Generators::Project {
using inja::Environment;
using nlohmann::json;
using std::filesystem::path;
using Utils::CLI::Prompt;
json getTemplateIndex() {
  std::cout << "Getting Template Index" << std::endl;
  std::string index_url = "https://github.com/frate-dev/templates/releases/"
                          "latest/download/index.json";

  json index = json::parse(Utils::fetchText(index_url));
  return index;
  } 


  void from_json(const json& j, Template& t){
    t.name = j.at("name").get<std::string>();
    t.git = j.at("git").get<std::string>();
    t.description = j.at("description").get<std::string>();
  }

  std::pair<bool, Template> promptForProjectName(json index){
    Prompt template_name_prompt("Project type: ");
    for(Template templ: index){
      template_name_prompt.AddOption(templ.name);
    }

    template_name_prompt.PrintValidOptions();

    template_name_prompt.Run();

    auto [success,template_id] = template_name_prompt.Get<std::string>();

    if(!success){
      error << "Error while getting template name" << std::endl;
      return {false, Template()};
    }

    Template templ;
    for(Template t: index){
      if(t.name == template_id){
        templ = t;
        break;
      }
    }

    if(templ.name.empty()){
      error << "Template not found" << std::endl;
      return {false, Template()};
    }
    return {true, templ};
  }

  bool downloadTemplate(std::string git_url, path project_path) {
    //Delete old template before downloading new one
    if(std::filesystem::exists(project_path / "template")){
      Frate::info << "Deleting old template" << std::endl;
      std::filesystem::remove_all(project_path / "template");
    }


    info << "Downloading template" << std::endl;
    git_repository *repo = NULL; 
    Frate::info << "Cloning " << git_url << " into " << (project_path / "template").c_str() << std::endl;
    int clone_status = git_clone(&repo, git_url.c_str(), (project_path / "template").c_str(), NULL);
    if(clone_status != 0){
      Frate::error << "Error while cloning repository" << std::endl;
      return false;
    }
    git_repository_free(repo);
    try{
      std::filesystem::remove(project_path / "template/.git");
    }catch(...){
      Frate::error << "could not find .git folder in template/" << std::endl;
    }

    return true;
  }

  bool refreshTemplate(Environment &env, sol::state &lua, std::shared_ptr<Command::Project> pro) {
    info << "Refreshing template" << std::endl;
    std::vector<path> paths_to_refresh{
      pro->project_path / "template/CMakeLists.txt.inja",
    };

    LuaAPI::registerAPI(lua);

    if(!LuaAPI::registerProject(lua, pro)){
      error << "Error while registering project" << std::endl;
      return false;
    }

    if(!LuaAPI::registerProjectScripts(env, lua,pro->project_path / "template/scripts")){
      error << "Error while registering project scripts" << std::endl;
      return false;
    }

    for(const path& current_p: paths_to_refresh){
      std::string rendered_file = env.render_file(current_p, pro->toJson());
      std::string new_file = current_p.string();
      new_file = new_file.replace(new_file.find(".inja"), 5, "");
      std::ofstream file;
      try{
        file.open(new_file);
      }catch(...){
        error << "Error while opening file: " << new_file << std::endl;
        return false;
      }
      file << rendered_file;
    }
    return true;
  }
      

  bool renderTemplate(
      Environment &env,
      sol::state &lua,
      std::shared_ptr<Command::Project> pro){


    info << "Rendering templates" << std::endl;

    LuaAPI::registerAPI(lua);
    
    std::string CPM;

    CPM = Utils::fetchText("https://raw.githubusercontent.com/cpm-cmake/CPM.cmake/v0.38.6/cmake/CPM.cmake");
    std::ofstream CPMFile;
    try{
      if(!std::filesystem::exists(pro->project_path / "cmake"))
        std::filesystem::create_directories(pro->project_path / "cmake");
      CPMFile.open(pro->project_path / "cmake/CPM.cmake");
    }catch(...){
      Utils::debug("Error while opening file: CPM.cmake");
      return false;
    }

    CPMFile << CPM;
    if(!LuaAPI::registerProject(lua, pro)){
      error << "Error while registering project" << std::endl;
      return false;
    }

    if(!LuaAPI::registerProjectScripts(env, lua,pro->project_path / "template/scripts")){
      error << "Error while registering project scripts" << std::endl;
      return false;
    } 
  

    
    std::filesystem::copy(
        pro->project_path / "template",
        pro->project_path,
        std::filesystem::copy_options::recursive  | std::filesystem::copy_options::overwrite_existing
        );

    std::vector<path> paths_to_remove;

    for(const path& current_p: std::filesystem::recursive_directory_iterator(pro->project_path)){
      if(current_p.string().find("template/") != std::string::npos){
        continue;
      }
      if(current_p.extension() == ".inja"){
        std::string rendered_file = env.render_file(current_p, pro->toJson());
        std::string new_file = current_p.string();
        new_file = new_file.replace(new_file.find(".inja"), 5, "");
        std::ofstream file;
        try{
          file.open(new_file);
        }catch(...){
          error << "Error while opening file: " << new_file << std::endl;
          return false;
        }
        file << rendered_file;
        paths_to_remove.push_back(current_p);
      }
      if (current_p.extension() == ".json"){
        try{
          std::filesystem::rename(
              current_p,
              pro->project_path / "frate-project.json"
              );
        }catch(...){
          error << "Error while copying frate-project.json" << std::endl;
          return false;
        }
        pro->save();
      }
      if(current_p.string().find("/scripts") != std::string::npos){
        paths_to_remove.push_back(current_p);
      }
    }

    for(const path& current_p: std::filesystem::recursive_directory_iterator(pro->project_path / pro->src_dir)){
      if(current_p.extension() != "." + pro->lang){
        paths_to_remove.push_back(current_p);
      }
    }

    for(const path& p: paths_to_remove){
      if(std::filesystem::is_directory(p)){
        std::filesystem::remove_all(p);
      }else{
        std::filesystem::remove(p);
      }
    }

    
    return true;
  }
  bool create(std::shared_ptr<Command::Project> pro){
    info << "Creating Project" << std::endl;
    

    json index = getTemplateIndex();
    bool has_template = false;

    Template current_template;

    for(Template templ: index){
      if(pro->project_type == templ.name){
        has_template = true;
        current_template = templ;
        break;
      }
    }

    if(!has_template){
      auto [success, templ] = promptForProjectName(index);
      if(!success){
        error << "Error while prompting for project name" << std::endl;
        return false;
      }
      current_template = templ;
      info << "Creating project from template: " << templ.name << std::endl;
    }

    if(!downloadTemplate(current_template.git, pro->project_path)){
      error << "Error while downloading template" << std::endl;
      return false;
    }

    Environment env;
    sol::state lua;
    if(!renderTemplate(env, lua,  pro)){
      error << "Error while rendering template to tmp" << std::endl;
      return false;
    }
    return true;
  }

  bool refresh(std::shared_ptr<Command::Project> pro){
    Environment env;
    sol::state lua;
    if(!refreshTemplate(env, lua,  pro)){
      error << "Error while rendering template to tmp" << std::endl;
      return false;
    }
    return true;
  }
}
