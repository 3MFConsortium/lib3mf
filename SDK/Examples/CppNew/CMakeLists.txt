cmake_minimum_required (VERSION 3.5)
project(Examples)
set(CMAKE_CXX_STANDARD 11)

# Determine the platform and set lib3mf_DIR accordingly
if(WIN32)
	# Path for Windows
	set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../lib3mf-2.3.0-Windows/lib/cmake/lib3mf")
elseif(APPLE)
	# Path for macOS (Darwin)
	set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../lib3mf-2.3.0-Darwin/lib/cmake/lib3mf")
else()
	# Assuming the else case is Linux or other Unix-like except macOS
	set(lib3mf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../lib3mf-2.3.0-Linux/lib/cmake/lib3mf")
endif()

# Find package (lib3mf)
find_package(lib3mf REQUIRED Cpp)

add_definitions(-DTEXTURESPATH="${CMAKE_CURRENT_SOURCE_DIR}/../Files/Textures/")

add_executable(Example_ColorCube Source/ColorCube.cpp)
target_link_libraries(Example_ColorCube lib3mf::lib3mf)

add_executable(Example_Components Source/Components.cpp)
target_link_libraries(Example_Components lib3mf::lib3mf)

add_executable(Example_Converter Source/Converter.cpp)
target_link_libraries(Example_Converter lib3mf::lib3mf)

add_executable(Example_Cube Source/Cube.cpp)
target_link_libraries(Example_Cube lib3mf::lib3mf)

add_executable(Example_SecureCube Source/SecureCube.cpp)
target_link_libraries(Example_SecureCube lib3mf::lib3mf)

add_executable(Example_ExtractInfo Source/ExtractInfo.cpp)
target_link_libraries(Example_ExtractInfo lib3mf::lib3mf)

add_executable(Example_TextureCube Source/TextureCube.cpp)
target_link_libraries(Example_TextureCube lib3mf::lib3mf)

add_executable(Example_Slice Source/Slice.cpp)
target_link_libraries(Example_Slice lib3mf::lib3mf)

add_executable(Example_BeamLattice Source/BeamLattice.cpp)
target_link_libraries(Example_BeamLattice lib3mf::lib3mf)

if (${MSVC})
	IF(${CMAKE_VERSION} VERSION_LESS 3.6.3)
		MESSAGE ("Note: You need to manually select a StartUp-project in Visual Studio.")
	ELSE()
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Example_Cube)
	ENDIF()
endif()
