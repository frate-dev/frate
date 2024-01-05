#pragma once
#include <Frate/Interface.hpp>
#include <functional>
#include <memory>
#include <uv.h>

namespace Frate::System {
  using Frate::Command::Interface;

  class UVDirectoryWatcher {
  private:
    static const time_t DEBOUNCE_DELAY = 1000;

    bool is_debouncing{false};

    struct Watcher {
      std::unique_ptr<uv_fs_event_t> handle;
      std::string path;
      std::unique_ptr<uv_timer_t> debounce_timer;
    };

    std::unique_ptr<uv_loop_t> loop;
    std::unique_ptr<uv_timer_t> debounce_timer;
    std::vector<Watcher> watchers;
    std::function<void(std::string)> callback;

    void add_watcher(const std::string &path);
    static void fs_event_callback(uv_fs_event_t *handle, const char *filename,
                                  int events, int status);

    // static void debounce_timer_callback(uv_timer_t *handle);

  public:
    UVDirectoryWatcher() : loop(std::make_unique<uv_loop_t>()) {
      uv_loop_init(loop.get());
    };

    ~UVDirectoryWatcher();
    void watch(const std::string &path,
               std::function<void(std::string)> callback);
    void run();
  };
} // namespace Frate::System
