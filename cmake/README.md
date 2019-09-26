## Building lib3mf

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
