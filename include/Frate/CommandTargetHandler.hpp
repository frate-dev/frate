#pragma once
#include <Frate/CommandHandler.hpp>

namespace Frate::Command {

  class TargetPackageHandler : public CommandHandler {
  public:
    TargetPackageHandler(std::shared_ptr<Interface> inter)
        : CommandHandler(inter) {}

    ~TargetPackageHandler() override = default;

    void run() override;
    void registerOptions() override;
    void checkInput() override;
    void add();
    void remove();
    void list();
    void update();
  };

  class TargetPackageIndexHandler : public CommandHandler {
  public:
    TargetPackageIndexHandler(std::shared_ptr<Interface> inter)
        : CommandHandler(inter) {
          implemented = false;
        }

    ~TargetPackageIndexHandler() override = default;

    void run() override {

    }
    void registerOptions() override {

    }
    void checkInput() override {
    }

    void update();
  };

} // namespace Frate::Command
