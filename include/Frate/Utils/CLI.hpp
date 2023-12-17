#pragma once
#include <cmath>
#include <functional>
#include <sstream>
#include <string>
#include <stdint.h>
#include <termcolor/termcolor.hpp>
#include <vector>



namespace Frate::Utils::CLI {
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
  class Prompt{
    private:
      std::string prompt;
      std::string color;
      std::string input;
      std::string default_input;
      int cursor_position{0};
      size_t max_length{0};
      int columns{1};
      int result_limit{static_cast<int>(std::floor(6.9))};
      bool print_valid_options{false};
      bool exit_on_failure{false};
      bool is_bool{false};
      std::function<bool(std::string)> validator;
      std::vector<std::string> options;
      std::vector<std::string> visible_options;
      void get_input();
      bool is_in_options(std::string option);
      bool is_valid();
      void current_input();
      bool has_options();
      bool has_max_length();
      bool has_validator();
      void prompt_prefix(int cursor_position);
    public:
      /*
       * Create a prompt builder
       * @param prompt the prompt to display
       * @param color the color of the prompt
       */
      Prompt(std::string prompt);
      Prompt(std::string prompt, std::string default_input);
      Prompt& Message(std::string prompt);
      /*
       * Adds a vector of <T> options to the prompt
       * @param options the options to add
       * @return this
       */
      Prompt& addOptions(std::vector<std::string> options);
      /*
       * Adds an option to the prompt as a string
       * @param option the option to add
       * @return this
       */
      Prompt& addOption(std::string option);
      /*
       * Adds an option to the prompt as an int
       * @param option the option to add
       * @return this
       */
      Prompt& addOption(int option);
      /*
       * Adds an option to the prompt as a float
       * @param option the option to add
       * @return this
       */
      Prompt& addOption(float option);
      /*
       * Adds an option to the prompt as a size_t
       * @param option the option to add
       * @return this
       */
      Prompt& addOption(size_t option);
      /*
       * Sets the maximum length of the input
       * @param max_length the maximum length of the input
       * @return this
       */
      Prompt& maxLength(int max_length);
      /*
       * Sets the color of the prompt
       * @param color the color to set
       * @return this
       */
      Prompt& setColor(std::string color);

      /*
       * Sets a validator for the input
       * @param validator a function that takes a T and returns a bool
       * @return this
       */
      Prompt& setValidator(std::function<bool(std::string)> validator);
      /*
       * Runs the prompt, asks for input and handles exceptions and validations
       * @return true if the prompt was successful
       */
      Prompt& exitOnFailure();
      /*
       * Prints the valid options on run
       */
      Prompt& printValidOptions();
      /*
       * Sets the prompt to be a boolean prompt
       */
      Prompt& isBool();
      /*
       * runs the prompt
       */
      bool run();
      /*
       * Gets the realized value of the prompt
       * @return the value of the prompt
       */
      template<typename T>
      std::pair<bool,T> get();


      // template<>
      // std::pair<bool,std::string> Get();
      // template<>
      // std::pair<bool,int> Get();
      // template<>
      // std::pair<bool,float> Get();
      // template<>
      // std::pair<bool,bool> Get();
  };
}
