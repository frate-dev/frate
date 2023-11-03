<p align="center">
 <img width="250px" src="https://github.com/cmaker-dev/cmaker/blob/main/source/images/cmakerlogo.svg" align="center" alt="GitHub Readme Stats" />
 <h2 align="center">CMaker</h2>
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
clangd >= 18
cmake >= 3
```

### Basic Usage
```bash
$ cmaker init
#Follow prompt to build your project

$ cmaker run
#If everything worked properly, it will build and run the example project
#which is a basic hello world program

$ cmaker add dep <package name>
#Pick one of the results

$ cmaker run
#To rebuild your project with the new dependency
```


### Installation
```bash
git clone https://github.com/cmaker-dev/cmaker.git
cd cmaker
cmake -DCMAKE_BUILD_TYPE=Release ./
sudo make install
```

#### Alternatively, you can simply copy the binary from our [release page](https://github.com/cmaker-dev/cmaker/releases) to your path
`/usr/local/bin/`

### Why use CMaker
- Easy project creation and maintenance
- Simple default configurations that are easy to extend
- Get the functionality of utilities like cargo, npm , or yarn and use a language you're already familiar with
- Dencentralized package management


### Your friend in the world of C/C++
cmake-generator is a command line utility used to expedite building modern c/c++ applications.

- [x] Simple project initialization
- [x] Simple dependency management
- [ ] Multi platform builds
- [ ] Existing cmake project migration
- [ ] Extensible toml configuration
