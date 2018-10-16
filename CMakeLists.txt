cmake_minimum_required (VERSION 2.8)

IF (NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 3.1)
  cmake_policy(SET CMP0054 OLD)
ENDIF ()

project (lib3MF)

# Define Version
set(LIB3MF_VERSION_MAJOR 1)				# increase on every backward-compatibility breaking change of the API
set(LIB3MF_VERSION_MINOR 6)				# increase on every backward compatible change of the API
set(LIB3MF_VERSION_MICRO 0)				# increase on on every change that does not alter the API

set(CMAKE_INSTALL_BINDIR bin CACHE PATH "directory for installing binary files")
set(CMAKE_INSTALL_LIBDIR lib CACHE PATH "directory for installing library files")
set(CMAKE_INSTALL_INCLUDEDIR include/lib3mf CACHE PATH "directory for installing header files")

# By default no COM interface is built
# use the cmake option -DNMR_COM_NATIVE:BOOL=ON to build the COM interface
option(NMR_COM_NATIVE "Implement a COM interface" OFF)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  add_definitions(-DBUILD_DLL)
  add_compile_options(-Wall)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # using GCC
  add_definitions(-DBUILD_DLL)
  add_compile_options(-Wall) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
  set(CMAKE_MACOSX_RPATH ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  add_definitions(-DBUILD_DLL -DLIB3MF_EXPORTS /W3)
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} xmllite.lib")
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  if(NMR_COM_NATIVE)
	add_definitions(-DNMR_COM_NATIVE)
  endif()
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Include/Model/COM/NMR_COMVersion.h.in" "${CMAKE_CURRENT_BINARY_DIR}/Include/Model/COM/NMR_COMVersion.h")

include(Source/CMakeLists.txt)

if (WIN32)
	SET(SRCS_MAIN_PLATFORM
	"${SRCS_WIN_MAIN}"
	)
else()
	# Linux and MAC
	SET(SRCS_MAIN_PLATFORM
	"${SRCS_UNIX_MAIN}"
	)
endif()


#########################################################
# Set version info
if(DEFINED ENV{BUILD_NUMBER})
	set(BUILD_NUMBER $ENV{BUILD_NUMBER})
else()
	set(BUILD_NUMBER 0)
endif()

if (WIN32)
	message("BUILD_NUMBER ... " ${BUILD_NUMBER})
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/Release/VersionInfo)
	
	include(generate_product_version)
	generate_product_version(
	   VERSION_FILES_OUTPUTLOCATION
	   NAME "lib3MF"
	   FILE_DESCRIPTION "Lib3MF is the reference-implementation of the 3D Manufacturing Format file standard"
	   BUNDLE "3MF Consortium"
	   VERSION_MAJOR ${LIB3MF_VERSION_MAJOR}
	   VERSION_MINOR ${LIB3MF_VERSION_MINOR}
	   VERSION_PATCH ${LIB3MF_VERSION_MICRO}
	   VERSION_REVISION ${BUILD_NUMBER}
	   PRODUCT_VERSION_MAJOR ${LIB3MF_VERSION_MAJOR}
	   PRODUCT_VERSION_MINOR ${LIB3MF_VERSION_MINOR}
	   PRODUCT_VERSION_PATCH ${LIB3MF_VERSION_MICRO}
	   COMPANY_NAME "3MF Consortium"
	)
	message("VERSION_FILES_OUTPUTLOCATION ... " ${VERSION_FILES_OUTPUTLOCATION})
else()
	set(VERSION_FILES_OUTPUTLOCATION "")
endif()


#########################################################
# Static library (as base for other targets)
add_library(${PROJECT_NAME}_s STATIC ${SRCS_MAIN_PLATFORM} ${SRCS_COMMON})
target_include_directories(${PROJECT_NAME}_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_include_directories(${PROJECT_NAME}_s PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Include)
SET_TARGET_PROPERTIES(${PROJECT_NAME}_s PROPERTIES PREFIX "" IMPORT_PREFIX "")
if (UNIX OR MINGW)
	target_compile_options(${PROJECT_NAME}_s PUBLIC "-fPIC")
	# Uncomment the following to but the version info into the .so-file.
	SET_TARGET_PROPERTIES(${PROJECT_NAME}_s PROPERTIES VERSION "${LIB3MF_VERSION_MAJOR}.${LIB3MF_VERSION_MINOR}.${LIB3MF_VERSION_MICRO}.${BUILD_NUMBER}")
	SET_TARGET_PROPERTIES(${PROJECT_NAME}_s PROPERTIES SOVERSION "${LIB3MF_VERSION_MAJOR}")
	if (NOT APPLE)
		SET_TARGET_PROPERTIES(${PROJECT_NAME}_s PROPERTIES LINK_FLAGS -s)
	endif()
	find_library(LIBUUID_PATH uuid)
	if(NOT LIBUUID_PATH)
		message(FATAL_ERROR "libuuid not found")
	endif()
	target_link_libraries(${PROJECT_NAME}_s ${LIBUUID_PATH})
else()
	# wd4996 masks the deprecated-warning
	target_compile_options(${PROJECT_NAME}_s PUBLIC "$<$<CONFIG:DEBUG>:/Od;/Ob0;/Gm;/sdl;/W3;/WX;/FC;/wd4996>")
	target_compile_options(${PROJECT_NAME}_s PUBLIC "$<$<CONFIG:RELEASE>:/O2;/sdl;/WX;/Oi;/Gy;/FC;/wd4996>")
endif()


#########################################################
# Shared library
add_library(${PROJECT_NAME} SHARED ${SRCS_MAIN_PLATFORM}
	"./Source/Model/COM/NMR_DLLInterfaces.cpp"
	${VERSION_FILES_OUTPUTLOCATION}
)
set(STARTUPPROJECT ${PROJECT_NAME})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "" IMPORT_PREFIX "")
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_s)
if (UNIX OR MINGW)
	# Uncomment the following to but the version info into the .so-file.
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION "${LIB3MF_VERSION_MAJOR}.${LIB3MF_VERSION_MINOR}.${LIB3MF_VERSION_MICRO}.${BUILD_NUMBER}")
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SOVERSION "${LIB3MF_VERSION_MAJOR}")
endif()
install(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(DIRECTORY Include/Model DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY Include/Common DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

#########################################################
if(NOT DEFINED LIB3MF_TESTS)
	set(LIB3MF_TESTS TRUE)
endif()
message("LIB3MF_TESTS ... " ${LIB3MF_TESTS})
if(LIB3MF_TESTS)
	include(CTest)
	add_subdirectory(UnitTests)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	IF(${CMAKE_VERSION} VERSION_LESS 3.6.3)
		MESSAGE ("Note: You need to manually select a StartUp-project in Visual Studio.")
	ELSE()
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${STARTUPPROJECT})
	ENDIF()
endif()
