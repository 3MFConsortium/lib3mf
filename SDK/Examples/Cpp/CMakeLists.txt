cmake_minimum_required (VERSION 2.6)

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

add_definitions ( -DTEXTURESPATH="${CMAKE_CURRENT_SOURCE_DIR}/../Files/Textures/" )

project (Examples)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Bindings/Cpp)

if (WIN32)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Lib)
	link_libraries(lib3MF)
else()
	# Unix prefixes the name of the library with "lib" anyway
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Bin)
	link_libraries(3MF)
	if(APPLE)
		link_directories(${CMAKE_CURRENT_BINARY_DIR})
	endif()
endif()

function(CopySharedLibrary MyTarget)
	add_custom_command(TARGET ${MyTarget} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/lib3MF.${LSUFFIX}" $<TARGET_FILE_DIR:${MyTarget}>/lib3MF${LSUFFIXOUT})
endfunction(CopySharedLibrary)

set(CPP_BINDING_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../Bindings/Cpp/lib3mf.cpp)

add_executable(Example_ColorCube Source/ColorCube.cpp ${CPP_BINDING_SOURCE_FILES})
CopySharedLibrary(Example_ColorCube)

add_executable(Example_Components Source/Components.cpp ${CPP_BINDING_SOURCE_FILES})
CopySharedLibrary(Example_Components)

add_executable(Example_Converter Source/Converter.cpp ${CPP_BINDING_SOURCE_FILES})
CopySharedLibrary(Example_Converter)

add_executable(Example_Cube Source/Cube.cpp ${CPP_BINDING_SOURCE_FILES})
CopySharedLibrary(Example_Cube)

# add_executable(Example_ExtractInfo Source/ExtractInfo.cpp ${CPP_BINDING_SOURCE_FILES})
# CopySharedLibrary(Example_ExtractInfo)

add_executable(Example_TextureCube Source/TextureCube.cpp ${CPP_BINDING_SOURCE_FILES})
CopySharedLibrary(Example_TextureCube)

# add_executable(Example_Slice Source/Slice.cpp ${CPP_BINDING_SOURCE_FILES})
# CopySharedLibrary(Example_Slice)

# add_executable(Example_BeamLattice Source/BeamLattice.cpp ${CPP_BINDING_SOURCE_FILES})
# CopySharedLibrary(Example_BeamLattice)

if (WIN32)
	IF(${CMAKE_VERSION} VERSION_LESS 3.6.3)
		MESSAGE ("Note: You need to manually select a StartUp-project in Visual Studio.")
	ELSE()
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Example_Cube)
	ENDIF()
endif()
