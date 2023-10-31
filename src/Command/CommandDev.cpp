#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <thread>
#include <unordered_map>
#include "Command.hpp"

enum class FileStatus { created, modified, erased };

class FileWatcher {
  public:
    std::string path_to_watch;
    std::chrono::duration<int, std::milli> delay;
    bool running_ = true;
    std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
    // Keep a record of files from the base directory and their last modification
    // time
    FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay): path_to_watch{path_to_watch}, delay{delay} {
      for (auto &file :
           std::filesystem::recursive_directory_iterator(path_to_watch)) {
        paths_[file.path().string()] = std::filesystem::last_write_time(file);
      }
    }
    bool contains(const std::string &key) {
      auto el = paths_.find(key);
      return el != paths_.end();
    }
    void start(const std::function<void(std::string, FileStatus)> &action) {
      while (running_) {
        // Wait for "delay" milliseconds
        std::this_thread::sleep_for(delay);

        auto it = paths_.begin();
        while (it != paths_.end()) {
          if (!std::filesystem::exists(it->first)) {
            action(it->first, FileStatus::erased);
            it = paths_.erase(it);
          } else {
            it++;
          }
        }

        // Check if a file was created or modified
        for (auto &file : std::filesystem::recursive_directory_iterator(path_to_watch)) {
          auto current_file_last_write_time =
              std::filesystem::last_write_time(file);

          // File creation
          if (!contains(file.path().string())) {
            paths_[file.path().string()] = current_file_last_write_time;
            action(file.path().string(), FileStatus::created);
            // File modification
          } 
          else {
            if (paths_[file.path().string()] != current_file_last_write_time) {
              paths_[file.path().string()] = current_file_last_write_time;
              action(file.path().string(), FileStatus::modified);
            }
          }
        }
      }
    }
};

namespace Command {
  bool dev(std::shared_ptr<Command::Context> ctx) {
    std::cout << "Starting dev mode" << std::endl;
    std::cout << "Watching for changes in " << ctx->src_dir << std::endl;
    FileWatcher fw{ctx->src_dir, std::chrono::milliseconds(500)};
    fw.start([ctx](std::string path_to_watch, FileStatus status) -> void {
      std::string command = "cmake . && make && ./" + ctx->build_dir + "/" + ctx->project_name;
      switch (status) {
      case FileStatus::created:
        std::cout << "File created: " << path_to_watch << '\n';
        break;
      case FileStatus::modified:
        std::cout << "File modified: " << path_to_watch << '\n';
        break;
      case FileStatus::erased:
        std::cout << "File erased: " << path_to_watch << '\n';
        break;
      default:
        std::cout << "Error! Unknown file status.\n";
      }
      system(command.c_str());
    });
    return true;
  }

}
