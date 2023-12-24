#include <Frate/Utils/Logging.hpp>
#include <Frate/ProjectPrompt.hpp>
#include <Frate/Utils/Macros.hpp>

namespace Frate::Command {

  void to_json(json &j, const ProjectPrompt &p){
    TO_JSON_FIELD(p, text);
    TO_JSON_FIELD(p, value);
    TO_JSON_FIELD(p, default_value);
    TO_JSON_FIELD(p, type);
    TO_JSON_FIELD(p, required);

  }
  void from_json(const json &j, ProjectPrompt &p){ 
    FROM_JSON_FIELD(p, text);
    FROM_JSON_FIELD(p, value);
    FROM_JSON_FIELD(p, default_value);
    FROM_JSON_FIELD(p, type);
    FROM_JSON_FIELD(p, required);
    FROM_JSON_FIELD(p, options);
  }
  template<>
  std::string ProjectPrompt::get<std::string>(){

    if(this->type != "string"){ 
      Utils::error <<
        "Cannot get string from non-string option in project options" << std::endl;
      exit(1);
    }

    if(!this->validator(this->value)){
      if(this->required){
        Utils::error <<
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
      Utils::error <<
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
      Utils::error <<
        "Cannot get float from non-float option in project options" << std::endl;
      exit(1);
    }

    if(!this->validator(this->value)){
      if(this->required){
        Utils::error <<
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
      Utils::error <<
        "Cannot get int from non-int option in project options" << std::endl;
      exit(1);
    }

    if(!this->validator(this->value)){
      if(this->required){
        Utils::error <<
          "Invalid value for required option: " << this->value << std::endl;
        exit(1);
      }
      return std::stoi(this->default_value);
    }

    return std::stoi(this->value);
  }
}
