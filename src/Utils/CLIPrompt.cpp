#include "CLI.hpp"


namespace Utils::CLI {

  template <typename T>
  Prompt<T>::Prompt(std::string prompt, std::string color){
    this->prompt = prompt;
    this->color = color;
  }

  template <typename T>
  Prompt<T>* Prompt<T>::AddOption(T option){
    this->options.push_back(option);
    return this;
  }
  template <typename T> 
  Prompt<T>* Prompt<T>::MaxLength(int max_length){
    this->max_length = max_length;
    return this;
  }
  template <typename T>
  Prompt<T>* Prompt<T>::Options(std::vector<T> options){
    this->options = options;
    return this;
  }
  template <typename T>
  Prompt<T>* Prompt<T>::Color(std::string color){
    this->color = color;
    return this;
  }
  template <typename T>
  Prompt<T>* Prompt<T>::Validator(std::function<bool(T)> validator){
    this->validator = validator;
    return this;
  }
  template <typename T>
  bool Prompt<T>::has_options(){
    return options.size() > 0;
  }
  template <typename T>
  bool Prompt<T>::has_max_length(){
    return max_length > 0;
  }
  template <typename T>
  bool Prompt<T>::has_validator(){
    return validator != nullptr;
  }
  template <typename T>
  void Prompt<T>::get_input(){
    std::cout << color << prompt << Ansi::RESET;
    if(std::is_same<T, bool>::value){
      std::cout << " [y/n] ";

    }else if(has_options()){
      for(int i = 0; i < options.size(); i++){
        std::cout << "[ " << options[i] << " ]";
      }
    }
    std::getline(std::cin, input);
  };
  template <typename T>
  bool Prompt<T>::is_in_options(T option){
    for(int i = 0; i < options.size(); i++){
      if(options[i] == option){
        return true;
      }
    }
    return false;
  }
  template <typename T>
  T Prompt<T>::Get(){
    if(value == nullptr){
      std::cout << "Prompt has not been run yet" << std::endl;
      return nullptr;
    }
    return value;
  }
  template <typename T>
  bool Prompt<T>::Run(){
    get_input();
    if(has_max_length()){
      while(input.length() > max_length){
        std::cout << "Input too long" << std::endl;
        return false;
      }
    }
    //Checking to see if the input is one of the approved options
    if(std::is_same<T, int>::value){
      try{
        value = std::stoi(input);
      }catch(std::invalid_argument){
        std::cout << "Invalid integer value" << std::endl;
        return false;
      }
    }else if(std::is_same<T, float>::value){
      try{
        value = std::stof(input);
      }catch(std::invalid_argument){
        std::cout << "Invalid float value" << std::endl;
        return false;
      }
    }else if(std::is_same<T, double>::value){
      try{
        value = std::stod(input);
      }catch(std::invalid_argument){
        std::cout << "Invalid double value" << std::endl;
        return false;
      }
    }else if(std::is_same<T, bool>::value){
      //If bool is the type we just need to check if the input is y or n
      if(input != "y" && input != "n"){
        std::cout << "Invalid input expected either y or n" << std::endl;
        return false;
      }
      value = input == "y";
    }else{
      std::cout << "Invalid type for prompt" << std::endl;
      return false;
    }
    if(has_validator()){
      while(!validator(value)){
        std::cout << "Invalid input: " << std::endl;
        return false;
      }
    }
  }
}

