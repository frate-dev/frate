#pragma once
#include <functional>
#include <sstream>
#include <string>
#include <stdint.h>
#include <iostream>
#include <termcolor/termcolor.hpp>
#include <vector>



namespace Utils::CLI {
  namespace Ansi{
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BRIGHT_RED = "\033[91m";
  }
  typedef struct ListItem {
    std::string primary;
    std::string subtext;
    ListItem(std::string primary, std::string subtext="");
  } ListItem;
  class List {
    private:
      std::string index_color;
      std::string primary_color;
      std::string subtext_color;
      std::string title;
      bool reversed_index;
      bool numbered;
      std::vector<ListItem> items;
      void push_line(int index, ListItem& item);
    public:
      std::stringstream stream;

      List();
      List(std::string title);
      ~List();
      /*
       * Sets the color of the index
       * [index] <--- primary
       *    [subtext]
       */
      List* IndexColor(std::string);
      /*
       * Sets the primary color
       * [index] primary <----
       *    [subtext]
       */
      List* PrimaryColor(std::string);
      /*
       * Sets the subtext color
       * [index] primary
       *   [subtext] <----
       */
      List* SubTextColor(std::string);
      /*
       * If the list should be numbered
       */
      List* Numbered();
      /*
       * Displays the index in reverse order
       */
      List* ReverseIndexed();
      /*
       * Builds the list
       * intdended to be used like this:
       * std::cout << List->Build() << std::endl;
       */
      std::string Build();
      /*
       * Adds a new line to the list
       * @param item the item to be added
       */
      void pushFront(ListItem item);
      /*
       * Adds a new line to the list
       * @param item the item to be added
       */
      void pushBack(ListItem item);
  };
  template <typename T>
  class Prompt{
    static_assert(
        std::is_same<T,std::string>::value 
        || std::is_same<T, int>::value 
        || std::is_same<T, float>::value 
        || std::is_same<T, double>::value
        || std::is_same<T, bool>::value,
        "Prompt only supports std::string, int, float, and double, bool");
    private:
      std::string prompt;
      std::string color{Ansi::WHITE};
      std::string input;
      T value;
      size_t max_length{0};
      bool print_valid_options{false};
      bool exit_on_failure{false};
      std::function<bool(T)> validator;
      std::vector<T> options;
      void get_input();
      bool is_in_options(T option);
      bool yoink();
      virtual bool has_options();
      virtual bool has_max_length();
      virtual bool has_validator();
    public:
      /*
       * Create a prompt builder
       * @param prompt the prompt to display
       * @param color the color of the prompt
       */
      Prompt(std::string prompt);
      Prompt* Message(std::string prompt);
      /*
       * Adds a vector of <T> options to the prompt
       * @param options the options to add
       * @return this
       */
      Prompt* Options(std::vector<T> options);
      /*
       * Adds an option to the prompt in the form of a <T> type
       * @param option the option to add
       * @return this
       */
      Prompt* AddOption(T option);
      /*
       * Sets the maximum length of the input
       * @param max_length the maximum length of the input
       * @return this
       */
      Prompt* MaxLength(int max_length);
      /*
       * Sets the color of the prompt
       * @param color the color to set
       * @return this
       */
      Prompt* Color(std::string color);
      /*
       * Sets a validator for the input
       * @param validator a function that takes a T and returns a bool
       * @return this
       */
      Prompt* Validator(std::function<bool(T)> validator);
      /*
       * Runs the prompt, asks for input and handles exceptions and validations
       * @return true if the prompt was successful
       */
      Prompt* ExitOnFailure();
      /*
       * Prints the valid options on run
       */
      Prompt* PrintValidOptions();
      /*
       * runs the prompt
       */
      bool Run();
      /*
       * Gets the realized value of the prompt
       * @return the value of the prompt
       */
      T Get();
  };
  template class Prompt<std::string>;
  template class Prompt<int>;
  template class Prompt<float>;
  template class Prompt<double>;
  template class Prompt<bool>;
}
