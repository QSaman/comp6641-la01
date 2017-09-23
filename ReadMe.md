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

# Examples

You can see various examples in the following line. For more information about available arguments run `./httpc --help`.

```
$ ./httpc -v get "http://httpbin.org/get?course=networking&assignment=1"
$ ./httpc -v post "http://httpbin.org/post" -h "Content-Type: application/json" -d '{"Assignment": 1}'
$ ./httpc -v post "http://httpbin.org/post" -h "Content-Type: application/json" -f sample_files/post_data_1.txt
$ ./httpc -v post "http://httpbin.org/post" -h "Content-Type: application/json" -f sample_files/post_data_2 .txt
$ ./httpc -v post "http://httpbin.org/post" -h "Content-Type: application/x-www-form-urlencoded" -f sample_files/post_data_3.txt
```

You can find some sample files for `--file` option in `sample_files` directory.

# Optional Tasks

## Support Redirection

I implemented both absolute and relative redirection.

```
$ ./httpc -v --redirect get "http://httpbin.org/redirect-to?url=http://example.com"
$ ./httpc -v --redirect  get "http://httpbin.org/redirect/6"
$ ./httpc -v --redirect --max-redirection=3 get "http://httpbin.org/redirect/6"
$ ./httpc -v --redirect  get "http://httpbin.org/relative-redirect/6"
$ ./httpc -v --redirect  get "http://httpbin.org/absolute-redirect/6"
```

## Writing The Body of The Response to a file

We can store both text and binary files using `-o` flag.

```
$ ./httpc -v  get "http://httpbin.org/image/png" -o image.png
$ ./httpc -v get "http://download.blender.org/peach/bigbuckbunny_movies/big_buck_bunny_480p_surround-fix.avi" -o big_buck_bunny.avi
```

# Third-Party Libraries

I've used the following external libraries. You don't need to download them. It's the job of CMake:

* [Asio](https://think-async.com/) for cross-platform socket programming
* [cxxopts](https://github.com/jarro2783/cxxopts) for command line interface
* [LUrlParser](https://github.com/corporateshark/LUrlParser) for parsing URLs