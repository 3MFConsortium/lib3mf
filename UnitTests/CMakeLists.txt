ADD_SUBDIRECTORY (googletest EXCLUDE_FROM_ALL)
enable_testing()

SET_TARGET_PROPERTIES (gtest PROPERTIES FOLDER gtest)
SET_TARGET_PROPERTIES (gtest_main PROPERTIES FOLDER gtest)

SET(gtest_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest")
add_definitions( -DTESTFILESPATH="${CMAKE_CURRENT_SOURCE_DIR}/../TestFiles/")
add_definitions( -DLTESTFILESPATH=L"${CMAKE_CURRENT_SOURCE_DIR}/../TestFiles/")
add_definitions( -DLOUTFILESPATH=L"${CMAKE_BINARY_DIR}/")
add_definitions( -DOUTFILESPATH="${CMAKE_BINARY_DIR}/")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Writer)

# Test the CPP-Bindings of the library
add_subdirectory(CPP_Bindings)

# Test internal classes of the library
# add_subdirectory(CPP)

set(STARTUPPROJECT ${STARTUPPROJECT} PARENT_SCOPE)