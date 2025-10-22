include_guard(GLOBAL)

# Minimal helper routines shared across CMake entry points.

function(lib3mf_collect_sources OUT_SOURCES OUT_HEADERS)
    set(_sources)
    set(_headers)

    if(USE_INCLUDED_LIBZIP)
        file(GLOB _libzip_common
            "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/libzip/Source/*.c")
        if(UNIX)
            file(GLOB _libzip_platform
                "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/libzip/Source/unix/*.c")
        else()
            file(GLOB _libzip_platform
                "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/libzip/Source/win/*.c")
        endif()
        list(APPEND _sources ${_libzip_common} ${_libzip_platform})
    endif()

    if(USE_INCLUDED_ZLIB)
        file(GLOB _zlib_sources
            "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/zlib/Source/*.c")
        list(APPEND _sources ${_zlib_sources})
    endif()

    file(GLOB_RECURSE _core_sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
    list(APPEND _sources ${_core_sources})

    file(GLOB_RECURSE _core_headers CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")
    list(APPEND _headers ${_core_headers})

    list(REMOVE_DUPLICATES _sources)
    list(REMOVE_DUPLICATES _headers)

    set(${OUT_SOURCES} ${_sources} PARENT_SCOPE)
    set(${OUT_HEADERS} ${_headers} PARENT_SCOPE)
endfunction()

function(lib3mf_add_thirdparty_includes TARGET)
    file(GLOB _vendor_dirs LIST_DIRECTORIES true
        "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/*/Include")
    if(_vendor_dirs)
        list(FILTER _vendor_dirs EXCLUDE REGEX ".*/(zlib|libzip|libressl|cpp-base64)(/|$)")
        if(_vendor_dirs)
            target_include_directories(${TARGET} PRIVATE ${_vendor_dirs})
        endif()
    endif()
    target_include_directories(${TARGET} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Libraries)
endfunction()

function(lib3mf_configure_uuid TARGET)
    if(WIN32)
        target_compile_definitions(${TARGET} PRIVATE GUID_WINDOWS)
        target_link_libraries(${TARGET} PRIVATE ole32 uuid)
    else()
        if(USE_PLATFORM_UUID)
            if(APPLE)
                target_compile_definitions(${TARGET} PRIVATE GUID_CFUUID)
                target_link_libraries(${TARGET} PRIVATE "-framework Foundation")
            else()
                target_compile_definitions(${TARGET} PRIVATE GUID_LIBUUID)
                target_link_libraries(${TARGET} PRIVATE uuid)
            endif()
        else()
            target_compile_definitions(${TARGET} PRIVATE GUID_CUSTOM)
        endif()
    endif()
endfunction()
