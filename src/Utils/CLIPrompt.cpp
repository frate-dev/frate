#include "Frate/Utils/General.hpp"
#include <Frate/Utils/CLI.hpp>
#include <exception>
#include <string>
#include <termios.h>
#include <termio.h>
#include <unistd.h>

namespace Utils::CLI {
  Prompt::Prompt(std::string prompt){
    this->prompt = prompt;
    this->color = Ansi::GREEN;
  }
  Prompt::Prompt(std::string prompt, std::string default_input){
    this->prompt = prompt;
    this->input = default_input;
    this->color = Ansi::GREEN;
  }
  Prompt* Prompt::AddOption(std::string option){
    this->options.push_back(option);
    return this;
  }
  Prompt* Prompt::AddOption(int option){
    this->options.push_back(std::to_string(option));
    return this;
  }
  Prompt* Prompt::AddOption(float option){
    this->options.push_back(std::to_string(option));
    return this;
  }
  Prompt* Prompt::AddOption(size_t option){
    this->options.push_back(std::to_string(option));
    return this;
  }
  Prompt* Prompt::MaxLength(int max_length){
    this->max_length = max_length;
    return this;
  }
  Prompt* Prompt::Options(std::vector<std::string> options){
    this->options = options;
    return this;
  }
  Prompt* Prompt::Color(std::string color){
    this->color = color;
    return this;
  }
  Prompt* Prompt::Validator(std::function<bool(std::string)> validator){
    this->validator = validator;
    return this;
  }
  bool Prompt::has_options(){
    return options.size() > 0;
  }
  bool Prompt::has_max_length(){
    return max_length > 0;
  }
  bool Prompt::has_validator(){
    return validator != nullptr;
  }
  bool Prompt::is_valid(){
    if(has_max_length()){
      if(input.length() > max_length){
        return false;
      }
    }
    if(has_options()){
      if(!is_in_options(input)){
        return false;
      }
    }
    if(has_validator()){
      if(!validator(input)){
        return false;
      }
    }
    return true;
  }
  void Prompt::current_input(){
    if(is_valid()){
      std::cout << termcolor::green << input << Ansi::RESET;
    }else{
      std::cout << termcolor::red << input << Ansi::RESET;
    }
  }
  void Prompt::prompt_prefix(int term_width){
    int text_length = 0;
    if(is_bool){
      std::cout << color << prompt << Ansi::RESET;
      std::string  booloptions = "[y/n]";
      std::cout << prompt;
      text_length += prompt.length() + booloptions.length();
    }else if(has_options()){
      std::cout 
        << color 
        << prompt 
        << termcolor::white 
        << "[" << color 
        << default_input 
        << termcolor::white << "]:" 
        << Ansi::RESET << "\n";
      text_length = 0;
      cursor_position++;
      if(print_valid_options){
        for(size_t i = 0; i < options.size(); i++){
          std::cout << "[" << options[i] << "]";
          text_length += options[i].length() + 2;
          if(i % 3 == 2){
            std::cout << "\n";
            text_length = 0;
            cursor_position++;
          }else if(i == options.size() - 1){
            text_length = 0;
            cursor_position++;
            std::cout << "\n";
          }
        }
      }
      std::cout << ">";
      text_length += 1;
    }else{
      std::ostringstream size_fraction;
      if(has_max_length()){
        size_fraction << termcolor::green << "[" << input.size() << "/" << max_length << "]";
      }else{
        size_fraction << "";
      }
      std::cout << color << prompt << termcolor::white << size_fraction.str() << " : " << Ansi::RESET;
      text_length += prompt.length() + size_fraction.str().length() + 3;
    }
    cursor_position += text_length / term_width;
  }
  void Prompt::get_input(){




    struct termios oldt, newt;
    struct winsize term_size;
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_size);

    prompt_prefix(term_size.ws_col);
    std::cout << input;

    char c;
    std::string completion;
    int completion_index = 0;
    while(true){
      c = getchar();
      if(c == '\n'){
        if(completion.size() > 0){
          input = completion;
        }
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
        return;
      //Tab
      }else if(c == 9){
        if(has_options()){
          if(completion.size() == 0){
            if(options.size() > 1){
              completion = options[1];
            }else if(options.size() == 1){
              completion = options[0];
            }else{
              completion = options[completion_index];
            }
          }else{
            completion_index++;
            if(completion_index >= options.size()){
              completion_index = 0;
            }
            completion = options[completion_index];
          }
        }
      }else if(c == 127){
        if(completion.size() > 0){
          input = completion;
          completion = "";
        }

        if(input.size() > 0){
          input.pop_back();
        }
      }else{
        input += c;
      }
      for(int i = 0; i < cursor_position; i++){
        std::cout << "\33[2K\r";
        std::cout << "\33[A";
      }
      std::cout << "\33[2K\r";
      cursor_position = 0;
      prompt_prefix(term_size.ws_col);
      if(completion.size() > 0){
        std::cout << completion;
      }else{
        current_input();
      }
    }
    //std::getline(std::cin, input);
  };
  bool Prompt::is_in_options(std::string option){
    for(size_t i = 0; i < options.size(); i++){
      if(options[i] == option){
        return true;
      }
    }
    return false;
  }
  template <>
  std::pair<bool, bool> Prompt::Get(){
    if(input == "y" || input == "Y"){
      return std::make_pair(true, true);
    }else if(input == "n" || input == "N"){
      return std::make_pair(true, false);
    }else{
      return std::make_pair(false, false);
    }
  }
  template <>
  std::pair<bool, std::string> Prompt::Get(){
    if(has_options()){
      if(is_in_options(input)){
        return std::make_pair(true, input);
      }else{
        std::cout << "Invalid option: " << input << std::endl;
        return std::make_pair(false, "");
      }
    }else{
      return std::make_pair(true, input);
    }
  }
  template <>
  std::pair<bool, int> Prompt::Get(){
    if(has_options()){
      if(is_in_options(input)){
        try{
          return std::make_pair(true, std::stoi(input));
        }catch(std::exception& e){
        std::cout << "Invalid int: " << input << std::endl;
          return std::make_pair(false, 0);
        }
      }else{
        return std::make_pair(false, 0);
      }
    }else{
      try{
        return std::make_pair(true, std::stoi(input));
      }catch(std::exception& e){
        std::cout << "Invalid int: " << input << std::endl;
        return std::make_pair(false, 0);
      }
    }
  }
  template<>
  std::pair<bool, float> Prompt::Get(){
    if(has_options()){
      if(is_in_options(input)){
        try{
          return std::make_pair(true, std::stof(input));
        }catch(std::exception& e){
        std::cout << "Invalid float: " << input << std::endl;
          return std::make_pair(false, 0);
        }
      }else{
        return std::make_pair(false, 0);
      }
    }else{
      try{
        return std::make_pair(true, std::stof(input));
      }catch(std::exception& e){
        std::cout << "Invalid float: " << input << std::endl;
        return std::make_pair(false, 0);
      }
    }
  }
  Prompt* Prompt::IsBool(){
    is_bool = true;
    return this;
  }
  Prompt* Prompt::ExitOnFailure(){
    this->exit_on_failure = true;
    return this;
  }
  Prompt* Prompt::PrintValidOptions(){
    print_valid_options = true;
    return this;
  }
  bool Prompt::Run(){
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
    std::cout << std::endl;
    return true;
  }
}

