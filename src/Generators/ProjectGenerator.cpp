
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
    return true;
  }

  bool renderTemplate(Environment &env, sol::state &lua, std::shared_ptr<Command::Project> pro){


    info << "Rendering templates to tmp" << std::endl;

    LuaAPI::registerAPI(lua);

    if(!LuaAPI::registerProject(lua, pro)){
      error << "Error while registering project" << std::endl;
      return false;
    }

    if(!LuaAPI::registerProjectScripts(env, lua,pro->project_path / "template/scripts")){
      error << "Error while registering project scripts" << std::endl;
      return false;
    } 

    std::unordered_map<std::string, bool> allowed_source_extensions = {
      {"cpp", false},
      {"c", false},
    };

    std::vector<std::string> possible_source_extensions;

    for(auto& [key, value]: allowed_source_extensions){
      if(value){
        possible_source_extensions.push_back(key);
      }
    }

    if(pro->lang == "cpp"){
      allowed_source_extensions["cpp"] = true;
    }else if(pro->lang == "c"){
      allowed_source_extensions["c"] = true;
    }

    std::filesystem::copy(
        pro->project_path / "template",
        pro->project_path,
        std::filesystem::copy_options::recursive
        );

    for(const path& current_p: std::filesystem::recursive_directory_iterator(pro->project_path)){
      
      if(current_p.string().find("template/") != std::string::npos){
        continue;
      }
      if(current_p.extension() == ".inja"){
        std::cout << "Rendering file: " << current_p << std::endl;
        std::string rendered_file = env.render_file(current_p, pro->toJson());
        std::string new_file = current_p.string();
        new_file = new_file.replace(new_file.find(".inja"), 5, "");
        std::cout << "Writing file: " << new_file << std::endl;
        std::ofstream file;
        try{
          file.open(new_file);
        }catch(...){
          error << "Error while opening file: " << new_file << std::endl;
          return false;
        }
        file << rendered_file;

        std::filesystem::remove(current_p);
      }
    }
    



//     for(const path& p: std::filesystem::recursive_directory_iterator(pro->project_path / "template")){
//       std::cout << "Path: " << p << std::endl;
// 
// 
// 
//       if(std::filesystem::is_directory(p)){
//         std::string new_dir = p.string();
//         new_dir = new_dir.substr((pro->project_path / "template").string().length());
// 
//         std::cout << "Creating directory: " << pro->project_path.string() + new_dir << std::endl;
//         std::cout << "Project path: " << pro->project_path << std::endl;
// 
//         if(std::filesystem::exists(pro->project_path.string() + new_dir)) continue;
//         
//         std::filesystem::create_directories(pro->project_path.string() + new_dir);
//       }else{
//         std::string new_file = p.string();
//         new_file = new_file.substr((pro->project_path / "template").string().length());
//         std::cout << "Copying file: " << new_file << std::endl;
//         std::filesystem::copy_file(
//             p,
//             pro->project_path / new_file);
// 
//       }
//     }
    return true;
  }


  bool create(std::shared_ptr<Command::Project> pro){
    info << "Creating Project" << std::endl;
    json index = getTemplateIndex();

    auto [success, templ] = promptForProjectName(index);
    if(!success){
      error << "Error while prompting for project name" << std::endl;
      return false;
    }
    info << "Creating project from template: " << templ.name << std::endl;

    if(!downloadTemplate(templ.git, pro->project_path)){
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
}
