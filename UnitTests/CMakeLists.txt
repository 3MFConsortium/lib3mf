ADD_SUBDIRECTORY (googletest EXCLUDE_FROM_ALL)
enable_testing()
	
SET(gtest_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest")
add_definitions( -DTESTFILESPATH="${CMAKE_CURRENT_SOURCE_DIR}/../TestFiles")
add_definitions( -DLTESTFILESPATH=L"${CMAKE_CURRENT_SOURCE_DIR}/../TestFiles")
add_definitions( -DLOUTFILESPATH=L"${CMAKE_BINARY_DIR}/")

# Tests on the C-interface of the library
add_subdirectory(C_Interface)

# Tests on internal classes of the library
# add_subdirectory(CPP)

set(STARTUPPROJECT ${STARTUPPROJECT} PARENT_SCOPE)