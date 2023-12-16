#include <Frate/Generators.hpp>

namespace Frate::Generators::Project {
  std::pair<bool, Template> promptForTemplateName(json index){
    Prompt template_name_prompt("Project type: ");
    for(Template templ: index){
      template_name_prompt.addOption(templ.name);
    }

    template_name_prompt.printValidOptions();

    template_name_prompt.run();

    auto [success,template_id] = template_name_prompt.get<std::string>();

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
        prompt.isBool();
      }

      for(std::string option: tmpl_prompt.options){
        prompt.addOption(option);
      }

      prompt.printValidOptions();

      prompt.run();
      auto [valid, value] = prompt.get<std::string>();  
      if(!valid){
        error << "Error while getting prompt: " << key << std::endl;
        return false;
      }
      pro->prompts[key].value = value;
    }
    return true;
  }
}
