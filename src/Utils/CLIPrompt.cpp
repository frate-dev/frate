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
    std::getline(std::cin, input);
  };
  template <typename T>
  void Prompt<T>::Run(){
    get_input();
    if(has_max_length()){
      while(input.length() > max_length){
        get_input();
      }
    }
  }
}

