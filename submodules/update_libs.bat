@echo off
setlocal EnableDelayedExpansion

set base_path=%~dp0
cd %base_path%

if not exist "..\Libraries\" (mkdir "..\Libraries")

set update_lib=%~1

for %%A in ("fast_float" "") do ( 
    if "%update_lib%"==%%A (
        REM updating the fast_float library
        set lib_name="fast_float"
        set lib_path_src="!base_path!!lib_name!\"

        cd !lib_name!
        set "tag="
        for /f %%i in ('git describe --contains HEAD') do set "tag=%%i"

        cd !base_path!
        REM delete the destination library folder when updating
        RD /S /Q "..\Libraries\!lib_name!"
        set lib_path_dest="..\Libraries\!lib_name!\"
        mkdir !lib_path_dest!

        mkdir "!lib_path_dest!Include"
        copy !lib_path_src!include\!lib_name!\*.h "!lib_path_dest!Include" /Y

        echo "!tag!" >"!lib_path_dest!!lib_name!_!tag!.txt"
    )
)
