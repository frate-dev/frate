#include <Frate/Generators.hpp>

namespace Frate::Generators::Project {
  std::pair<bool, Template> promptForTemplateName(json index){
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
  bool runTemplatePrompts(std::shared_ptr<Command::Project> pro){
    for(auto [key, tmpl_prompt] : pro->prompts){
      Prompt prompt(tmpl_prompt.text, tmpl_prompt.default_value);
      if(tmpl_prompt.type == "bool"){
        prompt.IsBool();
      }

      for(std::string option: tmpl_prompt.options){
        prompt.AddOption(option);
      }

      prompt.PrintValidOptions();

      prompt.Run();
      auto [valid, value] = prompt.Get<std::string>();  
      if(!valid){
        error << "Error while getting prompt: " << key << std::endl;
        return false;
      }
      pro->prompts[key].value = value;
    }
    return true;
  }
}
