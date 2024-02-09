# lib3mfConfig.cmake

# Initial setup for known components and default selection
set(lib3mf_known_components "C" "CDynamic" "Cpp" "CppDynamic")
set(lib3mf_selected_variant "CPPDynamic") # Default variant

# Check if any known component was specified and select it
foreach(comp ${lib3mf_FIND_COMPONENTS})
    if(comp IN_LIST lib3mf_known_components)
        set(lib3mf_selected_variant ${comp})
        break() # Use the first specified known component
    endif()
endforeach()

# Configure paths based on the selected variant
set(lib3mf_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../include/Bindings/${lib3mf_selected_variant}")
set(lib3mf_LIBRARY_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../lib")

# Adjust library file name based on platform
if(WIN32)
    set(lib3mf_LIBRARY "${lib3mf_LIBRARY_DIR}/lib3mf.dll")
    set(lib3mf_LIBRARY_IMPORT "${lib3mf_LIBRARY_DIR}/lib3mf.lib") # For importing symbols
elseif(APPLE)
    set(lib3mf_LIBRARY "${lib3mf_LIBRARY_DIR}/lib3mf.dylib")
else() # Linux and others
    set(lib3mf_LIBRARY "${lib3mf_LIBRARY_DIR}/lib3mf.so")
endif()

# Define the imported target
add_library(lib3mf::lib3mf SHARED IMPORTED)

# Set target properties for include directory and library path
set_target_properties(lib3mf::lib3mf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${lib3mf_INCLUDE_DIR}"
    IMPORTED_LOCATION "${lib3mf_LIBRARY}"
)

if(WIN32)
    set_property(TARGET lib3mf PROPERTY IMPORTED_IMPLIB "${lib3mf_LIBRARY_IMPORT}")
endif()

