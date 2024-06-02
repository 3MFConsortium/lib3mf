# lib3mfConfig.cmake

if(VCPKG_TOOLCHAIN)
    message("Lib3MF - VCPKG Tool Chain")
    set(LIB3MF_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../..")
else()
    message("Lib3MF - General CMake Tool Chain")
    set(LIB3MF_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../..")
endif()


# Initial setup for known components and default selection
set(lib3mf_known_components "C" "CDynamic" "Cpp" "CppDynamic")
set(lib3mf_selected_variant "Cpp") # Default variant

# Check if any known component was specified and select it
foreach(comp ${lib3mf_FIND_COMPONENTS})
    if(comp IN_LIST lib3mf_known_components)
        set(lib3mf_selected_variant ${comp})
        break() # Use the first specified known component
    endif()
endforeach()

# Configure paths based on the selected variant
set(lib3mf_INCLUDE_DIR "${LIB3MF_ROOT_DIR}/include/Bindings/${lib3mf_selected_variant}")
set(lib3mf_LIBRARY_DIR "${LIB3MF_ROOT_DIR}/lib")
set(lib3mf_BINARY_DIR "${LIB3MF_ROOT_DIR}/bin")

# Adjust library file name based on platform
if(WIN32)
    set(lib3mf_LIBRARY "${lib3mf_BINARY_DIR}/lib3mf.dll")
    set(lib3mf_LIBRARY_IMPORT "${lib3mf_LIBRARY_DIR}/lib3mf.lib") # For importing symbols
elseif(APPLE)
    set(lib3mf_LIBRARY "${lib3mf_LIBRARY_DIR}/lib3mf.dylib")
else() # Linux and others
    set(lib3mf_LIBRARY "${lib3mf_LIBRARY_DIR}/lib3mf.so")
endif()

# Print the chosen variant
message("***********************************")
message("LIB3MF Chosen Variant : " ${lib3mf_selected_variant})
message("***********************************")

# Create a special interface for dynamic loading scenarios
if("${lib3mf_selected_variant}" STREQUAL "CppDynamic" OR "${lib3mf_selected_variant}" STREQUAL "CDynamic")
    if("${lib3mf_selected_variant}" STREQUAL "CDynamic")
        message("*****************************************************************************")
        message("")
        message("       For CDynamic Variant, an additional source called lib3mf_dynamic.cc  ")
        message("                              is required                                   ")
        message("  It is made available using the variable LIB3MF_CDYNAMIC_ADDITIONAL_SOURCE ")
        message("                  You must append this to your sources                      ")
        message("")
        message("*****************************************************************************")
        set(LIB3MF_CDYNAMIC_ADDITIONAL_SOURCE "${LIB3MF_ROOT_DIR}/include/Bindings/CDynamic/lib3mf_dynamic.cc")
    endif()
    add_library(lib3mfdynamic INTERFACE)
    # Now alias lib3mfdynamic to include the namespace
    add_library(lib3mf::lib3mf ALIAS lib3mfdynamic)
    # Set properties and compile definitions for lib3mfdynamic
    target_compile_definitions(lib3mfdynamic INTERFACE
            "LIB3MF_LIBRARY_LOCATION=\"${lib3mf_LIBRARY}\""
    )
    target_include_directories(lib3mfdynamic INTERFACE "${lib3mf_INCLUDE_DIR}")
else()
    # Define the imported target for static linking
    add_library(lib3mf::lib3mf SHARED IMPORTED)
    set_target_properties(lib3mf::lib3mf PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${lib3mf_INCLUDE_DIR}"
            IMPORTED_LOCATION "${lib3mf_LIBRARY}"
    )
    if(WIN32)
        set_property(TARGET lib3mf::lib3mf PROPERTY IMPORTED_IMPLIB "${lib3mf_LIBRARY_IMPORT}")
    endif()

    # Define a custom function to handle library copying
    function(copy_lib3mf_libraries target)
        if(TARGET ${target})
            if(APPLE)
                # On macOS, copy .dylib files, preserving symlinks
                add_custom_command(TARGET ${target} POST_BUILD
                        COMMAND sh -c "cp -P '${lib3mf_LIBRARY_DIR}/lib3mf.dylib'* '$<TARGET_FILE_DIR:${target}>' || true"
                        COMMENT "Copying all lib3mf library files to target directory on MacOS")

            elseif(UNIX)
                # On Unix-like systems (excluding macOS), copy .so files, preserving symlinks
                add_custom_command(TARGET ${target} POST_BUILD
                        COMMAND sh -c "cp -P '${lib3mf_LIBRARY_DIR}/lib3mf.so'* '$<TARGET_FILE_DIR:${target}>'"
                        COMMENT "Copying all lib3mf.so* files to target directory on Linux")
            else()
                # On Windows, directly copy the .dll file without worrying about symlinks
                add_custom_command(TARGET ${target} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${lib3mf_LIBRARY}" "$<TARGET_FILE_DIR:${target}>"
                        COMMENT "Copying lib3mf.dll to target directory on Windows")
            endif()
        else()
            message(WARNING "Target '${target}' not found. lib3mf library was not copied.")
        endif()
    endfunction()
endif()
