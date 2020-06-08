cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
	set(LSUFFIX "dll")
	set(LSUFFIXOUT ".dll")
elseif(UNIX AND NOT APPLE)
	set(LSUFFIX "so")
	set(LSUFFIXOUT ".so.2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
else()
	set(LSUFFIX "dylib")
	set(LSUFFIXOUT ".2.dylib")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
endif()

add_definitions( -DTEXTURESPATH="${CMAKE_CURRENT_SOURCE_DIR}/../Files/Textures/" )

project(Examples)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Bindings/Cpp)

if (WIN32)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Lib)
	link_libraries(lib3mf)
else()
	# Unix prefixes the name of the library with "lib" anyway
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Bin)
	link_libraries(3mf)
	link_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

function(CopySharedLibrary MyTarget)
	add_custom_command(TARGET ${MyTarget} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/lib3mf.${LSUFFIX}" $<TARGET_FILE_DIR:${MyTarget}>/lib3mf${LSUFFIXOUT})
endfunction(CopySharedLibrary)

add_executable(Example_ColorCube Source/ColorCube.cpp)
CopySharedLibrary(Example_ColorCube)

add_executable(Example_Components Source/Components.cpp)
CopySharedLibrary(Example_Components)

add_executable(Example_Converter Source/Converter.cpp)
CopySharedLibrary(Example_Converter)

add_executable(Example_Cube Source/Cube.cpp)
CopySharedLibrary(Example_Cube)

add_executable(Example_SecureCube Source/SecureCube.cpp)
CopySharedLibrary(Example_SecureCube)

add_executable(Example_ExtractInfo Source/ExtractInfo.cpp)
CopySharedLibrary(Example_ExtractInfo)

add_executable(Example_TextureCube Source/TextureCube.cpp)
CopySharedLibrary(Example_TextureCube)

add_executable(Example_Slice Source/Slice.cpp)
CopySharedLibrary(Example_Slice)

add_executable(Example_BeamLattice Source/BeamLattice.cpp)
CopySharedLibrary(Example_BeamLattice)

if (${MSVC})
	IF(${CMAKE_VERSION} VERSION_LESS 3.6.3)
		MESSAGE ("Note: You need to manually select a StartUp-project in Visual Studio.")
	ELSE()
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Example_Cube)
	ENDIF()
endif()
