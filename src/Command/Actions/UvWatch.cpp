#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <Frate/Project.hpp>
#include <Frate/Utils/General.hpp>
#include <filesystem>
#include <uv.h>

namespace Frate::Command::UvWatch {

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->add_options()(
        "c,command",
        "Command to run",
        cxxopts::value<std::string>()->default_value("help"))(
        "e,execute",
        "Execute command",
        cxxopts::value<std::string>()->default_value("false"))(
        "r,remote",
        "Build server to use",
        cxxopts::value<bool>()->default_value("false"));
    return inter->parse();
  }

  static bool event_triggered = false;

  class Watch {
  public:
    std::function<bool(std::shared_ptr<Interface>)> callback;
    std::shared_ptr<Interface> data;

    Watch(std::shared_ptr<Interface> data, std::string path) {
      uv_loop_t *loop = uv_default_loop();

      if (loop == nullptr) {
        std::cerr << "Error initializing uv loop" << std::endl;
        return;
      }
      this->loop = loop;
      auto watcher = new uv_fs_event_t;
      this->data = data;
      std::cout << "watching path: " << path << std::endl;
      watcher->data = this;
      watchers.emplace_back(watcher);
      uv_fs_event_init(this->loop, watcher);
      uv_fs_event_start(watcher, fs_event_callback, path.c_str(), 0);
      start_watchers_for_directory(path, loop, watchers);
    };

    void addCallback(std::function<bool(std::shared_ptr<Interface>)> callback) {
      this->callback = callback;
    };

    static void fs_event_callback(uv_fs_event_t *handle,
                                  const char *filename,
                                  int events,
                                  int status) {
      (void)filename, (void)events;
      if (event_triggered) {
        // Ignoring subsequent events
        return;
      }
      std::cout << "get data" << std::endl;

      Watch *watch = static_cast<Watch *>(handle->data);
      std::cout << "got data" << std::endl;

      if (status < 0) {
        fprintf(stderr, "Filesystem watch error: %s\n", uv_strerror(status));
        return;
      }
      event_triggered = true;
      std::cout << "Filesystem change detected, rebuilding..." << std::endl;
      std::cout << "\nWatching for changes..." << std::endl;
      watch->callback(watch->data);

      uv_timer_t timer;
      uv_timer_init(handle->loop, &timer);
      timer.data = handle;

      uv_timer_start(
          &timer, [](uv_timer_t *_) { event_triggered = false; }, 1000, 0);
    }

    void start_watchers_for_directory(const std::filesystem::path &path,
                                      uv_loop_t *loop,
                                      std::vector<uv_fs_event_t *> &watchers) {
      for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_directory(entry)) {
          auto watcher = new uv_fs_event_t;
          watcher->data = this;
          uv_fs_event_init(loop, watcher);
          uv_fs_event_start(
              watcher, fs_event_callback, entry.path().c_str(), 0);
          watchers.emplace_back(watcher);
        }
      }
    }

    bool run() {
      std::cout << "Watching for changes..." << std::endl;
      if (uv_run(loop, UV_RUN_DEFAULT) != 0) {
        std::cerr << "Error running uv loop" << std::endl;
        return false;
      }

      return true;
    };

    ~Watch() {
      int close = uv_loop_close(loop);
      if (close != 0) {
        std::cerr << "Error closing uv loop" << std::endl;
      }
      for (auto watcher : watchers) {
        uv_fs_event_stop(watcher);
      }
      uv_fs_event_stop(&fs_event);
    };

  private:
    uv_loop_t *loop;
    uv_fs_event_t fs_event;
    std::vector<uv_fs_event_t *> watchers;
  };

  std::string remote_build_command(std::shared_ptr<Interface> inter) {
    inter->pro->build_server = inter->config.getBuildServer();

    // Get the destination path from environment variables
    std::string remote_dest_path = std::getenv("REMOTE_DEST_PATH")
                                       ? std::getenv("REMOTE_PATH")
                                       : "/tmp/" + inter->pro->name;
    std::cout << "Remote destination path: " << remote_dest_path << std::endl;
    std::cout << "project: " << json(*inter->pro).dump(2) << std::endl;
    std::cout << "pro->name: " << inter->pro->name << std::endl;
    // Construct the rsync command
    std::string sync_files =
        "rsync -avh --exclude-from='.gitignore' --update -e 'ssh -p " +
        std::to_string(inter->pro->build_server.port) + "' --progress " +
        inter->pro->path.string() + " " + inter->pro->build_server.username +
        "@" + inter->pro->build_server.ip + ":" + remote_dest_path + " ";

    // SSH command to build the project
    std::string ssh = "&& ssh -p " +
                      std::to_string(inter->pro->build_server.port) + " " +
                      inter->pro->build_server.username + "@" +
                      inter->pro->build_server.ip + " ";
    std::string build =
        "'cd " + remote_dest_path + " && cmake . && make -j $(nproc)'";

    // Add option to run a specific command after building, if set
    std::string command;
    if (inter->args->count("execute")) {
      std::string command_to_run =
          inter->args->operator[]("execute").as<std::string>();
      command = ssh + command_to_run;
    }

    return sync_files + ssh + build + command;
  }

  bool runCommand(std::shared_ptr<Interface> inter) {

    std::cout << "Running command" << std::endl;
    std::string command = "cmake . && make  && " + inter->pro->path.string() +
                          "/" + inter->pro->build_dir + "/" + inter->pro->name;
#ifdef DEBUG
    std::cout << "Running in debug mode" << std::endl;
    command = "cd build && cmake . && make  && " + inter->pro->build_dir + "/" +
              inter->pro->name;
#endif

    bool build_server = inter->args->operator[]("remote").as<bool>();
    if (build_server == true) {

      command = remote_build_command(inter);
    }

    if (Utils::hSystem(command) != 0) {
      std::cout << "Error running command: " << command << std::endl;
      exit(1);
    };

    return true;
  }

  bool watch(std::shared_ptr<Interface> inter) {
    options(inter);
    inter->pro->load();
    Generators::Project::refresh(inter->pro);

    Watch watch(inter, inter->pro->path / inter->pro->src_dir);
    watch.addCallback(runCommand);
    watch.run();

    return true;
  };
} // namespace Frate::Command::UvWatch
