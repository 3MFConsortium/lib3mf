#########################################################
# Unittests on internal classes of the library

SET(TESTNAME "UnitTestsCPP")

set(SRCS_UNITTEST
./Source/UnitTest_AllTests.cpp
./Source/UnitTest_Geometry.cpp
./Source/UnitTest_Mesh.cpp
./Source/UnitTest_MeshFormat_STL.cpp
./Source/UnitTest_Model.cpp
./Source/UnitTest_ModelFactory.cpp
./Source/UnitTest_ZIP.cpp
)

set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR})
add_executable(${TESTNAME} ${SRCS_UNITTEST})

if (WIN32)
	target_compile_options(${TESTNAME} PUBLIC "$<$<CONFIG:DEBUG>:/Od;/Ob0;/Gm;/sdl;/W3;/WX;/FC;/MTd;/wd4996>")
	target_compile_options(${TESTNAME} PUBLIC "$<$<CONFIG:RELEASE>:/O2;/sdl;/WX;/Oi;/Gy;/FC;/MT;/wd4996>")
endif()

target_include_directories(${TESTNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include ${gtest_SOURCE_DIR}/include)
target_link_libraries(${TESTNAME} ${PROJECT_NAME} gtest gtest_main)
add_dependencies(${TESTNAME} ${PROJECT_NAME})
set_target_properties(${TESTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/")

add_test(${TESTNAME} ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME})
