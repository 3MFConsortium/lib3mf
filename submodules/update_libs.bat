@echo off

set base_path=%~dp0
echo %base_path%
cd %base_path%

if not exist "..\Libraries\" (mkdir "..\Libraries")


REM updating the cpp-base64 library
set lib_name="cpp-base64"
set lib_path_src="%base_path%%lib_name%\"

cd %lib_name%
set "tag="
for /f %%i in ('git describe --contains HEAD') do set "tag=%%i"

cd %base_path%
REM delete the destination library folder when updating
RD /S /Q "..\Libraries\%lib_name%"
set lib_path_dest="..\Libraries\%lib_name%\"
mkdir %lib_path_dest%

mkdir "%lib_path_dest%Include"
mkdir "%lib_path_dest%Source"
copy %lib_path_src%base*.h "%lib_path_dest%Include" /Y
copy  %lib_path_src%base*.cpp "%lib_path_dest%Source" /Y

echo "%tag%" >"%lib_path_dest%%lib_name%_%tag%.txt"



REM updating the zlib library
set lib_name="zlib"
set lib_path_src="%base_path%%lib_name%\"

cd %lib_name%
set "tag="
for /f %%i in ('git describe --contains HEAD') do set "tag=%%i"

cd %base_path%
REM delete the destination library folder when updating
RD /S /Q "..\Libraries\%lib_name%"
set lib_path_dest="..\Libraries\%lib_name%\"
mkdir %lib_path_dest%

mkdir "%lib_path_dest%Include"
mkdir "%lib_path_dest%Source"
copy %lib_path_src%*.h "%lib_path_dest%Include" /Y
copy  %lib_path_src%*.c "%lib_path_dest%Source" /Y

echo "%tag%" >"%lib_path_dest%%lib_name%_%tag%.txt"