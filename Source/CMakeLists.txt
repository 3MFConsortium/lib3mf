include_guard(GLOBAL)

if(NOT COMMAND lib3mf_collect_sources)
    include(${CMAKE_SOURCE_DIR}/cmake/lib3mfProjectAPI.cmake)
endif()

lib3mf_collect_sources(SRCS_COMMON HDRS_COMMON)
