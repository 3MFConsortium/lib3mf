## Building Lib3MF

Call GenerateVS2015.bat (Windows) or GenerateMake.sh (Linux, Mac), or
define other targets by specifying custom [CMake generators](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html).

A new folder “build” is created and contains projects for the IDE/build target you selected.
- Windows:
  1. Build the VS Solution in the build folder
  2. Select the project you want to run as “Start Up Project”
  3. Run/debug this project
- Linux/Mac:
  1. navigate to the “build”-folder 
  2. Call “make” to build the projects
  3. Run/debug a project 

By default, lib3MF is built as a shared (dynamic) library, and tests are enabled.
- The shared library only offers the C-like interface
- The static library only offers both the C-like interface as well as the underlying C++ classes. To enable static library building, add the cmake option `-DBUILD_SHARED_LIBS:BOOL=OFF` either in the scripts in this folder or in the parent project that includes the lib3MF-project.
- To disable the tests, add the cmake option `-DBUILD_TESTING:BOOL=OFF` either in the scripts in this folder or in the parent project that includes the lib3MF-project.
