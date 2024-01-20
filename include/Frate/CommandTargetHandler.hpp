#pragma once
#include <Frate/Command/Exceptions.hpp>
#include <Frate/CommandHandler.hpp>

namespace Frate::Command {
  class TargetHandler {
  public:

    virtual void actionAdd() {}

    virtual void actionRemove() {}

    virtual void actionList() {}

    virtual void actionUpdate() {}

    virtual void actionSet() {}

    virtual void actionGet() {}

    virtual void actionSearch() {}

  };

  class TargetPackageHandler : public TargetHandler, public CommandHandler {
  public:
    TargetPackageHandler(std::shared_ptr<Interface> inter)
        : CommandHandler(inter) {}

    ~TargetPackageHandler() override = default;

    void run() override {

    }
    void registerOptions() override {

    }
    void checkInput() override {

    }
    void actionAdd() override {}
    void actionRemove() override {}
    void actionList() override {}
    void actionUpdate() override {}
  };

  class TargetPackageIndexHandler : public TargetHandler, public CommandHandler {
  public:
    TargetPackageIndexHandler(std::shared_ptr<Interface> inter)
        : CommandHandler(inter) {}

    ~TargetPackageIndexHandler() override = default;

    void run() override {

    }
    void registerOptions() override {

    }
    void checkInput() override {

    }
    void actionUpdate() override {}
  };

} // namespace Frate::Command
