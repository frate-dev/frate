#include "Command.hpp"
#include <functional>
#include <iostream>
#include <sys/epoll.h>
#include <sys/inotify.h>

namespace Command {

  size_t walk(int epoll_fd){
    std::vector<int> filedescriptors;
    for(auto filetoken:std::filesystem::recursive_directory_iterator("./src")){
      if(std::filesystem::is_directory(filetoken.path())){
        std::cout << filetoken.path() << std::endl;
        int temp_fd = inotify_init();
        if(temp_fd < 0){
          std::cout << "Error creating inotify instance" << std::endl;
          exit(1);
        }
        int err = inotify_add_watch(temp_fd, filetoken.path().c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
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


  void watcher(const std::function<void()> &changeCallback) {
    int inotify_fd = inotify_init();
    int epoll_fd = epoll_create(1);

    // Add inotify file descriptor to the epoll instance
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = inotify_fd;
    // TODO: Add recursive directory watching
    size_t watch_desc = inotify_add_watch(inotify_fd, "./src", IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    if (watch_desc < 0) {
      std::cout << "Error adding watch" << std::endl;
      return;
    }
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, inotify_fd, &ev);
    size_t num_dirs = walk(epoll_fd);



    // Add the directory you want to watch


    // Now, you can use epoll to wait for events
    struct epoll_event events[1];
    while (true) {
      std::cout << "\nWaiting for changes...\n" << std::endl;
      int num_events = epoll_wait(epoll_fd, events, 1, -1);
      std::cout << "\nChange detected!\n" << std::endl;
      std::vector<std::string> dirs;
      for(auto filetoken:std::filesystem::recursive_directory_iterator("./src")){
          if(std::filesystem::is_directory(filetoken.path())){
            dirs.push_back(filetoken.path());
          }
      }
      if (dirs.size() != num_dirs){
        std::cout << "\nDirectory change detected\n" << std::endl;
        num_dirs = walk(epoll_fd);
      }
      if (num_events > 0) {
        char buffer[1024];
        while (read(events[0].data.fd,&buffer, sizeof(buffer))<0) {

        }
        std::cout << buffer << std::endl;
        changeCallback();

      }
    }


    // Don't forget to clean up your file descriptors when you're done
    close(watch_desc);
    close(inotify_fd);
    close(epoll_fd);
  }

  bool Interface::dev() {
    // This is where you call the watcher function and provide a lambda for the
    // callback
    watcher([this]() {
        std::string command =
        "cmake . && make && ./" + ctx->build_dir + "/" + ctx->project_name;
        system(command.c_str());

        // Call your recompilation command or any other action you want
        });
    return true;
  }
} // namespace Command

