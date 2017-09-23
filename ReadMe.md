# Requirements

The following requirements are mandatory:
* A recent C++ compiler which fully supports C++11
* [CMake](https://cmake.org)

# How to Build

Note that this program depends on some libraries. `CMake` first download those libraries and then it builds the program.

## How to Build in Linux

You can use the graphical interface of CMake (`cmake-gui`) to generate a `makefile`. If you prefer command line interface, first go to the project path and then execute the following commands:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
For more information about using CMake check out this [link](https://cmake.org/runningcmake/).
## How to Build in Windows

You can use the graphical interface of CMake to generate a project (for example Visual Studio project). If you prefer command line interface, first go to the project path and then execute the following commands:

```
mkdir build
cd build
cmake ..
```

Then you can open the generated project with your favorite IDE (e.g. Visual Studio). For more information about using CMake check out this [link](https://cmake.org/runningcmake/).

# Third-Party Libraries

I've used the following external libraries. You don't need to download them. It's the job of CMake:

* [Asio](https://think-async.com/) for cross-platform socket programming
* [cxxopts](https://github.com/jarro2783/cxxopts) for command line interface
* [LUrlParser](https://github.com/corporateshark/LUrlParser) for parsing URLs