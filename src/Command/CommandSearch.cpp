#include <CMaker/Command.hpp>
#include <CMaker/Command/Package.hpp>


namespace Command {

  bool OptionsInit::Search(Interface* inter){
    inter->InitHeader();
    inter->options->parse_positional({"command","subcommand","query"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("query", "Arguments to pass to the command", cxxopts::value<std::string>());
    return inter->parse();
  }

  bool Interface::search(){
    std::string query;
    std::string target;
    if(args->operator[]("subcommand").count() > 0){
      target = args->operator[]("subcommand").as<std::string>();
    }
    if(args->operator[]("query").count() > 0){
      query = args->operator[]("query").as<std::string>();
    }else{
      std::cout << "No query provided" << ENDL;
      std::cout << "Usage: cmaker search <query>" << ENDL;
      return false;
    }
    if(target == "package" || target == "p"){
      return Packages::search(this);
    }
    return true;
  }
}
