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
  
  std::vector<Handler> Interface::getSearchHandlers(){
    return {
      Handler{
        .aliases = {"package","p"},
        .flags = {},
        .positional_args = {"query"},
        .docs = "Search for packages",
        .callback = [this]() {
          return Packages::search(this);
        }
      }
    };
  }

  bool Interface::search(){
    std::string query;
    std::string target;
    std::vector<Handler> handlers = getSearchHandlers();

    if(args->operator[]("subcommand").count() == 0){
      std::cout << "No subcommand specified" << std::endl;
      getHelpString("search", handlers);
      return false;
    }

    target = args->operator[]("subcommand").as<std::string>();

    if(args->operator[]("query").count() == 0){
      std::cout << "No query specified" << std::endl;
      getHelpString("search", handlers);
      return false;
    }

    query = args->operator[]("query").as<std::string>();

    for(auto handler : handlers){
      if(std::find(handler.aliases.begin(), handler.aliases.end(), target) != handler.aliases.end()){
        return handler.callback();
      }
    }

    std::cout << "Unknown subcommand: " << target << std::endl;
    getHelpString("search", handlers);

    return false;
  }
}
