import argparse
import urllib.request
import json
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

    core_version = version
    core_version_with_pre_release = version
    core_version_with_modified_pre_release = version
    if prerelease and prerelease.group(1):
        core_version_with_pre_release += ".{0}".format(prerelease.group(1))
        core_version_with_modified_pre_release += ".0"

    # Return all three values as a JSON object
    return json.dumps({
        "core_version": core_version,
        "core_version_with_pre_release": core_version_with_pre_release,
        "core_version_with_modified_pre_release": core_version_with_modified_pre_release
    })

def get_integration_sdk_url(index):
    url = "https://api.github.com/repos/3MFConsortium/test_suites/releases"
    
    try:
        with urllib.request.urlopen(url) as response:
            data = response.read().decode('utf-8')
        
        releases = json.loads(data)
        selected_release = releases[index]  # Select the release by index
        for asset in selected_release['assets']:
            asset_name = str(asset['name'])
            if asset_name.startswith("3MF_Conformance_Test_Suites") and asset_name.endswith(".zip"):
                return asset['browser_download_url']
        return None
    except Exception as e:
        print(f"Error fetching the SDK URL: {e}")
        return None    

def get_sdk_url(index):
    url = "https://api.github.com/repos/3MFConsortium/lib3mf/releases"
    
    try:
        with urllib.request.urlopen(url) as response:
            data = response.read().decode('utf-8')
        
        releases = json.loads(data)
        selected_release = releases[index]  # Select the release by index
        for asset in selected_release['assets']:
            asset_name = str(asset['name']) 
            if asset_name.startswith("lib3mf_sdk") and asset_name.endswith(".zip"):
                return asset['browser_download_url']
        return None
    except Exception as e:
        print(f"Error fetching the SDK URL: {e}")
        return None

def main():
    parser = argparse.ArgumentParser(description="Multi-utility script for CI/CD.")
    
    subparsers = parser.add_subparsers(dest='command')
    
    # Subparser for the fetch-integration-test-sdk-url command
    parser_fetch_sdk_url = subparsers.add_parser('fetch-integration-test-sdk-url', help='Fetch the SDK URL for a specific integration test release index.')
    parser_fetch_sdk_url.add_argument('index', type=int, help='Index of the release (0 for latest, 1 for second latest, etc.)')

    # Subparser for the fetch-sdk-url command
    parser_fetch_sdk_url = subparsers.add_parser('fetch-sdk-url', help='Fetch the SDK URL for a specific release index.')
    parser_fetch_sdk_url.add_argument('index', type=int, help='Index of the release (0 for latest, 1 for second latest, etc.)')

    # Subparser for the extract-version command
    parser_extract_version = subparsers.add_parser('extract-version', help='Extract the version from CMakeLists.txt.')

    args = parser.parse_args()

    if args.command == 'fetch-sdk-url':
        url = get_sdk_url(args.index)
        if url:
            print(url)
        else:
            print("FAIL")
    elif args.command == 'fetch-integration-test-sdk-url':
        url = get_integration_sdk_url(args.index)
        if url:
            print(url)
        else:
            print("FAIL")
    elif args.command == 'extract-version':
        try:
            version = extract_version_from_cmake()
            print(version)
        except Exception as e:
            print("FAIL")
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
