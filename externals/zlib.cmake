# build zlib
#----------------------------------------------------------------------
# zlib 
# https://github.com/madler/zlib
#----------------------------------------------------------------------
include (ExternalProject)


set(zlib_url            https://github.com/madler/zlib)
set(zlib_install        ${CMAKE_CURRENT_BINARY_DIR}/external/zlib-install)
set(zlib_tag            v1.3)
set(zlib_prefix         ${CMAKE_CURRENT_BINARY_DIR}/external/zlib-prefix)

if(WIN32)
    set(_ZLIB_LIB_NAME "zlib")
    set(_ZLIB_STATIC_LIB_NAME "zlibstatic")
else()
    set(_ZLIB_LIB_NAME "z")
    set(_ZLIB_STATIC_LIB_NAME "z")
endif()

# Windows need the "d" suffix at the end.
if(WIN32 AND CMAKE_BUILD_TYPE MATCHES "Debug")
    message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
    set(_ZLIB_LIB_SUFFIX "d")
endif()


set(ZLIB_INCLUDE_DIRS ${zlib_install}/include)
set(ZLIB_LIBRARIES
    "${zlib_install}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${_ZLIB_STATIC_LIB_NAME}${_ZLIB_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")



ExternalProject_Add( zlibsource
    PREFIX ${zlib_prefix}
    GIT_REPOSITORY ${zlib_url}
    GIT_TAG ${zlib_tag}
    INSTALL_DIR ${zlib_install}
    BUILD_IN_SOURCE 1
    EXCLUDE_FROM_ALL 1
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${zlib_install}
	LOG_DOWNLOAD 1
    LOG_INSTALL 1
)

