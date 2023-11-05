#include "CLI.hpp"
#include <string>

namespace Utils::CLI {
  template <typename T>
  Prompt<T>::Prompt(std::string prompt){
    this->prompt = prompt;
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
    }catch(std::invalid_argument e){
      return false;
    }
    return true;
  }
  template <>
  bool Prompt<float>::yoink(){
    try{
      value = std::stof(input);
    }catch(std::invalid_argument e){
      return false;
    }
    return true;
  }
  template <>
  bool Prompt<double>::yoink(){
    try{
      value = std::stod(input);
    }catch(std::invalid_argument e){
      return false;
    }
    return true;
  }
  template <>
  bool Prompt<std::string>::yoink(){
    value = input;
    return true;
  }
  template <>
  bool Prompt<bool>::yoink(){
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
      std::cout << color << prompt << Ansi::RESET << "\n";
      for(size_t i = 0; i < options.size(); i++){
        std::cout << "[ " << options[i] << " ]" << "\n";
      }
      std::cout << ">";
    }else{
      std::cout << color << prompt << Ansi::RESET;
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
  bool Prompt<T>::Run(){
    get_input();
    if(has_max_length()){
      while(input.length() > max_length){
        std::cout << "Input too long" << std::endl;
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
        return false;
      }
    }

    if(has_options() && !std::is_same<T, bool>::value){
      if(!is_in_options(value)){
        std::cout << "Invalid input: " << std::endl;
        return false;
      }
    }
    return true;
  }
}

