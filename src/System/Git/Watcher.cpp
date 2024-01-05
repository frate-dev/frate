#include <Frate/System/Watcher.hpp>
#include <filesystem>
#include <uv.h>

namespace Frate::System {

  void UVDirectoryWatcher::add_watcher(const std::string &path) {
    Watcher watcher;
    watcher.handle = std::make_unique<uv_fs_event_t>();
    watcher.path = path;
    watcher.handle->data = this;
    uv_fs_event_init(loop.get(), watcher.handle.get());
    uv_fs_event_start(watcher.handle.get(), fs_event_callback,
                      watcher.path.c_str(), 0);
    watchers.push_back(std::move(watcher));
  };

  void UVDirectoryWatcher::fs_event_callback(uv_fs_event_t *handle,
                                             const char *filename, int events,
                                             int status) {

    auto *watcher = static_cast<UVDirectoryWatcher *>(handle->data);
    // combining the path and the filename
    std::string changed_path =
        (std::string)handle->path + "/" + (std::string)filename;
    // For some reason the filename returns a number, so we need to check if the
    // path exists
    bool path_exists = std::filesystem::exists(changed_path);

    // If we have a callback then call it wit the filename
    if (watcher->callback && !watcher->is_debouncing && path_exists) {

      watcher->callback(changed_path);
      watcher->is_debouncing = true;
      watcher->debounce_timer = std::make_unique<uv_timer_t>();
      // Creating a new timer to wait for the debounce delay
      uv_timer_init(watcher->loop.get(), watcher->debounce_timer.get());

      watcher->debounce_timer->data = watcher;

      uv_timer_start(
          watcher->debounce_timer.get(),
          [](uv_timer_t *handle) {
            // When the timer is done, we can set the debouncing to false
            auto *watcher = static_cast<UVDirectoryWatcher *>(handle->data);
            watcher->is_debouncing = false;
          },
          UVDirectoryWatcher::DEBOUNCE_DELAY, 0);
    }
  }

  UVDirectoryWatcher::~UVDirectoryWatcher() { uv_loop_close(loop.get()); }

  void UVDirectoryWatcher::watch(const std::string &path,
                                 std::function<void(std::string)> callback) {
    this->callback = callback;
    add_watcher(path);

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(path)) {
      if (entry.is_directory()) {
        add_watcher(entry.path().string());
      }
    }
  }

  void UVDirectoryWatcher::run() { uv_run(loop.get(), UV_RUN_DEFAULT); };
} // namespace Frate::System
