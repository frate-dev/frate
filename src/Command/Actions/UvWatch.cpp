#include <uv.h>
#include <Frate/Interface.hpp>
#include <filesystem>
#include <fstream>
#include <Frate/Project.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp>


namespace Frate::Command::UvWatch{
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->add_options()
      ("c,command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("e,execute", "Execute command", cxxopts::value<std::string>()->default_value("false"))
      ("r,remote-build", "Build server to use", cxxopts::value<bool>()->default_value("false"));
    return inter->parse();
  }
  //TODO Move to  RemoteServer.hpp

  std::optional<json> parseJsonFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return std::nullopt;
    }

    try {
        json j = json::parse(file);
        return j;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error in file " << filePath << ": " << e.what() << std::endl;
        return std::nullopt;
    }
  }

  RemoteServer get_current_build_server() {
    std::string current_build_server = std::string(std::getenv("HOME")) +
      "/.config/frate/" +
      "current_build_server.json";
    std::optional<json> data = parseJsonFile(current_build_server);
    if (!data.has_value()) {
      std::cerr << "No current build server found" << std::endl;
      std::cerr << "Please run frate add server to add a server" << std::endl;
      std::cerr << "Please run frate set server to set a default a server" << std::endl;
      return RemoteServer();
    }
    json current_build_server_json = data.value();
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


  std::string remote_build_command(std::shared_ptr<Interface> inter) {
    inter->pro->build_server = get_current_build_server();

    // Get the destination path from environment variables
    std::string remote_dest_path = std::getenv("REMOTE_DEST_PATH") ? std::getenv("REMOTE_PATH") : "/tmp/" + inter->pro->name;
    std::cout << "Remote destination path: " << remote_dest_path << std::endl;
    std::cout << "project: " << json(*inter->pro).dump(2) << std::endl;
    std::cout << "pro->name: " << inter->pro->name << std::endl;
    // Construct the rsync command
    std::string sync_files = "rsync -avh --exclude-from='.gitignore' --update -e 'ssh -p " +
                             std::to_string(inter->pro->build_server.port) + "' --progress . " +
                             inter->pro->build_server.username + "@" + inter->pro->build_server.ip +
                             ":" + remote_dest_path + " ";

    // SSH command to build the project
    std::string ssh = "&& ssh -p " + std::to_string(inter->pro->build_server.port) + " " +
                            inter->pro->build_server.username + "@" + inter->pro->build_server.ip + " ";
    std::string build =  "'cd " + remote_dest_path + " && cmake . && make -j $(nproc)'";

    // Add option to run a specific command after building, if set
    std::string command;
    if (inter->args->count("execute")) {
        std::string command_to_run = inter->args->operator[]("execute").as<std::string>();
        command =  ssh + command_to_run;
    }

    return sync_files + ssh +  build + command;
  }



  bool runCommand(std::shared_ptr<Interface> inter){
    //TODO  CLEAN UP THIS SHIT
    std::string command = "cmake . && make  && " + inter->pro->path.string() + "/" + inter->pro->build_dir + "/" +inter->pro->name;
    #ifdef DEBUG
      command = "cd build && cmake . && make  && " + inter->pro->build_dir + "/" +inter->pro->name;
    #endif
    if (inter->args->count("command") != 0) {
      std::string command_run;
      
      std::string command_to_run = inter->args->operator[]("command").as<std::string>();
      command = "cmake . && make && ./" + inter->pro->build_dir + "/" +
        inter->pro->name + " " + command_run;
    }

    bool build_server =inter->args->operator[]("remote-build").as<bool>();
    if (build_server == true) {

      command = remote_build_command(inter);
    }

    if (Utils::hSystem(command) != 0){
      std::cout << "Error running command: " << command << std::endl;
      exit(1);
    };

    return true;
  }

  static int event_triggered = 0;

  void fs_event_callback(uv_fs_event_t *handle, const char *filename, int events, int status) {
    (void)filename, (void)events;
    if (event_triggered) {
      // Ignoring subsequent events
      return;
    }
    std::shared_ptr<Interface> inter = *static_cast<std::shared_ptr<Interface>*>(handle->data);

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

  void start_watchers_for_directory(const fs::path& path, uv_loop_t* loop, std::vector<uv_fs_event_t*>& watchers, std::shared_ptr<Interface> inter) {
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
    inter->pro->load();
    Generators::Project::refresh(inter->pro);
    uv_loop_t *loop = uv_default_loop();
    std::vector<uv_fs_event_t*> watchers;


    uv_fs_event_t fs_event{.data = inter.get()};
    uv_fs_event_init(loop, &fs_event);
    if (uv_fs_event_start(&fs_event, fs_event_callback, (inter->pro->path / inter->pro->src_dir).c_str(), UV_FS_EVENT_RECURSIVE)!=0) {
      fprintf(stderr, "Error starting filesystem watcher.\n");
      return 1;
    }
    std::cout << "Watching for changes in " << inter->pro->src_dir << std::endl;

    if (uv_run(loop, UV_RUN_DEFAULT) != 0) {
      std::cerr << "Error running uv loop" << std::endl;
      return false;
    }

    #ifdef __linux__
      start_watchers_for_directory(inter->pro->src_dir, loop, watchers, inter);
    #endif

    int close = uv_loop_close(loop);
    if (close != 0) {
      std::cerr << "Error closing uv loop" << std::endl;
      return false;
    }

    return true; 
  };
}
