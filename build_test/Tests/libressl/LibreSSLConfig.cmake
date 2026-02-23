
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was LibreSSLConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set(LIBRESSL_VERSION 3.8.2)
set_and_check(LIBRESSL_INCLUDE_DIR /home/runner/work/lib3mf/lib3mf/build_test/include)

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/LibreSSL-Crypto.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/LibreSSL-Crypto.cmake")
  set(LIBRESSL_CRYPTO_LIBRARY LibreSSL::Crypto)
  set(LibreSSL_Crypto_FOUND TRUE)
endif()

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/LibreSSL-SSL.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/LibreSSL-SSL.cmake")
  set(LIBRESSL_SSL_LIBRARY LibreSSL::SSL)
  set(LibreSSL_SSL_FOUND TRUE)
endif()

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/LibreSSL-TLS.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/LibreSSL-TLS.cmake")
  set(LIBRESSL_TLS_LIBRARY LibreSSL::TLS)
  set(LibreSSL_TLS_FOUND TRUE)
endif()

set(LIBRESSL_LIBRARIES
  ${LIBRESSL_CRYPTO_LIBRARY}
  ${LIBRESSL_SSL_LIBRARY}
  ${LIBRESSL_TLS_LIBRARY}
)

check_required_components(LibreSSL)

if(DEFINED LibreSSL_FOUND)
	set(LIBRESSL_FOUND ${LibreSSL_FOUND})
else()
	set(LIBRESSL_FOUND TRUE)
endif()
