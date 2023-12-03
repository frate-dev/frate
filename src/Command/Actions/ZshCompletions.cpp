#include <Frate/Command/Actions/Completions.hpp>
namespace Frate {

  namespace Command::Completions {

    /*
     * options=( // top level options
     *  example: "Example command"
     *  example2: "Example command 2"
     * )
     * sub_options=( // subcommand options
     * example: "Example command"
     * example2: "Example command 2"
     * )
     */
    //TODO: make this work
    void genOptions(std::vector<Handler> top_level_commands,std::string cmd, std::string &script,int level=0){
      std::string indent = "";
      for(int i = 0; i < level; i++){
        indent += "  ";
      }
      script += indent + "options=(\n";
      for(auto &handler : top_level_commands){
        script += indent + "  " + handler.aliases[0] + ": \"" + handler.docs + "\"\n";
      }
      script += indent + ")\n";
      script += indent + "sub_options=(\n";
      for(auto &handler : top_level_commands){
        if(handler.aliases[0] == cmd){
          for(auto &sub_handler : handler.subcommands){
            script += indent + "  " + sub_handler.aliases[0] + ": \"" + sub_handler.docs + "\"\n";
          }
        }
      }
      script += indent + ")\n";
    }

    bool ZshCompletions(Interface* inter){
      std::string script;
      script += "#compdef frate\n";
      script += "function _frate() {\n";

      std::cout << script;
      return true;
    }
// R"(
// #compdef frate
// _frate() {
//       genCompletions(inter->commands,script);
//       script += ")\n";
//       script += "case $words[1] in\n";
//       script += "  frate)\n";
//       script += "    case $words[2] in\n";
//       script += "      ${commands[@]}\n";
//       script += "    esac\n";
//       script += "esac\n";
//       script += "return 0\n";
//       std::cout << script;
//       return true;
//     }
//   }
// }
//
  }
}
