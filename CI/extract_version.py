import os
import re

def extract_version_from_cmake():
    cmake_file = 'CMakeLists.txt'

    if not os.path.exists(cmake_file):
        raise FileNotFoundError(f"{cmake_file} not found in the current directory")

    with open(cmake_file, 'r') as file:
        content = file.read()

    major = re.search(r'set\(LIB3MF_VERSION_MAJOR\s+([0-9]+)\)', content)
    minor = re.search(r'set\(LIB3MF_VERSION_MINOR\s+([0-9]+)\)', content)
    micro = re.search(r'set\(LIB3MF_VERSION_MICRO\s+([0-9]+)\)', content)
    prerelease = re.search(r'set\(LIB3MF_VERSION_PRERELEASE\s+"([^"]*)"\)', content)

    if not major or not minor or not micro:
        raise ValueError("Could not find version components in CMakeLists.txt")

    version = f"{major.group(1)}.{minor.group(1)}.{micro.group(1)}"
    if prerelease and prerelease.group(1):
        version += f"-{prerelease.group(1)}"

    return version

if __name__ == "__main__":
    try:
        version = extract_version_from_cmake()
        print(version)
    except Exception as e:
        print(f"Error: {str(e)}")