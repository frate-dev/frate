#include <Frate/LuaAPI.hpp>



namespace Frate::LuaAPI {
  enum token_type {
    open_bracket,
    close_bracket,
    char_literal,
    specifier
  };
  struct token {
    token_type type;
    std::string value;
  };

  std::string resolveSpecifier(
      std::vector<std::string> &specifiers,
      sol::table table,
      std::string result = "",
      size_t index = 0
      ){

    if(index == specifiers.size()){
      return result;
    }


    for(auto [key, value] : table){
      if(key.as<std::string>() == specifiers[index]){
        //Finding the type of the value, it's a table then we need to recurse
        if(value.is<std::string>()){
          result += value.as<std::string>();
        }else if(value.is<int>()){
          result += std::to_string(value.as<int>());
        }else if(value.is<float>()){
          result += std::to_string(value.as<float>());
        }else if(value.is<bool>()){
          result += std::to_string(value.as<bool>());
        }else if(value.is<sol::table>()){
          result += resolveSpecifier(specifiers, value.as<sol::table>(), result, index+1);
        }else if(value.is<sol::object>()){
          result += resolveSpecifier(specifiers, value.as<sol::table>(), result, index+1);
        }else{
          Utils::error << "LuaAPI Error: Unrecognized type while resolving specifier" << std::endl;
          exit(1);
        }
      }
    }
    return result;
  }
  

  std::string FrateApi::format(const std::string &str, sol::table in_table, sol::this_state s) {

    Utils::warning << str << std::endl;
    std::string result = "";

    sol::state_view lua(s);

    std::string keyword_buffer;

    std::vector<token> tokens;

    char next_char = ' ';

    for(size_t i = 0; i < str.size(); i++){
      keyword_buffer += str[i];
      next_char = str[i+1];
      if(str[i] == '{'){
        i++;
        if(next_char != std::string::npos && next_char == '%'){
          tokens.push_back(token{open_bracket, "{%"});
          keyword_buffer = "";
          i++;
          //Start of the opening bracket
          while(str[i] != '%'){
            if(i == str.size()){
              Utils::error << 
                "LuaAPI Error: while parsing format string no closing brace" 
                << std::endl;
              exit(1);
            }
            //We don't want to add whitespace to the specifier
            if(str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r'){
              keyword_buffer += str[i];
            }
            //If we have a dot then we start looking for the specifier
            if(str[i] == '.'){
              keyword_buffer.pop_back();
              tokens.push_back(token{specifier, keyword_buffer});
              keyword_buffer = "";
            }
            i++;
          }
          i++;
          //Start of the closing bracket
          tokens.push_back(token{specifier, keyword_buffer});
          tokens.push_back(token{close_bracket, "%}"});
          keyword_buffer = "";
        }
      }else{
        //Everything else gets stored as a char literal
        tokens.push_back(token{char_literal, keyword_buffer});
        keyword_buffer = "";
      }
    }
    std::vector<std::string> specifiers;
    for(size_t i = 0; i < tokens.size(); i++){
      //If we have a specifier then we start recording it in the specifier vector
      if(tokens[i].type == specifier){
        specifiers.push_back(tokens[i].value);
        Utils::info << tokens[i].value << std::endl;
        //If we have a close bracket then we resolve the specifier
        //By recursively looking through the table
      }else if(tokens[i].type == close_bracket){
        result += resolveSpecifier(specifiers, in_table);
        specifiers.clear();
        //All char literals get added to the result
      }else if(tokens[i].type == char_literal){
        result += tokens[i].value;
      }
    }
    Utils::info << result << std::endl;
    return result;
  }
}
