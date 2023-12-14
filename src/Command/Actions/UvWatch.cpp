#include <uv.h>
#include <Frate/Command.hpp>
#include <filesystem>
#include <fstream>


namespace Frate::Command::UvWatch{
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("r,remote-build", "Build server to use", cxxopts::value<bool>()->default_value("false"))
      ("c,args", "command to pass to dev", cxxopts::value<std::vector<std::string>>());
    return inter->parse();
  }
  //TODO Move to  RemoteServer.hpp
  RemoteServer get_current_build_server() {
    std::string current_build_server = std::string(std::getenv("HOME")) +
      "/.config/frate/" +
      "current_build_server.json";
    json current_build_server_json =
      json::parse(std::ifstream(current_build_server));
    if (!current_build_server_json["name"].is_null()) {
      return RemoteServer(
          current_build_server_json["name"].get<std::string>(),
          current_build_server_json["address"].get<std::string>(),
          current_build_server_json["username"].get<std::string>(),
          current_build_server_json["authMethod"].get<std::string>(),
          current_build_server_json["password"].get<std::string>(),
          current_build_server_json["key"].get<std::string>(),
          current_build_server_json["port"].get<int>());
    }
    return RemoteServer();
  }

  bool runCommand(Interface* inter){
    //TODO  CLEAN UP THIS SHIT
    std::string command = "cmake . && make  && " + inter->pro->path.string() + "/" + inter->pro->build_dir + "/" +inter->pro->name;
#ifdef DEBUG
    command = "cd build && cmake . && make  && " + inter->pro->build_dir + "/" +inter->pro->name;
#endif


    bool build_server =inter->args->operator[]("remote-build").as<bool>();
    if (build_server == true) {
      std::string current_build_server = std::string(std::getenv("HOME")) +
        "/.config/frate/" +
        "current_build_server.json";
      inter->pro->build_server = get_current_build_server();
      command =
        "rsync -avh  --exclude-from='.gitignore' --update -e 'ssh -p  " +
        std::to_string(inter->pro->build_server.port) + "' --progress . " +
        inter->pro->build_server.username + "@" +inter->pro->build_server.ip +
        ":/tmp/frate2 && ssh -p " +
        std::to_string(inter->pro->build_server.port) + " " +
        inter->pro->build_server.username + "@" +inter->pro->build_server.ip +
        "  'cd /tmp/frate2 && cmake . && make -j ${nproc} && " + inter->pro->build_dir + "/" +
        inter->pro->name + "'";
    }
    if (inter->args->count("args") != 0) {
      std::cout << "estamos aqui" << std::endl;
      std::vector<std::string> args_vec =
        inter->args->operator[]("args").as<std::vector<std::string>>();
      std::string command_args = args_vec[0];
      if (args_vec.size() > 1) {
        command_args = std::accumulate(
            args_vec.begin(), args_vec.end(), args_vec[0],
            [](std::string a, std::string b) { return a + " " + b; });
        std::cout << "args size is 0" << std::endl;
        return false;
      }

      std::cout << "command_args: " << command_args << std::endl;
      command = "cmake . && make && ./" + inter->pro->build_dir + "/" +
        inter->pro->name + "  " + command_args;
    }
    Utils::hSystem(command);

    return true;
  }

  static int event_triggered = 0;

  void fs_event_callback(uv_fs_event_t *handle, const char *filename, int events, int status) {
    (void)filename, (void)events;
    if (event_triggered) {
      // Ignoring subsequent events
      return;
    }
    Interface* inter = static_cast<Interface*>(handle->data);
    if (status < 0) {
      fprintf(stderr, "Filesystem watch error: %s\n", uv_strerror(status));
      return;
    }

    event_triggered = 1;
    std::cout << "Filesystem change detected, rebuilding..." << std::endl;
    runCommand(inter); std::cout << "\nWatching for changes..." << std::endl;

    // Reset the flag after a delay
    uv_timer_t *timer = new uv_timer_t;
    uv_timer_init(handle->loop, timer);
    timer->data = handle;

    uv_timer_start(timer, [](uv_timer_t *timer) {
        event_triggered = 0;
        uv_close((uv_handle_t *)timer, [](uv_handle_t *handle) {
            free(handle);
            });
        }, 2000, 0); // 1000 ms delay
  }



#ifdef __linux__
  namespace fs = std::filesystem;

  void start_watchers_for_directory(const fs::path& path, uv_loop_t* loop, std::vector<uv_fs_event_t*>& watchers,std::shared_ptr<Interface> inter) {
    for (const auto& entry : fs::directory_iterator(path)) {
      if (fs::is_directory(entry)) {
        uv_fs_event_t* watcher = new uv_fs_event_t{
          .data = inter.get()
        };
        uv_fs_event_init(loop, watcher);
        uv_fs_event_start(watcher, fs_event_callback, entry.path().c_str(), 0);
        watchers.push_back(watcher);
        start_watchers_for_directory(entry.path(), loop, watchers, inter);
      }
    }
  }
#endif

  bool watch(std::shared_ptr<Interface> inter){

    options(inter);
    uv_loop_t *loop = uv_default_loop();
        std::vector<uv_fs_event_t*> watchers;


    uv_fs_event_t fs_event{.data = inter.get()};
    uv_fs_event_init(loop, &fs_event);
#ifdef __linux__
    start_watchers_for_directory(inter->pro->src_dir, loop, watchers, inter);
#endif
    if (uv_fs_event_start(&fs_event, fs_event_callback, (inter->pro->path / inter->pro->src_dir).c_str(), UV_FS_EVENT_RECURSIVE)!=0) {
      fprintf(stderr, "Error starting filesystem watcher.\n");
      return 1;
    }
    std::cout << "Watching for changes in " << inter->pro->src_dir << std::endl;
    return uv_run(loop, UV_RUN_DEFAULT) == 0;
  };
}
