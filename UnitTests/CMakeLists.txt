#########################################################
# platform independent tests on the C-interface of the library

ADD_SUBDIRECTORY (googletest EXCLUDE_FROM_ALL)
enable_testing()
SET(TEST_CINTERFACE "Test_CInterface")
SET(gtest_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest")
add_definitions( -DTESTFILESPATH="${CMAKE_CURRENT_SOURCE_DIR}/../TestFiles")
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SRCS_UNITTEST
./Source/UnitTest_AllTests.cpp
./Source/UnitTest_Attachments.cpp
./Source/UnitTest_BeamLattice.cpp
./Source/UnitTest_Production.cpp
./Source/UnitTest_ReaderStrictMode.cpp
./Source/UnitTest_Extensions.cpp
./Source/UnitTest_OPC_Issues.cpp
./Source/UnitTest_ReadWrite.cpp
./Source/UnitTest_Slice.cpp
./Source/UnitTest_Textures.cpp
./Source/UnitTest_Thumbnails.cpp
)

set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_executable(${TEST_CINTERFACE} ${SRCS_UNITTEST})

target_include_directories(${TEST_CINTERFACE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
add_dependencies(${TEST_CINTERFACE} ${PROJECT_NAME})

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/TestOutput")

target_link_libraries(${TEST_CINTERFACE} gtest gtest_main ${PROJECT_NAME})
set_target_properties(${TEST_CINTERFACE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/")

add_test(${TEST_CINTERFACE} ${TEST_CINTERFACE})
