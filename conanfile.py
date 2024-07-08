from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout

class Lib3MFConan(ConanFile):
    name = "lib3mf"
    version = "2.3.2"  # Update to the current version
    license = "MIT"
    url = "https://github.com/3MFConsortium/lib3mf"
    description = "Lib3MF is an open-source C++ implementation of the 3D Manufacturing Format file standard."
    topics = ("3mf", "3d-printing", "file-format")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}
    generators = "CMakeToolchain", "CMakeDeps"
    exports_sources = "*"

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={"LIB3MF_TESTS": "OFF"})
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["lib3mf"]
