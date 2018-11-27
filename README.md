# lib3mf
[![Build Status](https://travis-ci.org/3MFConsortium/lib3mf.svg?branch=master)](https://travis-ci.org/3MFConsortium/lib3mf)

Lib3MF is a C++ implementation of the 3D Manufacturing Format file standard.

As 3MF shall become an universal 3D Printing standard, its quick adoption is very important. 
This library shall lower all barriers of adoption to any possible user, let it be software 
providers, hardware providers, service providers or middleware tools.
The specification can be downloaded at
[http://3mf.io/specification/](http://3mf.io/specification/)

Its aim is to offer an open source way to integrate 3MF reading and writing capabilities, as well 
as conversion and validation tools for input and output data. The 3MF Library shall provide a 
clean and easy-to-use API to speed up the development and keep integration costs at a minimum.

A lot of energy has been put into keeping it as platform independent as far as possible. For
example, it compiles well with the GCC compiler, but there is some work left to recode a few
platform specific functionalities, which are now covered by the WinRT platform (like XML parsing
and ZIP compression). This will come when time passes.

## Using lib3MF
The current code runs on Windows, Linux and Mac and has very little external dependencies.

### Dependencies
* Unix: [uuid](https://linux.die.net/man/3/uuid)
    * Linux (Ubuntu/Debian): `sudo apt-get install uuid-dev`
    * MAC: `brew install ossp-uuid`

You then have two options:



### A) Build lib3mf yourself
1. Clone the repository\
`git clone https://github.com/3MFConsortium/lib3mf`
2. Enter local checkout\
`cd lib3mf`
3. Initialize the [googletest](https://github.com/google/googletest) submodule by running\
`git submodule update --init`
4. To generate projects for Visual Studio or Unix Makefiles use the scripts in the folder [cmake](cmake):\
Call `GenerateVS2015.bat` (Windows) or `GenerateMake.sh` (Linux, Mac), or
define other targets by specifying custom [CMake generators](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html).\
A new folder `build` is created and contains projects for the IDE/build target you selected.
5. Build the projects:
    * Windows:
      1. Open the VS Solution in the build folder and build it
      2. Select the project you want to run as "Start Up Project"
      3. Run/debug this project in VS
    * Linux/Mac:
      1. navigate to the `build`-folder 
      2. Call `make` to build the projects
      3. Run/debug a project 

### B) Download the precompiled binary SDK of lib3mf
Instead of building lib3MF from source, you can download the compiled shared libarary as part of a minimal SDK from the [official releases](https://github.com/3MFConsortium/lib3mf/releases),
or use the [nightly builds](https://github.com/3MFConsortium/lib3mf-binaries).

## Examples 
For example code, please refer to the examples in the SDK (SDK/Examples). They are also part of the binary SDK from the [official releases](https://github.com/3MFConsortium/lib3mf/releases).

## Documentation
For a more detailed documentation, please take a look into the [PDF](Lib3MF-1.pdf) in the root directory
of this repository.

## License
Lib3MF is licensed under the [simplified BSD license](LICENSE).
