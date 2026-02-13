if(NOT DEFINED TARGET_FILE OR TARGET_FILE STREQUAL "")
  message(FATAL_ERROR "VerifyTargetFile.cmake: TARGET_FILE is not set.")
endif()

if(NOT EXISTS "${TARGET_FILE}")
  message(FATAL_ERROR "VerifyTargetFile.cmake: built target file not found: ${TARGET_FILE}")
endif()
