cmake_minimum_required(VERSION 3.5)

project(Example_CDynamic CXX)

# Determine the platform and set lib3mf_DIR accordingly
if(WIN32)
  set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib3mf-$ENV{LIB3MF_VERSION}-Windows/lib/cmake/lib3mf")
  find_package(lib3mf REQUIRED COMPONENTS CDynamic)
elseif(APPLE)
  set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib3mf-$ENV{LIB3MF_VERSION}-Darwin/lib/cmake/lib3mf")
  find_package(lib3mf REQUIRED COMPONENTS CDynamic)
else()
  find_package(lib3mf QUIET COMPONENTS CDynamic)
  if(NOT lib3mf_FOUND)
    set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib3mf-$ENV{LIB3MF_VERSION}-Linux/lib/cmake/lib3mf")
    find_package(lib3mf REQUIRED COMPONENTS CDynamic)
  endif()
endif()

set(CMAKE_CXX_STANDARD 11)

add_executable(Example_CDynamic "${CMAKE_CURRENT_SOURCE_DIR}/Source/Version.cpp")
target_link_libraries(Example_CDynamic lib3mf::lib3mf ${CMAKE_DL_LIBS})

# lib3mf_dynamic.cc provides the C dynamic loader symbols required by this example.
if(DEFINED LIB3MF_CDYNAMIC_ADDITIONAL_SOURCE)
  target_sources(Example_CDynamic PRIVATE "${LIB3MF_CDYNAMIC_ADDITIONAL_SOURCE}")
elseif(DEFINED lib3mf_INCLUDE_DIRS)
  target_sources(Example_CDynamic PRIVATE "${lib3mf_INCLUDE_DIRS}/lib3mf_dynamic.cc")
endif()

if(${MSVC})
  if(${CMAKE_VERSION} VERSION_LESS 3.6.3)
    message("Note: You need to manually select a StartUp-project in Visual Studio.")
  else()
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Example_CDynamic)
  endif()
endif()
