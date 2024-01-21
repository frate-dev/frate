#pragma once
#include <cmath>
#include <functional>
#include <string>
namespace  Frate::Utils::CLI {

  class Prompt {
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
    Prompt &Message(std::string prompt);
    /*
     * Adds a vector of <T> options to the prompt
     * @param options the options to add
     * @return this
     */
    Prompt &addOptions(std::vector<std::string> options);
    /*
     * Adds an option to the prompt as a string
     * @param option the option to add
     * @return this
     */
    Prompt &addOption(std::string option);
    /*
     * Adds an option to the prompt as an int
     * @param option the option to add
     * @return this
     */
    Prompt &addOption(int option);
    /*
     * Adds an option to the prompt as a float
     * @param option the option to add
     * @return this
     */
    Prompt &addOption(float option);
    /*
     * Adds an option to the prompt as a size_t
     * @param option the option to add
     * @return this
     */
    Prompt &addOption(size_t option);
    /*
     * Sets the maximum length of the input
     * @param max_length the maximum length of the input
     * @return this
     */
    Prompt &maxLength(int max_length);
    /*
     * Sets the color of the prompt
     * @param color the color to set
     * @return this
     */
    Prompt &setColor(std::string color);

    /*
     * Sets a validator for the input
     * @param validator a function that takes a T and returns a bool
     * @return this
     */
    Prompt &setValidator(std::function<bool(std::string)> validator);
    /*
     * Runs the prompt, asks for input and handles exceptions and validations
     * @return true if the prompt was successful
     */
    Prompt &exitOnFailure();
    /*
     * Prints the valid options on run
     */
    Prompt &printValidOptions();
    /*
     * Sets the prompt to be a boolean prompt
     */
    Prompt &isBool();
    /*
     * runs the prompt
     */
    void run();
    /*
     * Gets the realized value of the prompt
     * @return the value of the prompt
     */
    template <typename T> T get();
  };
}
