#pragma once
#include <Frate/Interface.hpp>

namespace Frate::Command::Add {
  bool options(std::shared_ptr<Interface> inter);
  bool run(std::shared_ptr<Interface> inter);
  std::vector<Handler> handlers(std::shared_ptr<Interface> inter);
} // namespace Frate::Command::Add

// namespace Frate::Command {
//   class AddHandler : public CommandHandler {
//   public:
//     AddHandler(std::shared_ptr<Interface> inter) : CommandHandler(inter) {
//       this->addSubcommand(std::make_unique<AddPackageHandler>(inter));
//       this->addOption({"h", "help", cxxopts::value<bool>()->default_value("false")});
//     }
// 
//     void registerOptions() override;
//     void run() override;
//     void checkInput() override;
//   };
// } // namespace Frate::Command
