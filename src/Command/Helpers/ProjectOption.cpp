#include <Frate/Command.hpp>


namespace Frate::Command {

  void to_json(json &j, const ProjectPrompt &p){
    j = json{
      {"value", p.value},
      {"default_value", p.default_value},
      {"type", p.type},
      {"required", p.required},
      // {"validator", p.validator}
    };
  }
  void from_json(const json &j, ProjectPrompt &p){ j.at("value").get_to(p.value);
    j.at("default_value").get_to(p.default_value);
    j.at("type").get_to(p.type);
    j.at("required").get_to(p.required);
    //j.at("validator").get_to(p.validator); need a way to make this a lua function
  }
  template<>
  std::string ProjectPrompt::get<std::string>(){

    if(this->type != "string"){ 
      Frate::error <<
        "Cannot get string from non-string option in project options" << std::endl;
      exit(1);
    }

    if(!this->validator(this->value)){
      if(this->required){
        Frate::error <<
          "Invalid value for required option: " << this->value << std::endl;
        exit(1);
      }
      return this->default_value;
    }

    return this->value;
  }

  template<>
  bool ProjectPrompt::get<bool>(){

    if(this->type != "bool"){ 
      Frate::error <<
        "Cannot get bool from non-bool option in project options" << std::endl;
      exit(1);
    }

    if(!this->validator(this->value)) return false;

    if (
        this->value == "true" ||
        this->value == "yes" ||
        this->value == "1"
       ) return true;

    return false;
  }

  template<>
  float ProjectPrompt::get<float>(){

    if(this->type != "float"){ 
      Frate::error <<
        "Cannot get float from non-float option in project options" << std::endl;
      exit(1);
    }

    if(!this->validator(this->value)){
      if(this->required){
        Frate::error <<
          "Invalid value for required option: " << this->value << std::endl;
        exit(1);
      }
      return std::stof(this->default_value);
    }

    return std::stof(this->value);
  }

  template<>
  int ProjectPrompt::get<int>(){

    if(this->type != "int"){ 
      Frate::error <<
        "Cannot get int from non-int option in project options" << std::endl;
      exit(1);
    }

    if(!this->validator(this->value)){
      if(this->required){
        Frate::error <<
          "Invalid value for required option: " << this->value << std::endl;
        exit(1);
      }
      return std::stoi(this->default_value);
    }

    return std::stoi(this->value);
  }
}
