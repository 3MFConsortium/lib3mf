
if (USE_INCLUDED_LIBZIP)
  file(GLOB LIBZIP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Libraries/libzip/Source/*.c")
  if (UNIX)
	file(GLOB LIBZIP_FILES_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Libraries/libzip/Source/unix/*.c")
  else()
	file(GLOB LIBZIP_FILES_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Libraries/libzip/Source/win/*.c")
  endif()
endif()

if (USE_INCLUDED_ZLIB)
  file(GLOB ZLIB_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Libraries/zlib/Source/*.c")
endif()

file (GLOB CPPBASE64_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Libraries/cpp-base64/Source/*.cpp")

# sources
set(SRCS_PLATFORM
  ${LIBZIP_FILES}
  ${LIBZIP_FILES_PLATFORM}
  Source/Common/Platform/NMR_Platform.cpp
  Source/Common/Platform/NMR_ImportStream_Native.cpp
  Source/Common/Platform/NMR_ExportStream_Native.cpp
  Source/Common/Platform/NMR_ExportStream_ZIP.cpp
)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS Source/*.cpp) # Recursively get all .cpp files, the false is to not follow symlinks

set(SRCS_COMMON
${SRCS_PLATFORM}
${ZLIB_FILES}
${CPPBASE64_FILES}
${SOURCES}
)

# FILELISTVARIABLE must be the name of a variable that holds a list of source-file paths relative to ${CMAKE_CURRENT_SOURCE_DIR}
# FOLDERNAME will be the name of this folder in the IDE
# FOLDERHOME paths in the IDE will be relative to this path
function(sourceGroupByFolder FILELISTVARIABLE FOLDERNAME FOLDERHOME)
  SET(SOURCE_GROUP_DELIMITER "/")
  SET(last_dir "")
  SET(files "")
  FOREACH(file ${${FILELISTVARIABLE}})
    file(RELATIVE_PATH relative_file "${FOLDERHOME}" "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
    GET_FILENAME_COMPONENT(dir "${relative_file}" DIRECTORY)
  IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${FOLDERNAME}${SOURCE_GROUP_DELIMITER}${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${FOLDERNAME}${SOURCE_GROUP_DELIMITER}${last_dir}" FILES ${files})
  ENDIF (files)
endfunction(sourceGroupByFolder)

sourceGroupByFolder(SRCS_COMMON "Source Files" "${CMAKE_CURRENT_SOURCE_DIR}/Source")

file(GLOB_RECURSE HDRS_COMMON RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Include/*.h" "Include/*.hpp")
sourceGroupByFolder(HDRS_COMMON "Header Files" "${CMAKE_CURRENT_SOURCE_DIR}/Include")
