#include "Command.hpp"
#include <functional>
#include <iostream>
#ifdef __linux__
#include <sys/epoll.h>
#include <sys/inotify.h>
#include "../Utils/General.hpp"

namespace Command {

  size_t walk(int epoll_fd, std::string path){
    std::vector<int> filedescriptors;

    for(auto filetoken:std::filesystem::recursive_directory_iterator(path)){
      if(std::filesystem::is_directory(filetoken.path())){
        std::cout << filetoken.path() << std::endl;
        int temp_fd = inotify_init();
        if(temp_fd < 0){
          std::cout << "Error creating inotify instance" << std::endl;
          exit(1);
        }
        int err = inotify_add_watch(temp_fd, filetoken.path().c_str(), IN_MODIFY | IN_CREATE | IN_DELETE );
        if(err < 0){
          std::cout << "Error adding watch" << std::endl;
          exit(1);
        }
        filedescriptors.push_back(temp_fd);
      }
    };


    for(int fd: filedescriptors){
      struct epoll_event temp_ev;
      temp_ev.data.fd = fd;
      temp_ev.events = EPOLLIN;
      epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &temp_ev);
    }
    return filedescriptors.size();

  }


  void watcher(const std::function<void()> &changeCallback, std::string path) {
    int inotify_fd = inotify_init();
    int epoll_fd = epoll_create(1);

    // Add inotify file descriptor to the epoll instance
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = inotify_fd;
    // TODO: Add recursive directory watching

    size_t watch_desc = inotify_add_watch(inotify_fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE );
    if (watch_desc < 0) {
      std::cout << "Error adding watch" << std::endl;
      return;
    }
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, inotify_fd, &ev);
    size_t filedescriptors = walk(epoll_fd, path);



    // Add the directory you want to watch


    // Now, you can use epoll to wait for events
    struct epoll_event events[1];
    while (true) {
      std::cout << "\nWaiting for changes...\n" << std::endl;
      int num_events = epoll_wait(epoll_fd, events, 1, -1);
      std::cout << "\nChange detected!\n" << std::endl;
      std::vector<std::string> dirs;
#ifdef DEBUG
      for(auto filetoken:std::filesystem::recursive_directory_iterator(path)){
#else
      for(auto filetoken:std::filesystem::recursive_directory_iterator(path)){
#endif
          if(std::filesystem::is_directory(filetoken.path())){
            dirs.push_back(filetoken.path());
          }
      }
      if (dirs.size() != filedescriptors){
        std::cout << "\nDirectory change detected\n" << std::endl;
        filedescriptors= walk(epoll_fd, path.c_str());
      }
      if (num_events > 0) {
        char buffer[1024];
        while (read(events[0].data.fd,&buffer, sizeof(buffer))<0) {

        }
        std::cout << buffer << std::endl;

      }
      changeCallback();
    }


    // Don't forget to clean up your file descriptors when you're done
    close(watch_desc);
    close(inotify_fd);
    close(epoll_fd);
  }

  bool Interface::watch() {
    // This is where you call the watcher function and provide a lambda for the
    // callback

    
#ifdef DEBUG
    const std::string path = "./build/src";
#else
    const std::string path = "./src";

#endif

    watcher([this]() {
  #ifdef DEBUG
        //TODO: please use project path
        std::string command = "cmake ./build/ && ./build/make && ./build/" + pro->build_dir + "/" + pro->project_name;
        if (args->count("args") != 0) {
          std::vector<std::string> args_vec =  args->operator[]("args").as<std::vector<std::string>>();
          std::string command_args = std::accumulate(
            args_vec.begin(), args_vec.end(), args_vec[0],
            [](std::string a, std::string b) { return a + " " + b; }
          );
        command = "rsync -avh  --exclude-from='.gitignore' --update -e 'ssh -p  " + std::to_string(pro->build_server.port)  + "' --progress . " 
          + pro->build_server.username + "@" + pro->build_server.ip 
          +  ":/tmp/cmaker && ssh -p " + std::to_string(pro->build_server.port)  + " " +  pro->build_server.username + "@" + pro->build_server.ip  
          + "  'cd /tmp/cmaker && cmake . && make && ./build/new'"; 
           //command = "cmake ./build/ && make && ./build/" + pro->build_dir + "/" + pro->project_name + "  " + command_args;
            
        }
        bool  build_server=args->operator[]("remote-build").as<bool>();
        if (build_server == true) {
          std::string current_build_server= std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "current_build_server.json";
          json current_build_server_json = json::parse(std::ifstream(current_build_server));
          if (!current_build_server_json["name"].is_null()) {
            pro->build_server = BuildServer(
              current_build_server_json["name"].get<std::string>(),
              current_build_server_json["address"].get<std::string>(), 
              current_build_server_json["username"].get<std::string>(),
              current_build_server_json["authMethod"].get<std::string>(),
              current_build_server_json["password"].get<std::string>(),
              current_build_server_json["key"].get<std::string>(),
              current_build_server_json["port"].get<int>()
            );
          }
          command = "rsync -avh  --exclude-from='.gitignore' --update -e 'ssh -p  " + std::to_string(pro->build_server.port)  + "' --progress . " 
            + pro->build_server.username + "@" + pro->build_server.ip 
            +  ":/tmp/cmaker && ssh -p " + std::to_string(pro->build_server.port)  + " " +  pro->build_server.username + "@" + pro->build_server.ip  
            + "  'cd /tmp/cmaker && cmake . && make && ./build/" + pro->project_name + "'"; 

        }

  #else
        std::string command = "cmake . && make  && ./" + pro->build_dir + "/" + pro->project_name;

        bool  build_server=args->operator[]("remote-build").as<bool>();
        if (build_server == true) {
          std::string current_build_server= std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "current_build_server.json";
          json current_build_server_json = json::parse(std::ifstream(current_build_server));
          if (!current_build_server_json["name"].is_null()) {
            pro->build_server = BuildServer(
              current_build_server_json["name"].get<std::string>(),
              current_build_server_json["address"].get<std::string>(), 
              current_build_server_json["username"].get<std::string>(),
              current_build_server_json["authMethod"].get<std::string>(),
              current_build_server_json["password"].get<std::string>(),
              current_build_server_json["key"].get<std::string>(),
              current_build_server_json["port"].get<int>()
            );
          }
          command = "rsync -avh  --exclude-from='.gitignore' --update -e 'ssh -p  " + std::to_string(pro->build_server.port)  + "' --progress . " 
            + pro->build_server.username + "@" + pro->build_server.ip 
            +  ":/tmp/cmaker && ssh -p " + std::to_string(pro->build_server.port)  + " " +  pro->build_server.username + "@" + pro->build_server.ip  
            + "  'cd /tmp/cmaker && cmake . && make -j ${nproc} && ./build/" + pro->project_name + "'"; 

        }
//        else{
//          std::string build_servers = std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "build_servers.json";
//          json build_servers_json = json::parse(std::ifstream(build_servers));
//          std::string build_server =  args->operator[]("server").as<std::string>();
//          for (auto &bserver: build_servers_json){
//            if (bserver["name"].get<std::string>() == build_server){
//              pro->build_server = BuildServer(
//                bserver["name"].get<std::string>(),
//                bserver["authMethod"].get<std::string>(),
//                bserver["password"].get<std::string>(),
//                bserver["key"].get<std::string>(),
//                bserver["port"].get<int>()
//
//              );
//              command = "rsync -avh  --exclude-from='.gitignore' --update -e 'ssh -p  " + std::to_string(pro->build_server.port)  + "' --progress . " 
//                + pro->build_server.username + "@" + pro->build_server.ip 
//                +  ":/tmp/cmaker && ssh -p " + std::to_string(pro->build_server.port)  + " " +  pro->build_server.username + "@" + pro->build_server.ip  
//                + "  'cd /tmp/cmaker && cmake . && make && ./build/" + pro->project_name + "'"; 
//              break;
//            }
//          }
//        }
        if (args->count("args") != 0) {
          std::cout << "estamos aqui" << std::endl;
          std::vector<std::string> args_vec =  args->operator[]("args").as<std::vector<std::string>>();
          std::string command_args = args_vec[0];
          if (args_vec.size() > 1) {
            command_args = std::accumulate(
              args_vec.begin(), args_vec.end(), args_vec[0],
              [](std::string a, std::string b) { return a + " " + b; }
          );
            std::cout << "args size is 0" << std::endl;
            return;
          }

          std::cout << "command_args: " << command_args << std::endl;
          command = "cmake . && make && ./" + pro->build_dir + "/" + pro->project_name + "  " + command_args;

        }
  #endif
        std::cout << "Running command: " << command << std::endl;
        int success = Utils::hSystem(command.c_str());
        if (success != 0) {
          std::cout << "Error running project" << std::endl;
        }

        // Call your recompilation command or any other action you want
        },path);
    return true;
  }
} // namespace Command
#endif
