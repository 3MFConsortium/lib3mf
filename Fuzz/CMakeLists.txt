macro(fuzzer name)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include)
    add_executable(${name} ${name}.cpp)
    set_target_properties(${name}
            PROPERTIES
            COMPILE_FLAGS "-fsanitize=fuzzer"
            LINK_FLAGS "-fsanitize=fuzzer")
    target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)
    target_link_libraries(${name}
            PRIVATE
            ${PROJECT_NAME}_s
            )
endmacro()

fuzzer(Fuzz_ReadFromBuffer)