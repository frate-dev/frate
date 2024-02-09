#include <Frate/Command.hpp>
#include <Frate/Command/Actions/Update.hpp>
#include <Frate/Command/TemplateIndex.hpp>

namespace Frate::Command::Update {

  using nlohmann::json;

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->add_options()(
        "command",
        "Command to run",
        cxxopts::value<std::string>()->default_value("help"))(
        "subcommand", "Subcommand to run", cxxopts::value<std::string>())(
        "h,help", "Print usage");
    return inter->parse();
  }

  std::vector<Handler> handlers(std::shared_ptr<Interface> inter) {
    return {
      Handler{
        .aliases = {"index"},
        .docs = "Update package index",
        .callback = [](std::shared_ptr<Interface> inter) {
          (void)inter;
          return updateIndex();
        }
      },
      Handler{
        .aliases = {"template-index", "tindex"},
        .docs = "Update template index",
        .callback = &TemplateIndex::update,
      },
    };
  }

  bool run(std::shared_ptr<Interface> inter) {
    options(inter);
    std::vector<Handler> updateHandlers = handlers(inter);
    std::string target = "";
    if (inter->args->count("subcommand") == 0) {
      inter->getHelpString("update", updateHandlers);
      return false;
    }
    target = inter->args->operator[]("subcommand").as<std::string>();

    return runCommand(inter, target, updateHandlers);

    return true;
  }
} // namespace Frate::Command::Update
