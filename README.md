<p align="center">
 <img width="250px" src="https://github.com/frate-dev/frate/blob/main/source/images/fratelogo.svg" align="center" alt="GitHub Readme Stats" />
 <h2 align="center">Frate</h2>
 <p align="center">This command line application came to fruition out of the desperate hopes and dreams of a few people looking to softly lighten their suffering while using c++. </p>
</p>

# THIS PROJECT IS STILL IN ALPHA DEVELOPMENT, STUFF MAY BREAK BETWEEN NIGHTLYS


# STAR THE REPO

![](./source/images/average_cpp_learner.png)


## Built with
- tears
- sadness
- neovim
- vscode *yuck*

## Build the binary from source
### Prerequisites

```
gcc >= 13
and
cmake >= 3
```

### Usage
```bash
  new | n <project_name/dir>  [ -d,--defaults ] : Create a new project
  run  [ -m,--build-mode -t,--target ] : Run the project
  help | h : Display help
  ftp : Deletes the entire project F*ck This Project
  add <target>
   ├── packages | p | package <package,...>  [ -l,--latest -m,--mode -t,--target ] 
   │     Add a package to the project
   ├── flag | f <"flag"> 
   │     Add a flag to the project
   ├── lib | l <library-name> 
   │     Add a library to link to your project (Not implemented)
   ├── mode | m <mode-name> 
   │     Adds a build mode to your project
   ├── server | s 
   │     Add a remote server to your local config that you can later build to
   ├── toolchain | t 
   │     Add a crosscompile toolchain to your project
   └── author | a <author-name> 
         Add an author to your project

  set <target>
   ├── license | lc 
   │     Set the project's license
   ├── name | n 
   │     Set the project's name (Not implemented)
   ├── version | ver | v 
   │     Set the project's version (Not implemented)
   └── keywords | kw 
         Set the project's keywords (Not implemented)

  search <target>
   ├── mode | m <mode-name> 
   │     Adds a build mode to your project
   ├── server | s 
   │     Add a remote server to your local config that you can later build to
   ├── toolchain | t 
   │     Add a crosscompile toolchain to your project
   └── author | a <author-name> 
         Add an author to your project

  set <target>
   ├── license | lc 
   │     Set the project's license (Not implemented)
   ├── name | n 
   │     Set the project's name (Not implemented)
   ├── version | ver | v 
   │     Set the project's version (Not implemented)
   └── keywords | kw 
         Set the project's keywords (Not implemented)

  search <target>
   └── package | p <query> 
         Search for packages

  list | ls <target>
   ├── modes | m 
   │     List modes
   ├── servers | remote-servers 
   │     List remote servers
   ├── installed-packages | packages | p  [ -m --mode ] 
   │     List installed packages
   ├── flags | f 
   │     List flags (Not implemented)
   ├── authors | a 
   │     List authors (Not implemented)
   ├── available-targets | at 
   │     List available targets
   └── licenses | l 
         List possible licenses

  remove | rm : remove sub command
  update <target>
   └── index 
         Update package index

  clean  [ -c --cache ] : clean sub command
  build : build sub command
  watch : watches the project for changes
```


### Installation
```bash
git clone https://github.com/frate-dev/frate.git
cd frate
cmake -DCMAKE_BUILD_TYPE=Release ./
sudo make install
```

#### Alternatively, you can simply copy the binary from our [release page](https://github.com/frate-dev/frate/releases) to your path
`/usr/local/bin/`

### Why use Frate
- Easy project creation and maintenance
- Simple default configurations that are easy to extend
- Get the functionality of utilities like cargo, npm , or yarn and use a language you're already familiar with
- Dencentralized package management


### Your friend in the world of C/C++
frate is a command line utility used to expedite building modern c/c++ applications.

- [x] Easy project initialization
- [x] Easy dependency management
- [x] Easy license management
- [x] Multi mode builds
    - [x] Modal dependency management
- [ ] Multi platform builds
- [ ] Extensible json configuration
