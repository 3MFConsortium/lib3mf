del ..\AutomaticComponentToolkit\act.win32.exe 
del ..\AutomaticComponentToolkit\act.win64.exe
del ..\AutomaticComponentToolkit\act.linux32 
del ..\AutomaticComponentToolkit\act.linux64 
del ..\AutomaticComponentToolkit\act.linux.arm 
del ..\AutomaticComponentToolkit\act.linux64.arm 
del ..\AutomaticComponentToolkit\act.darwin

cd AutomaticComponentToolkit
call Build\Build.bat

copy act.win32.exe ..\..\AutomaticComponentToolkit\ /Y
copy act.win64.exe ..\..\AutomaticComponentToolkit\ /Y
copy act.linux32 ..\..\AutomaticComponentToolkit\ /Y
copy act.linux64 ..\..\AutomaticComponentToolkit\ /Y
copy act.linux32.arm ..\..\AutomaticComponentToolkit\ /Y
copy act.linux64.arm ..\..\AutomaticComponentToolkit\ /Y
copy act.darwin ..\..\AutomaticComponentToolkit\ /Y

git rev-parse --verify HEAD >"..\..\AutomaticComponentToolkit\_githash_act.txt"

cd ..
pause
