There is a bash/batch script that updates the libraries and copies the header and source files to the respective Libraries folder.

## Prerequisites

- Bash shell/ Powershell environment
- Git installed

## Usage
To run the script, execute the following command:

.\update_libs.bat <library_name>

or 

bash update_libs.sh <library_name>

- If no library name is provided, all libraries will be updated.
- If a library name is provided, only that library will be updated.

## Note

- Libzip - In copying header and source files script, a cmake script is called to generate zip_err_str.c and while configuring CMake of Lib3MF, Libzip needs to be configured as well to generate platfrom specific header files config.h and zipconf.h.

