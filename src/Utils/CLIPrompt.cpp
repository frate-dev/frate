#include "Frate/Utils/General.hpp"
#include <Frate/Utils/CLI.hpp>
#include <exception>
#include <string>

namespace Utils::CLI {
  template <typename T>
  Prompt<T>::Prompt(std::string prompt){
    this->prompt = prompt;
    this->color = Ansi::GREEN;
  }
  template <>
  Prompt<std::string>::Prompt(std::string prompt){
    this->prompt = prompt;
    this->value = "";
    this->color = Ansi::GREEN;
  }
  template <>
  Prompt<int>::Prompt(std::string prompt){
    this->prompt = prompt;
    this->value = 0;
    this->color = Ansi::GREEN;
  }
  template <>
  Prompt<float>::Prompt(std::string prompt){
    this->prompt = prompt;
    this->value = 0.0f;
    this->color = Ansi::GREEN;
  }
  template <>
  Prompt<double>::Prompt(std::string prompt){
    this->prompt = prompt;
    this->value = 0.0;
    this->color = Ansi::GREEN;
  }
  
  template <typename T>
  Prompt<T>::Prompt(std::string prompt, T default_value){
    this->prompt = prompt;
    this->value = default_value;
    this->color = Ansi::GREEN;
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
  template <>
  bool Prompt<int>::yoink(){
    try{
      value = std::stoi(input);
    }catch(std::exception& e){
      Utils::debug("Failed to convert input to int");
      return false;
    }
    return true;
  }
  template <>
  bool Prompt<float>::yoink(){
    try{
      value = std::stof(input);
    }catch(std::exception& e){
      Utils::debug("Failed to convert input to float");
      return false;
    }
    return true;
  }
  template <>
  bool Prompt<double>::yoink(){
    try{
      value = std::stod(input);
    }catch(std::exception& e){
      Utils::debug("Failed to convert input to double");
      return false;
    }
    return true;
  }
  template <>
  bool Prompt<std::string>::yoink(){
    if(value.size() == 0){
      value = input;
      return true;
    }
    return true;
  }
  template <>
  bool Prompt<bool>::yoink(){
#ifdef TEST 
    return true;
#endif
    if(input == "y" || input == "Y"){
      value = true;
      return true;
    }else if(input == "n" || input == "N"){
      value = false;
      return true;
    }
    return false;
  }
  template <typename T>
  void Prompt<T>::get_input(){

    if(std::is_same<T, bool>::value){
      std::cout << color << prompt << Ansi::RESET;
      std::cout << "[y/n]";
    }else if(has_options()){
      std::cout << color << prompt << termcolor::white << " [" << color << value << termcolor::white << "] : " << Ansi::RESET << "\n";
      if(print_valid_options){
        for(size_t i = 0; i < options.size(); i++){
          std::cout << "[ " << options[i] << " ] ";
          if(i % 3 == 2){
            std::cout << "\n";
          }else if(i == options.size() - 1){
            std::cout << "\n";
          }
        }
      }
      std::cout << ">";
    }else{
      std::cout << color << prompt << termcolor::white << " [" << color << value << termcolor::white << "] : " << Ansi::RESET;
    }
    std::getline(std::cin, input);
  };
  template <typename T>
  bool Prompt<T>::is_in_options(T option){
    for(size_t i = 0; i < options.size(); i++){
      if(options[i] == option){
        return true;
      }
    }
    return false;
  }
  template <typename T>
  T Prompt<T>::Get(){
    return value;
  }
  template <typename T>
  Prompt<T>* Prompt<T>::ExitOnFailure(){
    this->exit_on_failure = true;
    return this;
  }
  template <typename T>
  Prompt<T>* Prompt<T>::PrintValidOptions(){
    print_valid_options = true;
    return this;
  }
  template <typename T>
  bool Prompt<T>::Run(){
    get_input();
    if(has_max_length()){
      while(input.length() > max_length){
        std::cout << "Input too long" << std::endl;
        if(exit_on_failure){
          exit(1);
        }
        return false;
      }
    }
    if(!Prompt<T>::yoink()){
      std::cout << "Invalid input for type " << std::endl;
      return false;
    }
    if(has_validator()){
      while(!validator(value)){
        std::cout << "Invalid input: " << std::endl;
        if(exit_on_failure){
          exit(1);
        }
        return false;
      }
    }

    if(has_options() && !std::is_same<T, bool>::value){
      if(!is_in_options(value)){
        std::cout << "Invalid input: " << std::endl;
        if(exit_on_failure){
          exit(1);
        }
        return false;
      }
    }
    return true;
  }
}

