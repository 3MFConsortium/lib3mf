cmake_minimum_required (VERSION 3.5)
project(Examples)
set(CMAKE_CXX_STANDARD 11)

# read the version of the 3MF Library from environment variable LIB3MF_VERSION
if (DEFINED ENV{LIB3MF_VERSION})
	set(LIB3MF_VERSION $ENV{LIB3MF_VERSION})
else()
	set(LIB3MF_VERSION "2.3.1")
endif()

# Determine the platform and set lib3mf_DIR accordingly
if(WIN32)
	# Path for Windows
	set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib3mf-${LIB3MF_VERSION}-Windows/lib/cmake/lib3mf")
	find_package(lib3mf REQUIRED COMPONENTS Cpp)
elseif(APPLE)
	# Path for macOS (Darwin)
	set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib3mf-${LIB3MF_VERSION}-Darwin/lib/cmake/lib3mf")
	find_package(lib3mf REQUIRED COMPONENTS Cpp)
else()
	# Path for Linux (Here we check twice to test for Debian / RPM packages properly)
	find_package(lib3mf QUIET COMPONENTS Cpp)
	# Check if the package was not found
	if(NOT lib3mf_FOUND)
		# lib3mf not found, so set lib3mf_DIR to the fallback directory
		set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib3mf-${LIB3MF_VERSION}-Linux/lib/cmake/lib3mf")
		# Find package (lib3mf)
		find_package(lib3mf REQUIRED COMPONENTS Cpp)
	endif()
endif()

add_definitions(-DTEXTURESPATH="${CMAKE_CURRENT_SOURCE_DIR}/../Files/Textures/")

add_executable(Example_ColorCube Source/ColorCube.cpp)
target_link_libraries(Example_ColorCube lib3mf::lib3mf)
copy_lib3mf_libraries(Example_ColorCube)

add_executable(Example_Components Source/Components.cpp)
target_link_libraries(Example_Components lib3mf::lib3mf)
copy_lib3mf_libraries(Example_Components)

add_executable(Example_Converter Source/Converter.cpp)
target_link_libraries(Example_Converter lib3mf::lib3mf)
copy_lib3mf_libraries(Example_Converter)

add_executable(Example_Cube Source/Cube.cpp)
target_link_libraries(Example_Cube lib3mf::lib3mf)
copy_lib3mf_libraries(Example_Cube)

add_executable(Example_SecureCube Source/SecureCube.cpp)
target_link_libraries(Example_SecureCube lib3mf::lib3mf)
copy_lib3mf_libraries(Example_SecureCube)

add_executable(Example_ExtractInfo Source/ExtractInfo.cpp)
target_link_libraries(Example_ExtractInfo lib3mf::lib3mf)
copy_lib3mf_libraries(Example_ExtractInfo)

add_executable(Example_TextureCube Source/TextureCube.cpp)
target_link_libraries(Example_TextureCube lib3mf::lib3mf)
copy_lib3mf_libraries(Example_TextureCube)

add_executable(Example_Slice Source/Slice.cpp)
target_link_libraries(Example_Slice lib3mf::lib3mf)
copy_lib3mf_libraries(Example_Slice)

add_executable(Example_BeamLattice Source/BeamLattice.cpp)
target_link_libraries(Example_BeamLattice lib3mf::lib3mf)
copy_lib3mf_libraries(Example_BeamLattice)

if (${MSVC})
	IF(${CMAKE_VERSION} VERSION_LESS 3.6.3)
		MESSAGE ("Note: You need to manually select a StartUp-project in Visual Studio.")
	ELSE()
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Example_Cube)
	ENDIF()
endif()
