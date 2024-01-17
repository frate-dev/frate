#include <Frate/Command/Actions/Template.hpp>
#include <Frate/Project/Config.hpp>

namespace Frate::Command::Templates {
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()(
        "command", "Command to run",
        cxxopts::value<std::string>()->default_value("help"))(
        "subcommand", "Subcommand to run",
        cxxopts::value<std::string>())("h,help", "Print usage");
    inter->options->help();
    return inter->parse();
  }

  std::vector<Handler> handlers(std::shared_ptr<Interface> inter) {
    (void)inter;
    return {
        Handler{
          .aliases = {"new", "n"},
          .flags = {},
          .callback =
            [](std::shared_ptr<Interface> inter) {
              (void)inter;
            return true;
          }
        },
        Handler{
          .aliases = {"generate", "g"},
          .flags = {},
          .callback =
            [](std::shared_ptr<Interface> inter) {
              (void)inter;
            return true;
          }
        },

    };
  }

  bool run(std::shared_ptr<Interface> inter) {
    options(inter);
    std::vector<Handler> template_handlers = handlers(inter);
    std::string subcommand;
    if (inter->args->count("subcommand") > 0) {

      subcommand = inter->args->operator[]("subcommand").as<std::string>();
    }
    else {
      Utils::error << "No subcommand given" << std::endl;

      inter->getHelpString("template", template_handlers);

      return false;
    }

    inter->pro->load();

    return runCommand(inter, subcommand, template_handlers);

    return true;
  }
} // namespace Frate::Command::Templates
