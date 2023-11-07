#include "Command.hpp"


namespace Command {
  bool Interface::search(){
    std::string query;
    if(args->operator[]("query").count() > 0){
      query = args->operator[]("query").as<std::string>();
    }else{
      std::cout << "No query provided" << ENDL;
      std::cout << "Usage: cmaker search <query>" << ENDL;
      return false;
    }
    searchPackage(query);
    return true;
  }
}
